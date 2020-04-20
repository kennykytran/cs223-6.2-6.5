//
//  tree.c
//  tree
//
//  Created by William McCarthy on 134//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "slist.h"
#include "tree.h"
#include "utils.h"
#define PRINTPREVIOUS_YES 1
#define PRINTPREVIOUS_NO 0
#define PRINTPREVIOUS_NEWGROUP 2

//-------------------------------------------------
tnode* tnode_create(const char* word) {
  tnode* p = (tnode*)malloc(sizeof(tnode));
  p->word = strdup(word);    // copy of word allocated on heap
  p->count = 1;
  slist* lines = slist_create();//list of line numbers
  p->left = NULL;
  p->right = NULL;
  
  return p;
}

//====================================================================
void tnode_delete(tnode* p) {
    free((void*)p->word);
    free(p);
}

//====================================================================
tree* tree_create(void) {
  tree* p = (tree*)malloc(sizeof(tree));
  p->root = NULL;
  p->size = 0;
  
  return p;
}

//====================================================================
static void tree_deletenodes(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_deletenodes(t, p->left);
  tree_deletenodes(t, p->right);
  tnode_delete(p);
  t->size--;
}

//====================================================================
void tree_clear(tree* t) {
  tree_delete(t);
  t->root = NULL;
  t->size = 0;
}

//====================================================================
void tree_delete(tree* t) { tree_deletenodes(t, t->root); }

//====================================================================
bool tree_empty(tree* t)  { return t->size == 0; }

//====================================================================
size_t tree_size(tree* t) { return t->size; }

//====================================================================
static tnode* tree_addnode(tree* t, tnode** p, const char* word) {
  int compare;
  
  if (*p == NULL) {
    *p = tnode_create(word);
  } else if ((compare = strcmp(word, (*p)->word)) == 0) {
    (*p)->count++;
  } else if (compare < 0) { tree_addnode(t, &(*p)->left, word);
  } else {
    tree_addnode(t, &(*p)->right, word);
  }
  return *p;
}

//====================================================================
static char* str_process(char* s, char* t) {
  char* p = s;
  char ignore[] = "\'\".,;;?!()/’";
  while (*t != '\0') {
    if (strchr(ignore, *t) == NULL || (*t == '\'' && (p != s || p != s + strlen(s) - 1))) {
      *p++ = tolower(*t);
    }
    ++t;
  }
  *p++ = '\0';
  return s;
}

//====================================================================
tnode* tree_add(tree* t, char* word) {
  char buf[100];
  
  if (word == NULL) { return NULL; }
  str_process(buf, word);
  
  tnode* p = tree_addnode(t, &(t->root), buf);
  t->size++;

  return p;
}

//====================================================================
static void tree_printme(tree* t, tnode* p) {
  if (p->count > 1) { printf("%5d -- ", p->count); }
  else {
    printf("         ");
  }
  printf("%-18s", p->word);
  printf("\n");
}

//====================================================================
static void tree_printnodes(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes(t, p->left);
  tree_printme(t, p);
  tree_printnodes(t, p->right);
}

//====================================================================
static void tree_printnodes_preorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printme(t, p);
  tree_printnodes(t, p->left);
  tree_printnodes(t, p->right);
}

//====================================================================
static void tree_printnodes_postorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes_postorder(t, p->left);
  tree_printnodes_postorder(t, p->right);
  tree_printme(t, p);
}

//====================================================================
static void tree_printnodes_reverseorder(tree* t, tnode* p) {
  if (p == NULL) { return; }
  
  tree_printnodes_reverseorder(t, p->right);
  tree_printme(t, p);
  tree_printnodes_reverseorder(t, p->left);
}

//====================================================================
//void tree_print_levelorder(tree* t);

//====================================================================
void tree_print(tree* t)              { tree_printnodes(t, t->root);               printf("\n"); }

//====================================================================
void tree_print_preorder(tree* t)     { tree_printnodes_preorder(t, t->root);      printf("\n"); }

//====================================================================
void tree_print_postorder(tree* t)    { tree_printnodes_postorder(t, t->root);     printf("\n"); }

//====================================================================
void tree_print_reverseorder(tree* t) { tree_printnodes_reverseorder(t, t->root);  printf("\n"); }

//====================================================================
void tree_test(tree* t) {
  printf("=============== TREE TEST =================================\n");
  printf("\n\nprinting in order...========================================\n");
  tree_print(t);
  printf("end of printing in order...=====================================\n\n");

  /*printf("\n\nprinting in reverse order...================================\n");
  tree_print_reverseorder(t);
  printf("end of printing in reverse order...=============================\n\n");
  printf("tree size is: %zu\n", tree_size(t));
  */
  
  printf("clearing tree...\n");
  tree_clear(t);
  printf("a clearing tree, size is: %zu\n", tree_size(t));
  tree_print(t);
  
  printf("=============== END OF TREE TEST ==========================\n");
}

