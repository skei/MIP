#ifndef mip_paint_context_included
#define mip_paint_context_included
//----------------------------------------------------------------------

#include "gui/mip_painter.h"

//----------

struct MIP_PaintContext {
  MIP_Painter*  painter     = nullptr;
  MIP_DRect     updateRect  = {};
};

//----------------------------------------------------------------------
#endif
