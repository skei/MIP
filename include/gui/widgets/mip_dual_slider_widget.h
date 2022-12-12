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
    setValue(1,AValue2);
    setDragDirection(MIP_RIGHT);
    setValueSize(12);
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


    if (MIsInteracting) { painter->setFillColor(MISliderColor); }
    else { painter->setFillColor(MSliderColor); }

    double v1 = getValue(0);
    double v2 = getValue(1);

    if (v1 > v2) {
      double temp = v1;
      v1 = v2;
      v2 = temp;
    }

    double pos1 = mrect.x + (v1 * mrect.w);
    double pos2 = mrect.x + (v2 * mrect.w);

    //double x = mrect.x;
    double y = mrect.y;
    double w = pos2 - pos1; // mrect.w * v1;
    double h = mrect.h;

    painter->fillRect(pos1,y,w,h);

    switch (MSelectedEdge) {
      case 1: {
        painter->setFillColor(MIP_COLOR_WHITE);
        painter->fillRect(pos1,y,(5.0*S),h);
        break;
      }
      case 2: {
        painter->setFillColor(MIP_COLOR_WHITE);
        painter->fillRect(pos2-(5.0*S),y,(5.0*S),h);
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
    painter->setTextColor(MValueColor);
    painter->setTextSize(MValueSize * S);

    // value 1

    double value = getValue(0);
    char value_txt[33] = {0};

    MIP_Parameter* parameter = getParameter(0);
    if (parameter) {
      //value = parameter->denormalizeValue(value);
      parameter->valueToText(value,value_txt,32);
    }
    else {
      sprintf(value_txt,"%.3f",value);
    }

    double bounds[4] = {0};
    painter->getTextBounds(value_txt,bounds);
    double x = mrect.x - bounds[0];
    double y = mrect.y - bounds[1];
    double w = bounds[2] - bounds[0];
    double h = bounds[3] - bounds[1];

//    if      (MValueAlignment & MIP_TEXT_ALIGN_LEFT)       { }
//    //else if (MValueAlignment & MIP_TEXT_ALIGN_RIGHT)      { x = mrect.w - w + x; }
//    //else                                                  { x += ((mrect.w - w) * 0.5); }
//    //if      (MValueAlignment & MIP_TEXT_ALIGN_TOP)        { }
//    //else if (MValueAlignment & MIP_TEXT_ALIGN_BOTTOM)     { y = mrect.h - h + y; }
//    else                                                  { y += ((mrect.h - h) * 0.5); }

    y += ((mrect.h - h) * 0.5);

    painter->drawText(x,y,value_txt);

    // value 2

    value = getValue(1);

    parameter = getParameter(1);
    if (parameter) {
      //value = parameter->denormalizeValue(value);
      parameter->valueToText(value,value_txt,32);
    }
    else {
      sprintf(value_txt,"%.3f",value);
    }

    //double bounds[4] = {0};
    painter->getTextBounds(value_txt,bounds);
    x = mrect.x - bounds[0];
    y = mrect.y - bounds[1];
    w = bounds[2] - bounds[0];
    h = bounds[3] - bounds[1];

//    //if      (MValueAlignment & MIP_TEXT_ALIGN_LEFT)       { }
//    else if (MValueAlignment & MIP_TEXT_ALIGN_RIGHT)      { x = mrect.w - w + x; }
//    //else                                                  { x += ((mrect.w - w) * 0.5); }
//    //if      (MValueAlignment & MIP_TEXT_ALIGN_TOP)        { }
//    //else if (MValueAlignment & MIP_TEXT_ALIGN_BOTTOM)     { y = mrect.h - h + y; }
//    else                                                  { y += ((mrect.h - h) * 0.5); }

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
    //if (MDrawText) drawText(AContext);
    if (MDrawValue) drawDualValues(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);
  }

  //----------

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
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

        switch (MSelectedEdge) {
          case 1: {
            double value = getValue(0);
            switch (MDragDirection) {
              case MIP_LEFT:  value -= deltax;   break;
              case MIP_RIGHT: value += deltax;   break;
              case MIP_DOWN:  value += deltay;   break;
              case MIP_UP:    value -= deltay;   break;
            }
            value = MIP_Clamp(value,minval,maxval);
            setValue(0,value);
            break;
          }
          case 2: {
            double value = getValue(1);
            switch (MDragDirection) {
              case MIP_LEFT:  value -= deltax;   break;
              case MIP_RIGHT: value += deltax;   break;
              case MIP_DOWN:  value += deltay;   break;
              case MIP_UP:    value -= deltay;   break;
            }
            value = MIP_Clamp(value,minval,maxval);
            setValue(1,value);
            break;
          }
        }

        // check if we need to swap?
        // also swapp MSelectedEdge

        MPrevXpos = AXpos;
        MPrevYpos = AYpos;

        do_widget_update(this);
        do_widget_redraw(this);

      } // range

//---

    }
    else {
      MIP_DRect mrect = getRect();
      double v1 = getValue(0);
      double v2 = getValue(1);
      double pos1 = mrect.x + (v1 * mrect.w);
      double pos2 = mrect.x + (v2 * mrect.w);
      //MIP_Print("v1 %.2f v2 %.2f pos1 %.2f pos2 %.2f\n",v1,v2,pos1,pos2);
      double dist1 = fabs(pos1 - AXpos);
      double dist2 = fabs(pos2 - AXpos);
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






