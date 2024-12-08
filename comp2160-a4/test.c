//-----------------------------------------
// NAME: Sam Heppner
// STUDENT NUMBER: 7998801
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Tim Zapp
// ASSIGNMENT: 4, QUESTION: question 1
// 
// REMARKS: This program is a test suite for the functions provided through the
// ObjectManager.h interface.
//
//-----------------------------------------

#include "ObjectManager.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define TEST_REFS 100

int numTestsRun = 0;
int numTestsPassed = 0;

int main() {
    Ref temp1;
    Ref temp2;
    Ref temp3;
    Ref temp4;
    Ref temp5;
    Ref testRefs[TEST_REFS];

    char *tempLocation;
    char testStr[101] = "This is a test string with length 100.abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz1234567890";

    //tests for initPool();
    testInitPool();
    testDestroyPool();
    
    // tests for insertObject()
    initPool();
    
    temp1 = testInsertObject(10, true);
    dropReference(temp1);
    temp2 = testInsertObject(MEMORY_SIZE, true);
    testInsertObject(10, false);
    dropReference(temp2);

    for (int i = 0; i < TEST_REFS; i++) {
        testRefs[i] = testInsertObject(100, true);
    }

    for (int i = 0; i < TEST_REFS; i++) {
        dropReference(testRefs[i]);
    }

    destroyPool();

    //tests for retrieveObject()
    initPool();

    temp1 = insertObject(10);
    temp2 = insertObject(100);

    tempLocation = (char *) retrieveObject(temp2);

    for (int i = 0; i < TEST_REFS; i++) {
        tempLocation[i] = testStr[i];
    }

    temp3 = insertObject(100);
    dropReference(temp3);
    temp4 = insertObject(MEMORY_SIZE);

    testRetrieveObject(temp1, true);
    testRetrieveObject(temp2, true);
    testRetrieveObject(temp3, false);
    testRetrieveObject(temp4, false);

    printf("Data at %p should match this:\n %s\n", tempLocation, testStr);
    printf("Data at %p: ", tempLocation);

    for (int i = 0; i < TEST_REFS; i++) {
        printf("%c", tempLocation[i]);
    }

    printf("\n");

    destroyPool();

    // tests for addReference()
    initPool();

    temp1 = insertObject(10);
    dropReference(temp1);
    testAddReference(temp1, 10);
    dropReference(temp1);

    temp2 = insertObject(10);
    temp3 = insertObject(10);
    temp4 = insertObject(10);
    temp5 = insertObject(10);

    dropReference(temp2);
    dropReference(temp3);
    dropReference(temp4);
    dropReference(temp5);

    testAddReference(temp3, 10);
    dropReference(temp3);

    destroyPool();

    //tests for dropReference()
    initPool();

    temp1 = insertObject(10);
    testDropReference(temp1);
    dropReference(temp1);

    temp1 = insertObject(10);
    temp2 = insertObject(10);
    temp3 = insertObject(10);

    insertObject(MEMORY_SIZE);

    dropReference(temp1);
    dropReference(temp2);
    testDropReference(temp3);

    for (int i = 0; i < TEST_REFS; i++) {
        testRefs[i] = 0;
        testRefs[i] = insertObject(10);
        dropReference(testRefs[i]);
    }

    printf("Garbage collector should report no objects in use and 1000 bytes freed:\n");
    temp1 = insertObject(MEMORY_SIZE);

    destroyPool();

    printf("\n--------------------------------------\n");
    printf("TESTS PASSED: %d | TESTS TOTAL: %d\n", numTestsPassed, numTestsRun);
}

//------------------------------------------------------
// testInsertObject
//
// PURPOSE: Tests inserting an object of given size.
// INPUT PARAMETERS:
// size (ulong): The size of the object to be inserted.
// shouldInsert (bool): Whether the object should successfully insert.
// OUTPUT PARAMETERS:
// return (Ref): The reference to the object that was inserted. (NULL_REF if unsuccessful) 
//------------------------------------------------------
Ref testInsertObject(ulong size, bool shouldInsert) {
    Ref ref = NULL_REF;

    assert(size > 0ul);

    if (size > 0ul) {
        printf("Testing inserting an object of size %lu.\n", size);

        ref = insertObject(size);

        if (shouldInsert == (ref != NULL_REF)) {
            if (shouldInsert) {
                printf("Object of size %lu successfully inserted, reference ID #%lu\n", size, ref);
            } else {
                printf("Object of size %lu was correctly not added.\n", size);
            }

            numTestsPassed += 1;
        } else {
            printf("Result of inserting object of size %lu did not match what was expected.\n", ref);
        }

        numTestsRun += 1;
    }

    return ref;
}

