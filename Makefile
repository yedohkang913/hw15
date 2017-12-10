all: control.c writer.c
	gcc -o control control.c 
	gcc -o main writer.c

clean: 
	rm a.out
	rm *~
	rm story.txt