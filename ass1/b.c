#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "textbuffer.h"

typedef struct entry {
    char *buf;
    int len;
    struct entry *next;
}entry;

typedef struct textbuffer {
    entry *first;
    entry *last;
    int lines;
}textbuffer;

/* Allocate a new textbuffer whose contents is initialised with the text given
 * in the array.
 */
TB newTB (char text[]) {
    int i, j, tmp;
    TB table;
    entry *et;

    if (text == NULL || strlen(text) == 0) { return NULL; }
    if (text[strlen(text) - 1] != '\n') { return NULL; }
    table = (TB)malloc(sizeof(textbuffer));
    table->first = table->last = NULL;
    table->lines = 0;

    i = j = 0;
    while (text[j] != '\0') {
        tmp = j;
        while (text[j] != '\0' && text[j] != '\n') { j++; }
        if (text[j] == '\0') { break; }
        j++;
        i = tmp;

        et = (entry*)malloc(sizeof(entry));
        et->buf = (char*)malloc(j - i + 1);
        strncpy(et->buf, text + i, j - i);
       
        et->buf[j-i] = '\0';
       
        et->len = j - i;
        et->next = NULL;
        if (table->first == NULL) {
            table->first = table->last = et;
        } else {
            table->last->next = et;
            table->last = et;
        }
        table->lines++;
    }
    return table;
}

/* Free the memory occupied by the given textbuffer.  It is an error to access
 * the buffer afterwards.
 */
void releaseTB (TB tb) {
    entry *et, *tmp;
   
    if (!tb) { return; }
    et = tb->first;
    while (et) {
        free(et->buf);
        tmp = et;
        et = tmp->next;
        free(tmp);
    }
   
    tb->lines = 0;
    tb->first = tb->last = 0;
    tb = NULL;
    free(tb);
}

/* Allocate and return an array containing the text in the given textbuffer.
 * add a prefix corrosponding to line number iff showLineNumbers == TRUE
 */
char *dumpTB (TB tb, int showLineNumbers){
    entry *et;
    int size, line;
    char *buf;

    if (!tb) { return NULL; }
    size = 0;
    et = tb->first;
    while (et) {
        size += et->len;
        et = et->next;
    }
    if (showLineNumbers) { size += tb->lines * 3; }
    else { size += 1; }

    buf = (char*)malloc(size);
    strcpy(buf, "");
    line = 1;
    et = tb->first;
    while (et) {
        if (showLineNumbers) {
        	// memcpy
        	// free
        	// malloc
        	// memcpy
        // realloc(buf, total size)
            sprintf(buf + strlen(buf), "%d. ", line);
            strcat(buf + strlen(buf), et->buf);
        } else {
            strcat(buf + strlen(buf), et->buf);
        }
        et = et->next;
        line++;
    }
    return buf;
}

/* Return the number of lines of the given textbuffer.
 */
int linesTB (TB tb){
    if (!tb) { return -1; }
    else { return tb->lines; }
}

/* Add a given prefix to all lines between pos1 and pos2
 *
 * - The program is to abort() with an error message if line 'pos1' or line
 *   'pos2' is out of range.  The first line of a textbuffer is at position 0.
 */
