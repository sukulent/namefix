/* This file is part of Namefix - fix bad filenames
 *
 * Copyright (C) 2023 sukulent
 *
 * https://github.com/sukulent/namefix
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *
 */


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
#define NAMEFIX_VERSION "1.1.0"
#define NAMEFIX_VERSION_MAJOR 1
#define NAMEFIX_VERSION_MINOR 1
#define NAMEFIX_VERSION_PATCH 0

#define DEFAULT_MAX_CHARACTERS 64
#define DEFAULT_NON_ASCII_REPLACEMENT "?"
#define DEFAULT_SPACES_REPLACEMENT "_"
#define DEFAULT_PERIODS_REPLACEMENT "-"


// make ASCII from input
std::string makeASCII(const std::string &input, const std::string &placeholder);

// change spaces to replacement (in place)
void changeSpaces(std::string &input, const std::string &replacement);

// change periods to replacement (in place)
void changePeriods(std::string &input, const std::string &replacement);

#endif
