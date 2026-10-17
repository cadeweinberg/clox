// Copyright (c) 2026 Cade Weinberg
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef CLOX_LINES_H
#define CLOX_LINES_H

#include "clox/common.h"

// The goal is to define a data structure that represents the 
// source code lines on which a given bytecode instruction appears.
// The current implementation uses a single array that is exactly
// as long as the bytecode array. 
// A instruction is reffered to by it's offset into the bytecode array.
// That same offset can be used to index the lines array and retrieve 
// a line number. This line number is the soure code line number that 
// produced (was compiled into) the bytecode instruction.
//
// Notice three important pieces of information.
// 1. Each bytecode instruction can be retrieved with a single number,
//    it's offset into the bytecode array.
// 2. Each line number can be retrieved with a single number, 
//    the offset of the "bound" instruction. (producing? associated?)
// 3. Not obvious from the above description, many line numbers are 
//    repeated for subsequent instructions. Said another way, a single 
//    line of source code will often compile to many bytecode 
//    instructions.
//
// We hope to leverage fact 3 by way of a run length encoding of the given 
// line numbers. Since we know that many instructions are produced by the 
// same line of source text, we would like to store the line number once,
// and associate it with many bytecode instructions.
// Lets gather some more facts surrounding the bytecode;
//
// 1. Bytecode instructions are uniquely identified by their 
//    position within the bytecode array.
//
// If we think about it, we know that a given bytecode instruction must 
// be stored within the larger array at some offset. That particular 
// instruction is only appearing in that one location. Even if we observe 
// an identical bytecode instruction at a later point in the code
// we know that it is a different instruction. They are not equal.
// This is due to the nature of the compilation into bytecode itself,
// if we see, "1 + 1" in the source code, and the bytecode uses
// "push 1; push 1; add" we can see that there are two identical "push 1"
// instructions, and they are both necessary for the correct functioning 
// of the program. They both appear in the bytecode, and they are both
// associated with the same line of source code. The fact that they are 
// semantically and syntactically identical does not imply that we can 
// treat them as the same instruction.
//
// 2. Bytecode instructions appear sequentially
//
// This observation is based on the fact that the bytecode is compiled 
// from the source code in order from left to right, top to bottom.
// Each bytecode instruction appears one after another within the array,
// and we know that the next bytecode instruction to execute/decode/inspect/etc
// the so called "next" instruction, is the next instruction within the array. 
// relative to the last bytecode instruction. The instruction at index `n`
// is preceded by the instruction at index `n - 1`, and will be followed by the 
// instruction at index `n + 1`.
//
// 3. Source line numbers appear sequentially
// 
// each line of source code is compiled one after another into the bytecode 
// instructions. A given source code line, `s`, is compiled into one or more bytecode
// instructions, `s -> [n, n + m]` (where n is the index of the first bytecode isntruction,
// and m is the index of the last bytecode instruction compiled from that line). 
// and the association between that line of source
// code and those bytecode instructions is defined by the fact that the compiler 
// produced those instructions from that source code. The next line of source code
// `s + 1`, is compiled into bytecode instructions appearing immediately after the 
// previous ones, `s + 1 -> [i, i + j]`
//
// given facts 2 and 3, we know that `i == n + m + 1` 
// and j is similarly equal to the index of the last instruction produced by the compiler
// from the line of source text.
// I think then, that we can store two numbers to represent a `run` of `s -> [n, n + m]`
// the pair (s, m).
// The is because the first instruction appears at index `0` and the first line of source 
// code is compiled into bytecode going until `m` in the array. 
// The next line of source code starts at `m + 1` and runs until, say, `i`
// we then have a run array that looks like [(1, m), (2, i)]
// 
// note the requirements for the api of this data structure. we need to 
// 1. given a pair, `(s, n)`, where `s` is the line of source code
//    and `n` is the index of the given piece of bytecode. store that 
//    pairing such that;
// 2. given a index `n` we may retrieve the line of source text `s`
//
// given the array `[(1, m), (2, i)]` 
// if we are given a pair `(s, n)` we know to either
// increment i by one, because n must be sequential with respect to i, due to facts 2 and 3.
// or we need to create a new entry, `(s, n)`. where we know that `n == i + 1` and `s == 3`
// also due to facts 2 and 3.
//

typedef struct _Run {
    uint32_t line;
    uint32_t index;
} Run;

typedef struct _Lines {
    uint32_t count;
    uint32_t capacity;
    Run *runs;
} Lines;

void lines_init(Lines *lines);
void lines_free(Lines *lines);

/**
 * @brief Set the instruction as produced by the given source line
 */
void lines_set(Lines *lines, uint32_t line, uint32_t index);

/**
 * @brief get the source line that produced the instruction 
 *
 * @return uint32_t the source line which produced the instruction
 * @return 0 when the index was not found
 */
uint32_t lines_get(Lines *lines, uint32_t index);

#endif // !CLOX_LINES_H
