Pour MacOS, récupération des sources de libarchive 3.3.1 et build via les
commandes :
export MACOSX_DEPLOYMENT_TARGET=10.10
./configure --without-bz2lib --without-iconv --without-lz4 --without-lzma \
--with-lzo2 --without-nettle --without-openssl --without-xml2 --without-expat \
--disable-shared --enable-static --disable-bsdtar --disable-bsdcat \
--disable-bsdcpio --disable-rpath --disable-xattr --disable-acl \
--prefix=${PWD}/out
make
make install

Pour Windows, les binaires ont été récupérés sur gnuw32 / ezwinports.
libarchive : 3.3.1
liblz4 : 1.7.5
libnettle : 3.3
libbz2 : 1.0.6
liblzma : 5.0.3
libxml2 : 2.7.8
libz : 1.2.3
liblzo : 1.08