void addPrefixTB (TB tb, int pos1, int pos2, char* prefix){
    int index, prefixLen;
    entry *et, *newEntry;

    if (pos1 > pos2 || pos1 < 0 || pos2 >= tb->lines ) { abort(); }
    if (!tb || !prefix) { return; }

    index = 0;
    et = tb->first;

    if (pos1 != 0) {
        while (index + 1 != pos1) {
            et = et->next;
            index++;
        }
    }

    prefixLen = strlen(prefix);
    index = pos1;
    newEntry = (entry*)malloc(sizeof(entry));
    while (index != pos2 + 1) {
        // newEntry = (entry*)malloc(sizeof(entry));
        if (index == pos1 && pos1 == 0) {
            newEntry->len = prefixLen + et->len + 1;
            newEntry->buf = (char*)malloc(newEntry->len);
            strcpy(newEntry->buf, prefix);
            strcat(newEntry->buf, et->buf);
            newEntry->next = et->next;
            free(et->buf);
            free(et);
            tb->first = newEntry;
            et = newEntry;
        } else {
            newEntry->len = prefixLen + et->next->len + 1;
            newEntry->buf = (char*)malloc(newEntry->len);
            strcpy(newEntry->buf, prefix);
            strcat(newEntry->buf, et->next->buf);
            newEntry->next = et->next->next;
            free(et->next->buf);
            free(et->next);
            et->next = newEntry;
        }
        if (index == pos1 && pos1 == 0) {
            index++;
        } else {
            et = et->next;
            index++;
        }
        newEntry = (entry*)malloc(sizeof(entry));
    }
    if (pos2 == tb->lines - 1) {
        tb->last = newEntry;
    }
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
    entry *et;
    int index;

    // if (pos < 0 || pos >= tb1->lines) { abort(); }
    if (pos < 0 ) { abort(); }
    if (pos >= tb1->lines - 1) {
        tb1->last->next = tb2->first;
        tb1->last = tb2->last;
        tb1->lines += tb2->lines;
        tb2->first = tb2->last = NULL;
        tb2->lines = 0;
    } else if (pos == 0) {
        mergeTB(tb2, tb2->lines - 1, tb1);
        tb1->first = tb2->first;
        tb1->last = tb2->last;
        tb1->lines = tb2->lines;
        tb2->first = tb2->last = NULL;
        tb2->lines = 0;
    } else {
        index = 0;
        et = tb1->first;
        while (index + 1 != pos) {
            et = et->next;
            index++;
        }
        tb2->last->next = et->next;
        et->next = tb2->first;
        tb2->first = tb2->last = NULL;
        tb2->lines = 0;
    }
    tb1->lines += tb2->lines;
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
    TB tb;
    char *buf;
    int size;
    entry *et;

    size = 0;
    et = tb2->first;
    while (et) {
        size += et->len;
        et = et->next;
    }
    buf = (char*)malloc(size + 1);
    strcpy(buf, "");
    et = tb2->first;
    while (et) {
        strcat(buf, et->buf);
        et = et->next;
    }
    tb = newTB(buf);
    free(buf);
    mergeTB(tb1, pos, tb);
}

/* Cut the lines between and including 'from' and 'to' out of the textbuffer
 * 'tb'.
 *
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The program is to abort() with an error message if 'from' or 'to' is out
 *   of range.
 */
TB cutTB (TB tb, int from, int to){
    TB ret;
    char *buf;
    int size, index;
    entry *et, *tmp;

    if (from < 0 || to >= tb->lines) { abort(); }
    et = tb->first;
    size = index = 0;
    while (index != from) {
        et = et->next;
        index++;
    }
    tmp = et;
    while (index != to + 1) {
        size += et->len;
        index++;
    }

    buf = (char*)malloc(size + 1);
    strcpy(buf, "");
    index = from;
    while (index != to + 1) {
        strcat(buf, tmp->buf);
        tmp = tmp->next;
        index++;
    }
    ret = newTB(buf);
    free(buf);
    deleteTB(tb, from, to);

    return ret;
}

/*  Return a linked list of Match nodes of all the matches of string search
 *  in tb
 *
 * - The textbuffer 'tb' will remain unmodified.
 * - The user is responsible of freeing the returned list
 */
Match searchTB (TB tb, char* search){
    Match mth, last, tmp;
    entry *et;
    int i, length, line;

    if (!search) { return NULL; }
    last = mth = NULL;
    length = strlen(search);
    line = 1;
    et = tb->first;
    while (et) {
        if (et->len < length) {
            et = et->next;
            continue;
        }
        for (i = 0; i < et->len - length; i++) {
            if (strncmp(search, et->buf + i, length) == 0) {
                tmp = (Match)malloc(sizeof(matchNode));
                tmp->lineNumber = line;
                tmp->charIndex = i;
                tmp->next = NULL;
                if (!mth) {
                    mth = last = tmp;
                } else {
                    last->next = tmp;
                    last = tmp;
                }
            }
        }
        et = et->next;
        line++;
    }

    return mth;
}

