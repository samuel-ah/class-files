//-----------------------------------------
// NAME: Sam Heppner
// STUDENT NUMBER: 7998801
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Tim Zapp
// ASSIGNMENT: 3, QUESTION: 1
// 
// REMARKS: Implementation of the Container data type
//
//-----------------------------------------

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "container.h"

Node* createNode(char *str);

// createContainer
//
// Purpose: Allocate space and initialize an empty Container
// Input parameters:
// -
// Output parametes:
// return (Container *): Pointer to the newly created Container instance
Container* createContainer() {
    Container *new = (Container *) malloc(sizeof(Container));

    assert(new != NULL);

    if (new != NULL) {
        new->top = NULL;
        new->curr = NULL;
        new->size = 0;
    }

    return new;
}

// createNode
//
// Purpose: Allocate space and initialize a Node with the provided string
// Input parameters:
// str (char *): The string to be contained inside the new Node
// Output parameters:
// return (Node *): Pointer to the newly created Node
Node* createNode(char *str) {
    Node *new;
    char *newStr = NULL;
    
    assert(str != NULL);

    // allocate space and copy string to prevent modifications to (str)
    // affecting the contents of the container
    if (str != NULL) {
        newStr = (char *) malloc(sizeof(char) * strlen(str));
        strcpy(newStr, str);
    }
    
    new = (Node *) malloc(sizeof(Node));
    
    assert(new != NULL);

    if (new != NULL) {
        new->data = newStr;
        new->next = NULL;
    }

    return new;
}

// destroyContainer
//
// Purpose: Destroy a given Container
// Input parameters:
// cont (Container *): The container to be destroyed
// Output parameters:
// -
void destroyContainer(Container *cont) {
    assert(cont != NULL);

    if (cont != NULL) {
        clear(cont);
        free(cont);
    }
}

// insertItem
//
// Purpose: Insert a string into the given Container
// Input parameters:
// str (char *): The string to insert
// Output parameters:
// cont (Container *): The container to insert into
void insertItem(Container *cont, char *str) {
    Node *curr;
    Node *new;

    assert(cont != NULL);
    assert(str != NULL);

    if (cont != NULL && str != NULL) {
        new = createNode(str);

        if (cont->top == NULL) {
            cont->top = new;
        } else {
            curr = cont->top;

            // find end of container to insert at
            while (curr->next != NULL) {
                curr = curr->next;
            }

            curr->next = new;
        }

        if (new != NULL) {
            cont->size += 1;
        }
    }
}

// deleteItem
//
// Purpose: Delete a string from the given Container, if present
// Input parameters:
// str (char *): The string to delete
// Output parameters:
// cont (Container *): The container to delete from
void deleteItem(Container *cont, char *str) {
    Node *curr;
    Node *temp;

    assert(cont != NULL);
    assert(str != NULL);

    if (cont != NULL && str != NULL) {
        curr = cont->top;

        // deleting from top of container
        if (curr != NULL && !strcmp(curr->data, str)) {
            cont->top = curr->next;
            cont->size -= 1;

            if (cont->curr != NULL && !strcmp(str, cont->curr->data)) {
                cont->curr = NULL;
            }

            free(curr->data);
            free(curr);
        // deleting from any other place in the container
        } else {
            while (curr != NULL && curr->next != NULL && strcmp(curr->next->data, str)) {
                curr = curr->next;
            }

            if (curr != NULL && curr->next != NULL) {
                temp = curr->next->next;

                // if the node to be deleted is the same node as the current node being iterated on in the 
                //container, reset the pointer
                if (curr == cont->curr) {
                    cont->curr = cont->top;
                }

                free(curr->next->data);
                free(curr->next);
                curr->next = temp;
                cont->size -= 1;
            }
        }
    }
}

// clear
//
// Purpose: Clear a given container of its contents
// Input parameters:
// -
// Output parameters:
// cont (Container *): The container to be cleared
void clear(Container *cont) {
    Node *curr;
    Node *temp;

    assert(cont != NULL);

    if (cont != NULL) {
        curr = cont->top;
        
        while (curr != NULL) {
            temp = curr;
            curr = curr->next;
            free(temp->data);
            free(temp);
        }
        
        cont->top = NULL;
        cont->curr = NULL;
        cont->size = 0;
    }
}

// firstItem
// 
// Purpose: Return the first string in the given container
// Input parameters:
// cont (Container *): The container to search
// Output parameters:
// return (char *): The item found at the start of the container
char* firstItem(Container *cont) {
    char *data = NULL;

    assert(cont != NULL);

    if (cont != NULL) {
        cont->curr = cont->top;

        if (cont->top != NULL) {
            data = cont->top->data;
        }
    }

    return data;
}

// nextItem
//
// Purpose: Return the next item after the current item being looked at by
// the given Container
// Input parameters:
// cont (Container *): The container to read from
// Output parameters:
// return (char *): The next item found
char* nextItem(Container *cont) {
    char *data = NULL;

    assert(cont != NULL);
    
    if (cont != NULL && cont->curr != NULL) {
        cont->curr = cont->curr->next;

        if (cont->curr != NULL) {
            data = cont->curr->data;
        }
    // in the case where cont->curr is null, reset to top of container
    } else if (cont != NULL && cont->curr == NULL) {
        data = firstItem(cont);
    }

    return data;
}

// contains
//
// Purpose: Returns true if the given Container contains the given item
// Input parameters:
// cont (Container *): The container to be searched
// str (char *): The string to search for
// Output parameters:
// return (Boolean): Whether the item was found
Boolean contains(Container *cont, char *str) {
    Node *curr;
    Boolean found = false;

    assert(cont != NULL && str != NULL);

    if (cont != NULL && str != NULL) {
        curr = cont->top;

        while (curr != NULL) {
            if (curr->data != NULL && !strcmp(curr->data, str)) {
                found = true;
            }

            curr = curr->next;
        }
    }

    return found;
}

// size
//
// Purpose: Returns the size of a given Container
// Input parameters:
// cont (Container *): The container to measure
// Output parameters:
// return (int): The size of the container
int size(Container *cont) {
    int size = 0;

    assert(cont != NULL);
    
    if (cont != NULL) {
        size = cont->size;
    }

    return size;
}