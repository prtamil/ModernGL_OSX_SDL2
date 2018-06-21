#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "linmath.h"

GLuint shader_build_program(const char* vs, const char* fs);
GLuint shader_create(GLenum stype, const char* sfile);
void Draw_X_Y(GLuint vao,GLuint program, int x, int y);

enum PositionState{
	ATZERO,
	XMAX,
	YMAX
}currPosition;

enum DirectionState{
	XDIR,
	YDIR
}currDirection;

int main()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
                        SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Window *win = SDL_CreateWindow("Bouncer", 
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_GLContext contxt = SDL_GL_CreateContext(win);

	if(contxt == NULL){
		printf(" Error Creating OpenGL Context\n");
		return 0;
	}

	const unsigned char *version = glGetString(GL_VERSION);

	SDL_GL_MakeCurrent(win, contxt);

	//Init Glew
	GLenum err = glewInit();

	if(err != GLEW_OK){
		printf("Error while initing GLEW\n");
		return -1;
	}

	const float trivert[] = { //positions
		0.0f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		//colors
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f 
	};

	GLuint shader_program;
	shader_program = shader_build_program("vert.glsl", "frag.glsl");
	if(shader_program == -1){
		SDL_Quit();
		return 0;
	}

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint trivbo;
	glGenBuffers(1, &trivbo);
	glBindBuffer(GL_ARRAY_BUFFER, trivbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trivert), trivert, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	printf("Any error sofar %d\n", glGetError());

	int xdir = 0;
	int ydir = 0;
	currPosition = ATZERO;
	currDirection = XDIR;
	int i = 1;
	int j=1;

	char gameloop_running = 1;
	while(gameloop_running){
		SDL_Event e;
		if(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT)
				gameloop_running = 0;
			else if (e.type == SDL_KEYUP &&
					e.key.keysym.sym == SDLK_ESCAPE)
				gameloop_running = 0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if((i > 200) && (currDirection == XDIR))
			currPosition = XMAX;
		if((i == -200) && (currDirection == XDIR)){
			currPosition = ATZERO;
			currDirection = YDIR;
			j++;
		}

		if((j > 200) && (currDirection == YDIR))
			currPosition = YMAX;
		if((j == -200) && (currDirection == YDIR)){
			currPosition = ATZERO;
			currDirection = XDIR;
		}

		switch(currDirection){
			case XDIR:
				switch(currPosition){
					case ATZERO:
						xdir = 1;
						ydir = 0;
						break;
					case XMAX:
						xdir = -1;
						ydir = 0;
						break;
					default:
						break;
				}
				break;
			case YDIR:
				switch(currPosition){
					case ATZERO:
						ydir = 1;
						xdir = 0;
						break;
					case YMAX:
						ydir = -1;
						xdir = 0;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}

		i = i+ xdir;
		j = j+ ydir;

		Draw_X_Y(vao, shader_program, i, j);
		Draw_X_Y(vao, shader_program, 100+i, 100-j);

		glBindVertexArray(0);
		glUseProgram(0);

		//Swap
		SDL_GL_SwapWindow(win);
	}

	//cleanup
	glDeleteBuffers(1, &trivbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shader_program);

	SDL_GL_DeleteContext(contxt);
	SDL_Quit();

	return 0;

}
void Draw_X_Y(GLuint vao,GLuint program, int x, int y)
{
	GLuint offsetloc = glGetUniformLocation(program, "offset");

	glUseProgram(program);
	glUniform2i(offsetloc, x,y);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}
GLuint shader_build_program(const char *vs, const char *fs)
{
	GLuint verts;
	GLuint frags;

	verts = shader_create(GL_VERTEX_SHADER, vs);
	frags = shader_create(GL_FRAGMENT_SHADER, fs);

	GLuint tmp_prog;
	tmp_prog = glCreateProgram();

	glAttachShader(tmp_prog, verts);
	glAttachShader(tmp_prog, frags);

	glLinkProgram(tmp_prog);

	//Error check
	GLint status;
	glGetProgramiv(tmp_prog, GL_LINK_STATUS, &status);
	if(status == GL_FALSE){
		GLint iloglen;
		glGetProgramiv(tmp_prog, GL_INFO_LOG_LENGTH, &iloglen);

		GLchar sinfolog[4096];
		glGetProgramInfoLog(tmp_prog, iloglen, NULL, sinfolog);
		printf(" Shader Failure : \n %s \n", sinfolog);
		return -1;
	}
	else
		printf("shader linked successfully\n");

	glDetachShader(tmp_prog, verts);
	glDetachShader(tmp_prog, frags);

	return tmp_prog;
}

GLuint shader_create(GLenum stype, const char* sfile)
{
	char shadersrc[4096];
	char inchar;
	FILE *fp;
	int i = 0;

	fp = fopen(sfile, "r");
	while(fscanf(fp,"%c", &inchar) > 0){
		shadersrc[i++] = inchar;
	}
	shadersrc[i-1] = '\0';
	fclose(fp);

	GLuint shader = glCreateShader(stype);
	const char *ss = shadersrc;
	glShaderSource(shader, 1, &ss, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		GLint iloglen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &iloglen);
		GLchar sinfolog[4096];
		glGetShaderInfoLog(shader, iloglen, NULL, sinfolog);

		char shadertype[16];
		switch(stype){
			case GL_VERTEX_SHADER: sprintf(shadertype, "vertex");break;
			case GL_FRAGMENT_SHADER: sprintf(shadertype, "fragment");break;
			case GL_GEOMETRY_SHADER: sprintf(shadertype, "geometry");break;
		}

		printf("Compile failure \n %s shader:\n%s\n", shadertype, sinfolog);
		return -1;
	}
	else
		puts("Shader compilation successfull");

	return shader;
}
