

cmake_minimum_required(VERSION 3.30)

project(CLOX 
	LANGUAGES C 
	VERSION 0.0.0
	DESCRIPTION "A Bytecode Interpreter in C."
)

# We set export compile commands here to save ourselves the effort 
# of passing the argument in each time. THe alternative for our current
# editor setup is to use a json file which specifies the arguments to 
# given build types, which are themselves a custom definition,
# though commonly, Debug, RelWithDebInfo, MinSizeRel, Release.
# and I like to add Profile, Sanitize, for code profiling and sanitizers
# respectively. code profiling can be used for profile guided optimization
# as well as fuzz testing. I think the more modern approach would be to 
# allow the user to set this variable only if they desire the compile commands.
# as this is really only useful for someone working with the code,
# and using clangd at the same time. So the cleaner, less intrusive approach is
# cmake build presets. so
# TODO: refactor this set(...) into an options on cmake build presets.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# There is a question here about which standard to use. 
# C99 presents the most compatibility with the ecosystem, as it has been 
# around the longest, has the widest adoption, and is the only C standard
# supported on windows.
# C11 cleans up some language semantics, I need to refresh myself on the particulars.
# C23 cleans up more language semantics, and adds bool as a keyword, among
# more details I need to refresh myself on. In the spirit of being with the 
# bleeding-edge of technology. The best practices as honed by the decades of experience 
# that have come before from as many engineers as possible. My preference is 
# to adopt the latest standards. 
# The compatibility argument is strong, to me, and I would use C99 if it meant
# the different between getting the project to work or not.
# however, it doesn't, this is a hobby project, so I can do what feels comfortable,
# and that means C23.
set(CMAKE_C_STANDARD 23)
# Requiring the standard to be available is, to me, a given if you are going to set 
# the standard in the CMAKE file at all. Given the project is thus going to rely on 
# features from the standard of choice.
set(CMAKE_C_STANDARD_REQUIRED ON)
# And, I want to test and build with both GCC and Clang. So turning off support for 
# compiler specific extensions makes sense. Though here again, I don't mind being tied
# to a particular compiler for this project. 
set(CMAKE_C_EXTENSIONS OFF)

# When using CMAKE I like to set these directory variables in each project I use,
# they make specifying the targets cleaner, and allow the project to define a stable
# set of variables to build off of, which can be modified upstream to allow for easier 
# project management. Using the raw CMake Vraibles directly would require changes to 
# be made pervasively if they needed to be made.
set(CLOX_BASE_DIR ${PROJECT_SOURCE_DIR})
set(CLOX_INCLUDE_DIR ${CLOX_BASE_DIR}/include)
set(CLOX_SOURCE_DIR ${CLOX_BASE_DIR}/source)
set(CLOX_TEST_DIR ${CLOX_BASE_DIR}/test)
set(CLOX_TEST_INCLUDE_DIR ${CLOX_TEST_DIR}/include)
set(CLOX_TEST_SOURCE_DIR ${CLOX_TEST_DIR}/source)
set(CLOX_EXTERNAL_DIR ${CLOX_BASE_DIR}/external)

# #TODO: add Profiling and Sanitization builds. 

set(CLOX_WARNINGS_GNU
	-Wall
	-Wextra
	-Wpedantic
	-Wshadow
	-Wconversion
	-Wformat=2 # stricter 'printf' style format string checks
)

# MSVC has better support for C11/17 as of the time of writing (10/14/26).
# So maybe one day this project will have cross-platform compatibility 
# without using MINGW or giving up the safety and modern updates to the C
# language that C23 adds.
set(CLOX_WARNINGS_MSVC
	/W4
	/permissive-
)

add_library(clox_library)

target_compile_options(clox_library PRIVATE
	$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>>:${CLOX_WARNINGS_GNU}>
	$<$<C_COMPILER_ID:MSVC>:${CLOX_WARNINGS_MSVC}>
)

target_include_directories(clox_library PUBLIC ${CLOX_INCLUDE_DIR})

target_sources(clox_library PRIVATE
	${CLOX_SOURCE_DIR}/chunk.c
	${CLOX_SOURCE_DIR}/debug.c
	${CLOX_SOURCE_DIR}/lines.c
	${CLOX_SOURCE_DIR}/memory.c
	${CLOX_SOURCE_DIR}/value.c
)

set_target_properties(clox_library PROPERTIES
	OUTPUT_NAME clox
)

add_executable(clox)

target_compile_options(clox PRIVATE
	$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>>:${CLOX_WARNINGS_GNU}>
	$<$<C_COMPILER_ID:MSVC>:${CLOX_WARNINGS_MSVC}>
)

target_include_directories(clox PUBLIC ${CLOX_INCLUDE_DIR})

target_link_libraries(clox PRIVATE clox_library)

target_sources(clox PRIVATE
	${CLOX_SOURCE_DIR}/main.c
)

# testing

add_library(munit)

target_sources(munit PRIVATE
	${CLOX_EXTERNAL_DIR}/munit/munit.c
)

set_target_properties(munit PROPERTIES
	C_STANDARD 17
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)

add_executable(clox_test)

set_target_properties(clox_test PROPERTIES
	OUTPUT_NAME test
)


target_compile_options(clox_test PRIVATE
	$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>>:${CLOX_WARNINGS_GNU}>
	$<$<C_COMPILER_ID:MSVC>:${CLOX_WARNINGS_MSVC}>
)

target_include_directories(clox_test PUBLIC 
	${CLOX_INCLUDE_DIR}
	${CLOX_TEST_INCLUDE_DIR}
	${CLOX_EXTERNAL_DIR}/munit
)

target_link_libraries(clox_test PRIVATE 
	clox_library
	munit
)

target_sources(clox_test PRIVATE
	${CLOX_TEST_SOURCE_DIR}/chunk.c
	${CLOX_TEST_SOURCE_DIR}/main.c
	${CLOX_TEST_SOURCE_DIR}/memory.c
)
