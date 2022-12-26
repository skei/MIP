#ifndef mip_value_widget_included
#define mip_value_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_text_widget.h"

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
  double      MIValueSize     = 16.0;

  MIP_Color   MValueColor     = MIP_COLOR_DARK_GRAY;
  MIP_Color   MIValueColor    = MIP_COLOR_BLACK;

  //double      MValue          = 0.0;
  uint32_t    MValueAlignment = MIP_TEXT_ALIGN_RIGHT;
  MIP_DRect   MValueOffset    = MIP_DRect(0,0,5,0);

  int32_t     MNumDigits      = 2;

//------------------------------
public:
//------------------------------

  MIP_ValueWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0)
  : MIP_TextWidget(ARect,AText) {
    //MValue          = AValue;
    setNumParameters(1);
    MValues[0]      = AValue;
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

  virtual void setDrawValue(bool ADraw=true)          { MDrawValue = ADraw; }
  virtual void setValueSize(double ASize)             { MValueSize = ASize; }
  virtual void setValueColor(MIP_Color AColor)        { MValueColor = AColor; }
  virtual void setIValueColor(MIP_Color AColor)       { MIValueColor = AColor; }
  //virtual void setValue(double AValue)                { MValue = AValue; }
  virtual void setValueAlignment(uint32_t AAlignment) { MValueAlignment = AAlignment; }
  virtual void setValueOffset(MIP_DRect AOffset)      { MValueOffset = AOffset; }

  virtual void setNumDigits(int32_t ANum)             { MNumDigits = ANum; }

//------------------------------
public:
//------------------------------

  virtual void drawValue(MIP_PaintContext* AContext) {
    char fmt[16] = {0};

    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();

    double value = getValue();
    char value_txt[33] = {0};

    MIP_Parameter* parameter = getParameter();
    if (parameter) {
      //value = parameter->denormalizeValue(value);
      parameter->valueToText(value,value_txt,32);
    }
    else {
      fmt[0] = '%';
      fmt[1] = '.';
      fmt[2] = '0' + MNumDigits;
      fmt[3] = 'f';
      sprintf(value_txt,fmt/*"%.2f"*/,value);
    }

    MIP_DRect vo = MValueOffset;
    vo.scale(S);
    mrect.shrink(vo);

    MIP_Color color;
    if (MIsInteracting) color = MIValueColor;
    else color = MValueColor;
    if (isDisabled()) color.blend(MDisabledColor,MDisabledAlpha);

    painter->setTextColor(color);
    painter->setTextSize(MValueSize * S);
    //char temp[16] = {0};
    //sprintf(temp,"%.2f",getValue());

    double bounds[4] = {0};
    painter->getTextBounds(value_txt,bounds);
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
    painter->drawText(x,y,value_txt);

  }

//------------------------------
public:
//------------------------------

//  void on_widget_connect(MIP_Parameter* AParameter) override {
//    MIP_TextWidget::on_widget_connect(AParameter);
//    double value = AParameter->getValue();
//    setValue(value);
//    //const char* name = AParameter->getName();
//    //setText(name);
//  }


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



