/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "test/vm.h"
#include "clox/vm.h"

static void *test_vm_negate_setup(
    [[maybe_unused]] const MunitParameter *params,
    [[maybe_unused]] void *data);

static void test_vm_negate_teardown(
    [[maybe_unused]] void *data);

static MunitResult test_vm_negate(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

MunitTest vm_tests[] = {
    {
        "/negate",
        test_vm_negate,
        test_vm_negate_setup,
        test_vm_negate_teardown,
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

static void *test_vm_negate_setup(
    [[maybe_unused]] const MunitParameter *params,
    [[maybe_unused]] void *data) {
    VM *vm = malloc(sizeof(VM));
    vm_ctor(vm);
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk_ctor(chunk);

    chunk_code_write_constant(chunk, 1.0, 1);

    for (uint32_t i = 0; i < 1; ++i) {
        chunk_code_write_negate(chunk, 1);
    }

    chunk_code_write_return(chunk, 1);

    vm->chunk = chunk;
    return vm;
}

static void test_vm_negate_teardown(
    [[maybe_unused]] void *data) {
    VM *vm = (VM *)data;
    chunk_dtor(vm->chunk);
    free(vm->chunk);
    vm_dtor(vm);
    free(vm);
}

static MunitResult test_vm_negate(
    [[maybe_unused]] const MunitParameter *,
    void *data) {
    // to measure the performance of the vm executing the 
    // negate instruction, (to see if we can tell the difference
    // between push/pop and peek implementations of negate) we 
    // move the xtor out into the setup/teardown functions.
    VM* vm = (VM *)data;
    InterpretResult result = vm_interpret(vm, vm->chunk);

    munit_assert(result == INTERPRET_OK);
    return MUNIT_OK;
}

