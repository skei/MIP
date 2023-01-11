#ifndef mip_knob_widget_included
#define mip_knob_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_drag_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_KnobWidget
: public MIP_DragValueWidget {

//------------------------------
private:
//------------------------------


//------------------------------
protected:
//------------------------------


  bool      MDrawArc                  = true;
  MIP_Color MArcBackColor             = MIP_COLOR_DARK_GRAY;
  MIP_Color MArcValueColor            = MIP_COLOR_LIGHT_GRAY;
  MIP_Color MInteractiveArcValueColor = MIP_COLOR_WHITE;
  double    MArcThickness             = 10.0;

  double    MModArcOffset             = 13.0;
  double    MModArcThickness          = 2.0;
  MIP_Color MModArcColor              = MIP_COLOR_BLACK;

//------------------------------
public:
//------------------------------

  MIP_KnobWidget(MIP_DRect ARect, const char* AText="%", double AValue=0.0)
  : MIP_DragValueWidget(ARect,AText,AValue) {

    setFillBackground(false);
    setDrawBorder(false);

    //setDrawValue(false);
    setDrawValue(true);
    setValueAlignment(MIP_TEXT_ALIGN_CENTER);
    setValueOffset(0);
    setValueSize(8);

    //setDrawText(false);
    setDrawText(true);
    setTextAlignment(MIP_TEXT_ALIGN_BOTTOM);
    setTextOffset(0);
    setTextSize(8);

    //setDrawIndicator(true);

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
  virtual void setInteractiveArcValueColor(MIP_Color AColor)  { MInteractiveArcValueColor = AColor; }
  virtual void setArcThickness(double AThickness)  { MArcThickness = AThickness; }

//------------------------------
public:
//------------------------------

  virtual void drawArc(MIP_PaintContext* AContext) {

    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_DRect mrect = getRect();
    MIP_Painter* painter = AContext->painter;

    double value = getValue();
    MIP_Parameter* param = getParameter();
    if (param) {
      value = param->normalizeValue(value);
    }

    double thickness  = MArcThickness * S;
    //double bthick = MArcThickness * S * 0.5;

    double cx = mrect.x + (mrect.w * 0.5);
    double cy = mrect.y + (mrect.h * 0.5);
    double r  = (mrect.w - thickness)  * 0.5;
    //double br = (mrect.w - bthick) * 0.5;

    // draw background arc

    MIP_Color color = MArcBackColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
    painter->setDrawColor(color);
    painter->setLineWidth(thickness);
    painter->drawArc(cx,cy,r,0.15*MIP_PI2,0.8*MIP_PI2);

    // draw value arc

    painter->setLineWidth(thickness);

    if (MIsInteracting) {
      MIP_Color color = MInteractiveArcValueColor;
      if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
      painter->setDrawColor(color);
    }
    else {
      MIP_Color color = MArcValueColor;
      if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
      painter->setDrawColor(color);
    }

    // a1 = start, from 3.oclock, counterclockwise
    // a2 = length of arc
    // curve is a1 'back' from end of a2


    if (MBipolar) {
      if (value < 0.5) {
        double vvv = 0.5 - value;
        double aa1 = (0.35 + (0.5 * 0.8)) * MIP_PI2;
        double aa2 = (       (vvv * 0.8)) * MIP_PI2;
        painter->drawArc(cx,cy,r,aa1,aa2);
      }
      else if (value > 0.5) {
        double vvv = (value - 0.5) * 2.0; // -> 0..1
        double aa1 = (0.75 + (vvv * 0.4)) * MIP_PI2;
        double aa2 = (       (vvv * 0.4)) * MIP_PI2;
        painter->drawArc(cx,cy,r,aa1,aa2);
      }
    }
    else {
      double a1 = (0.35 + (value * 0.8)) * MIP_PI2;
      double a2 = (       (value * 0.8)) * MIP_PI2;
      painter->drawArc(cx,cy,r,a1,a2);
    }

    // draw modulation

    if (MDrawModulation) {
      if (!isDisabled()) {

        painter->setLineWidth(MModArcThickness*S);
        painter->setDrawColor(MModArcColor);

        double modulation = MModulations[0];

        if (modulation < 0) {
          if ((value + modulation) < 0) modulation = - value;
          double aa1 = (0.35 + ((value)  * 0.8)) * MIP_PI2;
          double aa2 = (       ((-modulation) * 0.8)) * MIP_PI2;
          painter->drawArc(cx,cy,r - (MModArcOffset*S),aa1,aa2);
        }
        else if (modulation > 0) {
          if ((value + modulation) > 1) modulation = 1 - value;
          double aa1 = (0.35 + ((value+modulation)  * 0.8)) * MIP_PI2;
          double aa2 = (       ((modulation) * 0.8)) * MIP_PI2;
          painter->drawArc(cx,cy,r - (MModArcOffset*S),aa1,aa2);
        }
        //else {
        //}

      }
    }

    // draw indicator

    if (MDrawIndicator) {
      if (!isDisabled()) {
        if (MIndicatorValue >= 0.0) {
          double iv = MIndicatorValue + (0.02 * 0.5);
          double a1 = (0.35 + (iv * 0.8)) * MIP_PI2;
          double a2 =  0.02 * MIP_PI2;
          painter->setLineWidth(thickness);
          color = MIndicatorColor;
          painter->setDrawColor(color);
          painter->drawArc(cx,cy,r,a1,a2);
        }
      }
    }

  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    if (MDrawArc) drawArc(AContext);
    if (MDrawText) drawText(AContext);
    if (MDrawValue) drawValue(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);
  }


};

//----------------------------------------------------------------------
#endif






