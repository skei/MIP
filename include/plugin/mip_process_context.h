#ifndef mip_process_context_included
#define mip_process_context_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/mip_note_manager.h"
//#include "plugin/mip_parameter_manager.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct MIP_ProcessContext {
  const clap_process_t* process     = nullptr;
  MIP_ParameterArray*   parameters  = {};
  double                samplerate  = 0.0;
  uint32_t              minbufsize  = 0;
  uint32_t              maxbufsize  = 0;
  uint32_t              counter     = 0;
//float**               buffers     = nullptr;
};

//----------------------------------------------------------------------
#endif
