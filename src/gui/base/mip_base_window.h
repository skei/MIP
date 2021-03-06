#ifndef mip_base_window_included
#define mip_base_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/types/mip_color.h"
#include "gui/mip_drawable.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------

class MIP_BaseWindow
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  //void* MParentPtr = nullptr;

  bool      MFillBackground   = true;
  MIP_Color MBackgroundColor  = MIP_Color(0.5);

//------------------------------
public:
//------------------------------

  MIP_BaseWindow(int32_t AWidth, int32_t AHeight, const char* ATitle, void* AParentPtr)
  : MIP_Widget(MIP_FRect(AWidth,AHeight)) {
    //MParentPtr = AParentPtr;
  }

  //----------

  virtual ~MIP_BaseWindow() {
  }

//------------------------------
public:
//------------------------------

  void setFillBackground(bool AState=true)  { MFillBackground = true; }
  void setBackgroundColor(MIP_Color AColor) { MBackgroundColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void setPos(uint32_t AXpos, uint32_t AYpos) {}
  virtual void setSize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void setTitle(const char* ATitle) {}
  virtual void open() {}
  virtual void close() {}
  virtual void eventLoop() {}
  virtual void reparent(void* AParent) {}
  virtual void startTimer(uint32_t ms) {}
  virtual void stopTimer(void) {}
  virtual void setMouseCursor(uint32_t ACursor) {}
  virtual void setMouseCursorPos(int32_t AXpos, int32_t AYpos) {}
  virtual void hideMouseCursor(void) {}
  virtual void showMouseCursor(void) {}
  virtual void grabMouseCursor(void) {}
  virtual void releaseMouseCursor(void) {}
  virtual void beginPaint() {}
  virtual void paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void endPaint() {}
  virtual void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void sendEvent(uint32_t AData, uint32_t AType) {}
  virtual void flush(void) {}
  virtual void sync(void) {}

  virtual void fill(MIP_Color AColor) {}
  virtual void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, MIP_Color AColor) {}
  virtual void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource) {}
  virtual void blit(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}

//------------------------------
public:
//------------------------------

  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_open() {}
  virtual void on_window_close() {}
  virtual void on_window_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) {}
  virtual void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) {}
  virtual void on_window_timer() {}
  virtual void on_window_idle() {}
  virtual void on_window_clientMessage(uint32_t AData, void* APtr) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}

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
