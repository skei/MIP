#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"

#include "plugin/mip_descriptor.h"
#include "plugin/mip_instance.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_plugin_list.h"

extern MIP_Instance*  MIP_CreateInstance(uint32_t AIndex, MIP_Descriptor* ADescriptor);
extern MIP_Editor*    MIP_CreateEditor(uint32_t AIndex, MIP_EditorListener* AListener, MIP_Descriptor* ADescriptor);

#include "plugin/exe/mip_exe.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/vst2/mip_vst2.h"
#include "plugin/vst3/mip_vst3.h"
#include "plugin/lv2/mip_lv2.h"

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
