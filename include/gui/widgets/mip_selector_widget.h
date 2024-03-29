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

  // hack?
  friend class MIP_MenuWidget;

//------------------------------
protected:
//------------------------------

  bool            MDrawArrow    = true;
  MIP_Color       MArrowColor   = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
  MIP_MenuWidget* MMenu         = nullptr;
  int32_t         MSelected     = -1;
  bool            MMenuIsOpen   = false;

//------------------------------
public:
//------------------------------

  MIP_SelectorWidget(MIP_DRect ARect, const char* AText, MIP_MenuWidget* AMenu)
  : MIP_TextWidget(ARect,AText) {
    //MName = "MIP_SelectorWidget";
    MMenu = AMenu;
    setCursor(MIP_CURSOR_FINGER);
    MTextAlignment = MIP_TEXT_ALIGN_LEFT;
    setNumParameters(1);
  }

  //----------

  virtual ~MIP_SelectorWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawArrow(bool ADraw=true)    { MDrawArrow = ADraw; }
  virtual void setArrowColor(MIP_Color AColor)  { MArrowColor = AColor; }

//------------------------------
public:
//------------------------------

  void setValue(double AValue) override {
    MIP_Widget::setValue(AValue);
    MSelected = AValue;
    //do_widget_update(this);
    //do_widget_redraw(this);
  }

  //----------

  void setValue(uint32_t AIndex, double AValue) override {
    MIP_Widget::setValue(AIndex,AValue);
    MSelected = AValue;
    //do_widget_update(this);
    //do_widget_redraw(this);
  }

  //----------

  double getValue(uint32_t AIndex=0) override {
    double value = MIP_Widget::getValue(AIndex);
    //MIP_Print("%.3f\n",value);
    return value;
  }

  //----------

  virtual void setSelected(int32_t AIndex) {
    //if (AIndex >= MChildren.size()) return;
    //MIP_Print("%i\n",AIndex);
    MSelected = AIndex;
    setValue(AIndex);
  }

  //----------

  void on_menu_selected(MIP_MenuWidget* AMenu, int32_t AIndex) override {
    if (AIndex >= 0) {
      MSelected = AIndex;
      setValue(MSelected);
      do_widget_update(this);
      do_widget_redraw(this);
    }
  }

//------------------------------
public:
//------------------------------

  //TODO: proportional positions, not pixels..
  // or better, MIP_SymbolWidget

  virtual void drawArrow(MIP_PaintContext* AContext) {
    MIP_DRect mrect = getRect();
    if (MDrawArrow) {
      MIP_Painter* painter = AContext->painter;
      //MIP_DRect rect = MRect;
      //rect.shrink(2);
      //painter->fontSize(MTextSize);
      double x1 = mrect.x2() - 2 - 8;
      double x2 = x1 + 4;
      double x3 = x1 + 8;
      double y1 = mrect.y2() - 5 - 6;
      double y2 = y1 + 6;
      painter->setFillColor(MArrowColor);
      double coords[6] = { x1,y1, x3,y1, x2,y2 };
      painter->fillLineStrip(3,coords);
    }
  }

//------------------------------
public: // parent to child
//------------------------------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    if (AButton == MIP_BUTTON_LEFT) {
      MMenu->setMenuListener(this);
      MMenu->open(AXpos,AYpos,true);
      MMenuIsOpen = true;
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

};

//----------------------------------------------------------------------
#endif



