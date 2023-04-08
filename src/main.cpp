#include "main.hpp"

int main(int argc, char **argv)
{

	// setting up the argument parser
	args::ArgumentParser parser("Namefix - fix bad filenames\nTries to renames files so all characters are ASCII, keeps extensions untouched");

	args::Flag verbose(parser, "verbose", "Verbose output", {'v', "verbose"}, false);
	args::Flag verbose_long(parser, "verbose-long", "Verbose output - including absolute paths", {'V', "verbose-long"}, false);

	args::Flag dry_run(parser, "dry-run", "Dry run, don't do anything, just print what would have been done (implies -v)", {"dry-run"}, false);

	args::ValueFlag<std::string> spaces(parser, "STR", "Replace spaces with STR", {'s', "spaces"}, "_");
	args::Flag no_spaces(parser, "no-spaces", "Do not replace spaces - default replacement is a underscore", {'S', "no-spaces"}, false);

	args::ValueFlag<std::string> dots(parser, "STR", "Replace dots with STR", {'d',"dots"}, "-");
	args::Flag no_dots(parser, "no-dots", "Do not replace dots - default replacement is a hyphen/dash", {'D',"no-dots"}, false);

	args::ValueFlag<int> characters(parser, "NUM", "The maximum length for the new filename (without extension!)", {'c', "characters"}, DEFAULT_MAX_CHARACTERS);

	args::ValueFlag<std::string> to_ascii(parser, "STR", "Test a string - transforms a UTF-8 string into a ASCII characters and prints it, does not keep extensions", {'t', "to-ascii"}, "");

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
	bool custom_verbose = verbose > 0 || dry_run > 0 ? true : false;

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
		// skip file if it doesn't exist
		if (!std::filesystem::exists(iter))
		{
			std::cerr << "File \"" << iter << "\" not found" << std::endl;
			continue;
		}

		// create path objects
		std::filesystem::path original = iter;
		std::string extension = original.extension();
		std::string stem(original.stem().string());

		// convert the filename to ascii
		stem = makeASCII(stem);

		// resize the name if it's too long
		if (static_cast<int>(stem.size()) > characters.Get())
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

		// duplicate the path and modify the filename
		std::filesystem::path renamed(original);
		renamed.remove_filename();
		renamed.append(stem + extension);

		// if the filenames are the same, nothing to do
		if (original == renamed)
		{
			if (verbose_long)
				std::cout << std::filesystem::absolute(original).string() << "\t -> Is alread OK!" << std::endl;
			else if (custom_verbose)
				std::cout << original.string() << "\t -> Is already OK" << std::endl;
			continue;
		}
		else
		{
			if (verbose_long)
				std::cout << std::filesystem::absolute(original).string() << "\t -> \t" << std::filesystem::absolute(renamed).string() << std::endl;
			else if (custom_verbose)
				std::cout << original.string() << "\t -> \t" << renamed.string() << std::endl;
		}

		// rename the file (if we not running dry)
		if (!dry_run)
		{
			try
			{
				std::filesystem::rename(original, renamed);
			}
			catch (const std::filesystem::__cxx11::filesystem_error &e)
			{
				std::cerr << "Error while renaming " << original << "\t -> \t" << renamed << std::endl;
				return 1;
			}
		}
	}

	return 0;
}

// Functions

// take a string and make it ASCII - two step process
std::string makeASCII(const std::string &input)
{
	UErrorCode status = U_ZERO_ERROR;

	// make a unicode string from input
	icu::UnicodeString uinput = icu::UnicodeString::fromUTF8(input);

	// Create a Normalizer2 object for Unicode canonical decomposition
	const icu::Normalizer2 *nfd_normalizer = icu::Normalizer2::getNFDInstance(status);

	// Normalize the string to the NFD form - should remove combining characters & stuff
	nfd_normalizer->normalize(uinput, status);

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
