//-----------------------------------------
// NAME: Sam Heppner
// STUDENT NUMBER: 7998801
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Tim Zapp
// ASSIGNMENT: 3, QUESTION: 1
// 
// REMARKS: This program is a testing suite for the Container data type.
//
//-----------------------------------------

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "main.h"

int numTestsRun = 0;
int numTestsPassed = 0;

int main() {
    Container *cont;
    char *empty = "";
    char *s1 = "one";
    char *s2 = "two";
    char *s3 = "three";
    char *s4 = "four";

    // ----- tests for createContainer()
    
    testCreateContainer();

    // ---------------------------------

    // ----- tests for destroyContainer()

    cont = createContainer();
    testDestroyContainer(cont);

    cont = createContainer();
    insertItem(cont, s1);
    testDestroyContainer(cont);

    cont = createContainer();
    insertItem(cont, s1);
    insertItem(cont, s2);
    testDestroyContainer(cont);

    cont = createContainer();
    insertItem(cont, s1);
    insertItem(cont, s2);
    insertItem(cont, s3);
    testDestroyContainer(cont);

    // ----------------------------------

    // ----- tests for insertItem()

    cont = createContainer();

    testInsert(cont, s1);
    testInsert(cont, s2);
    testInsert(cont, s3);
    testInsert(cont, s1);

    destroyContainer(cont);

    // ----------------------------

    // ----- tests for deleteItem()

    cont = createContainer();
    testDelete(cont, s1, false);

    insertItem(cont, s1);
    testDelete(cont, s1, true);

    clear(cont);

    insertItem(cont, s1);
    insertItem(cont, s2);
    insertItem(cont, s3);
    testDelete(cont, s4, false);

    clear(cont);

    insertItem(cont, s1);
    insertItem(cont, s2);
    insertItem(cont, s3);
    insertItem(cont, s4);
    testDelete(cont, s2, true);
    testDelete(cont, s1, true);
    testDelete(cont, s4, true);

    clear(cont);

    insertItem(cont, s1);
    insertItem(cont, s1);
    testDelete(cont, s1, true);
    
    clear(cont);

    insertItem(cont, empty);
    testDelete(cont, empty, true);

    destroyContainer(cont);

    // ----------------------------

    // ----- tests for clear()

    cont = createContainer();
    testClear(cont);

    insertItem(cont, s1);
    testClear(cont);

    insertItem(cont, s1);
    insertItem(cont, s2);
    testClear(cont);

    insertItem(cont, s1);
    insertItem(cont, s2);
    insertItem(cont, s3);
    testClear(cont);

    destroyContainer(cont);

    // -----------------------

    // ----- tests for firstItem()

    cont = createContainer();
    testFirstItem(cont, NULL);

    insertItem(cont, s1);
    testFirstItem(cont, s1);

    insertItem(cont, s2);
    nextItem(cont);
    testFirstItem(cont, s1);

    destroyContainer(cont);

    // ---------------------------

    // ----- tests for nextItem()

    cont = createContainer();

    testNextItem(cont, NULL);
    
    insertItem(cont, s1);
    testNextItem(cont, s1);

    clear(cont);

    insertItem(cont, s1);
    insertItem(cont, s2);
    insertItem(cont, s3);

    firstItem(cont);

    testNextItem(cont, s2);
    testNextItem(cont, s3);
    testNextItem(cont, NULL);

    firstItem(cont);
    deleteItem(cont, s1);

    testNextItem(cont, s2);

    destroyContainer(cont);

    // --------------------------

    // ----- tests for contains()

    cont = createContainer();

    testContains(cont, s1, false);

    insertItem(cont, s1);
    testContains(cont, s1, true);

    insertItem(cont, s2);
    testContains(cont, s2, true);

    insertItem(cont, s3);
    testContains(cont, s2, true);

    insertItem(cont, s1);
    testContains(cont, s1, true);

    deleteItem(cont, s2);
    testContains(cont, s2, false);

    clear(cont);
    insertItem(cont, empty);
    testContains(cont, empty, true);

    destroyContainer(cont);

    // --------------------------

    // ----- tests for size()

    cont = createContainer();

    testSize(cont, 0);

    insertItem(cont, s1);
    testSize(cont, 1);

    insertItem(cont, s2);
    testSize(cont, 2);

    insertItem(cont, s3);
    testSize(cont, 3);

    deleteItem(cont, s2);
    testSize(cont, 2);

    clear(cont);
    testSize(cont, 0);

    destroyContainer(cont);

    // ----------------------

    printf("\nTESTS RUN: %d\n", numTestsRun);
    printf("TESTS PASSED: %d\n", numTestsPassed);
    printf("TESTS FAILED: %d\n", numTestsRun - numTestsPassed);
    printf("Program successfully terminated.\n");
    return EXIT_SUCCESS;
}

