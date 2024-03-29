#ifndef mip_win32_window_included
#define mip_win32_window_included
//----------------------------------------------------------------------

/*
  from: ??
  1. Set a flag when you see the WM_ENTERSIZEMOVE.
  2. Change your WM_SIZE handler to check the flag and do nothing if set.
  3. Change your WM_PAINT handler to check the flag and do a simple, fast
     fill of the window in a solid color if it's set.
  4. Clear the flag when you see WM_EXITSIZEMOVE, and then trigger your
     layout code and invalidate your window so that everything gets
     updated based on the final size.
  If your slow window is a child rather than your application's top-level
  window, you'll have to signal the child window when the top-level
  window gets the WM_ENTERSIZEMOVE and WM_EXITSIZEMOVE in order to
  implement steps 1 and 4.
*/

//-----

/*
  not working in wine: (?)
    window class styles:
    - CS_HREDRAW
    - CS_VREDRAW
    window styles:
    - WM_NCLBUTTONDOWN
    - WM_NCLBUTTONUP
    - WM_ENTERSIZEMOVE
    - WM_EXITSIZEMOVE
    - WM_SIZING
  works:
  - WM_SIZE
    (invalidate while area, and we get one paint message for entire window)
  - WM_GETMINMAXINFO
*/

//----------------------------------------------------------------------

#include "base/mip.h"
#include "base/utils/mip_strutils.h"
#include "gui/base/mip_base_window.h"
#include "gui/mip_paint_source.h"
#include "gui/mip_paint_target.h"

//----------------------------------------------------------------------

/*
  CS_OWNDC    Allocates a unique device context for each window in the class
  CS_HREDRAW  Redraws the entire window if a movement or size adjustment
              changes the width of the client area.
  CS_VREDRAW  Redraws the entire window if a movement or size adjustment
              changes the height of the client area.

*/

// h/v redraw doesn't seem to work in wine?
// i have to manually invalidate the entire window to get repaints..

#define MIP_WIN32_CLASS_STYLE         ( CS_OWNDC | CS_HREDRAW | CS_VREDRAW )
#define MIP_WIN32_STANDALONE_STYLE    ( WS_OVERLAPPEDWINDOW )
#define MIP_WIN32_EMBEDDED_STYLE      ( WS_CHILD | WS_VISIBLE )
#define MIP_WIN32_EX_STANDALONE_STYLE ( 0 /*WS_EX_OVERLAPPEDWINDOW*/ )
#define MIP_WIN32_EX_EMBEDDED_STYLE   ( 0 /* WS_EX_TOOLWINDOW */ )

//----------

char* MIP_Win32ClassName(void);
LRESULT CALLBACK mip_eventproc_win32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

