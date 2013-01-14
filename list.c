/* linked list */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "list.h"

/* creates a single node */
struct node *getNode(void *value)
{
	struct node *root = (struct node *)malloc(sizeof(struct node));
	if(root == NULL){
		perror("error: cannot create node");
		return NULL;
	}
	root->value = value;
	root->next = NULL;
	return root;
}

/* creates a new empty list */
struct list *newList()
{
	struct list *myList = (struct list*)malloc(sizeof(struct list));
	if(myList == NULL){
		perror("error: cannot create list");
		return NULL;
	}
	myList->head = NULL;
	return myList;
}

/* frees all the memory in the list */
void destroy(struct list *myList)
{
	struct node *temp = myList->head;
	struct node *next = NULL;
	if(temp == NULL) return;
	else {
		while(temp != NULL){
			next = temp->next;
			free(temp->value);
			free(temp);
			temp = next;
		}
	}
}

/* pops node off the top and returns it */
struct node *pop(struct list *myList)
{
	struct node *head = myList->head;
	if(head != NULL){
		myList->head = head->next;
	}
	return head;
}

/* removes element from back of list */
/* thought I might need this for history, but ended up 
not using it at all */
 struct node *rem(struct list* myList)
{
	/* iterate to back of list */
	struct node *n = myList->head;
	struct node *delete = n->next;
	if(n == NULL) return NULL;
	if(n->next == NULL) return NULL;
	while(delete->next != NULL)
	{
		n = n->next;
		delete = delete->next;
	}
	n->next = NULL;
	return myList->head;
}

/* adds element to the back of the list */
/* thought I might need this for history, but ended up
not using it at all */
void add(struct list *myList, void *value)
{
	struct node *tail = myList->head;
	struct node *append = getNode((char*)value);
	if(tail == NULL) myList->head = append;
	while(tail->next != NULL)
	{
		tail = tail->next;
	}
	tail->next = append;
}

/* pushes the node onto the list */
void push(struct list *myList, void *value)
{
	struct node *head = myList->head;
	struct node *newHead = getNode(value);
	newHead->next = head;
	myList->head = newHead;

}

/* returns the value at a certain index */
char* get(struct list* myList, int i)
{
	int counter = 0;
	struct node *head = myList->head;
	if(head == NULL) return NULL;
	while(head != NULL && counter < i)
	{
		head = head->next;
		counter++;
	}
	char*temp = (char*)malloc(strlen(head->value));
	strcpy(temp, head->value);
	return head->value;
}

/* returns size of list */
int size(struct list* myList)
{
	int counter = 0;
	struct node *head = myList->head;
	while(head != NULL)
	{
		counter++;
		head = head->next;
	}
	return counter;
}
