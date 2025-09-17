# Changelog

All notable changes to this project will be documented in this file.

### 1.3.0
- add smart fixes and corresponding --no-smart flag
- fix wrong version
- change default non ASCII replacement from ? to _

### [1.2.0] - 2024-05-12

##### Added
- -F, --force flag for enabling overwriting of existing files
- manual page

### [1.1.0] - 2024-01-23

##### Added
- -D, --no-directory flag for ignoring, shorten error output
- -Y, --no-symlinks flag for ignoring symlinks
- override flags now accept empty string as replacement

##### Fixed
- renaming & copying directories,
- renaming & copying symlinks,
- forgotten internal version number


### [1.0.1] - 2023-11-09
- Add target which doesn't strip the exectuable
- Move PKGBUILD to separate branch

## [1.0.0] - 2023-04-20

### Initial release

[1.2.0]: https://github.com/sukulent/namefix/releases/tag/v1.2.0
[1.1.0]: https://github.com/sukulent/namefix/releases/tag/v1.1.0
[1.0.1]: https://github.com/sukulent/namefix/releases/tag/v1.0.1
[1.0.0]: https://github.com/sukulent/namefix/releases/tag/v1.0.0
