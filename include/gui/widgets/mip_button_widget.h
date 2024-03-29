#ifndef mip_button_widget_included
#define mip_button_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_value_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ButtonWidget
: public MIP_ValueWidget {

//------------------------------
protected:
//------------------------------


//------------------------------
public:
//------------------------------

  MIP_ButtonWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0)
  : MIP_ValueWidget(ARect,AText,AValue) {
    setCursor(MIP_CURSOR_FINGER);
    setDrawValue(false);
  }

  //----------

  virtual ~MIP_ButtonWidget() {
  }

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        if (getValue() >= 0.5) setValue(0.0);
        else setValue(1.0);
        do_widget_update(this);
        do_widget_redraw(this);
        break;
      }
    }
  }

  //----------


};

//----------------------------------------------------------------------
#endif




