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
include CMakeFiles/myls_R.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/myls_R.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myls_R.dir/flags.make

CMakeFiles/myls_R.dir/myls_R.cpp.o: CMakeFiles/myls_R.dir/flags.make
CMakeFiles/myls_R.dir/myls_R.cpp.o: ../myls_R.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/looper/CLionProjects/linuxExp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myls_R.dir/myls_R.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myls_R.dir/myls_R.cpp.o -c /home/looper/CLionProjects/linuxExp/myls_R.cpp

CMakeFiles/myls_R.dir/myls_R.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myls_R.dir/myls_R.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/looper/CLionProjects/linuxExp/myls_R.cpp > CMakeFiles/myls_R.dir/myls_R.cpp.i

CMakeFiles/myls_R.dir/myls_R.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myls_R.dir/myls_R.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/looper/CLionProjects/linuxExp/myls_R.cpp -o CMakeFiles/myls_R.dir/myls_R.cpp.s

# Object files for target myls_R
myls_R_OBJECTS = \
"CMakeFiles/myls_R.dir/myls_R.cpp.o"

# External object files for target myls_R
myls_R_EXTERNAL_OBJECTS =

myls_R: CMakeFiles/myls_R.dir/myls_R.cpp.o
myls_R: CMakeFiles/myls_R.dir/build.make
myls_R: CMakeFiles/myls_R.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/looper/CLionProjects/linuxExp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable myls_R"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myls_R.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myls_R.dir/build: myls_R

.PHONY : CMakeFiles/myls_R.dir/build

CMakeFiles/myls_R.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myls_R.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myls_R.dir/clean

CMakeFiles/myls_R.dir/depend:
	cd /home/looper/CLionProjects/linuxExp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/looper/CLionProjects/linuxExp /home/looper/CLionProjects/linuxExp /home/looper/CLionProjects/linuxExp/cmake-build-debug /home/looper/CLionProjects/linuxExp/cmake-build-debug /home/looper/CLionProjects/linuxExp/cmake-build-debug/CMakeFiles/myls_R.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myls_R.dir/depend

