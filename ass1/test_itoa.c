#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

char *newstr(int a, char *s1);

int main() {
	char *s1 = "hello";
	char *buf = newstr(12, s1);  
	printf("%s\n", buf);
	printf("%d\n", strlen(buf));
	return 0;
}

char *newstr(int a, char *s1) {
	char *s = calloc(strlen(s1) + 9,sizeof(char));
	sprintf(s, "%d", a);
	s = strcat(s, ". ");
	s = strcat(s, s1);
//	free(p);
	return s;
}
