# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11 -fPIC

# Archiver
AR = ar

# Archive flags
ARFLAGS = rcs

# Library name
LIB = libreactor.a

# Source files
SOURCES = reactor.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(LIB)

# Rule to create the static library
$(LIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

# Rule to compile source files to object files
%.o: %.cpp reactor.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS) $(LIB)

# Phony targets
.PHONY: all clean