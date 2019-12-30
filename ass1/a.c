
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "textbuffer.h"

#define TRUE 1
#define FALSE 0

void dumpFunctionTest();
void linesFunctionTest();
void addPrefixFunctionTest();
void mergeFunctionTest();
void pasteFunctionTest();
void cutFunctionTest();
void deleteFunctionTest();
void formRichTextTest();

int main(int argc, char *argv[]) {

	dumpFunctionTest();
	printf("Pass dumpTB function tests\n");
	linesFunctionTest();
	printf("Pass linesTB function tests\n");
	addPrefixFunctionTest();
	printf("Pass addPrefixTB function tests\n");
	mergeFunctionTest();
	printf("Pass mergeTB function tests\n");
	pasteFunctionTest();
	printf("Pass pasteTB function tests\n");
	cutFunctionTest();
	printf("Pass cutTB function tests\n");
	deleteFunctionTest();
	printf("Pass deleteTB function tests\n");
	formRichTextTest();
	printf("Pass formRichTextTB function tests\n");
	printf("Pass all black box tests\n");
	return EXIT_SUCCESS;
}
// test output of dumpTB function
void dumpFunctionTest() {

	char str1[] = "";
	char *s1;
	TB tb1 = newTB(str1);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	releaseTB(tb1);

	char str2[] = "\n";
	char *s = strdup(str2);
	char *s2;
	TB tb2 = newTB(str2);
	s2 = dumpTB(tb2, FALSE);
	assert(strcmp(s2, s) == 0);
	free(s);
	free(s2);
	releaseTB(tb2);

	char str3[] = "line 01\n"
				  "line 02\n"
				  "line 03\n"
				  "line 04\n"
				  "line 05\n";
	char str4[] = "1. line 01\n"
				  "2. line 02\n"
				  "3. line 03\n"
				  "4. line 04\n"
				  "5. line 05\n";
	char *s3;
	TB tb3 = newTB(str3);
	s3 = dumpTB(tb3, TRUE);
	assert(strcmp(s3, str4) == 0);
	free(s3);
	releaseTB(tb3);

	char str5[] = "\n\n";
	char str6[] = "1. \n"
				  "2. \n";
	char *s4;
	TB tb4 = newTB(str5);
	s4 = dumpTB(tb4, TRUE);
	assert(strcmp(s4, str6) == 0);
	free(s4);
	releaseTB(tb4);
}
// test output of linesTB function
void linesFunctionTest() {
	char str1[] = "";
	TB tb1 = newTB(str1);
	assert(linesTB(tb1) == 0);
	releaseTB(tb1);

	char str2[] = "\n";
	TB tb2 = newTB(str2);
	assert(linesTB(tb2) == 1);
	releaseTB(tb2);

	char str3[] = "line 01\n"
				  "line 02\n"
				  "line 03\n"
				  "line 04\n"
				  "line 05\n";
	TB tb3 = newTB(str3);
	assert(linesTB(tb3) == 5);
	releaseTB(tb3);

	char str4[] = "\n\n";
	TB tb4 = newTB(str4);
	assert(linesTB(tb4) == 2);
	releaseTB(tb4);
}
// test output of addPrefix Function
void addPrefixFunctionTest() {
	char str1[] = "line 01\n";
	char *prefix = "";
	char *s;
	TB tb1 = newTB(str1);
	addPrefixTB(tb1, 0, 0, prefix);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, "line 01\n") == 0);
	free(s);
	releaseTB(tb1);

	char str2[] = "line 01\n"
				  "line 02\n"
				  "line 03\n"
				  "line 04\n"
				  "line 05\n";
	char str3[] = "prefix line 01\n"
				  "prefix line 02\n"
				  "prefix line 03\n"
				  "line 04\n"
				  "line 05\n";
	prefix = "prefix ";
	TB tb2 = newTB(str2);
	addPrefixTB(tb2, 0, 2, prefix);
	s = dumpTB(tb2, FALSE);
	assert(strcmp(s, str3) == 0);
	free(s);
	releaseTB(tb2);

	char str4[] = "\n\n";
	char str5[] = "1. prefix \n2. prefix \n";
	prefix = "prefix ";
	TB tb3 = newTB(str4);
	addPrefixTB(tb3, 0, 1, prefix);
	s = dumpTB(tb3, TRUE);
	assert(strcmp(s, str5) == 0);
	free(s);
	releaseTB(tb3);
}
// test merge function
void mergeFunctionTest() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n";

	char str2[] = "merge line 01\n"
				  "merge line 02\n";

	char str3[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
				  "merge line 01\n"
				  "merge line 02\n";
	char *s;
	TB tb1 = newTB(str1);
	TB tb2 = newTB(str2);
	mergeTB(tb1, 5, tb2);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, str3) == 0);
	free(s);
	releaseTB(tb1);
	// tb1 is empty
	char str4[] = "merge line 01\n"
				  "merge line 02\n";
	char str5[] = "";
	tb1 = newTB(str5);
	tb2 = newTB(str4);
	mergeTB(tb1, 0, tb2);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, "merge line 01\nmerge line 02\n") == 0);
	free(s);
	releaseTB(tb1);
	//merge middle
	char str6[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n";

	char str7[] = "merge line 01\n"
				  "merge line 02\n";

	char str8[] = "line 01\n"
				  "merge line 01\n"
				  "merge line 02\n"
				  "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n";
	tb1 = newTB(str6);
	tb2 = newTB(str7);
	mergeTB(tb1, 1, tb2);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, str8) == 0);
	free(s);
	releaseTB(tb1);
}
// test paste function
void pasteFunctionTest() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n";

	char str2[] = "paste line 01\n"
				  "paste line 02\n";

	char str3[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
				  "paste line 01\n"
				  "paste line 02\n";
	char *s;
	TB tb1 = newTB(str1);
	TB tb2 = newTB(str2);
	pasteTB(tb1, 5, tb2);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, str3) == 0);
	free(s);
	releaseTB(tb1);
	releaseTB(tb2);
	// tb1 is empty
	char str4[] = "paste line 01\n"
				  "paste line 02\n";
	char str5[] = "";
	tb1 = newTB(str5);
	tb2 = newTB(str4);
	pasteTB(tb1, 0, tb2);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, "paste line 01\npaste line 02\n") == 0);
	free(s);
	releaseTB(tb1);
	releaseTB(tb2);
	//paste middle
	char str6[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n";

	char str7[] = "paste line 01\n"
				  "paste line 02\n";

	char str8[] = "line 01\n"
				  "paste line 01\n"
				  "paste line 02\n"
				  "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n";
	tb1 = newTB(str6);
	tb2 = newTB(str7);
	pasteTB(tb1, 1, tb2);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, str8) == 0);
	free(s);
	releaseTB(tb1);
	releaseTB(tb2);
}
// test cut function
void cutFunctionTest() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n";

	char str2[] = "line 01\n"
				  "line 02\n";

	char str3[] = "line 03\n"
		          "line 04\n"
		          "line 05\n";
	char *s1;
	char *s2;
	TB tb1 = newTB(str1);
	TB tb2 = cutTB(tb1, 0, 1);
	s1 = dumpTB(tb1, FALSE);
	s2 = dumpTB(tb2, FALSE);
	assert(strcmp(s1, str3) == 0);
	assert(strcmp(s2, str2) == 0);
	free(s1);
	free(s2);
	releaseTB(tb1);
	releaseTB(tb2);

	char str4[] = "line 01\n"
		     	  "line 02\n"
		     	  "line 03\n"
		     	  "line 04\n"
		     	  "line 05\n";

	char str5[] = "line 02\n"
		     	  "line 03\n";

	char str6[] = "line 01\n"
			 	  "line 04\n"
		     	  "line 05\n";
	tb1 = newTB(str4);
	tb2 = cutTB(tb1, 1, 2);
	s1 = dumpTB(tb1, FALSE);
	s2 = dumpTB(tb2, FALSE);
	assert(strcmp(s1, str6) == 0);
	assert(strcmp(s2, str5) == 0);
	free(s1);
	free(s2);
	releaseTB(tb1);
	releaseTB(tb2);
}