const char* mip_win32_cursors[] = {
  IDC_ARROW,        // MIP_CURSOR_DEFAULT
  IDC_ARROW,        // MIP_CURSOR_ARROW
  IDC_UPARROW,      // MIP_CURSOR_ARROW_UP
  IDC_ARROW,        // MIP_CURSOR_ARROW_DOWN
  IDC_ARROW,        // MIP_CURSOR_ARROW_LEFT
  IDC_ARROW,        // MIP_CURSOR_ARROW_RIGHT
  IDC_SIZENS,       // MIP_CURSOR_ARROW_UP_DOWN
  IDC_SIZEWE,       // MIP_CURSOR_ARROW_LEFT_RIGHT
  IDC_SIZENWSE,     // MIP_CURSOR_ARROW_TOP_LEFT
  IDC_SIZENESW,     // MIP_CURSOR_ARROW_TOP_RIGHT
  IDC_SIZENESW,     // MIP_CURSOR_ARROW_BOTTOM_LEFT
  IDC_SIZENWSE,     // MIP_CURSOR_ARROW_BOTTOM_RIGHT
  IDC_ARROW,        // MIP_CURSOR_ARROW_LEFT_SIDE
  IDC_ARROW,        // MIP_CURSOR_ARROW_RIGHT_SIDE
  IDC_ARROW,        // MIP_CURSOR_ARROW_TOP_SIDE
  IDC_ARROW,        // MIP_CURSOR_ARROW_BOTTOM_SIDE
  IDC_SIZEALL,      // MIP_CURSOR_MOVE
  IDC_WAIT,         // MIP_CURSOR_WAIT
  IDC_APPSTARTING,  // MIP_CURSOR_ARROW_WAIT
  IDC_HAND,         // MIP_CURSOR_HAND
  IDC_HAND,         // MIP_CURSOR_FINGER
  IDC_CROSS,        // MIP_CURSOR_CROSS
  IDC_ARROW,        // MIP_CURSOR_CROSS2
  IDC_ARROW,        // MIP_CURSOR_PENCIL
  IDC_ARROW,        // MIP_CURSOR_PLUS
  IDC_HELP,         // MIP_CURSOR_QUESTION
  IDC_IBEAM,        // MIP_CURSOR_IBEAM
  IDC_ARROW,        // MIP_CURSOR_ARROW_INVALID
  IDC_ARROW,        // MIP_CURSOR_INVALID
  IDC_ARROW         // MIP_CURSOR_X
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Win32Window
: public MIP_BaseWindow
, public MIP_PaintSource
, public MIP_PaintTarget {

  friend LRESULT CALLBACK mip_win32_eventproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//------------------------------
private:
//------------------------------

  HWND        MHandle           = nullptr;
  HDC         MWinPaintDC       = nullptr;
  PAINTSTRUCT MWinPaintStruct   = {};
  HDC         MScreenDC         = nullptr;
  HDC         MWindowDC         = nullptr;

  intptr_t    MParent           = 0;
  int32_t     MAdjustedWidth    = 0;
  int32_t     MAdjustedHeight   = 0;

  uint32_t    MScreenWidth      = 0;
  uint32_t    MScreenHeight     = 0;
  int32_t     MScreenDepth      = 0;

  int32_t     MMouseXpos        = -1;
  int32_t     MMouseYpos        = -1;
  int32_t     MCurrentCursor    = -1;

  HCURSOR     MWinCursor        = nullptr;
  HCURSOR     MDefaultCursor    = nullptr;
  HCURSOR     MUserCursors[128] = {0};

//  double      MInitialWidth     = 0.0;
//  double      MInitialHeight    = 0.0;
//  double      MWidthScale       = 1.0;
//  double      MHeightScale      = 1.0;

//------------------------------
protected:
//------------------------------

  bool        MEmbedded         = false;

  int32_t     MWindowXpos       = 0;
  int32_t     MWindowYpos       = 0;
  int32_t     MWindowWidth      = 0;
  int32_t     MWindowHeight     = 0;
  const char* MWindowTitle      = "MIP_Win32Window";

  //bool        MFillBackground   = false;
  //uint32_t    MBackgroundColor  = 0x808080;


//------------------------------
public:
//------------------------------

  MIP_Win32Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : MIP_BaseWindow(AWidth,AHeight,AParent) {

    MWindowWidth  = AWidth;
    MWindowHeight = AHeight;
    MParent       = AParent;

    MScreenDC     = GetDC(0);
    MScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
    MScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    MScreenDepth  = GetDeviceCaps(MScreenDC,BITSPIXEL) * GetDeviceCaps(MScreenDC,PLANES);

    if (AParent == 0) {
      createStandaloneWindow(AWidth,AHeight);
    }
    else {
      createEmbeddedWindow(AWidth,AHeight,AParent);
    }

    SetWindowLongPtr(MHandle,GWLP_USERDATA,(LONG_PTR)this);

    setTitle(MWindowTitle);
    memset(MUserCursors,0,sizeof(MUserCursors));
    setCursor(MIP_CURSOR_DEFAULT);

  }

  //----------

  virtual ~MIP_Win32Window() {
    ReleaseDC(0,MScreenDC);
    ReleaseDC(MHandle,MWindowDC);
    DestroyWindow(MHandle);
  }

//------------------------------
private:
//------------------------------

  void createStandaloneWindow(uint32_t AWidth, uint32_t AHeight) {
    //MIP_Print("w %i h %i\n",AWidth,AHeight);
    MEmbedded     = false;
    RECT rect = { 0,0, (int)AWidth, (int)AHeight };
    AdjustWindowRectEx(&rect,MIP_WIN32_STANDALONE_STYLE,FALSE,MIP_WIN32_EX_STANDALONE_STYLE);
    int32_t width   = rect.right - rect.left; // AWidth;
    int32_t height  = rect.bottom - rect.top; // AHeight;
    //MIP_Print("adjusted: w %i h %i\n",width,height);
    // center window
    int32_t cx = ((MScreenWidth  - width) >> 1);
    int32_t cy = ((MScreenHeight - height) >> 1);
    MHandle = CreateWindowEx(
      MIP_WIN32_EX_STANDALONE_STYLE,
      MIP_Win32ClassName(),
      "",
      MIP_WIN32_STANDALONE_STYLE,
      cx,
      cy,
      width,  // + 1,
      height, // + 1,
      NULL,
      0,
      MIP_GLOBAL_WIN32_INSTANCE,
      0
    );
    MIP_Assert(MHandle);
    MWindowDC = GetDC(MHandle);
    SetFocus(MHandle);
  }

  //----------

  void createEmbeddedWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent) {
    //MIP_Print("w %i h %i\n",AWidth,AHeight);
    MEmbedded     = true;
    RECT rect = { 0,0, (int)AWidth, (int)AHeight };
    AdjustWindowRectEx(&rect,MIP_WIN32_EMBEDDED_STYLE,FALSE,MIP_WIN32_EX_EMBEDDED_STYLE);
    int32_t width   = rect.right - rect.left; // AWidth;
    int32_t height  = rect.bottom - rect.top; // AHeight;
    //MIP_Print("adjusted: w %i h %i\n",width,height);
    MHandle = CreateWindowEx(
      MIP_WIN32_EX_EMBEDDED_STYLE,
      MIP_Win32ClassName(),
      nullptr,                    // title
      MIP_WIN32_EMBEDDED_STYLE,
      0,                          // x
      0,                          // y
      width,
      height,
      (HWND)AParent,
      0,                          // menu
      MIP_GLOBAL_WIN32_INSTANCE,
      0                           // params
    );
    MIP_Assert(MHandle);
    MWindowDC = GetDC(MHandle);
    SetFocus(MHandle);
  }

  //----------

  /*
    AdjustWindowRectEx
    Calculates the required size of the window rectangle, based on the desired
    size of the client rectangle. The window rectangle can then be passed to
    the CreateWindowEx function to create a window whose client area is the
    desired size.

    - rect: A pointer to a RECT structure that contains the coordinates of the
      top-left and bottom-right corners of the desired client area. When the
      function returns, the structure contains the coordinates of the top-left
      and bottom-right corners of the window to accommodate the desired client
      area.
    - style: The window style of the window whose required size is to be
      calculated. Note that you cannot specify the WS_OVERLAPPED style.
    - menu:
    - ex style
  */

//------------------------------
public:
//------------------------------

  HWND  getHandle(void)       { return MHandle; }
  HDC   getWinPaintDC(void)   { return MWinPaintDC; }
  HDC   getScreenDC(void)     { return MScreenDC; }
  HDC   getWindowDC(void)     { return MWindowDC; }

  uint32_t getScreenWidth()   { return MScreenWidth; }
  uint32_t getScreenHeight()  { return MScreenHeight; }
  int32_t  getScreenDepth()   { return MScreenDepth; }

//------------------------------
public:
//------------------------------

//  virtual void    setFillBackground(bool AFill=true)  { MFillBackground = AFill; }
//  virtual void    setBackgroundColor(uint32_t AColor) { MBackgroundColor = AColor; }

//------------------------------
public: // paint source, target
//------------------------------

  bool    srcIsWindow()   override { return true; }
  int32_t srcGetWidth()   override { return MWindowWidth; }
  int32_t srcGetHeight()  override { return MWindowHeight; }
  int32_t srcGetDepth()   override { return MScreenDepth; }
  HWND    srcGetHWND()    override { return MHandle; }
  HDC     srcGetHDC()     override { return MWindowDC; }

  bool    tgtIsWindow()   override { return true; }
  int32_t tgtGetWidth()   override { return MWindowWidth; }
  int32_t tgtGetHeight()  override { return MWindowHeight; }
  int32_t tgtGetDepth()   override { return MScreenDepth; }
  HWND    tgtGetHWND()    override { return MHandle; }
  HDC     tgtGetHDC()     override { return MWindowDC; }
//HDC     tgtGetHDC()     override { return MWinPaintDC; }

//------------------------------
private: // painting
//------------------------------

  void flush(void) {
    GdiFlush();
  }

  //----------


//------------------------------
public: // MIP_BaseWindow
//------------------------------

  void setPos(uint32_t AXpos, uint32_t AYpos) override {
    MWindowXpos = AXpos;
    MWindowYpos = AYpos;
    SetWindowPos(MHandle,0,AXpos,AYpos,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
  }

  //----------

  void setSize(uint32_t AWidth, uint32_t AHeight) override {
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    SetWindowPos(MHandle,HWND_TOP,0,0,AWidth,AHeight, SWP_NOMOVE);
  }

  //----------

  void getSize(uint32_t* AWidth, uint32_t* AHeight) override {
    *AWidth = MWindowWidth;
    *AHeight = MWindowHeight;
  }

  //----------

  void setTitle(const char* ATitle) override {
    MWindowTitle = ATitle;
    SetWindowText(MHandle, ATitle);
  }


  //----------

  void open() override {
    on_window_open();
    ShowWindow(MHandle,SW_SHOW); // sende WM_SIZE
//    #ifdef MIP_GUI_IDLE_TIMER
//      startTimer(MIP_GUI_IDLE_TIMER_MS,MIP_GUI_IDLE_TIMER_ID);
//    #endif
  }

  //----------

  void close() override {
//    #ifdef MIP_GUI_IDLE_TIMER
//      stopTimer(MIP_GUI_IDLE_TIMER_ID);
//    #endif
    ShowWindow(MHandle,SW_HIDE);
    on_window_close();
  }

//------------------------------
public: // events
//------------------------------

  uint32_t eventLoop() override {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
    //while (GetMessage(&msg, MWinHandle,0,0)) {
      //uint32_t time = msg.time;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return 0;
  }

  //----------

  //virtual
  //void renderLoop(void) {
  //  MSG msg;
  //  while (true) {
  //    while (PeekMessage(&msg,nullptr,0,0,PM_REMOVE)) {
  //      TranslateMessage(&msg);
  //      DispatchMessage(&msg);
  //    }
  //    if (msg.message == WM_QUIT) break;
  //    if (MWindowListener) MWindowListener->on_windowRenderFrame(this);
  //  }
  //}

  //----------

  //void startEventThread() override {
  //}

  //----------

  //void stopEventThread() override {
  //}

  //----------

  // null -> timerproc
  //static void Timerproc( HWND unnamedParam1, UINT unnamedParam2, UINT_PTR unnamedParam3, DWORD unnamedParam4)   {}

  //----------

  void startTimer(uint32_t ms, uintptr_t id/*=MIP_GUI_IDLE_TIMER_ID*/) override {
    SetTimer(MHandle,id,ms,nullptr);
  }

  //----------

  // The KillTimer function does not remove WM_TIMER messages already posted
  // to the message queue.

  void stopTimer(uintptr_t id/*=MIP_GUI_IDLE_TIMER_ID*/)  override {
    KillTimer(MHandle,id);
  }

  //----------

  void sendClientMessage(uint32_t AData, uint32_t AType) override {
  }

  //----------

  /*
    The invalidated areas accumulate in the update region until the region is
    processed when the next WM_PAINT message occurs or until the region is
    validated by using the ValidateRect or ValidateRgn function.
    The system sends a WM_PAINT message to a window whenever its update region
    is not empty and there are no other messages in the application queue for
    that window.
    If the bErase parameter is TRUE for any part of the update region, the
    background is erased in the entire region, not just in the specified part.
  */

  /*
    The UpdateWindow function updates the client area of the specified window
    by sending a WM_PAINT message to the window if the window's update region
    is not empty. The function sends a WM_PAINT message directly to the window
    procedure of the specified window, bypassing the application queue. If the
    update region is empty, no message is sent.
  */

  /*
    The RedrawWindow function updates the specified rectangle or region in a
    window's client area.
  */

  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    RECT R;
    R.left   = AXpos;
    R.top    = AYpos;
    R.right  = AXpos + AWidth;  // - 1;
    R.bottom = AYpos + AHeight; // - 1;
    InvalidateRect(MHandle,&R,false);
    //UpdateWindow(MWinHandle);
    //RedrawWindow(MWinHandle,&R,NULL,RDW_UPDATENOW);
  }

  //----------

  /*
    AParent: A handle to the new parent window. If this parameter is NULL, the
    desktop window becomes the new parent window. If this parameter is
    HWND_MESSAGE, the child window becomes a message-only window.

    An application can use the SetParent function to set the parent window of a
    pop-up, overlapped, or child window.

    If the window identified by the hWndChild parameter is visible, the system
    performs the appropriate redrawing and repainting.

    For compatibility reasons, SetParent does not modify the WS_CHILD or
    WS_POPUP window styles of the window whose parent is being changed.
    Therefore:
    - if hWndNewParent is NULL, you should also clear the WS_CHILD bit
    and set the WS_POPUP style after (!!) calling SetParent.
    - Conversely, if hWndNewParent is not NULL and the window was previously a
    child of the desktop, you should clear the WS_POPUP style and set the
    WS_CHILD style before  (!!) calling SetParent.

    When you change the parent of a window, you should synchronize the UISTATE
    of both windows. For more information, see WM_CHANGEUISTATE and
    WM_UPDATEUISTATE.
  */

  virtual void reparent(intptr_t AParent) {
    LONG_PTR style   = GetWindowLongPtr(MHandle,GWL_STYLE);
    LONG_PTR exstyle = GetWindowLongPtr(MHandle,GWL_EXSTYLE);
    if (AParent) {
      //MIP_Print("reparenting %s -> EMBEDDED\n",MWindowType);
      MEmbedded = true;

      style   &= ~MIP_WIN32_STANDALONE_STYLE;
      style   |=  MIP_WIN32_EMBEDDED_STYLE;
      exstyle &= ~MIP_WIN32_EX_STANDALONE_STYLE;
      exstyle |=  MIP_WIN32_EX_EMBEDDED_STYLE;

      SetWindowLongPtr( MHandle, GWL_STYLE,   style   );
      SetWindowLongPtr( MHandle, GWL_EXSTYLE, exstyle );
      SetParent(MHandle,(HWND)AParent);

      RECT rect = { 0,0, MWindowWidth, MWindowHeight };
      AdjustWindowRectEx(&rect,MIP_WIN32_EMBEDDED_STYLE,FALSE,MIP_WIN32_EX_EMBEDDED_STYLE);
      int32_t width   = rect.right - rect.left;
      int32_t height  = rect.bottom - rect.top;
      setPos(0,0);
      setSize(width,height);

    }
    else {
      MIP_Print("unhandled: reparent to null (root)\n");
    }

  }

  /*
    The BeginPaint function prepares the specified window for painting and
    fills a PAINTSTRUCT structure with information about the painting.

    The BeginPaint function automatically sets the clipping region of the
    device context to exclude any area outside the update region. The update
    region is set by the InvalidateRect or InvalidateRgn function and by the
    system after sizing, moving, creating, scrolling, or any other operation
    that affects the client area.
  */

  void beginPaint() override {
    MWinPaintDC = BeginPaint(MHandle,&MWinPaintStruct);
  }

  //----------

  /*
    The EndPaint function marks the end of painting in the specified window.
    This function is required for each call to the BeginPaint function, but
    only after painting is complete.

    EndPaint releases the display device context that BeginPaint retrieved.
  */

  void endPaint() override {
    //flush();
    EndPaint(MHandle,&MWinPaintStruct);
    //UpdateWindow(MWinHandle);
    MWinPaintDC = nullptr;
  }

//------------------------------
public: // mouse
//------------------------------

  void setCursor(int32_t ACursor) override {
    if (ACursor != MCurrentCursor) {
      MCurrentCursor = ACursor;
      if (ACursor>=128) MWinCursor = MUserCursors[ACursor-128];
      else MWinCursor = LoadCursor(NULL,mip_win32_cursors[ACursor]);
      SetCursor(MWinCursor);
    }
  }

  //----------

  // Moves the cursor to the specified screen coordinates
  // will fire a WM_MOUSEMOVE event..

  void setCursorPos(int32_t AXpos, int32_t AYpos) override {
    POINT pos;
    pos.x = AXpos;
    pos.y = AYpos;
    ClientToScreen(MHandle,&pos);
    int32_t x = pos.x;
    int32_t y = pos.y;
    SetCursorPos(x,y);
  }

  //----------

  void hideCursor(void) override {
    ShowCursor(false);
  }

  //----------

  void showCursor(void) override {
    ShowCursor(true);
  }

  //----------

  void grabCursor(void) override {
    SetCapture(MHandle);
  }

  //----------

  void releaseCursor(void) override {
    ReleaseCapture();
  }

  //----------

  /*

    virtual // S3_Window_Base
    void createUserCursor(uint32 AIndex, int32 AWidth, int32 AHeight, const uint8* AAndMask, const uint8* AXorMask, int32 AXhotspot, int32 AYhotspot) {
      if ((AIndex>=128) && (AIndex<=255)) {
        HCURSOR cursor = CreateCursor(s3_global_WinInstance,AXhotspot,AYhotspot,AWidth,AHeight,AAndMask,AXorMask);
        MUserCursors[AIndex-128] = cursor;
      }
    }

    virtual // S3_Window_Base
    void destroyUserCursors(void) {
      for (uint32 i=0; i<128; i++) {
        HCURSOR cursor = MUserCursors[i];
        if (cursor) DestroyCursor(cursor);
      }
    }

    //virtual // S3_Window_Base
    //void getCursorPos(int32* AXpos, int32* AYpos) {
    //  *AXpos = 0;
    //  *AYpos = 0;
    //}

    virtual // S3_Window_Base
    void warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {
      RECT rc;
      POINT pt = { AXpos, AYpos };
      POINT pt2 = { AXpos+AWidth-1, AYpos+AHeight-1 };
      ClientToScreen(MWinHandle, &pt);
      ClientToScreen(MWinHandle, &pt2);
      SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);
      ClipCursor(&rc);
    }

    //----------

    virtual // S3_Window_Base
    void unwarpCursor(void) {
      ClipCursor(NULL);
    }

  */

//------------------------------
public: // paint
//------------------------------


  //----------

  // only valid inside begin/endPaint

  void fill(uint32_t AColor) override {
    fill(0,0,MWindowWidth,MWindowHeight,AColor);
  }

  //----------

  // only valid inside begin/endPaint

  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) override {
    RECT R;
    R.left    = AXpos;
    R.top     = AYpos;
    R.right   = AXpos + AWidth;
    R.bottom  = AYpos + AHeight;
    HPEN pen = CreatePen(PS_NULL,0,0);
    HGDIOBJ old_pen = SelectObject(MWinPaintDC,pen);
    HBRUSH brush = CreateSolidBrush(AColor);
    HGDIOBJ old_brush = SelectObject(MWinPaintDC,brush);
    Rectangle(MWinPaintDC,R.left,R.top,R.right+1,R.bottom+1);
    SelectObject(MWinPaintDC,old_pen);
    DeleteObject(pen);
    SelectObject(MWinPaintDC,old_brush);
    DeleteObject(brush);
  }

  //----------

  //BOOL BitBlt(
  //  HDC hdcDest,  // Handle to the destination device context.
  //  int nXDest,   // logical x-coordinate of the upper-left corner of the destination rectangle.
  //  int nYDest,   // logical y-coordinate of the upper-left corner of the destination rectangle.
  //  int nWidth,   // logical width of the source and destination rectangles.
  //  int nHeight,  // logical height of the source and the destination rectangles.
  //  HDC hdcSrc,   // Handle to the source device context.
  //  int nXSrc,    // logical x-coordinate of the upper-left corner of the source rectangle.
  //  int nYSrc,    // logical y-coordinate of the upper-left corner of the source rectangle.
  //  DWORD dwRop   // raster-operation code.
  //);

  //----------

  // only valid inside begin/endPaint

  void blit(int32_t ADstX, int32_t ADstY, void* AData, uint32_t AStride, int32_t ASrcW, int32_t ASrcH) override {
  }

  //----------

  /*
    a bitmap can't be attached to more than one dc at a time..
    and it's currently (probably) attached to the painter
    so, we would need to detach it from there first if we want to
    use this method..
  */

  //virtual void drawSurface(int32_t ADstX, int32_t ADstY, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {
  //  //HDC dst_dc = GetDC(MWinHandle);
  //  HDC tempdc = ASource->srcGetDC();
  //  BitBlt(MWindowDC,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,SRCCOPY);
  //  //ReleaseDC(MWinHandle,dst_dc);
  //}

  // only valid inside begin/endPaint

  void blit(int32_t ADstX, int32_t ADstY, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
    if (ASource->srcIsSurface()) {
      HDC tempdc = CreateCompatibleDC(MWinPaintDC);
      HBITMAP bmp = ASource->srcGetHBITMAP();
      HGDIOBJ old_bitmap = SelectObject(tempdc,bmp);
      BitBlt(MWinPaintDC,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,SRCCOPY);
      SelectObject(tempdc,old_bitmap);
      DeleteDC(tempdc);
    }
  }

  //----------

  //    //BOOL StretchBlt(
  //    //  __in  HDC hdcDest,
  //    //  __in  int nXOriginDest,
  //    //  __in  int nYOriginDest,
  //    //  __in  int nWidthDest,
  //    //  __in  int nHeightDest,
  //    //  __in  HDC hdcSrc,
  //    //  __in  int nXOriginSrc,
  //    //  __in  int nYOriginSrc,
  //    //  __in  int nWidthSrc,
  //    //  __in  int nHeightSrc,
  //    //  __in  DWORD dwRop
  //    //);
  //
  //    //virtual void stretchImage( axImage*  aImage,  int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
  //    virtual void stretchSurface( h_PaintSource* aSource, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
  //      {
  //        //HDC tempdc = (HDC)aImage->getHandle();
  //        HDC tempdc = (HDC)aSource->getDC();
  //        AlphaBlend(m_DC,aX,aY,aW,aH,tempdc,aSrcX,aSrcY,aSrcW,aSrcH,m_BlendFunc);
  //        // link with: libmsimg32
  //      }

  void stretch(int32_t ADstX, int32_t ADstY, int32_t ADstW, int32_t ADstH, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
    // StretchBlt
  }

  //----------

  //    //typedef struct _BLENDFUNCTION {
  //    //  BYTE BlendOp;
  //    //  BYTE BlendFlags;
  //    //  BYTE SourceConstantAlpha;
  //    //  BYTE AlphaFormat;
  //    //} BLENDFUNCTION, *PBLENDFUNCTION, *LPBLENDFUNCTION;
  //
  //    //virtual void renderImage( axImage*  aImage,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
  //    virtual void blendSurface( h_PaintSource* aSource,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
  //      {
  //        //HDC tempdc = (HDC)aImage->getHandle();
  //        HDC tempdc = /*(HDC)*/aSource->getDC();
  //        AlphaBlend(m_DC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,aSrcW,aSrcH,m_BlendFunc);
  //        // link with: libmsimg32
  //      }

  void blend(int32_t ADstX, int32_t ADstY, int32_t ADstW, int32_t ADstH, MIP_PaintSource* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) override {
    // AlphaBlend
  }

