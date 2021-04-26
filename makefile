main: main.c
	gcc -o a.out main.c
test: main.c
	gcc -o a.out -Wall -ggdb3 main.c
test1: test
	./script
test2: test
	./script2
.PHONY: clear
clear:
	-rm -f *.o *.out
