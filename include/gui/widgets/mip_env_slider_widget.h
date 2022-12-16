#ifndef mip_env_slider_widget_included
#define mip_env_slider_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_dual_slider_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_EnvSliderWidget
: public MIP_DualSliderWidget {

//------------------------------
public:
//------------------------------

  MIP_EnvSliderWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0, double AValue2=0.0)
  : MIP_DualSliderWidget(ARect,AText,AValue,AValue2) {
    //setNumParameters(2);
    //setValue(1,AValue2);
    //setDragDirection(MIP_RIGHT);
    //setDragSensitivity( 1.0 / 400.0 );
    //setValueSize(12);
    //setValueOffset(MIP_DRect(5,0,5,0));
    //setTextAlignment(MIP_TEXT_ALIGN_CENTER);
  }

  //----------

  virtual ~MIP_EnvSliderWidget() {
  }

//------------------------------
public:
//------------------------------

  void drawDualSlider(MIP_PaintContext* AContext) override {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();
    double S5 = S * 5.0;
    if (MIsInteracting) {
      MIP_Color color = MISliderColor;
      if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
      painter->setFillColor(color);
    }
    else {
      MIP_Color color = MSliderColor;
      if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
      painter->setFillColor(color);
    }
    double v1 = getValue(0);
    double v2 = getValue(1);
    MIP_Parameter* param1 = getParameter(0);
    MIP_Parameter* param2 = getParameter(1);
    if (param1) v1 = param1->normalizeValue(v1);
    if (param2) v2 = param2->normalizeValue(v2);
    if (v1 > v2) {
      double temp = v1;
      v1 = v2;
      v2 = temp;
    }
    double mw = mrect.w - S5;
    double p1 = mrect.x + (v1 * mw);
    double p2 = mrect.x + (v2 * mw);
    double w  = p2 - p1;
    w += S5;
    p2 += S5;
    double y = mrect.y;
    double h = mrect.h;

    double lines[6] = {0};
    lines[0] = mrect.x;
    lines[1] = mrect.y2();
    lines[2] = p1;
    lines[3] = mrect.y;
    lines[4] = p1;
    lines[5] = mrect.y2();
    painter->fillLineStrip(3,lines);
    lines[0] = p2;
    lines[1] = mrect.y;
    lines[2] = mrect.x2();
    lines[3] = mrect.y2();
    lines[4] = p2;
    lines[5] = mrect.y2();
    painter->fillLineStrip(3,lines);

    painter->fillRect(p1,y,w,h);

    MIP_Color color;
    if (MIsInteracting) color = MIEdgeColor;
    else color = MEdgeColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
    painter->setFillColor(color);
    switch (MSelectedEdge) {
      case 1: {
        painter->fillRect(p1,y,S5,h);
        MLastMouseX = p1 + (S5 * 0.5);
        break;
      }
      case 2: {
        painter->fillRect(p2-S5,y,S5,h);
        MLastMouseX = p2 - (S5 * 0.5);
        break;
      }
    }
  }

//------------------------------
public:
//------------------------------

  //void on_widget_paint(MIP_PaintContext* AContext) override {
  //  if (MFillBackground) fillBackground(AContext);
  //  if (MDrawSlider) drawDualSlider(AContext);
  //  if (MDrawText) drawText(AContext);
  //  if (MDrawValue) drawDualValues(AContext);
  //  paintChildWidgets(AContext);
  //  if (MDrawBorder) drawBorder(AContext);
  //}

};

//----------------------------------------------------------------------
#endif







