#!/bin/bash

#rm -rf ../bin/release

if [[ ! -d ../bin/release ]]
then
  mkdir ../bin/release
fi

# ---------- linux ----------

#mkdir ../bin/release/linux
if [[ ! -d ../bin/release/linux ]]
then
  mkdir ../bin/release/linux
fi

./compile.sh -i ../source/skei.audio/$1.cpp -o ../bin/release/linux/$1.clap -f clap

mkdir ../bin/release/linux/$1.vst3
mkdir ../bin/release/linux/$1.vst3/Contents
mkdir ../bin/release/linux/$1.vst3/Contents/x86_64-linux

./compile.sh -i ../source/skei.audio/$1.cpp -o ../bin/release/linux/$1.vst3/Contents/x86_64-linux/$1.so -f vst3

# ---------- win32 ----------

#mkdir ../bin/release/win
if [[ ! -d ../bin/release/win ]]
then
  mkdir ../bin/release/win
fi

./compile_win32.sh -i ../source/skei.audio/$1.cpp -o ../bin/release/win/$1.clap -f clap

mkdir ../bin/release/win/$1.vst3
mkdir ../bin/release/win/$1.vst3/Contents
mkdir ../bin/release/win/$1.vst3/Contents/x86_64-win

./compile_win32.sh -i ../source/skei.audio/$1.cpp -o ../bin/release/win/$1.vst3/Contents/x86_64-win/$1.vst3 -f vst3

