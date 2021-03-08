# How to build Qt 5.6.3 static on Windows

## Download Qt 5.6.3 installer and sources

Download [Qt 5.6.3 installer][qt-exe] and [Qt 5.6.3 sources][qt-src].

## Install Qt 5.6.3 from the exe file

* Skip Qt account page ;
* Choose installation directory: `C:\Qt\Qt5.6.3` ;
* Choose components: **Qt 5.6.3/MinGW 4.9.2 32 bit** and **Tools/MinGW 4.9.2**.

Click **next** to install.

## Decompress the Qt sources

Extract `qt-everywhere-opensource-src-5.6.3.zip` into `C:\Qt\`.

## Open Qt 5.6.3 terminal

Open the Qt 5.6.3 terminal from **Start menu ➡️️ Qt 5.6.3 ➡️️ 5.6.3 ➡️️ MinGW 4.9.2 (32-bit)**.

```bash
cd c:\Qt\qt-everywhere-opensource-src-5.6.3
```

## Configure the build, we skip unwanted components

First, edit the file `C:\Qt\qt-everywhere-opensource-src-5.6.3\qtbase\mkspecs\win32-g++\qmake.conf`.

Add this at the end of the file to enable static build.

```
# [QT-STATIC-PATCH]
QMAKE_LFLAGS += -static -static-libgcc
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -Os -momit-leaf-frame-pointer
DEFINES += QT_STATIC_BUILD
```

Then, start the configure process.

```bash
configure.bat -static -debug-and-release -platform win32-g++ -target xp -no-directwrite -prefix ^
    C:\Qt\Qt5.6.3-static -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype -no-opengl -no-openvg ^
    -no-angle -qt-sql-sqlite -opensource -confirm-license -make libs -nomake tools -nomake examples ^
    -nomake tests -skip qtwebengine -skip qtwebview -skip qtquickcontrols -skip qtquickcontrols2 ^
    -skip qtlocation -skip qtscript -skip qtserialbus -skip qtactiveqt -skip qtwayland -skip qtwebchannel ^
    -skip qtgraphicaleffects -skip qtwebsockets -skip qtsensors -skip qt3d -skip qtconnectivity ^
    -skip qtdeclarative -skip qtcanvas3d -skip qtserialport -skip qtmultimedia -skip qtenginio ^
    -skip qtlocation -skip qtandroidextras -skip qtx11extras -skip qtsvg -no-audio-backend ^
    -no-wmf-backend -libproxy -no-dbus -no-native-gestures -no-compile-examples ^
    -openssl-linked -I C:\MinGW\openssl-1.0.2l\include -L C:\MinGW\openssl-1.0.2l\lib
```

## Build and install

Edit the file `C:\Qt\qt-everywhere-opensource-src-5.6.3\qtbase\mkspecs\qmodule.pri` and change the line `OPENSSL_LIBS` with this:

```
OPENSSL_LIBS    = -lssl -lcrypto -lgdi32
```

Now, we can start the build.

```bash
mingw32-make -j4
mingw32-make install
```

## Clean sources

Remove the directory `C:\Qt\qt-everywhere-opensource-src-5.6.3`.

## Set the Qt static terminal

In the directory `C:\Qt\Qt5.6.3\5.6.3\mingw49_32\bin`, create a file named `static-qtenv2.bat`
and add the following content.

```bat
echo off
echo Setting up environment for Static Qt usage...
set PATH=C:\Qt\Qt5.6.3-static\bin;C:\Qt\Qt5.6.3\Tools\mingw492_32\bin;%PATH%
cd /D C:\Qt\Qt5.6.3\5.6.3\mingw49_32
```

Now, from the **Start menu ➡️️ Qt 5.6.3 ➡️️ 5.6.3 ➡️️ MinGW 4.9.2 (32-bit)**, duplicate **Qt 5.6.3 for Desktop (MinGW 4.9.2 32 bit)**
to **Qt 5.6.3 Static for Desktop (MinGW 4.9.2 32 bit)** and change the target to use **static-qtenv2.bat**.

[qt-exe]: https://download.qt.io/new_archive/qt/5.6/5.6.3/qt-opensource-windows-x86-mingw492-5.6.3.exe
[qt-src]: https://download.qt.io/new_archive/qt/5.6/5.6.3/single/qt-everywhere-opensource-src-5.6.3.zip

