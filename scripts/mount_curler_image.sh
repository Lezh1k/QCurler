#!/bin/sh
rootdir=/opt/image_mounts/curler.rpi;
bootdir=$rootdir/curler.boot;
fsdir=$rootdir/curler.fs;
image=/media/raistlin/diske/storage/images/rpi.2019.curler.img;
#mkdir -p $bootdir;
mkdir -p $fsdir;

umount $bootdir;
umount $fsdir;
#sleep 1;
#losetup -d /dev/loop0;
#losetup -d /dev/loop1;
#sleep 1;
#losetup /dev/loop0 $image -o $((8192*512));
#losetup /dev/loop1 $image -o $((98304*512));
#sleep 1;
#mount /dev/loop0 -o loop $bootdir;
#mount /dev/loop1 -o loop,rw $fsdir;
mount -o loop,offset=$((98304*512)) $image $fsdir;
#sleep 1;
