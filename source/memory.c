/**
 * Copyright (c) 2026 cade
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>

#include "clox/memory.h"

// note that old size is unused, at most until we add garbage collection. when we get to that chapter.
bool reallocate(void **pointer, [[maybe_unused]] size_t old_size, size_t new_size) {
    assert(pointer != NULL);
    // note that *pointer is allowed to be NULL, as this implies a first time allocation.

    if (new_size == 0) {
        free(*pointer);
        *pointer = NULL;
        return true;
    }

    void *result = realloc(*pointer, new_size);
    if (result == NULL) { return false; }

    (*pointer) = result;
    return true;
}


bool array_grow(ArrayGrowInfo info, size_t element_size, void **array, uint32_t *capacity) {
    assert(array != NULL);
    assert(capacity != NULL);
    assert(element_size > 0);
    
    if (*capacity == 0) { 
        (*capacity) = info.initial_capacity;
    } else {
        (*capacity) *= info.growth_factor;
    }

    size_t size = element_size * (*capacity);

    void *ptr = realloc(*array, size);
    if (ptr == NULL) { return false; }
    
    *array = ptr;
    return true;
}

