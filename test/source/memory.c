/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "clox/memory.h"
#include "test/memory.h"

// reallocate has the signature
// `bool reallocate(void** pointer, size_t old_size, size_t new_size);`
// it provides the contract:
// 1. reallocate allocates memory from the heap
// 2. the parameter pointer is assigned to the allocated memory
// 3. if the pointer is NULL the memory returned will be newly
//    allocated
// 4. if the pointer is allocated by a previous call to reallocate
//    then reallocate is free to grow the size of the allocation,
//    or provide a new allocation region.
// 5. the size of the allocated region will be equal to the size of 
//    the parameter `new_size` if allocation succeeds.
// 6. if the new_size is zero, the call to reallocate witll behave 
//    as if it were a call to 'free'. returning the allocation to the 
//    free memory available to be allocated.
// 7. if the new_size is zero, and the memory is free'd, then the pointer
//    parameter will be set to NULL.
// 8. if realloc fails, then false is returned.
// 9. if realloc suceeds then true is returned.
// 
// for testing purposes, calling reallocate passing in an object of any size
// asserts 1,2,3,5,9
// calling reallocate on that allocation with a new_size of zero 
// asserts 6,7,9
// using the memory returned 
// asserts 5
// and 4 doesn't need to be tested, because it is an implementation detail
// of reallocate. it is a `nice to have` not something essential about the contract.
// (side-note what is the proper way to state that sort of portion of the contract,
//  or api?)
//
// testing reallocate by allocating a single integer allows us to establish a basic working 
// implementation. allocating an array of objects allows us to establish that the implementation
// is correctly responding to the size parameter.
// We could devise a series of tests which stress test the implementation of reallocate, with 
// a lot of varied sized blocks of memory, in order to see how it operates under load.
// especially if we are rolling our own implementation, this sort of metric is particularly 
// useful in deriving facts about the pros-and-cons of different styles of allocator implementation.
// as the current iteration is based on stdlib 'realloc' we will take as a given that the implementation
// is well tested. When we roll our own, we will derive more tests, and see if we can write a good
// version of a malloc/realloc/free.
//

static MunitResult test_reallocate_simple([[maybe_unused]] const MunitParameter *params, [[maybe_unused]] void *data);
static MunitResult test_reallocate_array([[maybe_unused]] const MunitParameter *params, [[maybe_unused]] void *data);


// array grow has the signature

MunitTest memory_tests[] = {
    {"/reallocate", test_reallocate_simple, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {"/reallocate/array", test_reallocate_array, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static MunitResult test_reallocate_simple([[maybe_unused]] const MunitParameter *params, [[maybe_unused]] void *data) {
    int *ip = NULL;
    munit_assert(reallocate((void**)&ip, 0, sizeof(int)));
    munit_assert(ip != NULL);

    *ip = 4;
    munit_assert_int(*ip, ==, 4);

    munit_assert(reallocate((void**)&ip, sizeof(int), 0));
    munit_assert(ip == NULL);

    return MUNIT_OK;
}

static MunitResult test_reallocate_array([[maybe_unused]] const MunitParameter *params, [[maybe_unused]] void * data) {
    int length = munit_rand_int_range(10, 100);
    int *ap = NULL;
    size_t size = sizeof(*ap) * (size_t)length;
    munit_assert(reallocate((void**)&ap, 0, size));


    for (int i = 0; i < length; ++i) {
        ap[i] = i;
    }

    for (int i = 0; i < length; ++i) {
        munit_assert_int(i, ==, ap[i]);
    }

    munit_assert(reallocate((void**)&ap, size, 0));

    return MUNIT_OK;
}