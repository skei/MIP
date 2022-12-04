#ifndef mip_xcb_surface_included
#define mip_xcb_surface_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/base/mip_base_surface.h"
#include "gui/mip_paint_source.h"
#include "gui/mip_paint_target.h"
#include "gui/xcb/mip_xcb.h"

//#ifdef MIP_USE_CAIRO
//  #include "gui/cairo/mip_cairo.h"
//#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_XcbSurface
: public MIP_BaseSurface
, public MIP_PaintSource
, public MIP_PaintTarget {

//------------------------------
private:
//------------------------------

  //HBITMAP   MHandle = nullptr;
  int32_t   MWidth  = 0;
  int32_t   MHeight = 0;
  int32_t   MDepth  = 0;

//------------------------------
private:
//------------------------------

  xcb_connection_t* MConnection       = nullptr;
  xcb_pixmap_t      MHandle           = XCB_NONE;

  MIP_PaintTarget*  MTarget           = nullptr;

//  xcb_visualid_t    MTargetVisual     = XCB_NONE;
//  xcb_drawable_t    MTargetDrawable   = XCB_NONE;

//  MIP_Drawable*     MTarget           = nullptr;
//  bool              MIsWindow         = false;
//  xcb_window_t      MWindow           = XCB_NONE;
//
//  #ifdef MIP_USE_CAIRO
//    cairo_surface_t*  MCairoSurface     = nullptr;
//    //cairo_device_t*   MCairoDevice      = nullptr;
//  #endif

//------------------------------
public:
//------------------------------

  MIP_XcbSurface(MIP_PaintTarget* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : MIP_BaseSurface(ATarget,AWidth,AHeight,ADepth) {

    MTarget = ATarget;

    if (ATarget->tgtIsWindow()) {

      //MTarget         = ATarget;
      MWidth          = AWidth;
      MHeight         = AHeight;
      if (ADepth == 0) MDepth = ATarget->tgtGetDepth();
      else MDepth = ADepth;
      MConnection     = ATarget->tgtGetConnection();

      //MTargetDrawable = ATarget->tgtGetDrawable();
      //MTargetVisual   = ATarget->tgtGetVisual();

      xcb_drawable_t drawable = ATarget->tgtGetDrawable();

      MHandle = xcb_generate_id(MConnection);

      xcb_create_pixmap(
        MConnection,
        MDepth,
        MHandle,
        drawable,//MTargetDrawable,
        MWidth,
        MHeight
      );
      xcb_flush(MConnection);

      //#ifdef MIP_USE_CAIRO
      //  MCairoSurface = cairo_xcb_surface_create(
      //    MConnection,
      //    MPixmap,
      //    mip_xcb_find_visual(MConnection,MTargetVisual),
      //    MWidth,
      //    MHeight
      //  );
      //  //MCairoDevice = cairo_device_reference(cairo_surface_get_device(MCairoSurface));
      //#endif

    }
    MIP_Assert(MHandle);
  }

  //----------

  virtual ~MIP_XcbSurface() {
    xcb_free_pixmap(MConnection,MHandle);
    //#ifdef MIP_USE_CAIRO
    //  cairo_surface_destroy(MCairoSurface);
    //  //cairo_device_finish(MCairoDevice);
    //  //cairo_device_destroy(MCairoDevice);
    //#endif
  }

//------------------------------
public: // paint target
//------------------------------

  // src

  bool                    srcIsSurface()      override { return true; }
  bool                    srcIsDrawable()     override { return true; }
  int32_t                 srcGetWidth()       override { return MWidth; }
  int32_t                 srcGetHeight()      override { return MHeight; }
  int32_t                 srcGetDepth()       override { return MDepth; }
  xcb_connection_t*       srcGetConnection()  override { return MConnection; }
  virtual xcb_visualid_t  srcGetVisual()      override { return MTarget->tgtGetVisual(); }
  virtual xcb_drawable_t  srcGetDrawable()    override { return MHandle; }
  xcb_pixmap_t            srcGetPixmap()      override { return MHandle; }

  // tgt

  bool                    tgtIsSurface()      override { return true; }
  bool                    tgtIsDrawable()     override { return true; }
  int32_t                 tgtGetWidth()       override { return MWidth; }
  int32_t                 tgtGetHeight()      override { return MHeight; }
  int32_t                 tgtGetDepth()       override { return MDepth; }
  xcb_connection_t*       tgtGetConnection()  override { return MConnection; }
  virtual xcb_visualid_t  tgtGetVisual()      override { return MTarget->tgtGetVisual(); }
  virtual xcb_drawable_t  tgtGetDrawable()    override { return MHandle; }
  xcb_pixmap_t            tgtGetPixmap()      override { return MHandle; }

  #ifdef MIP_USE_XLIB
  Display*                tgtGetDisplay()     override { return MTarget->tgtGetDisplay(); }
  #endif

};

//----------------------------------------------------------------------
#endif

















  /*
    cairo_xcb_surface_create()

    Creates an XCB surface that draws to the given drawable. The way that
    colors are represented in the drawable is specified by the provided visual.

    Note: If drawable is a Window, then the function cairo_xcb_surface_set_size()
    must be called whenever the size of the window changes.

    When drawable is a Window containing child windows then drawing to the
    created surface will be clipped by those child windows. When the created
    surface is used as a source, the contents of the children will be included.

    Returns a pointer to the newly created surface. The caller owns the surface
    and should call cairo_surface_destroy() when done with it.

    This function always returns a valid pointer, but it will return a pointer
    to a "nil" surface if an error such as out of memory occurs. You can use
    cairo_surface_status() to check for this.
  */


  //void fill(MIP_Color AColor) override {}
  //void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, MIP_Color AColor) override {}
  //void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource) override {}
  //void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {}






