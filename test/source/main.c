/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>

#include "munit.h"

#include "test/chunk.h"
#include "test/lines.h"
#include "test/memory.h"

static MunitSuite suite_array[] = {
    {"/memory", memory_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {"/lines", lines_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {"/chunk", chunk_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE}
};

static MunitSuite main_suite = {
    "clox",
    NULL,
    suite_array,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main (int argc, char **argv) {
    return munit_suite_main(&main_suite, NULL, argc, argv);
}

