#!/usr/bin/env bash

FLAC_VERSION=1.2.1
VORBIS_VERSION=1.3.3
OGG_VERSION=1.3.2
OPUS_VERSION=1.1.2
OPUSFILE_VERSION=0.7

FLAC=flac-$FLAC_VERSION
VORBIS=libvorbis-$VORBIS_VERSION
OGG=libogg-$OGG_VERSION
OPUS=opus-$OPUS_VERSION
OPUSFILE=opusfile-$OPUSFILE_VERSION

SNDFILE_VERSION=1.0.25
SNDFILE=libsndfile-$SNDFILE_VERSION

FREETYPE_VERSION=2.4.0
FREETYPE=freetype-$FREETYPE_VERSION

mkdir build

wget -nc -P src http://downloads.xiph.org/releases/flac/$FLAC.tar.gz
if [ ! -d "$PWD/tmp/$FLAC" ]
then
    tar -C build -xf src/$FLAC.tar.gz
fi

wget -nc -P src http://downloads.xiph.org/releases/vorbis/$VORBIS.tar.gz
if [ ! -d "$PWD/tmp/$VORBIS" ]
then
    tar -C build -xf src/$VORBIS.tar.gz
fi

wget -nc -P src http://downloads.xiph.org/releases/ogg/$OGG.tar.gz
if [ ! -d "$PWD/tmp/$OGG" ]
then
    tar -C build -xf src/$OGG.tar.gz
fi

wget -nc -P src http://downloads.xiph.org/releases/opus/$OPUS.tar.gz
if [ ! -d "$PWD/tmp/$OPUS" ]
then
    tar -C build -xf src/$OPUS.tar.gz
fi

wget -nc -P src http://downloads.xiph.org/releases/opus/$OPUSFILE.tar.gz
if [ ! -d "$PWD/tmp/$OPUSFILE" ]
then
    tar -C build -xf src/$OPUSFILE.tar.gz
fi

wget -nc -P src http://download.savannah.gnu.org/releases/freetype/$FREETYPE.tar.gz
if [ ! -d "$PWD/tmp/$FREETYPE" ]
then
    tar -C build -xf src/$FREETYPE.tar.gz
fi

wget -nc -P src https://github.com/AerialX/openal-soft-android/archive/master.tar.gz
if [ ! -d "$PWD/tmp/openal-soft-android-master" ]
then
    tar -C build -xf src/master.tar.gz
fi

patch build/openal-soft-android-master/CMakeLists.txt patches/remove-so-version-suffix.diff
