CC=gcc
CFLAGS=-Wall -Werror -lm -std=c11




all: q1a q1b q1c q2 q3

q1a: 
	$(CC) $(CFLAGS) -o q1a pagerank.c AVL.c Pageset.c hgraph.c Hashing.c mylibrary.c Tfidf.c

q1b: 
	$(CC) $(CFLAGS) -o q1b inverted.c AVL.c Pageset.c hgraph.c Hashing.c mylibrary.c Tfidf.c

q1c: 
	$(CC) $(CFLAGS) -o q1c searchPagerank.c AVL.c Pageset.c hgraph.c Hashing.c mylibrary.c Tfidf.c

q2: 
	$(CC) $(CFLAGS) -o q2 searchTfIdf.c AVL.c Pageset.c hgraph.c Hashing.c mylibrary.c Tfidf.c

q3: 
	$(CC) $(CFLAGS) -o q3 scaledFootrule.c AVL.c Pageset.c hgraph.c Hashing.c mylibrary.c Tfidf.c



clean:
	rm  -f q1a q1b q1c q2 q3 *.o	
