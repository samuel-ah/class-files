//-----------------------------------------
// NAME: Sam Heppner
// STUDENT NUMBER: 7998801
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Tim Zapp
// ASSIGNMENT: 3, QUESTION: 2
// 
// REMARKS: This program is meant to be a testing suite for the Set data type.
//
//-----------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "main.h"

int numTestsRun = 0;
int numTestsPassed = 0;

int main() {
    Set *set1;
    Set *set2;
    Set *expected;

    // Tests for newSet()
    testNewSet();

    testMemoryUsage();
    // -----

    // Tests for deleteSet()
    set1 = newSet();
    testDeleteSet(set1);

    set1 = newSet();
    insertItem(set1, 1);
    insertItem(set1, 2);
    testDeleteSet(set1);

    set1 = newSet();
    insertItem(set1, 1);
    insertItem(set1, 2);
    insertItem(set1, 3);
    insertItem(set1, 4);
    testDeleteSet(set1);

    testMemoryUsage();
    // -----

    // Tests for insertItem()
    set1 = newSet();
    testInsertItem(set1, 1, true);
    testInsertItem(set1, 2, true);
    testInsertItem(set1, 3, true);
    testInsertItem(set1, 1, false);

    removeItem(set1, 2);

    testInsertItem(set1, 2, true);

    deleteSet(set1);

    testMemoryUsage();
    // -----

    // Tests for removeItem()
    set1 = newSet();
    testRemoveItem(set1, 1, false);
    insertItem(set1, 1);
    testRemoveItem(set1, 1, true);

    insertItem(set1, 1);
    insertItem(set1, 2);
    insertItem(set1, 3);

    testRemoveItem(set1, 2, true);

    insertItem(set1, 2);

    testRemoveItem(set1, 4, false);

    deleteSet(set1);

    testMemoryUsage();
    // -----

    // tests for areEqual()
    set1 = newSet();
    set2 = newSet();

    testAreEqual(set1, set2, true);

    insertItem(set1, 1);

    testAreEqual(set1, set2, false);

    insertItem(set2, 1);

    testAreEqual(set1, set2, true);

    insertItem(set1, 1);

    testAreEqual(set1, set2, true);

    insertItem(set1, 2);
    insertItem(set2, 2);
    insertItem(set1, 3);
    insertItem(set2, 3);
    insertItem(set1, 4);
    insertItem(set2, 4);

    testAreEqual(set1, set2, true);

    removeItem(set1, 2);

    testAreEqual(set1, set2, false);
    
    deleteSet(set1);
    deleteSet(set2);

    testMemoryUsage();
    // -----

    // Tests for areDisjoint()
    set1 = newSet();
    set2 = newSet();

    testAreDisjoint(set1, set2, true);

    insertItem(set1, 1);

    testAreDisjoint(set1, set2, true);

    insertItem(set2, 1);

    testAreDisjoint(set1, set2, false);

    removeItem(set1, 1);

    testAreDisjoint(set1, set2, true);

    insertItem(set1, 2);
    insertItem(set1, 3);

    testAreDisjoint(set1, set2, true);

    insertItem(set2, 2);

    testAreDisjoint(set1, set2, false);

    deleteSet(set1);
    deleteSet(set2);

    testMemoryUsage();
    // -----

    // Tests for unionOf()
    set1 = newSet();
    set2 = newSet();
    expected = newSet();

    testUnionOf(set1, set2, expected);

    insertItem(set1, 1);
    insertItem(expected, 1);

    testUnionOf(set1, set2, expected);

    removeItem(set1, 1);
    removeItem(expected, 1);
    insertItem(set2, 1);
    insertItem(expected, 1);

    testUnionOf(set1, set2, expected);

    insertItem(set1, 1);

    testUnionOf(set1, set2, expected);

    insertItem(set1, 2);
    insertItem(set1, 3);
    insertItem(set1, 4);
    insertItem(expected, 2);
    insertItem(expected, 3);
    insertItem(expected, 4);

    testUnionOf(set1, set2, expected);

    deleteSet(set1);
    deleteSet(set2);
    deleteSet(expected);

    testMemoryUsage();
    // -----

    // Tests for symmetricDifferenceOf()
    set1 = newSet();
    set2 = newSet();
    expected = newSet();

    testSymmetricDifference(set1, set2, expected);

    insertItem(set1, 1);
    insertItem(expected, 1);

    testSymmetricDifference(set1, set2, expected);

    insertItem(set2, 1);
    removeItem(expected, 1);

    testSymmetricDifference(set1, set2, expected);

    insertItem(set1, 1);

    testSymmetricDifference(set1, set2, expected);

    insertItem(set1, 2);
    insertItem(set1, 3);
    insertItem(set1, 4);
    insertItem(set2, 2);

    insertItem(expected, 3);
    insertItem(expected, 4);

    testSymmetricDifference(set1, set2, expected);

    removeItem(set1, 1);
    insertItem(expected, 1);

    testSymmetricDifference(set1, set2, expected);

    deleteSet(set1);
    deleteSet(set2);
    deleteSet(expected);

    testMemoryUsage();
    // -----

    printf("TESTS RUN: %d\n", numTestsRun);
    printf("TESTS PASSED: %d\n", numTestsPassed);
    printf("TESTS FAILED: %d\n", numTestsRun - numTestsPassed);
    return EXIT_SUCCESS;
}

