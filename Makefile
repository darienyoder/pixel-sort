# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I include -Wall -Wextra -std=c++17

# Source files in the root directory
SOURCES = $(wildcard *.cpp)

# Header files in the root directory
HEADERS = $(wildcard *.hpp)

# Source files in the include directory (recursively)
INCLUDE_SOURCES = $(wildcard include/**/*.cpp)

# All source files
ALL_SOURCES = $(SOURCES) $(INCLUDE_SOURCES)

# Object files (replace .cpp with .o)
OBJECTS = $(ALL_SOURCES:.cpp=.o)

# Target executable
TARGET = sorter

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
