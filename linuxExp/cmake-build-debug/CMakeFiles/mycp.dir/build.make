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
CMAKE_SOURCE_DIR = /home/looper/CLionProjects/linuxExp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/looper/CLionProjects/linuxExp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/mycp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mycp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mycp.dir/flags.make

CMakeFiles/mycp.dir/mycp.cpp.o: CMakeFiles/mycp.dir/flags.make
CMakeFiles/mycp.dir/mycp.cpp.o: ../mycp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/looper/CLionProjects/linuxExp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mycp.dir/mycp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mycp.dir/mycp.cpp.o -c /home/looper/CLionProjects/linuxExp/mycp.cpp

CMakeFiles/mycp.dir/mycp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mycp.dir/mycp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/looper/CLionProjects/linuxExp/mycp.cpp > CMakeFiles/mycp.dir/mycp.cpp.i

CMakeFiles/mycp.dir/mycp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mycp.dir/mycp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/looper/CLionProjects/linuxExp/mycp.cpp -o CMakeFiles/mycp.dir/mycp.cpp.s

# Object files for target mycp
mycp_OBJECTS = \
"CMakeFiles/mycp.dir/mycp.cpp.o"

# External object files for target mycp
mycp_EXTERNAL_OBJECTS =

mycp: CMakeFiles/mycp.dir/mycp.cpp.o
mycp: CMakeFiles/mycp.dir/build.make
mycp: CMakeFiles/mycp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/looper/CLionProjects/linuxExp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mycp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mycp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mycp.dir/build: mycp

.PHONY : CMakeFiles/mycp.dir/build

CMakeFiles/mycp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mycp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mycp.dir/clean

CMakeFiles/mycp.dir/depend:
	cd /home/looper/CLionProjects/linuxExp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/looper/CLionProjects/linuxExp /home/looper/CLionProjects/linuxExp /home/looper/CLionProjects/linuxExp/cmake-build-debug /home/looper/CLionProjects/linuxExp/cmake-build-debug /home/looper/CLionProjects/linuxExp/cmake-build-debug/CMakeFiles/mycp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mycp.dir/depend

