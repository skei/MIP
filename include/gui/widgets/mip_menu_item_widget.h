#ifndef mip_menu_item_widget_included
#define mip_menu_item_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_MenuItemWidget
: public MIP_TextWidget {

//------------------------------
protected:
//------------------------------

  MIP_Color MHighlightBackgroundColor = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
  MIP_Color MHighlightTextColor       = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;
  MIP_Color MNormalBackgroundColor    = MBackgroundColor;
  MIP_Color MNormalTextColor          = MTextColor;

//------------------------------
public:
//------------------------------

  MIP_MenuItemWidget(MIP_DRect ARect, const char* AText)
  : MIP_TextWidget(ARect,AText) {
    //MName = "MIP_MenuItemWidget";
    MDrawBorder = false;
    MFillBackground = true;
    //MFillGradient = false;
    MNormalBackgroundColor = MBackgroundColor;
    MNormalTextColor = MTextColor;
    setCursor(MIP_CURSOR_FINGER);
  }

  //----------

  virtual ~MIP_MenuItemWidget() {
  }

//------------------------------
public:
//------------------------------


//------------------------------
public: // parent to child
//------------------------------

  //void on_widget_paint(MIP_PaintContext* AContext) override {
  //  MIP_TextWidget::on_widget_paint(AContext);
  //}

  //----------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    switch (AButton) {
      case MIP_BUTTON_LEFT:
        do_widget_notify(this,MIP_WIDGET_NOTIFY_SELECT,getIndex());
        break;
      case MIP_BUTTON_RIGHT:
        do_widget_notify(this,MIP_WIDGET_NOTIFY_CANCEL,-1);
        break;
    }
    //do_widget_update(this);
  }

  //----------

  void on_widget_enter(double AXpos, double AYpos) override {
    MIP_TextWidget::on_widget_enter(AXpos,AYpos);
    MNormalBackgroundColor = MBackgroundColor;
    MNormalTextColor = MTextColor;
    MBackgroundColor = MHighlightBackgroundColor;
    MTextColor = MHighlightTextColor;
    do_widget_redraw(this);
  }

  //----------

  void on_widget_leave(double AXpos, double AYpos) override {
    MIP_TextWidget::on_widget_leave(AXpos,AYpos);
    MBackgroundColor = MNormalBackgroundColor;
    MTextColor = MNormalTextColor;
    do_widget_redraw(this);
  }


//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
