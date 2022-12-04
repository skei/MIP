#ifndef mip_painter_included
#define mip_painter_included
//----------------------------------------------------------------------

#ifdef MIP_PAINTER_GDI
  #include "gui/gdi/mip_gdi_painter.h"
  typedef MIP_GdiPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_GLX
  #include "gui/glx/mip_glx_painter.h"
  typedef MIP_GlxPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_NVG
  #include "gui/nvg/mip_nvg_painter.h"
  typedef MIP_NvgPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_WGL
  #include "gui/wgl/mip_wgl_painter.h"
  typedef MIP_WglPainter MIP_ImplementedPainter;
#endif

#ifdef MIP_PAINTER_XCB
  #include "gui/xcb/mip_xcb_painter.h"
  typedef MIP_XcbPainter MIP_ImplementedPainter;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Painter
: public MIP_ImplementedPainter {

public:

  MIP_Painter(MIP_PaintTarget* ATarget)
  : MIP_ImplementedPainter(ATarget) {
  }

  virtual ~MIP_Painter() {
  }

};

//----------------------------------------------------------------------
#endif
