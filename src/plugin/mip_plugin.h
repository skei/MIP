#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"

#include "plugin/mip_plugin_descriptor.h"
#include "plugin/mip_plugin_instance.h"
#include "plugin/mip_plugin_editor.h"
#include "plugin/mip_plugin_list.h"

extern MIP_PluginInstance*  MIP_CreateInstance(uint32_t AIndex, MIP_PluginDescriptor* ADescriptor);
extern MIP_PluginEditor*    MIP_CreateEditor(uint32_t AIndex, MIP_EditorListener* AListener, MIP_PluginDescriptor* ADescriptor);

#define MIP_EXE_MAIN
#define MIP_CLAP_MAIN
#define MIP_VST2_MAIN
#define MIP_VST3_MAIN
#define MIP_LV2_MAIN

#ifdef MIP_PLUGIN_CLAP
  #undef MIP_CLAP_MAIN
  #include "plugin/clap/mip_clap.h"
#endif

#ifdef MIP_PLUGIN_EXE
  #undef MIP_EXE_MAIN
  #include "plugin/exe/mip_exe.h"
#endif

#ifdef MIP_PLUGIN_LV2
  #undef MIP_LV2_MAIN
  #include "plugin/lv2/mip_lv2.h"
#endif

#ifdef MIP_PLUGIN_VST2
  #undef MIP_VST2_MAIN
  #include "plugin/vst2/mip_vst2.h"
#endif

#ifdef MIP_PLUGIN_VST3
  #undef MIP_VST3_MAIN
  #include "plugin/vst3/mip_vst3.h"
#endif

//----------------------------------------------------------------------

#define MIP_MAIN    \
                    \
  MIP_EXE_MAIN      \
  MIP_CLAP_MAIN     \
  MIP_VST2_MAIN     \
  MIP_VST3_MAIN     \
  MIP_LV2_MAIN      \


//----------------------------------------------------------------------
#endif
