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

//#define MIP_RECT_STACK_SIZE 1024
typedef MIP_Stack<MIP_DRect,MIP_CLIP_RECT_STACK_SIZE> MIP_RectStack;

//----------------------------------------------------------------------

class MIP_Painter
: public MIP_ImplementedPainter {

//------------------------------
private:
//------------------------------

  MIP_DRect MClipRect       = {};
  MIP_RectStack MClipStack  = {};

//------------------------------
public:
//------------------------------

  MIP_Painter(MIP_PaintTarget* ATarget)
  : MIP_ImplementedPainter(ATarget) {
    int32_t w = ATarget->tgtGetWidth();
    int32_t h = ATarget->tgtGetHeight();
    MClipRect = MIP_DRect(0,0,w,h);
  }

  //----------

  virtual ~MIP_Painter() {
  }

//------------------------------
public: // clipping
//------------------------------

  /*
    - push current clip rect
    - set new clip rect
  */

  virtual void pushClip(MIP_DRect ARect) {
    //MIP_Print("pushing %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    MClipStack.push(MClipRect);
    MClipRect = ARect;
    resetClip();
    //MIP_Print("setting %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    setClip(MClipRect);
  }

  //----------

  virtual void pushOverlapClip(MIP_DRect ARect) {
    MIP_DRect r = ARect;
    r.overlap(MClipRect);
    pushClip(r);
  }

  //----------

  /*
    - pop rect
    - set clip rect to popped rect
  */

  virtual MIP_DRect popClip() {
    //MIP_Assert( !MClipStack.isEmpty() );
    MClipRect = MClipStack.pop();
    //MIP_Print("popped %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    //resetClip();
    //MIP_Print("setting %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    setClip(MClipRect);
    return MClipRect;
  }

  //----------

  virtual void resetClipStack() {
    MClipStack.reset();
  }

  //----------

  virtual void setClipRect(MIP_DRect ARect) {
    MClipRect = ARect;
  }

  //----------

  virtual MIP_DRect getClipRect() {
    return MClipRect;
  }

//------------------------------
public:
//------------------------------

  virtual void drawTextBox(MIP_DRect ARect, const char* AText, uint32_t AAlignment) {
    double bounds[4] = {0};
    getTextBounds(AText,bounds);
    double x = ARect.x - bounds[0];
    double y = ARect.y - bounds[1];
    double w = bounds[2] - bounds[0];
    double h = bounds[3] - bounds[1];
    if      (AAlignment & MIP_TEXT_ALIGN_LEFT)        { }
    else if (AAlignment & MIP_TEXT_ALIGN_RIGHT)       { x = ARect.w - w + x; }
    else /*if (AAlignment & MIP_TEXT_ALIGN_CENTER)*/  { x += ((ARect.w - w) * 0.5); }
    if      (AAlignment & MIP_TEXT_ALIGN_TOP)         { }
    else if (AAlignment & MIP_TEXT_ALIGN_BOTTOM)      { y = ARect.h - h + y; }
    else /*if (AAlignment & MIP_TEXT_ALIGN_CENTER)*/  { y += ((ARect.h - h) * 0.5); }
    drawText(x,y,AText);
  }

};

//----------------------------------------------------------------------
#endif
