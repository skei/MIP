#ifndef mip_note_included
#define mip_note_included
//----------------------------------------------------------------------

#include "base/mip.h"

struct MIP_Note {
  int16_t port;
  int16_t channel;
  int16_t key;
  int16_t noteid;
};

//----------------------------------------------------------------------
#endif
