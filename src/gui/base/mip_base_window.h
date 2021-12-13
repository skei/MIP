#ifndef mip_base_window_included
#define mip_base_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_BaseWindow
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  void* MParentPtr = nullptr;
//------------------------------
public:
//------------------------------

  MIP_BaseWindow(int32_t AWidth, int32_t AHeight, void* AParentPtr)
  : MIP_Widget(MIP_FRect(AWidth,AHeight)) {
    MParentPtr = AParentPtr;
  }

  //----------

  virtual ~MIP_BaseWindow() {
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
public:
//------------------------------

  virtual void on_window_open() {}
  virtual void on_window_close() {}

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
