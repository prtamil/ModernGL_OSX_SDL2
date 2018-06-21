objects = bouncer.o

bouncer:  $(objects)
	gcc -ggdb -std=c99 -o bouncer $(objects) -lSDL2  -lGLEW -framework Cocoa  -framework OpenGL -framework IOKit  -lm

bouncer.o : bouncer.c
	gcc -ggdb -std=c99 -c bouncer.c
clean:
	rm -f bouncer bouncer.o
