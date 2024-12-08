#ifndef _TEST_H
#define _TEST_H

#include "ObjectManager.h"

// Tests inserting an object of given size
Ref testInsertObject(ulong size, bool shouldInsert);
// Tests retrieving a given reference
void testRetrieveObject(Ref ref, bool shouldExist);
// Tests adding a reference to given ref
// prereq: All objects must have 0 references
// Testing checks to see if given ref will remain in existence after garbage collection.
void testAddReference(Ref ref, ulong size);
// Tests dropping a reference from given ref
// prereq: All other objects must have 0 references, and ref must only have one.
// Testing checks to see if given ref is collected on triggering collection.
void testDropReference(Ref ref);
// Tests initPool() by printing a message if no assertions or crashes occur.
void testInitPool();
// Tests destroyPool() by printing a message if no assertions or crashes occur.
void testDestroyPool();

#endif