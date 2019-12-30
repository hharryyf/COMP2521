all: testRadix ans
testRadix: Queue.c Radix.c testRadix.c Queue.h Radix.h
	gcc -Wall -Werror -lm -std=gnu99 -o testRadix Queue.c Radix.c testRadix.c
ans: standard.c
	gcc -Wall -Werror -lm -std=gnu99 -o ans standard.c
clean:
	rm -f *.o testRadix	standard
