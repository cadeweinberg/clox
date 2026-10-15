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


void chunk_init(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

static bool chunk_full(const Chunk *chunk) {
    return chunk->capacity < chunk->count + 1;
}

static void chunk_grow(Chunk *chunk) {
    ArrayGrowthInfo info = {.growth_factor = 2, .initial_capacity = 8};
    if (!array_growth(info, sizeof(*chunk->code), (void**)&chunk->code, &chunk->capacity)) {
        exit(EXIT_FAILURE);
    }
}

void chunk_write(Chunk *chunk, uint8_t data) {
    if (chunk_full(chunk)) {
        chunk_grow(chunk);
    }

    chunk->code[chunk->count++] = data;
}

