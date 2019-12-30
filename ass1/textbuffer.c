 #include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "textbuffer.h"

#define TRUE 1
#define FALSE 0
#define COPY 2 
#define DELETE 3
#define MERGE 4
#define PASTE 5
#define UNDO 0
#define REDO 1
#define OTHER 2
#define UNREVERSABLE 3
#define SAVE 1
#define REMOVE  -1
#define TMP  0

struct node {
	char *str;
	struct node *next;   // this struct is a fixed-size stack nodes
	struct node *prev;   // used for the undo and redo
	int from;
	int to;
	int command_code;
};

typedef struct stack {
	int size;
	struct node *last;
	struct node *first;
}Stack;

typedef struct line {
	int line_num;       
	char *text;
	struct line *next;
	struct line *prev;
}*Line;

struct textbuffer{
	Line first;
	Line last;
	int total_lines;
	Stack *undo;
	Stack *redo;
	int process;   // process it a markerindicate whether push to redo or undo
	int last_call;
};

// new functions
static int max(int a, int b);
static int find_lcs(int *f[], char *str1[], char *str2[], int n1, int n2);
static void operations(Line l);
static Line newLine(int lnum,char *s);
static TB copyTB (TB tb, int from, int to);
static Match insert_matchnode(Match list, int line, int index);
static int KMP(char *str, int slen, char *ptr, int plen);
static void cal_next(char *str, int *next, int len);
static void free_node(Match l);
static int isvalidTB(TB tb);
static Stack *new_stack();
static void push(Stack *s, char *str, int from, int to, int commandcode);
static int isempty(Stack *s);
static void pop(Stack *s);
static void destroy_stack(TB tb);
static void locate_lcs(int *f[], int state1[], int state2[], char *str1[], 
						char * str2[], int len1, int len2);
static void new_dump_test();
static void line_test();
static void prefix_test();
static void delete_test();
static void merge_test();
static void paste_test();
static void cut_test();
static void search_test();
static void richText_test();
// static void diff_test();
// this function creates a new stack size 0 with first, last points to NULL
static Stack *new_stack() {
	Stack *s = (Stack *) malloc(sizeof(Stack));
	s->size = 0;
	s->first = s->last = NULL;
	return s;
}
// this function is stack push
static void push(Stack *s, char *str, int from, int to, int commandcode) {
	struct node *new_node = (struct node *) malloc(sizeof(struct node));
	assert(new_node != NULL);
	struct node *tmp;
	new_node->prev = NULL;
    new_node->next = NULL;
	new_node->from = from;
	new_node->to = to;
	new_node->command_code = commandcode;
	new_node->str = strdup(str);   // add information of the new node
	if (s->size == 0) {
		s->first = s->last = new_node;
		s->size++;      // case of push to an empty stack
		return;
	}
	
	if (s->size == 10) {
		new_node->next = s->first;   // case when the size is 10
		s->first->prev = new_node;  
		s->first = new_node;
		tmp = s->last;
		s->last = s->last->prev;
		free(tmp->str);        // destroy the last element in the stack
		free(tmp);
		s->last->next = NULL;
		return;
	}
	
	new_node->next = s->first;  // case when there are 1-9 elements
	s->first->prev = new_node; // normal stack push operation
	s->first = new_node;
	s->size++;
}
// this function judge whether the stack is empty
static int isempty(Stack *s) {
	return s->size == 0 ? TRUE : FALSE;
}
// this pop is quite different it is actually free up the first node
static void pop(Stack *s) {
	struct node *tmp;
	s->size--;
	if (s->size == 0) {
		free(s->first->str);
		free(s->first);
		s->first = s->last = NULL;
		return;
	}
	
	tmp = s->first;
	s->first = s->first->next;
	free(tmp->str);
	free(tmp);
	s->first->prev = NULL;
}
// this function create a new line of the given text
static Line newLine(int lnum,char *s) {
	Line l = (Line) malloc(sizeof(struct line));
	l->line_num = lnum;
	l->text = strdup(s);
	l->next = NULL;
	l->prev = NULL;
	return l;
}
// this function free up nodes in an linked lists
// it is just used for the white box test to prevent
// memory leak   and the method is recursion
static void free_node(Match l) {
	if (l == NULL) {
		return;        // base case
	}
	free_node(l->next);   // recursive case
	free(l);
}
/* Allocate a new textbuffer whose contents is initialised with the text given
 * in the array.
 */
TB newTB (char text[]) {
	TB tb = (TB) malloc(sizeof(struct textbuffer));
	assert(tb != NULL);
	tb->first = tb->last = NULL;
	tb->total_lines = 0;		// init the newTB
	tb->process = 1;
	tb->undo = new_stack();
	tb->redo = new_stack();
	tb->last_call = OTHER;  // a none destructive operation
	int i, k = -1;
	Line l = NULL;
	char *s = (char *) malloc(sizeof(char));
	assert(s != NULL);
	s[0] = '\0'; // set s[0] to be '\0' a NULL terminate string
	for (i = 0 ; i < strlen(text); i++) {
		if (text[i] != '\n') {
		// keeps adding text to the temparary string
				s = (char *) realloc(s ,sizeof(char) * (strlen(s) + 3));
				assert(s != NULL);
				s[strlen(s)] = '\0';
				s[strlen(s) + 1] = '\0';
				s[strlen(s)] = text[i];
		} else {  // when a new line is reached insert a new node to the TB
			k++;
			if (tb->total_lines == 0) {  // case when insert the first line
				tb->last = tb->first = newLine(k, s);
				tb->total_lines++;
			} else {
				l = newLine(k, s);
				l->prev = tb->last;
				tb->last->next = l;
				tb->last = l;
				tb->total_lines++;
			}
	//		s = strdup("\0");
			free(s);
			s =  (char *) calloc(1, sizeof(char));
			assert(s != NULL);
			s[0] = '\0';  // this line is actually useless
		}
	}
	free(s); // free up the tmp array to prevent leak
	return tb;
}

/* Free the memory occupied by the given textbuffer.  It is an error to access
 * the buffer afterwards.
 */

void releaseTB (TB tb) {
    if (tb == NULL) {
    	return;  // tb is NULL do nothing
    }
	Line curr = tb->first;
//	struct node *p;
	while (curr != NULL) {
		tb->first = tb->first->next;
		free(curr->text);    // free the text
		free(curr);          // free the node of a line
		curr = tb->first;
	}
	
	destroy_stack(tb);  // destroy the stack of TB
	free(tb->first);	
	free(tb);
}

/* Allocate and return an array containing the text in the given textbuffer.
 * add a suffix corrosponding to line number iff showLineNumbers == TRUE
 */
