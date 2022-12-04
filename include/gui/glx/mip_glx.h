#ifndef mip_glx_included
#define mip_glx_included
//----------------------------------------------------------------------

/*

  GLX functions should not be called between glBegin and glEnd operations. If a
  GLX function is called within a glBegin/glEnd pair, then the result is undefined;
  however, no error is reported.

  https://stackoverflow.com/questions/47918078/creating-opengl-structures-in-a-multithreaded-program
  The requirement for OpenGL is that the context created for rendering should
  be owned by single thread at any given point and the thread that owns context
  should make it current and then call any gl related function. If you do that
  without owning and making context current then you get segmentation faults.
  By default the context will be current for the main thread.

  .. other option is to make one context per thread and make it current when
  thread starts.
*/

//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/sogl/mip_sogl.h"

#include <GL/gl.h>
#include <GL/glx.h>

//----------------------------------------------------------------------
#endif
