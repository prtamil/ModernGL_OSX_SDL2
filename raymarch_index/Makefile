objects = raymarchindex.o

raymarchindex:  $(objects)
	gcc -std=c99 -o raymarchindex $(objects) -lSDL2  -framework Cocoa      -framework OpenGL -framework IOKit  -lm

raymarchindex.o : raymarchindex.c
	gcc -std=c99 -c raymarchindex.c
clean:
	rm -f raymarchindex raymarchindex.o