//------------------------------------------------------
// testRetrieveObject
//
// PURPOSE: Tests retrieving the given Ref.
// INPUT PARAMETERS:
// ref (Ref): The reference to retrieve.
// shouldExist (bool): Whether the given Ref should exist.
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void testRetrieveObject(Ref ref, bool shouldExist) {
    void *location;

    printf("Testing retrieving object ID #%lu.\n", ref);

    location = retrieveObject(ref);

    if (shouldExist && location != NULL) {
        printf("Object ID #%lu exists as expected.\n", ref);
        numTestsPassed += 1;
    } else if (!shouldExist && location == NULL) {
        printf("Object ID #%lu was correctly not found.\n", ref);
        numTestsPassed += 1;
    } else {
        printf("Object ID #%lu was expected not to exist and was wrongly found.\n", ref);
    }

    numTestsRun += 1;
}

//------------------------------------------------------
// testAddReference
//
// PURPOSE: Tests adding a reference to the given Ref.
// Notes: Requires that all objects in the pool have 0 references, as it tests by triggering the garbage collector
// and determining whether the given Ref still exists after incrementing it.
// INPUT PARAMETERS:
// ref (Ref): The reference to increment.
// size (ulong): The size of the given Ref's object.
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void testAddReference(Ref ref, ulong size) {
    Ref temp;
    void *location;

    assert(ref > 0ul);
    assert(size > 0ul);

    if (ref > 0ul && size > 0ul) {
        addReference(ref);
        temp = insertObject(MEMORY_SIZE - size);
        dropReference(temp);

        location = retrieveObject(ref);
        if (location != NULL) {
            printf("Object ID #%lu reference count successfully incremented.\n", ref);
            numTestsPassed += 1;
        } else {
            printf("Object ID #%lu reference count failed to increment.\n", ref);
        }
        
        numTestsRun += 1;
    }
}

// req: all objects in pool except ref have 0 references

//------------------------------------------------------
// testDropReference
//
// PURPOSE: Tests dropping a reference from the given Ref.
// Notes: Requires that all other objects have 0 references, and the given ref has exactly 1.
// Tests by determining if all objects are collected after dropping a reference from the given
// Ref.
// INPUT PARAMETERS:
// ref (Ref): The reference to decrement.
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void testDropReference(Ref ref) {
    void *location;
    Ref temp;

    assert(ref > 0ul);

    if (ref > 0ul) {
        printf("Testing dropping reference to ID #%lu.\n", ref);

        dropReference(ref);
        temp = insertObject(MEMORY_SIZE);
        dropReference(temp);
        location = retrieveObject(ref);
        
        if (location == NULL) {
            printf("Reference to ID #%lu successfully dropped.\n", ref);
            numTestsPassed += 1;
        } else {
            printf("Reference to ID #%lu unsuccessfully dropped.\n", ref);
        }

        numTestsRun += 1;
    }
}

//------------------------------------------------------
// testInitPool
//
// PURPOSE: Tests initializing the pool by printing a message if no assertions or crashes occur.
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void testInitPool() {
    numTestsRun += 1;

    printf("Testing for errors when initializing the pool.\n");

    initPool();

    printf("Pool initialized without error.\n");

    numTestsPassed += 1;
}

//------------------------------------------------------
// testDestroyPool
//
// PURPOSE: Tests destroying the pool by printing a message if no assertions or crashes occur.
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// -
//------------------------------------------------------
void testDestroyPool() {
    numTestsRun += 1;

    printf("Testing for errors when destroying the pool.\n");

    destroyPool();

    printf("Pool destroyed without error.\n");

    numTestsPassed += 1;
}