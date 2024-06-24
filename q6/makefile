# Makefile for Reactor Server Project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Target executable
TARGET = reactor_server

# Source files
SRCS = reactor.cpp graph_server_reactor.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = reactor.hpp

# Default target
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -lpthread

# Rule to compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
