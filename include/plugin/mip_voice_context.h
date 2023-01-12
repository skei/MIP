#ifndef mip_voice_context_included
#define mip_voice_context_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_process_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct MIP_VoiceContext {
  MIP_ProcessContext* process_context   = nullptr;
  float*              buffer            = nullptr;
  double              samplerate        = 0.0;
  uint32_t            min_frames_count  = 0;
  uint32_t            max_frames_count  = 0;
//uint32_t            offset            = 0;
//uint32_t            length            = 0;
  MIP_ParameterArray* parameters        = nullptr;

};


//----------------------------------------------------------------------
#endif
