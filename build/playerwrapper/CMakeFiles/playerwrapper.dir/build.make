# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/robot/projects/robotic

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/robot/projects/robotic/build

# Include any dependencies generated for this target.
include playerwrapper/CMakeFiles/playerwrapper.dir/depend.make

# Include the progress variables for this target.
include playerwrapper/CMakeFiles/playerwrapper.dir/progress.make

# Include the compile flags for this target's objects.
include playerwrapper/CMakeFiles/playerwrapper.dir/flags.make

playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o: playerwrapper/CMakeFiles/playerwrapper.dir/flags.make
playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o: ../playerwrapper/playerwrapper.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/robot/projects/robotic/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o"
	cd /home/robot/projects/robotic/build/playerwrapper && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o -c /home/robot/projects/robotic/playerwrapper/playerwrapper.cxx

playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/playerwrapper.dir/playerwrapper.cxx.i"
	cd /home/robot/projects/robotic/build/playerwrapper && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/robot/projects/robotic/playerwrapper/playerwrapper.cxx > CMakeFiles/playerwrapper.dir/playerwrapper.cxx.i

playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/playerwrapper.dir/playerwrapper.cxx.s"
	cd /home/robot/projects/robotic/build/playerwrapper && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/robot/projects/robotic/playerwrapper/playerwrapper.cxx -o CMakeFiles/playerwrapper.dir/playerwrapper.cxx.s

playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o.requires:
.PHONY : playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o.requires

playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o.provides: playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o.requires
	$(MAKE) -f playerwrapper/CMakeFiles/playerwrapper.dir/build.make playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o.provides.build
.PHONY : playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o.provides

playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o.provides.build: playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o

# Object files for target playerwrapper
playerwrapper_OBJECTS = \
"CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o"

# External object files for target playerwrapper
playerwrapper_EXTERNAL_OBJECTS =

playerwrapper/libplayerwrapper.a: playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o
playerwrapper/libplayerwrapper.a: playerwrapper/CMakeFiles/playerwrapper.dir/build.make
playerwrapper/libplayerwrapper.a: playerwrapper/CMakeFiles/playerwrapper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libplayerwrapper.a"
	cd /home/robot/projects/robotic/build/playerwrapper && $(CMAKE_COMMAND) -P CMakeFiles/playerwrapper.dir/cmake_clean_target.cmake
	cd /home/robot/projects/robotic/build/playerwrapper && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/playerwrapper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
playerwrapper/CMakeFiles/playerwrapper.dir/build: playerwrapper/libplayerwrapper.a
.PHONY : playerwrapper/CMakeFiles/playerwrapper.dir/build

playerwrapper/CMakeFiles/playerwrapper.dir/requires: playerwrapper/CMakeFiles/playerwrapper.dir/playerwrapper.cxx.o.requires
.PHONY : playerwrapper/CMakeFiles/playerwrapper.dir/requires

playerwrapper/CMakeFiles/playerwrapper.dir/clean:
	cd /home/robot/projects/robotic/build/playerwrapper && $(CMAKE_COMMAND) -P CMakeFiles/playerwrapper.dir/cmake_clean.cmake
.PHONY : playerwrapper/CMakeFiles/playerwrapper.dir/clean

playerwrapper/CMakeFiles/playerwrapper.dir/depend:
	cd /home/robot/projects/robotic/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/robot/projects/robotic /home/robot/projects/robotic/playerwrapper /home/robot/projects/robotic/build /home/robot/projects/robotic/build/playerwrapper /home/robot/projects/robotic/build/playerwrapper/CMakeFiles/playerwrapper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : playerwrapper/CMakeFiles/playerwrapper.dir/depend

