#ifndef MAIN_HPP
#define MAIN_HPP

// includes
#include "args.hxx"
#include <iostream>
#include <filesystem>
#include <map>
#include <fstream>
#include <filesystem>
#include <regex>
#include <unicode/unistr.h>
#include <unicode/translit.h>
#include <unicode/utf.h>
#include <unicode/normalizer2.h>

// defines
#define NAMEFIX_VERSION "1.0.0"
#define NAMEFIX_VERSION_MAJOR 1
#define NAMEFIX_VERSION_MINOR 0
#define NAMEFIX_VERSION_PATCH 0

#define DEFAULT_MAX_CHARACTERS 64
#define DEFAULT_NON_ASCII_REPLACEMENT "?"
#define DEFAULT_SPACES_REPLACEMENT "_"
#define DEFAULT_PERIODS_REPLACEMENT "-"


// make ASCII from input
std::string makeASCII(const std::string &input);

// change spaces to replacement (in place)
void changeSpaces(std::string &input, const std::string &replacement);

// change periods to replacement (in place)
void changePeriods(std::string &input, const std::string &replacement);

#endif
