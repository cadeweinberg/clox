/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "test/lines.h"
#include "clox/lines.h"

static MunitResult test_lines_xtor([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void*);

static MunitResult test_lines_xet([[maybe_unused]] const MunitParameter *,
                                  [[maybe_unused]] void *);

MunitTest lines_tests[] = {
    {
        "xtor",
        test_lines_xtor,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "xet",
        test_lines_xet,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        NULL,
        NULL,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    }
};

static MunitResult test_lines_xtor([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void*) {
    Lines l;
    lines_ctor(&l);
    munit_assert_uint32(l.count, ==, 0);
    munit_assert_uint32(l.capacity, ==, 0);
    munit_assert_ptr(l.runs, ==, NULL);

    lines_dtor(&l);
    munit_assert_uint32(l.count, ==, 0);
    munit_assert_uint32(l.capacity, ==, 0);
    munit_assert_ptr(l.runs, ==, NULL);

    return MUNIT_OK;
}

static MunitResult test_lines_xet([[maybe_unused]] const MunitParameter *,
                                  [[maybe_unused]] void *) {
    Lines l;
    lines_ctor(&l);

    // source lines 1 - 10 produce 10 instructions each
    // line 1: will have instructions 0 - 9
    // 2: 10 - 19
    // 3: 20 - 29
    // 4: 30 - 39
    // ...
    // 9: 90 - 99
    // we need a counter i that goes 1 - 10
    // and a counter j that goes 0 - 99
    // and we want j to count in bounds of ten for 
    // each iteration of i. so we want a nested for
    // loop. just, what is the equation that expresses
    // this equality?
    for (uint32_t i = 1; i < 11; ++i) {
        for (uint32_t j = 0; j < 10; ++j) {
            // i: j - j + 9
            // i + 1: j - j + 9
            // line = i
            // index = j
            // 
            // line = 1
            // and we want j to be 0 - 9
            // i, j
            //
            // line = 2
            // and we want j to be 10 - 19
            // i, j + 10
            //
            // line = 3
            // and we want j to be 20 - 29
            // i, j + 20
            //
            //
            // [i == 1] 0  = (i - 1) * 10
            // [i == 2] 10 = (i - 1) * 10
            // [i == 3] 20 = (i - 1) * 10
            // [i == 4] 30 = (i - 1) * 10
            //
            // i, j + ((i - 1) * 10)
            lines_set(&l, i, j + ((i - 1) * 10));
        }
    }
    
    // to test this, we can run the same loop, and compare 
    // the retrieved value with the expected value.
    for (uint32_t i = 1; i < 11; ++i) {
        for (uint32_t j = 0; j < 10; ++j) {
            uint32_t line = lines_get(&l, j + ((i - 1) * 10));
            // the index we access will increment 0 - 99
            // so we just need to check that the returned
            // line is equal to i
            munit_assert_uint32(i, ==, line);
        }
    }

    lines_dtor(&l);
    return MUNIT_OK;
}