/* Remove the lines between and including 'from' and 'to' from the textbuffer
 * 'tb'.
 *
 * - The program is to abort() with an error message if 'from' or 'to' is out
 *   of range.
 */
void deleteTB (TB tb, int from, int to){
    entry *et, *tmp, *save;
    int index;

    if (from < 0 || to >= tb->lines) { abort(); }
    if (from == 0 && to == tb->lines - 1) {
        releaseTB(tb);
    } else {
        et = tb->first;
        if (from == 0) {
            index = 0;
            while (index != to + 1) {
                tmp = et->next;
                free(et->buf);
                free(et);
                et = tmp;
                index++;
            }
            tb->first = et;
        } else if (to == tb->lines - 1) {
            index = 0;
            while (index + 1 != from) {
                et = et->next;
                index++;
            }
            tb->last = et;
            tb->last->next = NULL;
            et = et->next;
            while (et) {
                tmp = et->next;
                free(et->buf);
                free(et);
                et = tmp;
            }
        } else {
            index = 0;
            while (index + 1 != from) {
                et = et->next;
                index++;
            }
            save = et;
            et = et->next;
            while (index != to) {
                tmp = et->next;
                free(et->buf);
                free(et);
                et = tmp;
                index++;
            }
            save->next = et;
        }
        tb->lines -= (to - from + 1);
    }
}


/* Search every line of tb for each occurrence of a set of specified subsitituions
 * and alter them accordingly
 *
 * refer to spec for table.
 */
void formRichText (TB tb){
    TB table;
    char *buf;
    entry *et;
    int size, i, j;
    int flag = 0;
    char notation;

   
    et = tb->first;
    size = 0;
    while (et) {
        size += et->len;
        et = et->next;
    }

    // printf("d");
    buf = (char*)malloc(size * 2);
    // buf = (char*)malloc(1000);
    strcpy(buf, "");
  
    et = tb->first;
    while(et){
       if(et->buf[0] == '#'){
            buf = strcat(buf, "<h1>");
            buf = strcat(buf, et->buf+1);
            buf[strlen(buf) - 1] = '<';
            buf = strcat(buf, "/h1>\n");
           
        }
        else{
            i = 0;
            while(i < et->len){
                if(et->buf[i] == '*' || et->buf[i] == '_'){
                    notation = et->buf[i];
                    for(j = i + 1; j < et->len; j++){
                        if(et->buf[j] == notation){
                            if(i + 1 == j) break;
                            if(notation == '*'){
                                buf = strcat(buf, "<b>");
                                int buf_len = strlen(buf);
                                int k = i + 1;
                                while(k < j){
                                    buf[buf_len++] = et->buf[k];
                                    k++;
                                }
                                buf = strcat(buf, "</b>");
                            }else{
                                buf = strcat(buf, "<i>");
                                int buf_len = strlen(buf);
                                int k = i + 1;
                                while(k < j){
                                    buf[buf_len++] = et->buf[k];
                                    k++;
                                }
                                buf = strcat(buf, "</i>");
                            }
                            flag = 1;
                            i = j;
                            break;
                       }
                   }
               }
               if(flag == 0){
                   //printf("%d\n", et->buf[i]);
                   //printf("%d\n", strlen(buf));
                   buf[strlen(buf)] = et->buf[i];
               }else{
                   flag = 0;
               }
               i++;
           }
       }
       et = et->next;
    }

   
    table = newTB(buf);
    tb->first = table->first;
    tb->last = table->last;
    tb->lines = table->lines;
    free(buf);
}

/* Your whitebox tests
 */
