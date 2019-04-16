# How to build OpenSSL in static mode on Windows

This document is based on this [link][reflink].

## Install Active Perl

Download [ActivePerl-5.24.2.2403-MSWin32-x86-64int-403863.exe][perl].
Install it.

## Install MinGW

Download [mingw-get-setup.exe][mingw].
Run it. Use default options.

Choose components:
* mingw-developer-toolkit
* mingw32-base
* mingw32-gcc-g++
* msys-base

Then, click on Installation ➡️ Apply Changes ➡️ Apply.

## Downgrade w32api version, else compilation will not work.

Open a terminal.

```bat
cd \MinGW\bin
mingw-get.exe upgrade "w32api<5.0.2"
```

## Download OpenSSL

Get [openssl-1.0.2l.tar.gz][openssl] and save it into `C:\MinGW`.

## Check environment

Start MSYS prompt: click on `C:\MinGW\msys\1.0\msys.bat`.

Check that the following commands are working.

* `perl -v`
* `gcc -v`
* `make -v`
* `tar --version`

## Decompress OpenSSL

Decompress the file `openssl-1.0.2l.tar.gz`.

```bat
cd /mingw
tar xvf openssl-1.0.2l.tar.gz
cd openssl-1.0.2l
```

## Build OpenSSL in static mode

```bat
perl Configure mingw no-shared
make
```

## Get the library

Copy generated a files into lib.

```bat
mkdir lib
cp -v libssl.a libcrypto.a lib/
```

[reflink]: http://www.stringcat.com/company_blog/2015/12/07/compiling-openssl-on-windows-mingw32/
[perl]: http://downloads.activestate.com/ActivePerl/releases/5.24.2.2403/ActivePerl-5.24.2.2403-MSWin32-x86-64int-403863.exe
[mingw]: https://sourceforge.net/projects/mingw/files/Installer/mingw-get-setup.exe/download
[openssl]: https://ftp.openssl.org/source/old/1.0.2/openssl-1.0.2l.tar.gz
