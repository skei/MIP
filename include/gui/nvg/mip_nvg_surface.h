#ifndef mip_nvg_surface_included
#define mip_nvg_surface_included
//----------------------------------------------------------------------

#include "gui/base/mip_base_surface.h"
#include "gui/mip_paint_source.h"
#include "gui/mip_paint_target.h"

//

class MIP_NvgSurface
: public MIP_BaseSurface
, public MIP_PaintSource
, public MIP_PaintTarget {

//------------------------------
public:
//------------------------------

  //nvgCreateImage

  MIP_NvgSurface(MIP_PaintTarget* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : MIP_BaseSurface(ATarget,AWidth,AHeight,ADepth) {

    //MIP_Window* window = (MIP_Window*)AOwnerWindow;
    //MIP_Painter* painter = window->getWindowPainter();
    //painter->makeCurrent();
    //MImage = painter->loadImage("/home/skei/Pictures/skei_256.png");
    //painter->resetCurrent();

  }

  //----------

  virtual ~MIP_NvgSurface() {
  }

//------------------------------
public:
//------------------------------

  // src

  //bool  srcIsNanovg()     override { return true; }
  //int   srcGetNvgImage()  override { return MImage; }

//  bool                    srcIsSurface()      override { return true; }
//  bool                    srcIsDrawable()     override { return true; }
//  int32_t                 srcGetWidth()       override { return MWidth; }
//  int32_t                 srcGetHeight()      override { return MHeight; }
//  int32_t                 srcGetDepth()       override { return MDepth; }
//  xcb_connection_t*       srcGetConnection()  override { return MConnection; }
//  virtual xcb_visualid_t  srcGetVisual()      override { return MTarget->tgtGetVisual(); }
//  virtual xcb_drawable_t  srcGetDrawable()    override { return MHandle; }
//  xcb_pixmap_t            srcGetPixmap()      override { return MHandle; }

  // tgt

//  bool                    tgtIsSurface()      override { return true; }
//  bool                    tgtIsDrawable()     override { return true; }
//  int32_t                 tgtGetWidth()       override { return MWidth; }
//  int32_t                 tgtGetHeight()      override { return MHeight; }
//  int32_t                 tgtGetDepth()       override { return MDepth; }
//  xcb_connection_t*       tgtGetConnection()  override { return MConnection; }
//  virtual xcb_visualid_t  tgtGetVisual()      override { return MTarget->tgtGetVisual(); }
//  virtual xcb_drawable_t  tgtGetDrawable()    override { return MHandle; }
//  xcb_pixmap_t            tgtGetPixmap()      override { return MHandle; }

};

//----------------------------------------------------------------------
#endif

