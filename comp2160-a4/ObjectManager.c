#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "ObjectManager.h"

typedef struct INDEXNODE IndexNode;

struct INDEXNODE {
    Ref ref;
    uchar *location;
    IndexNode *next;
    ulong size;
    int numRefs;
};

Ref nextRef = 1; // newest unused reference 
IndexNode *indexTop = NULL;
uchar *bufferOne;
uchar *bufferTwo;
bool activeBuffer = true; // true for buffer one, false for buffer two

static void compact();
static ulong cleanIndex();

static bool checkIndex();
static bool checkIndexNode(IndexNode *node);

static IndexNode* createIndexNode(ulong size);

static void addNode(IndexNode *node);
static void printIndexNode(IndexNode *node);
static bool spaceAvailable(ulong size);
static IndexNode* findObjectIndex(Ref ref);

//------------------------------------------------------
// insertObject
//
// PURPOSE: Attempts to insert an object of the given size into the pool,
// triggering garbage collection once if there is no space and attempting one more time
// to insert.
// INPUT PARAMETERS:
// size (ulong): The size of the object to be inserted.
// OUTPUT PARAMETERS:
// return (Ref): Reference to the newly inserted object (NULL_REF if failed).
//------------------------------------------------------
Ref insertObject(ulong size) {
    Ref ref = NULL_REF;
    IndexNode *newNode = NULL;

    assert(size >= 0ul);

    if (checkIndex()) {
        // first check before compacting to see if there is space
        if (spaceAvailable(size)) {
            newNode = createIndexNode(size);

            if (checkIndexNode(newNode)) {
                addNode(newNode);
                ref = newNode->ref;
            }
        } else {
            // if no space before compacting then attempt to add after compacting once
            compact();

            if (spaceAvailable(size)) {
                newNode = createIndexNode(size);

                if (checkIndexNode(newNode)) {
                    addNode(newNode);
                    ref = newNode->ref;
                }
            } else {
                // failure to insert object
                printf("No space available for object of size %lu.\n", size);
            }
        }
    }

    checkIndex();

    return ref;
}

//------------------------------------------------------
// retrieveObject
//
// PURPOSE: Returns a pointer to the location of the data referenced by the given Ref.
// INPUT PARAMETERS:
// ref (Ref): The reference to locate.
// OUTPUT PARAMETERS:
// return (void *): The location in memory of the given Ref (NULL if not found).
//------------------------------------------------------
void* retrieveObject(Ref ref) {
    IndexNode *node;
    void *location = NULL;

    assert(ref >= 0ul);

    if (ref >= 0ul) {
        node = findObjectIndex(ref);
        
        if (node != NULL) {
            location = (void *) node->location;
        }
    }

    return location;
}

//------------------------------------------------------
// addReference
//
// PURPOSE: Increments the number of references for the object referenced by the
// given Ref.
// INPUT PARAMETERS:
// ref (Ref): The reference to increment.
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void addReference(Ref ref) {
    IndexNode *curr;

    assert(ref >= 0ul);

    curr = findObjectIndex(ref);

    if (curr != NULL && checkIndexNode(curr)) {
        curr->numRefs += 1;
    }

    checkIndex();
}

//------------------------------------------------------
// dropReference
//
// PURPOSE: Decrements the number of references for the object referenced by the given Ref.
// INPUT PARAMETERS:
// ref (Ref): The reference to decrement.
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void dropReference(Ref ref) {
    IndexNode *curr;

    assert(ref >= 0ul);

    curr = findObjectIndex(ref);

    if (curr != NULL && checkIndexNode(curr)) {
        // clamp number of references to nonnegative numbers
        if (curr->numRefs > 0) {
            curr->numRefs -= 1;
        }
    }

    checkIndex();
}

//------------------------------------------------------
// initPool
//
// PURPOSE: Allocates space for the two buffers used by the ObjectManager. 
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void initPool() {
    assert(bufferOne == NULL);
    assert(bufferTwo == NULL);

    bufferOne = (uchar *) malloc(MEMORY_SIZE);
    bufferTwo = (uchar *) malloc(MEMORY_SIZE);
    // buffer 1 active
    activeBuffer = true;

    assert(bufferOne != NULL);
    assert(bufferTwo != NULL);
    assert(activeBuffer);
}

