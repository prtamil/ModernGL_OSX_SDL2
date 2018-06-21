objects = simpletri.o

buffer:  $(objects)
	gcc -std=c99 -o simpletri $(objects) -lSDL2 -lGLEW  -lm -framework OpenGL

buffer.o : buffer.c
	gcc -std=c99 -c simpletri.c
clean:
	rm -f simpletri simpletri.o
