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
  bool        MDrawParamText  = false;//true;
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

    const char* txt = MText;
    if (MDrawParamText) {
      MIP_Parameter* parameter = getParameter();
      if (parameter) {
        MText = parameter->getName();
        //double par_val = parameter->getValue();
        //char par_txt[17] = {0};
        //parameter->valueToText(par_val, par_txt, 16);
        //double value = parameter->normalizeValue(par_val);
      }
    }

    MIP_DRect to = MTextOffset;
    to.scale(S);
    mrect.shrink(to);

    MIP_Color color = MTextColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);
    painter->setTextColor(color);

    painter->setTextSize(MTextSize * S);

    painter->drawTextBox(mrect,txt,MTextAlignment);

  }

//------------------------------
public:
//------------------------------

//  virtual void on_widget_connect(MIP_Parameter* AParameter) {
//    MIP_PanelWidget::on_widget_connect(AParameter);
//    //double value = AParameter->getValue();
//    //setValue(value);
//      const char* name = AParameter->getName();
//      setText(name);
//  }

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


