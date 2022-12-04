#ifndef mip_surface_included
#define mip_surface_included
//----------------------------------------------------------------------

#include "base/mip.h"

//----------------------------------------------------------------------

#ifdef MIP_GUI_WIN32
  #include "gui/gdi/mip_gdi_surface.h"
  typedef MIP_GdiSurface MIP_ImplementedSurface;
#endif

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_surface.h"
  typedef MIP_XcbSurface MIP_ImplementedSurface;
#endif


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Surface
: public MIP_ImplementedSurface {

//------------------------------
public:
//------------------------------

  MIP_Surface(MIP_PaintTarget* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : MIP_ImplementedSurface(ATarget,AWidth,AHeight,ADepth) {
  }

  //----------

  virtual ~MIP_Surface() {
  }

};

//----------------------------------------------------------------------
#endif
