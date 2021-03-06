#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <SDL2/SDL_opengl.h>

GLuint shader_build_program(const char* vs, const char* fs);
GLuint shader_create(GLenum stype, const char* sfile);

int main()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Window *win = SDL_CreateWindow("First SDL2 GL", 
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_GLContext contxt = SDL_GL_CreateContext(win);

	if(contxt == NULL)
	{
		printf(" Error Creating OpenGL Context\n");
		return 0;
	}

	const unsigned char *version = glGetString(GL_VERSION);
    printf("VER -> %s\n",version);
	if(version == NULL)
	{
		printf("Error Creating GL Context\n");
		return 1;
	}


    //Align Vertex Counter Clockwise
    //
	const float vertices[] = { //positions
        -0.8f, -0.8f,
        0.8f, -0.8f,
        0.8f, 0.8f,
        -0.8f, 0.8f
     };
    const float colors[] = {
        1,0,0,1,
        0,1,0,1,
        0,0,1,1,
        1,1,1,1
    };

    short indices[] = {
        0,1,2,
        0,2,3
    };

	GLuint shader_program;
	shader_program = shader_build_program("vert.glsl", "frag.glsl");
	if(shader_program == -1)
	{
		SDL_Quit();
		return 0;
	}

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vboVert;
	glGenBuffers(1, &vboVert);
	glBindBuffer(GL_ARRAY_BUFFER, vboVert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


    GLuint vboCol;
    glGenBuffers(1, &vboCol);
    glBindBuffer(GL_ARRAY_BUFFER, vboCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint ebo;
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	printf("Any error sofar %d\n", glGetError());

	char gameloop_running = 1;
	while(gameloop_running)
	{
		SDL_Event e;
		if(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
				gameloop_running = 0;
			else if (e.type == SDL_KEYUP &&
					e.key.keysym.sym == SDLK_ESCAPE)
				gameloop_running = 0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//drawing code 
		glUseProgram(shader_program);
		glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		//Swap
		SDL_GL_SwapWindow(win);
	}

	//cleanup
	//glDeleteBuffers(1, &trivbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shader_program);

	SDL_GL_DeleteContext(contxt);
	SDL_Quit();

	return 0;
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
	if(status == GL_FALSE)
	{
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
	while(fscanf(fp,"%c", &inchar) > 0)
	{
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
	if(status == GL_FALSE)
	{
		GLint iloglen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &iloglen);
		GLchar sinfolog[4096];
		glGetShaderInfoLog(shader, iloglen, NULL, sinfolog);

		char shadertype[16];
		switch(stype)
		{
			case GL_VERTEX_SHADER: sprintf(shadertype, "vertex");break;
			case GL_FRAGMENT_SHADER: sprintf(shadertype, "fragment");break;
//			case GL_GEOMETRY_SHADER: sprintf(shadertype, "geometry");break;
		}

		printf("Compile failure \n %s shader:\n%s\n", shadertype, sinfolog);
		return -1;
	}
	else
		puts("Shader compilation successfull");

	return shader;
}
