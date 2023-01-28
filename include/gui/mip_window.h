#ifndef mip_window_included
#define mip_window_included
//----------------------------------------------------------------------
/*

  this has to nbe seriously cleaned up..
  refactored..

  ---

  repainting:

  a) WM_PAINT/EXPOSE
  b) timer, 50hz
  c) do_widget update

*/
//----------------------------------------------------------------------

#include "base/mip.h"
#include "base/system/mip_lock.h"
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

class MIP_WindowListener {
public:
  virtual void do_window_listener_timer() {}
};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Window
: public MIP_ImplementedWindow
, public MIP_WidgetListener {

  // hack
  friend class MIP_Editor;
  friend class MIP_Plugin;

//------------------------------
private:
//------------------------------

  MIP_WindowListener* MWindowListener = nullptr;

  /*
  MBackgroundWidget
  MRootWidget
  MOverlayWidget
  */

  MIP_Widget*       MRootWidget           = nullptr;
  MIP_Painter*      MWindowPainter        = nullptr;
  MIP_PaintContext  MPaintContext         = {};
  uint32_t          MInitialWidth         = 0;
  uint32_t          MInitialHeight        = 0;
  double            MWindowScale          = 1.0;

  MIP_Widget*       MHoverWidget          = nullptr;
//MIP_Widget*       MCapturedMouseWidget  = nullptr;
//MIP_Widget*       MCapturedKeysWidget   = nullptr;
  MIP_Widget*       MCapturedWidget       = nullptr;
  MIP_Widget*       MMouseLockedWidget    = nullptr;
  MIP_Widget*       MModalWidget          = nullptr;

  int32_t           MMouseClickedX        = 0;
  int32_t           MMouseClickedY        = 0;
  uint32_t          MMouseClickedB        = 0;
  int32_t           MMousePreviousX       = 0;
  int32_t           MMousePreviousY       = 0;
  int32_t           MMouseDragX           = 0;
  int32_t           MMouseDragY           = 0;

  int32_t           MCurrentCursor        = MIP_CURSOR_DEFAULT;

  MIP_Queue<MIP_Widget*,MIP_WINDOW_MAX_DIRTY_WIDGETS> MDirtyWidgetsQueue = {};
  uint32_t          MDirtyIndex           = 0xffffffff;

  uint32_t          MRenderBufferWidth    = 0;
  uint32_t          MRenderBufferHeight   = 0;
  void*             MRenderBuffer         = nullptr;

  //MIP_Lock          MPaintLock            = {};

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
    //MIP_Assert(MWindowPainter);
    #ifdef MIP_WINDOW_BUFFERED
    if (MWindowPainter) {
        MWindowPainter->makeCurrent(0);
        // we create a buffer equal to the screen-size,
        // so we never have to resize it.. :-/
        createRenderBuffer(getScreenWidth(),getScreenHeight());
        //createRenderBuffer(AWidth,AHeight);
        MWindowPainter->resetCurrent(0);
        //MIP_Print("MRenderBuffer %p\n",MRenderBuffer);
    }
    else {
      MIP_Print("Error creating MWindowPainter\n");
    }
    #endif
  }

  //----------

  virtual ~MIP_Window() {
    //    PaintLock.lock();
    if (MWindowPainter) {
      #ifdef MIP_WINDOW_BUFFERED
      deleteRenderBuffer();
      #endif
    }
    delete MWindowPainter;
    //    MPaintLock.unlock();
  }

