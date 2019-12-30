CC=gcc
CFLAGS= -Wall -Werror -lm -std=gnu99

testrbt: testrbt.o RedBlackTree.o
		$(CC) $(CFLAGS) -o testrbt testrbt.o RedBlackTree.o

testrbt.o: testrbt.c RedBlackTree.h
RedBlackTree.o : RedBlackTree.c RedBlackTree.h

clean:
	rm -f *.o testrbt
	
