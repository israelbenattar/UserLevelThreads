# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /cygdrive/c/Users/istea/AppData/Local/JetBrains/CLion2021.2/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/istea/AppData/Local/JetBrains/CLion2021.2/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/UserLavelTreads.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/UserLavelTreads.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/UserLavelTreads.dir/flags.make

CMakeFiles/UserLavelTreads.dir/uthreads.cpp.o: CMakeFiles/UserLavelTreads.dir/flags.make
CMakeFiles/UserLavelTreads.dir/uthreads.cpp.o: ../uthreads.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/UserLavelTreads.dir/uthreads.cpp.o"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/UserLavelTreads.dir/uthreads.cpp.o -c /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/uthreads.cpp

CMakeFiles/UserLavelTreads.dir/uthreads.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/UserLavelTreads.dir/uthreads.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/uthreads.cpp > CMakeFiles/UserLavelTreads.dir/uthreads.cpp.i

CMakeFiles/UserLavelTreads.dir/uthreads.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/UserLavelTreads.dir/uthreads.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/uthreads.cpp -o CMakeFiles/UserLavelTreads.dir/uthreads.cpp.s

CMakeFiles/UserLavelTreads.dir/main.cpp.o: CMakeFiles/UserLavelTreads.dir/flags.make
CMakeFiles/UserLavelTreads.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/UserLavelTreads.dir/main.cpp.o"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/UserLavelTreads.dir/main.cpp.o -c /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/main.cpp

CMakeFiles/UserLavelTreads.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/UserLavelTreads.dir/main.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/main.cpp > CMakeFiles/UserLavelTreads.dir/main.cpp.i

CMakeFiles/UserLavelTreads.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/UserLavelTreads.dir/main.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/main.cpp -o CMakeFiles/UserLavelTreads.dir/main.cpp.s

# Object files for target UserLavelTreads
UserLavelTreads_OBJECTS = \
"CMakeFiles/UserLavelTreads.dir/uthreads.cpp.o" \
"CMakeFiles/UserLavelTreads.dir/main.cpp.o"

# External object files for target UserLavelTreads
UserLavelTreads_EXTERNAL_OBJECTS =

UserLavelTreads.exe: CMakeFiles/UserLavelTreads.dir/uthreads.cpp.o
UserLavelTreads.exe: CMakeFiles/UserLavelTreads.dir/main.cpp.o
UserLavelTreads.exe: CMakeFiles/UserLavelTreads.dir/build.make
UserLavelTreads.exe: CMakeFiles/UserLavelTreads.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable UserLavelTreads.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/UserLavelTreads.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/UserLavelTreads.dir/build: UserLavelTreads.exe
.PHONY : CMakeFiles/UserLavelTreads.dir/build

CMakeFiles/UserLavelTreads.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/UserLavelTreads.dir/cmake_clean.cmake
.PHONY : CMakeFiles/UserLavelTreads.dir/clean

CMakeFiles/UserLavelTreads.dir/depend:
	cd /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/cmake-build-debug /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/cmake-build-debug /cygdrive/c/Users/istea/CLionProjects/UserLavelTreads/cmake-build-debug/CMakeFiles/UserLavelTreads.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/UserLavelTreads.dir/depend

