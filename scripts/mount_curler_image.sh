#!/bin/sh
rootdir=/opt/image_mounts/curler.rpi;
bootdir=$rootdir/curler.boot;
fsdir=$rootdir/curler.fs;
image=/home/lezh1k/PImages/RPI/rpi.2019.launchable.img;
mkdir -p $bootdir;
mkdir -p $fsdir;

umount $bootdir;
umount $fsdir;
mount -o loop,offset=$((98304*512)) $image $fsdir;
