#!/usr/bin/bash

! command -v cmake &>/dev/null && echo "CMake is not installed." && exit

cd "$(dirname "$0")"
mkdir -p ./build
cd ./build
cmake ..
cmake --build .
exit
