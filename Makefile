# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/2.8.9/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/2.8.9/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/Cellar/cmake/2.8.9/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/bpartridge/Dropbox/Developer/Music/jucebouncer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/bpartridge/Dropbox/Developer/Music/jucebouncer

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/2.8.9/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/2.8.9/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/bpartridge/Dropbox/Developer/Music/jucebouncer/CMakeFiles /Users/bpartridge/Dropbox/Developer/Music/jucebouncer/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/bpartridge/Dropbox/Developer/Music/jucebouncer/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named jucebouncer

# Build rule for target.
jucebouncer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 jucebouncer
.PHONY : jucebouncer

# fast build rule for target.
jucebouncer/fast:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/build
.PHONY : jucebouncer/fast

lib/juce/modules/juce_audio_basics/juce_audio_basics.o: lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.o
.PHONY : lib/juce/modules/juce_audio_basics/juce_audio_basics.o

# target to build an object file
lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.o
.PHONY : lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.o

lib/juce/modules/juce_audio_basics/juce_audio_basics.i: lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.i
.PHONY : lib/juce/modules/juce_audio_basics/juce_audio_basics.i

# target to preprocess a source file
lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.i
.PHONY : lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.i

lib/juce/modules/juce_audio_basics/juce_audio_basics.s: lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.s
.PHONY : lib/juce/modules/juce_audio_basics/juce_audio_basics.s

# target to generate assembly for a file
lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.s
.PHONY : lib/juce/modules/juce_audio_basics/juce_audio_basics.cpp.s

lib/juce/modules/juce_audio_devices/juce_audio_devices.o: lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.o
.PHONY : lib/juce/modules/juce_audio_devices/juce_audio_devices.o

# target to build an object file
lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.o
.PHONY : lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.o

lib/juce/modules/juce_audio_devices/juce_audio_devices.i: lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.i
.PHONY : lib/juce/modules/juce_audio_devices/juce_audio_devices.i

# target to preprocess a source file
lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.i
.PHONY : lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.i

lib/juce/modules/juce_audio_devices/juce_audio_devices.s: lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.s
.PHONY : lib/juce/modules/juce_audio_devices/juce_audio_devices.s

# target to generate assembly for a file
lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.s
.PHONY : lib/juce/modules/juce_audio_devices/juce_audio_devices.cpp.s

lib/juce/modules/juce_audio_formats/juce_audio_formats.o: lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.o
.PHONY : lib/juce/modules/juce_audio_formats/juce_audio_formats.o

# target to build an object file
lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.o
.PHONY : lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.o

lib/juce/modules/juce_audio_formats/juce_audio_formats.i: lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.i
.PHONY : lib/juce/modules/juce_audio_formats/juce_audio_formats.i

# target to preprocess a source file
lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.i
.PHONY : lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.i

lib/juce/modules/juce_audio_formats/juce_audio_formats.s: lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.s
.PHONY : lib/juce/modules/juce_audio_formats/juce_audio_formats.s

# target to generate assembly for a file
lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.s
.PHONY : lib/juce/modules/juce_audio_formats/juce_audio_formats.cpp.s

lib/juce/modules/juce_audio_processors/juce_audio_processors.o: lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.o
.PHONY : lib/juce/modules/juce_audio_processors/juce_audio_processors.o

# target to build an object file
lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.o
.PHONY : lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.o

lib/juce/modules/juce_audio_processors/juce_audio_processors.i: lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.i
.PHONY : lib/juce/modules/juce_audio_processors/juce_audio_processors.i

# target to preprocess a source file
lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.i
.PHONY : lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.i

lib/juce/modules/juce_audio_processors/juce_audio_processors.s: lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.s
.PHONY : lib/juce/modules/juce_audio_processors/juce_audio_processors.s

# target to generate assembly for a file
lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.s
.PHONY : lib/juce/modules/juce_audio_processors/juce_audio_processors.cpp.s

lib/juce/modules/juce_audio_utils/juce_audio_utils.o: lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.o
.PHONY : lib/juce/modules/juce_audio_utils/juce_audio_utils.o

# target to build an object file
lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.o
.PHONY : lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.o

lib/juce/modules/juce_audio_utils/juce_audio_utils.i: lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.i
.PHONY : lib/juce/modules/juce_audio_utils/juce_audio_utils.i

# target to preprocess a source file
lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.i
.PHONY : lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.i

lib/juce/modules/juce_audio_utils/juce_audio_utils.s: lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.s
.PHONY : lib/juce/modules/juce_audio_utils/juce_audio_utils.s

# target to generate assembly for a file
lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.s
.PHONY : lib/juce/modules/juce_audio_utils/juce_audio_utils.cpp.s

