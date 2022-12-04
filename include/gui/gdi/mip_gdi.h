#ifndef mip_gdi_included
#define mip_gdi_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/gdi/mip_gdi.h"

//----------------------------------------------------------------------

COLORREF MIP_GdiColor(MIP_Color AColor) {
  uint8_t r = (255.0 * AColor.r);
  uint8_t g = (255.0 * AColor.g);
  uint8_t b = (255.0 * AColor.b);
  return RGB(r,g,b);
  //uint8_t a = (255.0 * AColor.a);
  //return (r) + (g << 8) + (b << 16) + (a << 24);
}

//----------------------------------------------------------------------
#endif
