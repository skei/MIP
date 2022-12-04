#ifndef mip_sogl_included
#define mip_sogl_included
//----------------------------------------------------------------------

/*
  simple-opengl-loader
  https://github.com/tsherif/simple-opengl-loader
*/

//----------------------------------------------------------------------

#include "base/mip.h"

#define SOGL_MAJOR_VERSION 3
#define SOGL_MINOR_VERSION 2

#ifdef MIP_LINUX
  #define SOGL_IMPLEMENTATION_X11
#endif

#ifdef MIP_WIN32
  #define SOGL_IMPLEMENTATION_WIN32
#endif

#include "extern/sogl/simple-opengl-loader.h"

//----------------------------------------------------------------------

// make context current before calling this

bool MIP_LoadOpenGL() {
  //MIP_Print("calling sogl_loadOpenGL\n");
  if (!sogl_loadOpenGL()) {
    MIP_Print("Error loading OpenGL\n");
    const char** failures = sogl_getFailures();
    while (*failures) {
      MIP_DPrint("> %s\n",*failures);
      failures++;
    }
    return false;
  }
//  int result = sogl_loadOpenGL();
//  MIP_Print("sogl_loadOpenGL() says: %i\n",result);
  return true;
}

//----------

void MIP_UnloadOpenGL() {
  sogl_cleanup();
}

//----------------------------------------------------------------------
#endif
