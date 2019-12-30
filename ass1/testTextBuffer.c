#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "textbuffer.h"

#define TRUE 1
#define FALSE 0

void blackBoxTests();
void blackbox_richText_test();
void blackbox_search_test();
void blackbox_new_dump_test();
void blackbox_line_test();
void blackbox_prefix_test();
void general_cut_delete_merge_paste_undo_redo_tests_1();
void general_cut_delete_merge_paste_undo_redo_tests_2();
void diff_tests();
void special_empty_tb_tests();
void destroy_list(Match l);
void general_cut_delete_merge_paste_tests();
void blackbox_delete_test();
void blackbox_merge_test();
void blackbox_paste_test();
void blackbox_cut_test();
void blackbox_redo_undo_test();

int main(int argc, char *argv[]) {
    // whiteBoxTests();
    blackBoxTests();
    return EXIT_SUCCESS;
}



void blackBoxTests() {
	 blackbox_richText_test();
//	 printf("pass the black_box form richText test\n");
	 blackbox_prefix_test();
//	 printf("pass the black_box prefix test\n");
	 blackbox_new_dump_test();
//	 printf("pass the black_box_new_dump test\n");
	 blackbox_search_test();
//	 printf("pass the black_box searching test\n");
	 blackbox_line_test();
//	 printf("pass the counting line test\n");
	 blackbox_delete_test();
//	 printf("pass the blackbox_delete_test\n");
	 blackbox_merge_test();
//	 printf("pass the blackbox_merge_test\n");
	 blackbox_paste_test();
//	 printf("pass the blackbox_paste_test\n");
	 blackbox_cut_test();
//	 printf("pass the blackbox_cut_test\n");
//	 blackbox_redo_undo_test();
//	 printf("pass the redo and undo test\n");
	 general_cut_delete_merge_paste_tests();
//	 printf("pass the general operation test group 0\n");
//	 general_cut_delete_merge_paste_undo_redo_tests_1();
//	 printf("pass the general operation test group 1\n");
//	 general_cut_delete_merge_paste_undo_redo_tests_2();
//	 printf("pass the general operation test group 2\n");
//	 special_empty_tb_tests();
//	 printf("pass the special empty tb tests\n");
//	 diff_tests();
	 
}

void blackbox_redo_undo_test() {
	char str1[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n";
	char str2[] = "1\n2\n3\n4\n5\n6\n7\n8\n";
	char str_ans1[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n"
					  "1\n2\n3\n4\n5\n6\n7\n8\n";
	char str_ans2[] = "2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n";
	char str_ans3[] = "4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n";
	char str_ans4[] = "14\n15\n";
	char str_ans5[] = "13\n14\n15\n";
	char *s1;
	TB tb1 = newTB(str1), tb2 = newTB(str2);
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE); // undo has no effect since it is nondestructive
	assert(strcmp(s1, str1) == 0);
	free(s1);
	redoTB(tb1);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	pasteTB(tb1, 15, tb2);
	undoTB(tb1);  // nothing done to tb1
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	redoTB(tb1);  // redo tb1
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str_ans1) == 0);
	free(s1);
	undoTB(tb1); // recover to original
	deleteTB(tb1, 0, 0);
	redoTB(tb1);  // check whether the redo stack is dead
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str_ans2) == 0);
	free(s1);
	addPrefixTB(tb1, 0, 12, ""); // call a unrecoverable operation
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);  // check whether the undo stack is dead
	assert(strcmp(s1, str_ans2) == 0);
	free(s1);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	deleteTB(tb1, 0, 0);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1);
	undoTB(tb1); // exceed the 10 time record 
	s1 = dumpTB(tb1, FALSE);  // check whether the undo stack is 10 size
	assert(strcmp(s1, str_ans3) == 0);
	free(s1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	s1 = dumpTB(tb1, FALSE);  // check whether the redo stack is 10 size
	assert(strcmp(s1, str_ans4) == 0);
	free(s1);
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);  // check whether the redo stack is 10 size
	assert(strcmp(s1, str_ans5) == 0);
	free(s1);
	addPrefixTB(tb1, 0, 1, ""); // check the undo stack is dead
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);  
	assert(strcmp(s1, str_ans5) == 0);
	free(s1);
	mergeTB(tb1, 0, tb2);
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);  // check whether the merge operation is fine
	assert(strcmp(s1, str_ans5) == 0);
	free(s1);
	tb2 = cutTB(tb1, 0, 1);
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);  // check whether the cut operation is fine
	assert(strcmp(s1, str_ans5) == 0);
	free(s1);
	releaseTB(tb1);
	releaseTB(tb2);
}

