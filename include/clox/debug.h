// Copyright (c) 2026 Cade Weinberg
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include <stdio.h>

#include "clox/chunk.h"

void     disassemble_chunk(FILE *out, Chunk *chunk, const char *name);
uint32_t disassemble_instruction(FILE *out, Chunk *chunk, uint32_t offset);

#endif // !CLOX_DEBUG_H