//------------------------------
public:
//------------------------------

  virtual void setWindowListener(MIP_WindowListener* AListener) {
    MWindowListener = AListener;
  }

  //----------

  virtual MIP_Widget* getRootWidget() {
    return MRootWidget;
  }

  //----------

  virtual MIP_Painter* getWindowPainter() {
    return MWindowPainter;
  }

  //----------

  virtual void* getRenderBuffer() {
    return MRenderBuffer;
  }

  //----------

  virtual double getWindowScale() {
    return MWindowScale;
  }

  //----------

  virtual void setRootWidget(MIP_Widget* AWidget, MIP_WidgetListener* AListener=nullptr) {
    MRootWidget = AWidget;
    if (AListener) AWidget->setWidgetListener(AListener);
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

  //----------

//  void paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
//    MWindowPainter->setClip(MIP_DRect(AXpos,AYpos,AWidth,AHeight));
//    MWindowPainter->setClipRect(MIP_DRect(AXpos,AYpos,AWidth,AHeight));
//    if (MFillBackground) {
//      MWindowPainter->setFillColor(MBackgroundColor);
//      MWindowPainter->fillRect(AXpos,AYpos,AWidth,AHeight);
//    }
//    MRootWidget->on_widget_paint(&MPaintContext);
//    MWindowPainter->resetClip();
//  }

  //----------

//  void paintWidget(MIP_Widget* AWidget, uint32_t AMode=0) {
//    MIP_DRect r = AWidget->getRect();
//    //queueDirtyRect(r.x,r.y,r.w,r.h);
//    //queueDirtyWidget(AWidget);
//    invalidate(r.x,r.y,r.w,r.h);
//  }

//------------------------------
public: // buffer
//------------------------------

  void createRenderBuffer(uint32_t AWidth, uint32_t AHeight) {
    //    MPaintLock.lock();
    //MIP_Print("AWidth %i AHeight %i\n",AWidth,AHeight);
    MIP_Assert(MWindowPainter);
    if (MWindowPainter) {
      MRenderBufferWidth = AWidth;
      MRenderBufferHeight = AHeight;
      MRenderBuffer = MWindowPainter->createRenderBuffer(AWidth,AHeight);
      //MIP_Print("MRenderBuffer: %p\n",MRenderBuffer);
      MIP_Assert(MRenderBuffer);
    }
    //    MPaintLock.unlock();
  }

  //----------

  void deleteRenderBuffer() {
    //MIP_PRINT;
    //    MPaintLock.lock();
    //MIP_Assert(MWindowPainter);
    if (MWindowPainter) {
      MIP_Assert(MRenderBuffer);
      MWindowPainter->deleteRenderBuffer(MRenderBuffer);
      MRenderBufferWidth = 0;
      MRenderBufferHeight = 0;
      MRenderBuffer = nullptr;
      //MIP_Print("MRenderBuffer: %p\n",MRenderBuffer);
    }
    //    MPaintLock.unlock();
  }

  //----------

  void resizeRenderBuffer(uint32_t AWidth, uint32_t AHeight) {
    //    MPaintLock.lock();
    //MIP_Print("AWidth %i AHeight %i\n",AWidth,AHeight);
    //MIP_Print("MRenderBufferWidth %i MRenderBufferHeight %i\n",MRenderBufferWidth,MRenderBufferHeight);
    if ((MRenderBufferWidth == AWidth) && (MRenderBufferHeight == AHeight)) {
      //MIP_Print("no change in size, returning..\n");
      return;
    }
    //deleteRenderBuffer();
    void* new_render_buffer = MWindowPainter->createRenderBuffer(AWidth,AHeight);
    //MIP_Print("new_render_buffer %p\n",new_render_buffer);
    MWindowPainter->deleteRenderBuffer(MRenderBuffer);
    //createRenderBuffer(AWidth,AHeight);
    MRenderBufferWidth = AWidth;
    MRenderBufferHeight = AHeight;
    MRenderBuffer = new_render_buffer;
    //    MPaintLock.unlock();
  }

//------------------------------
private: // dirty rects
//------------------------------

  // called by do_widget_redraw
  // gui (tweak knob)

  //void queueDirtyRect(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
  void queueDirtyWidget(MIP_Widget* AWidget) {
    //MIP_IRect rect = {AXpos,AYpos,AWidth,AHeight };
    //MDirtyRectsQueue.write(rect);
    MDirtyWidgetsQueue.write(AWidget);
  }

  //----------

  // called by on_window_timer

  //void flushDirtyRects() {
  void flushDirtyWidgets() {
    //MIP_IRect r = {0};
    //MIP_IRect dr = {0};
    //while (MDirtyRectsQueue.read(&dr)) r.combine(dr);
    //if (r.isNotEmpty()) invalidate(0,0,MWindowWidth,MWindowHeight);
    __MIP_UNUSED MIP_DRect frect = {0};
    __MIP_UNUSED MIP_DRect wrect = {0};
    MIP_Widget* widget = nullptr;
    uint32_t count = 0;
    while (MDirtyWidgetsQueue.read(&widget)) {
      wrect = widget->getRect();
      count += 1;
      #ifndef MIP_WINDOW_FULL_UPDATE_RECT
        #ifdef MIP_WINDOW_COMBINE_UPDATE_RECTS
          frect.combine( wrect );
        #else
          invalidate(wrect.x,wrect.y,wrect.w,wrect.h);
        #endif
      #endif
    }
    if (count > 0) {
      #ifdef MIP_WINDOW_FULL_UPDATE_RECT
        invalidate(0,0,MWindowWidth,MWindowHeight);
      #else
        #ifdef MIP_WINDOW_COMBINE_UPDATE_RECTS
          if (frect.isNotEmpty()) {
            invalidate(frect.x,frect.y,frect.w,frect.h);
          }
        #endif
      #endif
    }
  }

//------------------------------
public:
//------------------------------

  // we want to start the timer, and tell the rootwidget we're open

  void open() override {
    MIP_ImplementedWindow::open();
    //#ifdef MIP_WINDOW_TIMER_FLUSH_DIRTY_RECTS
      startTimer(MIP_WINDOW_TIMER_MS,MIP_WINDOW_TIMER_ID);
    //#endif
    if (MRootWidget) MRootWidget->open(this);
  }

  //----------

  // stop the timer we started in open()

  void close() override {
    if (MRootWidget) MRootWidget->close(this);  // crash...
    //#ifdef MIP_WINDOW_TIMER_FLUSH_DIRTY_RECTS
     // MIP_PRINT;
      stopTimer(MIP_WINDOW_TIMER_ID);
    //#endif
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
    //    uint32_t w2 = MIP_NextPowerOfTwo(AWidth);
    //    uint32_t h2 = MIP_NextPowerOfTwo(AHeight);
    //    if ((w2 =! MWindowWidth) || (h2 =! MWindowHeight)) {
    //      MIP_Print("TODO: resize buffer: %i, %i\n",w2,h2);
    //    }
    //    #ifdef MIP_WINDOW_BUFFERED
    //      resizeWindowBuffer(AWidth,AHeight);
    //    #endif
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

    MIP_Assert(MWindowPainter);
    MIP_DRect r = MIP_DRect(AXpos,AYpos,AWidth,AHeight);
    //queueDirtyRect(r.x,r.y,r.w,r.h);

//MPaintLock.lock();

//#if 0

    #ifdef MIP_WINDOW_BUFFERED
      MIP_Assert(MRenderBuffer);
      MWindowPainter->beginPaint(0,0,MWindowWidth,MWindowHeight,0);

      // draw to fbo

      MWindowPainter->selectRenderBuffer(MRenderBuffer);
      MWindowPainter->setViewport(0,0,MRenderBufferWidth,MRenderBufferHeight);
      MWindowPainter->beginFrame(MRenderBufferWidth,MRenderBufferHeight,1.0);

        MWindowPainter->setClip(MIP_DRect(r.x,r.y,r.w,r.h));
        MWindowPainter->setClipRect(MIP_DRect(r.x,r.y,r.w,r.h));

        //if (MFillBackground) {
        //  MWindowPainter->setFillColor(MBackgroundColor);
        //  MWindowPainter->fillRect(AXpos,AYpos,AWidth,AHeight);
        //}

        MIP_PaintContext pc;
        pc.painter = MWindowPainter;
        pc.updateRect = MIP_DRect(r.x,r.y,r.w,r.h);
        if (MRootWidget) MRootWidget->on_widget_paint(&pc);
        MWindowPainter->resetClip();

      MWindowPainter->endFrame();

      // draw fbo to window

      MWindowPainter->selectRenderBuffer(nullptr);
      MWindowPainter->setViewport(0,0,MWindowWidth,MWindowHeight);
      MWindowPainter->beginFrame(MWindowWidth,MWindowHeight,1.0);

        int image = MWindowPainter->getImageFromRenderBuffer(MRenderBuffer);
        MWindowPainter->setFillImage(image,r.x,r.y,1.0,1.0);
        MWindowPainter->fillRect(r.x,r.y,r.w,r.h);

      MWindowPainter->endFrame();
      MWindowPainter->endPaint(0);

    #else

      // draw to screen

      MWindowPainter->beginPaint(0,0,MWindowWidth,MWindowHeight,0);
      MWindowPainter->selectRenderBuffer(nullptr);
      MWindowPainter->setViewport(0,0,MWindowWidth,MWindowHeight);
      MWindowPainter->beginFrame(MWindowWidth,MWindowHeight,1.0);

        MWindowPainter->setClip(MIP_DRect(r.x,r.y,r.w,r.h));
        MWindowPainter->setClipRect(MIP_DRect(r.x,r.y,r.w,r.h));
        //if (MFillBackground) {
        //  MWindowPainter->setFillColor(MBackgroundColor);
        //  MWindowPainter->fillRect(AXpos,AYpos,AWidth,AHeight);
        //}
        MIP_PaintContext pc;
        pc.painter = MWindowPainter;
        pc.updateRect = MIP_DRect(r.x,r.y,r.w,r.h);
        if (MRootWidget) MRootWidget->on_widget_paint(&pc);
        MWindowPainter->resetClip();

      MWindowPainter->endFrame();

      //MWindowPainter->beginPaint(0,0,MWindowWidth,MWindowHeight,0);
      //MWindowPainter->makeCurrent(0);

      MWindowPainter->endPaint(0);

    #endif
//#endif // 0
//MPaintLock.unlock();

  }

  //----------

  void on_window_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
    if (MCapturedWidget) MCapturedWidget->on_widget_key_press(AKey,AState,ATime);
  }

  //----------

  void on_window_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
    if (MCapturedWidget) MCapturedWidget->on_widget_key_release(AKey,AState,ATime);
  }

  //----------

  void on_window_mouse_click(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (!MCapturedWidget) {
      MIP_Print("click\n");
      MMouseClickedX = AXpos;
      MMouseClickedY = AYpos;
      MMouseClickedB = AButton;
      MMouseDragX = AXpos;
      MMouseDragY = AYpos;
      if (MModalWidget) {
        if (AButton == MIP_BUTTON_LEFT) {
          if (!MModalWidget->getRect().contains(AXpos,AYpos)) {
            //MIP_PRINT;
            MModalWidget->on_widget_notify(0,0);
          }
        } // left
        if (AButton == MIP_BUTTON_RIGHT) {
          //MIP_PRINT;
          MModalWidget->on_widget_notify(0,0);
        } // right
      } // modal
      if (MHoverWidget) {
        //MMouseLockedWidget = MHoverWidget;
        MCapturedWidget = MHoverWidget;
        MHoverWidget->on_widget_mouse_click(AButton,AState,AXpos,AYpos,ATime);
      } // hover
    } // !captured
  }

  //----------

  void on_window_mouse_release(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //if (MMouseClicked) {
    //}
    if (MCapturedWidget) {
      if (MMouseClickedB == AButton) {
        MIP_Print("release\n");
        //MMouseLockedWidget = nullptr;
        MCapturedWidget->on_widget_mouse_release(AButton,AState,AXpos,AYpos,ATime);
        MCapturedWidget = nullptr;
        updateHoverWidget(AXpos,AYpos);
      }
    }
  }

  //----------

  void on_window_mouse_dblclick(uint32_t AButton, uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    if (MCapturedWidget) MCapturedWidget->on_widget_mouse_dblclick(AButton,AState,AXpos,AYpos,ATime);
  }

  //----------

  void on_window_mouse_move(uint32_t AState, int32_t AXpos, int32_t AYpos, uint32_t ATime) override {

    if (MMouseLockedWidget) { // todo: also if mouse_clicked?
      if ((AXpos == MMouseClickedX) && (AYpos == MMouseClickedY)) {
        MMousePreviousX = AXpos;
        MMousePreviousY = AYpos;
        return;
      }
      int32_t deltax = AXpos - MMouseClickedX;
      int32_t deltay = AYpos - MMouseClickedY;
      MMouseDragX += deltax;
      MMouseDragY += deltay;
      //setCursorPos(MMouseClickedX,MMouseClickedY);
      if (MCapturedWidget) {
        MCapturedWidget->on_widget_mouse_move(AState,MMouseDragX,MMouseDragY,ATime);
      }

      setCursorPos(MMouseClickedX,MMouseClickedY);

    }
    else {

      if (MModalWidget) {
        updateHoverWidget(AXpos,AYpos,MModalWidget); // only from modal widget and downwards?
        MModalWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
      }

      else if (MCapturedWidget) {
        MCapturedWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
      }
      else {
        updateHoverWidget(AXpos,AYpos);
        if (MHoverWidget) {
          //if (MHoverWidget->Options.wantHoverEvents) {
          MHoverWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
          //}
        }
      }
    }

    MMousePreviousX = AXpos;
    MMousePreviousY = AYpos;

    //if (MCapturedMouseWidget) MCapturedMouseWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
    ////else if (MModalMouseWidget) MModalMouseWidget->on_widget_mouse_move(AState,AXpos,AYpos,ATime);
    //else {
    //  updateHoverWidget(AXpos,AYpos);
    //}

  }

  //----------

  void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
    //if (!MCapturedMouseWidget) {
    //  updateHoverWidget(AXpos,AYpos);
    //}
  }

  //----------

  void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATime) override {
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
    if (MWindowListener) MWindowListener->do_window_listener_timer();
    if (!MWindowPainter) return;
    #ifdef MIP_WINDOW_BUFFERED
      if (!MRenderBuffer) return;
    #endif
    flushDirtyWidgets();

    //#ifdef MIP_WINDOW_FULL_UPDATE_RECT
    //  invalidate(0,0,MWindowWidth,MWindowHeight);
    //#else
    //  //flushDirtyRects();
    //  flushDirtyWidgets();
    //#endif

    //#ifdef MIP_WINDOW_BUFFERED
    //#endif

  }

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
  }

  //----------

  void do_widget_redraw(MIP_Widget* AWidget, uint32_t AMode=0) override {
    queueDirtyWidget(AWidget);
  }

  //----------

  void do_widget_set_cursor(MIP_Widget* AWidget, int32_t ACursor) override {
    switch (ACursor) {
      case MIP_CURSOR_LOCK:
        MMouseLockedWidget = AWidget;
        break;
      case MIP_CURSOR_UNLOCK:
        MMouseLockedWidget = nullptr;
        break;
      case MIP_CURSOR_SHOW:
        showCursor();
        setCursor(MCurrentCursor);
        break;
      case MIP_CURSOR_HIDE:
        hideCursor();
        break;
      default:
        if (ACursor != MCurrentCursor) {
          setCursor(ACursor);
          MCurrentCursor = ACursor;
        }
        break;
    }
  }

  //----------

  void do_widget_set_cursor_pos(MIP_Widget* AWidget, int32_t AXpos, int32_t AYpos) override {
    setCursorPos(AXpos,AYpos);
  }

  //----------

  void do_widget_set_capture(MIP_Widget* AWidget, uint32_t AMode=0) override {
    //MCapturedWidget = AWidget;
  }

  //----------

  void do_widget_set_modal(MIP_Widget* AWidget, uint32_t AMode=0) override {
    MModalWidget = AWidget;
  }

  //----------

  void do_widget_set_hint(MIP_Widget* AWidget, const char* AHint) override {
  }

  //----------

  //void do_widget_set_timer(MIP_Widget* AWidget, uint32_t ATime) override {
  //}

  //----------

  void do_widget_notify(MIP_Widget* AWidget, uint32_t AReason, int32_t AValue) override {
  }