void blackbox_cut_test() {
	char str1[] =        "merge_line 1\n"
				  		 "merge_line 2\n"
				  		 "\n"
						 "line 01\n"
		          		 "line 02\n"
						  "line 03\n"
						  "line 04\n"
						  "line 05\n"
						  "line 06\n"
						  "merge_line 1\n"
				  		 "merge_line 1\n"
				  		 "merge_line 2\n"
				  		 "\n"
				  		 "merge_line 2\n"
				  		 "\n";
	char cut_front_1[] = 
						 "1. line 01\n"
		          		 "2. line 02\n"
						  "3. line 03\n"
						  "4. line 04\n"
						  "5. line 05\n"
						  "6. line 06\n"
						  "7. merge_line 1\n"
				  		 	"8. merge_line 1\n"
				  		 "9. merge_line 2\n"
				  		 "10. \n"
				  		 "11. merge_line 2\n"
				  		 "12. \n";
	char cut_front_2[] = "1. merge_line 1\n"
				  		 "2. merge_line 2\n"
				  		 "3. \n";
	char cut_rear_1[] = 
						 "1. line 01\n"
		          		 "2. line 02\n"
						  "3. line 03\n"
						  "4. line 04\n"
						  "5. line 05\n"
						  "6. line 06\n"
						  "7. merge_line 1\n"
				  		 	"8. merge_line 1\n"
				  		 "9. merge_line 2\n";
				  		 
	char cut_rear_2[] = "1. \n"
				  		 "2. merge_line 2\n"
				  		 "3. \n";
	char cut_middle_1[] = "1. line 01\n"
						  "2. merge_line 1\n"
				  		 	"3. merge_line 1\n"
				  		 "4. merge_line 2\n";
	char cut_middle_2[] = 
		          		 "1. line 02\n"
						  "2. line 03\n"
						  "3. line 04\n"
						  "4. line 05\n"
						  "5. line 06\n";
						  
	TB tb1 = newTB(str1);
	TB tb2;
	char *s1, *s2;
	tb2 = cutTB(tb1, 0, 2);
	s1 = dumpTB(tb1, TRUE);  // cut front case
	s2 = dumpTB(tb2, TRUE);
	assert(strcmp(s1, cut_front_1) == 0);
	assert(strcmp(s2, cut_front_2) == 0);
	assert(linesTB(tb1) == 12); // 15 - 3 = 12
	assert(linesTB(tb2) == 3);
	free(s1);
	free(s2);
	releaseTB(tb2);  
	// cut rear case
	tb2 = cutTB(tb1, 9, 11);
	s1 = dumpTB(tb1, TRUE);
	s2 = dumpTB(tb2, TRUE);
	assert(strcmp(s1, cut_rear_1) == 0);
	assert(strcmp(s2, cut_rear_2) == 0);
	assert(linesTB(tb1) == 9);  // 12 - 3 = 9
	assert(linesTB(tb2) == 3);
	free(s1);
	free(s2);
	releaseTB(tb2);
	tb2 = cutTB(tb1, 1, 5);
	s1 = dumpTB(tb1, TRUE);
	s2 = dumpTB(tb2, TRUE);
	assert(strcmp(s1, cut_middle_1) == 0);
	assert(strcmp(s2, cut_middle_2) == 0);
	assert(linesTB(tb1) == 4);  /// 9 - 4 = 5
	assert(linesTB(tb2) == 5);
	free(s1);
	free(s2);
	releaseTB(tb2);
	tb2 = cutTB(tb1, 1, 0); // to < from then it is NULL
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, cut_middle_1) == 0);
	assert(tb2 == NULL);
	assert(linesTB(tb1) == 4);  /// 9 - 4 = 5
	// assert(linesTB(tb2) == 5);
	free(s1);
	// free(s2);
	releaseTB(tb2);
	releaseTB(tb1);
}

// this test the function of deleteTB
// and every deletion we check the line number since if it fails
// the deletion is incomplete
void blackbox_delete_test() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	char delete_0[] = "1. line 02\n"
		              "2. line 03\n"
		              "3. line 04\n"
		              "4. line 05\n"
		              "5. line 06\n";
	char delete_last[] = "1. line 02\n"
		                 "2. line 03\n"
		                 "3. line 04\n"
		                 "4. line 05\n";
	char delete_middle[] = "1. line 02\n"
		                 "2. line 03\n"
		                 "3. line 05\n";
	char delete_back[] = 
		                 "1. line 02\n";
	char delete_one_line[] = "1. 1. line 02\n"
		                    "2. 3. line 05\n";
	char *s1;
	TB tb1 = newTB(str1);
	deleteTB(tb1, 0, 0);
	s1 = dumpTB(tb1, TRUE); // delete front case
	assert(strcmp(s1, delete_0) == 0);
	assert(linesTB(tb1) == 5);
	free(s1);
	deleteTB(tb1, 4, 4);  // delete last case
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, delete_last) == 0);
	assert(linesTB(tb1) == 4);
	free(s1);
	deleteTB(tb1, 2, 2); // delete a middle node
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, delete_middle) == 0);
	assert(linesTB(tb1) == 3);
	free(s1);
	deleteTB(tb1, 1, 2); // delete include a front line
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, delete_back) == 0);
	assert(linesTB(tb1) == 1);
	free(s1);
	deleteTB(tb1, 0, 0); // delete to empty
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, "") == 0);
	assert(linesTB(tb1) == 0);
	free(s1);
	releaseTB(tb1);
	tb1 = newTB(delete_middle);
	deleteTB(tb1, 1, 1);
	s1 = dumpTB(tb1, TRUE);
	// printf("%s\n", s1);
	assert(strcmp(s1, delete_one_line) == 0);
	assert(linesTB(tb1) == 2);
	free(s1);
	releaseTB(tb1);
}

