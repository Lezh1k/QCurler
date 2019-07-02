#!/bin/bash
source ./cross_variables.sh
export PATH_TO_CC=/home/lezh1k/gcc-linaro-5.4.1-2017.05-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/bin
#export PATH_TO_CC=/opt/gcc-linaro-5.4.1-2017.05-x86_64_arm-linux-gnueabihf/bin
export PATH_TO_CC_TARGET=$PATH_TO_CC/arm-linux-gnueabihf-

export CC=${PATH_TO_CC_TARGET}gcc
export CXX=${PATH_TO_CC_TARGET}g++
export AR=${PATH_TO_CC_TARGET}ar
export AS=${PATH_TO_CC_TARGET}as
export LD=${PATH_TO_CC_TARGET}ld
export RANLIB=${PATH_TO_CC_TARGET}ranlib
export READELF=${PATH_TO_CC_TARGET}readelf
export STRIP=${PATH_TO_CC_TARGET}strip

export KOBUS_ROOT=/opt/image_mounts/curler.rpi/curler.fs
#export KOBUS_UBOOT=/media/rpi/boot

export PATH_TO_QT_SOURCES=/media/raistlin/diske/storage/distr/soft-src/qt-everywhere-src-5.12.4
export PATH_TO_QT_KOBUS=/opt/qt-5.12.4
