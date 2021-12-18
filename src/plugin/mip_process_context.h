#ifndef mip_plugin_context_included
#define mip_plugin_context_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

struct MIP_ProcessContext {

  void*     formatptr     = nullptr;

  uint32_t  mode          = 0;
  //uint32_t  offset        = 0;

  float     samplerate    = 44100.0;
  float     tempo         = 120.0;
  uint32_t  timesignum    = 4;
  uint32_t  timesigdenom  = 4;

  uint32_t  playstate     = 0;
  uint32_t  samplepos     = 0;
  float     beatpos       = 0;

  uint32_t  blocksize     = 256;
  uint32_t  numinputs     = 0;
  uint32_t  numoutputs    = 0;
  float**   inputs        = nullptr;
  float**   outputs       = nullptr;




};

//----------------------------------------------------------------------
#endif