// testNewSet
//
// PURPOSE: This function tests the newSet() function
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// -
void testNewSet() {
    Set *new;

    printf("Testing if a set was successfully created.\n");

    new = newSet();

    if (new != NULL) {
        printf("SUCCESS: New set successfully created.\n");

        numTestsPassed += 1;
    } else {
        printf("FAILED: New set was not successfully created.\n");
    }

    deleteSet(new);

    numTestsRun += 1;
}

// testDeleteSet
//
// PURPOSE: This function tests the deleteSet() function. 
// Prerequisite: No Set objects other than the one passed are in existence.
// INPUT PARAMETERS:
// theSet (Set *): The set to be deleted.
// OUTPUT PARAMETERS:
// -
void testDeleteSet(Set *theSet) {
    assert(theSet != NULL);

    if (theSet != NULL) {
        printf("Testing if a set was successfully deleted.\n");

        deleteSet(theSet);

        if (validateMemUse()) {
            printf("SUCCESS: Set successfully deleted.\n");

            numTestsPassed += 1;
        } else {
            printf("FAILED: Set improperly freed.\n");
        }

        numTestsRun += 1;      
    }
}

// testInsertItem
//
// PURPOSE: This function performs a test on the insertItem() function.
// INPUT PARAMETERS:
// newItem (int): The item to insert
// expected (Boolean): Whether or not the new item should be inserted
// OUTPUT PARAMETERS:
// theSet (Set * const): The set to insert into
void testInsertItem(Set * const theSet, const int newItem, Boolean expected) {
    assert(theSet != NULL);

    if (theSet != NULL) {
        printf("Testing if %d will be inserted into the set.\n", newItem);

        if (insertItem(theSet, newItem) == expected) {
            printf("SUCCESS: The item was%s inserted into the set.\n", expected ? "" : " not");

            numTestsPassed += 1;
        } else {
            printf("FAILED: insertItem() returned the wrong value.\n");
        }

        numTestsRun += 1;
    }
}

// testRemoveItem
//
// PURPOSE: This function performs a test on the removeItem() function.
// INPUT PARAMETERS:
// newItem (int): The item to remove
// expected (Boolean): Whether or not the new item should be removed
// OUTPUT PARAMETERS:
// theSet (Set * const): The set to remove from
void testRemoveItem(Set * const theSet, const int givenItem, Boolean expected) {
    assert(theSet != NULL);

    if (theSet != NULL) {
        printf("Testing if %d will be removed from the set.\n", givenItem);

        if (removeItem(theSet, givenItem) == expected) {
            printf("SUCCESS: removeItem() returned the expected value of %s.\n", expected ? "true" : "false");

            numTestsPassed += 1;
        } else {
            printf("FAILED: removeItem() returned the wrong value.\n");
        }

        numTestsRun += 1;
    }
}