// this checks the pasteTB function
// including paste front rear centre cases
// and after each test we need to check the lines in tb2 and tb1
// if it fails means that something wrong with the connection
void blackbox_paste_test() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	
	char str2[] = "merge_line 1\n"
				  "merge_line 2\n""\n";
	char paste_front[] = "1. merge_line 1\n"
				  		 "2. merge_line 2\n"
				  		 "3. \n"
						 "4. line 01\n"
		          		 "5. line 02\n"
						  "6. line 03\n"
						  "7. line 04\n"
						  "8. line 05\n"
						  "9. line 06\n";
	char paste_rear[] = "1. merge_line 1\n"
				  		 "2. merge_line 2\n"
				  		 "3. \n"
						 "4. line 01\n"
		          		 "5. line 02\n"
						  "6. line 03\n"
						  "7. line 04\n"
						  "8. line 05\n"
						  "9. line 06\n"
						  "10. merge_line 1\n"
				  		 "11. merge_line 2\n"
				  		 "12. \n";
	char paste_middle[] = "1. merge_line 1\n"
				  		 "2. merge_line 2\n"
				  		 "3. \n"
						 "4. line 01\n"
		          		 "5. line 02\n"
						  "6. line 03\n"
						  "7. line 04\n"
						  "8. line 05\n"
						  "9. line 06\n"
						  "10. merge_line 1\n"
				  		 	"11. merge_line 1\n"
				  		 "12. merge_line 2\n"
				  		 "13. \n"
				  		 "14. merge_line 2\n"
				  		 "15. \n";
	char *s1;
	TB tb1 = newTB(str1), tb2 = newTB(str2);
	pasteTB(tb1, 0, tb2); // paste front case
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, paste_front) == 0);
	assert(linesTB(tb1) == 9);
	assert(linesTB(tb2) == 3);
	pasteTB(tb1, 9, tb2); // paste rear case
	free(s1);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, paste_rear) == 0);
	assert(linesTB(tb1) == 12);
	assert(linesTB(tb2) == 3);
	pasteTB(tb1, 10, tb2); // paste middle case
	free(s1);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, paste_middle) == 0);
	assert(linesTB(tb1) == 15);
	assert(linesTB(tb2) == 3);
	free(s1);
	releaseTB(tb1);
	releaseTB(tb2);
}

// this test the function of mergeTB
// and every merge we check the line number since if it fails
// the merge is incomplete
void blackbox_merge_test() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	
	char str2[] = "merge_line 1\n"
				  "merge_line 2\n""\n";
	char merge_front[] = "1. merge_line 1\n"
				  		 "2. merge_line 2\n"
				  		 "3. \n"
						 "4. line 01\n"
		          		 "5. line 02\n"
						  "6. line 03\n"
						  "7. line 04\n"
						  "8. line 05\n"
						  "9. line 06\n";
	char merge_rear[] = "1. merge_line 1\n"
				  		 "2. merge_line 2\n"
				  		 "3. \n"
						 "4. line 01\n"
		          		 "5. line 02\n"
						  "6. line 03\n"
						  "7. line 04\n"
						  "8. line 05\n"
						  "9. line 06\n"
						  "10. merge_line 1\n"
				  		 "11. merge_line 2\n"
				  		 "12. \n";
	char merge_middle[] = "1. merge_line 1\n"
				  		 "2. merge_line 2\n"
				  		 "3. \n"
						 "4. line 01\n"
		          		 "5. line 02\n"
						  "6. line 03\n"
						  "7. line 04\n"
						  "8. line 05\n"
						  "9. line 06\n"
						  "10. merge_line 1\n"
				  		 	"11. merge_line 1\n"
				  		 "12. merge_line 2\n"
				  		 "13. \n"
				  		 "14. merge_line 2\n"
				  		 "15. \n";
	char *s1;
	TB tb1 = newTB(str1), tb2 = newTB(str2);
	TB tb3 = newTB(str2);
	TB tb4 = newTB(str2);
	mergeTB(tb1, 0, tb2); // merge front case
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, merge_front) == 0);
	assert(linesTB(tb1) == 9);
	mergeTB(tb1, 9, tb3); // merge rear case
	free(s1);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, merge_rear) == 0);
	assert(linesTB(tb1) == 12);
	mergeTB(tb1, 10, tb4); // merge middle case
	free(s1);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, merge_middle) == 0);
	assert(linesTB(tb1) == 15);
	free(s1);
	releaseTB(tb1);
}


