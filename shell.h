const int BUFFER_SIZE = 128;

struct list *myPath;
struct list *myHistory;

void forkExec(char*);
int changeDirectory(char*);
void path(char*);
void delete(void*);
void run(char*);
int end(char*);
void history();
char* bang(char*);
void addHistory(char*);
