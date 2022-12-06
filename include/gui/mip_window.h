#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_paint_context.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_GUI_WIN32
  #include "gui/win32/mip_win32_window.h"
  typedef MIP_Win32Window MIP_ImplementedWindow;
#endif

#ifdef MIP_GUI_XCB
  #include "gui/xcb/mip_xcb_window.h"
  typedef MIP_XcbWindow MIP_ImplementedWindow;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow
, public MIP_WidgetListener {

//------------------------------
private:
//------------------------------

  /*
  MBackgroundWidget
  MRootWidget
  MOverlayWidget
  */

  MIP_Widget*       MRootWidget     = nullptr;
  MIP_Painter*      MWindowPainter  = nullptr;
  MIP_PaintContext  MPaintContext   = {};
  uint32_t          MInitialWidth   = 0;
  uint32_t          MInitialHeight  = 0;
  double            MWindowScale    = 1.0;

  MIP_Widget* MHoverWidget          = nullptr;
  MIP_Widget* MCapturedMouseWidget  = nullptr;
  MIP_Widget* MCapturedKeysWidget   = nullptr;

//------------------------------
protected:
//------------------------------

  bool        MFillBackground   = false;
  MIP_Color   MBackgroundColor  = MIP_COLOR_DARK_RED;

//------------------------------
public:
//------------------------------

  MIP_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : MIP_ImplementedWindow(AWidth,AHeight,AParent) {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
    MWindowPainter = new MIP_Painter(this);
  }

  //----------

  virtual ~MIP_Window() {
    delete MWindowPainter;
  }

//------------------------------
public:
//------------------------------

  virtual MIP_Widget* getRootWidget() {
    return MRootWidget;
  }

  //----------

  virtual MIP_Painter* getWindowPainter() {
    return MWindowPainter;
  }

  //----------

  virtual double getWindowScale() {
    return MWindowScale;
  }

  //----------

  virtual void setRootWidget(MIP_Widget* AWidget, MIP_WidgetListener* AListener=nullptr) {
    MRootWidget = AWidget;
    if (AListener) AWidget->setListener(AListener);
  }

  //----------

  virtual void setFillBackground(bool AFill=true) {
    MFillBackground = AFill;
  }

  //----------

  virtual void setBackgroundColor(MIP_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  virtual void setInitialSize(uint32_t AWidth, uint32_t AHeight) {
    MInitialWidth = AWidth;
    MInitialHeight = AHeight;
  }

  //----------

  /*
    called from:
      MIP_Editor.setParent
  */

  virtual void setWindowScale(double AScale) {
    MWindowScale = AScale;
    if (MRootWidget) MRootWidget->scaleChildWidgets(AScale);
  }

//------------------------------
public:
//------------------------------

  void open() override {
    MIP_ImplementedWindow::open();
    //startTimer(MIP_GUI_TIMER_MS,MIP_GUI_TIMER_ID);
    if (MRootWidget) MRootWidget->open(this);
  }

  //----------

  void close() override {
    if (MRootWidget) MRootWidget->close(this);  // crash...
    //stopTimer(MIP_GUI_TIMER_ID);
    MIP_ImplementedWindow::close();
  }

//------------------------------
public: // window
//------------------------------

  void on_window_open() override {
  }

  //----------

  void on_window_close() override {
  }

  //----------

  void on_window_move(int32_t AXpos, int32_t AYpos) override {
  }

  //----------

  /*
    see also: MIP_Editor::gui.set_size, MIP_Editor::adjustSize
    win32: called with MInitialWidth == 0
  */

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    MIP_Print("%i,%i\n",AWidth,AHeight);
    MWindowPainter->setClipRect(MIP_DRect(0,0,AWidth,AHeight));
    if (MInitialWidth > 0) {
      double s = (double)AWidth / (double)MInitialWidth;
      setWindowScale(s);
    }
    if (MRootWidget) {
      MRootWidget->setSize(AWidth,AHeight);
    }
  }

  //----------

  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {

//    MIP_Print("%i,%i,%i,%i\n",AXpos,AYpos,AWidth,AHeight);

    MIP_DRect updaterect = MIP_DRect(AXpos,AYpos,AWidth,AHeight);

    if (MRootWidget) {
      MPaintContext.painter = MWindowPainter;
      MPaintContext.updateRect = updaterect;

      MWindowPainter->beginPaint(0,0,MWindowWidth,MWindowHeight);

      //MWindowPainter->resetClip();
      //MWindowPainter->setClipRect(MIP_DRect(0,0,MWindowWidth,MWindowHeight));
      MWindowPainter->setClip(updaterect);

      if (MFillBackground) {
        MWindowPainter->setFillColor(MBackgroundColor);
        MWindowPainter->fillRect(AXpos,AYpos,AWidth,AHeight);
      }

      //MRootWidget->paintChildWidgets(&MPaintContext);
      MRootWidget->on_widget_paint(&MPaintContext);

      //MWindowPainter->resetClip();

      MWindowPainter->endPaint();

    }
  }

  //----------

  void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
    if (MCapturedKeysWidget) MCapturedKeysWidget->on_widget_key_press(AKey,AState,ATime);
  }

  //----------

  void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
    if (MCapturedKeysWidget) MCapturedKeysWidget->on_widget_key_release(AKey,AState,ATime);
  }

  //----------

  void on_window_mouse_click(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (MHoverWidget) {
      MCapturedMouseWidget = MHoverWidget;
      MHoverWidget->on_widget_mouse_click(AButton,AState,AXpos,AYpos,ATime);
    }
  }

  //----------

  void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (MCapturedMouseWidget) {
      MCapturedMouseWidget->on_widget_mouse_release(AButton,AState,AXpos,AYpos,ATime);
      MCapturedMouseWidget = nullptr;
      updateHoverWidget(AXpos,AYpos);
    }
  }

  //----------

  void on_window_mouse_dblclick(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (MCapturedMouseWidget) MCapturedMouseWidget->on_widget_mouse_dblclick(AButton,AState,AXpos,AYpos,ATime);
  }

  //----------

  void on_window_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (MCapturedMouseWidget) MCapturedMouseWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
    //else if (MModalMouseWidget) MModalMouseWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
    else {
      updateHoverWidget(AXpos,AYpos);
    }
  }

  //----------

  void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
    //if (!MCapturedMouseWidget) {
    //  updateHoverWidget(AXpos,AYpos);
    //}
  }

  //----------

  void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //MIP_PRINT;
    //if (!MCapturedMouseWidget) {
    //  if (MHoverWidget) MHoverWidget->on_widget_leave(AXpos,AYpos);
    //}
    //MHoverWidget = nullptr;
  }

  //----------

  void on_window_client_message(uint32_t AData) override {
  }

  //----------

  void on_window_timer() override {
    //MIP_Print("\n");
  }

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
  }

  void do_widget_redraw(MIP_Widget* AWidget, uint32_t AMode=0) override {
    MIP_DRect r = AWidget->getRect();
    invalidate(r.x,r.y,r.w,r.h);
  }

  void do_widget_set_cursor(MIP_Widget* AWidget, int32_t ACursor) override {
  }

  void do_widget_set_key_capture(MIP_Widget* AWidget, uint32_t AMode) override {
  }

  void do_widget_set_modal(MIP_Widget* AWidget, uint32_t AMode) override {
  }

  void do_widget_set_hint(MIP_Widget* AWidget, const char* AHint) override {
  }

  //void do_widget_set_timer(MIP_Widget* AWidget, uint32_t ATime) override {
  //}

  void do_widget_notify(MIP_Widget* AWidget, int32_t AValue) override {
  }

//------------------------------
private:
//------------------------------

  /*
    returns child/sub-widget at x,y
    // or self if no widget at that point
  */

  void updateHoverWidget(int32_t AXpos, int32_t AYpos) {
    if (MRootWidget) {
      MIP_Widget* hover = MRootWidget->findChildWidget(AXpos,AYpos,true);
      if (hover != MHoverWidget) {
        if (MHoverWidget) MHoverWidget->on_widget_leave(AXpos,AYpos);
        if (hover) hover->on_widget_enter(AXpos,AYpos);
      }
      MHoverWidget = hover;
    }
  }


};

//----------------------------------------------------------------------
#endif