char *dumpTB (TB tb, int showLineNumbers) {
	char *str = (char *) calloc(1, sizeof(char));
	char *tmp = (char *) calloc(1, sizeof(char));
	assert(str != NULL && tmp != NULL);
	Line curr;
	str[0] = '\0';  // useless but make me feel safer
	tmp[0] = '\0';  // useless but make me feel safer
	if (tb == NULL) {
		free(tmp);
		return str;
	}
	
	if (tb->total_lines == 0) {
		free(tmp);
		return str;
	}
	
	if (showLineNumbers == FALSE) {
		curr = tb->first;
		while (curr != NULL) {
			str = (char *) realloc(str, sizeof(char) * 
								(strlen(str) + strlen(curr->text) + 2));
			assert(str != NULL);
			str = strcat(str, curr->text);
			str[strlen(str)] = '\0';
			str[strlen(str) + 1] = '\0';
			str[strlen(str)] = '\n';
			curr = curr->next;
		}
	} else if (showLineNumbers == TRUE) {
		curr = tb->first;
		while (curr != NULL) {
			tmp = (char *) realloc(tmp, 14 * sizeof(char));
			assert(tmp != NULL);
			sprintf(tmp, "%d", curr->line_num + 1); // use sprintf to let int->str
			strcat(tmp, ". ");
			str = (char *) realloc(str, sizeof(char) * 
									(strlen(str) + strlen(tmp) + 2));
			assert(tmp != NULL);
			strcat(str, tmp); // add the int. to the str
			str = (char *) realloc(str, sizeof(char) * 
							  (strlen(str) + strlen(curr->text) + 3));
			assert(str != NULL);
			str = strcat(str, curr->text); // add the text to the str
			str[strlen(str)] = '\0';
			str[strlen(str) + 1] = '\0'; // string to be NULL terminated
			str[strlen(str)] = '\n';
			free(tmp);
			tmp = (char *) calloc(1, sizeof(char));
			assert(tmp != NULL);
			tmp[0] = '\0';
			curr = curr->next;
		}
	}
	free(tmp);  // free the temparary array to prevent memory leak
	return str;
}

/* Return the number of lines of the given textbuffer.
 */
int linesTB (TB tb){
	return tb->total_lines;   // directly read from the struct
}

/* Add a given suffix to all lines between pos1 and pos2
 *
 * - The program is to abort() with an error message if line 'pos1' or line
 *   'pos2' is out of range.  The first line of a textbuffer is at position 0.
 */
void addPrefixTB (TB tb, int pos1, int pos2, char* prefix){
	assert(tb != NULL);
	if (prefix == NULL) {
		abort();
	}
	tb->last_call = UNREVERSABLE;
	if (prefix[0] == '\0') {
		return;
	}
	int start, end;
	Line curr = tb->first;
	char *tmp = (char *) calloc(2, sizeof(char));
	assert(tmp != NULL);
	if (pos1 < 0 || pos2 >= linesTB(tb) || pos1 > pos2) {
		fprintf(stderr, "the numbers are out of range\n"); // invalid pos
		abort();
	} else {
		start = pos1;
		end = pos2;
		curr = tb->first;
		while (curr != NULL) {
			if (curr->line_num >= start && curr->line_num <= end) {
				tmp = realloc(tmp, 2 + strlen(curr->text) + strlen(prefix));
				assert(tmp != NULL);
				strcat(tmp, prefix);
				strcat(tmp, curr->text);  // use a tmp to store the prefix+text
				curr->text = realloc(curr->text, 
									2 + strlen(curr->text) + strlen(prefix));
				assert(curr->text != NULL);
				strcpy(curr->text, tmp);
				free(tmp);
				tmp = (char *) calloc(2, sizeof(char));
				assert(tmp != NULL);
			}
			curr = curr->next;
		}
	}
	free(tmp);
}
// this function destroys stack 
// of the textbuffer (redo and undo stack)

static void destroy_stack(TB tb) {
	struct node *p;
	if (!isempty(tb->undo)) {   // destroy the undo stack
		p = tb->undo->first;
		while (p != NULL) {
			tb->undo->first = tb->undo->first->next;
			free(p->str);
			free(p);
			p = tb->undo->first;
		}
		free(tb->undo->first);
		
	}
	free(tb->undo);
	if (!isempty(tb->redo)) {  // destroy the redo stack
		p = tb->redo->first;
		while (p != NULL) {
			tb->redo->first = tb->redo->first->next;
			free(p->str);
			free(p);
			p = tb->redo->first;
		}
		free(tb->redo->first);
		
	}
	free(tb->redo);
}
/* Merge 'tb2' into 'tb1' at line 'pos'.
 *
 * - Afterwards line 0 of 'tb2' will be line 'pos' of 'tb1'.
 * - The old line 'pos' of 'tb1' will follow after the last line of 'tb2'.
 * - After this operation 'tb2' can not be used anymore (as if we had used
 *   releaseTB() on it).
 * - The program is to abort() with an error message if 'pos' is out of range.
 */
void mergeTB (TB tb1, int pos, TB tb2){
	assert(tb1 != NULL && tb2 != NULL);
	if (tb1 == tb2) {
		return;
	}
	
	if (pos > linesTB(tb1) || pos < 0) {
		fprintf(stderr, "the numbers are out of range\n");
		abort();
	}
	tb1->last_call = OTHER;
	char *str = dumpTB(tb2, FALSE);
	int lines = 0;
	// use the process as a marker to decide which stack to push to
	if (tb1->process == 1) {
		push(tb1->undo, str, pos, pos + linesTB(tb2) - 1, MERGE);
	} else {
		push(tb1->redo, str, pos, pos + linesTB(tb2) - 1, MERGE);
		tb1->process = 1;
	}
	free(str);
	if (linesTB(tb2) == 0) {
		releaseTB(tb2);  // special case just release tb2
		return;
	}
	Line curr = tb1->first;
	if (linesTB(tb1) == 0) { // case when tb1 is empty
		tb1->first = tb2->first;
		tb1->last = tb2->last;
		tb1->total_lines = tb2->total_lines;
		destroy_stack(tb2);
		free(tb2); 
		curr = tb1->first;
		lines = 0;
		while (curr != NULL) {
			curr->line_num = lines++;
			curr = curr->next;
		}
		return;
	}
	if (pos == 0) {  // case to insert at front
		tb2->last->next = tb1->first;
		tb1->first->prev = tb2->last;
		tb1->first = tb2->first;
		tb1->total_lines += tb2->total_lines;
		destroy_stack(tb2);  // this line is very important destroy the stack
		free(tb2); 
	} else if (pos == linesTB(tb1)) { // append at the back
		tb1->last->next = tb2->first;
		tb2->first->prev = tb1->last;
		tb1->last = tb2->last;
		tb1->total_lines += tb2->total_lines;
		destroy_stack(tb2);  // this line is crucial to prevent memory leak
		free(tb2);
	} else {  // insert in the centre
		while (curr != NULL) {
			if (curr->line_num == pos - 1) {
				tb2->last->next = curr->next;
				curr->next->prev = tb2->last;
				curr->next = tb2->first;
				tb2->first->prev = curr;
				tb1->total_lines += tb2->total_lines;
				destroy_stack(tb2);
				free(tb2);
				break;
			}
			curr = curr->next;
		}
	}
	curr = tb1->first;  // rewrite the linenums
	lines = 0;
	while (curr != NULL) {
		curr->line_num = lines++;
		curr = curr->next;
	}
}

