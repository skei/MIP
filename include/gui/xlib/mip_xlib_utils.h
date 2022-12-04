#ifndef mip_xlib_utils_included
#define mip_xlib_utils_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/xlib/mip_xlib.h"

//----------------------------------------------------------------------

//static
int (*mip_xlib_old_x_error_handler)(Display*,XErrorEvent*);

//----------------------------------------------------------------------

static
int mip_xlib_x_error_handler(Display* d, XErrorEvent* e) {
  char error_text[512] = {0};
  XGetErrorText(d,e->error_code,error_text,511);
  MIP_DPrint("X Error %i: %s\n",e->error_code,error_text);
  return 0;
}

//----------

void MIP_XlibInitErrorHandler() {
  mip_xlib_old_x_error_handler = XSetErrorHandler(mip_xlib_x_error_handler);
}

//----------

void MIP_XlibCleanupErrorHandler() {
  XSetErrorHandler(mip_xlib_old_x_error_handler);
}

  //----------

  //static void glx_error_message(const char *format, ...) {
  //  va_list args;
  //  va_start(args, format);
  //  fprintf(stderr, "glx error: ");
  //  vfprintf(stderr, format, args);
  //  va_end(args);
  //}

//----------------------------------------------------------------------
#endif
