#!/bin/bash

# fail whenever
set -eo pipefail

# check if we have nfpm
if  ! command -v nfpm 2>&1 1> /dev/null  ; then
	echo "Error: nfpm not installed"
	exit 1
fi

# check if we know what we should do
if  [[ ( "deb" != "$1"  &&   "rpm" != "$1" ) || -z $1 ]] ; then
	echo "Error: Specify target: rpm or deb"
	exit 1
fi

pkgver="1.2.0"
pkgname=namefix

# don't download if not needed
if [[ -f "v$pkgver" || -f "namefix-$pkgver.tar.gz" ]] ; then
	echo "Using existing archives..."
else
	wget https://codeload.github.com/sukulent/namefix/tar.gz/refs/tags/v$pkgver
fi

#rename
if [ -f v$pkgver ] ; then
	mv v$pkgver namefix-$pkgver.tar.gz
fi

# prepare checksum
echo "8f72618045340dd92948b91d4e6cb8e2bd6f4ee07e739f37b12b56f1079bd5b7 namefix-$pkgver.tar.gz" > sha.txt

# check
if [[ $( sha256sum -c sha.txt 2>&1 1> /dev/null ) ]] ; then
	echo "ERROR: sha256 missmatch"
	exit 1
fi

# remove unneeded
rm sha.txt

# prepare for build
echo "Checksums OK, unpacking... "
tar -zxf "namefix-$pkgver.tar.gz"

mv "namefix-$pkgver" namefix-build

cd namefix-build

# build
echo "Compiling..."
make

cd ..

echo "Generating archive..."

# run nfmp accordingly
if [[ "deb" == "$1" ]] ; then
	nfpm pkg --packager deb
elif [[ "rpm" == "$1" ]] ; then
	nfpm pkg --packager rpm
else # this should never happen, checked at the beggining
	exit 1
fi

# cleanup
rm -r namefix-build

exit 0