/* Copy 'tb2' into 'tb1' at line 'pos'.
 *
 * - Afterwards line 0 of 'tb2' will be line 'pos' of 'tb1'.
 * - The old line 'pos' of 'tb1' will follow after the last line of 'tb2'.
 * - After this operation 'tb2' is unmodified and remains usable independent
 *   of 'tb1'.
 * - The program is to abort() with an error message if 'pos' is out of range.
 */
void pasteTB (TB tb1, int pos, TB tb2) {
	assert(tb1 != NULL);
	assert(tb2 != NULL);
	if (tb1 == tb2) {
		return;
	}
	tb1->last_call = OTHER;
    char *str = dumpTB(tb2, FALSE);
	TB tb3 = newTB(str);
	mergeTB(tb1, pos, tb3);
	free(str);
}

/* Copy the lines between and including 'from' and 'to' out of the textbuffer
 * 'tb'.
 *
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The program is to abort() with an error message if 'from' or 'to' is out
 *   of range.
 */
static TB copyTB (TB tb, int from, int to) {
	if (from < 0 || to >= linesTB(tb) || from > to) {
		fprintf(stderr, "the numbers are out of range\n");
		abort();
	}
	TB tb_new = (TB) malloc(sizeof(struct textbuffer));
	assert(tb_new != NULL);
	tb_new->first = tb_new->last = NULL;
	tb_new->total_lines = 0;
	tb_new->redo = new_stack();
	tb_new->undo = new_stack();
	tb_new->process = 1;
	tb->last_call = OTHER;
	Line curr = tb->first, tmp;
	while (curr != NULL) {
		if (curr->line_num >= from && curr->line_num <= to) {
			if (linesTB(tb_new) == 0) {
				tb_new->first = tb_new->last = newLine(tb_new->total_lines,curr->text);
				tb_new->total_lines++;
			} else {
				tmp = newLine(tb_new->total_lines,curr->text);
				tmp->prev = tb_new->last;
				tb_new->last->next = tmp;
				tb_new->last = tmp;
				tb_new->total_lines++;
			}
		}
		curr = curr->next;
	}
	return tb_new;
}

/* Cut the lines between and including 'from' and 'to' out of the textbuffer
 * 'tb'.
 *
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The program is to abort() with an error message if 'from' or 'to' is out
 *   of range.
 */
TB cutTB (TB tb, int from, int to) {
	assert(tb != NULL);
	if (from < 0 || to >= linesTB(tb) || from >= linesTB(tb) || to < 0) {
		fprintf(stderr, "the numbers are out of range\n");
		abort();
	}
    if (to < from) {
    	tb->last_call = OTHER;
    	push(tb->undo, "", 0, 0, MERGE);	
    	return NULL;
    }
    tb->last_call = OTHER;
	TB tb_new = copyTB(tb, from, to);
	tb_new->last_call = OTHER;
	deleteTB(tb, from, to);
	// char *str = dumpTB(tb_new, FALSE);
	// push(tb_new->undo, str, 0, linesTB(tb_new) - 1, MERGE);
	// free(str);
	return tb_new;
}

/*  Return a linked list of Match nodes of all the matches of string search
 *  in tb
 *
 * - The textbuffer 'tb' will remain unmodified.
 * - The user is responsible of freeing the returned list
 */
 // time complexity is optimize by using KMP and scan from the back
Match searchTB (TB tb, char* search){
	assert(tb != NULL);
    if (search == NULL) {
    	abort();
    }
    if (search[0] == '\0') {
    	return NULL;
    }
	Line curr = tb->last;  // scan from the back will make time complexity good
	int charIndex = -1;
	Match matchlist = NULL;
	while (curr != NULL) {
	  charIndex = KMP(curr->text, strlen(curr->text), search, strlen(search));
	  if (charIndex >= 0) {
		 matchlist = insert_matchnode(matchlist,curr->line_num + 1,charIndex);
	  }  // if we have a node that match then insert to the single link list
	  curr = curr->prev;
	}
	return matchlist;
}
// this is a fast comparing sub-string alogrithim using KMP
// matching str and ptr
static int KMP(char *str, int slen, char *ptr, int plen) {
    int *next = (int *) calloc(plen + 1, sizeof(int) );
    assert(next != NULL);
    cal_next(ptr, next, plen);   // calculate the next position to match
    int k = -1, i;
    for (i = 0; i < slen; i++) {
        while (k > -1 && ptr[k + 1] != str[i]) {  // if it doesn't match
            k = next[k];      // instead of comparing with 0 compare with next
        }
        if (ptr[k + 1] == str[i]) {
            k = k + 1;
        }
        if (k == plen-1) {   // found one return i - plen + 1 is the index
        	free(next);
            return i - plen + 1;
        }
        
    }
    free(next);
    return -1;  // cannot found one
}
// this function cooperate with the KMP algorithum
static void cal_next(char *str, int *next, int len) {
    next[0] = -1;
    int k = -1, q;
    for (q = 1; q <= len-1; q++) {
        while (k > -1 && str[k + 1] != str[q]) {
            k = next[k];  // back-track
        }
        if (str[k + 1] == str[q]) {
            k = k + 1;
        }
        next[q] = k;
    }
}
// this function  insert a node to the front of the list
static Match insert_matchnode(Match list, int line, int index) {
	Match new_node = (Match) malloc(sizeof(struct _matchNode));
	assert(new_node != NULL);
	new_node->lineNumber = line;
	new_node->charIndex = index;
	new_node->next = NULL;
	if (list == NULL) {
		list = new_node;
		return list;
	}
	new_node->next = list;
	list = new_node;
	return list;
}

