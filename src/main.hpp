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
#define VERSION "0.1"
#define DEFAULT_MAX_CHARACTERS 64
#define NON_ASCII_PLACEHOLDER "?"
#define DEFAULT_SYSTEM_LOOKUP_PATH 	"confusablesSummary.txt"
#define DEFAULT_USER_LOOKUP_PATH	"namefix.table"


// make ASCII from input
std::string makeASCII(const std::string &input);

// change spaces to replacement (in place)
void changeSpaces(std::string &input, const std::string &replacement);


void print_translit_IDs();

#endif