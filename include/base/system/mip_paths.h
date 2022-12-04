#ifndef mip_paths_included
#define mip_paths_included
//----------------------------------------------------------------------

#include "base/mip.h"

//----------

/*
  char* MIP_GetHomePath(char* ABuffer)
  const char* MIP_GetExeFilename(char* ABuffer)
  const char* MIP_GetExePath(char* ABuffer)
  const char* MIP_GetLibFilename(char* ABuffer)
  const char* MIP_GetLibPath(char* ABuffer)
  // #define MIP_GetBaseFilename
  // #define MIP_GetBasePath
*/

//----------

#ifdef MIP_LINUX
  #include "base/system/linux/mip_linux_paths.h"
#endif

#ifdef MIP_WIN32
  #include "base/system/win32/mip_win32_paths.h"
#endif

//----------------------------------------------------------------------
#endif
