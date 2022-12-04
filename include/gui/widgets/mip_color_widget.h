#ifndef mip_color_widget_included
#define mip_color_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ColorWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  MIP_Color MColor = MIP_COLOR_WHITE;

//------------------------------
public:
//------------------------------

  MIP_ColorWidget(MIP_DRect ARect, MIP_Color AColor)
  : MIP_Widget(ARect) {
    MColor = AColor;
  }

  //----------

  virtual ~MIP_ColorWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_Painter*  painter = AContext->painter;
    MIP_DRect     mrect   = getRect();
    painter->setFillColor(MColor);
    painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
    paintChildWidgets(AContext);
  }

  //----------


};

//----------------------------------------------------------------------
#endif