//====================================================================
tree* tree_from_file(int argc, const char* argv[]) {
  if (argc != 2) { return NULL; }

  FILE* fin;
  const char* filename = argv[1];
  if ((fin = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Could not open file: '%s'\n", filename);
    exit(1);
  }

  char buf[BUFSIZ];
  char delims[] = " \n";
  int size = 0;
  memset(buf, 0, sizeof(buf));

  tree* t = tree_create();
  while (fgets(buf, BUFSIZ, fin)) {
    char* word = strtok(buf, delims);
   // if(isNoise(word)==0){//6.3
    tree_add(t, word);
    ++size;
   // }
    while ((word = strtok(NULL, delims)) != NULL) {
      //if(isNoise(word)==0){//6.3
      tree_add(t, word);
      ++size;
      //}
    }
  }
  printf("%d words added...\n", size);
  fclose(fin);

  return t;
}


//====================================================================
int getch(void);
void ungetch(int);

char buf[BUFSIZ];
int bufp = 0;

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp >= BUFSIZ)
		printf("ungetch: too many characters\n");
	else buf[bufp++] = c;
}

int getword(char *word, int lim) {
    int c;
    char *w = word;
    static int lnBegin = 1;
    static int aSlash = 0;
    int aStar = 0; 

    if(isspace(c = getch())) aSlash = 0;
    while(isspace(c)) {
      if(c == '\n') lnBegin = 1;
      c = getch();
    }

    if(c != EOF) *w++ = c;

    if(c == '#' && lnBegin == 1){
      while((c = getch()) != '\n' && c != EOF);
      return getword(word, lim);}
    
    lnBegin = 0;

    if(c == '\\') aSlash = aSlash ? 0 : 1;

    else if(c == '/' ) {
      if((c = getch()) == '*' && !aSlash) {
        while((c = getch()) != EOF) {
          if(c == '/') {
             if(aStar)
             return getword(word, lim);}
             
		   else if(c == '*' && !aSlash) aStar = 1;
             else if(c == '\\') aSlash = aSlash ? 0 : 1;
             else{
               aStar = 0;
               aSlash = 0;}
         }
      }

    aSlash = 0;
        
    if(c != EOF) ungetch(c);
    }

    else if(c == '\"') {
       if(!aSlash) { --w;
          while((c = getch()) != EOF) {
            if(c == '\"' && !aSlash)
              break;
              else if(c == '\\') aSlash = aSlash ? 0 : 1;
              else aSlash = 0;
              *w++ = c;
          }
          *w = '\0';
            if(c == EOF) return EOF;
            else return getword(word, lim);
        }
        aSlash = 0;
    }

    if(!isalpha(c) && c != '_') {
        *w = '\0';
        if(c != '\\') aSlash = 0;
        return c;}

    aSlash = 0;

    for (; --lim > 0; ++w) {
	  if (!isalnum(*w = getch()) && *w != '_') {
	     ungetch(*w);
		break; }}

    *w = '\0';
    return word[0];
}

void treeprint(struct tnode *p, int n) {
    static int printPrevious = 1;
    static struct tnode *previous;

    if (p != NULL) {
        treeprint(p->left, n);

        /* n is how many characters we should compare, if it is
         * 0 then we just print all of them */
        if (n == 0)
            /* Print all the words. */
            printf("%4d %s\n", p->count, p->word);
        else {
            if (previous != NULL) {
                if (strncmp(previous->word, p->word, n) == 0) {
                    if (printPrevious == PRINTPREVIOUS_NEWGROUP) {
                        /* If we find a new group, add an extra newline to separate the
                         * groups in the output */
                        printf("\n%4d %s\n", previous->count, previous->word);
                        printPrevious = PRINTPREVIOUS_NO;
                    } else if (printPrevious == PRINTPREVIOUS_YES) {
                        /* If we find another word in the current group, just add a single
                         * newline. */
                        printf("%4d %s\n", previous->count, previous->word);
                        printPrevious = PRINTPREVIOUS_NO;
                    }
                    /* We do not print the current word by default as if it doesn't have the same
                     * root as the next value then that would be a mistake. Hence why we need
                     * printPrevious. */
                    printf("%4d %s\n", p->count, p->word);
                } else
                    printPrevious = PRINTPREVIOUS_NEWGROUP;
            }
            /* Keep track of previous word. */
            previous = p;
        }

        treeprint(p->right, n);
    }
}

#define DEFAULT 6

void wordFreq(int argc, const char* argv[])
{
  int countComp;
  if(argc == 2) countComp = DEFAULT;
  else if(argc == 3) countComp = atoi(argv[2]);
  else{
    fprintf(stderr, "Invalid Arguments. \n");
    exit(0);}

  tree* t = tree_from_file(argc, argv);

  treeprint(t->root,countComp);

  tree_delete(t);
}

char* noise[19] = {"a","an","and","are","for","from","in","is","it","not", "of","on","or","that","the","this","to","was","with"};

int isNoise(char* word){
  for(int i =0; i<19; i++){
    if(strcmp(word,noise[i])==0) return 1;}
  return 0;
}
