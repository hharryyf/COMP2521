#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include "mylibrary.h"
// strdup function (dealing with the poor -std=c11)
char *mystrdup(char *str) {
	char *s = NULL;
	assert(str != NULL);
	s = calloc(strlen(str) + 1, sizeof(char));
	strcpy(s, str);
	return s;
}

int should_remove(char c) {
	if (c == '.' || c == '?' || c == ',' || c == ';') {
		return 1;
	}
	return 0;
}
// normalise a word
void normalise(char tmp[]) {
	int i;
	for (i = 0 ; i < strlen(tmp); i++) {
		tmp[i] = tolower(tmp[i]);
	}
	// strlen(tmp) > 1
	if (strlen(tmp) == 0) {
		return;
	}
	if (should_remove(tmp[strlen(tmp) - 1])) {
	    tmp[strlen(tmp) - 1] = '\0';
	}
}
// search the end tag of section2
int match_end_2(char str[]) {
	char tmp[strlen(str) + 1], text[strlen(str) + 1];
	strcpy(text, str);
	char *token;
	char delim[] = " \n\t\r\v\f";  
	int count1 = -4, count2 = -4, count = 0;
	for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
	       strcpy(tmp, token);
	       if (strcmp(tmp, "#end") == 0) {
	       		count1 = count;
	       }
	       if (strcmp(tmp, "Section-2") == 0) {
	       		count2 = count;
	       }
	       count++;
	}  
	if (count2 == 1 && count1 == 0) {
		return 1;
	}
	return 0;
}

int match_start_2(char str[]) {
	char tmp[strlen(str) + 1], text[strlen(str) + 1];
	strcpy(text, str);
	char *token;
	char delim[] = " \n\t\r\v\f";  
	int count1 = -4, count2 = -4, count = 0;
	for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
	       strcpy(tmp, token);
	       if (strcmp(tmp, "#start") == 0) {
	       		count1 = count;
	       }
	       if (strcmp(tmp, "Section-2") == 0) {
	       		count2 = count;
	       }
	       count++;
	}  
	if (count2 == 1 && count1 == 0) {
		return 1;
	}
	return 0;
}

int match_end_1(char str[]) {
	char tmp[strlen(str) + 1], text[strlen(str) + 1];
	strcpy(text, str);
	char *token;
	char delim[] = " \n\t\r\v\f";  
	int count1 = -4, count2 = -4, count = 0;
	for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
	       strcpy(tmp, token);
	       if (strcmp(tmp, "#end") == 0) {
	       		count1 = count;
	       }
	       if (strcmp(tmp, "Section-1") == 0) {
	       		count2 = count;
	       }
	       count++;
	}  
	if (count2 == 1 && count1 == 0) {
		return 1;
	}
	return 0;
}

int match_start_1(char str[]) {
	char tmp[strlen(str) + 1], text[strlen(str) + 1];
	strcpy(text, str);
	char *token;
	char delim[] = " \n\t\r\v\f";  
	int count1 = -4, count2 = -4, count = 0;
	for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
	       strcpy(tmp, token);
	       if (strcmp(tmp, "#start") == 0) {
	       		count1 = count;
	       }
	       if (strcmp(tmp, "Section-1") == 0) {
	       		count2 = count;
	       }
	       count++;
	}  
	if (count2 == 1 && count1 == 0) {
		return 1;
	}
	return 0;
}
