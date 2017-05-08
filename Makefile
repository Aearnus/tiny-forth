debug:
	gcc -DDEBUG -lreadline -o tiny-forth-debug main.c

clean:
	rm tiny-forth-debug
	rm tiny-forth

all:
	gcc -lreadline -g -o tiny-forth main.c
