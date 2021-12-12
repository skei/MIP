#ifndef mip_utils_included
#define mip_utils_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

//TODO: -> MIP_StrUtils.h

// https://stackoverflow.com/questions/7666509/hash-function-for-string

uint32_t MIP_HashString(const char* buffer) {
  char* ptr = (char*)buffer;
  unsigned long h = 5381;
  int c;
  while ((c = *ptr++)) {
    h = ((h << 5) + h) + c; // h * 33 + c
  }
  return h;
}


//----------------------------------------------------------------------
#endif