lib/juce/modules/juce_core/juce_core.o: lib/juce/modules/juce_core/juce_core.cpp.o
.PHONY : lib/juce/modules/juce_core/juce_core.o

# target to build an object file
lib/juce/modules/juce_core/juce_core.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_core/juce_core.cpp.o
.PHONY : lib/juce/modules/juce_core/juce_core.cpp.o

lib/juce/modules/juce_core/juce_core.i: lib/juce/modules/juce_core/juce_core.cpp.i
.PHONY : lib/juce/modules/juce_core/juce_core.i

# target to preprocess a source file
lib/juce/modules/juce_core/juce_core.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_core/juce_core.cpp.i
.PHONY : lib/juce/modules/juce_core/juce_core.cpp.i

lib/juce/modules/juce_core/juce_core.s: lib/juce/modules/juce_core/juce_core.cpp.s
.PHONY : lib/juce/modules/juce_core/juce_core.s

# target to generate assembly for a file
lib/juce/modules/juce_core/juce_core.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_core/juce_core.cpp.s
.PHONY : lib/juce/modules/juce_core/juce_core.cpp.s

lib/juce/modules/juce_data_structures/juce_data_structures.o: lib/juce/modules/juce_data_structures/juce_data_structures.cpp.o
.PHONY : lib/juce/modules/juce_data_structures/juce_data_structures.o

# target to build an object file
lib/juce/modules/juce_data_structures/juce_data_structures.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_data_structures/juce_data_structures.cpp.o
.PHONY : lib/juce/modules/juce_data_structures/juce_data_structures.cpp.o

lib/juce/modules/juce_data_structures/juce_data_structures.i: lib/juce/modules/juce_data_structures/juce_data_structures.cpp.i
.PHONY : lib/juce/modules/juce_data_structures/juce_data_structures.i

# target to preprocess a source file
lib/juce/modules/juce_data_structures/juce_data_structures.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_data_structures/juce_data_structures.cpp.i
.PHONY : lib/juce/modules/juce_data_structures/juce_data_structures.cpp.i

lib/juce/modules/juce_data_structures/juce_data_structures.s: lib/juce/modules/juce_data_structures/juce_data_structures.cpp.s
.PHONY : lib/juce/modules/juce_data_structures/juce_data_structures.s

# target to generate assembly for a file
lib/juce/modules/juce_data_structures/juce_data_structures.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_data_structures/juce_data_structures.cpp.s
.PHONY : lib/juce/modules/juce_data_structures/juce_data_structures.cpp.s

lib/juce/modules/juce_events/juce_events.o: lib/juce/modules/juce_events/juce_events.cpp.o
.PHONY : lib/juce/modules/juce_events/juce_events.o

# target to build an object file
lib/juce/modules/juce_events/juce_events.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_events/juce_events.cpp.o
.PHONY : lib/juce/modules/juce_events/juce_events.cpp.o

lib/juce/modules/juce_events/juce_events.i: lib/juce/modules/juce_events/juce_events.cpp.i
.PHONY : lib/juce/modules/juce_events/juce_events.i

# target to preprocess a source file
lib/juce/modules/juce_events/juce_events.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_events/juce_events.cpp.i
.PHONY : lib/juce/modules/juce_events/juce_events.cpp.i

lib/juce/modules/juce_events/juce_events.s: lib/juce/modules/juce_events/juce_events.cpp.s
.PHONY : lib/juce/modules/juce_events/juce_events.s

# target to generate assembly for a file
lib/juce/modules/juce_events/juce_events.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_events/juce_events.cpp.s
.PHONY : lib/juce/modules/juce_events/juce_events.cpp.s

lib/juce/modules/juce_graphics/juce_graphics.o: lib/juce/modules/juce_graphics/juce_graphics.cpp.o
.PHONY : lib/juce/modules/juce_graphics/juce_graphics.o

# target to build an object file
lib/juce/modules/juce_graphics/juce_graphics.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_graphics/juce_graphics.cpp.o
.PHONY : lib/juce/modules/juce_graphics/juce_graphics.cpp.o

lib/juce/modules/juce_graphics/juce_graphics.i: lib/juce/modules/juce_graphics/juce_graphics.cpp.i
.PHONY : lib/juce/modules/juce_graphics/juce_graphics.i

# target to preprocess a source file
lib/juce/modules/juce_graphics/juce_graphics.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_graphics/juce_graphics.cpp.i
.PHONY : lib/juce/modules/juce_graphics/juce_graphics.cpp.i

lib/juce/modules/juce_graphics/juce_graphics.s: lib/juce/modules/juce_graphics/juce_graphics.cpp.s
.PHONY : lib/juce/modules/juce_graphics/juce_graphics.s

