#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
void f(char *s) ;
int main() {
	char *s = strdup("hello");
	printf("%p",s);
//	f(s);
	s = (char *) realloc(s, sizeof(char *) * 9);
	printf("%p",s);
	s[0] = 'b';
	printf("%s\n", s);
	return 0;
}

void f(char *s) {
	
	s[0] = 'b';
	s[1] = 'q';
}

