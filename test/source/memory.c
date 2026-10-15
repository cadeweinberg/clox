/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "clox/memory.h"
#include "test/memory.h"

static MunitResult test_reallocate([[maybe_unused]] const MunitParameter *params, [[maybe_unused]] void *data);

MunitTest memory_tests[] = {
    {"/reallocate", test_reallocate, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
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