void deleteFunctionTest() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n";

	char str2[] = "line 03\n"
		          "line 04\n"
		          "line 05\n";
	char *s;
	TB tb1 = newTB(str1);
	deleteTB(tb1, 0, 1);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, str2) == 0);
	free(s);
	releaseTB(tb1);

	char str3[] = "line 01\n"
		     	  "line 02\n"
		     	  "line 03\n"
		     	  "line 04\n"
		     	  "line 05\n";

	char str4[] = "line 01\n"
			 	  "line 04\n"
		     	  "line 05\n";
	tb1 = newTB(str3);
	deleteTB(tb1, 1, 2);
	s = dumpTB(tb1, FALSE);
	assert(strcmp(s, str4) == 0);
	free(s);
	releaseTB(tb1);
}

void formRichTextTest() {
	char str1[] = "#\n"
				  "***\n"
				  "*line_01_*\n"
				  "*line_02_\n"
				  "*line*_03_\n"
				  "#line_04_\n";

	char str2[] = "#\n"
				  "***\n"
				  "<b>line_01_</b>\n"
				  "*line<i>02</i>\n"
				  "<b>line</b><i>03</i>\n"
				  "<h1>line_04_</h1>\n";
	TB tb1 = newTB(str1);
	formRichText(tb1);
	char *s = dumpTB(tb1, FALSE);
	assert(strcmp(s,str2) == 0);
	free(s);
	releaseTB(tb1);
}

