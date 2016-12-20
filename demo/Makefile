include ../include.mk

all: ./main.c
	gcc -g -O2 -o main main.c -L${PREFIX}/lib/wordcheck -I${PREFIX}/include -pthread -lwordcheck -lm `pkg-config --cflags --libs glib-2.0`

clean:
	rm -rf main
