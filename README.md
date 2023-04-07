# Namefix 
Fix bad filenames - tries to rename files so all characters are ASCII, replaces all other non-ASCII characters with a placeholder and keeps extensions untouched

### Usage
 ./namefix {OPTIONS} [input...]

    Namefix - fix bad filenames
    Tries to renames files so all characters are ASCII, keeps extensions
    untouched

  OPTIONS:

      -h, --help                        Disp+lay this help menu
      -v, --verbose                     Verbose output
      -V, --verbose-long                Verbose output - including absolute
                                        paths
      --version                         Outputs the version
      -d, --dry-run                     Dry run, don't do anything, just print
                                        what would have been done (implies -v)
      -t[STR], --to-ascii=[STR]         Test a string - transforms a UTF-8
                                        string into a ASCII char and prints it
      -c[NUM], --characters=[NUM]       The maximum length for the new filename
                                        (without extension!), keeps extension
      input...                          Input files
      "--" can be used to terminate flag options and force all following
      arguments to be treated as positional options


Using the usefull: https://github.com/Taywee/args