//------------------------------
private: // remap
//------------------------------

  int32_t remapMouseButton(int32_t AButton) {
    int32_t result = MIP_BUTTON_NONE;
    switch(AButton) {
      case MK_LBUTTON:      result = MIP_BUTTON_LEFT;   break;
      case MK_MBUTTON:      result = MIP_BUTTON_MIDDLE; break;
      case MK_RBUTTON:      result = MIP_BUTTON_RIGHT;  break;
    //case MK_XBUTTON1:     result = smb_x1;            break;
    //case MK_XBUTTON2:     result = smb_x2;            break;
    //case WM_LBUTTONDOWN:  result = smb_left;          break;
    //case WM_MBUTTONDOWN:  result = smb_middle;        break;
    //case WM_RBUTTONDOWN:  result = smb_right;         break;
    //case 4:               result = smb_wheelUp;       break;
    //case 5:               result = smb_wheelDown;     break;
    //case 8:               result = smb_backward;      break;
    //case 9:               result = smb_forward;       break;
    }
    return result;
  }

  //----------

  int32_t remapMouseKey(int32_t AState) {
    int32_t result = MIP_KEY_NONE;
    if ((AState & MK_SHIFT  ) != 0) result |= MIP_KEY_SHIFT;
    //if ((AState & ) != 0) result |= MIP_KEY_CAPS;
    if ((AState & MK_CONTROL  ) != 0) result |= MIP_KEY_CTRL;
    //if ((AState & ) != 0) result |= MIP_KEY_ALT;
    //if ((AState & 128) != 0) result |= MIP_KEY_ALTGR;
    return result;
  }

  //----------

  /*
  WM_KEYDOWN

  wParam  The virtual-key code of the nonsystem key. See Virtual-Key Codes.
  lParam  The repeat count, scan code, extended-key flag, context code,
  previous key-state flag, and transition-state flag, as shown following.

  Bits	Meaning
  0-15  The repeat count for the current message. The value is the number of
        times the keystroke is autorepeated as a result of the user holding
        down the key. If the keystroke is held long enough, multiple messages
        are sent. However, the repeat count is not cumulative.
  16-23 The scan code. The value depends on the OEM.
  24    Indicates whether the key is an extended key, such as the right-hand
        ALT and CTRL keys that appear on an enhanced 101- or 102-key keyboard.
        The value is 1 if it is an extended key; otherwise, it is 0.
  25-28 Reserved; do not use.
  29    The context code. The value is always 0 for a WM_KEYDOWN message.
  30    The previous key state. The value is 1 if the key is down before the
        message is sent, or it is zero if the key is up.
  31    The transition state. The value is always 0 for a WM_KEYDOWN message.
  */

  /*
  )int WINAPI ToAscii(
    _In_           UINT   uVirtKey,
    _In_           UINT   uScanCode,
    _In_opt_ const BYTE   *lpKeyState,
    _Out_          LPWORD lpChar,
    _In_           UINT   uFlags
  );
  */

  int32_t remapKeyCode(WPARAM wParam, LPARAM lParam) {
    switch(wParam) {
     case VK_BACK: return MIP_KEY_BACKSPACE;
    }
    return 0;
  }

  //----------

  LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    // NULL for mouse events?
    //if (hWnd != MWinHandle) {
    //  //MIP_Print("wrong window\n");
    //  return DefWindowProc(hWnd,message,wParam,lParam);
    //}

    uint32_t time = GetMessageTime();
    //MIP_Print("GetMessageTime(): %i\n",time);

    LRESULT result = 0;
    switch (message) {

      //-----

      case WM_DESTROY: {
        if (!MEmbedded) PostQuitMessage(0);
        break;
      }

      //-----

      case WM_GETDLGCODE: {
        // we want keypresses
        result = DLGC_WANTALLKEYS;
        break;
      }

      //-----

      case WM_ERASEBKGND: {
        // tell window not to erase background
        result = 1;
        break;
      }

      //-----

      case WM_SETCURSOR: {
        // called every mouse move
        if (LOWORD(lParam)==HTCLIENT) {
          SetCursor(MWinCursor);
          //setCursor(MCurrentCursor);
          result = 1;
        }
        else result = DefWindowProc(hWnd,message,wParam,lParam);
        break;
      }

      //-----

      /*
        Sent to a window when the size or position of the window is about to
        change. An application can use this message to override the window's
        default maximized size and position, or its default minimum or maximum
        tracking size.

        typedef struct tagMINMAXINFO {
          POINT ptReserved;
          POINT ptMaxSize;
          POINT ptMaxPosition;
          POINT ptMinTrackSize;
          POINT ptMaxTrackSize;
        } MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;
      */

      //case WM_GETMINMAXINFO: {
      //  //MIP_Print("WM_GETMINMAXINFO\n");
      //  //MINMAXINFO* info = (MINMAXINFO*)lParam;
      //  break;
      //}

      //-----

      /*
        Sent one time to a window after it enters the moving or sizing modal
        loop. The window enters the moving or sizing modal loop when the user
        clicks the window's title bar or sizing border, or when the window
        passes the WM_SYSCOMMAND message to the DefWindowProc function and the
        wParam parameter of the message specifies the SC_MOVE or SC_SIZE value.
        The operation is complete when DefWindowProc returns.
        The system sends the WM_ENTERSIZEMOVE message regardless of whether the
        dragging of full windows is enabled.
      */

      // not in wine..
      //case WM_ENTERSIZEMOVE: {
      //  MIP_Print("WM_ENTERSIZEMOVE\n");
      //  break;
      //}

      //-----

      /*
        Sent one time to a window, after it has exited the moving or sizing
        modal loop. The window enters the moving or sizing modal loop when the
        user clicks the window's title bar or sizing border, or when the window
        passes the WM_SYSCOMMAND message to the DefWindowProc function and the
        wParam parameter of the message specifies the SC_MOVE or SC_SIZE value.
        The operation is complete when DefWindowProc returns.
      */

      // not in wine..
      //case WM_EXITSIZEMOVE: {
      //  MIP_Print("WM_EXITSIZEMOVE\n");
      //  break;
      //}

      //-----

      /*
        Sent to a window that the user is resizing. By processing this message,
        an application can monitor the size and position of the drag rectangle
        and, if needed, change its size or position.
        wParam  The edge of the window that is being sized.
          case WMSZ_BOTTOM:       break; // Bottom edge
          case WMSZ_BOTTOMLEFT:   break; // Bottom-left corner
          case WMSZ_BOTTOMRIGHT:  break; // Bottom-right corner
          case WMSZ_LEFT:         break; // Left edge
          case WMSZ_RIGHT:        break; // Right edge
          case WMSZ_TOP:          break; // Top edge
          case WMSZ_TOPLEFT:      break; // Top-left corner
          case WMSZ_TOPRIGHT:     break; // Top-right corner
        lParam  A pointer to a RECT structure with the screen coordinates of
                the drag rectangle. To change the size or position of the drag
                rectangle, an application must change the members of this
                structure.
        An application should return TRUE if it processes this message.
      */

      // not in wine..
      //case WM_SIZING: {
      //  RECT* R = (RECT*)lParam;
      //  int32_t x = R->left;
      //  int32_t y = R->top;
      //  int32_t w = R->right - R->left;
      //  int32_t h = R->bottom - R->top;
      //  MIP_Print("WM_SIZING x %i y %i w %i h %i\n",x,y,w,h);
      //  break;
      //}

      //-----

      /*
        Sent to a window after its size has changed.
        wParam: The type of resizing requested.
          case SIZE_MAXHIDE:    // Message is sent to all pop-up windows when some other window is maximized.
          case SIZE_MAXIMIZED:  // The window has been maximized.
          case SIZE_MAXSHOW:    // Message is sent to all pop-up windows when some other window has been restored to its former size.
          case SIZE_MINIMIZED:  // The window has been minimized.
          case SIZE_RESTORED:   // The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
        The low-order word of lParam specifies the new width of the client area.
        The high-order word of lParam specifies the new height of the client area.
      */

      // is this sent before the window is open, during creation?

      // showWindow sends WM_SIZE,

      case WM_SIZE: {
        //MIP_Print("WM_SIZE\n");
        int32_t w = short(LOWORD(lParam));
        int32_t h = short(HIWORD(lParam));
        //MIP_Print("WM_SIZE w %i h %i (type %i)\n",w,h,wParam);

        #ifdef MIP_WIN32_INVALIDATE_WHEN_RESIZING
        invalidate(0,0,w,h);
        #endif

        //if ( (w != MRect.w) || (h != MRect.h) ) {
        //if ( (w != MWindowWidth) || (h != MWindowHeight) ) {
          //MIP_Print("-> on_window_resize\n");

//          MWidthScale  = (double)MWindowWidth / MInitialWidth;
//          MHeightScale = (double)MWindowHeight / MInitialHeight;

          //MIP_Print("%.3f, %.3f\n",MWidthScale,MHeightScale);

          on_window_resize(w,h);

          MWindowWidth  = w;
          MWindowHeight = h;

          //invalidateRegion(0,0,w,h);
          //if (MFlags & s3_wf_autoalign) on_widgetAlign(this);
          //#ifndef S3_NO_WINDOW_BACKBUFFER
          //if (MWindowListener) MWindowListener->on_bufferPaint(this,S3_NULL,s3_pm_normal); //redraw;
          //#endif
        //}
        break;
      }

      //-----

      /*
        The WM_PAINT message is sent when the system or another application
        makes a request to paint a portion of an application's window.
        The message is sent when the UpdateWindow or RedrawWindow function is
        called, or by the DispatchMessage function when the application obtains
        a WM_PAINT message by using the GetMessage or PeekMessage function.

        //is it possible to receive a WM_PAINT when some other thread is
        //drawing to the backbuffer?
      */

      case WM_PAINT: {
        //MIP_Print("WM_PAINT\n");
        beginPaint();
          //MWinPaintDC = BeginPaint(MHandle,&MWinPaintStruct);
          int32_t x = MWinPaintStruct.rcPaint.left;
          int32_t y = MWinPaintStruct.rcPaint.top;
          int32_t w = MWinPaintStruct.rcPaint.right  - MWinPaintStruct.rcPaint.left;// + 1;
          int32_t h = MWinPaintStruct.rcPaint.bottom - MWinPaintStruct.rcPaint.top;// + 1;
          //MIP_Print("WM_PAINT x %i y %i w %i h %i\n",x,y,w,h);
          //if (MFillBackground) fill(x,y,w,h,MBackgroundColor);
          on_window_paint(x,y,w,h);
        endPaint();
        //flush();
        //EndPaint(MHandle,&MWinPaintStruct);
        //UpdateWindow(MWinHandle);
        break;
      }

      case WM_MOUSEMOVE: {
        int32_t x = short(LOWORD(lParam));
        int32_t y = short(HIWORD(lParam));
        on_window_mouse_move(remapMouseKey(wParam),x,y,0);
        MMouseXpos = x;
        MMouseYpos = y;
        break;
      }

      case WM_XBUTTONDOWN:
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN: {
        int32_t b = message;
        switch (message) {
          case WM_LBUTTONDOWN:  b = MIP_BUTTON_LEFT;   break;
          case WM_MBUTTONDOWN:  b = MIP_BUTTON_MIDDLE; break;
          case WM_RBUTTONDOWN:  b = MIP_BUTTON_RIGHT;  break;
          case WM_XBUTTONDOWN:
            if ((wParam & MK_XBUTTON1) != 0) b = MIP_BUTTON_BACKWARD;
            if ((wParam & MK_XBUTTON2) != 0) b = MIP_BUTTON_FORWARD;
            break;
          default:              b = MIP_BUTTON_NONE;   break;
        }
        int32_t x = short(LOWORD(lParam));
        int32_t y = short(HIWORD(lParam));
      //if (MWindowListener) MWindowListener->on_mouseDown(this,x,y,b,remapKey(wParam));
        on_window_mouse_click(b,remapMouseKey(wParam),x,y,time);
        //        if (MFlags & s3_wf_capture) grabCursor();
        break;
      }

      //-----

      case WM_XBUTTONUP:
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP: {
        int32_t b = message;
        switch (message) {
          case WM_LBUTTONUP:  b = MIP_BUTTON_LEFT;   break;
          case WM_MBUTTONUP:  b = MIP_BUTTON_MIDDLE; break;
          case WM_RBUTTONUP:  b = MIP_BUTTON_RIGHT;  break;
          //case WM_XBUTTONUP:  b = MIP_BUTTON_s3_mb_side;   break;
          default:            b = MIP_BUTTON_NONE;   break;
        }
        int32_t x = short(LOWORD(lParam));
        int32_t y = short(HIWORD(lParam));
        //if (MWindowListener) MWindowListener->on_mouseUp(this,x,y,b,remapKey(wParam));
        on_window_mouse_release(b,remapMouseKey(wParam),x,y,time);
        //        if (MFlags&s3_wf_capture) releaseCursor();
        break;
      }

      //-----

      /*
        Only windows that have the CS_DBLCLKS style can receive
        WM_LBUTTONDBLCLK messages, which the system generates whenever the
        user presses, releases, and again presses the left mouse button
        within the system's double-click time limit. Double-clicking the left
        mouse button actually generates a sequence of four messages:
        WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, and WM_LBUTTONUP.
      */

      #ifdef MIP_MOUSE_DOUBLECLICK
      /*
      case WM_LBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_XBUTTONDBLCLK: {
        switch (message) {
          case WM_LBUTTONDBLCLK:  b = s3_mb_left;   break;
          case WM_MBUTTONDBLCLK:  b = s3_mb_middle; break;
          case WM_RBUTTONDBLCLK:  b = s3_mb_right;  break;
          case WM_XBUTTONDBLCLK:  b = s3_mb_side;   break;
          default:                b = s3_mb_none;   //break;
        }
        if (b==s3_mb_side) {
          if ((wParam & MK_XBUTTON1) != 0) b = s3_mb_backward;
          if ((wParam & MK_XBUTTON2) != 0) b = s3_mb_forward;
        }
        //S3_Trace("dblclick: %i\n",b);
        x = short(LOWORD(lParam));
        y = short(HIWORD(lParam));
        //x = GET_X_LPARAM(lParam);
        //y = GET_Y_LPARAM(lParam);
        //if (MWindowListener) MWindowListener->on_mouseUp(this,x,y,b,remapKey(wParam));
        on_widgetMouseDoubleClick(this,x,y,b,remapMouseKey(wParam));
        break;
      }
      */
      #endif // MIP_MOUSE_DOUBLECLICK

      //-----

      case WM_MOUSEWHEEL: {
        int32_t d = GET_WHEEL_DELTA_WPARAM(wParam);
        //if (d>0) { if (MWindowListener) MWindowListener->on_mouseDown(this,MMouseXpos,MMouseYpos,smb_wheelUp,  smb_none); }
        //if (d<0) { if (MWindowListener) MWindowListener->on_mouseDown(this,MMouseXpos,MMouseYpos,smb_wheelDown,smb_none); }
        if (d > 0) { on_window_mouse_click(MIP_BUTTON_SCROLL_UP,   MIP_KEY_NONE, MMouseXpos,MMouseYpos,time); }
        if (d < 0) { on_window_mouse_click(MIP_BUTTON_SCROLL_DOWN, MIP_KEY_NONE, MMouseXpos,MMouseYpos,time); }
        break;
      }

      //-----

      case WM_KEYDOWN: {
        //if (MWindowListener) MWindowListener->on_keyDown(this,wParam,lParam);
        //k = remapKeyCode(wParam,lParam);
        on_window_key_press(wParam,lParam,time);
        break;
      }

      //-----

      case WM_KEYUP: {
        //if (MWindowListener) MWindowListener->on_keyUp(this,wParam,lParam);
        //k = remapKeyCode(wParam,lParam);
        on_window_key_release(wParam,lParam,time);
        break;
      }

      //-----

      //case WM_CHAR:
      //  break;

      //-----

      case WM_TIMER: {
        //if (wParam == MIP_GUI_IDLE_TIMER_ID)
        on_window_timer();
        break;
      }

      //-----

      default: {
        result = DefWindowProc(hWnd,message,wParam,lParam);
        break;
      }

    } // switch

    return result;
  }

};

