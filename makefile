CXX = g++
CXXFLAGS= -g -std=c++17 -Wall -Wpedantic -Wextra -licuuc -licui18n
SOURCES = $(wildcard src/*.cpp)
NAME = namefix

main: $(SOURCES) 
	$(CXX) $^ -o $(NAME) $(CXXFLAGS)

sanitize: $(SOURCES) 
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) -fsanitize=leak

all: main

clean:
	rm -f $(NAME)
