#!/bin/bash

# fail whenever
set -eo pipefail

echo "Starting namefix package gen"

# check if we have nfpm
if  ! command -v nfpm 2>&1 1> /dev/null  ; then
	if [[ "nfpm" != "$2" ]] ; then
		echo "Error: nfpm not installed, try passing "nfpm" as second argument"
		exit 1
	else
		echo "will get nfpm binary..."
	fi
fi

# check if we know what we should do
if  [[ ( "deb" != "$1"  &&   "rpm" != "$1" ) || -z "$1" ]] ; then
	echo "error: specify target: rpm or deb"
	exit 1
fi

pkgver="1.2.0"
pkgname=namefix

# get namefix archive if needed
if [[ -f "v$pkgver" || -f "namefix-$pkgver.tar.gz" ]] ; then
	echo "using existing namefix archive..."
else
	wget "https://codeload.github.com/sukulent/namefix/tar.gz/refs/tags/v$pkgver"
fi

nfpm_deb="nfpm_2.37.1_amd64.deb"

# get nfpm
if [[ "nfpm" == "$2" ]] ; then
	if [[ -f "$nfpm_deb" ]] ; then
		echo "using existing nfpm archive..."
	else
		wget "https://github.com/goreleaser/nfpm/releases/download/v2.37.1/nfpm_2.37.1_amd64.deb"
	fi

	# prepare checksum
	echo "dc43d829cf3b1933be9136ae610422a4495e6fa495da005b0b07f7e99a8af0d6 $nfpm_deb" > sha.txt

	# check
	if [[ $( sha256sum -c sha.txt 2>&1 1> /dev/null ) ]] ; then
		echo "error: sha256 missmatch"
		exit 1
	fi
	ar x "$nfpm_deb" data.tar.gz
	tar -zxf data.tar.gz ./usr/bin/nfpm
	mv usr/bin/nfpm nfpm
	rm -r data.tar.gz usr
fi


#rename
if [[ -f "v$pkgver" ]] ; then
	mv "v$pkgver" "namefix-$pkgver.tar.gz"
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

if [[ "nfpm" == "$2" ]] ; then
	nfpm_bin=./nfpm
else
	nfpm_bin=nfpm
fi

if [[ "deb" == "$1" ]] ; then
	$nfpm_bin pkg --packager deb
elif [[ "rpm" == "$1" ]] ; then
	$nfpm_bin pkg --packager rpm
else # this should never happen, checked at the beggining
	exit 1
fi

# cleanup
rm -r namefix-build

exit 0

