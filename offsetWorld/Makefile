objects = offsetworld.o

offsetworld:  $(objects)
	gcc -ggdb -std=c99 -o offsetworld $(objects) -lSDL2 -framework Cocoa -framework OpenGL -framework IOKit  -lGLEW -lm

offsetworld.o : offsetworld.c
	gcc -ggdb -std=c99 -c offsetworld.c
clean:
	rm -f offsetworld offsetworld.o
