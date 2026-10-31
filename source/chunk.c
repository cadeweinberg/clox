/**
 * Copyright (c) 2026 cade
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>
#include <string.h>

#include "clox/chunk.h"
#include "clox/memory.h"
#include "clox/value.h"


void chunk_ctor(Chunk* chunk) {
    assert(chunk != NULL);
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    lines_ctor(&chunk->lines);
    value_array_ctor(&chunk->constants);
}

void chunk_dtor(Chunk *chunk) {
    assert(chunk != NULL);
    lines_dtor(&chunk->lines);
    value_array_dtor(&chunk->constants);
    if (!reallocate((void**)&chunk->code, chunk->capacity * sizeof(*chunk->code), 0)) {
        exit(EXIT_FAILURE);
    }
    chunk_ctor(chunk);
}

static bool chunk_full(const Chunk *chunk) {
    return chunk->capacity < chunk->count + 1;
}

static void chunk_grow(Chunk *chunk) {
    ArrayGrowInfo info = {.growth_factor = 2, .initial_capacity = 8};
    if (!array_grow(info, sizeof(*chunk->code), (void**)&chunk->code, &chunk->capacity)) {
        exit(EXIT_FAILURE);
    }
}

static void chunk_code_write(Chunk *chunk, uint8_t data) {
    if (chunk_full(chunk)) {
        chunk_grow(chunk);
    }

    chunk->code[chunk->count++] = data;
}

static void chunk_lines_write(Chunk *chunk, uint32_t line) {
    lines_set(&chunk->lines, line, chunk->count);
}

void chunk_code_write_return(Chunk *chunk, uint32_t line) {
    chunk_lines_write(chunk, line);
    chunk_code_write(chunk, OP_RETURN);
}

void chunk_code_write_constant(Chunk *chunk, Value constant, uint32_t line) {
    chunk_lines_write(chunk, line);
    uint32_t index = value_array_write(&chunk->constants, constant);
    if (index < UINT8_MAX) {
        chunk_code_write(chunk, OP_CONSTANT);
        chunk_code_write(chunk, (uint8_t)index);
        return;
    }

    chunk_code_write(chunk, OP_CONSTANT_EXTENDED);
    uint8_t buffer[sizeof(uint32_t)];
    memcpy(buffer, &index, sizeof(uint32_t));
    for (uint32_t i = 0; i < sizeof(uint32_t); ++i) {
        chunk_code_write(chunk, buffer[i]);
    }
}

uint32_t chunk_source_line_read(Chunk *chunk, uint32_t index) {
    assert(chunk != NULL);
    return lines_get(&chunk->lines, index);
}


Value chunk_constant_read(Chunk *chunk, uint32_t index) {
    assert(chunk != NULL);
    return value_array_read(&chunk->constants, index);
}
