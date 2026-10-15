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

    chunk_write(&chunk, OP_RETURN);
    disassemble_chunk(&chunk, "Hello Chunk disassembly");

    chunk_free(&chunk);
    return 0;
}

