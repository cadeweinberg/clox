// Copyright (c) 2026 Cade Weinberg
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_STACK_H
#define CLOX_STACK_H

#include <stdint.h>
#include <stdio.h>

#include "clox/value.h"

typedef struct _Stack {
    uint32_t count;
    uint32_t capacity;
    Value *stack;
} Stack;

void stack_ctor(Stack *stack);
void stack_dtor(Stack *stack);

void stack_push(Stack *stack, Value value);
bool stack_peek(Stack *stack, Value **out);
bool stack_pop(Stack *stack, Value *out);

void stack_print(FILE *out, Stack *stack);

#endif // !CLOX_STACK_H

