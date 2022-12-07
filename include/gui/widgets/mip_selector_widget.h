#ifndef mip_selector_widget_included
#define mip_selector_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_SelectorWidget
: public MIP_TextWidget
, public MIP_MenuListener {

//------------------------------
protected:
//------------------------------

  bool            MDrawArrow    = true;
  MIP_Color       MArrowColor   = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
  MIP_MenuWidget* MMenu         = nullptr;

  int32_t         MSelected     = -1;

//------------------------------
public:
//------------------------------

  MIP_SelectorWidget(MIP_DRect ARect, const char* AText, MIP_MenuWidget* AMenu)
  : MIP_TextWidget(ARect,AText) {
    //MName = "MIP_SelectorWidget";
    MMenu = AMenu;
    setCursor(MIP_CURSOR_FINGER);
    MTextAlignment = MIP_TEXT_ALIGN_LEFT;
  }

  //----------

  virtual ~MIP_SelectorWidget() {
  }

//------------------------------
public:
//------------------------------

  void setValue(double AValue) override {
    MValue = AValue;
    MSelected = AValue;
    //do_widget_update(this);
    //do_widget_redraw(this);
  }

  double getValue() override {
    double value = MValue;
    //MIP_Print("%.3f\n",value);
    return value;
  }

  virtual void setSelected(int32_t AIndex) {
    //if (AIndex >= MChildren.size()) return;
    //MIP_Print("%i\n",AIndex);
    MSelected = AIndex;
    MValue = AIndex;
  }

  //----------

  void on_menu_selected(MIP_MenuWidget* AMenu, int32_t AIndex) override {
    if (AIndex >= 0) {
      MSelected = AIndex;
      MValue = MSelected;
      do_widget_update(this);
      do_widget_redraw(this);
    }
  }


//------------------------------
public: // parent to child
//------------------------------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (AButton == MIP_BUTTON_LEFT) {
      MMenu->setListener(this);
      MMenu->open(AXpos,AYpos,true);
    }
  }

  //----------

  void on_widget_paint(MIP_PaintContext* AContext) override {

    fillBackground(AContext);
    paintChildWidgets(AContext);

    //MIP_Print("MSelected %i\n",MSelected);

    if ((MSelected >= 0) && (MSelected < (int)MMenu->getNumChildWidgets())) {
      MIP_MenuItemWidget* mi = (MIP_MenuItemWidget*)MMenu->getChildWidget(MSelected);
      const char* name = mi->getText();
      //strcpy(MText,name);
      setText(name);
    }

    drawText(AContext);
    drawArrow(AContext);
    drawBorder(AContext);
  }



//------------------------------
public:
//------------------------------

  //TODO: proportional positions, not pixels..
  // or better, MIP_SymbolWidget

  virtual void drawArrow(MIP_PaintContext* AContext) {
//    MIP_DRect mrect = getRect();
    if (MDrawArrow) {
//      MIP_Painter* painter = AContext->painter;
      //MIP_DRect rect = MRect;
      //rect.shrink(2);
      //painter->fontSize(MTextSize);
//      double x1 = mrect.x2() - 5 - 6;
//      double x2 = x1 + 3;
//      double x3 = x1 + 6;
//      double y1 = mrect.y2() - 5 - 5;
//      double y2 = y1 + 5;
//      painter->beginPath();
//      //painter->drawTextBox(rect,MText,MTextAlignment,MTextColor);
//      painter->moveTo(x1,y1);
//      painter->lineTo(x3,y1);
//      painter->lineTo(x2,y2);
//      painter->closePath();
//      painter->fillColor(MArrowColor);
//      painter->fill();
    }
  }

};

//----------------------------------------------------------------------
#endif


