objects = simplematrix.o

simplematrix:  $(objects)
	gcc -ggdb -std=c99 -o simplematrix $(objects) -lSDL2 -framework Cocoa -framework OpenGL -framework IOKit  -lGLEW -lm

simplematrix.o : simplematrix.c
	gcc -ggdb -std=c99 -c simplematrix.c
clean:
	rm -f simplematrix simplematrix.o
