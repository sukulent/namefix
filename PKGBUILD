# Maintainer: sukulent
pkgname='namefix'
pkgver="1.0.0"
pkgrel="1"
# epoch=
pkgdesc="Fix bad filenames"
arch=("x86_64")
url="https://github.com/sukulent/namefix"
license=('GPL-3-or-later')
# groups=()
# depends=()
makedepends=('make' 'binutils' 'gcc')
# checkdepends=()
# optdepends=()
# provides=()
# conflicts=()
# replaces=()
# backup=()
# options=()
# install=
# changelog=
source=("https://codeload.github.com/sukulent/namefix/tar.gz/refs/tags/v$pkgver")
# noextract=()
md5sums=('912b0a6378621905453d7588df1aaf24')
# validpgpkeys=()


build() {
	cd "$pkgname-$pkgver"
	make
}

package() {
	cd "$pkgname-$pkgver"
	install -Dm755 ./namefix "$pkgdir/usr/bin/namefix"
	install -Dm644 ./README.md "$pkgdir/usr/share/doc/$pkgname"
	# make install
}
