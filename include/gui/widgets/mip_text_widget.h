#ifndef mip_text_widget_included
#define mip_text_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_TextWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool        MDrawText       = true;
  double      MTextSize       = 14.0;
  MIP_Color   MTextColor      = MIP_COLOR_DARK_GRAY;
  const char* MText           = "";
  uint32_t    MTextAlignment  = MIP_TEXT_ALIGN_CENTER;
  MIP_DRect   MTextOffset     = MIP_DRect(0);

//------------------------------
public:
//------------------------------

  MIP_TextWidget(MIP_DRect ARect, const char* AText="")
  : MIP_PanelWidget(ARect) {
    MText = AText;
  }

  //----------

  virtual ~MIP_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawText(bool ADraw=true)           { MDrawText = ADraw; }
  virtual void setTextSize(double ASize)              { MTextSize = ASize; }
  virtual void setTextColor(MIP_Color AColor)         { MTextColor = AColor; }
  virtual void setText(const char* AText)             { MText = AText; }
  virtual void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }
  virtual void setTextOffset(MIP_DRect AOffset)       { MTextOffset = AOffset; }

//------------------------------
public:
//------------------------------

  virtual void drawText(MIP_PaintContext* AContext) {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();
    MIP_DRect to = MTextOffset;
    to.scale(S);
    mrect.shrink(to);
    painter->setTextColor(MTextColor);
    painter->setTextSize(MTextSize * S);
    double bounds[4] = {0};
    painter->getTextBounds(MText,bounds);
    double x = mrect.x - bounds[0];
    double y = mrect.y - bounds[1];
    double w = bounds[2] - bounds[0];
    double h = bounds[3] - bounds[1];
    if      (MTextAlignment & MIP_TEXT_ALIGN_LEFT)        { }
    else if (MTextAlignment & MIP_TEXT_ALIGN_RIGHT)       { x = mrect.w - w + x; }
    else /*if (MTextAlignment & MIP_TEXT_ALIGN_CENTER)*/  { x += ((mrect.w - w) * 0.5); }
    if      (MTextAlignment & MIP_TEXT_ALIGN_TOP)         { }
    else if (MTextAlignment & MIP_TEXT_ALIGN_BOTTOM)      { y = mrect.h - h + y; }
    else /*if (MTextAlignment & MIP_TEXT_ALIGN_CENTER)*/  { y += ((mrect.h - h) * 0.5); }
    painter->drawText(x,y,MText);

  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    if (MDrawText) drawText(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);

  }

  //----------


};

//----------------------------------------------------------------------
#endif


