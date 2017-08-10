# Contributor: Navet56 <evandib@gmail.com> 
# Maintainer: Navet56 <evandib@gmail.com> 

pkgname=opmon
pkgver=0.12.1
pkgrel=1 
license=('GPL') 
arch=('x86_64') 
pkgdesc="A open source RPG monster fighting game" 
url="http://opmon-game.ga" 
depends=('sfml' 'gcc' 'libstdc++5' 'smpeg') 
makedepends=('make') 
source=(https://github.com/jlppc/OpMon/raw/master/opmon-arch.tar.gz) 
md5sums=('f69cef4b0d1b39b4b59f2865f8d4d603') 

build() { 
cd $srcdir/opmon-arch

make || return 1 
 
}

package() 
{

mkdir $pkgdir/usr
mkdir $pkgdir/usr/share
mkdir $pkgdir/usr/share/OpMon

cp $srcdir/opmon-arch/arch_install/bin/usr/bin $pkgdir/usr/
cp -r $srcdir/opmon-arch/arch_install/bin/usr/share/OpMon $pkgdir/usr/share/OpMon
cp -r $srcdir/opmon-arch/arch_install/bin/usr/share/applications $pkgdir/usr/share/applications
cp -r $srcdir/opmon-arch/Makefile $pkgdir/

cd "$srcdir/opmon-arch"
make DESTIDIR="$pkgdir"

chmod +x $pkgdir/usr/bin/opmon
chmod 777 $pkgdir/usr/share/OpMon
chmod 777 $pkgdir/usr/bin
}
