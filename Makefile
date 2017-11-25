FILES=main.c run.c memory.c execute.c compile.c

debug:
	gcc -DDEBUG -lreadline -o tiny-forth-debug $(FILES)

clean:
	rm tiny-forth-debug
	rm tiny-forth

all:
	gcc -lreadline -g3 -o tiny-forth $(FILES)
