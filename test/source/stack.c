/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "test/stack.h"
#include "clox/stack.h"
#include "munit.h"

static MunitResult test_stack_xtor([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *);

static MunitResult test_stack_push([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *);

static MunitResult test_stack_peek([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *);

static MunitResult test_stack_pop([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *);

MunitTest stack_tests[] = {
    {
        "/xtor",
        test_stack_xtor,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/push",
        test_stack_push,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/peek",
        test_stack_peek,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/pop",
        test_stack_pop,
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

static MunitResult test_stack_xtor([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *) {
    Stack stack;
    stack_ctor(&stack);
    munit_assert_uint32(stack.capacity, ==, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, ==, NULL);

    stack_dtor(&stack);
    munit_assert_uint32(stack.capacity, ==, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, ==, NULL);

    return MUNIT_OK;
}

static MunitResult test_stack_push([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *) {
    Stack stack;
    stack_ctor(&stack);
    munit_assert_uint32(stack.capacity, ==, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, ==, NULL);

    Value v0 = 1.0;
    stack_push(&stack, v0);
    munit_assert_uint32(stack.capacity, >, 0);
    munit_assert_uint32(stack.count, >, 0);
    munit_assert_ptr(stack.stack, !=, NULL);

    stack_dtor(&stack);
    munit_assert_uint32(stack.capacity, ==, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, ==, NULL);
    
    return MUNIT_OK;
}

static MunitResult test_stack_peek([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *) {
    Stack stack;
    stack_ctor(&stack);
    munit_assert_uint32(stack.capacity, ==, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, ==, NULL);

    Value v0 = 1.0;
    stack_push(&stack, v0);
    munit_assert_uint32(stack.capacity, >, 0);
    munit_assert_uint32(stack.count, >, 0);
    munit_assert_ptr(stack.stack, !=, NULL);

    Value* v1;
    munit_assert_true(stack_peek(&stack, &v1));
    munit_assert_uint32(stack.capacity, >, 0);
    munit_assert_uint32(stack.count, >, 0);
    munit_assert_ptr(stack.stack, !=, NULL);
    munit_assert_double_equal(v0, *v1, 9);

    stack_dtor(&stack);
    munit_assert_uint32(stack.capacity, ==, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, ==, NULL);
    
    return MUNIT_OK;
}

static MunitResult test_stack_pop([[maybe_unused]] const MunitParameter *,
                                   [[maybe_unused]] void *) {
    Stack stack;
    stack_ctor(&stack);
    munit_assert_uint32(stack.capacity, ==, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, ==, NULL);

    Value v0 = 1.0;
    stack_push(&stack, v0);
    munit_assert_uint32(stack.capacity, >, 0);
    munit_assert_uint32(stack.count, >, 0);
    munit_assert_ptr(stack.stack, !=, NULL);

    Value v1;
    munit_assert_true(stack_pop(&stack, &v1));
    munit_assert_uint32(stack.capacity, >, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, !=, NULL);
    munit_assert_double_equal(v0, v1, 9);

    stack_dtor(&stack);
    munit_assert_uint32(stack.capacity, ==, 0);
    munit_assert_uint32(stack.count, ==, 0);
    munit_assert_ptr(stack.stack, ==, NULL);
    
    return MUNIT_OK;
}