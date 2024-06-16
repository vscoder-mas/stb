#!/bin/bash

rm -rf build
mkdir build
cd build


# Define the iOS SDK version
IOS_SDK_VERSION=$(xcrun --sdk iphoneos --show-sdk-version)

# Define the architectures to build for
ARCHS="arm64"

# Define the path to the libpng source code
LIBPNG_PATH="/Users/admin/Documents/work/swap/libpng"

# Loop over each architecture
for ARCH in ${ARCHS}; do
    # Define the host and compiler settings for the current architecture
    if [[ "${ARCH}" == "i386" || "${ARCH}" == "x86_64" ]]; then
        HOST="${ARCH}-apple-darwin"
        COMPILER="clang -arch ${ARCH} -mios-simulator-version-min=${IOS_SDK_VERSION}"
    else
        HOST="arm-apple-darwin"
        COMPILER="clang -arch ${ARCH} -mios-version-min=${IOS_SDK_VERSION}"
    fi

    # Configure the build process
    ../configure --prefix=/Users/admin/Documents/work/swap/libpng/build --host=${HOST} CC="${COMPILER}" \
        CPP="${COMPILER} -E" CFLAGS="-isysroot $(xcrun --sdk iphoneos --show-sdk-path)"

    # Build the library
    make clean
    make
    # make install

    # # Copy the built library to a separate directory
    # cp /usr/local/lib/libpng.a libpng-${ARCH}.a
done

# Combine the libraries for each architecture into a single library
lipo -create libpng-*.a -output libpng.a


