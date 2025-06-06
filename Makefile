# Compiler and flags
CXX = g++

# NOTE: for developement, can remove/add: -DDEBUG_PRINT -Wall -Wextra -pedantic
CXXFLAGS = -std=c++20 -I$(SRC_DIR)

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
DOCS_DIR = docs

# Output binary
TARGET = ipk25chat-client

# Source and object files - https://stackoverflow.com/questions/4036191/sources-from-subdirectories-in-makefile
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

# Link the binary
$(TARGET): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@rm -f $(OBJS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@) # Create the object directory if it doesn't exist
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	@rm -rf $(OBJ_DIR)

# Test target
test: all
	@echo "Running tests..."
	# TODO: add Catch2 or Google Test

# Zip target
zip:
	@zip -0 -r xpliha02.zip $(SRC_DIR) $(DOCS_DIR) CHANGELOG.md LICENSE Makefile README.md -x $(OBJ_DIR)/**\*

# Reference server target
ref: all
	@echo "Running program with reference server settings..."
	./$(TARGET) -t tcp -s anton5.fit.vutbr.cz -p 4567


# Help target
help:
	@echo "Available targets:"
	@echo "  make	   - Compile the project"
	@echo "  clean	  - Remove compiled files"
	@echo "  test	   - Run tests"
	@echo "  zip		- Create a zip file for submission"
	@echo "  ref		- Run the program with reference server settings"
	@echo "  help	   - Show this help message"

.PHONY: all clean test zip ref help