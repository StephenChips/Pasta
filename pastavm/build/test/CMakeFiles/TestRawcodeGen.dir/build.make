# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dongcai/Documents/pasta/pastavm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dongcai/Documents/pasta/pastavm/build

# Include any dependencies generated for this target.
include test/CMakeFiles/TestRawcodeGen.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/TestRawcodeGen.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/TestRawcodeGen.dir/flags.make

test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o: test/CMakeFiles/TestRawcodeGen.dir/flags.make
test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o: ../test/test_rawcode_gen.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dongcai/Documents/pasta/pastavm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o   -c /home/dongcai/Documents/pasta/pastavm/test/test_rawcode_gen.c

test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.i"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dongcai/Documents/pasta/pastavm/test/test_rawcode_gen.c > CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.i

test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.s"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dongcai/Documents/pasta/pastavm/test/test_rawcode_gen.c -o CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.s

test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o.requires:

.PHONY : test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o.requires

test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o.provides: test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o.requires
	$(MAKE) -f test/CMakeFiles/TestRawcodeGen.dir/build.make test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o.provides.build
.PHONY : test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o.provides

test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o.provides.build: test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o


# Object files for target TestRawcodeGen
TestRawcodeGen_OBJECTS = \
"CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o"

# External object files for target TestRawcodeGen
TestRawcodeGen_EXTERNAL_OBJECTS =

test/TestRawcodeGen: test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o
test/TestRawcodeGen: test/CMakeFiles/TestRawcodeGen.dir/build.make
test/TestRawcodeGen: src/libcodegen.a
test/TestRawcodeGen: test/CMakeFiles/TestRawcodeGen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dongcai/Documents/pasta/pastavm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable TestRawcodeGen"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestRawcodeGen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/TestRawcodeGen.dir/build: test/TestRawcodeGen

.PHONY : test/CMakeFiles/TestRawcodeGen.dir/build

test/CMakeFiles/TestRawcodeGen.dir/requires: test/CMakeFiles/TestRawcodeGen.dir/test_rawcode_gen.c.o.requires

.PHONY : test/CMakeFiles/TestRawcodeGen.dir/requires

test/CMakeFiles/TestRawcodeGen.dir/clean:
	cd /home/dongcai/Documents/pasta/pastavm/build/test && $(CMAKE_COMMAND) -P CMakeFiles/TestRawcodeGen.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/TestRawcodeGen.dir/clean

test/CMakeFiles/TestRawcodeGen.dir/depend:
	cd /home/dongcai/Documents/pasta/pastavm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dongcai/Documents/pasta/pastavm /home/dongcai/Documents/pasta/pastavm/test /home/dongcai/Documents/pasta/pastavm/build /home/dongcai/Documents/pasta/pastavm/build/test /home/dongcai/Documents/pasta/pastavm/build/test/CMakeFiles/TestRawcodeGen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/TestRawcodeGen.dir/depend
