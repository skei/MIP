#ifndef mip_base_window_included
#define mip_base_window_included
//----------------------------------------------------------------------

#include "gui/mip_paint_source.h"

class MIP_BaseWindow {

//------------------------------
public:
//------------------------------

  MIP_BaseWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {}
  virtual ~MIP_BaseWindow() {}

//------------------------------
public:
//------------------------------

  virtual void      initWidgets() {}

  virtual void      setPos(uint32_t AXpos, uint32_t AYpos) {}
  virtual void      setSize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void      getSize(uint32_t* AWidth, uint32_t* AHeight) { *AWidth = 0; *AHeight = 0; }
  virtual void      setTitle(const char* ATitle) {}

//  virtual double    getScale() { return 1.0; }

  virtual void      open() {}
  virtual void      close() {}

  virtual uint32_t  eventLoop() { return 0; }
  virtual void      startEventThread() {}
  virtual void      stopEventThread() {}

  virtual void      startTimer(uint32_t ms, uintptr_t id) {}
  virtual void      stopTimer(uintptr_t id)  {}

  virtual void      sendClientMessage(uint32_t AData, uint32_t AType) {}
  virtual void      invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void      reparent(intptr_t AParent) {}
  virtual void      beginPaint() {}
  virtual void      endPaint() {}

  virtual void      setCursor(int32_t ACursor) {}
  virtual void      setCursorPos(int32_t AXpos, int32_t AYpos) {}
  virtual void      hideCursor(void) {}
  virtual void      showCursor(void) {}
  virtual void      grabCursor(void) {}
  virtual void      releaseCursor(void) {}

  virtual void      fill(uint32_t AColor) {}
  virtual void      fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) {}
  virtual void      blit(int32_t ADstX, int32_t ADstY, void* AData, uint32_t AStride, int32_t ASrcW, int32_t ASrcH) {}
  virtual void      blit(int32_t ADstX, int32_t ADstY, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}
  virtual void      stretch(int32_t ADstX, int32_t ADstY, int32_t ADstW, int32_t ADstH, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}
  virtual void      blend(int32_t ADstX, int32_t ADstY, int32_t ADstW, int32_t ADstH, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}

//------------------------------
public:
//------------------------------

  virtual void on_window_open() {}
  virtual void on_window_close() {}
  virtual void on_window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void on_window_resize(int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {}
  virtual void on_window_mouse_click(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_dblclick(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void on_window_client_message(uint32_t AData) {}

  virtual void on_window_timer() {}

  //virtual void on_window_start_event_thread(uint32_t AMode=0) { MIP_PRINT; }
  //virtual void on_window_stop_event_thread(uint32_t AMode=0) { MIP_PRINT; }

};


//----------------------------------------------------------------------
#endif
