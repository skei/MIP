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

  bool      MIsDragging       = false;
  double    MPrevXpos         = 0.0;
  double    MPrevYpos         = 0.0;

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
    setCursor(MIP_CURSOR_ARROW_UP_DOWN);
    setAutoHideCursor(true);
    setAutoLockCursor(true);
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

  virtual bool isDragging() { return MIsDragging; }

//------------------------------
public:
//------------------------------

  virtual void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        MIsDragging = true;
        MPrevXpos = AXpos;
        MPrevYpos = AYpos;
        if (MAutoHideCursor) do_widget_set_cursor(this,MIP_CURSOR_HIDE);
        if (MAutoLockCursor) do_widget_set_cursor(this,MIP_CURSOR_LOCK);
        MIsInteracting = true;
        redraw();
        break;
      }
    }
  }

  //----------

  virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
    switch (AButton) {
      case MIP_BUTTON_LEFT: {
        MIsDragging = false;
        MIsInteracting = false;
        if (MAutoLockCursor) do_widget_set_cursor(this,MIP_CURSOR_UNLOCK);
        if (MAutoHideCursor) do_widget_set_cursor(this,MIP_CURSOR_SHOW);
        MIsInteracting = false;
        redraw();
        break;
      }
    }
  }

  //----------

  virtual void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
    if (MIsDragging) {
      double deltax = AXpos - MPrevXpos;
      double deltay = AYpos - MPrevYpos;

      double minval = 0.0;
      double maxval = 1.0;
      MIP_Parameter* parameter = getParameter();
      if (parameter) {
        minval = parameter->getMinValue();
        maxval = parameter->getMaxValue();
      }
      double range = maxval - minval;
      if (range > 0) {

        //MIP_Print("range %f\n",range);

        double sens = MDragSensitivity;
        if (AState & MIP_KEY_SHIFT) sens *= MDragSensitivity2;

        sens *= range;

        deltax *= sens;
        deltay *= sens;
        double value = getValue();
        switch (MDragDirection) {
          case MIP_LEFT: {
            value -= deltax;
            break;
          }
          case MIP_RIGHT: {
            value += deltax;
            break;
          }
          case MIP_DOWN: {
            value += deltay;
            break;
          }
          case MIP_UP: {
            value -= deltay;
            break;
          }
        }

        //MIP_Print("minval %f maxval %f\n",minval,maxval);
        value = MIP_Clamp(value,minval,maxval);

        setValue(value);
        MPrevXpos = AXpos;
        MPrevYpos = AYpos;
        do_widget_update(this);
        do_widget_redraw(this);
      }

    } // range
  }

  //----------

};

//----------------------------------------------------------------------
#endif