void whiteBoxTests() {
    entry *tmp;
    TB tb1, tb2, tb3, tb4, tb5, tb6,
       tb7, tb8, tb9, tb10, tb11, tb12;
    Match mth;
    char *dump;
   
     
   
   
    printf("====== test0 formRichText ======\n");
    /*char text[] = "*some string*lol*\n"
                  "the following word is bolded *i am bolded* and i am _italizised_\n"
                 "#string_stuff_\n"
                 "line 02\n"
                 "*some string\n"
                 "*some string*lol*\n"
                 "*some_string*again_\n"
                 "*some* _string_\n"
                 "some *string_agai                                                 /n_\n"
                 "some#string*once_again*\n"
                 "#string_stuff_\n"
                 "*some_string_*\n"
                 "#some _string_\n"
                 "**\n";*/
    char text[] =
        "===TB1===\n"
        "the following word is bolded <b>i am bolded</b> and i am <i>italizised</i>\n"
        "isn't html fun!\n"
        "<h1> how to use underscores i.e _hello_</h1>\n"
        "<b>i love _underscores_ </b>\n"
        "**double's should do nothing\n"
        "_<i>double should </i>_sometimes do something    \n";
                
   
    tb12 = newTB(text);
    formRichText(tb12);
    tmp = tb12->first;
   
   
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");


    char str[] = "line 01\n"
                 "line 02\n"
                 "line 03\n"
                 "line 04\n"
                 "line 05\n"
                 "line 06\n"
                 "line 07\n"
                 "line 08\n"
                 "line 09\n"
                 "line 10\n";
   
   
   
    tb1 = newTB(str);
    tb2 = newTB(str);
    tb3 = newTB(str);
    tb4 = newTB(str);
    tb5 = newTB(str);
    tb6 = newTB(str);
    tb7 = newTB(str);
    tb8 = newTB(str);
    tb9 = newTB(str);
    tb10 = newTB(str);

    printf("====== test1 ======\n");
    tmp = tb1->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test2 ======\n");
    addPrefixTB(tb2, 0, 9, "goodnight");
    tmp = tb2->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test3 ======\n");
    addPrefixTB(tb3, 0, 6, "goodnight");
    tmp = tb3->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test4 ======\n");
    addPrefixTB(tb4, 5, 9, "goodnight");
    tmp = tb4->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test5 ======\n");
    addPrefixTB(tb5, 5, 7, "goodnight");
    tmp = tb5->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test6 ======\n");
    deleteTB(tb6, 3, 7);
    tmp = tb6->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test7 ======\n");
    mergeTB(tb7, 3, tb3);
    tmp = tb7->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test8 ======\n");
    mergeTB(tb8, 0, tb4);
    tmp = tb8->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test9 ======\n");
    mergeTB(tb9, 10, tb5);
    tmp = tb9->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test10 ======\n");
    pasteTB(tb10, 5, tb2);
    tmp = tb10->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test11 ======\n");
    tb11 = cutTB(tb10, 1, 3);
    tmp = tb11->first;
    while (tmp) {
        printf("%s", tmp->buf);
        tmp = tmp->next;
    }
    printf("\n");

    printf("====== test12 ======\n");
    mth = searchTB(tb9, "night");
    while (mth) {
        printf("lineNumber: %d\n", mth->lineNumber);
        printf("charIndex: %d\n", mth->charIndex);
        mth = mth->next;
    }
    printf("\n");

    printf("====== test13 ======\n");
    dump = dumpTB(tb1, 1);
    printf("%s", dump);
    printf("\n");
    free(dump);
   
    printf("====== test14 ======\n");
    dump = dumpTB(tb1, 0);
    printf("%s", dump);
    printf("\n");
    free(dump);

    releaseTB(tb1);
    // because of mergeTB();
    releaseTB(tb2);
    releaseTB(tb3);
    releaseTB(tb4);
    releaseTB(tb5);
    releaseTB(tb6);
    releaseTB(tb7);
    releaseTB(tb8);
    releaseTB(tb9);
    releaseTB(tb10);
    releaseTB(tb11);
    free(mth);
}
