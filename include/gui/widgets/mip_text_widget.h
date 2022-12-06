#ifndef mip_text_widget_included
#define mip_text_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_panel_widget.h"

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

  virtual const char* getText() { return MText; }

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

    painter->drawTextBox(mrect,MText,MTextAlignment);

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


