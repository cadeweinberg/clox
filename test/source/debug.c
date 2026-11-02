/**
 * Copyright (c) 2026 Cade Weinberg
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <string.h>

#include "test/debug.h"
#include "clox/debug.h"

// test that the disassembly routines work for all given opcode types.

static MunitResult test_disassemble_return(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

static MunitResult test_disassemble_constant(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

static MunitResult test_disassemble_constant_extended(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *);

MunitTest debug_tests[] = {
    {
        "/debug/disassemble/return",
        test_disassemble_return,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/debug/disassemble/constant",
        test_disassemble_constant,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/debug/disassemble/constant_extended",
        test_disassemble_constant_extended,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        NULL,
        NULL,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
};

#ifdef _POSIX_C_SOURCE
#include <sys/stat.h>
static size_t file_length(FILE *stream) {
    int f = fileno(stream);

    struct stat s;
    if (fstat(f, &s) < 0) {
        perror("fstat failed");
        exit(EXIT_FAILURE);
    }

    return (size_t)s.st_size;
}
#else 
    #error "POSIX required for retrieving file length"
#endif

// how can we test that disassembly works?
// the output is string based, so the main way
// would be string comparisons, however that 
// is fragile to slight changes in the printing
// algorithm that are aesthetic, and don't undermine
// the function. if we just check that the code
// prints "something" then we miss the important case
// of did we add the code for printing the given opcode
// to the disassembly subroutine.
//
// Well, if the invariant to hold is "does the string
// associated with the OpCode enum value appear in the 
// output of the disassembly" we could disassemble,
// then for the test, do a simple string search for the
// opcode name.

static MunitResult search(FILE *tmp, char const *str) {
    // the file was just written to, so before reading we must
    // sync the files contents, and seek to the beginning of the 
    // file.
    munit_assert (fflush(tmp) != EOF); // fflush failed
    rewind(tmp);

    size_t size = file_length(tmp);
    char buffer[size + 1];
    buffer[size] = '\0';

    size_t read = fread(buffer, sizeof(unsigned char), size, tmp);
    if (read != size) {
        if (feof(tmp)) {
            munit_error("unexpected eof\n");
        } 

        munit_errorf("fread failed: %s", strerror(ferror(tmp)));
    }

    char *found = strstr(buffer, str);
    munit_assert_ptr(found, !=, NULL);

    return MUNIT_OK;
}

static MunitResult test_disassemble_return(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    // it looks like we can use tmpfile to create a dummy file 
    // to write the disassembly to. "The file is created in "wb+"
    // mode. a binary file for update. ah. "b" this flag has 
    // no effect on POSIX systems. got it.
    FILE *tmp = tmpfile();
    Chunk chunk;
    chunk_ctor(&chunk);

    chunk_code_write_return(&chunk, 1);

    disassemble_chunk(tmp, &chunk, "test");

    MunitResult found = search(tmp, "OP_RETURN");
    
    fclose(tmp);
    chunk_dtor(&chunk);
    return found;
}

static MunitResult test_disassemble_constant(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    FILE *tmp = tmpfile();
    Chunk chunk;
    chunk_ctor(&chunk);

    chunk_code_write_constant(&chunk, 1.0, 1);

    disassemble_chunk(tmp, &chunk, "test");

    MunitResult found = search(tmp, "OP_CONSTANT");
    
    fclose(tmp);
    chunk_dtor(&chunk);
    return found;
}

static MunitResult test_disassemble_constant_extended(
    [[maybe_unused]] const MunitParameter *,
    [[maybe_unused]] void *) {
    FILE *tmp = tmpfile();
    Chunk chunk;
    chunk_ctor(&chunk);

    // OP_CONSTANT_EXTENDED will be emitted with the 256th constant.
    for (uint32_t i = 0; i < 257; ++ i) {
        chunk_code_write_constant(&chunk, 1.0, 1);
    }

    disassemble_chunk(tmp, &chunk, "test");

    MunitResult found = search(tmp, "OP_CONSTANT_EXTENDED");
    
    fclose(tmp);
    chunk_dtor(&chunk);
    return found; 
}
