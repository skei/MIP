#ifndef mip_paint_target_included
#define mip_paint_target_included
//----------------------------------------------------------------------

#ifdef MIP_USE_WIN32
  //#include "gui/win32/mip_win32.h"
#endif

#ifdef MIP_USE_XCB
  #include "gui/xcb/mip_xcb.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_PaintTarget {
public:

  virtual bool              tgtIsWindow()       { return false; }
  virtual bool              tgtIsSurface()      { return false; }
  virtual bool              tgtIsBitmap()       { return false; }
  virtual bool              tgtIsDrawable()     { return false; }

  virtual int32_t           tgtGetWidth()       { return 0; }
  virtual int32_t           tgtGetHeight()      { return 0; }
  virtual int32_t           tgtGetDepth()       { return 0; }
  virtual int32_t           tgtGetStride()      { return 0; }
  virtual uint32_t*         tgtGetBuffer()      { return nullptr; }

  #ifdef MIP_USE_CAIRO
  #endif

  #ifdef MIP_USE_GDI
  #endif

  #ifdef MIP_USE_GLX
  #endif

  #ifdef MIP_USE_NVG
  #endif

  #ifdef MIP_USE_WGL
  #endif

  #ifdef MIP_USE_WIN32
  virtual HWND              tgtGetHWND()        { return nullptr; }
  virtual HDC               tgtGetHDC()         { return nullptr; }
  virtual HBITMAP           tgtGetHBITMAP()     { return nullptr; }
  #endif

  #ifdef MIP_USE_XCB
  virtual xcb_connection_t* tgtGetConnection()  { return nullptr; }
  virtual xcb_visualid_t    tgtGetVisual()      { return 0; }
  virtual xcb_drawable_t    tgtGetDrawable()    { return 0; }
  virtual xcb_window_t      tgtGetWindow()      { return 0; }
  virtual xcb_pixmap_t      tgtGetPixmap()      { return 0; }
  #endif

  #ifdef MIP_USE_XLIB
  virtual Display*          tgtGetDisplay()     { return nullptr; }
  #endif

};

//----------------------------------------------------------------------
#endif
