#!/bin/bash
source ./build_variables.sh
make clean -j4
BUILDPATH=/home/raistlin/projects/rpi.curler/qt.shadow.build
rm -Rf $BUILDPATH 
mkdir $BUILDPATH
cd $BUILDPATH
#$PATH_TO_QT_SOURCES/configure  -debug -qt-zlib -qt-libjpeg -qt-libpng -system-xcb -opengl es2 \
#					-device linux-rasp-pi3-g++ -device-option CROSS_COMPILE=$PATH_TO_CC_TARGET  \
#					-sysroot $KOBUS_ROOT -opensource -confirm-license -optimized-qmake \
#					-make libs -prefix $PATH_TO_QT_KOBUS -no-pch -nomake examples -nomake tests \
#					-no-xcb -eglfs -evdev -v -skip webengine -no-webkit -no-use-gold-linker
					
$PATH_TO_QT_SOURCES/configure  -debug -qt-zlib -qt-libjpeg -qt-libpng -system-xcb -opengl es2 \
					-device linux-rasp-pi3-g++ -device-option CROSS_COMPILE=$PATH_TO_CC_TARGET  \
					-sysroot $KOBUS_ROOT -opensource -confirm-license -optimized-qmake \
					-make libs -prefix $PATH_TO_QT_KOBUS -no-pch -nomake examples -nomake tests \
					-no-xcb -eglfs -evdev -v -skip webengine -no-use-gold-linker
					
make -j4
#sudo make install
