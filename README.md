# Namefix 
Fix bad filenames - tries to rename files so all characters are ASCII, replaces all other non-ASCII characters with a placeholder and keeps extensions untouched

### Usage
  ./namefix {OPTIONS} [input...]

    Namefix - fix bad filenames
    Tries to renames files so all characters are ASCII, keeps extensions
    untouched

  OPTIONS:

      -v, --verbose                     Verbose output
      -V, --verbose-long                Verbose output - including absolute
                                        paths
      --dry-run                         Dry run, don't do anything, just print
                                        what would have been done (implies -v)
      -s[STR], --spaces=[STR]           Replace spaces with STR
      -S, --no-spaces                   Do not replace spaces - default
                                        replacement is a underscore
      -d[STR], --dots=[STR]             Replace dots with STR
      -D, --no-dots                     Do not replace dots - default
                                        replacement is a hyphen/dash
      -c[NUM], --characters=[NUM]       The maximum length for the new filename
                                        (without extension!)
      -t[STR], --to-ascii=[STR]         Test a string - transforms a UTF-8
                                        string into a ASCII characters and
                                        prints it, does not keep extensions
      --version                         Outputs the version
      -h, --help                        Display this help menu
      input...                          Input files
      "--" can be used to terminate flag options and force all following
      arguments to be treated as positional options


Using the usefull: https://github.com/Taywee/args
