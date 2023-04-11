CXX = g++
CXXFLAGS= -std=c++17 -Wall -Wpedantic -Wextra -licuuc -licui18n
SOURCES = $(wildcard src/*.cpp)
NAME = namefix

main: $(SOURCES) 
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) && strip $(name)

sanitize: $(SOURCES) 
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) -g -fsanitize=leak

all: main

clean:
	rm -f $(NAME)