//----------------------------------------------------------------------
//
// event proc
//
//----------------------------------------------------------------------

LRESULT CALLBACK mip_win32_eventproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  MIP_Win32Window* window = (MIP_Win32Window*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	if (!window) return DefWindowProc(hWnd,message,wParam,lParam);
  return window->eventHandler(hWnd, message, wParam, lParam);
}

//------------------------------
// window class
//------------------------------

/*
  https://msdn.microsoft.com/en-us/library/windows/desktop/ms633586%28v=vs.85%29.aspx

  All window classes that an application registers are unregistered when it
  terminates.
  No window classes registered by a DLL are unregistered when the DLL is
  unloaded. A DLL must explicitly unregister its classes when it is unloaded.
*/

//----------

/*
  this one is a bit messy..
  we need:
  - HINSTANCE (from DLL)
  - callback function (which calls the window class)
  it should be registered only once, even for multiple instances on a plugin,
  so the plugin can't destroy the window class when it dies, since another
  instance might still use the window class for its editor..
  so, we do something singleton-ish..
  a static class with a constructor that is called before DllMain and any
  other function/method/class..
  and a destructor that is called when the global variable goes out of scope,
  when the dll is unloaded..
*/

  /*
    MIP_GLOBAL_WIN32_INSTANCE is not initialized until DllMain has been called
    (_after_ global initialization)
  */

