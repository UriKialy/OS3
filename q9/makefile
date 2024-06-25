# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -pthread -std=c++11

# Target executable
TARGET = graph_server

# Source files
SRCS = graph_server.cpp reactor.cpp

# Header files
HEADERS = graph_server.hpp reactor.hpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

# Rule to clean up the build files
clean:
	rm -f $(TARGET) $(OBJS)

# Rule to run the server
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
