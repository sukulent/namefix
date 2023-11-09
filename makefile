CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wpedantic -Wextra -licuuc -licui18n
SOURCES = $(wildcard src/*.cpp)
NAME = namefix

all: namefix

namefix: $(SOURCES)
	$(CXX) $^ -o $@ $(CXXFLAGS)

strip: $(SOURCES)
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) && strip $(NAME)

sanitize: $(SOURCES)
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) -g -fsanitize=leak

install: namefix
	cp namefix /usr/local/bin/
	chown root:root /usr/local/bin/namefix
	chmod 755 /usr/local/bin/namefix

uninstall:
	rm /usr/local/bin/namefix

clean:
	rm -f $(NAME)

.PHONY: all sanitize install uninstall clean

.DEFAULT: all
