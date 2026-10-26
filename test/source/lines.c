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

MunitTest lines_tests[] = {
    {
        "/lines/xtor",
        test_lines_xtor,
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