//------------------------------------------------------
// destroyPool
//
// PURPOSE: Frees all memory allocated by the ObjectManager.
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void destroyPool() {
    IndexNode *curr = indexTop;
    IndexNode *temp;

    assert(bufferOne != NULL);
    assert(bufferTwo != NULL);

    free(bufferOne);
    free(bufferTwo);

    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);    
    }

    bufferOne = NULL;
    bufferTwo = NULL;
    indexTop = NULL;
}

//------------------------------------------------------
// dumpPool
//
// PURPOSE: Prints out the contents of the object pool. 
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void dumpPool() {
    IndexNode *curr;
    ulong bytesInUse = 0;

    if (checkIndex()) {
        curr = indexTop;

        while (curr != NULL) {
            printIndexNode(curr);
            bytesInUse += curr->size;
            curr = curr->next;
        }

        printf("Bytes in use: %lu | Bytes free: %lu\n", bytesInUse, MEMORY_SIZE - bytesInUse);
    }
}

//------------------------------------------------------
// printIndexNode
//
// PURPOSE: Prints relevant information for an IndexNode in the index
// INPUT PARAMETERS:
// curr (IndexNode *): Pointer to the IndexNode to print
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
static void printIndexNode(IndexNode *curr) {
    if (checkIndexNode(curr)) {
        printf("Block #%lu: Starts at %p, with size %lu. Referenced %d times.\n",
               curr->ref,
               curr->location,
               curr->size,
               curr->numRefs);
    }
}

//------------------------------------------------------
// spaceAvailable
//
// PURPOSE: Returns true if there is a contiguous block of space available after the last element
// in the buffer to allocate an object of given size.
// INPUT PARAMETERS:
// size (ulong): The size of the object to be added
// OUTPUT PARAMETERS:
// return (bool): True if there is space, false if not
//------------------------------------------------------
static bool spaceAvailable(ulong size) {
    IndexNode *curr;
    ulong offset;
    bool isSpace = false;

    assert(size > 0ul);

    if (checkIndex() && size > 0ul) {
        curr = indexTop;
        offset = 0ul;

        while (curr != NULL) {
            offset += curr->size;
            curr = curr->next;
        }

        if ((MEMORY_SIZE - offset) >= size) {
            isSpace = true;
        }
    }

    return isSpace;
}

//------------------------------------------------------
// findObjectIndex
//
// PURPOSE: Returns a pointer to the IndexNode that corresponds to a given Ref.
// INPUT PARAMETERS:
// ref (Ref): The reference to search for.
// OUTPUT PARAMETERS:
// return (IndexNode *): Pointer to the IndexNode referenced by the given Ref (NULL if not found).
//------------------------------------------------------
static IndexNode* findObjectIndex(Ref ref) {
    IndexNode *curr = NULL;

    assert(ref >= 0ul);

    if (checkIndex()) {
        curr = indexTop;

        while (curr != NULL && curr->ref != ref) {
            curr = curr->next;
        }
    }

    return curr;
}

//------------------------------------------------------
// cleanIndex
//
// PURPOSE: Removes all IndexNodes in the index with 0 references. 
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// return (ulong): The number of bytes that are newly available as a result of the cleaning.
//------------------------------------------------------
static ulong cleanIndex() {
    IndexNode *curr;
    IndexNode *temp;
    ulong bytesFreed = 0;

    if (checkIndex()) {
        curr = indexTop;

        // remove all unreferenced nodes from front of index
        while (curr != NULL && curr->numRefs == 0) {
            temp = curr;
            bytesFreed += curr->size;
            indexTop = curr->next;
            curr = curr->next;
            free(temp);
        }

        // remove any other unreferenced nodes from other places in the index
        while (curr != NULL && curr->next != NULL) {
            if (checkIndexNode(curr->next) && curr->next->numRefs == 0) {
                temp = curr->next;
                bytesFreed += curr->next->size;
                curr->next = curr->next->next;
                free(temp);
            } else {
                curr = curr->next;
            }
        }
    }

    checkIndex();

    return bytesFreed;
}

