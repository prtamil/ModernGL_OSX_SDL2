objects = raymarch.o

raymarch:  $(objects)
	gcc -std=c99 -o raymarch $(objects) -lSDL2  -framework Cocoa      -framework OpenGL -framework IOKit  -lm

raymarch.o : raymarch.c
	gcc -std=c99 -c raymarch.c
clean:
	rm -f raymarch raymarch.o
