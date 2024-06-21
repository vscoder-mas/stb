#!/bin/bash

rm -rf build
mkdir build
cd build

TOOLCHAIN=clang
ANDROID_VERSION=21
JAVA_INCLUDE_PATH2=/usr/local/openjdk-16/include/linux/
cmake -G "Unix Makefiles" \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_ARM_MODE=arm \
      -DANDROID_PLATFORM=android-${ANDROID_VERSION} \
      -DANDROID_TOOLCHAIN=${TOOLCHAIN} \
      -DCMAKE_ASM_FLAGS="--target=aarch64-linux-android${ANDROID_VERSION}" \
      -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
      ..
make

