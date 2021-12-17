#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"

#include "plugin/mip_plugin_descriptor.h"
#include "plugin/mip_plugin_instance.h"
#include "plugin/mip_plugin_editor.h"
#include "plugin/mip_plugin_list.h"

/*
  These are implemented by you (your plugin), but called from
  the global/staatic plugin-list constructor
*/

extern MIP_PluginInstance*  MIP_CreateInstance(uint32_t AIndex, MIP_PluginDescriptor* ADescriptor);
extern MIP_PluginEditor*    MIP_CreateEditor(uint32_t AIndex, MIP_EditorListener* AListener, MIP_PluginDescriptor* ADescriptor);

/*
  include entry points (and code) for selected plugin formats
*/

#ifdef MIP_PLUGIN_CLAP
  //#undef MIP_CLAP_MAIN
  #include "plugin/clap/mip_clap.h"
#else
  #define MIP_CLAP_MAIN
#endif

#ifdef MIP_PLUGIN_EXE
  //#undef MIP_EXE_MAIN
  #include "plugin/exe/mip_exe.h"
#else
  #define MIP_EXE_MAIN
#endif

#ifdef MIP_PLUGIN_LV2
  //#undef MIP_LV2_MAIN
  #include "plugin/lv2/mip_lv2.h"
#else
  #define MIP_LV2_MAIN
#endif

#ifdef MIP_PLUGIN_VST2
  //#undef MIP_VST2_MAIN
  #include "plugin/vst2/mip_vst2.h"
#else
  #define MIP_VST2_MAIN
#endif

#ifdef MIP_PLUGIN_VST3
  //#undef MIP_VST3_MAIN
  #include "plugin/vst3/mip_vst3.h"
#else
  #define MIP_VST3_MAIN
#endif

/*
  put this (MIP_MAIN at the bottom of your plugin main .cpp file
*/

#define MIP_MAIN    \
                    \
  MIP_EXE_MAIN      \
  MIP_CLAP_MAIN     \
  MIP_VST2_MAIN     \
  MIP_VST3_MAIN     \
  MIP_LV2_MAIN      \


//----------------------------------------------------------------------
#endif
