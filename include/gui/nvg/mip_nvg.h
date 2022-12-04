#ifndef mip_nanovg_included
#define mip_nanovg_included
//----------------------------------------------------------------------

#include "base/mip.h"

//----------

#ifdef MIP_LINUX
  #include "gui/glx/mip_glx.h"
#endif
#ifdef MIP_WIN32
  #include "gui/wgl/mip_wgl.h"
#endif

//----------

#if MIP_OPENGL_VERSION_MAJOR >= 3
  #define NANOVG_GL3_IMPLEMENTATION
#else
  #define NANOVG_GL2_IMPLEMENTATION
#endif

//----------

#include "extern/nanovg/nanovg.h"
#include "extern/nanovg/nanovg_gl.h"
#include "extern/nanovg/nanovg_gl_utils.h"

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmisleading-indentation"
    #include "extern/nanovg/nanovg.c"
  #pragma GCC diagnostic pop

//----------------------------------------------------------------------
#endif
