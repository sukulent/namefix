# Namefix 
Fix bad filenames - tries to rename files so all characters are ASCII, replaces spaces with an underscore and periods with a hypen, replaces all other non-ASCII characters with a question mark and keeps extensions untouched.

## Usage
  ./namefix {OPTIONS} [input...]

    Namefix - fix bad filenames
    Tries to rename files so all characters are ASCII, keeps extensions
    untouched

  OPTIONS:

      -v, --verbose                     Verbose output
      -V, --verbose-long                Verbose output - absolute paths
      -r, --report                      Just report all files not conforming, -V
                                        for absolute paths
      -d, --dry-run                     Dry run, don't do anything, just print
                                        what would have been done (implies -v)
      -I, --ignore-errors               Do not stop when error is encountered
      -s[STR], --spaces=[STR]           Replace spaces with STR
      -S, --no-spaces                   Do not replace spaces - default
                                        replacement is '_'
      -p[STR], --periods=[STR]          Replace periods with STR
      -P, --no-periods                  Do not replace periods - default
                                        replacement is '-'
      -n[STR], --non-ASCII=[STR]        Replace remaining non-ASCII characters
                                        with STR - default replacement is '?'
      -c[NUM], --characters=[NUM]       The maximum length for the new filename
                                        (without extension!) Default 64
      -o[A/B], --override=[A/B]         Replace A with B before fixing filename
      -O[A/B],
      --exclusive-override=[A/B]        Only replace A with B, change nothing
                                        else
      -k[PATH], --keep-path=[PATH]      Do not rename, make a copy with the new
                                        name and put it in PATH
      -K, --keep                        Do not rename, make a copy with new name
      -t[STR], --to-ASCII=[STR]         Test a string - transforms a UTF-8
                                        string into ASCII and prints it, does
                                        not keep extensions
      --version                         Outputs the version
      -h, --help                        Display this help menu
      input...                          Input files
      "--" can be used to terminate flag options and force all following
      arguments to be treated as positional options


## How is it done?

Firstly, these flags are checked:
- If help flag is set, show help and exit.
- If version flag is set, show version and exit.
- If to-ascii flag is set, convert the provided string to ASCII and exit. Does not replace spaces or periods.

Than for each file provided:
- Fail if the file does not exist (if ignore-errors flag is not set)
- If any override flag is set, replace accordingly.
- If exclusive-override flag is set, skip this, otherwise:
	- convert to ASCII
	- resize if bigger than character flag
	- if no-spaces flag is not set, replace spaces
	- if no-periods flag is not set, replace periods
- The new path is constructed from original, new filename and original extension
- If report flag is set, print the file if new path differs from original, print absolute paths if --verbose-long flag is set, continue to next file.
- If any verbose flag is set, print info and if original path and new path are the same, continue to next file.
- If dry-run flag is set, skip this, otherwise:
	- If any keep flag is set, copy the file (if it fails, and ignore-errors not set, exit)
	- Else rename the file (if it fails, and ignore-errors not set, exit)
- If all files were processed
	- and ignore-errors flag is set and errors happend, print how many there were, otherwise
- Exit.


### ASCII Conversion
I am using the unicode library for the conversion to ASCII.
It is done in these steps:
- normalize into fully decomposed form
- normalize into fully composed form
- transliterate from Any into Latin
- transliterate again from Any into ASCII
- check for all remaining non-ASCII characters and replace them with '?'
- resize the name
- replace spaces
- replace periods


### Other

Using the [args] (https://github.com/Taywee/args) library for argument parsing.


### License

This program is lincesed under the GPL-3-or-later license.
