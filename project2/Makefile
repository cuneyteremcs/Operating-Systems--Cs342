all: run test

run:	
	gcc callcount.c -o callcount -lpthread

test: test1

test1:
	./callcount 2 20 input1 input2 output 1900 1999 10000000 99999999