// testAreEqual
//
// PURPOSE: This function performs a test on the areEqual() function.
// INPUT PARAMETERS:
// setA (Set const * const): The first set to be compared
// setB (Set const * const): The second set to be compared
// expected (Boolean): Whether or not the two sets should be equal
// OUTPUT PARAMETERS:
// -
void testAreEqual(Set const * const setA, Set const * const setB, Boolean expected) {
    assert(setA != NULL);
    assert(setB != NULL);

    if (setA != NULL && setB != NULL) {
        printf("Testing if two sets are%s equal.\n", expected ? "" : " not");

        if (areEqual(setA, setB) == expected) {
            printf("SUCCESS: The two sets are%s equal.\n", expected ? "" : " not");

            numTestsPassed += 1;
        } else {
            printf("FAILED: areEqual() did not return the expected value.\n");
        }

        numTestsRun += 1;
    }
}

// testAreDisjoint
//
// PURPOSE: This function performs a test on the areDisjoint() function.
// INPUT PARAMETERS:
// setA (Set const * const): The first set to be compared
// setB (Set const * const): The second set to be compared
// expected (Boolean): Whether or not the two sets should be disjoint
// OUTPUT PARAMETERS:
// -
void testAreDisjoint(Set const * const setA, Set const * const setB, Boolean expected) {
    assert(setA != NULL);
    assert(setB != NULL);

    if (setA != NULL && setB != NULL) {
        printf("Testing if two sets are%s disjoint.\n", expected ? "" : " not");

        if (areDisjoint(setA, setB) == expected) {
            printf("SUCCESS: The two sets are%s disjoint.\n", expected ? "" : " not");

            numTestsPassed += 1;
        } else {
            printf("FAILED: areDisjoint() did not return the expected value.\n");
        }

        numTestsRun += 1;
    }  
}


// testUnionOf
//
// PURPOSE: This function performs a test on the unionOf() function.
// INPUT PARAMETERS:
// setA (Set const * const): The first set to operate on
// setB (Set const * const): The second set to operate on
// expected (Set const * const): The expected return value for unionOf()
// OUTPUT PARAMETERS:
// -
void testUnionOf(Set const * const setA, Set const * const setB, Set const * const expected) {
    Set *actual;

    assert(setA != NULL);
    assert(setB != NULL);
    assert(expected != NULL);

    if (setA != NULL && setB != NULL && expected != NULL) {
        printf("Testing the union of two sets.\n");

        actual = unionOf(setA, setB);

        if (areEqual(actual, expected)) {
            printf("SUCCESS: The union of the two sets matches what is expected.\n");

            numTestsPassed += 1;
        } else {
            printf("FAILED: The union of the two sets does not match what is expected.\n");
        }

        deleteSet(actual);

        numTestsRun += 1;
    }
}

// testSymmetricDifference
//
// PURPOSE: This function performs a test on the symmetricDifferenceOf() function.
// INPUT PARAMETERS:
// setA (Set const * const): The first set to operate on
// setB (Set const * const): The second set to operate on
// expected (Set const * const): The expected return value for symmetricDifferenceOf()
// OUTPUT PARAMETERS:
// -
void testSymmetricDifference(Set const * const setA, Set const * const setB, Set const * const expected) {
    Set *actual;

    assert(setA != NULL);
    assert(setB != NULL);
    assert(expected != NULL);

    if (setA != NULL && setB != NULL && expected != NULL) {
        printf("Testing the symmetric difference of two sets.\n");

        actual = symmetricDifferenceOf(setA, setB);

        if (areEqual(actual, expected)) {
            printf("SUCCESS: The symmetric difference of the two sets matches what is expected.\n");

            numTestsPassed += 1;
        } else {
            printf("FAILED: The symmetric difference of the two sets does not match what is expected.\n");
        }

        deleteSet(actual);

        numTestsRun += 1;
    }
}

// testMemoryUsage
//
// PURPOSE: This function tests for memory management issues.
// INPUT PARAMETERS:
// -
// OUTPUT PARAMETERS:
// -
void testMemoryUsage() {
    printf("Testing for any memory management issues.\n");

    if (validateMemUse()) {
        printf("SUCCESS: No memory management issues.\n");

        numTestsPassed += 1;
    } else {
        printf("FAILED: A memory issue occurred.\n");
    }

    numTestsRun += 1;
}