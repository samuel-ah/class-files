#ifndef _main_h
#define _main_h

#include "set.h"

// Tests the newSet() function
void testNewSet();
// Tests the deleteSet() function, given the only Set instance currently in existence
void testDeleteSet(Set *theSet);
// Performs a test on the insertItem() function, given a Set, an item to insert, and whether it should be inserted
void testInsertItem(Set * const theSet, const int newItem, Boolean expected);
// Performs a test on the removeItem() function, given a Set, an item to insert, and whether it should be removed
void testRemoveItem(Set * const theSet, const int givenItem, Boolean expected);
// Performs a test on the areEqual() function, given two Sets and whether or not they should be equal
void testAreEqual(Set const * const setA, Set const * const setB, Boolean expected);
// Performs a test on the areDisjoint() function, given two Sets and whether or not they should be disjoint
void testAreDisjoint(Set const * const setA, Set const * const SetB, Boolean expected);
// Performs a test on the unionOf() function, given two Sets and an expected output Set
void testUnionOf(Set const * const setA, Set const * const setB, Set const * const expected);
// Performs a test on the unionOf() function, given two Sets and an expected output Set
void testSymmetricDifference(Set const * const setA, Set const * const setB, Set const * const expected);
// Tests for memory management issues
void testMemoryUsage();

#endif