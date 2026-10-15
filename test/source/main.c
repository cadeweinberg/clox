/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdlib.h>

#include "test/memory.h"

int main (int argc, char **argv) {
    return munit_suite_main(&memory_suite, NULL, argc, argv);
}

