# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/looper/下载/CLion-2018.2.4/clion-2018.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/looper/下载/CLion-2018.2.4/clion-2018.2.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/looper/CLionProjects/SnakeWar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/looper/CLionProjects/SnakeWar/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/curses.h.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/curses.h.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/curses.h.dir/flags.make

CMakeFiles/curses.h.dir/test.cpp.o: CMakeFiles/curses.h.dir/flags.make
CMakeFiles/curses.h.dir/test.cpp.o: ../test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/looper/CLionProjects/SnakeWar/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/curses.h.dir/test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/curses.h.dir/test.cpp.o -c /home/looper/CLionProjects/SnakeWar/test.cpp

CMakeFiles/curses.h.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/curses.h.dir/test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/looper/CLionProjects/SnakeWar/test.cpp > CMakeFiles/curses.h.dir/test.cpp.i

CMakeFiles/curses.h.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/curses.h.dir/test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/looper/CLionProjects/SnakeWar/test.cpp -o CMakeFiles/curses.h.dir/test.cpp.s

# Object files for target curses.h
curses_h_OBJECTS = \
"CMakeFiles/curses.h.dir/test.cpp.o"

# External object files for target curses.h
curses_h_EXTERNAL_OBJECTS =

curses.h: CMakeFiles/curses.h.dir/test.cpp.o
curses.h: CMakeFiles/curses.h.dir/build.make
curses.h: CMakeFiles/curses.h.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/looper/CLionProjects/SnakeWar/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable curses.h"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/curses.h.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/curses.h.dir/build: curses.h

.PHONY : CMakeFiles/curses.h.dir/build

CMakeFiles/curses.h.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/curses.h.dir/cmake_clean.cmake
.PHONY : CMakeFiles/curses.h.dir/clean

CMakeFiles/curses.h.dir/depend:
	cd /home/looper/CLionProjects/SnakeWar/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/looper/CLionProjects/SnakeWar /home/looper/CLionProjects/SnakeWar /home/looper/CLionProjects/SnakeWar/cmake-build-debug /home/looper/CLionProjects/SnakeWar/cmake-build-debug /home/looper/CLionProjects/SnakeWar/cmake-build-debug/CMakeFiles/curses.h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/curses.h.dir/depend
