# How to build libarchive-3.3.3 on macOS 10.10 minimum

## Download libarchive source code from official repository.

```bash
curl -OL https://github.com/libarchive/libarchive/releases/download/v3.3.3/libarchive-3.3.3.zip
```

## Unzip the archive and enter the directory.

```bash
unzip libarchive-3.3.3.zip
cd libarchive-3.3.3/
```

## Set macOS deployment target: 10.10.

```bash
export MACOSX_DEPLOYMENT_TARGET=10.10
```

## Configure the build, with the minimum required to unzip.

```bash
./configure --without-bz2lib --without-iconv --without-lz4 --without-lzma \
    --with-lzo2 --without-nettle --without-openssl --without-xml2 --without-expat  \
    --disable-shared --enable-static --disable-bsdtar --disable-bsdcat \
    --disable-bsdcpio --disable-rpath --disable-xattr --disable-acl  \
    --prefix=${PWD}/out
```

## Build and install. Define `MAC_OS_X_VERSION_MIN_REQUIRED` to run on 10.10.

```bash
make -j4 MAC_OS_X_VERSION_MIN_REQUIRED=101000
make install
```

## Get the files.

* Headers are in `out/include/` directory.
* Static library is in `out/lib/` directory.
