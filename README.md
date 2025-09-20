## DEB-RPM

This is a special branch for namefix deb and rpm packaging setup.

You need `git` (or you could just download this branch), `make`, `g++`, `wget`, `sha256sum`, `libicu` and it's headers.
Additionally `ar`, `tar`, if using the `nfpm` argument, or have `nfpm` installed.

[Nfpm](https://github.com/goreleaser/nfpm) is a simple packager. You can use
'nfpm' as second argument to download `.deb` and use extracted nfpm binary
instead of system nfpm.

Clone the namefix repo and `git switch deb-rpm` to this branch.

Use `./gen.sh deb` for deb package\
and `./gen.sh rpm` for rpm package\
It will download the source archive, compile and generate the package accordingly.

If it fails, you'll need to delete the namefix-build manually before
running it again, after you install the missing prerequisites.

Header files are in package:
`libicu-dev` on *debian*
`libicu-devel` on *fedora*

For debian, the script will set dependency to the currently installed `libicuXX`
where `XX` is the version. (e.g. `libicu76` for trixie)

e.g.:
`./gen.sh deb nfpm`
`./gen.sh rpm nfpm`

