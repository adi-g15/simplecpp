# Maintainer: Aditya Gupta <ag15035@gmail.com>, Abhiram Ranade <ranade@iitb.ac.in>
pkgname="simplecpp-git"
pkgdesc="A simple 2D graphics library in C++"
pkgver=r12.e42a679
pkgrel=1
arch=('x86_64')
url="https://github.com/adi-g15/simplecpp"
license=('CC')
depends=('libx11' 'gcc')
makedepends=('git')
provides=("s++")
source=('simplecpp::git://github.com/adi-g15/simplecpp.git')
md5sums=('SKIP')

pkgver() {
	cd "$srcdir/${pkgname%-git}"

	printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

#prepare() {
#	cd "$srcdir/${pkgname%-git}"
#	patch -p1 -i "$srcdir/${pkgname%-git}.patch"
#}

build() {
	cd "$srcdir/${pkgname%-git}"
	# ./autogen.sh
	# ./configure --prefix=/usr
	make -j$(nproc)
}

package() {
	cd "$srcdir/${pkgname%-git}"
	make install
}
