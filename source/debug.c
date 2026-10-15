/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>
#include <stdio.h>

#include "clox/debug.h"

static uint32_t disassemble_instruction_simple(const char *name, uint32_t offset);

void disassemble_chunk(Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    // Note that since the instructions and data held within
    // the chunk are not uniform, we have to track the current
    // offset into the array, instead of being able to iterate
    // element by element. (simply a circumstance arising from
    // our choice of representation.)
    for (uint32_t offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}

uint32_t disassemble_instruction(Chunk *chunk, uint32_t offset) {
    printf("%04X ", offset);

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_RETURN:
        return disassemble_instruction_simple("OP_RETURN", offset);

    default:
        printf("Unknown opcode %X\n", instruction);
        return offset + 1;
    }
}

static uint32_t disassemble_instruction_simple(const char *name, uint32_t offset) {
    printf("%s\n", name);
    return offset + 1;
}


