
CXX = g++
CXXFLAGS = -Wall -Wextra -g -O0

# Folders
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Target Binary
TARGET = $(BIN_DIR)/lexer

# Source Files
SRCS = $(wildcard $(SRC_DIR)/*.cc)

# Object Files
OBJS = $(patsubst $(SRC_DIR)/%.cc, $(BUILD_DIR)/%.o, $(SRCS))

# Default Target
all: $(TARGET)

# Linking the Executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	echo "Linking $@"
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling Object Files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Ensure Directories Exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean Build Files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony Targets
.PHONY: all clean
