#!/bin/bash

# ---------- linux ----------

mkdir linux

./compile.sh -i ../source/skei.audio/$1.cpp -o linux/$1.clap -f clap

mkdir linux/$1.vst3
mkdir linux/$1.vst3/Contents
mkdir linux/$1.vst3/Contents/x86_64-linux

./compile.sh -i ../source/skei.audio/$1.cpp -o linux/$1.vst3/Contents/x86_64-linux/$1.so -f vst3

# ---------- win32 ----------

mkdir win

./compile_win32.sh -i ../source/skei.audio/$1.cpp -o win/$1.clap -f clap

mkdir win/$1.vst3
mkdir win/$1.vst3/Contents
mkdir win/$1.vst3/Contents/x86_64-win

./compile_win32.sh -i ../source/skei.audio/$1.cpp -o win/$1.vst3/Contents/x86_64-win/$1.vst3 -f vst3

