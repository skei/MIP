#ifndef mip_entry_included
#define mip_entry_included
//----------------------------------------------------------------------

#include "base/mip.h"

//----------------------------------------------------------------------

#ifdef MIP_PLUGIN_CLAP
  #include "plugin/clap/mip_clap_entry.h"
#endif

#ifdef MIP_PLUGIN_EXE
  #include "plugin/exe/mip_exe_entry.h"
#else
  // ugly..
  #ifdef MIP_EXE
    #include "plugin/exe/mip_exe_entry.h"
  #endif
#endif


#ifdef MIP_PLUGIN_VST2
  #include "plugin/vst2/mip_vst2_entry.h"
#endif

#ifdef MIP_PLUGIN_VST3
  #include "plugin/vst3/mip_vst3_entry.h"
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define MIP_DEFAULT_ENTRY(DESC,PLUG)                                                                                      \
                                                                                                                          \
  void MIP_Register() {                                                                                                   \
    MIP_REGISTRY.appendDescriptor(&DESC);                                                                                 \
  }                                                                                                                       \
                                                                                                                          \
  MIP_Plugin* MIP_CreatePlugin( uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) { \
    if (AIndex == 0) return new PLUG(ADescriptor,AHost);                                                                  \
    return nullptr;                                                                                                       \
  }                                                                                                                       \

//----------------------------------------------------------------------
#endif