//------------------------------------------------------
// checkIndex
//
// PURPOSE: Type invariant for the index linked list. Iterates over the list and checks each node,
// and sums the total number of bytes in use to ensure that it is always less than or equal to 
// the total size of the buffer.
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// return (bool): True if the index is valid, false if not
//------------------------------------------------------
static bool checkIndex() {
    IndexNode *curr = indexTop;
    bool isValid = true;
    ulong totalBytesInUse = 0;

    while (curr != NULL) {
        if (!checkIndexNode(curr)) {
            isValid = false;
        } else {
            totalBytesInUse += curr->size;
        }

        curr = curr->next;
    }

    assert(totalBytesInUse <= MEMORY_SIZE);

    if (totalBytesInUse > MEMORY_SIZE) {
        isValid = false;
    }

    return isValid;
}

//------------------------------------------------------
// checkIndexNode
//
// PURPOSE: Type invariant for a node in the index linked list.
// INPUT PARAMETERS:
// node (IndexNode *): Pointer to the node to be checked.
// OUTPUT PARAMETERS:
// return (bool): True if the given node is valid, false if not
//------------------------------------------------------
static bool checkIndexNode(IndexNode *node) {
    bool validNode = false;

    assert(node != NULL);
    assert(node->ref >= 0ul);
    assert(node->size > 0ul);
    assert(node->numRefs >= 0);

    if (node != NULL && node->ref >= 0ul
        && node->size > 0ul && node->numRefs >= 0) {
        validNode = true;
    }

    return validNode;
}

//------------------------------------------------------
// createIndexNode
//
// PURPOSE: Allocate space for and initialize a new IndexNode.
// INPUT PARAMETERS:
// size (ulong): The size of the object pointed to by the new IndexNode
// OUTPUT PARAMETERS:
// return (IndexNode *): Pointer to the newly created IndexNode (NULL if allocation failed).
//------------------------------------------------------
static IndexNode* createIndexNode(ulong size) {
    IndexNode *newNode;

    assert(size > 0ul);

    newNode = (IndexNode *) malloc(sizeof(IndexNode));

    assert(newNode != NULL);

    if (newNode != NULL && size > 0ul) {
        newNode->ref = nextRef;
        newNode->location = NULL;
        newNode->next = NULL;
        newNode->size = size;
        newNode->numRefs = 1ul;
    }

    if (checkIndexNode(newNode)) {
        nextRef += 1;

        // prevent overflow of nextRef from causing a new node to be allocated
        // with NULL_REF as its reference number
        if (nextRef == 0) {
            nextRef = 1;
        }
    }

    return checkIndexNode(newNode) ? newNode : NULL;
}

//------------------------------------------------------
// addNode
//
// PURPOSE: Adds a given IndexNode to the end of the index, and updates its location pointer to reflect the new
// location of its associated object
// INPUT PARAMETERS:
// node (IndexNode *): Pointer to the IndexNode to be added
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
static void addNode(IndexNode *node) {
    IndexNode *curr;

    if (checkIndexNode(node) && checkIndex()) {
        curr = indexTop;

        // update location to start of buffer if new node is the first added
        if (curr == NULL) {
            indexTop = node;
            node->location = activeBuffer ? bufferOne : bufferTwo;
        } else {
            while (checkIndexNode(curr) && curr->next != NULL) {
                curr = curr->next;
            }

            node->location = curr->location + curr->size;
            curr->next = node;        
        }
    }

    checkIndex();
}

//------------------------------------------------------
// compact
//
// PURPOSE: Removes all unreferenced nodes from the index and defragments the buffer.
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
static void compact() {
    IndexNode *curr;
    ulong offset = 0ul;
    ulong numObjects = 0ul;
    ulong bytesFreed = 0ul;

    if (checkIndex()) {
        bytesFreed = cleanIndex();
        checkIndex();
        curr = indexTop;

        while (curr != NULL) {
            // Copy current node's object to the new buffer and update its location pointer
            if (activeBuffer) {
                memcpy(bufferTwo + offset, curr->location, curr->size);
                curr->location = (bufferTwo + offset);
            } else {
                memcpy(bufferOne + offset, curr->location, curr->size);
                curr->location = (bufferOne + offset);
            }

            numObjects += 1;
            offset += curr->size;
            curr = curr->next;
        }

        // Swap which buffer is active
        activeBuffer = !activeBuffer;

        printf("\nGarbage collector statistics:\n");
        printf("Objects: %lu | Bytes in use: %lu | Bytes freed: %lu\n\n", numObjects, offset, bytesFreed);
    }

    checkIndex();
}