CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wpedantic -Wextra -licuuc -licui18n
SOURCES = $(wildcard src/*.cpp)
NAME = namefix

all: strip

namefix: $(SOURCES)
	$(CXX) $^ -o $@ $(CXXFLAGS)

strip: $(SOURCES)
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) && strip $(NAME)

sanitize: $(SOURCES)
	$(CXX) $^ -o $(NAME) $(CXXFLAGS) -g -fsanitize=leak

install: namefix
	install -Dm755 ./namefix "/usr/local/bin/namefix"
	install -Dm644 ./README.md "/usr/local/share/doc/namefix"
	install -Dm644 ./man/namefix.1 "/usr/local/share/man/man1/namefix.1"

uninstall:
	rm /usr/local/bin/namefix

clean:
	rm -f $(NAME)

.PHONY: all sanitize install uninstall clean

.DEFAULT: all
