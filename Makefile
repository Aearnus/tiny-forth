FILES=main.c run.c memory.c execute.c

debug:
	gcc -DDEBUG -lreadline -o tiny-forth-debug $(FILES)

clean:
	rm tiny-forth-debug
	rm tiny-forth

all:
	gcc -lreadline -g -o tiny-forth $(FILES)
