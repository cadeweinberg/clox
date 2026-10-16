/**
 * Copyright (c) 2026 cade
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */


#include "clox/common.h"
#include "clox/chunk.h"
#include "clox/debug.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[]) {
    Chunk chunk;
    chunk_init(&chunk);

    uint32_t constant = chunk_add_constant(&chunk, 1.2);
    chunk_write(&chunk, OP_CONSTANT, 1);
    chunk_write(&chunk, (uint8_t)constant, 1);

    chunk_write(&chunk, OP_RETURN, 2);
    disassemble_chunk(&chunk, "Hello Chunk disassembly");

    chunk_free(&chunk);
    return 0;
}

