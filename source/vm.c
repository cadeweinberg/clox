/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "clox/vm.h"
#include "clox/chunk.h"
#include "clox/debug.h"
#include "clox/stack.h"

#define DEBUG_TRACE

static InterpretResult run(VM *vm);

void vm_ctor(VM *vm) {
    assert(vm != NULL);
    vm->chunk = NULL;
    vm->ip    = NULL;
    stack_ctor(&vm->stack);
}

void vm_dtor(VM *vm) {
    assert(vm != NULL);
    vm->chunk = NULL;
    vm->ip    = NULL;
    stack_dtor(&vm->stack);
}

InterpretResult vm_interpret(VM *vm, Chunk *chunk) {
    assert(vm != NULL);
    assert(chunk != NULL);

    vm->chunk = chunk;
    vm->ip    = chunk->code;

    return run(vm);
}

static uint8_t read_byte(VM *vm) {
    return *vm->ip++;
}

static Value read_constant(VM *vm) {
    uint8_t index = read_byte(vm);
    return chunk_constant_read(vm->chunk, index);
}

static Value read_constant_extended(VM *vm) {
    uint32_t index;
    memcpy(&index, vm->ip, sizeof(uint32_t));
    vm->ip += sizeof(uint32_t);
    return chunk_constant_read(vm->chunk, index);
}

static InterpretResult run(VM *vm) {
#ifdef DEBUG_TRACE
    uint8_t *code = vm->chunk->code;
#endif

    while (true) {

#ifdef DEBUG_TRACE
        disassemble_instruction(stderr, vm->chunk, (uint32_t)(vm->ip - code));
#endif

        uint8_t instruction;
        switch (instruction = read_byte(vm)) {
        case OP_RETURN: {
            Value result = 0.0;
            stack_pop(&vm->stack, &result);
            value_print(stderr, result);
            fprintf(stderr, "\n");
            return INTERPRET_OK;
        }

        case OP_CONSTANT: {
            Value constant = read_constant(vm);
            stack_push(&vm->stack, constant);
            break;
        }

        case OP_CONSTANT_EXTENDED: {
            Value constant = read_constant_extended(vm);
            stack_push(&vm->stack, constant);
            break;
        }

        case OP_NEGATE: {
            // Value temp = 0.0;
            // if (!stack_pop(&vm->stack, &temp)) {
            //     return INTERPRET_RUNTIME_ERROR;
            // }
            Value *view = NULL;
            if (!stack_peek(&vm->stack, &view)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            // stack_push(&vm->stack, -temp);
            (*view) = -(*view);
            break;
        }

        case OP_ADD: {
            Value a;
            Value b;
            if (!stack_pop(&vm->stack, &b)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            if (!stack_pop(&vm->stack, &a)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            stack_push(&vm->stack, a + b);
            break;
        }

        case OP_SUBTRACT: {
            Value a;
            Value b;
            if (!stack_pop(&vm->stack, &b)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            if (!stack_pop(&vm->stack, &a)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            stack_push(&vm->stack, a - b);
            break;
        }

        case OP_MULTIPLY: {
            Value a;
            Value b;
            if (!stack_pop(&vm->stack, &b)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            if (!stack_pop(&vm->stack, &a)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            stack_push(&vm->stack, a * b);
            break;
        }

        case OP_DIVIDE: {
            Value a;
            Value b;
            if (!stack_pop(&vm->stack, &b)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            if (!stack_pop(&vm->stack, &a)) {
                return INTERPRET_RUNTIME_ERROR;
            }

            stack_push(&vm->stack, a / b);
            break;
        }

        }

#ifdef DEBUG_TRACE
        stack_print(stderr, &vm->stack);
#endif 
    }
}

