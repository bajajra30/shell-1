struct node {
	void *value;
	struct node *next;
};

struct list {
	struct node *head;
};

struct node *getNode(void *);
struct list *newList();
void destroy(struct list *);
struct node *pop(struct list *);
void push(struct list *, void *);
void add(struct list *, void*);
struct node *rem(struct list*);
char* get(struct list*, int i);
int size(struct list*);
