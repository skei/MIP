#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/mip_rect.h"
#include "gui/mip_widget.h"
#include "gui/base/mip_base_window.h"

//----------------------------------------------------------------------

#ifdef MIP_NO_GUI
  typedef MIP_BaseWindow MIP_ImplementedWindow;
#endif

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_window.h"
  typedef MIP_XcbWindow MIP_ImplementedWindow;
#endif



//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow {


//------------------------------
public:
//------------------------------

  MIP_Window(int32_t AWidth, int32_t AHeight, void* AParentPtr=nullptr)
  : MIP_ImplementedWindow(AWidth,AHeight,AParentPtr) {
    MIP_PRINT;
  }

  //----------

  virtual ~MIP_Window() {
    MIP_PRINT;
  }

//------------------------------
public: // MIP_Widget
//------------------------------

//  void do_widget_update(MIP_Widget* AWidget) override {
//    MIP_PRINT;
//  }
//
//  //----------
//
//  void do_widget_redraw(MIP_Widget* AWidget) override {
//    MIP_PRINT;
//    //TODO: widget->on_paint_..
//  }


};

//----------------------------------------------------------------------
#endif
