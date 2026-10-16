// Copyright (c) 2026 cade
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "clox/common.h"
#include "clox/value.h"
#include <stdint.h>

typedef enum {
    OP_RETURN,
    OP_CONSTANT,
} OpCode;

typedef struct {
    uint32_t count;
    uint32_t capacity;
    uint8_t *code;
    uint32_t *lines;
    ValueArray constants;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_free(Chunk *chunk);
void chunk_write(Chunk *chunk, uint8_t data, uint32_t line);
uint32_t chunk_add_constant(Chunk *chunk, Value value);
Value chunk_get_constant(Chunk *chunk, uint32_t index);


#endif // !CLOX_CHUNK_H

