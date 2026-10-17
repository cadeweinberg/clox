/**
 * Copyright (c) 2026 cade
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>

#include "clox/chunk.h"
#include "clox/memory.h"
#include "clox/value.h"


void chunk_init(Chunk* chunk) {
    assert(chunk != NULL);
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    lines_init(&chunk->lines);
    value_array_init(&chunk->constants);
}

void chunk_free(Chunk *chunk) {
    assert(chunk != NULL);
    lines_free(&chunk->lines);
    value_array_free(&chunk->constants);
    if (!reallocate((void**)&chunk->code, chunk->capacity * sizeof(*chunk->code), 0)) {
        exit(EXIT_FAILURE);
    }
    chunk_init(chunk);
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

void chunk_write(Chunk *chunk, uint8_t data, uint32_t line) {
    assert(chunk != NULL);
    if (chunk_full(chunk)) {
        chunk_grow(chunk);
    }

    lines_set(&chunk->lines, line, chunk->count);
    chunk->code[chunk->count++] = data;
}

uint32_t chunk_get_source_line(Chunk *chunk, uint32_t index) {
    assert(chunk != NULL);
    return lines_get(&chunk->lines, index);
}

uint32_t chunk_add_constant(Chunk *chunk, Value value) {
    assert(chunk != NULL);
    return value_array_write(&chunk->constants, value);
}


Value chunk_get_constant(Chunk *chunk, uint32_t index) {
    assert(chunk != NULL);
    return value_array_get(&chunk->constants, index);
}
