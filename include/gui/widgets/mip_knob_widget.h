#ifndef mip_knob_widget_included
#define mip_knob_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_KnobWidget
: public MIP_SliderWidget {

//------------------------------
private:
//------------------------------


//------------------------------
protected:
//------------------------------


  bool      MDrawArc        = true;
  MIP_Color MArcBackColor   = MIP_COLOR_DARK_GRAY;
  MIP_Color MArcValueColor  = MIP_COLOR_LIGHT_GRAY;
  double    MArcThickness   = 10.0;;


//------------------------------
public:
//------------------------------

  MIP_KnobWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0)
  : MIP_SliderWidget(ARect,AText,AValue) {

    MDrawSlider = false;
    MDrawBorder = false;

  }

  //----------

  virtual ~MIP_KnobWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawArc(bool ADraw=true)         { MDrawArc = ADraw; }
  virtual void setArcBackColor(MIP_Color AColor)   { MArcBackColor = AColor; }
  virtual void setArcValueColor(MIP_Color AColor)  { MArcValueColor = AColor; }
  virtual void setArcThickness(double AThickness)  { MArcThickness = AThickness; }

//------------------------------
public:
//------------------------------

  virtual void drawArc(MIP_PaintContext* AContext) {

    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_DRect mrect = getRect();
    MIP_Painter* painter = AContext->painter;

    double thick = MArcThickness * S;

    double cx = mrect.x + (mrect.w * 0.5);
    double cy = mrect.y + (mrect.h * 0.5);
    double r  = (mrect.w - thick) * 0.5;

    painter->setDrawColor(MArcBackColor);
    painter->setLineWidth(thick);
    painter->drawArc(cx,cy,r,0.15*MIP_PI2,0.8*MIP_PI2);

    double a1 = 0.35 + (MValue * 0.8);
    double a2 = MValue * 0.8;
    a1 *= MIP_PI2;
    a2 *= MIP_PI2;

    painter->setDrawColor(MArcValueColor);
    painter->setLineWidth(thick);
    painter->drawArc(cx,cy,r,a1,a2);

//    double x = mrect.x;
//    double y = mrect.y;
//    double w = mrect.w * MValue;
//    double h = mrect.h;
//    painter->fillRect(x,y,w,h);
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    if (MDrawArc) drawArc(AContext);
    //if (MDrawText) drawText(AContext);
    //if (MDrawValue) drawValue(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);
  }


};

//----------------------------------------------------------------------
#endif






