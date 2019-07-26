# How to build Qt 5.9.7 static on macOS

## Download Qt 5.9.7 installer and sources

```bash
mkdir $HOME/Qt && cd $HOME/Qt
curl -OL https://download.qt.io/official_releases/qt/5.9/5.9.7/qt-opensource-mac-x64-5.9.7.dmg
curl -OL https://download.qt.io/official_releases/qt/5.9/5.9.7/single/qt-everywhere-opensource-src-5.9.7.tar.xz
```

## Install Qt 5.9.7 from the DMG file

* Skip Qt account page ;
* Choose installation directory: `/Users/<yourlogin>/Qt/Qt5.9.7/` ;
* Choose components: **macOS** and **Qt Creator** ;

Click **next** to install.

## Add Qt to the PATH

In your shell configuration file, like `$HOME/.bash_profile`, add the following export.

```bash
export PATH="$HOME/Qt/Qt5.9.7/5.9.7/clang_64/bin/:$PATH"
```

Restart your shell, or source it with `. $HOME/.bash_profile`.

## Decompress the Qt sources

```bash
cd $HOME/Qt
tar xvf qt-everywhere-opensource-src-5.9.7.tar.xz
```

## Configure the build, we skip unwanted components

```bash
cd qt-everywhere-opensource-src-5.9.7/

./configure -static -debug-and-release -prefix $HOME/Qt/Qt5.9.7-static/ \
    -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype -no-opengl -no-openvg -opensource \
    -confirm-license -make libs -nomake tools -nomake examples -nomake tests -skip qtwebengine \
    -skip qtwebview -skip qtquickcontrols -skip qtquickcontrols2 -skip qtlocation -skip qtscript \
    -skip qtserialbus -skip qtactiveqt -skip qtwayland -skip qtwebchannel -skip qtgraphicaleffects \
    -skip qtwebsockets -skip qtsensors -skip qt3d -skip qtconnectivity -skip qtdeclarative \
    -skip qtcanvas3d -skip qtserialport -skip qtmultimedia -skip qtlocation -skip qtandroidextras \
    -skip qtx11extras -skip qtsvg -no-dbus -no-compile-examples
```

## Build and install

```bash
make -j4
make install
```

## Clean sources

```bash
cd ..
rm -rfv qt-everywhere-opensource-src-5.9.7/
```

## Set the Qt static PATH

In your shell configuration file, like `$HOME/.bash_profile`, replace the previous export with this one.

```bash
export PATH="$HOME/Qt/Qt5.9.7-static/bin/:$PATH"
```

Restart your shell, or source it with `. $HOME/.bash_profile`.
