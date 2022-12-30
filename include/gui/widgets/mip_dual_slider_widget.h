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
protected:
//------------------------------

  double    MLastMouseX   = 0.0;
  double    MLastMouseY   = 0.0;
  uint32_t  MSelectedEdge = 0;

  MIP_Color MEdgeColor            = MIP_Color(1,1,1,0.5);
  MIP_Color MInteractiveEdgeColor = MIP_Color(1,1,1,0.7);

//  double    MDragValue2   = 0.0;


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
    setDragSensitivity( 1.0 / 400.0 );
    setValueSize(10);
    setValueOffset(MIP_DRect(5,0,5,0));
    setTextSize(10);
    setTextAlignment(MIP_TEXT_ALIGN_CENTER);
    setInteractiveValueColor(MIP_COLOR_BLACK);

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

//  void calc_left_right(double* x1, double* x2) {
//    MIP_DRect mrect = getRect();
//    double v1 = getValue(0);
//    double v2 = getValue(1);
//    MIP_Parameter* param1 = getParameter(0);
//    MIP_Parameter* param2 = getParameter(1);
//    if (param1) v1 = param1->normalizeValue(v1);
//    if (param2) v2 = param2->normalizeValue(v2);
//    if (v1 > v2) {
//      double temp = v1;
//      v1 = v2;
//      v2 = temp;
//    }
//    double p1 = mrect.x + (v1 * mrect.w);
//    double p2 = mrect.x + (v2 * mrect.w);
//    *x1 = p1;
//    *x2 = p2;
//  }

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
      MIP_Color color = MInteractiveSliderColor;
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

//    double lines[6] = {0};
//    lines[0] = mrect.x;
//    lines[1] = mrect.y2();
//    lines[2] = p1;
//    lines[3] = mrect.y;
//    lines[4] = p1;
//    lines[5] = mrect.y2();
//    painter->fillLineStrip(3,lines);
//    lines[0] = p2;
//    lines[1] = mrect.y;
//    lines[2] = mrect.x2();
//    lines[3] = mrect.y2();
//    lines[4] = p2;
//    lines[5] = mrect.y2();
//    painter->fillLineStrip(3,lines);

    painter->fillRect(p1,y,w,h);

    MIP_Color color;
    if (MIsInteracting) color = MInteractiveEdgeColor;
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

    // draw indicator

    if (!isDisabled()) {
      if (MIndicatorValue >= 0.0) {
        double v = MIndicatorValue; // 0..1
        //v *= mrect.w;
        v *= w;
        painter->setLineWidth( 2.0 * S);
        color = MIndicatorColor;
        painter->setDrawColor(color);
        painter->drawLine(p1+v,y,p1+v,mrect.y2());
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
    if (MIsInteracting && (MSelectedEdge==1)) color = MInteractiveValueColor;
    else color = MValueColor;
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
    if (MIsInteracting && (MSelectedEdge==2)) color = MInteractiveValueColor;
    else color = MValueColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
    painter->setTextColor(color);
    value = getValue(1);
    parameter = getParameter(1);
    if (parameter) parameter->valueToText(value,value_txt,32);
    else sprintf(value_txt,"%.3f",value);
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

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_SliderWidget::on_widget_mouse_click(AButton,AState,AXpos,AYpos,ATime);
    MLastMouseX = AXpos;
    MLastMouseY = AYpos;
//    MDragValue = getValue();
//    MDragValue2 = getValue(1);
  }

  //----------

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_SliderWidget::on_widget_mouse_release(AButton,AState,AXpos,AYpos,ATime);
    do_widget_set_cursor_pos(this,MLastMouseX,MLastMouseY);
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

//    double value1 = MDragValue;
//    double value2 = MDragValue2;

    double nv1 = value1;
    double nv2 = value2;
    MIP_Parameter* param1 = getParameter(0);
    MIP_Parameter* param2 = getParameter(1);
    if (param1) nv1 = param1->normalizeValue(nv1);
    if (param2) nv2 = param2->normalizeValue(nv2);
    if (isDragging()) {
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

//----------

        double sens = MDragSensitivity;
        if (AState & MIP_KEY_SHIFT) sens *= MDragSensitivity2;
        sens *= range;
        deltax *= sens;
        deltay *= sens;

//----------

        // value
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
            setValue(0,value1);
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
            setValue(1,value2);
            break;
          }
        }

//----------

        /*
        switch (MSelectedEdge) {
          case 1: {
            switch (MDragDirection) {
              case MIP_LEFT:  MDragValue -= deltax;   break;
              case MIP_RIGHT: MDragValue += deltax;   break;
              case MIP_DOWN:  MDragValue += deltay;   break;
              case MIP_UP:    MDragValue -= deltay;   break;
            }
            MDragValue = MIP_Clamp(MDragValue,minval,maxval);
            if (MDragValue > MDragValue2) {
              double temp = MDragValue;
              MDragValue = MDragValue2;
              MDragValue2 = temp;
              MSelectedEdge = 2;
            }
            float v = MDragValue;
            if (MSnap && !(AState & MIP_KEY_SHIFT)) v = snapValue(v);
            v = MIP_Clamp(v,minval,maxval);
            setValue(0,MDragValue);
            break;
          }
          case 2: {
            switch (MDragDirection) {
              case MIP_LEFT:  MDragValue2 -= deltax;   break;
              case MIP_RIGHT: MDragValue2 += deltax;   break;
              case MIP_DOWN:  MDragValue2 += deltay;   break;
              case MIP_UP:    MDragValue2 -= deltay;   break;
            }
            MDragValue2 = MIP_Clamp(MDragValue2,minval,maxval);
            if (MDragValue2 < MDragValue) {
              double temp = MDragValue;
              MDragValue = MDragValue2;
              MDragValue2 = temp;
              MSelectedEdge = 1;
            }
            float v = MDragValue2;
            if (MSnap && !(AState & MIP_KEY_SHIFT)) v = snapValue(v);
            v = MIP_Clamp(v,minval,maxval);
            setValue(1,MDragValue2);
            break;
          }
        }
        */

//----------

        // check if we need to swap? also swap MSelectedEdge
        MPrevXpos = AXpos;
        MPrevYpos = AYpos;
        do_widget_update(this);
        do_widget_redraw(this);
      } // range
    }
    else {
      double p1 = mrect.x + (nv1 * mw);
      double p2 = mrect.x + (nv2 * mw);
      double dist1 = fabs(p1 - AXpos);
      double dist2 = fabs(p2 - AXpos);
      uint32_t prevsel = MSelectedEdge;
      MSelectedEdge = 0;
      if (dist1 < dist2) {
        if (dist1 < (10.0*S)) MSelectedEdge = 1;
      }
      else {
        if (dist2 < (10.0*S)) MSelectedEdge = 2;
      }
      if (MSelectedEdge != prevsel) do_widget_redraw(this);
    }
  }

  //----------

  //void on_widget_enter(double AXpos, double AYpos) override {
  //  on_widget_mouse_move(0,AXpos,AYpos,0);
  //}

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






