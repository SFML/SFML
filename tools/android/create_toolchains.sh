#!/usr/bin/env bash

create_toolchain () {

    export SYSROOT=$NDK/platforms/android-$1/arch-$2/

    MAKE=$NDK/build/tools/make-standalone-toolchain.sh

    PLATFORM=--platform=android-$1
    DIR=--install-dir=toolchains/$2

	# Abort if already created
	if [ -d "$PWD/toolchains/$2" ]
	then
		return
	fi

    if [ "$2" = "arm" ]
    then
        TOOLCHAIN=--toolchain=arm-linux-androideabi-4.8
    elif [ "$2" = "x86" ]
    then
        TOOLCHAIN=--toolchain=x86-4.8
    elif [ "$2" = "mips" ]
    then
        TOOLCHAIN=--toolchain=mipsel-linux-android-4.8
    else
        echo "Abort."
        exit 1
    fi

    $MAKE $PLATFORM $TOOLCHAIN $DIR --stl=stlport

    # move linux/soundcard.h to sys/soundcard.h
    mv $PWD/toolchains/$2/sysroot/usr/include/linux/soundcard.h $PWD/toolchains/$2/sysroot/usr/include/sys
}

create_toolchain 9 arm
create_toolchain 9 x86
create_toolchain 9 mips
