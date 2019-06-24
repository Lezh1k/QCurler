#!/bin/bash
#This script is ugly, feel free to fix it

if [ "$#" -ne 2 ]; then
    echo "usage ./cmd target-rootfs path-to-target-toolchain-compiler"
    exit -1
fi

#passed args
ROOTFS=$1
TOOLCHAIN=arm-linux-gnueabihf

if [ -x $TOOLCHAIN ]; then
    echo "Passed valid toolchain"
    MACHINE=$($TOOLCHAIN -dumpmachine)
    echo "Machine is " $MACHINE
    DEB_MULTI_ARCH_MADNESS=$ROOTFS/usr/lib/$MACHINE
fi

INITIAL_DIR=$PWD
echo $PWD
echo $TOOLCHAIN

function adjustSymLinks
{
    echo "Adjusting the symlinks in $1 to be relative"
    cd $1
    find . -maxdepth 1 -type l | while read i;
#    do qualifies=$(file $i | sed -e "s/.*\`\(.*\)'/\1/g" | grep ^/lib)
#    find . -maxdepth 1 -lname / | while read i;
    do qualifies=$(readlink $i | sed -e "s/.*\`\(.*\)'/\1/g" | grep ^/lib)
#    echo `readlink $i` $i
    linkPath=`readlink $i`
    #echo ${linkPath:0:1}
    #if [ -n "$qualifies" ]; then
    if [[ "$linkPath" == /* ]]; then
#    newPath=$(file $i | sed -e "s/.*\`\(.*\)'/\1/g" | sed -e "s,\`,,g" | sed -e "s,',,g" | sed -e "s,^/lib,$2/lib,g");
    newPath="$2""$linkPath"
#    echo $linkPath
#    echo $i
#    echo $newPath
    sudo rm $i;
    sudo ln -s $newPath $i;
    echo $1/$i to $newPath
    fi
    done
    cd $INITIAL_DIR
}

adjustSymLinks $ROOTFS/usr/lib "../.."
adjustSymLinks $ROOTFS/usr/lib/$TOOLCHAIN "../../.."
adjustSymLinks $ROOTFS/lib ".."
adjustSymLinks $ROOTFS/lib/$TOOLCHAIN "../.."

echo "Testing for existence of potential debian multi-arch dir: $DEB_MULTI_ARCH_MADNESS"

if [ -n "$DEB_MULTI_ARCH_MADNESS" -a -e "$DEB_MULTI_ARCH_MADNESS" ]; then
    echo "Debian multiarch dir exists, adjusting"
    adjustSymLinks $DEB_MULTI_ARCH_MADNESS "../../.."
fi 