// this is the general or mixed up operation test without redo undo
void general_cut_delete_merge_paste_tests() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	char str2[] = "";
	char str3[] = "line 06\n"
				  "line 07\n"
				  "line 08\n";
	char *s1;
	TB tb1 = newTB(str1), tb2 = newTB(str2), tb3 = newTB(str3);
	mergeTB(tb2, 0, tb1);
	s1 = dumpTB(tb2, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	deleteTB(tb3, 0, 0);
	s1 = dumpTB(tb3, TRUE);
	assert(strcmp(s1, "1. line 07\n2. line 08\n") == 0); // delete last line
	assert(linesTB(tb3) == 2); // check the update of the lines
	free(s1);
	deleteTB(tb3, 1, 1);
	s1 = dumpTB(tb3, TRUE);
	assert(strcmp(s1, "1. line 07\n") == 0); // delete last line
	assert(linesTB(tb3) == 1);  // check the update of the lines
	free(s1);
	deleteTB(tb3, 0, 0);
	s1 = dumpTB(tb3, TRUE);
	assert(strcmp(s1, "") == 0); // delete only line
	assert(linesTB(tb3) == 0);  // check the update of the lines
	free(s1);
	tb1 = cutTB(tb2, 1, 2);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, "1. line 02\n2. line 03\n") == 0); // delete only line
	assert(linesTB(tb1) == 2);  // check the update of the lines
	free(s1);
	s1 = dumpTB(tb2, TRUE);
	assert(strcmp(s1, 
	"1. line 01\n2. line 04\n3. line 05\n4. line 06\n") == 0); //delete to empty
	
	assert(linesTB(tb2) == 4);  // check the update of the lines
	free(s1);
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);	
}
// this is the special test for test cases involves empty buffer also redo and undo
void special_empty_tb_tests() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	 char str2[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
     char str3[] = "hello--line 01\n"
		          "hello--line 02\n"
		          "hello--line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	TB tb1, tb2;
	char *s1;
	tb1 = newTB(str1);
	tb2 = newTB(str2);
	deleteTB(tb2, 0, 5);
	deleteTB(tb1, 0, 5);
	mergeTB(tb1, 0, tb2);
	undoTB(tb1);
	s1 = dumpTB(tb1, TRUE); // we have a merge from empty actinon
	// printf("%s\n", s1);
	assert(strcmp(s1, "") == 0);
	free(s1);
	undoTB(tb1);// make sure we have an empty record
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	// printf("%s\n", s1);
	free(s1);
	redoTB(tb1);// make sure we have an empty record
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, "") == 0);
	// printf("%s\n", s1);
	free(s1);
	redoTB(tb1);// make sure we have an empty record
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, "") == 0);
	// printf("%s\n", s1);
	free(s1);
	redoTB(tb1);// already empty redo stack case
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, "") == 0);
	// printf("%s\n", s1);
	free(s1);
	redoTB(tb1);// already empty redo stack case
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, "") == 0);
	// printf("%s\n", s1);
	free(s1);
	undoTB(tb1);// already empty redo stack case
	s1 = dumpTB(tb1, FALSE);
    assert(strcmp(s1, "") == 0);
    // printf("%s\n", s1);
	free(s1);
	undoTB(tb1);// undo again
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	// printf("%s\n", s1);
	free(s1);
	redoTB(tb1);
	redoTB(tb1);
	redoTB(tb1);
	tb2 = newTB(str2);
	pasteTB(tb1, 0, tb2);
	redoTB(tb1);  // nothing happened
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	// printf("%s\n", s1);
	free(s1);
	undoTB(tb2);
	s1 = dumpTB(tb2, FALSE);
	assert(strcmp(s1, str1) == 0);
	// printf("%s\n", s1);
	free(s1);
	addPrefixTB(tb1, 0, 2, "hello--");
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str3) == 0);
	// printf("%s\n", s1);
	free(s1);
	releaseTB(tb1);
	releaseTB(tb2);
	/// the most special case cut from > to and try to recover
	tb1 = newTB(str1);
	deleteTB(tb1, 1, 2);
	tb2 = cutTB(tb1, 1, 0);  // valid but is NULL
	assert(tb2 == NULL);
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) != 0);
	free(s1);
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	releaseTB(tb1);
	releaseTB(tb2);
}
// this is the diff test
void diff_tests() {
	 char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	 char str2[] = "";
	 char str3[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	 char str4[] = "line 03\n"
		          "line 01\n"
		          "line 02\n"
		          "line 00\n"
		          "line 06\n"
		          "line 05\n"
		          "line 09\n";
	 char str5[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n";
	 char str6[] = "1\n12\n2\n11\n3\n10\n4\n9\n5\n8\n6\n7\n";
	 char str7[] = "1\n2\n3\n4\n5\n";
	 char str8[] = "6\n7\n8\n9\n10\n";
	 char *s1;
	 TB tb1 = newTB(str1), tb2 = newTB(str2);
	 s1 = diffTB(tb2, tb1);
	 printf("%s", s1);
	 free(s1);
	 releaseTB(tb2);
	 tb2 = newTB(str3);
	 s1 = diffTB(tb2, tb1);
	 printf("%s", s1);
	 free(s1);
	 releaseTB(tb2);
	 tb2 = newTB(str4);
	 s1 = diffTB(tb1, tb2);
	 printf("%s", s1);
	 free(s1);
	 releaseTB(tb2);
	 releaseTB(tb1);
	 tb1 = newTB(str5);
	 tb2 = newTB(str6);
	 s1 = diffTB(tb1, tb2);
	 printf("%s", s1);
	 free(s1);
	 releaseTB(tb2);
	 releaseTB(tb1);
     tb1 = newTB(str7);
	 tb2 = newTB(str8);
	 s1 = diffTB(tb1, tb2);
	 printf("%s", s1);
	 free(s1);
	 releaseTB(tb2);
	 releaseTB(tb1);
}
// this is the general test with redo and undo
void general_cut_delete_merge_paste_undo_redo_tests_2() {
	char str1[] = "line 01\n"
		          "line 02\n"
		          "line 03\n"
		          "line 04\n"
		          "line 05\n"
		          "line 06\n";
	char str2[] = "";
	char str3[] = "line 07\n"
		          "line 08\n"
		          "line 09\n";
	char str_ans1[] = "1. line 01\n"
		              "2. line 02\n"
		              "3. line 03\n"
		              "4. line 04\n"
		              "5. line 05\n"
		              "6. line 07\n"
		              "7. line 08\n"
		              "8. line 09\n"
		              "9. line 06\n";
	char str_ans2[] = "line 07\n"
		              "line 08\n"
		              "line 09\n"
					  "line 01\n"
		              "line 02\n"
		              "line 03\n"
		              "line 04\n"
		              "line 07\n"
		              "line 08\n"
		              "line 09\n"
		              "line 05\n"
		              "line 07\n"
		              "line 08\n"
		              "line 09\n"
		              "line 06\n";
	char str_ans3[] = 
					  "line 07\n"
		              "line 08\n"
		              "line 09\n"
					  "line 01\n"
		              "line 02\n"
		              "line 03\n"
		              "line 04\n"
		              "line 07\n"
		              "line 08\n"
		              "line 09\n"
		              "line 05\n"
		              "line 07\n"
		              "line 08\n"
		              "line 09\n"
		              "line 06\n"
		              "line 09\n";
	char *s1;
	TB tb1 = newTB(str1), tb2 = newTB(str2); // check the merge empty case
	mergeTB(tb1, 2, tb2);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	redoTB(tb1);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str1) == 0);
	free(s1);
	// check paste before the last line
    tb2 = newTB(str3);
    pasteTB(tb1, 5, tb2);
    s1 = dumpTB(tb1, TRUE);
    assert(strcmp(s1, str_ans1) == 0);
	free(s1);
	// call undo to recover the paste
	undoTB(tb1);
	s1 = dumpTB(tb1, FALSE);
    assert(strcmp(s1, str1) == 0);
	free(s1);
	pasteTB(tb1, 5, tb2);
    s1 = dumpTB(tb1, TRUE);
    assert(strcmp(s1, str_ans1) == 0);
	free(s1);
	// when I call redo not just after undo nothing should happen
	redoTB(tb1);
	s1 = dumpTB(tb1, TRUE);
    assert(strcmp(s1, str_ans1) == 0);
	free(s1);
	pasteTB(tb1, 0, tb2);
	pasteTB(tb1, 7, tb2);
    s1 = dumpTB(tb1, FALSE);
    assert(strcmp(s1, str_ans2) == 0);
	free(s1);
    deleteTB(tb2, 0, 1);  // delete the 0-1 line of second TB
    mergeTB(tb2, 0, tb1);
	s1 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, str_ans3) == 0);
	free(s1);
	redoTB(tb2);	// redo stack is empty do nothing
	s1 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, str_ans3) == 0);
	free(s1);
	undoTB(tb2);  // get the tb2 with one line
	s1 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, "line 09\n") == 0);
	free(s1);
	deleteTB(tb2, 0, 0);
	s1 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, "") == 0);
	free(s1);
	// the redo will do nothing since the stack is cleared
	redoTB(tb2);
	s1 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, "") == 0);
	free(s1);
	undoTB(tb2);
	undoTB(tb2);
	undoTB(tb2);
	undoTB(tb2);
	undoTB(tb2);
	undoTB(tb2);
	undoTB(tb2);
	undoTB(tb2);
	undoTB(tb2);
	undoTB(tb2); // exceed the maximum stack volume
	redoTB(tb2); // call redo once
	deleteTB(tb2, 0, 0);
	redoTB(tb2);  // this redo has no effect at all
	s1 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, "") == 0);
	free(s1);
	// finally we have a special cutTB test
	undoTB(tb2);
	tb1 = cutTB(tb2, 0, 0);
	s1 = dumpTB(tb2, TRUE);
    assert(strcmp(s1, "") == 0);
	free(s1);
	s1 = dumpTB(tb1, TRUE);
    assert(strcmp(s1, "1. line 09\n") == 0);
	free(s1);
	undoTB(tb1);
	undoTB(tb2);
	s1 = dumpTB(tb1, TRUE);
    assert(strcmp(s1, "1. line 09\n") == 0);
	free(s1);
	s1 = dumpTB(tb2, TRUE);
    assert(strcmp(s1, "1. line 09\n") == 0);
	free(s1);
	undoTB(tb1);  // check the depth of the undo stack
	s1 = dumpTB(tb1, TRUE);
    assert(strcmp(s1, "1. line 09\n") == 0);
	free(s1);
	releaseTB(tb1);
	releaseTB(tb2);
}

