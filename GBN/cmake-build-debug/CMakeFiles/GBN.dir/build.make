# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /snap/clion/129/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/129/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lyf/network/rdt-lab/GBN

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lyf/network/rdt-lab/GBN/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/GBN.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/GBN.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GBN.dir/flags.make

CMakeFiles/GBN.dir/src/GBN.cpp.o: CMakeFiles/GBN.dir/flags.make
CMakeFiles/GBN.dir/src/GBN.cpp.o: ../src/GBN.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyf/network/rdt-lab/GBN/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GBN.dir/src/GBN.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GBN.dir/src/GBN.cpp.o -c /home/lyf/network/rdt-lab/GBN/src/GBN.cpp

CMakeFiles/GBN.dir/src/GBN.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GBN.dir/src/GBN.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyf/network/rdt-lab/GBN/src/GBN.cpp > CMakeFiles/GBN.dir/src/GBN.cpp.i

CMakeFiles/GBN.dir/src/GBN.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GBN.dir/src/GBN.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyf/network/rdt-lab/GBN/src/GBN.cpp -o CMakeFiles/GBN.dir/src/GBN.cpp.s

CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.o: CMakeFiles/GBN.dir/flags.make
CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.o: ../src/GBNRdtReceiver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyf/network/rdt-lab/GBN/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.o -c /home/lyf/network/rdt-lab/GBN/src/GBNRdtReceiver.cpp

CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyf/network/rdt-lab/GBN/src/GBNRdtReceiver.cpp > CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.i

CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyf/network/rdt-lab/GBN/src/GBNRdtReceiver.cpp -o CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.s

CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.o: CMakeFiles/GBN.dir/flags.make
CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.o: ../src/GBNRdtSender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyf/network/rdt-lab/GBN/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.o -c /home/lyf/network/rdt-lab/GBN/src/GBNRdtSender.cpp

CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyf/network/rdt-lab/GBN/src/GBNRdtSender.cpp > CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.i

CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyf/network/rdt-lab/GBN/src/GBNRdtSender.cpp -o CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.s

# Object files for target GBN
GBN_OBJECTS = \
"CMakeFiles/GBN.dir/src/GBN.cpp.o" \
"CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.o" \
"CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.o"

# External object files for target GBN
GBN_EXTERNAL_OBJECTS =

../bin/GBN: CMakeFiles/GBN.dir/src/GBN.cpp.o
../bin/GBN: CMakeFiles/GBN.dir/src/GBNRdtReceiver.cpp.o
../bin/GBN: CMakeFiles/GBN.dir/src/GBNRdtSender.cpp.o
../bin/GBN: CMakeFiles/GBN.dir/build.make
../bin/GBN: ../lib/libnetsim.a
../bin/GBN: CMakeFiles/GBN.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyf/network/rdt-lab/GBN/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../bin/GBN"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GBN.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GBN.dir/build: ../bin/GBN

.PHONY : CMakeFiles/GBN.dir/build

CMakeFiles/GBN.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GBN.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GBN.dir/clean

CMakeFiles/GBN.dir/depend:
	cd /home/lyf/network/rdt-lab/GBN/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyf/network/rdt-lab/GBN /home/lyf/network/rdt-lab/GBN /home/lyf/network/rdt-lab/GBN/cmake-build-debug /home/lyf/network/rdt-lab/GBN/cmake-build-debug /home/lyf/network/rdt-lab/GBN/cmake-build-debug/CMakeFiles/GBN.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GBN.dir/depend

