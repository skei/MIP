#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/mip_rect.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_Window
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  void* MWindowParent = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Window(int32_t AWidth, int32_t AHeight, void* AParent)
  : MIP_Widget(MIP_FRect(AWidth,AHeight)) {
    MIP_PRINT;
    MWindowParent = AParent;
  }

  //----------

  virtual ~MIP_Window() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  virtual void open() {
    MIP_PRINT;
  }

  //----------

  virtual void close() {
    MIP_PRINT;
  }

  //----------

  virtual void eventLoop() {
    MIP_PRINT;
  }

//------------------------------
public: // MIP_Widget
//------------------------------

  void do_widget_update(MIP_Widget* AWidget) override {
    MIP_PRINT;
  }

  //----------

  void do_widget_redraw(MIP_Widget* AWidget) override {
    MIP_PRINT;
    //TODO: widget->on_paint_..
  }


};

//----------------------------------------------------------------------
#endif