void general_cut_delete_merge_paste_undo_redo_tests_1() {
	char str1[] = "line 1\n"
			      "line 2\n"
			      "line 3\n"
			      "line 4\n"
			      "line 5\n"
			      "line 6\n"
			      "line 7\n"
			      "line 8\n";
	char ans_1[] = 
			      "1. line 4\n"
			      "2. line 5\n"
			      "3. line 6\n"
			      "4. line 7\n"
			      "5. line 8\n";
	char ans_2a[] = "1. line 4\n"
			      "2. line 7\n"
			      "3. line 8\n";
	char ans_2b[] = "1. line 5\n"
			      "2. line 6\n";
	char ans_3[] = "1. line 5\n"
			      "2. line 6\n"
				  "3. line 4\n"
			      "4. line 7\n"
			      "5. line 8\n";
	char ans_4[] = "1. line 5\n"
			      "2. line 6\n"
				  "3. line 4\n"
			      "4. line 7\n"
			      "5. line 8\n"
			      "6. line 5\n"
			      "7. line 6\n";
	TB tb1 = newTB(str1);
	char *s1, *s2;
	deleteTB(tb1, 1,1);
	deleteTB(tb1, 0,1);
	deleteTB(tb1, 4,4); 
	undoTB(tb1);  // undo process
	s1 = dumpTB(tb1, TRUE);  // first 3 lines should remove
	assert(strcmp(s1, ans_1) == 0);
	free(s1);
	TB tb2 = cutTB(tb1, 1, 2);
	redoTB(tb1);  // nothing should happen in here
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, ans_2a) == 0);
	free(s1);
	s2 = dumpTB(tb2, TRUE);// check the cutting texts
	assert(strcmp(s2, ans_2b) == 0);
	undoTB(tb2);
	// printf("%s\n%s", s1, s2);
	free(s2);
	s2 = dumpTB(tb2, TRUE);
	assert(strcmp(s2, ans_2b) == 0);
	free(s2);
	redoTB(tb2);  // now we redo again the cutting text will be back
	s2 = dumpTB(tb2, TRUE);
	assert(strcmp(s2, ans_2b) == 0);
	free(s2);
	pasteTB(tb1, 0, tb2);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, ans_3) == 0);
	free(s1);
	redoTB(tb1); // nothing should happen
	redoTB(tb2); // nothing should happen
	pasteTB(tb1, 5, tb2);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, ans_4) == 0);
	free(s1);
	undoTB(tb1);
	mergeTB(tb1, 5, tb2);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, ans_4) == 0);
	free(s1);
	redoTB(tb1);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, ans_4) == 0);
	free(s1);
	undoTB(tb1);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, ans_3) == 0);
	free(s1);
	addPrefixTB(tb1, 0, 0, ""); // just a single destructive operation
	undoTB(tb1);
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(s1, ans_3) == 0);
	free(s1);
	releaseTB(tb1);
	// releaseTB(tb2);
}

