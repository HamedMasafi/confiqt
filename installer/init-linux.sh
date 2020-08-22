#!/bin/sh

QT_PATH=$1
OUTPUT_NAME=$2
SRC=$(pwd)
PLATFORM=linux

DST="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
DST_PLATFORM="$DST/$PLATFORM"
DST_DATA="$DST_PLATFORM/packages/ir.tooska.confiqt/data/"
BINARY_CREATOR=$(ls $QT_PATH/Tools/QtInstallerFramework/*/bin/binarycreator | head -1)

mkdir $DST_PLATFORM
cp $DST/template/* $DST_PLATFORM -R

echo "Installing from $SRC to $DST"

cp $SRC/installer/bin/QtConfig $DST_DATA
cp $DST/QtConfig.sh $DST_DATA

ldd $SRC/QtConfig | grep -i qt  | egrep -o '/.*\.so.* ' | sed -e 's/\s//g' | tr \\n \\0 | xargs -0 -I{} cp "{}" $DST_DATA

mkdir -p $DST_PLATFORM/output

$BINARY_CREATOR -c $DST_PLATFORM/config.xml -p $DST_PLATFORM/packages $DST_PLATFORM/output/qtconfig-linux
