#!/bin/bash
source ./cross_variables.sh
export PATH_TO_CC_TARGET=arm-linux-gnueabihf-

export CC=${PATH_TO_CC_TARGET}gcc
export CXX=${PATH_TO_CC_TARGET}g++
export AR=${PATH_TO_CC_TARGET}ar
export AS=${PATH_TO_CC_TARGET}as
export LD=${PATH_TO_CC_TARGET}ld
export RANLIB=${PATH_TO_CC_TARGET}ranlib
export READELF=${PATH_TO_CC_TARGET}readelf
export STRIP=${PATH_TO_CC_TARGET}strip

export KOBUS_ROOT=/opt/image_mounts/curler.rpi/curler.fs
export PATH_TO_QT_SOURCES=/home/lezh1k/SRC/kumtor/qt5
export PATH_TO_QT_KOBUS=/opt/qt-5.12.4