/* Remove the lines between and including 'from' and 'to' from the textbuffer
 * 'tb'.
 *
 * - The program is to abort() with an error message if 'from' or 'to' is out
 *   of range.
 */
void deleteTB (TB tb, int from, int to){
	// invalid from and to 
	assert(tb != NULL);
	if (from < 0 || to >= linesTB(tb) || from > to) {
		fprintf(stderr, "the numbers are out of range\n");
		abort();
		return;
	} 
	tb->last_call = OTHER;
	TB tb_new = copyTB(tb, from, to);  // make a copy of the lines
	char *str = dumpTB(tb_new, FALSE);
	if (tb->process == 1) {  // this means we are really deleting
		push(tb->undo, str, from, to, DELETE);  // push to the stack
	} else {  // this means we reverse a process involves delete
		push(tb->redo, str, from, to, DELETE);
		tb->process = 1;
	}
	releaseTB(tb_new);
	free(str);
	Line curr1 = tb->first, curr2;
	int line = 0;
	// case when delete the middle
	if (from != 0 && to != linesTB(tb) - 1) {
		while (curr1 != NULL) {
			if (curr1->next->line_num > to) {
				break;
			}
			
			if (curr1->next->line_num < from) {
				curr1 = curr1->next;
			} else {
				curr2 = curr1->next;
				curr1->next = curr1->next->next;
				free(curr2->text);
				free(curr2);
			}
		}
		curr1->next->prev = curr1;  // relink the prev node
		curr1 = tb->first;         // renumber the TB
		while (curr1 != NULL) {
			curr1->line_num = line;
			line++;
			curr1 = curr1->next;
		}   // recalculate total_lines
		tb->total_lines = tb->total_lines - (to - from + 1); 
		return;
	} else if (from == 0 && to != linesTB(tb) - 1) {
		while (curr1 != NULL) {  // case when delete from 0-sth
			if (curr1->line_num > to) {
				tb->first = curr1;
				tb->first->prev = NULL;
				break;
			} else {
				curr1 = tb->first;
				tb->first = tb->first->next;
				free(curr1->text);
				free(curr1);
				curr1 = tb->first;
			}
		}
		curr1 = tb->first;
		while (curr1 != NULL) {
			curr1->line_num = line;
			line++;
			curr1 = curr1->next;
		}
		tb->total_lines = tb->total_lines - (to - from + 1); 
		return;// case when delete sth-last
	} else if (from != 0 && to == linesTB(tb) - 1) {
		while (curr1->next != NULL) {
			if (curr1->next->line_num < from) {
				curr1 = curr1->next;
			} else {
				tb->last = curr1;
				break;
			}
		}
		
		curr1 = tb->last->next;
		while (curr1 != NULL) {
			tb->last->next = curr1->next;
			free(curr1->text);
			free(curr1);
			curr1 = tb->last->next;
		}
		curr1 = tb->first;
		while (curr1 != NULL) {
			curr1->line_num = line;
			line++;
			curr1 = curr1->next;
		}
		tb->total_lines = tb->total_lines - (to - from + 1); 
		return; // case when delete all
	} else if (from == 0 && to == linesTB(tb) - 1) {
		while (curr1 != NULL) {
				curr1 = tb->first;
				tb->first = tb->first->next;
				free(curr1->text);
				free(curr1);
				curr1 = tb->first;
		}
		free(tb->first);
		tb->first = tb->last = NULL;
		tb->total_lines = 0; 
	}
}


/* Search every line of tb for each occurrence of a set of specified subsitituions
 * and alter them accordingly
 *
 * refer to spec for table.
 */
void formRichText (TB tb){
	assert(tb != NULL);
	Line curr = tb->first;
	tb->last_call = UNREVERSABLE;
	while (curr != NULL) {
		operations(curr);
		curr = curr->next;
	}
}

void operations(Line curr) {
	int i, j, d, p;
	// char *s = strdup(curr->text);
	//printf("%s %d\n", str, strlen(str));
	for (i = 0 ; i < strlen(curr->text); i++) {
		p = -1;
		if (curr->text[i] == '_') {
			p = -1;
			for (j = i + 1 ; j < strlen(curr->text); j++) {
				if (curr->text[j] == '_' && j - i == 1) {
					break;
				}
				if (curr->text[j] == '_' && j - i > 1) {
					p = j;  // condition: when there is a substitution and str
					break;
				}
			}
			if (p != -1) {
				curr->text = (char *) realloc(curr->text, 
											  strlen(curr->text) + 9);
				assert(curr->text != NULL);
				// begining the tag
				d = strlen(curr->text) + 2;
				curr->text[d] = '\0';
				curr->text[d + 1] = '\0';
				curr->text[d + 2] = '\0';
				curr->text[d + 3] = '\0';
				curr->text[d + 4] = '\0';
				for (j = d; j >= i + 3; j --) {
					curr->text[j] = curr->text[j - 2];
				}
				curr->text[i] = '<';
				curr->text[i+1] = 'i';
				curr->text[i+2] = '>';
				p = p + 2;
				curr->text = (char *) realloc(curr->text, 
										      strlen(curr->text) + 9);
				assert(curr->text != NULL);
				// closing the tag
				d = strlen(curr->text) + 3;
				curr->text[d] = '\0';
				curr->text[d + 1] = '\0';
				curr->text[d + 2] = '\0';
				curr->text[d + 3] = '\0';
				curr->text[d + 4] = '\0';
				for (j = d; j >= p + 4; j --) {
					curr->text[j] = curr->text[j - 3];
				}
				curr->text[p] = '<';
				curr->text[p+1] = '/';
				curr->text[p+2] = 'i';
				curr->text[p+3] = '>';
				i = p + 1;
			}
		}
		if (curr->text[0] == '#' && strlen(curr->text) > 1) {
				curr->text = (char *) realloc(curr->text, strlen(curr->text) + 12);
				assert(curr->text != NULL);
				d = strlen(curr->text) + 3;
				curr->text[d] = '\0';
				curr->text[d + 1] = '\0';
				curr->text[d + 2] = '\0';
				curr->text[d + 3] = '\0';
				curr->text[d + 4] = '\0';
				curr->text[d + 5] = '\0';
				curr->text[d] = '<';
				curr->text[d + 1] = '/';
				curr->text[d + 2] = 'h';
				curr->text[d + 3] = '1';
				curr->text[d + 4] = '>';
				curr->text[d + 5] = '\0';
				curr->text[d + 6] = '\0';
				for (j = d - 1; j >= i + 4; j --) {
					curr->text[j] = curr->text[j - 3];
				}
				curr->text[i] = '<';
				curr->text[i+1] = 'h';
				curr->text[i+2] = '1';
				curr->text[i+3] = '>';
				break;
		}
		if (curr->text[i] == '*') {
			p = -1;
			for (j = i + 1 ; j < strlen(curr->text); j++) {
				if (curr->text[j] == '*' && j - i == 1) {
					break;;
				}
				if (curr->text[j] == '*' && j - i > 1) {
					p = j;
					break;
				}
			}
			if (p != -1) {
				curr->text = (char *) realloc(curr->text, strlen(curr->text) + 9);
				assert(curr->text != NULL);
				d = strlen(curr->text) + 2;
				curr->text[d] = '\0';
				curr->text[d + 1] = '\0';
				curr->text[d + 2] = '\0';
				curr->text[d + 3] = '\0';
				curr->text[d + 4] = '\0';
				for (j = d; j >= i + 3; j --) {
					curr->text[j] = curr->text[j - 2];
				}
				curr->text[i] = '<';
				curr->text[i+1] = 'b';
				curr->text[i+2] = '>';
				p = p + 2;
				curr->text = (char *) realloc(curr->text, strlen(curr->text) + 9);
				assert(curr->text != NULL);
				d = strlen(curr->text) + 3;
				curr->text[d] = '\0';
				curr->text[d + 1] = '\0';
				curr->text[d + 2] = '\0';
				curr->text[d + 3] = '\0';
				curr->text[d + 4] = '\0';
				for (j = d; j >= p + 4; j --) {
					curr->text[j] = curr->text[j - 3];
				}
				curr->text[p] = '<';
				curr->text[p+1] = '/';
				curr->text[p+2] = 'b';
				curr->text[p+3] = '>';
				i = p + 1;
			}
		}
	}
}
// this function used for testing the connection of a TB  
static int isvalidTB(TB tb) {
	Line curr = tb->first;
	int countup = 0, countdown = 0;
	while (curr != NULL) {
		if (countup != curr->line_num) {
			return FALSE;
		}
		countup++;
		curr = curr->next;
	}
	curr = tb->last;
	while (curr != NULL) {
		countdown++;
		curr = curr->prev;
	}
	if (countup != countdown || countup != linesTB(tb)) {
		return FALSE;
	}
	return TRUE;
}

