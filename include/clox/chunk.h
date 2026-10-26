// Copyright (c) 2026 cade
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "clox/common.h"
#include "clox/lines.h"
#include "clox/value.h"

typedef enum _OpCode {
    OP_RETURN,
    OP_CONSTANT,
} OpCode;

typedef struct _Chunk {
    uint32_t count;
    uint32_t capacity;
    uint8_t *code;
    Lines lines;
    ValueArray constants;
} Chunk;

void chunk_ctor(Chunk *chunk);
void chunk_dtor(Chunk *chunk);
void chunk_write(Chunk *chunk, uint8_t data, uint32_t line);
uint32_t chunk_get_source_line(Chunk *chunk, uint32_t index);
uint32_t chunk_add_constant(Chunk *chunk, Value value);
Value chunk_get_constant(Chunk *chunk, uint32_t index);


#endif // !CLOX_CHUNK_H

