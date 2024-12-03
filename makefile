# Makefile for compiling an OpenVDB project with multiple source files

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -I/usr/local/include/openvdb -I./include

# Linker flags for libraries
LDFLAGS = -L/usr/local/lib -lopenvdb -L/usr/lib/x86_64-linux-gnu -ltbb

# Output binary
OUTPUT = sciFock

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files
MAIN_SRC = main.cpp
SRC_SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
MAIN_OBJ = $(OBJ_DIR)/main.o
SRC_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_SRCS))
OBJS = $(MAIN_OBJ) $(SRC_OBJS)

# Rule to build the program
all: $(OUTPUT)

# Create output binary
$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(OBJS) $(LDFLAGS)

# Compile each .cpp file into obj/ directory
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure the obj directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to clean up build files
clean:
	rm -f $(OUTPUT)
	rm -rf $(OBJ_DIR)

.PHONY: all clean
