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
include test/CMakeFiles/TestIns.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/TestIns.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/TestIns.dir/flags.make

test/CMakeFiles/TestIns.dir/test_ins.c.o: test/CMakeFiles/TestIns.dir/flags.make
test/CMakeFiles/TestIns.dir/test_ins.c.o: ../test/test_ins.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dongcai/Documents/pasta/pastavm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/CMakeFiles/TestIns.dir/test_ins.c.o"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/TestIns.dir/test_ins.c.o   -c /home/dongcai/Documents/pasta/pastavm/test/test_ins.c

test/CMakeFiles/TestIns.dir/test_ins.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TestIns.dir/test_ins.c.i"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dongcai/Documents/pasta/pastavm/test/test_ins.c > CMakeFiles/TestIns.dir/test_ins.c.i

test/CMakeFiles/TestIns.dir/test_ins.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TestIns.dir/test_ins.c.s"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dongcai/Documents/pasta/pastavm/test/test_ins.c -o CMakeFiles/TestIns.dir/test_ins.c.s

test/CMakeFiles/TestIns.dir/test_ins.c.o.requires:

.PHONY : test/CMakeFiles/TestIns.dir/test_ins.c.o.requires

test/CMakeFiles/TestIns.dir/test_ins.c.o.provides: test/CMakeFiles/TestIns.dir/test_ins.c.o.requires
	$(MAKE) -f test/CMakeFiles/TestIns.dir/build.make test/CMakeFiles/TestIns.dir/test_ins.c.o.provides.build
.PHONY : test/CMakeFiles/TestIns.dir/test_ins.c.o.provides

test/CMakeFiles/TestIns.dir/test_ins.c.o.provides.build: test/CMakeFiles/TestIns.dir/test_ins.c.o


# Object files for target TestIns
TestIns_OBJECTS = \
"CMakeFiles/TestIns.dir/test_ins.c.o"

# External object files for target TestIns
TestIns_EXTERNAL_OBJECTS =

test/TestIns: test/CMakeFiles/TestIns.dir/test_ins.c.o
test/TestIns: test/CMakeFiles/TestIns.dir/build.make
test/TestIns: lib/cjson/libcjson.a
test/TestIns: src/libcodegen.a
test/TestIns: src/libvmcore.a
test/TestIns: lib/cjson/libcjson.a
test/TestIns: test/CMakeFiles/TestIns.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dongcai/Documents/pasta/pastavm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable TestIns"
	cd /home/dongcai/Documents/pasta/pastavm/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestIns.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/TestIns.dir/build: test/TestIns

.PHONY : test/CMakeFiles/TestIns.dir/build

test/CMakeFiles/TestIns.dir/requires: test/CMakeFiles/TestIns.dir/test_ins.c.o.requires

.PHONY : test/CMakeFiles/TestIns.dir/requires

test/CMakeFiles/TestIns.dir/clean:
	cd /home/dongcai/Documents/pasta/pastavm/build/test && $(CMAKE_COMMAND) -P CMakeFiles/TestIns.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/TestIns.dir/clean

test/CMakeFiles/TestIns.dir/depend:
	cd /home/dongcai/Documents/pasta/pastavm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dongcai/Documents/pasta/pastavm /home/dongcai/Documents/pasta/pastavm/test /home/dongcai/Documents/pasta/pastavm/build /home/dongcai/Documents/pasta/pastavm/build/test /home/dongcai/Documents/pasta/pastavm/build/test/CMakeFiles/TestIns.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/TestIns.dir/depend

