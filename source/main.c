/**
 * Copyright (c) 2026 cade
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */


#include "clox/common.h"
#include "clox/chunk.h"
#include "clox/debug.h"
#include "clox/vm.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[]) {
    VM vm;
    vm_ctor(&vm);

    Chunk chunk;
    chunk_ctor(&chunk);

    chunk_code_write_constant(&chunk, 2.0, 1);
    chunk_code_write_constant(&chunk, 3.0, 1);
    chunk_code_write_multiply(&chunk, 1);
    chunk_code_write_constant(&chunk, 4.0, 1);
    chunk_code_write_constant(&chunk, 5.0, 1);
    chunk_code_write_negate(&chunk, 1);
    chunk_code_write_divide(&chunk, 1);
    chunk_code_write_subtract(&chunk, 1);
    chunk_code_write_constant(&chunk, 1.0, 1);
    chunk_code_write_add(&chunk, 1);
    chunk_code_write_return(&chunk, 1);
    
    vm_interpret(&vm, &chunk);

    chunk_dtor(&chunk);
    vm_dtor(&vm);
    return 0;
}

