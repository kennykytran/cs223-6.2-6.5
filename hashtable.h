#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

struct nlist{
  struct nlist * next;
  char*name;
  char*defn;
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];

unsigned hash(char* s){
  unsigned hashval;
  for(hashval = 0; *s != '\0'; s++){hashval = *s + 31 * hashval;}
  return hashval % HASHSIZE;
}

struct nlist *lookup(char *s)
{
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;  /* found */
    return NULL;        /* not found */
}

struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL)  { /* not found */
        np = (struct nlist *) malloc(sizeof(*np));

        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else  /* already there */
        free((void *) np->defn);    /* free the previous defn */

    if ((np->defn = strdup(defn)) == NULL)
        return NULL;

    return np;
}

struct nlist* undef(char *name){
  struct nlist* found;

  found = lookup(name);

  if (found == NULL)
    return NULL;
  else{
      if(found->next != NULL){
        found->next = found->next->next;
      }
      else{
        hashtab[hash(name)] = NULL;
        free((void*) found);
      }
  }
  return found;
}

void undef_test(void) {
  struct nlist *table[4] = {
    (install("key", "value")),
    (install("key1", "value1")),
    (install("key2", "value2")),
    (install("key3", "value3"))
  };

    int i;

    for (i=0; i < 4; i++) {
        printf("%s->%s\n", table[i]->name, table[i]->defn);
    }

    undef("key");
    undef("key3");

    struct nlist *result;

    char *keys[4] = {
            "key",
            "key1",
            "key2",
            "key3"
    };

    for (i = 0; i < 4; i++) {
        if ((result = lookup(keys[i])) == NULL) {
            printf("key not found\n");
        } else {
            printf("%s->%s\n", result->name, result->defn);
        }
    }

}
