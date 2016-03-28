
all: C.c H.c main.c CH4.h
	gcc -Wno-cpp -std=c99 -o C C.c
	gcc -Wno-cpp -std=c99 -o H H.c
	gcc -Wno-cpp -std=c99 -g -o main main.c
	chmod 755 main

clean:
	-rm C H main

