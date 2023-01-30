#!/bin/bash

# --------------------------------------------------
# default values
# --------------------------------------------------

#ARCH=64
#DEBUG="off"
#GPL="off"
#FORMAT="vst3"
#GUI="nogui"
#PAINTER="nopainter"
#FLAGS=""
#LIBS=""

FLAGS="-Wall -Wno-unknown-pragmas -std=gnu++17 -shared -fPIC -Wl,--as-needed -pthread -O3 -ffast-math -s -static-libstdc++ -static-libgcc "
DEF=""
INC="-I../include -I../include/extern/vst3/vst3sdk "
LIB="-lX11 -lX11-xcb -lxcb -lxcb-util -lxcb-image -lxcb-cursor -lxkbcommon -lxcb-keysyms -lGL -lGLU -lGLX -ldl -lrt "
#POST="-s -lstdc++ -lm "

FLAGS+="-DMIP_NO_DEFAULT_WARNINGS "

# --------------------------------------------------
# help
# --------------------------------------------------

function print_help {
  echo "usage: ./compile <flags>"
  echo "  -i input file name/path"
  echo "  -o output file name/path"
  echo "  -f format (CLAP, vst2, vst3, ladspa, dssi, lv2, exe)"
  echo "  -g gui (NOGUI,xcb,win32)"
  echo "  -p painter (NOPAINTER,xcb,cairo,opengl,nanovg)"
#  echo "  -a architecture (32/64)"
#  echo "  -v version (v1)"
  echo "  -d debug"
  echo "  -h this help"
  echo "  -F additional gcc flags"
#  echo "  -G use gpl"
#  echo "  -D additional defines"
#  echo "  -I additional include directories"
#  echo "  -L additional linked libraries"
  exit 1
}

# --------------------------------------------------
# parse command line
# --------------------------------------------------

if [ "$#" -eq 0 ]; then
  print_help
fi

#while getopts i:o:t:g:a:v:dhF:GD:I:L: opt
while getopts i:o:f:g:p:a:dhG opt
do
  case "$opt" in
    i) INPUT="$OPTARG";;
    o) OUTPUT="$OPTARG";;
    f) FORMAT="$OPTARG";;
    g) GUI="$OPTARG";;
    p) PAINTER="$OPTARG";;
#   a) ARCH="$OPTARG";;
    d) DEBUG="on";;
    h) print_help;;
#   G) GPL="on";;
#   v) VERSION="$OPTARG";;
    F) FLAGS+="$OPTARG ";;
#   D) DEF+="-D$OPTARG ";;
#   I) INC+="-I$OPTARG ";;
#   L) LIB+="-l$OPTARG ";;
  esac
done

# --------------------------------------------------
# input, output
# --------------------------------------------------

