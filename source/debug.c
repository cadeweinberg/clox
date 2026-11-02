/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <string.h>

#include "clox/debug.h"
#include "clox/chunk.h"
#include "clox/value.h"

static uint32_t disassemble_instruction_simple(FILE *out, const char *name, uint32_t offset);
static uint32_t disassemble_instruction_constant(FILE *out, const char *name, Chunk *chunk, uint32_t offset);
static uint32_t disassemble_instruction_constant_extended(FILE *out, const char *name, Chunk *chunk, uint32_t offset);

void disassemble_chunk(FILE *out, Chunk *chunk, const char *name) {
    fprintf(out, "== %s ==\n", name);

    // Note that since the instructions and data held within
    // the chunk are not uniform, we have to track the current
    // offset into the array, instead of being able to iterate
    // element by element. (simply a circumstance arising from
    // our choice of representation.)
    for (uint32_t offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(out, chunk, offset);
    }
}

uint32_t disassemble_instruction(FILE *out, Chunk *chunk, uint32_t offset) {
    fprintf(out, "0x%04X ", offset);

    uint32_t current_line = chunk_source_line_read(chunk, offset);
    if (offset > 0) {
        uint32_t previous_line = chunk_source_line_read(chunk, offset);
        if (current_line == previous_line) {
            fprintf(out, "   | ");
        } else {
            fprintf(out, "%4d ", current_line);
        }
    } else {
        fprintf(out, "%4d ", current_line);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_RETURN:
        return disassemble_instruction_simple(out, "OP_RETURN", offset);

    case OP_CONSTANT:
        return disassemble_instruction_constant(out, "OP_CONSTANT", chunk, offset);

    case OP_CONSTANT_EXTENDED:
        return disassemble_instruction_constant_extended(out, "OP_CONSTANT_EXTENDED", chunk, offset);

    default:
        fprintf(out, "Unknown opcode %X\n", instruction);
        return offset + 1;
    }
}

static uint32_t disassemble_instruction_simple(FILE *out, const char *name, uint32_t offset) {
    fprintf(out, "%s\n", name);
    return offset + 1;
}

static uint32_t disassemble_instruction_constant(FILE *out, const char *name, Chunk *chunk, uint32_t offset) {
    uint8_t constant = chunk->code[offset + 1];
    fprintf(out, "%-16s %4d '", name, constant);
    value_print(out, chunk_constant_read(chunk, constant));
    fprintf(out, "'\n");
    return offset + 2;
}

static uint32_t disassemble_instruction_constant_extended(FILE *out, const char *name, Chunk *chunk, uint32_t offset) {
    uint32_t constant;
    memcpy(&constant, chunk->code + offset + 1, sizeof(uint32_t));
    fprintf(out, "%-16s %4d '", name, constant);
    value_print(out, chunk_constant_read(chunk, constant));
    fprintf(out, "'\n");
    return offset + 5;
}


