# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/build"

# Include any dependencies generated for this target.
include CMakeFiles/webserver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/webserver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/webserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserver.dir/flags.make

CMakeFiles/webserver.dir/webserver.c.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/webserver.c.o: /mnt/e/Uni/Rechnernetze\ und\ Verteilte\ Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/webserver.c
CMakeFiles/webserver.dir/webserver.c.o: CMakeFiles/webserver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/webserver.dir/webserver.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/webserver.dir/webserver.c.o -MF CMakeFiles/webserver.dir/webserver.c.o.d -o CMakeFiles/webserver.dir/webserver.c.o -c "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/webserver.c"

CMakeFiles/webserver.dir/webserver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/webserver.dir/webserver.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/webserver.c" > CMakeFiles/webserver.dir/webserver.c.i

CMakeFiles/webserver.dir/webserver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/webserver.dir/webserver.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/webserver.c" -o CMakeFiles/webserver.dir/webserver.c.s

# Object files for target webserver
webserver_OBJECTS = \
"CMakeFiles/webserver.dir/webserver.c.o"

# External object files for target webserver
webserver_EXTERNAL_OBJECTS =

webserver: CMakeFiles/webserver.dir/webserver.c.o
webserver: CMakeFiles/webserver.dir/build.make
webserver: CMakeFiles/webserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable webserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserver.dir/build: webserver
.PHONY : CMakeFiles/webserver.dir/build

CMakeFiles/webserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserver.dir/clean

CMakeFiles/webserver.dir/depend:
	cd "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/build" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/build" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis1/praxis1/build/CMakeFiles/webserver.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/webserver.dir/depend

