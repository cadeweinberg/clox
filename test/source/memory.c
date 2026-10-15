/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "clox/memory.h"
#include "test/memory.h"

static MunitResult test_reallocate([[maybe_unused]] const MunitParameter *params, [[maybe_unused]] void *data);

// we need to consider thge structure of the test suite.
// it is declared as a c-style array, similar to the one used 
// by getopt. with a NULL entry at the end to signify the end of 
// the array. Obviating the need for a separate count object.
// The c style array may be stored as a global static, as a local
// member of a function, or as a heap allocated object.
// if we store it as a global, we can extern a pointer to the 
// static via the header, and have main collate the pointers into 
// a suite of it's own.
// or we could declare a function which allocates the array, and have main 
// collate the dynamic memory.

static MunitTest memory_tests[] = {
    {"/reallocate", test_reallocate, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

const MunitSuite memory_suite = {
    "/memory",
    memory_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

static MunitResult test_reallocate([[maybe_unused]] const MunitParameter *params, [[maybe_unused]] void *data) {
    int *ip = NULL;
    munit_assert(reallocate((void**)&ip, 0, sizeof(int)));

    *ip = 4;
    munit_assert_int(*ip, ==, 4);

    munit_assert(reallocate((void**)&ip, sizeof(int), 0));

    // #TODO: test array allocation.

    return MUNIT_OK;
}
