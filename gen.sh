#!/bin/bash

set -eo pipefail

if  ! command -v nfpm 2>&1 1> /dev/null  ; then
	echo "Error: nfpm not installed"
	exit 1
fi

if  [[ ( "deb" != "$1"  &&   "rpm" != "$1" ) || -z $1 ]] ; then
	echo "Error: Specify target: rpm or deb"
	exit 1
fi

pkgver="1.1.0"
pkgname=namefix

if [[ -f "v$pkgver" || -f "namefix-$pkgver.tar.gz" ]] ; then
	echo "Using existing archives..."
else
	wget https://codeload.github.com/sukulent/namefix/tar.gz/refs/tags/v$pkgver
fi

if [ -f v$pkgver ] ; then
	mv v$pkgver namefix-$pkgver.tar.gz
fi

echo "36158febe2125598cc54b48caeaa1016e7fb92c4ef34bbe12b8e41da5d3374c8 namefix-$pkgver.tar.gz" > sha.txt

#if [[ $? -ne "0" ]] ; then

if [[ $( sha256sum -c sha.txt 2>&1 1> /dev/null ) ]] ; then
	echo "ERROR: sha256 missmatch"
	exit 1
fi

rm sha.txt

echo "Checksums OK, unpacking... "
tar -zxf "namefix-$pkgver.tar.gz"

mv "namefix-$pkgver" namefix-build

cd namefix-build

echo "Compiling..."
make

cd ..

echo "Generating archive..."

if [[ "deb" == "$1" ]] ; then
	nfpm pkg --packager deb
elif [[ "rpm" == "$1" ]] ; then
	nfpm pkg --packager rpm
else
	echo "Specify target: rpm or deb"
	exit 1
fi

rm -r namefix-build

exit 0