void destroy_list(Match l) {
	if (l == NULL) {
		return;
	}
	destroy_list(l->next);
	free(l);
}
// this is the searchTB test
void blackbox_search_test() {
	char str1[] = "line 001--line\n"
	              "he is line10\n"
	              "this line 0 is line\n"
	              "theline 01line 01\n"
	              "hi\n"
	              "li-ne 0\n"
	              "\n"
	              "llllliiline 0\n";
	int ans[5][2] = {{1,0}, {3, 5}, {4, 3}, {8, 7}};
	TB tb1 = newTB(str1);
	Match l = searchTB (tb1, "line 0"), p;
	int i = 0;
	p = l;
	while (p != NULL) {
		assert(p->lineNumber == ans[i][0] && p->charIndex == ans[i][1]);
		i++;
		p = p->next;
	}
	destroy_list(l);
	// second test case involves the nested matching word
	char str2[] = "abcbcbabcbcb\n"
	              "aabbccbbdd\n"
	              "abababbb\n"
	              "abababb\n"
	              "accbc\n"
	              "ababb\n"
	              "\n"
	              "aaakbbbb\n";
	int ans_2[6][2] = {{1,0}, {2, 1}, {3, 0}, {4, 0}, {6, 0}};
	TB tb2 = newTB(str2);
	l = searchTB (tb2, "ab");
	i = 0;
	p = l;
	while (p != NULL) {
		assert(p->lineNumber == ans_2[i][0] && p->charIndex == ans_2[i][1]);
		i++;
		p = p->next;
	}
	destroy_list(l);
	l = searchTB (tb2, "");    // case of searching '\0'
	assert(l == NULL);
	destroy_list(l);
//	l = searchTB (tb2, NULL);  // this case abort 	
	char str3[] = "abcabdabaaaba\n";
	int ans3[2][2] = {{1, 6}};
	TB tb3 = newTB(str3);
	l = searchTB(tb3, "abaaa");
	i = 0;
	p = l;
	while (p != NULL) {
		assert(p->lineNumber == ans3[i][0] && p->charIndex == ans3[i][1]);
		i++;
		p = p->next;
	}
	destroy_list(l);
	char str4[] = "abcabdabaaaba\n"  // case when no match node
				  "hello_world\n";
	// int ans3[2][2] = {{1, 6}};
	TB tb4 = newTB(str4);
	l = searchTB(tb4, "abdaa");
	assert(l == NULL);
	destroy_list(l);
	char str5[] = "cababadabaaaba\n"  // case when nested
				  "hello_world\n";
    int ans5[2][2] = {{1, 1}};
	TB tb5 = newTB(str5);
	l = searchTB(tb5, "aba");
	i = 0;
	p = l;
	while (p != NULL) {
		assert(p->lineNumber == ans5[i][0] && p->charIndex == ans5[i][1]);
		i++;
		p = p->next;
	}
	destroy_list(l);
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
	releaseTB(tb4);
	releaseTB(tb5);
}
// this is the line number counting test
void blackbox_line_test() {
	TB tb1, tb2, tb3, tb4, tb5;
	char str1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
	char str2[] = "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n";
	char str3[] = "";
	char str4[] = "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 0\n"
				  "I am line 3 - 4\n"
				  "I am line 5 - 1\n";
	char str5[] = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	tb1 = newTB(str1);
	tb2 = newTB(str2);
	tb3 = newTB(str3);
	tb4 = newTB(str4);
	tb5 = newTB(str5);
	// check the linesTB function
	assert(linesTB(tb1) == 6);   // case with \n\n
	assert(linesTB(tb2) == 5);  // case with \n\n
	assert(linesTB(tb3) == 0);  // case for empty TB
	assert(linesTB(tb4) == 5);  // normal case
	assert(linesTB(tb5) == 15);  // all with \n case
	// clean up the TBs and finish the testing
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
	releaseTB(tb4);
	releaseTB(tb5);
}
// this is the new_dump test
void blackbox_new_dump_test() {
	TB tb1, tb2, tb3, tb4, tb5;
	char *s1;
	char str1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
    // ans_str1 is the one with line numbers
	char ans_str1[] = "1. I am line 3 - 0\n"
				  	  "2. I am line 3 - 1\n"
				      "3. I am line 3 - 2\n"
				      "4. I am line 3 - 3\n"
				      "5. I am line 3 - 4\n6. \n";  
	char str2[] = "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n";
	char str3[] = "";
	char str4[] = "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 0\n"
				  "I am line 3 - 4\n"
				  "I am line 5 - 1\n";
    char ans_str4[] = "1. I am line 3 - 2\n"
				  "2. I am line 3 - 3\n"
				  "3. I am line 3 - 0\n"
				  "4. I am line 3 - 4\n"
				  "5. I am line 5 - 1\n";
	char str5[] = "\n\n\n\n\n\n\n\n\n\n";
	char ans_str5[] = "1. \n2. \n3. \n4. \n5. \n6. \n7. \n8. \n9. \n10. \n";
	tb1 = newTB(str1);
	s1 = dumpTB(tb1, FALSE);
	// check the dump result without showLine number
	assert(strcmp(s1, str1) == 0); 
	free(s1);
	s1 = dumpTB(tb1, TRUE);
	// check the dump result with showLine number
	// and make sure that \n is on a newline
	assert(strcmp(s1, ans_str1) == 0); 
	free(s1);
	tb2 = newTB(str2);
	s1 = dumpTB(tb2, FALSE);
	// check the dump result without showLine number
	assert(strcmp(s1, str2) == 0); 
	free(s1);
	tb3 = newTB(str3);
	s1 = dumpTB(tb3, TRUE);
	// check the dump result on an empty TB
	assert(strcmp(s1, str3) == 0); 
	free(s1);
	tb4 = newTB(str4);
	s1 = dumpTB(tb4, TRUE);
	// check the dump result with showLine number
	// and make sure that \n is on a newline
	assert(strcmp(s1, ans_str4) == 0); 
	free(s1);
	// check cases that are all newline
	tb5 = newTB(str5);
	s1 = dumpTB(tb5, TRUE);
	assert(strcmp(s1, ans_str5) == 0);
	free(s1);
	// clean up the TBs and finish the testing
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
	releaseTB(tb4);
	releaseTB(tb5);
}
// this is the add prefix test
void blackbox_prefix_test() {
	TB tb1, tb2, tb3;
	char *s1;
	char str1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
    // ans_str1 is the one with line numbers
	char ans_str1_prefix[] = "1. hello-I am line 3 - 0\n"
				  	  		 "2. hello-I am line 3 - 1\n"
				      		 "3. hello-I am line 3 - 2\n"
				      		 "4. hello-I am line 3 - 3\n"
				      		 "5. I am line 3 - 4\n6. \n";			  
	char str2[] = "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n";
	char ans_str2_prefix[] = "1. \n2. I am line *4 - 0 *\n"
				  "3. I am line __4_ - 1\n"
				  "4. hi- # I am line 4 - 2\n5. hi- \n";
	char str3[] = "I am line 1\n"
				  "I am line 2\n"
				  "I am line 3\n"
				  "I am line 4\n";
	tb1 = newTB(str1);
	tb2 = newTB(str2);
	tb3 = newTB(str3);
	// check the addPrefix function
	addPrefixTB(tb1, 0, 3, "hello-"); // normal case
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(ans_str1_prefix, s1) == 0);
	free(s1);
	addPrefixTB(tb2, 3, 4, "hi- "); // case with prefix to \n
	s1 = dumpTB(tb2, TRUE);
	assert(strcmp(ans_str2_prefix, s1) == 0);
	free(s1);
	addPrefixTB(tb2, 1, 2, ""); // case with '\0'
	s1 = dumpTB(tb3, FALSE);
	assert(strcmp(str3, s1) == 0);
	free(s1);
	
