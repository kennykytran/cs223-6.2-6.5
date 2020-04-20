#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "slist.h"
#include "utils.h"

snode* snode_create(int data, snode* next){
  snode* p = (snode*)malloc(sizeof(snode));
  
  p-> data= data;
  p-> next= next;
  return p;
}

slist* slist_create(void){
  slist* p = (slist*)malloc(sizeof(slist));
  
  p-> head= NULL;
  p-> tail= NULL;
  p-> size= 0;
  return p;
}

bool slist_empty(slist* list){
  return list-> size == 0;}
size_t slist_size(slist* list){ 
  return list-> size;}

void slist_popfront(slist* list){
  snode *p= list-> head-> next;
  free(list-> head);
  list-> head= p;
  --list-> size;
}

void slist_popback(slist* list){
  snode* p= list->head;
  while(p-> next != list-> tail){
    p= p-> next;}
  p-> next = NULL;
  free(list-> tail);
  list-> tail= p;
}

void slist_pushfront(slist* list, int data){
  snode* p= snode_create(data, list-> head);
  list-> head = p;
  ++list-> size;
}

void slist_pushback(slist* list, int data){
snode* p= snode_create(data, NULL);
snode* i= list->head;
if(i == NULL){
  list-> head= p; 
  ++list ->size;
  return;}
while (i-> next != NULL){
  i= i-> next;}
i-> next= p;
++list-> size;
}

void slist_clear(slist* list){
  while(!slist_empty(list)){
    slist_popfront(list);}
}

void slist_print(slist* list, const char* msg){
  printf("%s", msg);

  snode* p= list-> head;
  while(p != NULL) {
    printf("%d ", p-> data);
    p= p-> next;}
}

int slist_front(slist* list){
  return list-> head-> data;}
int slist_back(slist* list){
  snode* p= list->head;
  while(p-> next != NULL){ 
    p= p-> next;}
  return p-> data;}

//TESTING FUNCTIONS BELOW

static void slist_check_frontback(slist* list, int front, int back) {
  printf("assert front of list is: %d\n", front);
  assert(slist_front(list) == front);
  printf("assert back of list is: %d\n", back);
  assert(slist_back(list) == back);
}

static void slist_test_once(slist* list, int start, int end, int step, void (*fp)(slist*, int)) {
  for (int i = start; i < end; i += step) {
    fp(list, i);
  }
  slist_print(list, "list is created and is...\n");
  printf("list size is: %zu\n", slist_size(list));
  printf("list is empty? %s\n\n", yesorno(slist_empty(list)));

  int firstadded = start;
  int lastadded = start;
  while (lastadded + step < end) {
    lastadded += step;
  }
  if (fp == slist_pushback)  { slist_check_frontback(list, firstadded, lastadded);  }
  if (fp == slist_pushfront) { slist_check_frontback(list, lastadded, firstadded ); }

  slist_clear(list);
  printf("after clearing the list, is the list is now empty? %s\n\n", yesorno(slist_empty(list)));
}

void slist_test() {
  printf("\n//===================== TESTING SLIST =========================\n\n");
  slist* list = slist_create();
  slist_test_once(list, 10, 50, 10, slist_pushfront);
  slist_test_once(list, 10, 100, 20, slist_pushback);

  printf("\n    All Assertions passed!...\n");
  free(list);
  printf("//===================== END TESTING SLIST =========================\n\n");
}
