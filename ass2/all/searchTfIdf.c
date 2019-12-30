#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include "mylibrary.h"
#include "Hashing.h"
#include "AVL.h"
#include "Tfidf.h"

#define MAX_LEN 1004

int main(int argc, char *argv[]) {
	FILE *fp, *fp1;
	AVL t = NULL;
	Hash h = NULL;
	char file[MAX_LEN];
	char text[MAX_LEN];
	char tmp[MAX_LEN];
	char line[MAX_LEN];
	int count = 0;
	int total_url = 0;
	char *token;
	int i;
    int j;
    int all = 0;
    char delim[] = " \n\t\v\r\f";  
	Page *page = NULL;
	
	for (i = 1; i < argc; i++) {
		fp = fopen("invertedIndex.txt", "r");
		if (fp == NULL) {
			perror("fopen");
			return 1;
		}
		while (fgets(text, MAX_LEN, fp) != NULL) {
			j = 0;
			for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
				 if (j == 0 && strcmp(token, argv[i]) != 0) {
				   	break;
				 }
				 
				 if (j > 0) {
				 	total_url++;
				 }
				 j++;
			}
			if (j != 0) {
				break;
			}  
		}
		fclose(fp);
	}
	// get the urlfiles that should be "interested in"
	h = hash_init(total_url < 1003 ? total_url : 1003);
	total_url = 0;
	
	for (i = 1; i < argc; i++) {
		fp = fopen("invertedIndex.txt", "r");
		if (fp == NULL) {
			perror("fopen");
			return 1;
		}
		
		while (fgets(text, MAX_LEN, fp) != NULL) {
			j = 0;
			for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
				 if (j == 0 && strcmp(token, argv[i]) != 0) {
				   	break;
				 }
				 
				 if (j > 0 && hash_search(h, token) == -1) {
				 	hash_insert(h, hashkey(token, h), total_url, token);
				 	total_url++;
				 }
				 j++;
			}
			
			if (j != 0) {
				break;
			}  
		}
		fclose(fp);
	}
	
	int find = FALSE;
	page = calloc(total_url + 1, sizeof(Page));
	assert(page != NULL);
	i = 0;
	j = 0;
	fp = fopen("collection.txt", "r");
	
	if (fp == NULL) {
		perror("fopen");
		hash_destroy(h);
		destroy_page(page, total_url);
		destroyAVL(t);
		exit(1);
	}
	
	while (fscanf(fp, "%s", line) != EOF) {
		all++;
		if (hash_search(h, line) == -1) {
			continue;
		}
		strcpy(file, line);
		strcat(file, ".txt");
		fp1 = fopen(file, "r");
		if (fp1 == NULL) {
			continue;
		}
		find = 0;
		count = 0;/*
		while (fscanf(fp1, "%s", text) != EOF) {
			
			if (strcmp(text, "#end") == 0 && find == 1) {
				break;
			}
			
			if (find == 1) {
				strcpy(tmp, text);
	        	count++;
	        	normalise(tmp);
	        	t = insertAVL(t, tmp, line);  
			}
			
			if (strcmp(text, "Section-2") == 0) {
				find = 1;
			}
		}*/
		
		while (fgets(text, MAX_LEN, fp1) != NULL) {
			
			if (match_end_2(text)) {
				break;
			}
			
			if (find == TRUE) {
				for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
	        		strcpy(tmp, token);
	        		normalise(tmp);
	        		if (tmp[0] != '\0') {
	        			count++;
	        			t = insertAVL(t, tmp, line);
	        		}
	        	}  
			}
			
			if (match_start_2(text)) {
				find = TRUE;
			}
		}
		
		fclose(fp1);
		
		page[j].url = mystrdup(line);
		page[j].total_word = count;
		page[j].tfidf = 0.0;
		page[j].match = 0;
		// printf("%d\n", count);
		j++;
	}
	
	hash_destroy(h);
	
	cal_tfidf(page, total_url, t, argv, argc, all);
	
	destroy_page(page, total_url);
	// showAVL(t, 0);
	destroyAVL(t);
	fclose(fp);
	return 0;
}