// undo function


void undoTB(TB tb) {
	//printf("it is the undo stack\n");
	assert(tb != NULL);
	//printf("%d ", tb->undo->size);
	// printf("%d\n", tb->redo->size);
	if (isempty(tb->undo)) {
		tb->last_call = UNDO;
		return;
	}
	if (tb->last_call == UNREVERSABLE) {
		while(!isempty(tb->undo)) {
			pop(tb->undo);
		}
		return;
	}
	TB tb_new;
	struct node *p = tb->undo->first;
	if (p->command_code == DELETE) {
		if (strcmp(p->str, "") == 0) {
		    pop(tb->undo);
		    tb->last_call = UNDO;
			push(tb->redo, "", 0, 0, DELETE);
			return;
		}
		tb_new = newTB(p->str);
		tb->process = 0;
		mergeTB(tb, p->from, tb_new);
		pop(tb->undo);
		tb->last_call = UNDO;
		return;
	}
	if (p->command_code == MERGE) {
		if (strcmp(p->str, "") == 0) {
			pop(tb->undo);
			push(tb->redo, "", 0, 0, MERGE);
			tb->last_call = UNDO;
			return;
		}
		tb->process = 0;
		if (linesTB(tb) != 0) {
			deleteTB(tb, p->from, p->to);
		}
		pop(tb->undo);
		tb->last_call = UNDO;
		return;
	}
}

void redoTB(TB tb) {
	//printf("it is the undo stack\n");
	assert(tb != NULL);
	if (isempty(tb->redo)) {
		tb->last_call = REDO;
		return;
	}
	if (tb->last_call == OTHER || tb->last_call == UNREVERSABLE) {
		// printf("we free up the redo stack");
		while(!isempty(tb->redo)) {
			pop(tb->redo);
		}
		return;
	}
	TB tb_new;
	struct node *p = tb->redo->first;
	if (p->command_code == DELETE) {
		if (strcmp(p->str, "") == 0) {
		    pop(tb->redo);
		    tb->last_call = REDO;
		    push(tb->undo, "", 0, 0, DELETE);
			return;
		}
		tb_new = newTB(p->str);
		tb->process = 1;
		mergeTB(tb, p->from, tb_new);
		pop(tb->redo);
		tb->last_call = REDO;
		return;
	}
	if (p->command_code == MERGE) {
		if (strcmp(p->str, "") == 0) {
		    pop(tb->redo);
		    tb->last_call = REDO;
		    push(tb->undo, "", 0, 0, DELETE);
			return;
		}
		tb->process = 1;
		deleteTB(tb, p->from, p->to);
		pop(tb->redo);
		tb->last_call = REDO;
		return;
	}
}

