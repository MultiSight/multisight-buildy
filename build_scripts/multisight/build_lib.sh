#!/bin/bash

# ./build_lib.sh <name> <src> <path>

echo "Building: $1"

pushd $3

if [ -d "build" ]; then
    rm -rf build
fi

mkdir build

pushd build

cmake .. -DADDITIONAL_RELATIVE_RPATH=../GatewayLibs/libs

make

make install

popd

popd
