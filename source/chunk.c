/**
 * Copyright (c) 2026 cade
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <stdlib.h>

#include "clox/chunk.h"
#include "clox/memory.h"
#include "clox/value.h"


void chunk_init(Chunk* chunk) {
    assert(chunk != NULL);
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    value_array_init(&chunk->constants);
}

void chunk_free(Chunk *chunk) {
    assert(chunk != NULL);
    if (!reallocate((void**)&chunk->code, chunk->capacity * sizeof(*chunk->code), 0)) {
        exit(EXIT_FAILURE);
    }
    if (!reallocate((void**)&chunk->lines, sizeof(*chunk->lines) * chunk->capacity, 0)) {
        exit(EXIT_FAILURE);
    }
    chunk_init(chunk);
    value_array_free(&chunk->constants);
}

static bool chunk_full(const Chunk *chunk) {
    return chunk->capacity < chunk->count + 1;
}

static void chunk_grow(Chunk *chunk) {
    ArrayGrowthInfo info = {.growth_factor = 2, .initial_capacity = 8};
    if (!array_growth(info, sizeof(*chunk->code), (void**)&chunk->code, &chunk->capacity)) {
        exit(EXIT_FAILURE);
    }
    uint32_t spare = chunk->capacity;
    if (!array_growth(info, sizeof(*chunk->lines), (void**)&chunk->lines, &spare)) {
        exit(EXIT_FAILURE);
    }
}

void chunk_write(Chunk *chunk, uint8_t data, uint32_t line) {
    assert(chunk != NULL);
    if (chunk_full(chunk)) {
        chunk_grow(chunk);
    }

    chunk->lines[chunk->count] = line;
    chunk->code[chunk->count++] = data;
}

uint32_t chunk_add_constant(Chunk *chunk, Value value) {
    assert(chunk != NULL);
    return value_array_write(&chunk->constants, value);
}


Value chunk_get_constant(Chunk *chunk, uint32_t index) {
    assert(chunk != NULL);
    return value_array_get(&chunk->constants, index);
}
