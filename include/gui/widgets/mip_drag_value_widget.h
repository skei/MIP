#ifndef mip_drag_value_widget_included
#define mip_drag_value_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_DragValueWidget
: public MIP_ValueWidget {

//------------------------------
private:
//------------------------------

  bool MIsDragging        = false;
  double MPrevXpos        = 0.0;
  double MPrevYpos        = 0.0;

//------------------------------
protected:
//------------------------------

  uint32_t  MDragDirection    = MIP_UP;
  double    MDragSensitivity  = (1.0 / 200.0);
  double    MDragSensitivity2 = (1.0 / 20.0);

//------------------------------
public:
//------------------------------

  MIP_DragValueWidget(MIP_DRect ARect, const char* AText="", double AValue=0.0)
  : MIP_ValueWidget(ARect,AText,AValue) {
  }

  //----------

  virtual ~MIP_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDragDirection(uint32_t ADir) {
    MDragDirection = ADir;
  }

  //----------

  virtual void setDragSensitivity(double s, double s2=0.05) {
    MDragSensitivity  = s;
    MDragSensitivity2 = s2;
  }

//------------------------------
public:
//------------------------------

  virtual void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        MIsDragging = true;
        MPrevXpos = AXpos;
        MPrevYpos = AYpos;
        break;
      }
    }
  }

  //----------

  virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        MIsDragging = false;
        break;
      }
    }
  }

  //----------

  virtual void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
    if (MIsDragging) {
      double deltax = AXpos - MPrevXpos;
      double deltay = AYpos - MPrevYpos;
      double sens = MDragSensitivity;
      if (AState & MIP_KEY_SHIFT) sens *= MDragSensitivity2;
      deltax *= sens;
      deltay *= sens;
      switch (MDragDirection) {
        case MIP_LEFT: {
          MValue -= deltax;
          break;
        }
        case MIP_RIGHT: {
          MValue += deltax;
          break;
        }
        case MIP_DOWN: {
          MValue += deltay;
          break;
        }
        case MIP_UP: {
          MValue -= deltay;
          break;
        }
      }
      MValue = MIP_Clamp(MValue,0,1);
      MPrevXpos = AXpos;
      MPrevYpos = AYpos;
      do_widget_update(this);
      do_widget_redraw(this);
    }
  }

  //----------

};

//----------------------------------------------------------------------
#endif




