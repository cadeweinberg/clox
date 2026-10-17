// Copyright (c) 2026 cade
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#include "clox/common.h"

/**
 * @brief a wrapper around c stdlib 'realloc' with the additional
 * behavior that passing in zero new_size calls c stdlib 'free'.
 *
 * @pre pointer != NULL
 * @post (*pointer) will point to an allocation with `size == new_size`
 * @post the pointer given to reallocate is not modified in the case that 
 * the realloc failed.
 *
 * @post the pointer is set to NULL if the memory it referenced was free'd.
 *
 * @note we are wrapping 'realloc' because while following the crafting 
 * interpreters tutorial, we are going to implement garbage collection.
 * wrapping realloc, malloc and free, allows updating the virtuals machine
 * to use garbage collection without a major overhaul of the codebase.
 * This is the sort of descision that you make when you are planning ahead.
 * so it is a good thing to note here, for future codebases. For instance
 * when considering the firmware for the limnmoco, what choices will we be
 * making about the structure of that code, that will have an affect (effect?)
 * on the choices and refactors we can make down the line. something to 
 * keep in mind.
 *
 * @note if we write our own malloc implementation, we can remove the 
 * 'old_size' parameter, this is because that information is held within
 * the block of memory's metadata. so even if/when we want to add garbage 
 * collection. we can use the allocation metadata to retrieve the old size
 * for garbage collection purposes.
 *
 * @return true if realloc succeeded
 * @return false if realloc failed
 */
bool reallocate(void **pointer, size_t old_size, size_t new_size);

// Normally I would make this structure pass-by-pointer
// however, given that both growth_factor and initial_capacity
// are usually small numbers. a uint8_t is probably sufficient
// to hold both common values. which is able to be packed by 
// the compiler into a single word, and passed by value into the 
// function, with the same overhead as passing a pointer.
// with the benefit that the function can use the values 
// passed directly. without having to resolve the indirection.
// in fact, even as two uint32_t's this is probably able to happen,
// if they are packed into a single 32bit word, the bitshifting 
// is slightly less performant that just reading a single word.
// Though the difference is likely to be neglidgable and very 
// fast either way.

typedef struct _ArrayGrowInfo {
    uint16_t growth_factor;
    uint16_t initial_capacity;
} ArrayGrowInfo;

/**
 * @brief grow the given array, updating the capacity.
 *
 * @note The ArrayGrowthInfo structure allows the caller to configure 
 * the rate of growth and the initial capacity of arrays grown 
 * with this function.
 * 
 * @note array_grow is implemented in terms of tha above reallocate, and 
 * as such it's memory will be participating in garbage collection when that
 * is added to the language.
 *
 * @pre info.growth_factor != 0
 * @pre info.initial_capacity != 0
 * @pre element_size > 0
 * @pre array != NULL
 * @pre capacity != NULL
 * @post `(*capacity) == initial_capacity` if `(*capacity) == 0`; else `(*capacity) == (*capacity) * info.growth_factor` 
 * @post `(*array)` points to an allocation with `size == element_size * info.initial_capacity`
 *       if `(*capacity) == 0`; else `element_size * (*capacity)` 
 *
 * @return true if growth succeeded
 * @return false if growth failed
 */
bool array_grow(ArrayGrowInfo info, size_t element_size, void **array, uint32_t *capacity);

#endif // !CLOX_MEMORY_H

