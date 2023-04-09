#include "main.hpp"

int main(int argc, char **argv)
{
	// setting up the argument parser
	args::ArgumentParser parser("Namefix - fix bad filenames\nTries to rename files so all characters are ASCII, keeps extensions untouched");

	args::Flag verbose(parser, "verbose", "Verbose output", {'v', "verbose"}, false);
	args::Flag verbose_long(parser, "verbose-long", "Verbose output - absolute paths", {'V', "verbose-long"}, false);

	args::Flag report(parser, "report", "Just report all files not conforming", {'r', "report"}, false);
	args::Flag dry_run(parser, "dry-run", "Dry run, don't do anything, just print what would have been done (implies -v)", {"dry-run"}, false);
	args::Flag ignore_errors(parser, "ignore-errors", "Do not stop when error is encountered", {'I',"ignore-errors"}, false);

	args::ValueFlag<std::string> spaces(parser, "STR", "Replace spaces with STR", {'s', "spaces"}, "_");
	args::Flag no_spaces(parser, "no-spaces", "Do not replace spaces - default replacement is underscore", {'S', "no-spaces"}, false);

	args::ValueFlag<std::string> dots(parser, "STR", "Replace dots with STR", {'d', "dots"}, "-");
	args::Flag no_dots(parser, "no-dots", "Do not replace dots - default replacement is hyphen/dash", {'D', "no-dots"}, false);

	args::ValueFlag<u_int> characters(parser, "NUM", "The maximum length for the new filename (without extension!) Default " + std::to_string(DEFAULT_MAX_CHARACTERS), {'c', "characters"}, DEFAULT_MAX_CHARACTERS);

	args::ValueFlag<std::string> keep_path(parser, "PATH", "Do not rename, make a copy with the new name and put it in PATH", {'k', "keep-path"});
	args::Flag keep(parser, "PATH", "Do not rename, make a copy with new name", {'K', "keep"}, false);

	args::ValueFlag<std::string> to_ascii(parser, "STR", "Test a string - transforms a UTF-8 string into ASCII and prints it, does not keep extensions", {'t', "to-ascii"});
	args::Flag version(parser, "version", "Outputs the version", {"version"}, false);
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

	// list for input files
	args::PositionalList<std::string> input_file(parser, "input", "Input files");

	// parsing args
	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help &e)
	{
		std::cout << parser.Help();
		return 0;
	}
	catch (args::ParseError &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}
	catch (args::ValidationError &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

	// custom verbose variable so dry_run can force it
	bool custom_verbose = (verbose > 0) || (dry_run > 0) ? true : false;

	// count errors
	u_int error_num = 0;

	// Parsing done

	// just print version
	if (version)
	{
		std::cout << "Namefix version " << VERSION << std::endl;
		return 0;
	}

	// if flag test is specified, convert and print the string
	if (to_ascii)
	{
		std::cout << makeASCII(to_ascii.Get()) << std::endl;
		return 0;
	}

	// check for work
	if (!input_file)
	{
		std::cerr << "No files specified." << std::endl;
		return 1;
	}

	// go through input files
	for (auto const &iter : input_file.Get())
	{
		// check if file exists
		if (!std::filesystem::exists(iter))
		{
			std::cerr << "File \"" << iter << "\" not found" << std::endl;
			if (ignore_errors)
			{
				error_num++;
				continue;
			}
			return 1;
		}

		// create path objects
		std::filesystem::path original = iter;
		std::string extension = original.extension();
		std::string stem(original.stem().string());

		// convert the filename to ascii
		stem = makeASCII(stem);

		// resize the name if it's too long
		if (stem.size() > characters.Get())
		{
			stem.resize(characters.Get());
		}

		// space conversion pass
		if (!no_spaces)
		{
			changeSpaces(stem, spaces.Get());
		}

		// dot conversion pass
		if (!no_dots)
		{
			changeDots(stem, dots.Get());
		}

		// make the new path from original or specified
		std::filesystem::path renamed(original);
		if (keep_path)
		{
			if (!std::filesystem::exists(keep_path.Get()))
			{
				std::cerr << "Path \"" << keep_path.Get() << "\" not found" << std::endl;
				return 1;
			}
			renamed.assign(keep_path.Get());
			renamed.append(stem + extension);
		}
		else
		{
			renamed.remove_filename();
			renamed.append(stem + extension);
		}

		// just report bad files
		if (report)
		{
			if (original != renamed)
			{
				std::cout << original.string() << std::endl;
			}
			continue;
		}

		// if the filenames are the same, nothing to do
		if (original == renamed)
		{
			if (verbose_long)
				std::cout << std::filesystem::absolute(original).string() << " -> Is alread OK!" << std::endl;
			else if (custom_verbose)
				std::cout << original.string() << "\t -> Is already OK" << std::endl;
			continue;
		}
		else
		{
			if (verbose_long)
				std::cout << std::filesystem::absolute(original).string() << " -> " << std::filesystem::absolute(renamed).string() << std::endl;
			else if (custom_verbose)
				std::cout << original.string() << " -> " << renamed.string() << std::endl;
		}

		// rename the file (if we are not running dry)
		if (!dry_run)
		{
			if (keep_path || keep)
			{
				try
				{
					std::filesystem::copy_file(original, renamed, std::filesystem::copy_options::copy_symlinks);
				}
				catch (const std::filesystem::filesystem_error &e)
				{
					std::cerr << "Error while copying " << original << " -> " << renamed << " -- " << e.what() << std::endl;
					if (ignore_errors)
					{
						error_num++;
						continue;
					}
					return 1;
				}
			}
			else
			{
				try
				{
					std::filesystem::rename(original, renamed);
				}
				catch (const std::filesystem::filesystem_error &e)
				{
					std::cerr << "Error while renaming " << original << " -> " << renamed << " -- " << e.what() << std::endl;
					if (ignore_errors)
					{
						error_num++;
						continue;
					}
					return 1;
				}
			}
		}
	}

	if (error_num)
	{
		std::cerr << "Finished with " << error_num << " error(s)." << std::endl;
		return 1;
	}
	else
		return 0;
}

