# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/rn_praxis0.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rn_praxis0.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rn_praxis0.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rn_praxis0.dir/flags.make

CMakeFiles/rn_praxis0.dir/main.c.o: CMakeFiles/rn_praxis0.dir/flags.make
CMakeFiles/rn_praxis0.dir/main.c.o: ../main.c
CMakeFiles/rn_praxis0.dir/main.c.o: CMakeFiles/rn_praxis0.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/rn_praxis0.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rn_praxis0.dir/main.c.o -MF CMakeFiles/rn_praxis0.dir/main.c.o.d -o CMakeFiles/rn_praxis0.dir/main.c.o -c /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/main.c

CMakeFiles/rn_praxis0.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rn_praxis0.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/main.c > CMakeFiles/rn_praxis0.dir/main.c.i

CMakeFiles/rn_praxis0.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rn_praxis0.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/main.c -o CMakeFiles/rn_praxis0.dir/main.c.s

# Object files for target rn_praxis0
rn_praxis0_OBJECTS = \
"CMakeFiles/rn_praxis0.dir/main.c.o"

# External object files for target rn_praxis0
rn_praxis0_EXTERNAL_OBJECTS =

rn_praxis0: CMakeFiles/rn_praxis0.dir/main.c.o
rn_praxis0: CMakeFiles/rn_praxis0.dir/build.make
rn_praxis0: CMakeFiles/rn_praxis0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable rn_praxis0"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rn_praxis0.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rn_praxis0.dir/build: rn_praxis0
.PHONY : CMakeFiles/rn_praxis0.dir/build

CMakeFiles/rn_praxis0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rn_praxis0.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rn_praxis0.dir/clean

CMakeFiles/rn_praxis0.dir/depend:
	cd /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0 /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0 /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/cmake-build-debug /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/cmake-build-debug /mnt/c/Users/vladi/Documents/GitHub/Rechnernetze-und-verteilte-Systeme/praxis0/praxis0/rn-praxis0/cmake-build-debug/CMakeFiles/rn_praxis0.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rn_praxis0.dir/depend

