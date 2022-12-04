#ifndef mip_win32_paths_included
#define mip_win32_paths_included
//----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "base/mip.h"
#include "base/mip_win32.h"

//#undef  _WIN32_WINNT
//#define _WIN32_WINNT 0x0501
////#ifndef MIP_WIN32_GDIPLUS
//#define  WIN32_LEAN_AND_MEAN
////#endif
//#include <windows.h>

#include <shlobj.h>

//----------------------------------------------------------------------

/*
  MIP_GetHomePath    '/home/<user>/'
  MIP_GetExeFilename '/home/<user>/Desktop/skeilib/bin/skeilib_debug'
  MIP_GetExePath     '/home/<user>/Desktop/skeilib/bin/'
*/

//----------------------------------------------------------------------

char* MIP_GetHomePath(char* ABuffer) {
  //strcat(ABuffer, getenv("HOME"));
  ABuffer[0] = '\0';
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
  WCHAR wch[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW( NULL, CSIDL_PROFILE, NULL, 0, wch ))) {
    wcstombs(ABuffer,wch,MIP_MAX_STRING_LENGTH-1);
    strcat(ABuffer, (char*)"\\");
  }
  return ABuffer;
}

//----------

char* MIP_GetDesktopPath(char* ABuffer) {
  //strcat(ABuffer, getenv("HOME"));
  ABuffer[0] = '\0';
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
  WCHAR wch[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW( NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, wch ))) {
    wcstombs(ABuffer,wch,MIP_MAX_STRING_LENGTH-1);
    strcat(ABuffer, (char*)"\\");
  }
  return ABuffer;
}

//----------

const char* MIP_GetExeFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  //#if defined MIP_PLUGIN //|| defined MIP_PLUGIN_EXE
  //  GetModuleFileName(MIP_GLOBAL_WIN32_INSTANCE, ABuffer, MIP_MAX_PATH_LENGTH-1);
  //#else
    strcat(ABuffer,(char*)".\\");
  //#endif
  return ABuffer;//nullptr;
}

//----------

const char* MIP_GetExePath(char* ABuffer) {
  //#if defined MIP_PLUGIN //|| defined MIP_PLUGIN_EXE
  //  char filepath[MIP_MAX_PATH_LENGTH] = "";
  //  GetModuleFileName(MIP_GLOBAL_WIN32_INSTANCE, filepath, MIP_MAX_PATH_LENGTH-1);
  //  filepath[MIP_MAX_PATH_LENGTH-1] = '\0';
  //  const char* slash = strrchr(filepath,'\\');
  //  if (slash) {
  //    int len = (slash+1) - (char*)filepath;
  //    strncpy(ABuffer, filepath, len);
  //    ABuffer[len] = '\0';
  //  }
  //  else {
  //    strcat(ABuffer,(char*)".\\");
  //  }
  //#else
    strcat(ABuffer,(char*)".\\");
  //#endif
  return ABuffer;
}

//----------

const char* MIP_GetLibFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  //#if defined MIP_PLUGIN //|| defined MIP_PLUGIN_EXE
    GetModuleFileName(MIP_GLOBAL_WIN32_INSTANCE, ABuffer, MIP_MAX_PATH_LENGTH-1);
  //#else
  //  strcat(ABuffer,(char*)".\\");
  //#endif
  return ABuffer;//nullptr;
}

//----------

const char* MIP_GetLibPath(char* ABuffer) {
  //#if defined MIP_PLUGIN //|| defined MIP_PLUGIN_EXE
    char filepath[MIP_MAX_PATH_LENGTH] = "";
    GetModuleFileName(MIP_GLOBAL_WIN32_INSTANCE, filepath, MIP_MAX_PATH_LENGTH-1);
    filepath[MIP_MAX_PATH_LENGTH-1] = '\0';
    const char* slash = strrchr(filepath,'\\');
    if (slash) {
      int len = (slash+1) - (char*)filepath;
      strncpy(ABuffer, filepath, len);
      ABuffer[len] = '\0';
    }
    else {
      strcat(ABuffer,(char*)".\\");
    }
  //#else
  //  strcat(ABuffer,(char*)".\\");
  //#endif
  return ABuffer;
}

//----------------------------------------------------------------------

#ifdef MIP_PLUGIN
  #define MIP_GetBaseFilename  MIP_GetLibFilename
  #define MIP_GetBasePath      MIP_GetLibPath
#else
  #define MIP_GetBaseFilename  MIP_GetExeFilename
  #define MIP_GetBasePath      MIP_GetExePath
#endif

//----------------------------------------------------------------------


//----------

//const char* MIP_GetBaseFile(char* ABuffer) {
//  ABuffer[0] = '\0';
//  #if defined MIP_PLUGIN //|| defined MIP_PLUGIN_EXE
//    GetModuleFileName(MIP_GLOBAL_WIN32_INSTANCE, ABuffer, MIP_MAX_PATH_LENGTH-1);
//  #else
//    strcat(ABuffer,(char*)".\\");
//  #endif
//  return ABuffer;//nullptr;
//}

//----------

//char* MIP_GetBasePath(char* ABuffer) {
//  #if defined MIP_PLUGIN //|| defined MIP_PLUGIN_EXE
//    char filepath[MIP_MAX_PATH_LENGTH] = "";
//    GetModuleFileName(MIP_GLOBAL_WIN32_INSTANCE, filepath, MIP_MAX_PATH_LENGTH-1);
//    filepath[MIP_MAX_PATH_LENGTH-1] = '\0';
//    const char* slash = strrchr(filepath,'\\');
//    if (slash) {
//      int len = (slash+1) - (char*)filepath;
//      strncpy(ABuffer, filepath, len);
//      ABuffer[len] = '\0';
//    }
//    else {
//      strcat(ABuffer,(char*)".\\");
//    }
//  #else
//    strcat(ABuffer,(char*)".\\");
//  #endif
//  return ABuffer;
//}

//----------------------------------------------------------------------
#endif
