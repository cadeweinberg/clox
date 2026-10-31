/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "test/value.h"
#include "clox/value.h"

static MunitResult test_value_array_xtor(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

static MunitResult test_value_array_write_read(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

MunitTest value_tests[] = {
    {
        "array/xtor",
        test_value_array_xtor,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "array/write_read",
        test_value_array_write_read,
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

static MunitResult test_value_array_xtor(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    ValueArray v;
    value_array_ctor(&v);
    munit_assert_uint32(v.count, ==, 0);
    munit_assert_uint32(v.capacity, ==, 0);
    munit_assert_ptr(v.values, ==, NULL);

    value_array_dtor(&v);
    munit_assert_uint32(v.count, ==, 0);
    munit_assert_uint32(v.capacity, ==, 0);
    munit_assert_ptr(v.values, ==, NULL);

    return MUNIT_OK;
}


static MunitResult test_value_array_write_read(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    ValueArray v;
    value_array_ctor(&v);
    munit_assert_uint32(v.count, ==, 0);
    munit_assert_uint32(v.capacity, ==, 0);
    munit_assert_ptr(v.values, ==, NULL);

    uint32_t i0 = value_array_write(&v, 0.0);
    Value v0    = value_array_read(&v, i0);
    munit_assert_double_equal(v0, 0.0, 9);

    value_array_dtor(&v);
    munit_assert_uint32(v.count, ==, 0);
    munit_assert_uint32(v.capacity, ==, 0);
    munit_assert_ptr(v.values, ==, NULL);

    return MUNIT_OK;
}





