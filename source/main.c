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
    chunk_ctor(&chunk);

    chunk_code_write_constant(&chunk, 1.2, 1);
    chunk_code_write_return(&chunk, 2);
    disassemble_chunk(&chunk, "Hello Chunk disassembly");

    chunk_dtor(&chunk);
    return 0;
}

