
// nc -U -l -k /tmp/mip.socket
//#define MIP_DEBUG_PRINT_SOCKET

#define MIP_NO_PLUGIN
#define MIP_NO_GUI

//#define MIP_GUI_XCB
//#define MIP_USE_CAIRO

#include "mip.h"
#include "base/utils/mip_arguments.h"

//#include "claphost/arguments.h"

//----------------------------------------------------------------------

//----------------------------------------------------------------------

void print_help() {
  printf("usage: claphost plugin-path index [options]\n");
  printf("options:\n");
  printf("  -m  --midi-input    <path>\n");
  printf("  -a  --audio-input   <path>\n");
  printf("  -o  --audio-output  <path>\n");
  printf("  -s  --sample_rate   <samples>        (default 44100.0)\n");
  printf("  -b  --block_size    <samples>        (default 256)\n");
  printf("  -c  --channels      <inputs:outputs> (default 2:2)\n");
  printf("  -d  --decay-seconds <seconds>        (default 0.0)\n");
  printf("  -l  --list-plugins\n");
  printf("  -p  --print-descriptor\n");
  printf("  -f  --fuzz-block-size\n");
}

//----------------------------------------------------------------------

int main(int argc, char** argv) {
  MIP_Arguments arg;
  arg.init(argc,argv);
  uint32_t num_args = arg.count();

  const char* plugin_path       = "";
  uint32_t    plugin_index      = 0;
  const char* midi_input_file   = "";
  const char* audio_input_file  = "";
  const char* audio_output_file = "";
  float       sample_rate       = 0.0;
  uint32_t    block_size        = 0;
  uint32_t    num_audio_inputs  = 0;
  uint32_t    num_audio_outputs = 0;
  bool        list_plugins      = false;
  bool        print_descriptor  = false;
  bool        fuzz_block_size   = false;
  float       decay_seconds     = 0.0;
  int32_t     remap_cc          = -1;
  int32_t     remap_param       = -1;


  if ((num_args == 1) || (arg.hasOption("-h","--help"))) {
    print_help();
    exit(0);
  }

  else if (num_args == 2) {
    plugin_path = arg.getStr(1);
    plugin_index = 0;
  }

  else {
    plugin_path       = arg.getStr(1);
    plugin_index      = arg.getInt(2);
    midi_input_file   = arg.getArgStr(                  "-m",   "--midi-input");
    audio_input_file  = arg.getArgStr(                  "-a",   "--audio-input");
    audio_output_file = arg.getArgStr(                  "-o",   "--audio-output");
    sample_rate       = arg.getArgFloat(                "-s",   "--sample_rate");
    block_size        = arg.getArgInt(                  "-b",   "--block_size");
    num_audio_inputs  = arg.getArgInt(                  "-c",   "--channels");
    num_audio_outputs = arg.getArgIntAfterSymbol( ':',  "-c",   "--channels");
    decay_seconds     = arg.getArgFloat(                "-d",   "--decay-seconds");
    list_plugins      = arg.hasOption(                  "-l",   "--list-plugins");
    print_descriptor  = arg.hasOption(                  "-p",   "--print-descriptor");
    fuzz_block_size   = arg.hasOption(                  "-f",   "--fuzz-block-size");
    if (arg.hasOption("-r","--remap")) {
      remap_cc        = arg.getArgInt(                  "-r",   "--remap");
      remap_param     = arg.getArgIntAfterSymbol( ':',  "-r",   "--remap");
    }
  }

  printf("plugin_path       %s\n",plugin_path);
  printf("plugin_index      %i\n",plugin_index);
  printf("midi_input file   %s\n",midi_input_file);
  printf("audio_input file  %s\n",audio_input_file);
  printf("audio_output file %s\n",audio_output_file);
  printf("sample_rate       %.2f\n",sample_rate);
  printf("block_size        %i\n",block_size);
  printf("channels (in:out) %i:%i\n",num_audio_inputs,num_audio_outputs);
  printf("decay seconds     %.2f\n",decay_seconds);
  printf("list_plugins      %s\n",list_plugins?"yes":"no");
  printf("print_descriptor  %s\n",print_descriptor?"yes":"no");
  printf("fuzz block size   %s\n",fuzz_block_size?"yes":"no");
  printf("remap (cc:param)  %i:%i\n",remap_cc,remap_param);

//  if (list_plugins) {
//    MIP_Print("list plugins\n");
//  }
//  else if (print_descriptor) {
//    MIP_Print("print descriptor\n");
//  }
//  else {
//    // process
//    if (midi_input_file) {
//    }
//    if (audio_input_file) {
//    }
//    if (audio_output_file) {
//    }
//
//  }

  return 0;
}

