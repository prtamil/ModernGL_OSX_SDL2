objects = matrix.o

matrix:  $(objects)
	gcc -ggdb -std=c99 -o matrix $(objects) -lSDL2 -framework Cocoa -framework OpenGL -framework IOKit -lGLEW -lm

matrix.o : matrix.c
	gcc -ggdb -std=c99 -c matrix.c
clean:
	rm -f matrix matrix.o
