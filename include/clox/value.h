// Copyright (c) 2026 Cade Weinberg
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "clox/common.h"

typedef double Value;

typedef struct _ValueArray {
    uint32_t count;
    uint32_t capacity;
    Value *values;
} ValueArray;

void value_print(Value value);

void value_array_ctor(ValueArray *array);
void value_array_dtor(ValueArray *array);
uint32_t value_array_write(ValueArray *array, Value value);
Value value_array_read(ValueArray *array, uint32_t index);

#endif // !CLOX_VALUE_H

