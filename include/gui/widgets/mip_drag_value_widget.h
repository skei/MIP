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


//------------------------------
protected:
//------------------------------

  bool      MIsDragging       = false;
  double    MPrevXpos         = 0.0;
  double    MPrevYpos         = 0.0;

  uint32_t  MDragDirection    = MIP_UP;
  double    MDragSensitivity  = (1.0 / 200.0);
  double    MDragSensitivity2 = (1.0 / 20.0);

  double    MDragValue        = 0.0;

//  double    MMousePrevX       = 0.0;
//  double    MMousePrevY       = 0.0;

  bool        MSnap           = false;
  float       MSnapPos        = 0.5;
  float       MSnapDist       = 0.07;
  //uint32_t    MSnapMode       = 1;        // 0: always snap, 1: shift disables snapping

  bool        MQuantize       = false;
  bool        MBipolar        = false;


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

  //----------

  virtual bool isDragging() {
    return MIsDragging;
  }

  //----------

  virtual void setSnap(bool ASnap=true)             { MSnap = ASnap; }
  virtual void setSnapPos(float APos)               { MSnapPos = APos; }
  virtual void setSnapDist(float ADist)             { MSnapDist = ADist; }

  virtual void setQuantize(bool AQuantize=true)     { MQuantize = AQuantize; }
  virtual void setBipolar(bool ABipolar=true)       { MBipolar = ABipolar; }

  //----------

  void calcDelta(double AXpos, double AYpos, uint32_t AState, double* AXdelta, double* AYdelta) {
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
    }
    *AXdelta = deltax;
    *AYdelta = deltay;
  }

  //----------

  float snapValue(float AValue) {

    //float minval = getMinValue();
//    float maxval = getMaxValue();

    //double minval = 0.0;
    double maxval = 1.0;
    MIP_Parameter* parameter = getParameter();
    if (parameter) {
      //minval = parameter->getMinValue();
      maxval = parameter->getMaxValue();
    }

    float v = AValue;
    float s = 1.0f; // scale factor

    float dist = fabs( AValue - MSnapPos );

    if (dist < MSnapDist) {
      v = MSnapPos;
    }
    else {
      // scale left
      if (AValue < MSnapPos) {
        float sp_sd = MSnapPos - MSnapDist;
        if (sp_sd > 0) s = MSnapPos / sp_sd;
        v = AValue * s;
      }
      // scale right
      else if (AValue > MSnapPos) {
        float iv = maxval - AValue; // 1.0f - AValue;
        float isp = (maxval - MSnapPos); // (1.0f - MSnapPos);
        float isp_sd = isp - MSnapDist;
        if (isp_sd > 0) s = isp / isp_sd;
        v = iv * s;
        v = maxval - v; // 1.0f - v;
      }
    }
    //v = MIP_Clamp(v,minval,maxval); // (v,0.0f,1.0f);
    //MIP_Print("%.3f -> %.3f\n",AValue,v);
    return v;
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
        MDragValue = getValue();
        if (MAutoHideCursor) do_widget_set_cursor(this,MIP_CURSOR_HIDE);
        if (MAutoLockCursor) do_widget_set_cursor(this,MIP_CURSOR_LOCK);
        MIsInteracting = true;
        redraw();
        break;
      }
//      case MIP_BUTTON_RIGHT: {
//        if (AState & MIP_KEY_ALT) {
//          float v = getDefaultValue();
//          setValue(v);
//          do_widget_update(this);
//          do_widget_redraw(this);
//        }
//        break;
//      }
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

      // delta

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

//        // value
//
//        double value = getValue();
//
//        switch (MDragDirection) {
//          case MIP_LEFT: {
//            value -= deltax;
//            break;
//          }
//          case MIP_RIGHT: {
//            value += deltax;
//            break;
//          }
//          case MIP_DOWN: {
//            value += deltay;
//            break;
//          }
//          case MIP_UP: {
//            value -= deltay;
//            break;
//          }
//        }
//
//        //MIP_Print("minval %f maxval %f\n",minval,maxval);
//        value = MIP_Clamp(value,minval,maxval);
//        setValue(value);

        //

        //MDragValue += (ydiff * sens);

        switch (MDragDirection) {
          case MIP_LEFT: {
            MDragValue -= deltax;
            break;
          }
          case MIP_RIGHT: {
            MDragValue += deltax;
            break;
          }
          case MIP_DOWN: {
            MDragValue += deltay;
            break;
          }
          case MIP_UP: {
            MDragValue -= deltay;
            break;
          }
        }

        MDragValue = MIP_Clamp(MDragValue,minval,maxval);

        float v = MDragValue;
        if (MSnap && !(AState & MIP_KEY_SHIFT)) v = snapValue(v);
        v = MIP_Clamp(v,minval,maxval);
        setValue(v);

        //

        MPrevXpos = AXpos;
        MPrevYpos = AYpos;

        do_widget_update(this);
        do_widget_redraw(this);
      }

    } // range

  }

  //----------

//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
//    if (AState & MIP_KEY_SHIFT) {
//      MIP_Print("Shift\n");
//      MDragValue = getValue();
//    }
//  }

  //----------

//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
//    if (!(AState & MIP_KEY_SHIFT)) {
//      MIP_Print("not Shift\n");
//      MDragValue = getValue();
//    }
//  }

};

//----------------------------------------------------------------------
#endif