// testCreateContainer
//
// Purpose: Perform a test on the createContainer() function
// Input parameters:
// -
// Output parameters:
// -
void testCreateContainer() {
    Container *cont = createContainer();

    printf("Testing if a container was successfully created.\n");
    
    // assuming container successfully created if createContainer() does not return null
    if (cont != NULL) {
        printf("SUCCESS: Container successfully created.\n");
        numTestsPassed += 1;
    } else {
        printf("FAILED: Container was not successfully created.\n");    
    }

    numTestsRun += 1;

    destroyContainer(cont);
}

// testDestroyContainer
//
// Purpose: Perform a test on the destroyContainer() function
// Input parameters:
// cont (Container *): The container to be destroyed
// Output parameters:
// -
void testDestroyContainer(Container *cont) {
    assert(cont != NULL);

    if (cont != NULL) {
        printf("Testing if a container with %d items will be successfully destroyed.\n", size(cont));

        destroyContainer(cont);

        // assuming memory properly freed if no crash
        printf("SUCCESS: Container destroyed.\n");

        numTestsPassed += 1;
        numTestsRun += 1;
    }
}

// testInsert
//
// Purpose: Perform a test on the insertItem() function
// Input parameters:
// cont (Container *): The container to test on
// item (char *): The string to insert
// Output parameters:
// -
void testInsert(Container *cont, char *item) {
    int oldSize;
    int numOldItems;
    int numNewItems;

    assert(cont != NULL);
    assert(item != NULL);

    if (cont != NULL && item != NULL) {
        oldSize = size(cont);
        numOldItems = numItemsMatching(cont, item);

        printf("Testing inserting \"%s\" into container.\n", item);
        
        insertItem(cont, item);

        numNewItems = numItemsMatching(cont, item);

        // compare the number of instances of (item) before and after inserting
        if (size(cont) == oldSize + 1 && numNewItems == numOldItems + 1) {
            printf("SUCCESS: Container now includes \"%s\".\n", item);
            numTestsPassed += 1;
        } else {
            printf("FAILED: Container does not include \"%s\".\n", item);
        }

        numTestsRun += 1;
    }
}

// testDelete
//
// Purpose: Perform a test on the deleteItem() function
// Input parameters:
// cont (Container *): The container to test on
// item (char *): The string to delete (if present)
// sizeShouldDecrease (Boolean): Whether deleteItem() should cause the size of the
// container to decrease.
// Output parameters:
// -
void testDelete(Container *cont, char *item, Boolean sizeShouldDecrease) {
    int oldSize;
    int numOldItems = 0;
    int numNewItems = 0;

    assert(cont != NULL);
    assert(item != NULL);

    if (cont != NULL && item != NULL) {
        oldSize = size(cont);
        numOldItems = numItemsMatching(cont, item);

        printf("Testing deleting \"%s\" from container, expected to%s be deleted.\n", item, sizeShouldDecrease ? "" : " not");

        // compare the number of instances of (item) before and after deleting
        if (sizeShouldDecrease) {
            deleteItem(cont, item);

            numNewItems = numItemsMatching(cont, item);

            if (size(cont) == oldSize - 1 && numNewItems == numOldItems - 1) {
                printf("SUCCESS: \"%s\" successfully deleted from container.\n", item);
                numTestsPassed += 1;
            } else {
                printf("FAILED: \"%s\" unsuccessfully deleted from container.\n", item);
            }
        } else {
            deleteItem(cont, item);

            numNewItems = numItemsMatching(cont, item);

            if (size(cont) == oldSize && numNewItems == numOldItems) {
                printf("SUCCESS: \"%s\" was not deleted from container.\n", item);
                numTestsPassed += 1;
            } else {
                printf("FAILED: An item was wrongfully deleted from container.\n");
            }
        }

        numTestsRun += 1;
    }
}

