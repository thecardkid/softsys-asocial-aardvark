# A Makefile based on the Google Test sample Makefile
# https://github.com/google/googletest/blob/master/googletest/make/Makefile
#
# Modified with the help of http://nuclear.mutantstargoat.com/articles/make/
#
# SYNOPSIS:
#
#   make - makes the main file.
#   make clean  - removes all files generated by make.

# Which compilers to use
CC=g++ -std=c++11
C=gcc

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = lib/googletest-release-1.8.0/googletest

# Where to find user code.
USER_DIR = src

# Test code
TEST_DIR = tests

# Where to find build objects
BUILD_DIR = build

# Dependency header files
DEP_HEADERS = $(USER_DIR)/*.h

VPATH = $(USER_DIR):$(BUILD_DIR)

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# Graphics flags
GRAPHICSFLAGS = -lglut -lGL -lGLU

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
RUN = $(BUILD_DIR)/main

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
				$(GTEST_DIR)/include/gtest/internal/*.h

# Build targets.
.PHONY: all
all : $(RUN)

.PHONY: clean
clean :
	rm -f $(BUILD_DIR)/*


# Builds Google test dependencies
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
$(BUILD_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CC) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
			$(GTEST_DIR)/src/gtest-all.cc -o $@

$(BUILD_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CC) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
			$(GTEST_DIR)/src/gtest_main.cc -o $@

$(BUILD_DIR)/gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^ -o $@

$(BUILD_DIR)/gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^ -o $@

# Builds the main file.
OBJ = $(BUILD_DIR)/main.o \
	  $(BUILD_DIR)/Graphics.o $(BUILD_DIR)/RubiksCube.o $(BUILD_DIR)/CubeFace.o \
	  $(BUILD_DIR)/Logic.o \
	  $(BUILD_DIR)/CubeFaceTest.o $(BUILD_DIR)/RubiksCubeTest.o

$(BUILD_DIR)/main : $(OBJ) gtest_main.a
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ $(GRAPHICSFLAGS)

# Builds the dependency object files.
$(BUILD_DIR)/main.o : $(USER_DIR)/main.cpp $(DEP_HEADERS) $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o : $(USER_DIR)/%.cpp $(USER_DIR)/%.h
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o : $(USER_DIR)/%.c $(USER_DIR)/%.h
	$(C) -c $< -o $@

$(BUILD_DIR)/%.o : $(TEST_DIR)/%.cpp $(DEP_HEADERS) $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@