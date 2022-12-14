#ifndef mip_panel_widget_included
#define mip_panel_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widget.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_PanelWidget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  bool      MFillBackground   = true;
  bool      MDrawBorder       = true;

  double    MBorderWidth      = 1.0;
  uint32_t  MBorderEdges      = MIP_EDGE_ALL;

  MIP_Color MBackgroundColor  = MIP_COLOR_GRAY;
  MIP_Color MBorderColor      = MIP_COLOR_DARK_GRAY;

//------------------------------
public:
//------------------------------

  MIP_PanelWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
  }

  //----------

  virtual ~MIP_PanelWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  virtual void setDrawBorder(bool ADraw=true)       { MDrawBorder = ADraw; }
  virtual void setBorderWidth(double AWidth)        { MBorderWidth = AWidth; }
  virtual void setBackgroundColor(MIP_Color AColor) { MBackgroundColor = AColor; }
  virtual void setBorderColor(MIP_Color AColor)     { MBorderColor = AColor; }
  virtual void setBorderEdges(uint32_t AEdges)      { MBorderEdges = AEdges; }

//------------------------------
public:
//------------------------------

  virtual void fillBackground(MIP_PaintContext* AContext) {
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();

    MIP_Color color = MBackgroundColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
    painter->setFillColor(color);

    painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
  }

  //----------

  virtual void drawBorder(MIP_PaintContext* AContext) {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();

    MIP_Color color = MBorderColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
    painter->setDrawColor(color);

    painter->setLineWidth(MBorderWidth * S);
    if (MBorderEdges == MIP_EDGE_ALL) {
      painter->drawRect(mrect.x,mrect.y,mrect.w,mrect.h);
    }
    else {
      if (MBorderEdges == MIP_EDGE_TOP)     painter->drawLine( mrect.x,    mrect.y,   mrect.x2(), mrect.y    );
      if (MBorderEdges == MIP_EDGE_BOTTOM)  painter->drawLine( mrect.x,    mrect.y2(),mrect.x2(), mrect.y2() );
      if (MBorderEdges == MIP_EDGE_LEFT)    painter->drawLine( mrect.x,    mrect.y,   mrect.x,    mrect.y2() );
      if (MBorderEdges == MIP_EDGE_RIGHT)   painter->drawLine( mrect.x2(), mrect.y,   mrect.x2(), mrect.y2() );
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);

  }

  //----------


};

//----------------------------------------------------------------------
#endif