// testClear
//
// Purpose: Perform a test on the clear() function
// Input parameters:
// cont (Container *): The container to be cleared
// Output parameters:
// -
void testClear(Container *cont) {
    assert(cont != NULL);

    if (cont != NULL) {
        printf("Testing clearing container with %d items.\n", size(cont));
        
        clear(cont);

        // no items in container and all pointers cleared
        if (firstItem(cont) == NULL && size(cont) == 0) {
            printf("SUCCESS: Container successfully cleared.\n");

            numTestsPassed += 1;
        } else {
            printf("FAILED: Container unsuccessfully cleared.\n");
        }

        numTestsRun += 1;
    }
}

// testFirstItem
//
// Purpose: Perform a test on the firstItem() function
// Input parameters:
// cont (Container *): The container to test on
// expected (char *): The expected first item
// Output parameters:
// -
void testFirstItem(Container *cont, char *expected) {
    char *actual;

    assert(cont != NULL);

    if (cont != NULL) {
        printf("Testing that the first item in the container is \"%s\"\n", expected == NULL ? "NULL" : expected);

        actual = firstItem(cont);

        // edge case where strcmp will be undefined behaviour
        if (expected == NULL) {
            if (actual == NULL) {
                printf("SUCCESS: End of container reached.\n");

                numTestsPassed += 1;
            } else {
                printf("FAILED: firstItem returned something other than NULL.\n");
            }
        } else {
            if (actual != NULL && !strcmp(actual, expected)) {
                printf("SUCCESS: The first item in the container is \"%s\"\n", expected);

                numTestsPassed += 1;
            } else {
                printf("FAILED: The first item in the container was not \"%s\"\n", expected);
            }
        }
        
        numTestsRun += 1;
    }
}

// testNextItem
//
// Purpose: Perform a test on the nextItem() function
// Input parameters:
// cont (Container *): The container to test on
// expected (char *): The expected next item
// Output parameters:
// -
void testNextItem(Container *cont, char *expected) {
    char *actual;

    assert(cont != NULL);

    if (cont != NULL) {
        printf("Testing that the next item in the container is \"%s\"\n", expected == NULL ? "NULL" : expected);

        actual = nextItem(cont);

        // edge case where strcmp will have undefined behaviour
        if (expected == NULL) {
            if (actual == NULL) {
                printf("SUCCESS: End of container reached.\n");

                numTestsPassed += 1;
            } else {
                printf("FAILED: nextItem returned something other than NULL.\n");
            }
        } else {
            if (actual != NULL && !strcmp(actual, expected)) {
                printf("SUCCESS: The next item in the container is \"%s\"\n", expected);

                numTestsPassed += 1;
            } else {
                printf("FAILED: The next item in the container was not \"%s\", and was %s.\n", expected, actual);
            }
        }
        
        numTestsRun += 1;
    }
}

// testContains
//
// Purpose: Perform a test on the contains() function
// Input parameters:
// cont (Container *): The container to test on
// item (char *): The item to search for
// expected (Boolean): Whether the item should be found
// Output parameters:
// -
void testContains(Container *cont, char *item, Boolean expected) {
    assert(cont != NULL);
    assert(item != NULL);

    if (cont != NULL && item != NULL) {
        printf("Testing if the container contains \"%s\"\n", item);

        if (contains(cont, item) == expected) {
            printf("SUCCESS: Container contains \"%s\"\n", item);

            numTestsPassed += 1;
        } else {
            printf("FAILED: Container does not contain \"%s\"\n", item);
        }

        numTestsRun += 1;
    }
}

// testSize
//
// Purpose: Perform a test on the size() function
// Input parameters:
// cont (Container *): The container to test on
// expected (int): The expected size of the container
// Output parameters:
// -
void testSize(Container *cont, int expected) {
    assert(cont != NULL);

    if (cont != NULL) {
        printf("Testing if the size of the container is %d.\n", expected);

        if (size(cont) == expected) {
            printf("SUCCESS: The size of the container is %d.\n", expected);

            numTestsPassed += 1;
        } else {
            printf("FAILED: The size of the container is not %d.\n", expected);
        }

        numTestsRun += 1;
    }
}

// numItemsMatching
//
// Purpose: Return the number of items in a Container matching the input
// Input parameters:
// cont (Container *): The container to search
// item (char *): The item to search for
// Output parameters:
// return (int) The number of items matching the input
int numItemsMatching(Container *cont, char *item) {
    char *curr;
    int numItems = 0;

    assert(cont != NULL);
    assert(item != NULL);

    if (cont != NULL && item != NULL) {
        curr = firstItem(cont);

        while (curr != NULL) {
            if (!strcmp(curr, item)) {
                numItems += 1;
            }
            
            curr = nextItem(cont);
        }
    }

    return numItems;
}