//------------------------------
private:
//------------------------------

  /*
    returns child/sub-widget at x,y
    // or self if no widget at that point
  */

  void updateHoverWidget(int32_t AXpos, int32_t AYpos, MIP_Widget* ARoot=nullptr) {
    if (MRootWidget) {
      MIP_Widget* hover = nullptr;
      if (ARoot) hover = ARoot->findChildWidget(AXpos,AYpos,true);
      else
        hover = MRootWidget->findChildWidget(AXpos,AYpos,true);
      if (hover != MHoverWidget) {
        if (MHoverWidget) {
          MHoverWidget->setHovering(false);
          MHoverWidget->on_widget_leave(AXpos,AYpos);
        }
        if (hover) {
          hover->setHovering(true);
          hover->on_widget_enter(AXpos,AYpos);
        }
      }
      MHoverWidget = hover;
    }
  }

  //----------

  // when releasing mouse cursor after dragging
  // and entering window

  //void updateHoverWidgetFrom(MIP_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) {
  //  if (MHoverWidget != AFrom) {
  //    if (AFrom) AFrom->on_widget_leave(MHoverWidget,AXpos,AYpos,ATime);
  //    if (MHoverWidget) MHoverWidget->on_widget_enter(AFrom,AXpos,AYpos,ATime);
  //  }
  //}

};

//----------------------------------------------------------------------
#endif
