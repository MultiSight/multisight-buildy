#!/bin/bash

# ./build_lib.sh <name> <src> <path>

echo "Building: $1"

pushd $3

if [ -d "build" ]; then
    rm -rf build
fi

mkdir build

pushd build

cmake .. -G"Visual Studio 12 Win64"

cmake --build .

cmake --build . --target install

popd

popd
