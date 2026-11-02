/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <stdlib.h>

#include "clox/stack.h"
#include "clox/memory.h"

void stack_ctor(Stack *stack) {
    assert(stack != NULL);
    stack->count = 0;
    stack->capacity = 0;
    stack->stack = NULL;
}

void stack_dtor(Stack *stack) {
    assert(stack != NULL);
    reallocate((void**)&stack->stack, stack->capacity * sizeof(*stack->stack), 0);
    stack->count = 0;
    stack->capacity = 0;
}

static bool stack_full(const Stack *stack) {
    return stack->capacity < (stack->count + 1);
}

static void stack_grow(Stack *stack) {
    ArrayGrowInfo info = {.growth_factor = 2, .initial_capacity = 8};
    if (!array_grow(info, sizeof(*stack->stack), (void**)&stack->stack, &stack->capacity)) {
        exit(EXIT_FAILURE);
    }
}

void stack_push(Stack *stack, Value value) {
    assert(stack != NULL);
    if (stack_full(stack)) {
        stack_grow(stack);
    }

    stack->stack[stack->count++] = value;
}

bool stack_peek(Stack *stack, Value **out) {
    assert(stack != NULL);
    assert(out != NULL);
    if (stack->count == 0) { return false; }

    *out = &stack->stack[stack->count - 1];
    return true;
}

bool stack_pop(Stack *stack, Value *out) {
    assert(stack != NULL);
    assert(out != NULL);
    if (stack->count == 0) { return false; }

    *out = stack->stack[--stack->count];
    return true;
}

void stack_print(FILE *out, Stack *stack) {
    assert(out != NULL);
    assert(stack != NULL);
    fprintf(out, "        ");
    for (uint32_t i = 0; i < stack->count; ++i) {
        fprintf(out, "[");
        value_print(out, stack->stack[i]);
        fprintf(out, "]");
    }
    fprintf(out, "\n");
}

