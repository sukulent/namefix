CXX = g++
CXXFLAGS= -std=c++17 -Wall -Wpedantic -Wextra -licuuc -licui18n
SOURCES = $(wildcard src/*.cpp)
NAME = namefix

main: $(SOURCES) 
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) && strip $(name)

sanitize: $(SOURCES) 
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) -g -fsanitize=leak

all: main

install: main
	chown root:root namefix
	chmod 755 namefix
	cp namefix /usr/bin/

uninstall:
	rm /usr/bin/namefix

clean:
	rm -f $(NAME)
