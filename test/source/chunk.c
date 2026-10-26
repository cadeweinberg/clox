/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "test/chunk.h"
#include "clox/chunk.h"


// we need to test the contracts of 
// 1. chunk_write
// 2. chunk_get_source_line
// 3. chunk_add_constant
// 4. chunk_get_constant
// 5. chunk_init
// 6. chunk_free

static MunitResult test_chunk_xtor([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *);

static MunitResult test_chunk_write([[maybe_unused]] const MunitParameter *,
                                    [[maybe_unused]] void*);

static MunitResult test_chunk_get_source_line(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void*);

static MunitResult test_chunk_add_get_constant(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

MunitTest chunk_tests[] = {
    {
        "/chunk/xtor",
        test_chunk_xtor,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "/chunk/write",
        test_chunk_write,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "chunk/get_source_line",
        test_chunk_get_source_line,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/chunk/add_get_constant",
        test_chunk_add_get_constant,
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
        NULL
    }
};


static MunitResult test_chunk_xtor([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *) {
    Chunk chunk;
    chunk_ctor(&chunk);
    munit_assert_uint32(chunk.count, ==, 0);
    munit_assert_uint32(chunk.capacity, ==, 0);
    munit_assert_ptr(chunk.code, ==, NULL);

    chunk_dtor(&chunk);
    munit_assert_uint32(chunk.count, ==, 0);
    munit_assert_uint32(chunk.capacity, ==, 0);
    munit_assert_ptr(chunk.code, ==, NULL);

    return MUNIT_OK;
}

static MunitResult test_chunk_write([[maybe_unused]] const MunitParameter *,
                                    [[maybe_unused]] void *) {
    Chunk c;
    chunk_ctor(&c);

    chunk_write(&c, OP_RETURN, 1);
    munit_assert_uint32(c.count, >, 0);
    munit_assert_uint32(c.capacity, >, 0);
    munit_assert_ptr(c.code, !=, NULL);

    chunk_dtor(&c);
    return MUNIT_OK;
}

static MunitResult test_chunk_get_source_line(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    Chunk c;
    chunk_ctor(&c);

    chunk_write(&c, OP_RETURN, 1);
    munit_assert_uint32(c.count, >, 0);
    munit_assert_uint32(c.capacity, >, 0);
    munit_assert_ptr(c.code, !=, NULL);

    uint32_t line = chunk_get_source_line(&c, 0);
    munit_assert_uint32(line, ==, 1);

    chunk_dtor(&c);
    return MUNIT_OK;
}

static MunitResult test_chunk_add_get_constant(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    Chunk c;
    chunk_ctor(&c);

    uint32_t constant = chunk_add_constant(&c, 0.1);
    Value value = chunk_get_constant(&c, constant);
    munit_assert_double_equal(value, 0.1, 9);

    chunk_dtor(&c);
    return MUNIT_OK;
}