INPUT_FILE=${INPUT##*/}
INPUT_BASE=${INPUT_FILE%%.*}
INPUT_EXT=${INPUT_FILE##*.}
INPUT_DIR=${INPUT%$INPUT_FILE}

#echo ""
#echo "INPUT     : $INPUT"
#echo "INPUT_FILE: $INPUT_FILE"
#echo "INPUT_BASE: $INPUT_BASE"
#echo "INPUT_EXT : $INPUT_EXT"
#echo "INPUT_DIR : $INPUT_DIR"

# input = ../plugins/fx_pitch.cpp"
# input_file = fx_pitch.cpp
# input_base = fx_pitch
# input_ext = cpp
# input_dir = ../plugins/

OUTPUT_FILE=${OUTPUT##*/}
OUTPUT_BASE=${OUTPUT_FILE%%.*}
OUTPUT_EXT=${OUTPUT_FILE##*.}
OUTPUT_DIR=${OUTPUT%$OUTPUT_FILE}

#echo ""
#echo "OUTPUT     : $OUTPUT"
#echo "OUTPUT_FILE: $OUTPUT_FILE"
#echo "OUTPUT_BASE: $OUTPUT_BASE"
#echo "OUTPUT_EXT : $OUTPUT_EXT"
#echo "OUTPUT_DIR : $OUTPUT_DIR" 

# --------------------------------------------------
#
# --------------------------------------------------

#if [ "$OUTPUT" = "" ]; then
#  OUTPUT=$INPUT_BASE
#fi

# ----------

if [ "$DEBUG" = "on" ]; then
  FLAGS+="-DMIP_DEBUG "
#else
#  FLAGS=""
fi

# ----------

#if [ "$GPL" = "on" ]; then
#  INC+="-I../plugins/gpl "
#fi

# ----------

if [ "$FORMAT" = "clap" ]; then
  FLAGS+="-DMIP_PLUGIN -DMIP_PLUGIN_CLAP "
fi

if [ "$FORMAT" = "exe" ]; then
  FLAGS+="-DMIP_EXE -DMIP_PLUGIN_EXE "
fi

#if [ "$FORMAT" = "ladspa" ]; then
#  FLAGS+="-DMIP_PLUGIN -DMIP_PLUGIN_LADSPA "
#fi

#if [ "$FORMAT" = "dssi" ]; then
#  FLAGS+="-DMIP_PLUGIN -DMIP_PLUGIN_DSSI "
#fi

#if [ "$FORMAT" = "lv2" ]; then
#  FLAGS+="-DMIP_PLUGIN -DMIP_PLUGIN_LV2 "
#fi

if [ "$FORMAT" = "vst2" ]; then
  FLAGS+="-DMIP_PLUGIN -DMIP_PLUGIN_VST2 "
fi

if [ "$FORMAT" = "vst3" ]; then
  FLAGS+="-DMIP_PLUGIN -DMIP_PLUGIN_VST3 "
fi

# ----------

#if [ "$ARCH" = "32" ]; then
#  FLAGS+="-m32 "
#fi

#if [ "$ARCH" = "64" ]; then
#  FLAGS+="-m64 "
#fi

# ----------

if [ "$GUI" = "nogui" ]; then
  DEF+="-DMIP_NO_GUI "
fi

#if [ "$GUI" = "xcb" ]; then
#  FLAGS+="-DKODE_GUI_XCB "
#fi

# ----------

#if [ "$PAINTER" = "nopainter" ]; then
#  FLAGS+="-DKODE_NO_PAINTER "
#fi

#if [ "$PAINTER" = "cairo" ]; then
#  FLAGS+="-DKODE_PAINTER_CAIRO "
#fi

#if [ "$PAINTER" = "xcb" ]; then
#  FLAGS+="-DKODE_PAINTER_XCB "
#fi

# ----------

#INC+="-I"
#INC+=$INPUT_DIR

# --------------------------------------------------
# printout
# --------------------------------------------------

#echo "----------"
#echo "input:" $INPUT
#echo "output:" $OUTPUT
#echo "debug:" $DEBUG
#echo "type:" $TYPE
#echo "gui:" $GUI
#echo "flags:" $FLAGS
#echo "def:" $DEF
#echo "inc:" $INC
#echo "lib:" $LIB

# --------------------------------------------------
# create compile.cpp
# --------------------------------------------------

# echo 'compile.cpp:'
#echo '#include "base/kode.h"' >> compile.cpp
#echo '#include "'$INPUT_FILE'"' >> compile.cpp

# --------------------------------------------------
# finalize command line
# --------------------------------------------------

CMDLINE=""
CMDLINE+=$FLAGS
CMDLINE+=$DEF
CMDLINE+=$INC
CMDLINE+=$LIB

#echo ""
#echo "compiling $INPUT_BASE"
#echo "gcc" $CMDLINE $INPUT -o $OUTPUT $POST
#gcc $CMDLINE $INPUT -o $OUTPUT $POST

# echo "g++" $FLAGS $DEF $INC $INPUT -o $OUTPUT $LIB $POST
echo compiling $INPUT_BASE as linux $FORMAT
g++ $FLAGS $DEF $INC $INPUT -o $OUTPUT $LIB $POST


# gcc $CMDLINE compile.cpp -o $OUTPUT.so $POST
##rm compile.cpp


