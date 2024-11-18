//-----------------------------------------
// NAME: Sam Heppner
// STUDENT NUMBER: 7998801
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Tim Zapp
// ASSIGNMENT: 3, QUESTION: 1
// 
// REMARKS: Interface for the Container data type
//
//-----------------------------------------

#ifndef _container_h
#define _container_h

typedef struct CONTAINER Container;
typedef struct NODE Node;

typedef enum BOOLEAN Boolean;

enum BOOLEAN {
    false,
    true
};

struct NODE {
    char *data;
    Node *next;
};

struct CONTAINER {
    Node *top;
    Node *curr;
    int size;
};

// Creates a new instance of the Container
Container* createContainer();
// Destroys a Container and its contents
void destroyContainer(Container *cont);

// Inserts an item into the given container
void insertItem(Container *cont, char *str);
// Deletes an item from the given container, only one if there are multiple
void deleteItem(Container *cont, char *str);
// Deletes all items from the given container
void clear(Container *cont);

// Starts iterating over items in the given container
char* firstItem(Container *cont);
// Continues iterating over items in the given container
char* nextItem(Container *cont);
// Returns true if the container contains the given item
Boolean contains(Container *cont, char *str);
// Returns the size of the given container
int size(Container *cont);

#endif