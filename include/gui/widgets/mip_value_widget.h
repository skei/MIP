#ifndef mip_value_widget_included
#define mip_value_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ValueWidget
: public MIP_TextWidget {

//------------------------------
protected:
//------------------------------

  bool        MDrawValue      = true;
  double      MValueSize      = 14.0;
  MIP_Color   MValueColor     = MIP_COLOR_RED;//LIGHT_GRAY;
  //double      MValue          = 0.0;
  uint32_t    MValueAlignment = MIP_TEXT_ALIGN_RIGHT;
  MIP_DRect   MValueOffset    = MIP_DRect(0,0,5,0);

//------------------------------
public:
//------------------------------

  MIP_ValueWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0)
  : MIP_TextWidget(ARect,AText) {
    MValue          = AValue;
    MTextSize       = 14.0;
    MTextColor      = MIP_COLOR_DARK_GRAY;
    MTextAlignment  = MIP_TEXT_ALIGN_LEFT;
    MTextOffset     = MIP_DRect(5,0,0,0);
  }

  //----------

  virtual ~MIP_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValue(bool ADraw=true)           { MDrawValue = ADraw; }
  virtual void setValueSize(double ASize)              { MValueSize = ASize; }
  virtual void setValueColor(MIP_Color AColor)         { MValueColor = AColor; }
  //virtual void setValue(double AValue)                 { MValue = AValue; }
  virtual void setValueAlignment(uint32_t AAlignment)  { MValueAlignment = AAlignment; }
  virtual void setValueOffset(MIP_DRect AOffset)       { MValueOffset = AOffset; }

//------------------------------
public:
//------------------------------

  virtual void drawValue(MIP_PaintContext* AContext) {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();
    MIP_DRect vo = MValueOffset;
    vo.scale(S);
    mrect.shrink(vo);
    painter->setTextColor(MValueColor);
    painter->setTextSize(MValueSize * S);
    char temp[16] = {0};
    sprintf(temp,"%.3f",MValue);
    double bounds[4] = {0};
    painter->getTextBounds(temp,bounds);
    double x = mrect.x - bounds[0];
    double y = mrect.y - bounds[1];
    double w = bounds[2] - bounds[0];
    double h = bounds[3] - bounds[1];
    if      (MValueAlignment & MIP_TEXT_ALIGN_LEFT)        { }
    else if (MValueAlignment & MIP_TEXT_ALIGN_RIGHT)       { x = mrect.w - w + x; }
    else /*if (MValueAlignment & MIP_TEXT_ALIGN_CENTER)*/  { x += ((mrect.w - w) * 0.5); }
    if      (MValueAlignment & MIP_TEXT_ALIGN_TOP)         { }
    else if (MValueAlignment & MIP_TEXT_ALIGN_BOTTOM)      { y = mrect.h - h + y; }
    else /*if (MValueAlignment & MIP_TEXT_ALIGN_CENTER)*/  { y += ((mrect.h - h) * 0.5); }
    painter->drawText(x,y,temp);

  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    if (MDrawText) drawText(AContext);
    if (MDrawValue) drawValue(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);

  }

  //----------


};

//----------------------------------------------------------------------
#endif


