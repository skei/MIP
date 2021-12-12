#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

#include "mip.h"

//class MIP_Plugin {
//};

#include "plugin/mip_descriptor.h"
#include "plugin/mip_instance.h"
#include "plugin/mip_editor.h"

//#include "mip_host.h"
//#include "mip_parameter.h"
//#include "mip_plugin.h"
//#include "mip_process_context.h"

MIP_Descriptor*  _MIP_CreateDescriptor();
MIP_Instance*    _MIP_CreateInstance(MIP_Descriptor* d);
MIP_Editor*      _MIP_CreateEditor(MIP_EditorListener* l, MIP_Descriptor* d);

#include "plugin/exe/mip_exe.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/vst2/mip_vst2.h"
#include "plugin/vst3/mip_vst3.h"
#include "plugin/lv2/mip_lv2.h"

//----------------------------------------------------------------------

#define MIP_MAIN(D,I,E)                                                                               \
                                                                                                      \
  MIP_Descriptor* _MIP_CreateDescriptor()                                     { return new D();    }  \
  MIP_Instance*   _MIP_CreateInstance(MIP_Descriptor* d)                      { return new I(d);   }  \
  MIP_Editor*     _MIP_CreateEditor(MIP_EditorListener* l, MIP_Descriptor* d) { return new E(l,d); }  \
                                                                                                      \
  MIP_EXE_MAIN(D,I,E)                                                                                 \
  MIP_CLAP_MAIN(D,I,E)                                                                                \
  MIP_VST2_MAIN(D,I,E)                                                                                \
  MIP_VST3_MAIN(D,I,E)                                                                                \
  MIP_LV2_MAIN(D,I,E)                                                                                 \


//----------------------------------------------------------------------
#endif
