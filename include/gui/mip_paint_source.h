#ifndef mip_paint_source_included
#define mip_paint_source_included
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

class MIP_PaintSource {
public:

  virtual bool              srcIsWindow()       { return false; }
  virtual bool              srcIsSurface()      { return false; }
  virtual bool              srcIsBitmap()       { return false; }
  virtual bool              srcIsDrawable()     { return false; }
  virtual bool              srcIsNanovg()       { return false; }

  virtual int32_t           srcGetWidth()       { return 0; }
  virtual int32_t           srcGetHeight()      { return 0; }
  virtual int32_t           srcGetDepth()       { return 0; }
  virtual int32_t           srcGetStride()      { return 0; }
  virtual uint32_t*         srcGetBuffer()      { return nullptr; }

  #ifdef MIP_USE_CAIRO
  #endif

  #ifdef MIP_USE_GDI
  #endif

  #ifdef MIP_USE_GLX
  #endif

  #ifdef MIP_USE_NVG
  virtual int               srcGetNvgImage()    { return -1; }
  #endif

  #ifdef MIP_USE_WGL
  #endif

  #ifdef MIP_USE_WIN32
  virtual HWND              srcGetHWND()        { return nullptr; }
  virtual HDC               srcGetHDC()         { return nullptr; }
  virtual HBITMAP           srcGetHBITMAP()     { return nullptr; }
  #endif

  #ifdef MIP_USE_XCB
  virtual xcb_connection_t* srcGetConnection()  { return nullptr; }
  virtual xcb_visualid_t    srcGetVisual()      { return 0; }
  virtual xcb_drawable_t    srcGetDrawable()    { return 0; }
  virtual xcb_window_t      srcGetWindow()      { return 0; }
  virtual xcb_pixmap_t      srcGetPixmap()      { return 0; }
  #endif

  #ifdef MIP_USE_XLIB
  virtual Display*          srcGetDisplay()     { return nullptr; }
  #endif

};

//----------------------------------------------------------------------
#endif
