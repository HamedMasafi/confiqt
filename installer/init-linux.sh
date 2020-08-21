#!/bin/sh

SRC=$(pwd)
PLATFORM=linux
DST="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
DST_PLATFORM="$DST/$PLATFORM"
DST_DATA="$DST_PLATFORM/packages/ir.tooska.confiqt/data/"

mkdir $DST_PLATFORM
cp $DST/template/* $DST_PLATFORM -R

echo "Installing from $SRC to $DST"

cp $SRC/QtConfig $DST_DATA
cp $DST/QtConfig.sh $DST_DATA

ldd $SRC/QtConfig | grep -i qt  | egrep -o '/.*\.so.* ' | sed -e 's/\s//g' | tr \\n \\0 | xargs -0 -I{} cp "{}" $DST_DATA

~/Qt/Tools/QtInstallerFramework/3.2/bin/binarycreator -c $DST_PLATFORM/config.xml -p $DST_PLATFORM/packages $DST_PLATFORM/qtconfig-linux-64
