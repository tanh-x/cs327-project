#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct {
    void** array;
    int size;
    int capacity;
} ArrayList;

// Constructs and return the pointer to a new array list
ArrayList* constructArrayList(int initialCapacity);

// Attempts to insert an element into the array list, which can potentially fail if we need to resize the array,
// but realloc fails, in which case, no insertion can happen.
// Returns whether the insertion was successful
bool arrayList_insert(ArrayList* list, void* element);

// Frees the array list. Does not free the elements of the array list
void arrayList_free(ArrayList* list);

#endif
