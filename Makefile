# Makefile for Windows (using mingw32-make)

# Compiler
CXX := g++

# Project layout
SRC_DIR := src
BUILD_DIR := build
BIN := bin
TARGET := $(BIN)\app.exe

# Find all .cpp files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Compiler flags
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude
DEPSFLAGS := -MMD -MP

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN)
	@echo Linking $@
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo Compiling $<
	$(CXX) $(CXXFLAGS) $(DEPSFLAGS) -c $< -o $@

$(BIN):
	@if not exist $(BIN) mkdir $(BIN)

$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

clean:
	@echo Cleaning
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(BIN) rmdir /s /q $(BIN)

# Include dependency files if they exist
-include $(OBJS:.o=.d)

.PHONY: all clean