static int max(int a, int b) {
	return a > b ? a : b;
}
// this function find the LCS of two 2 dimensional string
// the method is the LCS dp formula
static int find_lcs(int *f[], char *str1[], char *str2[], int n1, int n2) {
	int i, j;
	for (i = 0 ; i <= n1; i++) {
		for (j = 0 ; j <= n2; j++) {
			if (i == 0 || j == 0) {
				if (strcmp(str1[i], str2[j]) == 0) {
					f[i][j] = 1;
				} else {
					f[i][j] = 0;
				}
			} else {
				if (strcmp(str1[i], str2[j]) == 0) {
					f[i][j] = f[i - 1][j - 1] + 1;
				} else {
					f[i][j] = max(f[i - 1][j], f[i][j - 1]);
				}
			}
		}
	}
	return f[n1][n2];
}
// this function locate the position of the LCS
static void locate_lcs(int *f[], int state1[], int state2[], char *str1[], 
										char * str2[], int len1, int len2) {
	int i, j;
	if (len1 == 0 || len2 == 0) {
		if (f[len1][len2] == 0) {
			return;
		}
		for (i = 0 ; i <=  len1; i++) {
			for (j = 0 ; j <= len2; j++) {
				if (strcmp(str1[i], str2[j]) == 0) {
					state1[len1] = SAVE;
					state2[len2] = SAVE;
					return;
				}
			}
		}
	}
	
//	printf("%d %d\n", len1, len2);
/*	if (len1 == 1 || len2 == 1) {
		if (f[len1][len2] == 0) {
			return;
		} 
		
		if (f[len1][len2] == 1) {
			for (i = 0 ; i <=  len1; i++) {
				for (j = 0 ; j <= len2; j++) {
					if (strcmp(str1[i], str2[j]) == 0) {
						state1[len1] = SAVE;
						state2[len2] = SAVE;
						return;
					}
				}
			}
		}
		
		if (f[len1][len2] == 2) {
			for (i = 0 ; i <=  len1; i++) {
				for (j = 0 ; j <= len2; j++) {
					if (strcmp(str1[i], str2[j]) == 0 && f[i][j] == 2) {
						state1[len1] = SAVE;
						state2[len2] = SAVE;
						locate_lcs(f, state1, state2, str1, str2, len1 - 1, len2 - 1);
					}
				}
			}
		}
	}*/
	if (f[len1][len2] == f[len1 - 1][len2 - 1] + 1 && strcmp(str1[len1], str2[len2]) == 0) {
		state1[len1] = SAVE;
		state2[len2] = SAVE;
		locate_lcs(f, state1, state2, str1, str2, len1 - 1, len2 - 1);
	} else if (f[len1][len2] == f[len1 - 1][len2]) {
		locate_lcs(f, state1, state2, str1, str2, len1 - 1, len2);
	} else {
		locate_lcs(f, state1, state2, str1, str2, len1, len2 - 1);
	}
}
// the strategy might be the one with least operations
// find the LCS first and delete the tb1 to lcs, then put it back
char *diffTB (TB tb1, TB tb2) {
	char **str1 = (char **) malloc(sizeof(char *) * (linesTB(tb1) + 1));
	char **str2 = (char **) malloc(sizeof(char *) * (linesTB(tb2) + 1));
	int **f = (int **) malloc(sizeof(int *) * (linesTB(tb1) + 1));
	int *state1 = (int *) malloc(sizeof(int) * (linesTB(tb1) + 1));
	int *lines1 = (int *) malloc(sizeof(int) * (linesTB(tb1) + 1));
	int *state2 = (int *) malloc(sizeof(int) * (linesTB(tb2) + 1));
	int *lines2 = (int *) malloc(sizeof(int) * (linesTB(tb2) + 1));
	char *str = (char *) calloc(2, sizeof(char));
	char *tmp = (char *) calloc(2, sizeof(char));
	// check the memory allocation successfully or not
	assert(tmp != NULL);
	assert(f != NULL);
	assert(state1 != NULL);
	assert(state2 != NULL);
	assert(lines1 != NULL);
	assert(lines2 != NULL);
	assert(str1 != NULL);
	assert(str2 != NULL);
	int i, j, k;
	str[0] = '\0';
	Line curr = tb1->first;
	for (i = 0 ; i < linesTB(tb1); i++) {
		str1[i] = strdup(curr->text);
		lines1[i] = i;
		state1[i] = TMP;
		f[i] = (int *) calloc((linesTB(tb2) + 2), sizeof(int));
		curr = curr->next;
	}
	curr = tb2->first;
	for (i = 0 ; i < linesTB(tb2); i++) {
		str2[i] = strdup(curr->text);
		lines2[i] = i;
		state2[i] = TMP;
		curr = curr->next;
	}
	// case for the line tb1 and tb2 are neither 0, we find and locate the LCS
	if (linesTB(tb1) != 0 && linesTB(tb2) != 0) {
		find_lcs(f, str1, str2, linesTB(tb1) - 1, linesTB(tb2) - 1);
//	printf("the lcs is %d\n", len);
	
		locate_lcs(f, state1, state2, str1, str2, linesTB(tb1) - 1, linesTB(tb2) - 1);
	}
	
	// deal with the deletion first
	for (i = 0 ; i < linesTB(tb1); i++) {
		if (state1[i] == TMP) {
			tmp = realloc(tmp, 14 * sizeof(char));
			sprintf(tmp, "%d", lines1[i]);
			str = realloc(str, (strlen(str) + strlen(tmp) + 5) * sizeof(char));
			k = strlen(str);
			str[k] = '-';
			str[k+1] = ',';
			str[k+2] = '\0';
			strcat(str, tmp);
			str[strlen(str)] = '\0';
			str[strlen(str) + 1] = '\0';
			str[strlen(str)] = '\n';
			free(tmp);
			state1[i] = REMOVE;
			k = 0;
			for (j = 0 ; j < linesTB(tb1); j++) {
				if (state1[j] != REMOVE) {
					lines1[j] = k;
					k++;
				}
			}
			tmp = (char *) calloc(2, sizeof(char));
		}		
	}
	// deal with the putback lines missing in tb2 
	for (i = 0 ; i < linesTB(tb2); i++) {
		if (state2[i] == TMP) {
			tmp = realloc(tmp, 14 * sizeof(char));
			sprintf(tmp, "%d", lines2[i]);
			str = realloc(str, (strlen(str) + strlen(str2[i]) + strlen(tmp) + 5) * sizeof(char));
			k = strlen(str);
			str[k] = '+';
			str[k+1] = ',';
			str[k+2] = '\0';
			strcat(str, tmp);
			str[strlen(str)] = '\0';
			str[strlen(str) + 1] = '\0';
			str[strlen(str)] = ',';
			strcat(str, str2[i]);
			str[strlen(str) + 1] = '\0';
			str[strlen(str)] = '\0';
			str[strlen(str)] = '\n';
			free(tmp);
			tmp = (char *) calloc(2, sizeof(char));
		}		
	}
	
	//printf("%s\n", str);
	
	for (i = 0; i < linesTB(tb1); i++) {
		free(f[i]);
		free(str1[i]);
	}
	
	for (i = 0; i < linesTB(tb2); i++) {
		free(str2[i]);
	}
	// free all the memories that are malloc
	free(tmp);
	free(f);
	free(str1);
	free(str2);
	free(lines1);
	free(state1);
	free(lines2);
	free(state2);
	return str;
}


