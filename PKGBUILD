# Maintainer: Aditya Gupta <ag15035@gmail.com>, Abhiram Ranade <ranade@iitb.ac.in>
pkgname="simplecpp-git"
pkgdesc="A simple 2D graphics library in C++"
pkgver=r14.1aec18c
pkgrel=1
arch=('x86_64')
url="https://github.com/adi-g15/simplecpp"
license=('CC')
depends=('libx11' 'gcc')
makedepends=('git')
provides=("s++")
conflicts=("simplecpp")
#source=('git://github.com/adi-g15/simplecpp.git#branch=dev')
source=('git://github.com/adi-g15/simplecpp.git')
md5sums=('SKIP')

pkgver() {
	cd "$pkgname"

	printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

#prepare() {
#	cd "$srcdir/${pkgname%-git}"
#	patch -p1 -i "$srcdir/${pkgname%-git}.patch"
#}

build() {
	cd "$pkgname"
	make -j$(nproc)
}

package() {
	cd "$pkgname"
	mkdir -p "$pkgdir/usr/include/simplecpp/include" \
			   "$pkgdir/usr/lib/simplecpp/"
	install -Dm 644 ${HDRS} "$pkgdir/usr/include/simplecpp/include/"
	install -Dm 644 lib/libsprite.a "$pkgdir/usr/lib/simplecpp/libsprite.a"
	install -Dm 755 s++ "$pkgdir/usr/bin/s++"
}
