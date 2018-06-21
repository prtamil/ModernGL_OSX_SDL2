objects = buffer.o

buffer:  $(objects)
	gcc -std=c99 -o buffer $(objects) -lSDL2  -framework Cocoa      -framework OpenGL -framework IOKit  -lm

buffer.o : buffer.c
	gcc -std=c99 -c buffer.c
clean:
	rm -f buffer buffer.o