void whiteBoxTests() {
	new_dump_test();
//	printf("pass the new_dump_test\n");
	line_test();
//	printf("pass the line test\n");
	prefix_test();
//	printf("pass the prefix test\n");
	delete_test();
//	printf("pass the delete test\n");
	merge_test();
//	printf("pass the merge test\n");
	paste_test();
//	printf("pass the paste test\n");
	cut_test();
//	printf("pass the cut test\n");
	search_test();
//	printf("pass the search test\n");
	richText_test();
//	printf("pass the rickText test\n");
//	diff_test();
	
}
/*
static void diff_test() {
	// test case 1 
	char str1_1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
	char str1_2[] = "I am line 3 - 2\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 0\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 5\n\n";
	char *s;
	TB tb1 = newTB(str1_1), tb2 = newTB(str1_2);
	s = diffTB(tb1, tb2);
	printf("the operation from:\n");
	printf("%s\n", str1_1);
	printf("to\n");
	printf("%s is\n", str1_2);
	printf("%s\n", s);
	
	free(s);
	releaseTB(tb1);
	releaseTB(tb2);
	// test case 2
	char str2_1[] = "I am line 3 - 5\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 0\n"
				  "I am line 3 - 4\n\n";
	char str2_2[] = "I am line 3 - 2\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 0\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 5\n\n";
	tb1 = newTB(str2_1);
	tb2 = newTB(str2_2);
	s = diffTB(tb1, tb2);
	printf("the operation from:\n");
	printf("%s\n", str2_1);
	printf("to\n");
	printf("%s is\n", str2_2);
	printf("%s\n", s);
	
	free(s);
	releaseTB(tb1);
	releaseTB(tb2);
	
	// test case 3
	char str3_1[] = "I am line 3 - 5\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 3\n\n";
	char str3_2[] = "I am line 3 - 2\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 0\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 5\n\n";
	tb1 = newTB(str3_1);
	tb2 = newTB(str3_2);
	s = diffTB(tb1, tb2);
	printf("the operation from:\n");
	printf("%s\n", str3_1);
	printf("to\n");
	printf("%s is\n", str3_2);
	printf("%s\n", s);
	
	free(s);
	releaseTB(tb1);
	releaseTB(tb2);
}*/

static void richText_test() {
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
				 "__\n"
				 "#\n";
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
				 "__\n"
				 "#\n";
    tb1 = newTB(str1);
	formRichText(tb1);
	char *s1 = dumpTB(tb1, FALSE);
	//printf("%s", s1);
	assert(strcmp(s1, ans) == 0);
	free(s1);
	releaseTB(tb1);
}

static void search_test() {
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
	free_node(l);
	releaseTB(tb1);
}

static void cut_test() {
	char str1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
    char cut_front_1[] = 
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
    char cut_front_2[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n";
    char cut_rear_1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n";
    char cut_rear_2[] = 
				  "I am line 3 - 4\n\n";
	char cut_centre_1[] = "I am line 3 - 0\n\n";
	char cut_centre_2[] = "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n";			  
    char *s1, *s2;
    TB tb1, tb2;
    tb1 = newTB(str1);
    tb2 = cutTB(tb1, 0, 1); // check cut in the front
    s1 = dumpTB(tb1, FALSE);
    s2 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, cut_front_1) == 0);
    assert(strcmp(s2, cut_front_2) == 0);
    assert(isvalidTB(tb1));
    assert(isvalidTB(tb2));
    free(s1);
    free(s2);
    undoTB(tb1);
    releaseTB(tb2);
    tb2 = cutTB(tb1, 4, 5);  // check cut at the back
    s1 = dumpTB(tb1, FALSE);
    s2 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, cut_rear_1) == 0);
    assert(strcmp(s2, cut_rear_2) == 0);
    assert(isvalidTB(tb1));
    assert(isvalidTB(tb2));
    free(s1);
    free(s2);
    undoTB(tb1);
    releaseTB(tb2);
    tb2 = cutTB(tb1, 1, 4);  // check cut in the centre
    s1 = dumpTB(tb1, FALSE);
    s2 = dumpTB(tb2, FALSE);
    assert(strcmp(s1, cut_centre_1) == 0);
    assert(strcmp(s2, cut_centre_2) == 0);
    assert(isvalidTB(tb1));
    assert(isvalidTB(tb2));
    free(s1);
    free(s2);
    undoTB(tb1);
    releaseTB(tb2);
    // check the out of range cases
    tb2 = cutTB(tb1, 1, 0);
    s1 = dumpTB(tb1, FALSE);
    assert(strcmp(s1, str1) == 0);
    assert(tb2 == NULL);
    free(s1);
//  tb2 = cutTB(tb1, -1, 0);  // from < 0
//  tb2 = cutTB(tb1, 1, 6);   // to >= lines
    releaseTB(tb1);
    releaseTB(tb2);
}

static void paste_test() {
	TB tb1, tb2, tb3;
	char *s1;
	char str1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
	char str2[] = "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n";
	char str3[] = "";
	char paste_front[] = 
	              "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n"
	              "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
	char paste_rear[] = 
	       		  "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n"
                  "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n";
    char paste_centre[] = 
                  "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				   "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
	              
	tb1 = newTB(str1);
	tb2 = newTB(str2);
	tb3 = newTB(str3);
	pasteTB(tb1, 0, tb2); // paste to the first line
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, paste_front) == 0);
	free(s1);
	assert(isvalidTB(tb1));
	pasteTB(tb1, 1, tb3); // paste an empty tb
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, paste_front) == 0);
	free(s1);
	assert(isvalidTB(tb1));
	undoTB(tb1);
	undoTB(tb1);   // recover tb1 to the original one
	assert(isvalidTB(tb2));  // tb2 should not be changed
//	pasteTB(tb1, -1, tb2);  // out of range case
//  pasteTB(tb1, 7, tb2);  // out of range case
	pasteTB(tb1, 6, tb2);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, paste_rear) == 0);  // append case
	free(s1);
	assert(isvalidTB(tb1));
	assert(isvalidTB(tb2));   // tb2 should not be changed
	undoTB(tb1);
	pasteTB(tb1, 2, tb2);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, paste_centre) == 0);  // paste_centre case
	free(s1);
	assert(isvalidTB(tb1));
	assert(isvalidTB(tb2)); 
	undoTB(tb1);
	deleteTB(tb1, 0, 5);
	pasteTB(tb1, 0, tb3);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str3) == 0); // paste empty to empty case do nothing
	assert(isvalidTB(tb2)); 
	free(s1);
	pasteTB(tb1, 0, tb2);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str2) == 0);  // paste to an empty case
	free(s1);
	assert(isvalidTB(tb1));
	assert(isvalidTB(tb2)); 
//	undoTB(tb1);
//	pasteTB(tb1, 1, tb2);  // out of range case
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
}

static void merge_test() {
	TB tb1, tb2, tb3;
	char *s1;
	char str1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
	char str2[] = "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n";
	char str3[] = "";
	char merge_front[] = 
	              "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n"
	              "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
	char merge_rear[] = 
	       		  "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n"
                  "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n";
    char merge_centre[] = 
                  "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				   "\nI am line *4 - 0 *\n"
				  "I am line __4_ - 1\n"
				  "# I am line 4 - 2\n\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n";
	              
	tb1 = newTB(str1);
	tb2 = newTB(str2);
	tb3 = newTB(str3);
	mergeTB(tb1, 0, tb2); // merge to the first line
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, merge_front) == 0);
	free(s1);
	assert(isvalidTB(tb1));
	mergeTB(tb1, 1, tb3); // merge an empty tb
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, merge_front) == 0);
	free(s1);
	assert(isvalidTB(tb1));
	undoTB(tb1);
	undoTB(tb1);   // recover tb1 to the original one
	tb2 = newTB(str2);
