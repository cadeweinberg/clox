// Copyright (c) 2026 Cade Weinberg
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "clox/chunk.h"
#include "clox/stack.h"

typedef struct _VM {
    Chunk *chunk;
    uint8_t *ip;
    Stack stack;
} VM;

typedef enum _InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void vm_ctor(VM *vm);
void vm_dtor(VM *vm);

InterpretResult vm_interpret(VM *vm, Chunk *chunk);

#endif // !CLOX_VM_H
