// Copyright (c) 2026 cade
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "clox/common.h"
#include "clox/lines.h"
#include "clox/value.h"

typedef enum _OpCode : uint8_t {
    OP_RETURN,
    OP_CONSTANT,
    OP_CONSTANT_EXTENDED,
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
void chunk_code_write_return(Chunk *chunk, uint32_t line);
void chunk_code_write_constant(Chunk *chunk, Value constant, uint32_t line);
uint32_t chunk_source_line_read(Chunk *chunk, uint32_t index);
Value chunk_constant_read(Chunk *chunk, uint32_t index);


#endif // !CLOX_CHUNK_H