class MIP_Win32WindowClass {

//------------------------------
private:
//------------------------------

  bool     MRegistered                  = false;
  char     MName[MIP_MAX_STRING_LENGTH] = "mip_window_0";
  ATOM     MAtom                        = 0;
  WNDCLASS MClass                       = {};

//------------------------------
public:
//------------------------------

  MIP_Win32WindowClass() {
    //printf("MIP_Win32WindowClass()\n");
    MRegistered = false;
  }

  //----------

  ~MIP_Win32WindowClass() {
    //printf("~MIP_Win32WindowClass()\n");
    if (MRegistered) {
      UnregisterClass(MName,MIP_GLOBAL_WIN32_INSTANCE);
    }
  }

  //----------

  char* getWindowClass() {
    //printf("MIP_Win32WindowClass.getWindowClass\n");
    if (!MRegistered) {
      MIP_CreateUniqueString(MName,(char*)"mip_window_",&MIP_GLOBAL_WIN32_INSTANCE);
      memset(&MClass,0,sizeof(MClass));
      MClass.style          = MIP_WIN32_CLASS_STYLE;
      MClass.lpfnWndProc    = &mip_win32_eventproc;
      MClass.hInstance      = MIP_GLOBAL_WIN32_INSTANCE;
      MClass.lpszClassName  = MName;
      MAtom = RegisterClass(&MClass);
      MRegistered = true;

      //#ifdef MIP_MOUSE_DOUBLECLICK
      //MClass.style |= CS_DBLCLKS;
      //#endif

    }
    return MName;
  }

};

//------------------------------
// global window class
//------------------------------

MIP_Win32WindowClass MIP_GLOBAL_WIN32_WINDOW_CLASS = {};

//----------

char* MIP_Win32ClassName() {
  return MIP_GLOBAL_WIN32_WINDOW_CLASS.getWindowClass();
}

//----------------------------------------------------------------------
#endif
