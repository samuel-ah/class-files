# COMP 2160 Assignment 3, Question 2: Testing results

## Compilation:
To change which set is being tested, edit the `SET` path in the `Makefile` to the appropriate set located in `./setsToTest` and recompile with `make`.

## Bugs in `set2.o`:

### 1. `areDisjoint()`
- `areDisjoint()` does not properly manage its memory, resulting in a memory leak.

### 2. `unionOf()`
- `unionOf()` does not properly manage its memory, resulting in a memory leak.

### 3. `symmetricDifferenceOf()`
- `symmetricDifferenceOf()` does not properly manage its memory, resulting in a memory leak.

## Bugs in `set3.o`:

### 1. `insertItem()`
- `insertItem()` incorrectly inserts duplicate items into the set.

### 2. `removeItem()`
- `removeItem()` always returns `false`, and does not remove items from the set.

## Bugs in `set4.o`

### 1. `areDisjoint()`
- `areDisjoint(setA, setB)` returns `false` when `setB` is empty, regardless of the contents of `setA`.

### 2. `symmetricDifferenceOf()`
- `symmetricDifferenceOf(setA, setB)` always returns the union of `setA` and `setB`, rather than their symmetric difference.

## Bugs in `set5.o`

### `areDisjoint()`
- `areDisjoint(setA, setB)` always returns `true`, regardless of input.

## Bugs in `set6.o`

### `areEqual()`
- `areEqual(setA, setB)` always returns `false`, regardless of input.

### Note:
Testing for `unionOf()` and `symmetricDifferenceOf()` requires comparing the returned `Set` from both functions against the expected output. As `areEqual()` is the only way to compare two `Set`s without a complicated workaround involving `areDisjoint()`, it is practically impossible to reliably test `unionOf()` and `symmetricDifferenceOf()` if it is not functioning properly.