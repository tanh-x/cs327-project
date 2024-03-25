#include <cstdlib>

#include "utils/arraylist.hpp"

// Constructs and return the pointer to a new array list
ArrayList* constructArrayList(int initialCapacity) {
    // Only make array lists of positive size/capacity
    if (initialCapacity <= 0) return nullptr;

    auto* list = static_cast<ArrayList*>(malloc(sizeof(ArrayList)));
    list->array = static_cast<void**>(malloc(initialCapacity * sizeof(void*)));
    if (list->array == nullptr) return nullptr;

    list->size = 0;
    list->capacity = initialCapacity;

    return list;
}

// Attempts to insert an element into the array list, which can potentially fail if we need to resize the array,
// but realloc fails, in which case, no insertion can happen.
// Returns whether the insertion was successful
bool arrayList_insert(ArrayList* list, void* element) {
    // Check if we need to resize the array list
    if (list->size == list->capacity) {
        list->capacity *= 2;
        void** resized = static_cast<void**>(realloc(list->array, list->capacity * sizeof(void*)));
        if (resized == nullptr) return false;
        list->array = resized;
    }

    list->array[list->size++] = element;
    return true;
}

// Frees the array list. Does not free the elements of the array list
void arrayList_free(ArrayList* list) {
    free(list->array);
    list->array = nullptr;
    free(list);
}