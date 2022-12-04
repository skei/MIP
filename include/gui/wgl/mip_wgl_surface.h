#ifndef mip_wgl_surface_included
#define mip_wgl_surface_included
//----------------------------------------------------------------------

#include "gui/base/mip_base_plugin.h"

class MIP_WglSurface
: public MIP_BaseSurface {

public:

  MIP_WglSurface(MIP_PaintTarget* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : MIP_BaseSurface(ATarget,AWidth,AHeight,ADepth) {
  }

  virtual ~MIP_WglSurface() {
  }

};

//----------------------------------------------------------------------
#endif


