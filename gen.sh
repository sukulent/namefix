#!/bin/bash

# fail whenever
set -eo pipefail

pkgver="1.3.0"
pkgname=namefix

echo "Starting $pkgname package gen"

# check if we have nfpm
if ! command -v nfpm 1>/dev/null 2>&1 ; then
	if [[ "nfpm" != "$2" ]] ; then
		echo "Error: nfpm not installed, try passing 'nfpm' as second argument"
		exit 1
	else
		echo "will get nfpm binary..."
	fi
fi

# check if we know what we should do
if [[ "deb" != "$1"  &&   "rpm" != "$1" ]] ; then
	echo "Error: specify target: rpm or deb"
	exit 1
fi

# get archive if needed
if [[ -f "v$pkgver" || -f "$pkgname-$pkgver.tar.gz" ]] ; then
	echo "using existing $pkgname archive..."
else
	wget "https://codeload.github.com/sukulent/$pkgname/tar.gz/refs/tags/v$pkgver"
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
	if ! sha256sum -c sha.txt 1> /dev/null 2>&1 ; then
		echo "Error: sha256 missmatch on nfpm"
		exit 1
	fi
	ar x "$nfpm_deb" data.tar.gz
	tar -zxf data.tar.gz ./usr/bin/nfpm
	mv usr/bin/nfpm nfpm
	rm -r data.tar.gz usr
fi


# rename
if [[ -f "v$pkgver" ]] ; then
	mv "v$pkgver" "$pkgname-$pkgver.tar.gz"
fi

# prepare checksum
echo "172ae75d01dc32e1376767b6b942a51d3154ae182f93e770e9c45d4d0a4712c1 $pkgname-$pkgver.tar.gz" > sha.txt

# check
if ! sha256sum -c sha.txt 1>/dev/null 2>&1 ; then
	echo "Error: sha256 missmatch on $pkgname archive"
	exit 1
fi

# remove unneeded
rm sha.txt

# prepare for build
echo "Checksums OK, unpacking... "
tar -zxf "$pkgname-$pkgver.tar.gz"

mv "$pkgname-$pkgver" $pkgname-build

cd $pkgname-build

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
	# replace libicu version with the one installed
	if ! grep "libicu[0-9][0-9]" "nfpm.yaml" > /dev/null 2>&1; then
		LIBICU_PKG=$(dpkg --list "libicu*" | grep "libicu[0-9][0-9]" | sed "s/\(ii  \)\(.*\)\(\:.*$\)/\2/")
		sed -i -e "s/libicu??/$LIBICU_PKG/" "nfpm.yaml"
	fi
	$nfpm_bin pkg --packager deb
elif [[ "rpm" == "$1" ]] ; then
	$nfpm_bin pkg --packager rpm
fi

# cleanup
rm -r "$pkgname-build"

exit 0