# target to generate assembly for a file
lib/juce/modules/juce_graphics/juce_graphics.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_graphics/juce_graphics.cpp.s
.PHONY : lib/juce/modules/juce_graphics/juce_graphics.cpp.s

lib/juce/modules/juce_gui_basics/juce_gui_basics.o: lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.o
.PHONY : lib/juce/modules/juce_gui_basics/juce_gui_basics.o

# target to build an object file
lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.o
.PHONY : lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.o

lib/juce/modules/juce_gui_basics/juce_gui_basics.i: lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.i
.PHONY : lib/juce/modules/juce_gui_basics/juce_gui_basics.i

# target to preprocess a source file
lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.i
.PHONY : lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.i

lib/juce/modules/juce_gui_basics/juce_gui_basics.s: lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.s
.PHONY : lib/juce/modules/juce_gui_basics/juce_gui_basics.s

# target to generate assembly for a file
lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.s
.PHONY : lib/juce/modules/juce_gui_basics/juce_gui_basics.cpp.s

lib/juce/modules/juce_gui_extra/juce_gui_extra.o: lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.o
.PHONY : lib/juce/modules/juce_gui_extra/juce_gui_extra.o

# target to build an object file
lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.o
.PHONY : lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.o

lib/juce/modules/juce_gui_extra/juce_gui_extra.i: lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.i
.PHONY : lib/juce/modules/juce_gui_extra/juce_gui_extra.i

# target to preprocess a source file
lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.i
.PHONY : lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.i

lib/juce/modules/juce_gui_extra/juce_gui_extra.s: lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.s
.PHONY : lib/juce/modules/juce_gui_extra/juce_gui_extra.s

# target to generate assembly for a file
lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.s
.PHONY : lib/juce/modules/juce_gui_extra/juce_gui_extra.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/jucebouncer.dir/build.make CMakeFiles/jucebouncer.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... jucebouncer"
	@echo "... rebuild_cache"
	@echo "... lib/juce/modules/juce_audio_basics/juce_audio_basics.o"
	@echo "... lib/juce/modules/juce_audio_basics/juce_audio_basics.i"
	@echo "... lib/juce/modules/juce_audio_basics/juce_audio_basics.s"
	@echo "... lib/juce/modules/juce_audio_devices/juce_audio_devices.o"
	@echo "... lib/juce/modules/juce_audio_devices/juce_audio_devices.i"
	@echo "... lib/juce/modules/juce_audio_devices/juce_audio_devices.s"
	@echo "... lib/juce/modules/juce_audio_formats/juce_audio_formats.o"
	@echo "... lib/juce/modules/juce_audio_formats/juce_audio_formats.i"
	@echo "... lib/juce/modules/juce_audio_formats/juce_audio_formats.s"
	@echo "... lib/juce/modules/juce_audio_processors/juce_audio_processors.o"
	@echo "... lib/juce/modules/juce_audio_processors/juce_audio_processors.i"
	@echo "... lib/juce/modules/juce_audio_processors/juce_audio_processors.s"
	@echo "... lib/juce/modules/juce_audio_utils/juce_audio_utils.o"
	@echo "... lib/juce/modules/juce_audio_utils/juce_audio_utils.i"
	@echo "... lib/juce/modules/juce_audio_utils/juce_audio_utils.s"
	@echo "... lib/juce/modules/juce_core/juce_core.o"
	@echo "... lib/juce/modules/juce_core/juce_core.i"
	@echo "... lib/juce/modules/juce_core/juce_core.s"
	@echo "... lib/juce/modules/juce_data_structures/juce_data_structures.o"
	@echo "... lib/juce/modules/juce_data_structures/juce_data_structures.i"
	@echo "... lib/juce/modules/juce_data_structures/juce_data_structures.s"
	@echo "... lib/juce/modules/juce_events/juce_events.o"
	@echo "... lib/juce/modules/juce_events/juce_events.i"
	@echo "... lib/juce/modules/juce_events/juce_events.s"
	@echo "... lib/juce/modules/juce_graphics/juce_graphics.o"
	@echo "... lib/juce/modules/juce_graphics/juce_graphics.i"
	@echo "... lib/juce/modules/juce_graphics/juce_graphics.s"
	@echo "... lib/juce/modules/juce_gui_basics/juce_gui_basics.o"
	@echo "... lib/juce/modules/juce_gui_basics/juce_gui_basics.i"
	@echo "... lib/juce/modules/juce_gui_basics/juce_gui_basics.s"
	@echo "... lib/juce/modules/juce_gui_extra/juce_gui_extra.o"
	@echo "... lib/juce/modules/juce_gui_extra/juce_gui_extra.i"
	@echo "... lib/juce/modules/juce_gui_extra/juce_gui_extra.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

