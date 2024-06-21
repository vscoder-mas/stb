#!/bin/bash

rm -rf build
mkdir build
cd build

IOS_PLATFORMDIR=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform
IOS_SYSROOT=($IOS_PLATFORMDIR/Developer/SDKs/iPhoneOS*.sdk)
export CFLAGS="-Wall -arch arm64 -miphoneos-version-min=8.0 -funwind-tables"

cat <<EOF >toolchain.cmake
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang)
EOF

cmake -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake \
  -DCMAKE_OSX_SYSROOT=${IOS_SYSROOT[0]} \
  ..

make


