#ifndef mip_utils_included
#define mip_utils_included
//----------------------------------------------------------------------

#include "mip.h"

// MIP_GetThreadId
#include <sys/syscall.h>
#include <sys/unistd.h>
#define gettid() syscall(SYS_gettid)

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

// uint32_t thread_id = MIP_GetThreadId();

pid_t MIP_GetProcessId() {
  return getpid();
}

//----------

pid_t MIP_GetThreadId() {
  return gettid();
}

//----------------------------------------------------------------------
#endif
