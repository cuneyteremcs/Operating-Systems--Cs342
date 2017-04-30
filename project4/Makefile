all: compile run

compile:
	gcc -Wall -o pagesim pagesim.c -L. -lpthread -lm

run: 
	./pagesim in1 in2 1 out -a 2 -r 0xf0010d00


clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -fr *.o *.a *~
