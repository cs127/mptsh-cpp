#!/usr/bin/bash

! command -v g++ &>/dev/null && echo "GCC/G++ is not installed." && exit

cd "$(dirname "$0")"
mkdir -p ./build
g++ -lxcb ./mptsh.cpp -o ./build/mptsh
exit
