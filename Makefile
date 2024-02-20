build: tema2.c tema2.h
	gcc -g -o quadtree tema2.c tema2.h -Wall

clean:
	rm quadtree