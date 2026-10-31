/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "test/chunk.h"
#include "clox/chunk.h"
#include "munit.h"


// we need to test the contracts of 
// 1. chunk_write
// 2. chunk_get_source_line
// 3. chunk_add_constant
// 4. chunk_get_constant
// 5. chunk_init
// 6. chunk_free

static MunitResult test_chunk_xtor([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *);

static MunitResult test_chunk_code_write([[maybe_unused]] const MunitParameter *,
                                    [[maybe_unused]] void*);

static MunitResult test_chunk_source_line_read(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void*);

static MunitResult test_chunk_constant_read_write(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

static MunitResult test_chunk_constant_read_write_extended(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

MunitTest chunk_tests[] = {
    {
        "xtor",
        test_chunk_xtor,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        "code_write",
        test_chunk_code_write,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "source_line_read",
        test_chunk_source_line_read,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "constant_read_write",
        test_chunk_constant_read_write,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "constant_read_write_extended",
        test_chunk_constant_read_write_extended,
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

static MunitResult test_chunk_code_write([[maybe_unused]] const MunitParameter *,
                                    [[maybe_unused]] void *) {
    Chunk c;
    chunk_ctor(&c);

    chunk_code_write_return(&c, 1);
    munit_assert_uint32(c.count, >, 0);
    munit_assert_uint32(c.capacity, >, 0);
    munit_assert_ptr(c.code, !=, NULL);

    chunk_dtor(&c);
    return MUNIT_OK;
}

static MunitResult test_chunk_source_line_read(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    Chunk c;
    chunk_ctor(&c);

    chunk_code_write_return(&c, 1);
    munit_assert_uint32(c.count, >, 0);
    munit_assert_uint32(c.capacity, >, 0);
    munit_assert_ptr(c.code, !=, NULL);

    uint32_t line = chunk_source_line_read(&c, 0);
    munit_assert_uint32(line, ==, 1);

    chunk_dtor(&c);
    return MUNIT_OK;
}

static MunitResult test_chunk_constant_read_write(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    Chunk c;
    chunk_ctor(&c);

    chunk_code_write_constant(&c, 0.1, 1);

    Value value = 0.0;

    if (c.code[0] == OP_CONSTANT) {
        uint8_t index = c.code[1];
        value = chunk_constant_read(&c, index);
    } else {
        return MUNIT_FAIL;
    }
    
    munit_assert_double_equal(value, 0.1, 9);

    chunk_dtor(&c);
    return MUNIT_OK;
}

static constexpr uint32_t NUM_CONSTANTS = 1000;

static MunitResult test_chunk_constant_read_write_extended(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    Chunk chunk;
    chunk_ctor(&chunk);
    Value expected_values[NUM_CONSTANTS] = {};

    for (uint32_t i = 0; i < NUM_CONSTANTS; ++i) {
        expected_values[i] = munit_rand_double();
        chunk_code_write_constant(&chunk, expected_values[i], 1);
    }
    chunk_code_write_return(&chunk, 2);

    uint32_t ip = 0;
    uint32_t expected_index = 0;
    while (true) {
        Value actual = 0.0;
        Value expected = expected_values[expected_index];
        uint32_t line = chunk_source_line_read(&chunk, ip);

        if (chunk.code[ip] == OP_RETURN) {
            munit_assert_uint32(line, ==, 2);
            break;
        } else if (chunk.code[ip] == OP_CONSTANT) {
            uint8_t actual_index = chunk.code[ip + 1];
            actual = chunk_constant_read(&chunk, actual_index);
            ip += 2;
        } else if (chunk.code[ip] == OP_CONSTANT_EXTENDED) {
            uint32_t actual_index = 0;
            memcpy(&actual_index, chunk.code + ip + 1, sizeof(uint32_t));
            actual = chunk_constant_read(&chunk, actual_index);
            ip += 5;
        } else {
            // We *should* never executed this statement.
            return MUNIT_FAIL;
        }

        munit_assert_uint32(line, ==, 1);
        munit_assert_double_equal(actual, expected, 9);
        ++expected_index;
    }

    chunk_dtor(&chunk);
    return MUNIT_OK;
}




