//-----------------------------------------
// NAME: Sam Heppner
// STUDENT NUMBER: 7998801
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Tim Zapp
// ASSIGNMENT: 3, QUESTION: 1
// 
// REMARKS: Interface for the testing suite for the Container data type
//
//-----------------------------------------

#ifndef _main_h
#define _main_h

#include "container.h"

// Tests creating a container
void testCreateContainer();
// Tests destroying a given container
void testDestroyContainer(Container *cont);
// Tests inserting a given item into a given container
void testInsert(Container *cont, char *item);
// Tests deleting a given item from a given container
void testDelete(Container *cont, char *item, Boolean sizeShouldDecrease);
// Tests clearing a given container
void testClear(Container *cont);
// Tests whether the first item in a given container is the given item
void testFirstItem(Container *cont, char *expected);
// Tests whether the next item in a given container is the given item
void testNextItem(Container *cont, char *expected);
// Tests whether or not a given container contains a given item
void testContains(Container *cont, char *item, Boolean expected);
// Tests to see if the actual size of a given container matches what is expected
void testSize(Container *cont, int expected);
// Returns the number of items in a given container matching the given item
int numItemsMatching(Container *cont, char *item);

#endif