#ifndef mip_button_row_widget_included
#define mip_button_row_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_grid_widget.h"

#define MIP_MAX_STATES 32

#define MIP_BUTTON_ROW_SINGLE  0
#define MIP_BUTTON_ROW_MULTI   1


class MIP_ButtonRowWidget
: public MIP_GridWidget {

//------------------------------
protected:
//------------------------------

  int32_t     MMode                   = 0;
  int32_t     MSelected               = 0;
  bool        MStates[MIP_MAX_STATES] = {0};
  const char* MLabels[MIP_MAX_STATES] = {0};

  MIP_Color   MTextColor              = MIP_Color(0.0);//MIP_COLOR_DARK_GRAY;
  MIP_Color   MActiveTextColor        = MIP_Color(0.0);//MIP_COLOR_BLACK;

  double      MTextSize               = 12.0;

  MIP_Color   MBackgroundCellColor    = MIP_Color(0.3);//MIP_COLOR_LIGHT_GRAY;
  MIP_Color   MActiveCellColor        = MIP_Color(0.5);//MIP_COLOR_GRAY;

  bool        MValueIsBits            = false;//true;
  uint32_t    MNumBits                = 8;//8;

  bool        MDrawRoundedBottom      = true;
  float       MRounded                = 8;

  bool MAllowZeroBits = true;

//------------------------------
public:
//------------------------------

  MIP_ButtonRowWidget(MIP_DRect ARect, int32_t ANum, const char** ATxt=nullptr, int32_t AMode=MIP_BUTTON_ROW_SINGLE)
  : MIP_GridWidget(ARect,ANum,1) {

    setNumParameters(1);

    //MName = "MIP_ButtonRowWidget";
    //setHint("buttonrow");
    setCursor(MIP_CURSOR_FINGER);
    MMode                 = AMode;
    MSelected             = 0;
    MDrawSelectedCells    = false;
    MDrawHorizontalLines  = false;
    MDrawVerticalLines    = false;

    if (ATxt) {
      for (int32_t i=0; i<ANum; i++) {
        MStates[i] = false;
        MLabels[i] = (char*)ATxt[i];
      }
    }

    setFillBackground(false);
    setBackgroundColor(0.6);
    setDrawBorder(false);
  }

  //----------

  virtual ~MIP_ButtonRowWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setTextColor(MIP_Color AColor)           { MTextColor = AColor; }
  virtual void setActiveTextColor(MIP_Color AColor)     { MActiveTextColor = AColor; }
  virtual void setBackgroundCellColor(MIP_Color AColor) { MBackgroundCellColor = AColor; }
  virtual void setActiveCellColor(MIP_Color AColor)     { MActiveCellColor = AColor; }

//------------------------------
public:
//------------------------------

  int32_t getSelected(void) {
    return MSelected;
  }

  void setValueIsBits(bool AIsBits=true, uint32_t ANumBits=8) {
    MValueIsBits = AIsBits;
    MNumBits = ANumBits;
  }

  void setDrawRoundedBottom(bool AState) {
    MDrawRoundedBottom = AState;
  }

  void setTextSize(double ASize) { MTextSize = ASize; }

  void setAllowZeroBits(bool AAllow=true) { MAllowZeroBits = AAllow; }

//------------------------------
public:
//------------------------------

  void setNumButtons(uint32_t ANum) {
    setNumColumns(ANum);
  }

  //----------

  bool getButtonState(int32_t i) {
    return MStates[i];
  }

  void setButtonState(int32_t i, bool s) {
    MStates[i] = s;
  }


  //----------

  uint32_t getButtonBits() {
    uint32_t bits = 0;
    for (uint32_t i=0; i<MNumBits; i++) {
      if (MStates[i] == true) bits |= (1 << i);
    }
    return bits;
  }

  //----------

  uint32_t getNumActiveBits() {
    uint32_t bits = 0;
    for (uint32_t i=0; i<MNumBits; i++) {
      if (MStates[i] == true) bits += 1;
    }
    return bits;
  }

  //----------

  void setButtonBits(uint32_t ABits) {
    for (uint32_t i=0; i<MNumBits; i++) {
      bool b = ( ABits & (1 << i) );
      MStates[i] = b;
    }
  }

  //----------

  const char* getButtonName(int32_t i) {
    return MLabels[i];
  }

  void setButtonName(int32_t i, const char* AName) {
    MLabels[i] = (char*)AName;
  }

  //----------

  double getValue(uint32_t AIndex=0) override {
    //MIP_PRINT;
    if (MValueIsBits) {
      return getButtonBits();
    }
    else {
      return MIP_GridWidget::getValue();
    }
  }

  //----------

  void setValue(double AValue) override {
    //MIP_Print("value %.3f\n",AValue);
    if (MValueIsBits) {
      int i = (int)AValue;
      setButtonBits(i);
      MIP_GridWidget::setValue(AValue);
    }
    else {
      MIP_GridWidget::setValue(AValue);
      selectValue(AValue);
    }
  }

  //----------

  void setValue(uint32_t AIndex,double AValue) override {
    setValue(AValue);
  }

  //----------

  void selectButton(int32_t index) {
    //MIP_Print("index %i\n",index);
    MSelected = index;

    if (MMode == MIP_BUTTON_ROW_SINGLE) {

      for (int32_t i=0; i<MNumColumns; i++) {
        if (i==MSelected) {
          MStates[i] = true;
//          MValues[0] = i;
        }
        else {
          MStates[i] = false;
        }
      }
      MValues[0] = MSelected;

//      float v = (float)MSelected / ((float)MNumColumns - 1.0f);
//      MIP_Widget::setValue(v);
    }
    else { // MULTI

      MStates[MSelected] = MStates[MSelected] ? false : true;
      if ( !MAllowZeroBits && (getNumActiveBits() == 0) ) {
        MStates[MSelected] = true;
      }
      //calcBitValue();
      MValues[0] = getButtonBits();

    }
  }

  //----------

  // value = 0..1

  void selectValue(float AValue) {
    //MIP_Print("value %.3f\n",AValue);
//    float num = AValue * MNumColumns;
//    num = MIP_Min(num,float(MNumColumns-1));
//    selectButton( (int)num );
    selectButton( (int)AValue );
  }

//------------------------------
public:
//------------------------------

  void on_clickCell(int32_t AX, int32_t AY, int32_t AB) override {
    //MIP_Print("AX %i\n",AX);
    if (AB == MIP_BUTTON_LEFT) {
      selectButton(AX);
      //setValue();
      do_widget_update(this);
      do_widget_redraw(this);
    }
  }

  //----------

  void on_paintCell(MIP_PaintContext* AContext, MIP_DRect ARect, int32_t AX, int32_t AY) override {
    char buf[256];
    MIP_Painter* painter= AContext->painter;
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();

    MIP_Color c1,c2;
    if (MStates[AX]) {
      c1 = MActiveCellColor;
      c2 = MBackgroundCellColor;
    }
    else {
      c1 = MBackgroundCellColor;
      c2 = MActiveCellColor;
    }

    if (MIndicatorValue >= 0.0) {
      int32_t ind = MIP_Trunc(MIndicatorValue) - 1;
      if (AX == ind) {
        c1.blend(MIndicatorColor,0.3);
      }
    }

    // background

    painter->setFillColor(c1);
    painter->fillRect(ARect.x,ARect.y,ARect.w,ARect.h);

    /*
    double b = ARect.h * 0.3;
    painter->beginPath();
    if (AX == 0) {
      painter->roundedRectVarying(ARect.x,ARect.y,ARect.w,ARect.h, b,0,0,b);
    }
    else if (AX == (MNumColumns-1)) {
      painter->roundedRectVarying(ARect.x,ARect.y,ARect.w,ARect.h, 0,b,b,0);
    }
    else {
      painter->rect(ARect.x,ARect.y,ARect.w,ARect.h);
    }
    MIP_PaintSource paint = painter->linearGradient(ARect.x,ARect.y,ARect.x,ARect.y2(), c1, c2);
    painter->fillPaint(paint);
    painter->fill();
    */

    // border

    painter->setDrawColor( MIP_Color(0.25) );
    painter->setLineWidth( 1.0 * S );
    painter->drawRect(ARect.x,ARect.y,ARect.w,ARect.h);

    /*
    painter->beginPath();
    if (AX == 0) {
      painter->roundedRectVarying(ARect.x,ARect.y,ARect.w,ARect.h, b,0,0,b);
    }
    else if (AX == (MNumColumns-1)) {
      painter->roundedRectVarying(ARect.x,ARect.y,ARect.w,ARect.h, 0,b,b,0);
    }
    else {
      painter->rect(ARect.x,ARect.y,ARect.w,ARect.h);
    }
    painter->strokeColor( MIP_Color(0.25); // MIP_COLOR_DARK_GRAY
    painter->strokeWidth(1);
    painter->stroke();
    */

    // text

    const char* txt = MLabels[AX];
    MIP_Color color = MTextColor;
    if (MStates[AX]) color = MActiveTextColor;
    else color = MTextColor;
    if (txt) {
      painter->setTextColor(color);
      painter->setTextSize(MTextSize * S);
      painter->drawTextBox(ARect,txt,MIP_TEXT_ALIGN_CENTER);
    }
    else {
      sprintf(buf,"%i",AX);
      painter->setTextColor(color);
      painter->setTextSize(MTextSize * S);
      painter->drawTextBox(ARect,buf,MIP_TEXT_ALIGN_CENTER);
    }

  }

};

#undef MIP_MAX_STATES

//----------------------------------------------------------------------
#endif
