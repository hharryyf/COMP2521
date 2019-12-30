#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include "mylibrary.h"
#include "AVL.h"
#define MAX_LEN 1004

int main() {
	AVL t = NULL;  // using the AVL data structure
	FILE *fp, *fp1, *fp2;  // fps, collection/url/invert
	char file[MAX_LEN];
	char line[MAX_LEN];
	char text[MAX_LEN];
	char tmp[MAX_LEN];
    char *token;
	// chdir("ex3");
	// int i;
	int find = 0;
	char delim[] = " \n\t\r\v\f";  
	fp = fopen("collection.txt", "r");
	if (fp == NULL) {
		perror("fopen");
		return 1;
	}
	// read from the collection
	while (fscanf(fp, "%s", line) != EOF) {
		strcpy(file, line);
		
		strcat(file, ".txt");
		fp1 = fopen(file, "r");
		if (fp1 == NULL) {
			continue;
		}
		find = 0;
		// fscanf read the body
		/*while (fscanf(fp1, "%s", text) != EOF) {
			
			if (strcmp(text, "#end") == 0 && find == 1) {
				break;
			}
			
			if (find == 1) {
				strcpy(tmp, text);
				normalise(tmp);
	        	t = insertAVL(t, tmp, line);
			}
			
			if (strcmp(text, "Section-2") == 0) {
				find = 1;
			}
		}*/
		
		while (fgets(text, MAX_LEN, fp1) != NULL) {
			// match the end section2 tag we break
			if (match_end_2(text)) {
				break;
			}
			// printf("%s\n", text);
			if (find == 1) {
				for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
	        		strcpy(tmp, token);
	        		normalise(tmp);
	        		if (tmp[0] != '\0')
	        			t = insertAVL(t, tmp, line);
	        	}  
			}
			// match the start section2 tag we start reading
			if (match_start_2(text)) {
				find = 1;
			}
		}
		fclose(fp1);
	}
	
	fp2 = fopen("invertedIndex.txt", "w+");
	inorder(t, fp2);
   // showAVL(t, 0);
    destroyAVL(t);
    fclose(fp2);
    fclose(fp);
	return 0;
}
