# CMAKE generated file: DO NOT EDIT!
<<<<<<< HEAD
# Generated by "Unix Makefiles" Generator, CMake Version 3.28
=======
# Generated by "Unix Makefiles" Generator, CMake Version 3.16
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

<<<<<<< HEAD
=======

>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

<<<<<<< HEAD
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
=======

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
<<<<<<< HEAD
RM = /usr/bin/cmake -E rm -f
=======
RM = /usr/bin/cmake -E remove -f
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
<<<<<<< HEAD
CMAKE_SOURCE_DIR = "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build"

# Include any dependencies generated for this target.
include CMakeFiles/webserver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/webserver.dir/compiler_depend.make
=======
CMAKE_SOURCE_DIR = /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build

# Include any dependencies generated for this target.
include CMakeFiles/webserver.dir/depend.make
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53

# Include the progress variables for this target.
include CMakeFiles/webserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserver.dir/flags.make

CMakeFiles/webserver.dir/webserver.c.o: CMakeFiles/webserver.dir/flags.make
<<<<<<< HEAD
CMakeFiles/webserver.dir/webserver.c.o: /mnt/e/Uni/Rechnernetze\ und\ Verteilte\ Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/webserver.c
CMakeFiles/webserver.dir/webserver.c.o: CMakeFiles/webserver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/webserver.dir/webserver.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/webserver.dir/webserver.c.o -MF CMakeFiles/webserver.dir/webserver.c.o.d -o CMakeFiles/webserver.dir/webserver.c.o -c "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/webserver.c"

CMakeFiles/webserver.dir/webserver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/webserver.dir/webserver.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/webserver.c" > CMakeFiles/webserver.dir/webserver.c.i

CMakeFiles/webserver.dir/webserver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/webserver.dir/webserver.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/webserver.c" -o CMakeFiles/webserver.dir/webserver.c.s

CMakeFiles/webserver.dir/http.c.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/http.c.o: /mnt/e/Uni/Rechnernetze\ und\ Verteilte\ Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/http.c
CMakeFiles/webserver.dir/http.c.o: CMakeFiles/webserver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/webserver.dir/http.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/webserver.dir/http.c.o -MF CMakeFiles/webserver.dir/http.c.o.d -o CMakeFiles/webserver.dir/http.c.o -c "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/http.c"

CMakeFiles/webserver.dir/http.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/webserver.dir/http.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/http.c" > CMakeFiles/webserver.dir/http.c.i

CMakeFiles/webserver.dir/http.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/webserver.dir/http.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/http.c" -o CMakeFiles/webserver.dir/http.c.s

CMakeFiles/webserver.dir/util.c.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/util.c.o: /mnt/e/Uni/Rechnernetze\ und\ Verteilte\ Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/util.c
CMakeFiles/webserver.dir/util.c.o: CMakeFiles/webserver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/webserver.dir/util.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/webserver.dir/util.c.o -MF CMakeFiles/webserver.dir/util.c.o.d -o CMakeFiles/webserver.dir/util.c.o -c "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/util.c"

CMakeFiles/webserver.dir/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/webserver.dir/util.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/util.c" > CMakeFiles/webserver.dir/util.c.i

CMakeFiles/webserver.dir/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/webserver.dir/util.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/util.c" -o CMakeFiles/webserver.dir/util.c.s

CMakeFiles/webserver.dir/data.c.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/data.c.o: /mnt/e/Uni/Rechnernetze\ und\ Verteilte\ Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/data.c
CMakeFiles/webserver.dir/data.c.o: CMakeFiles/webserver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/webserver.dir/data.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/webserver.dir/data.c.o -MF CMakeFiles/webserver.dir/data.c.o.d -o CMakeFiles/webserver.dir/data.c.o -c "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/data.c"

CMakeFiles/webserver.dir/data.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/webserver.dir/data.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/data.c" > CMakeFiles/webserver.dir/data.c.i

CMakeFiles/webserver.dir/data.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/webserver.dir/data.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/data.c" -o CMakeFiles/webserver.dir/data.c.s
=======
CMakeFiles/webserver.dir/webserver.c.o: ../webserver.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/webserver.dir/webserver.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/webserver.dir/webserver.c.o   -c /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/webserver.c

CMakeFiles/webserver.dir/webserver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/webserver.dir/webserver.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/webserver.c > CMakeFiles/webserver.dir/webserver.c.i

CMakeFiles/webserver.dir/webserver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/webserver.dir/webserver.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/webserver.c -o CMakeFiles/webserver.dir/webserver.c.s

CMakeFiles/webserver.dir/http.c.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/http.c.o: ../http.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/webserver.dir/http.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/webserver.dir/http.c.o   -c /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/http.c

CMakeFiles/webserver.dir/http.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/webserver.dir/http.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/http.c > CMakeFiles/webserver.dir/http.c.i

CMakeFiles/webserver.dir/http.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/webserver.dir/http.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/http.c -o CMakeFiles/webserver.dir/http.c.s

CMakeFiles/webserver.dir/util.c.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/util.c.o: ../util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/webserver.dir/util.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/webserver.dir/util.c.o   -c /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/util.c

CMakeFiles/webserver.dir/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/webserver.dir/util.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/util.c > CMakeFiles/webserver.dir/util.c.i

CMakeFiles/webserver.dir/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/webserver.dir/util.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/util.c -o CMakeFiles/webserver.dir/util.c.s

CMakeFiles/webserver.dir/data.c.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/data.c.o: ../data.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/webserver.dir/data.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/webserver.dir/data.c.o   -c /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/data.c

CMakeFiles/webserver.dir/data.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/webserver.dir/data.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/data.c > CMakeFiles/webserver.dir/data.c.i

CMakeFiles/webserver.dir/data.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/webserver.dir/data.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/data.c -o CMakeFiles/webserver.dir/data.c.s
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53

# Object files for target webserver
webserver_OBJECTS = \
"CMakeFiles/webserver.dir/webserver.c.o" \
"CMakeFiles/webserver.dir/http.c.o" \
"CMakeFiles/webserver.dir/util.c.o" \
"CMakeFiles/webserver.dir/data.c.o"

# External object files for target webserver
webserver_EXTERNAL_OBJECTS =

webserver: CMakeFiles/webserver.dir/webserver.c.o
webserver: CMakeFiles/webserver.dir/http.c.o
webserver: CMakeFiles/webserver.dir/util.c.o
webserver: CMakeFiles/webserver.dir/data.c.o
webserver: CMakeFiles/webserver.dir/build.make
webserver: CMakeFiles/webserver.dir/link.txt
<<<<<<< HEAD
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable webserver"
=======
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable webserver"
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserver.dir/build: webserver
<<<<<<< HEAD
=======

>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
.PHONY : CMakeFiles/webserver.dir/build

CMakeFiles/webserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserver.dir/clean

CMakeFiles/webserver.dir/depend:
<<<<<<< HEAD
	cd "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build" "/mnt/e/Uni/Rechnernetze und Verteilte Systeme/main/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles/webserver.dir/DependInfo.cmake" "--color=$(COLOR)"
=======
	cd /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2 /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2 /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build /mnt/d/RNVS/Rechnernetze-und-verteilte-Systeme/praxis2/build/CMakeFiles/webserver.dir/DependInfo.cmake --color=$(COLOR)
>>>>>>> 0abcf641de2c5d12781ae030ae06baad283f2c53
.PHONY : CMakeFiles/webserver.dir/depend

