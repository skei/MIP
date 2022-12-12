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

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    if (MDrawSlider) drawDualSlider(AContext);
    if (MDrawText) drawText(AContext);
    if (MDrawValue) drawValue(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);
  }

  //----------

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    if (!isDragging()) {
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

  void on_widget_leave(double AXpos, double AYpos) override {
    if (MSelectedEdge != 0) {
      MSelectedEdge = 0;
      do_widget_redraw(this);
    }
  }


};

//----------------------------------------------------------------------
#endif