//	mergeTB(tb1, -1, tb2);  // out of range case
//  mergeTB(tb1, 7, tb2);  // out of range case
	mergeTB(tb1, 6, tb2);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, merge_rear) == 0);  // append case
	free(s1);
	assert(isvalidTB(tb1));
	undoTB(tb1);
	tb2 = newTB(str2);
	mergeTB(tb1, 2, tb2);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, merge_centre) == 0);  // merge_centre case
	free(s1);
	assert(isvalidTB(tb1));
	undoTB(tb1);
	deleteTB(tb1, 0, 5);
	tb2 = newTB(str3);
	mergeTB(tb1, 0, tb2);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str3) == 0); // merge empty to empty case do nothing
	free(s1);
	tb2 = newTB(str2);
	mergeTB(tb1, 0, tb2);
	s1 = dumpTB(tb1, FALSE);
	assert(strcmp(s1, str2) == 0);  // merge to an empty case
	free(s1);
	undoTB(tb1);
	tb2 = newTB(str2);
//	mergeTB(tb1, 1, tb2);  // out of range case
	releaseTB(tb1);
	releaseTB(tb2);
	// releaseTB(tb3);
}

static void new_dump_test() {
	TB tb1, tb2, tb3, tb4;
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
	tb1 = newTB(str1);
	s1 = dumpTB(tb1, FALSE);
	assert(isvalidTB(tb1)); // check the connection of tb1
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
	assert(isvalidTB(tb2)); // check the connection of tb2
	// check the dump result without showLine number
	assert(strcmp(s1, str2) == 0); 
	free(s1);
	tb3 = newTB(str3);
	s1 = dumpTB(tb3, TRUE);
	assert(isvalidTB(tb3)); // check the valid of tb3(empty)
	// check the dump result on an empty TB
	assert(strcmp(s1, str3) == 0); 
	free(s1);
	tb4 = newTB(str4);
	s1 = dumpTB(tb4, TRUE);
	assert(isvalidTB(tb1)); // check the connection of tb4
	// check the dump result with showLine number
	// and make sure that \n is on a newline
	assert(strcmp(s1, ans_str4) == 0); 
	free(s1);
	// clean up the TBs and finish the testing
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
	releaseTB(tb4);
}

static void line_test() {
	TB tb1, tb2, tb3, tb4;
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
	tb1 = newTB(str1);
	tb2 = newTB(str2);
	tb3 = newTB(str3);
	tb4 = newTB(str4);
	// check the linesTB function
	assert(linesTB(tb1) == 6);   // case with \n\n
	assert(linesTB(tb2) == 5);  // case with \n\n
	assert(linesTB(tb3) == 0);  // case for empty TB
	assert(linesTB(tb4) == 5);  // normal case
	assert(isvalidTB(tb1));
	assert(isvalidTB(tb2));
	assert(isvalidTB(tb3));
	assert(isvalidTB(tb4));
	// clean up the TBs and finish the testing
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
	releaseTB(tb4);
}

static void prefix_test() {
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
	char str3[] = "";
	tb1 = newTB(str1);
	tb2 = newTB(str2);
	tb3 = newTB(str3);
	// check the addPrefix function
	addPrefixTB(tb1, 0, 3, "hello-"); // normal case
	s1 = dumpTB(tb1, TRUE);
	assert(strcmp(ans_str1_prefix, s1) == 0);
	assert(isvalidTB(tb1));
	free(s1);
	addPrefixTB(tb2, 3, 4, "hi- "); // case with prefix to \n
	s1 = dumpTB(tb2, TRUE);
	assert(strcmp(ans_str2_prefix, s1) == 0);
	assert(isvalidTB(tb2));
	free(s1);
//	addPrefixTB(tb3, 0, 0, "hello-");  // this should abort
	// clean up the TBs and finish the testing
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
}

static void delete_test() {
	TB tb1;
	char *s1;
	char str1[] = "I am line 3 - 0\n"
				  "I am line 3 - 1\n"
				  "I am line 3 - 2\n"
				  "I am line 3 - 3\n"
				  "I am line 3 - 4\n\n"
				  "I am line 3 - 6\n"
				  "I am line 3 - 8\n";
	char delete_front[] = "I am line 3 - 2\n"
				  		  "I am line 3 - 3\n"
				          "I am line 3 - 4\n\n"
				          "I am line 3 - 6\n"
				          "I am line 3 - 8\n";
    char delete_rear[] =   "I am line 3 - 0\n"
				  		   "I am line 3 - 1\n"
				           "I am line 3 - 2\n"
				           "I am line 3 - 3\n"
				           "I am line 3 - 4\n\n";
	char delete_middle[] = "I am line 3 - 0\n"
				  		   "I am line 3 - 1\n"
				  		   "\n"
				           "I am line 3 - 6\n"
				           "I am line 3 - 8\n";			     
	char delete_all[] = "";
	tb1 = newTB(str1);
	
	// check the deleteTB function
//	deleteTB(tb1, 1, 0);  // from > to abort
//	deleteTB(tb1, -1, 2); // invalid pos
//	deleteTB(tb1, 1, 8);  // invalid pos 
   	deleteTB(tb1, 0, 1);  // check delete line 0
   	s1 = dumpTB(tb1, FALSE);
   	assert(strcmp(s1, delete_front) == 0);  // delete include first node
   	free(s1);
   	undoTB(tb1); // recover to str1
   	deleteTB(tb1, 6, 7);
   	s1 = dumpTB(tb1, FALSE);
   	assert(strcmp(s1, delete_rear) == 0); // delete include last node
 	free(s1);
 	undoTB(tb1);  // recover to str1
   	deleteTB(tb1, 2, 4);
   	s1 = dumpTB(tb1, FALSE);
   	assert(strcmp(s1, delete_middle) == 0); // delete include last node
 	free(s1);
 	undoTB(tb1); // recover to str1
 	deleteTB(tb1, 0, 7);
   	s1 = dumpTB(tb1, FALSE);
   	assert(strcmp(s1, delete_all) == 0); // delete all case
   	assert(linesTB(tb1)  == 0);
 	free(s1);
 //	deleteTB(tb1, 0, 0); // try to delete an empty one // uncomment to run
 	releaseTB(tb1);  	
}