//	addPrefixTB(tb2, 1, 2, NULL); // this should abort
//	addPrefixTB(tb3, 0, 0, "hello-");  // this should abort
	// clean up the TBs and finish the testing
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
	
}
// this is the form rich text test
void blackbox_richText_test() {
	TB tb1;
	char str1[] = "*some string\n"
				  "*some string*lol*\n"
				  "*some_string*again_\n"
				 "*some* _string_\n"
				 "some *string_again_\n"
				 "some#string*once_again*\n"
				 "#string_stuff_\n"
				 "*some_string_*\n"
				 "*#__*_A_#***\n"
				 "#some _string_\n"
				 "**\n"
				 "*\n"
				 "__\n"
				 "#\n"  // do nothing case
				 "***\n"   // do nothing case
				 "___\n"   // do nothing case
				 "##\n"  // not doing nothing case
				 "**double's should do nothing\n"
				 "__double should __sometimes do something\n"
				 "**hello**\n"
				 "*_*_*_*_*_*_\n"  //  nested cases
				 "***_***_***h**h***_\n"  // nested case
				 "*hello * *_*_world*\n"
				 "__________\n"
				 "**********\n";
	char ans[] = "*some string\n"
				 "<b>some string</b>lol*\n"
			     "<b>some_string</b>again_\n"
				 "<b>some</b> <i>string</i>\n"
				 "some *string<i>again</i>\n"
				 "some#string<b>once_again</b>\n"
			     "<h1>string_stuff_</h1>\n"
			     "<b>some_string_</b>\n"
			     "<b>#__</b><i>A</i>#***\n"
		         "<h1>some _string_</h1>\n"
		         "**\n"
		         "*\n"
				 "__\n"
				 "#\n"
				 "***\n"
				 "___\n"
				 "<h1>#</h1>\n"
				 "**double's should do nothing\n"
				 "_<i>double should </i>_sometimes do something\n"
				 "*<b>hello</b>*\n"
				 "<b>_</b><i>*</i><b>_</b><i>*</i>\n"
				 "**<b>_</b>*<b>_</b>*<b>h</b><b>h</b>**_\n"
				 "<b>hello </b> <b>_</b>_world*\n"
				 "__________\n"
				 "**********\n";
    tb1 = newTB(str1);
	formRichText(tb1);
	char *s1 = dumpTB(tb1, FALSE);
	// printf("%s", s1);
	assert(strcmp(s1, ans) == 0);
	free(s1);
	releaseTB(tb1);
}

