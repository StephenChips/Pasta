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
include src/CMakeFiles/vmrawcodegen.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/vmrawcodegen.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/vmrawcodegen.dir/flags.make

src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o: src/CMakeFiles/vmrawcodegen.dir/flags.make
src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o: ../src/rawcode.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dongcai/Documents/pasta/pastavm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o"
	cd /home/dongcai/Documents/pasta/pastavm/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vmrawcodegen.dir/rawcode.c.o   -c /home/dongcai/Documents/pasta/pastavm/src/rawcode.c

src/CMakeFiles/vmrawcodegen.dir/rawcode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vmrawcodegen.dir/rawcode.c.i"
	cd /home/dongcai/Documents/pasta/pastavm/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dongcai/Documents/pasta/pastavm/src/rawcode.c > CMakeFiles/vmrawcodegen.dir/rawcode.c.i

src/CMakeFiles/vmrawcodegen.dir/rawcode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vmrawcodegen.dir/rawcode.c.s"
	cd /home/dongcai/Documents/pasta/pastavm/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dongcai/Documents/pasta/pastavm/src/rawcode.c -o CMakeFiles/vmrawcodegen.dir/rawcode.c.s

src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o.requires:

.PHONY : src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o.requires

src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o.provides: src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o.requires
	$(MAKE) -f src/CMakeFiles/vmrawcodegen.dir/build.make src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o.provides.build
.PHONY : src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o.provides

src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o.provides.build: src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o


# Object files for target vmrawcodegen
vmrawcodegen_OBJECTS = \
"CMakeFiles/vmrawcodegen.dir/rawcode.c.o"

# External object files for target vmrawcodegen
vmrawcodegen_EXTERNAL_OBJECTS =

src/libvmrawcodegen.a: src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o
src/libvmrawcodegen.a: src/CMakeFiles/vmrawcodegen.dir/build.make
src/libvmrawcodegen.a: src/CMakeFiles/vmrawcodegen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dongcai/Documents/pasta/pastavm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libvmrawcodegen.a"
	cd /home/dongcai/Documents/pasta/pastavm/build/src && $(CMAKE_COMMAND) -P CMakeFiles/vmrawcodegen.dir/cmake_clean_target.cmake
	cd /home/dongcai/Documents/pasta/pastavm/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vmrawcodegen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/vmrawcodegen.dir/build: src/libvmrawcodegen.a

.PHONY : src/CMakeFiles/vmrawcodegen.dir/build

src/CMakeFiles/vmrawcodegen.dir/requires: src/CMakeFiles/vmrawcodegen.dir/rawcode.c.o.requires

.PHONY : src/CMakeFiles/vmrawcodegen.dir/requires

src/CMakeFiles/vmrawcodegen.dir/clean:
	cd /home/dongcai/Documents/pasta/pastavm/build/src && $(CMAKE_COMMAND) -P CMakeFiles/vmrawcodegen.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/vmrawcodegen.dir/clean

src/CMakeFiles/vmrawcodegen.dir/depend:
	cd /home/dongcai/Documents/pasta/pastavm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dongcai/Documents/pasta/pastavm /home/dongcai/Documents/pasta/pastavm/src /home/dongcai/Documents/pasta/pastavm/build /home/dongcai/Documents/pasta/pastavm/build/src /home/dongcai/Documents/pasta/pastavm/build/src/CMakeFiles/vmrawcodegen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/vmrawcodegen.dir/depend

