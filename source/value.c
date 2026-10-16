/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <stdio.h>

#include "clox/memory.h"
#include "clox/value.h"

void value_array_init(ValueArray *array) {
    assert(array != NULL);
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void value_array_free(ValueArray *array) {
    assert(array != NULL);
    reallocate((void**)&array->values, array->capacity * sizeof(*array->values), 0);
    value_array_init(array);
}

static bool value_array_full(const ValueArray *array) {
    return array->capacity < (array->count + 1);
}

static void value_array_grow(ValueArray *array) {
    static const ArrayGrowthInfo info = {.growth_factor = 2, .initial_capacity = 8};
    array_growth(info, sizeof(*array->values), (void**)&array->values, &array->capacity);
}

uint32_t value_array_write(ValueArray *array, Value value) {
    assert(array != NULL);
    if (value_array_full(array)) {
        value_array_grow(array);
    }

    array->values[array->count++] = value;
    return array->count - 1;
}

Value value_array_get(ValueArray *array, uint32_t index) {
    assert(array != NULL);
    assert(array->count > index);
    return array->values[index];
}

void value_print(Value value) {
    printf("%g", value);
}
