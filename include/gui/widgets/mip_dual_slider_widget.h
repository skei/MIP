#ifndef mip_dual_slider_widget_included
#define mip_dual_slider_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_slider_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_DualSliderWidget
: public MIP_SliderWidget {

//------------------------------
private:
//------------------------------


  uint32_t MSelectedEdge = 0;


//------------------------------
protected:
//------------------------------


//  bool      MDrawSlider   = true;
//  MIP_Color MSliderColor  = MIP_COLOR_LIGHT_GRAY;
//  MIP_Color MISliderColor = MIP_COLOR_WHITE;


//------------------------------
public:
//------------------------------

  MIP_DualSliderWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0, double AValue2=0.0)
  : MIP_SliderWidget(ARect,AText,AValue) {
    setNumParameters(2);
    setValue(1,AValue2);
    setDragDirection(MIP_RIGHT);
    setValueSize(12);
    setValueOffset(MIP_DRect(5,0,5,0));
    setTextAlignment(MIP_TEXT_ALIGN_CENTER);
  }

  //----------

  virtual ~MIP_DualSliderWidget() {
  }

//------------------------------
public:
//------------------------------

//  virtual void setDrawSlider(bool ADraw=true)   { MDrawSlider = ADraw; }
//  virtual void setSliderColor(MIP_Color AColor) { MSliderColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void drawDualSlider(MIP_PaintContext* AContext) {

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
//    MIP_Print("param1 %p param2 %p v1 %f v2 %f\n",param1,param2,v1,v2);

    if (v1 > v2) {
      double temp = v1;
      v1 = v2;
      v2 = temp;
    }

    double mw   = mrect.w - S5;
    double p1 = mrect.x + (v1 * mw);
    double p2 = mrect.x + (v2 * mw);
    double w    = p2 - p1; // mrect.w * v1;

    w += S5;
    p2 += S5;

    //double x = mrect.x;
    double y = mrect.y;
    double h = mrect.h;

    painter->fillRect(p1,y,w,h);

    switch (MSelectedEdge) {
      case 1: {
        MIP_Color color = MIP_COLOR_WHITE;
        if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
        painter->setFillColor(color);
        painter->fillRect(p1,y,S5,h);
        break;
      }
      case 2: {
        MIP_Color color = MIP_COLOR_WHITE;
        if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
        painter->setFillColor(color);
        painter->fillRect(p2-S5,y,S5,h);
        break;
      }
    }

  }

  //----------

  virtual void drawDualValues(MIP_PaintContext* AContext) {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();

    MIP_DRect vo = MValueOffset;
    vo.scale(S);
    mrect.shrink(vo);

    MIP_Color color;
    if (MIsInteracting && (MSelectedEdge==1)) color = MIValueColor;
    else color = MValueColor;
    //MIP_Color color = MValueColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
    painter->setTextColor(color);

    painter->setTextSize(MValueSize * S);

    // value 1

    double value = getValue(0);
    char value_txt[33] = {0};
    MIP_Parameter* parameter = getParameter(0);

    if (parameter) parameter->valueToText(value,value_txt,32);
    else sprintf(value_txt,"%.3f",value);

    double bounds[4] = {0};
    painter->getTextBounds(value_txt,bounds);
    double x = mrect.x - bounds[0];
    double y = mrect.y - bounds[1];
    double w = bounds[2] - bounds[0];
    double h = bounds[3] - bounds[1];
    y += ((mrect.h - h) * 0.5);
    painter->drawText(x,y,value_txt);

    // value 2

    //MIP_Color color;
    if (MIsInteracting && (MSelectedEdge==2)) color = MIValueColor;
    else color = MValueColor;
    //MIP_Color color = MValueColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
    painter->setTextColor(color);

    value = getValue(1);
    parameter = getParameter(1);

    if (parameter) parameter->valueToText(value,value_txt,32);
    else sprintf(value_txt,"%.3f",value);

    //double bounds[4] = {0};
    painter->getTextBounds(value_txt,bounds);
    x = mrect.x - bounds[0];
    y = mrect.y - bounds[1];
    w = bounds[2] - bounds[0];
    h = bounds[3] - bounds[1];
    x = mrect.w - w + x;
    y += ((mrect.h - h) * 0.5);
    painter->drawText(x,y,value_txt);

  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    if (MDrawSlider) drawDualSlider(AContext);
    if (MDrawText) drawText(AContext);
    if (MDrawValue) drawDualValues(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);
  }

  //----------

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    MIP_DRect mrect = getRect();
    double S = window->getWindowScale();
    double S5 = S * 5.0;
    double mw = mrect.w - S5;

    double value1 = getValue(0);
    double value2 = getValue(1);
    double nv1 = value1;
    double nv2 = value2;

    MIP_Parameter* param1 = getParameter(0);
    MIP_Parameter* param2 = getParameter(1);
    if (param1) nv1 = param1->normalizeValue(nv1);
    if (param2) nv2 = param2->normalizeValue(nv2);

    if (isDragging()) {

//---

      // delta
      double deltax = AXpos - MPrevXpos;
      double deltay = AYpos - MPrevYpos;
      double minval = 0.0;
      double maxval = 1.0;
      MIP_Parameter* parameter = getParameter();
      if (parameter) {
        minval = parameter->getMinValue();
        maxval = parameter->getMaxValue();
      }
      double range = maxval - minval;
      if (range > 0) {
        //MIP_Print("range %f\n",range);
        double sens = MDragSensitivity;
        if (AState & MIP_KEY_SHIFT) sens *= MDragSensitivity2;
        sens *= range;
        deltax *= sens;
        deltay *= sens;
        // value

//        MIP_Parameter* param1 = getParameter(0);
//        MIP_Parameter* param2 = getParameter(1);

        switch (MSelectedEdge) {
          case 1: {
            switch (MDragDirection) {
              case MIP_LEFT:  value1 -= deltax;   break;
              case MIP_RIGHT: value1 += deltax;   break;
              case MIP_DOWN:  value1 += deltay;   break;
              case MIP_UP:    value1 -= deltay;   break;
            }
            value1 = MIP_Clamp(value1,minval,maxval);
            if (value1 > value2) {
              double temp = value1;
              value1 = value2;
              value2 = temp;
              MSelectedEdge = 2;
            }

            //setValue(0,value1);
//            if (param1) {
//              double nv1 = param1->denormalizeValue(value1);
//              setValue(0,nv1);
//            }
//            else {
              setValue(0,value1);
//            }

            break;
          }
          case 2: {
            switch (MDragDirection) {
              case MIP_LEFT:  value2 -= deltax;   break;
              case MIP_RIGHT: value2 += deltax;   break;
              case MIP_DOWN:  value2 += deltay;   break;
              case MIP_UP:    value2 -= deltay;   break;
            }
            value2 = MIP_Clamp(value2,minval,maxval);
            if (value2 < value1) {
              double temp = value1;
              value1 = value2;
              value2 = temp;
              MSelectedEdge = 1;
            }

            //setValue(1,value2);
//            if (param2) {
//              double nv2 = param2->denormalizeValue(value2);
//              setValue(1,nv2);
//            }
//            else {
              setValue(1,value2);
//            }

            break;
          }
        }

        // check if we need to swap?
        // also swap MSelectedEdge


        MPrevXpos = AXpos;
        MPrevYpos = AYpos;

        do_widget_update(this);
        do_widget_redraw(this);

      } // range

//---

    }
    else {

      //double p1 = mrect.x + (value1 * mrect.w);
      //double p2 = mrect.x + (value2 * mrect.w);

      double p1 = mrect.x + (nv1 * mw);
      double p2 = mrect.x + (nv2 * mw);

      double dist1 = fabs(p1 - AXpos);
      double dist2 = fabs(p2 - AXpos);
      uint32_t prevsel = MSelectedEdge;
      MSelectedEdge = 0;
      if (dist1 < dist2) {
        //MIP_Print("v1 dist %.2f\n",dist1);
        if (dist1 < (10.0*S)) MSelectedEdge = 1;
      }
      else {
        //MIP_Print("v2 dist %.2f\n",dist2);
        if (dist2 < (10.0*S)) MSelectedEdge = 2;
      }
      if (MSelectedEdge != prevsel) do_widget_redraw(this);
    }
  }

  //----------

  void on_widget_leave(double AXpos, double AYpos) override {
    if (MSelectedEdge != 0) {
      MSelectedEdge = 0;
      do_widget_redraw(this);
    }
  }


};

//----------------------------------------------------------------------
#endif






