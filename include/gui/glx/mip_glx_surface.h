#ifndef mip_glx_surface_included
#define mip_glx_surface_included
//----------------------------------------------------------------------

#include "gui/base/mip_base_plugin.h"

class MIP_GlxSurface
: public MIP_BaseSurface {

public:

  MIP_GlxSurface(MIP_PaintTarget* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : MIP_BaseSurface(ATarget,AWidth,AHeight,ADepth) {
  }

  virtual ~MIP_GlxSurface() {
  }

};

//----------------------------------------------------------------------
#endif


