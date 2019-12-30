#ifndef MY_LIBRARY
#define MY_LIBRARY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

int should_remove(char c);

char *mystrdup(char *str);

void normalise(char tmp[]);

int match_end_2(char str[]);

int match_start_2(char str[]);

int match_start_1(char str[]);

int match_end_1(char str[]);
#endif