// Functions

// take a string and make it ASCII - a multistep process
std::string makeASCII(const std::string &input)
{
	UErrorCode status = U_ZERO_ERROR;

	// make a unicode string from input
	icu::UnicodeString uinput = icu::UnicodeString::fromUTF8(input);

	// Create a Normalizer2 object for Unicode canonical decomposition
	const icu::Normalizer2 *normalizer_decompose = icu::Normalizer2::getNFDInstance(status);

	// Create a Normalizer2 object for Unicode canonical composition
	const icu::Normalizer2 *normalizer_compose = icu::Normalizer2::getNFCInstance(status);

	// Normalize the string to the NFC form - should decompose all characters
	uinput = normalizer_decompose->normalize(uinput, status);

	// Normalize the string to the NFC form - should compose all characters back
	uinput = normalizer_compose->normalize(uinput, status);

	// define the transtliterator identifiers
	const icu::UnicodeString ID_latin("Any-Latin");
	const icu::UnicodeString ID_ascii("Any-ASCII");

	// creat the transliterators
	icu::Transliterator *translit_latin = icu::Transliterator::createInstance(ID_latin, UTRANS_FORWARD, status);
	icu::Transliterator *translit_ascii = icu::Transliterator::createInstance(ID_ascii, UTRANS_FORWARD, status);

	// transliterate - firstly make latin-like, secondly ASCII from that.
	translit_latin->transliterate(uinput);
	translit_ascii->transliterate(uinput);

	// convert back to std::string
	std::string output;
	uinput.toUTF8String(output);

	// free the transliterator objects
	delete translit_latin;
	delete translit_ascii;

	// if any non-ASCII characters are left, replace them with placeholder
	std::regex non_ascii("[^\\x00-\\x7F]++"); // regular expression to match non-ASCII characters, if multiple are right after each other, will replace with one placeholder
	output = std::regex_replace(output, non_ascii, NON_ASCII_PLACEHOLDER);

	return output;
}

void changeSpaces(std::string &input, const std::string &replacement)
{
	std::regex regex(" ");
	input = std::regex_replace(input, regex, replacement);
}

void changeDots(std::string &input, const std::string &replacement)
{
	std::regex regex("\\.");
	input = std::regex_replace(input, regex, replacement);
}
