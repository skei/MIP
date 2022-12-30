#ifndef mip_slider_widget_included
#define mip_slider_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_SliderWidget
: public MIP_DragValueWidget {

//------------------------------
private:
//------------------------------


//------------------------------
protected:
//------------------------------


  bool      MDrawSlider   = true;
  MIP_Color MSliderColor  = MIP_Color(0.6);//MIP_COLOR_LIGHT_GRAY;
  MIP_Color MInteractiveSliderColor = MIP_Color(1.0);//MIP_COLOR_WHITE;


//------------------------------
public:
//------------------------------

  MIP_SliderWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0)
  : MIP_DragValueWidget(ARect,AText,AValue) {
  }

  //----------

  virtual ~MIP_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawSlider(bool ADraw=true)   { MDrawSlider = ADraw; }
  virtual void setSliderColor(MIP_Color AColor) { MSliderColor = AColor; }
  virtual void setInteractiveSliderColor(MIP_Color AColor) { MInteractiveSliderColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void drawSlider(MIP_PaintContext* AContext) {
    //MIP_Window* window = (MIP_Window*)getOwnerWindow();
    //double S = window->getWindowScale();
    MIP_DRect mrect = getRect();
    MIP_Painter* painter = AContext->painter;
    double value = getValue();

    MIP_Parameter* parameter = getParameter();
    if (parameter) {
      value = parameter->normalizeValue(value);
    }

    if (MIsInteracting) {
      MIP_Color color = MInteractiveSliderColor;
      if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
      painter->setFillColor(color);
    }
    else {
      MIP_Color color = MSliderColor;
      if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
      painter->setFillColor(color);
    }
    double x = mrect.x;
    double y = mrect.y;
    double w = mrect.w * value;//getValue();
    double h = mrect.h;
    painter->fillRect(x,y,w,h);
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    if (MDrawSlider) drawSlider(AContext);
    if (MDrawText) drawText(AContext);
    if (MDrawValue) drawValue(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);
  }


};

//----------------------------------------------------------------------
#endif





