/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */


#include "clox/lines.h"
#include "clox/memory.h"
#include <stdlib.h>


void lines_init(Lines *lines) {
    assert(lines != NULL);
    lines->count = 0;
    lines->capacity = 0;
    lines->runs = NULL;
}

void lines_free(Lines *lines) {
    assert(lines != NULL);
    if (!reallocate((void**)&lines->runs, sizeof(*lines->runs) * lines->capacity, 0)) {
        exit(EXIT_FAILURE);
    }
    lines_init(lines);
}

static bool lines_full(const Lines *lines) {
    return lines->capacity < (lines->count + 1);
}

static void lines_grow(Lines *lines) {
    static const ArrayGrowInfo info = {.growth_factor = 2, .initial_capacity = 8};
    array_grow(info, sizeof(*lines->runs), (void**)&lines->runs, &lines->capacity);
}

static void lines_append(Lines *lines, uint32_t line, uint32_t index) {
    if (lines_full(lines)) {
        lines_grow(lines);
    }

    lines->runs[lines->count++] = (Run){.line = line, .index = index};
}

void lines_set(Lines *lines, uint32_t line, uint32_t index) {
    assert(lines != NULL);
    
    // if the array is empty we can simply add a new run
    if (lines->count == 0) {
        lines_append(lines, line, index);
    }

    // we either need to update the last run, or add a new run
    Run *run = lines->runs + lines->count - 1;
    // if `line < run->line` that is malicious input. the line given
    // already has an association.
    assert(run->line <= line);
    if (run->line == line) {
        assert(index == (run->index + 1));
        run->index = index; // note that this is equivalent to `run->index++`
        // otherwise this is malicious input, the index given has skipped 
        // one or more intermediate instructions.
    } else {
        // note that, `line == run->line + 1` and `index == run->index + 1`
        // otherwise this is malicious input, the line given has skipped one or 
        // more intermediate lines, or the index given has skipped 
        // one or more intermediate instructions.
        assert(line == (run->line + 1));
        assert(index == (run->index + 1));
        lines_append(lines, line, index); 
    }
}

uint32_t lines_get(Lines *lines, uint32_t index) {
    // here we could be given the index of any bytecode instruction 
    // we have a record of. including ones we do not have yet.
    // in the well behaved case, `index <= lines->runs[lines->count - 1].index`
    // since we have a contract of a uint32_t, we don't have to worry about negative
    // inputs. so the only bad behavior is that the index > the largest input we 
    // have on record.
    for (uint32_t i = 0; i < lines->count; ++i) {
        // an index is associated with a given run if the `index <= run->index`
        // if we check one by one sequentially, then we are guarunteed to find 
        // the associated source code line. 
        Run *run = lines->runs + i;
        if (index <= run->index) {
            return run->line;
        }
    }
    // what do we do here, where we have search exaustively and still not found the index?
    // we need to signal an error. we could
    // return UINT32_MAX.
    // it is unlikely that we will ever encounter that many source lines.
    // so it is probably safe to reserve that single value for an error condition.
    // return 0. 
    // we start counting source lines at 1, so 0 is already unused within the valid set
    // of source lines.
    return 0;
}



