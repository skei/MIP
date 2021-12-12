#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/mip_rect.h"

//----------------------------------------------------------------------

//#include <vector>
class MIP_Widget;
typedef std::vector<MIP_Widget*> MIP_Widgets;

//----------------------------------------------------------------------

class MIP_Widget {

//------------------------------
protected:
//------------------------------

  MIP_Widget* MParent       = nullptr;
  MIP_Widgets MChildren     = {};

  MIP_FRect MRect           = {};
  int32_t   MParameterIndex = -1;

//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_FRect ARect) {
    MRect = ARect;
  }

  //----------

  virtual ~MIP_Widget() {
  }

//------------------------------
public:
//------------------------------

  int32_t getParameterIndex()               { return -1; }
  void    setParameterIndex(int32_t AIndex) { MParameterIndex = AIndex; }
  float   getValue()                        { return 0.0; }

//------------------------------
public:
//------------------------------

  virtual void updateValue(float AValue) {}
  virtual void redraw() {}

//------------------------------
public:
//------------------------------

  virtual void on_move() {}
  virtual void on_resize() {}
  virtual void on_paint() {}
  virtual void on_mouseClick() {}
  virtual void on_mouseMove() {}
  virtual void on_mouseRelease() {}
  virtual void on_keyPress() {}
  virtual void on_keyRelease() {}
  virtual void on_connect(int32_t AParameterIndex) {}
  //virtual void on_timer() {}

//------------------------------
public:
//------------------------------

  virtual void do_widget_update(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_update(AWidget);
  }

  virtual void do_widget_redraw(MIP_Widget* AWidget) {
    if (MParent) MParent->do_widget_redraw(AWidget);
  }

};

//----------------------------------------------------------------------
#endif
