// Copyright (c) 2026 cade
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "clox/common.h"

typedef enum {
    OP_RETURN,
} OpCode;

typedef struct {
    uint32_t count;
    uint32_t capacity;
    uint8_t *code;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_write(Chunk *chunk, uint8_t data);


#endif // !CLOX_CHUNK_H

