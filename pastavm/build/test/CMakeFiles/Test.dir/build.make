# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
include test/CMakeFiles/Test.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/Test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/Test.dir/flags.make

test/CMakeFiles/Test.dir/test.c.o: test/CMakeFiles/Test.dir/flags.make
test/CMakeFiles/Test.dir/test.c.o: ../test/test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dongcai/Documents/pasta/pastavm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/CMakeFiles/Test.dir/test.c.o"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Test.dir/test.c.o   -c /home/dongcai/Documents/pasta/pastavm/test/test.c

test/CMakeFiles/Test.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Test.dir/test.c.i"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dongcai/Documents/pasta/pastavm/test/test.c > CMakeFiles/Test.dir/test.c.i

test/CMakeFiles/Test.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Test.dir/test.c.s"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dongcai/Documents/pasta/pastavm/test/test.c -o CMakeFiles/Test.dir/test.c.s

test/CMakeFiles/Test.dir/test.c.o.requires:

.PHONY : test/CMakeFiles/Test.dir/test.c.o.requires

test/CMakeFiles/Test.dir/test.c.o.provides: test/CMakeFiles/Test.dir/test.c.o.requires
	$(MAKE) -f test/CMakeFiles/Test.dir/build.make test/CMakeFiles/Test.dir/test.c.o.provides.build
.PHONY : test/CMakeFiles/Test.dir/test.c.o.provides

test/CMakeFiles/Test.dir/test.c.o.provides.build: test/CMakeFiles/Test.dir/test.c.o


# Object files for target Test
Test_OBJECTS = \
"CMakeFiles/Test.dir/test.c.o"

# External object files for target Test
Test_EXTERNAL_OBJECTS =

test/Test: test/CMakeFiles/Test.dir/test.c.o
test/Test: test/CMakeFiles/Test.dir/build.make
test/Test: lib/cjson/libcjson.a
test/Test: src/libcodegen.a
test/Test: src/libvmcore.a
test/Test: lib/cjson/libcjson.a
test/Test: test/CMakeFiles/Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dongcai/Documents/pasta/pastavm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Test"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/Test.dir/build: test/Test

.PHONY : test/CMakeFiles/Test.dir/build

test/CMakeFiles/Test.dir/requires: test/CMakeFiles/Test.dir/test.c.o.requires

.PHONY : test/CMakeFiles/Test.dir/requires

test/CMakeFiles/Test.dir/clean:
	cd /home/dongcai/Documents/pasta/pastavm/build/test && $(CMAKE_COMMAND) -P CMakeFiles/Test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/Test.dir/clean

test/CMakeFiles/Test.dir/depend:
	cd /home/dongcai/Documents/pasta/pastavm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dongcai/Documents/pasta/pastavm /home/dongcai/Documents/pasta/pastavm/test /home/dongcai/Documents/pasta/pastavm/build /home/dongcai/Documents/pasta/pastavm/build/test /home/dongcai/Documents/pasta/pastavm/build/test/CMakeFiles/Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/Test.dir/depend

