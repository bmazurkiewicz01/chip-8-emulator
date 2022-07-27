#!/bin/bash

FILE=./chip_8_emulator

rm -rf build
mkdir build && cd build
cmake .. && make
if test -f "$FILE"; then
   echo "CHIP-8 emulator compiled successfully"
   echo -e "\n"
   $FILE
   echo -e "\n"
fi
cd ..
