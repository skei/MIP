#ifndef mip_wgl_painter_included
#define mip_wgl_painter_included
//----------------------------------------------------------------------

/*
  context from surface doesn't work..
  we use CreateCompatibleBitmap in MIP_GdiSurface
  try with CreateDIBSectioh or similar?

  ----------

  https://learn.microsoft.com/en-us/windows/win32/opengl/rendering-contexts

  A thread that makes OpenGL calls must have a current rendering context. If an
  application makes OpenGL calls from a thread that lacks a current rendering
  context, nothing happens; the call has no effect. An application commonly
  creates a rendering context, sets it as a thread's current rendering context,
  and then calls OpenGL functions. When it finishes calling OpenGL functions,
  the application uncouples the rendering context from the thread, and then
  deletes the rendering context. A window can have multiple rendering contexts
  drawing to it at one time, but a thread can have only one current, active
  rendering context.

  A current rendering context has an associated device context. That device
  context need not be the same device context as that used when the rendering
  context was created, but it must reference the same device and have the same
  pixel format.

  A thread can have only one current rendering context. A rendering context can
  be current to only one thread.

*/

//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_paint_target.h"
#include "gui/base/mip_base_painter.h"
#include "gui/wgl/mip_wgl.h"
#include "gui/wgl/mip_wgl_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

PIXELFORMATDESCRIPTOR MIP_WglWindowPFD = {
  sizeof(MIP_WglWindowPFD),
  1,
  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,// | PFD_DOUBLEBUFFER,
  PFD_TYPE_RGBA,
  32,               // bits
  0, 0, 0, 0, 0, 0,
  8,//0,            // alpha
  0,                // shift
  0,                // accumulation
  0, 0, 0, 0,
  24,               // depth
  8,//0,            // stencil
  0,                // aux
  PFD_MAIN_PLANE,
  0, 0, 0, 0
};

//----------

PIXELFORMATDESCRIPTOR MIP_WglSurfacePFD = {
  sizeof(MIP_WglSurfacePFD),
  1,
  PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI,// | PFD_DOUBLEBUFFER,
  PFD_TYPE_RGBA,
  24,//32,               // bits
  0, 0, 0, 0, 0, 0,
  8,//0,            // alpha
  0,                // shift
  0,                // accumulation
  0, 0, 0, 0,
  32,//24,               // depth
  8,//0,            // stencil
  0,                // aux
  PFD_MAIN_PLANE,
  0, 0, 0, 0
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_WglPainter
: public MIP_BasePainter {

  friend class MIP_NvgPainter;

//------------------------------
private:
//------------------------------

  HDC               MDC         = nullptr;
  HGLRC             MGLRC       = nullptr;
  MIP_PaintTarget*  MTarget     = nullptr;
  bool              MIsCurrent  = false;

//------------------------------
public:
//------------------------------

  /*
    dc = CreateCompatibleDC(NULL);///////creates a Device context
    memset(&info, 0, sizeof(info));
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = 768;
    info.bmiHeader.biHeight = 576;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 8*3;
    info.bmiHeader.biCompression = BI_RGB;
    bitmap = CreateDIBSection(dc, &info, DIB_RGB_COLORS,(void **)&outbits, NULL,0);
    HGDIOBJ res = SelectObject(dc, bitmap);
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI ;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 8*3;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cDepthBits = 32;
    pf = ChoosePixelFormat(dc, &pfd);
    int result = SetPixelFormat(dc, pf, &pfd);
    rc = wglCreateContext(dc);
  */

  //----------

  /*
  */

  MIP_WglPainter(MIP_PaintTarget* ATarget)
  : MIP_BasePainter(ATarget) {
    //MIP_PRINT;

    MTarget = ATarget;

    // pixel format

    if (MTarget->tgtIsWindow()) {
      HWND hwnd = MTarget->tgtGetHWND();
      MDC = GetDC(hwnd);
      if (!MDC) MIP_Win32PrintError("GetDC");
      int pf = ChoosePixelFormat(MDC, &MIP_WglWindowPFD);
      SetPixelFormat(MDC, pf, &MIP_WglWindowPFD);
    }
    else {
      //MIP_Print("opengl rendering to buffer not supported in windows (yet)..\n");
      //exit(1);
      HDC tempdc = GetDC(0);
      MDC = CreateCompatibleDC(tempdc);
      ReleaseDC(0,tempdc);
      int pf = ChoosePixelFormat(MDC, &MIP_WglSurfacePFD);
      SetPixelFormat(MDC, pf, &MIP_WglSurfacePFD);
    }

    // temp context

    HGLRC temp_ctx = wglCreateContext(MDC);
    if (temp_ctx) {
      //MIP_Print("created temp context, making it current\n");
      if (wglMakeCurrent(MDC,temp_ctx) == FALSE) {
        MIP_Print("wglMakeCurrent error\n");
      };
    }
    else {
      MIP_Print("error! couldn't create temp conext\n");
      exit(1);
    }

    // extension string

//    const char* get_extensions();
//    PFNWGLGETEXTENSIONSSTRINGEXTPROC wgl_GetExtensionsStringEXT;
//    wgl_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
//    if (!wgl_GetExtensionsStringEXT) {
//      MIP_Print("error! wglGetExtensionsStringEXT not found\n");
//      exit(1);
//    }
//    else {
//      MIP_Print("wglGetExtensionsStringEXT found\n");
//      const char* extensions = wgl_GetExtensionsStringEXT();
//      if (strstr(extensions, "WGL_ARB_create_context")) {
//        //ext->create_context_attribs = 1;
//      }
//      if (strstr(extensions, "WGL_EXT_swap_control")) {
//        //ext->swap_control = 1;
//        if (strstr(extensions, "WGL_EXT_swap_control_tear")) {
//          //ext->swap_control_tear = 1;
//        }
//      }
//      if (strstr(extensions, "NV")) {
//        //ext->appears_to_be_nvidia = 1;
//      }
//    }
//    //MIP_Assert( wgl_GetExtensionsStringEXT );

    // load opengl functions

//    if (!sogl_loadOpenGL()) {
//      MIP_Print("error! sogl_loadOpenGL failures: \n");
//      const char** failures = sogl_getFailures();
//      while (*failures) {
//        MIP_DPrint("%s ",*failures);
//        failures++;
//      }
//      MIP_DPrint("\n");
//      wglMakeCurrent(MDC, NULL);
//      wglDeleteContext(temp_ctx);
//      exit(1);
//    }
//    else {
//      //MIP_Print("loaded opengl functions (sogl_loadOpenGL)\n");
//    }

    MIP_LoadOpenGL();

    // check version

//    int maj, min;
//    glGetIntegerv(GL_MAJOR_VERSION, &maj);
//    glGetIntegerv(GL_MINOR_VERSION, &min);
//    MIP_Print("GL_MAJOR_VERSION: %i GL_MINOR_VERSION: %i\n",maj,min);   // GL_MAJOR_VERSION: 3 GL_MINOR_VERSION: 1
//
//    MIP_Print("GL_VERSION: %s\n",     (char*)glGetString(GL_VERSION));    // 3.1 Mesa 21.2.6
//    MIP_Print("GL_VENDOR: %s\n",      (char*)glGetString(GL_VENDOR));     // Mesa/X.org
//    MIP_Print("GL_RENDERER: %s\n",    (char*)glGetString(GL_RENDERER));   // llvmpipe (LLVM 12.0.0, 256 bits)
//    //MIP_Print("GL_EXTENSIONS: %s\n",  (char*)glGetString(GL_EXTENSIONS)); // crashes!

    // create context ARB

    PFNWGLCREATECONTEXTATTRIBSARBPROC wgl_CreateContextAttribsARB;
    wgl_CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wgl_CreateContextAttribsARB) {
      MIP_Print("error! wglCreateContextAttribsARB not found\n");
      exit(1);
    }
    else {
      //MIP_Print("wglCreateContextAttribsARBfound\n");
    }

    const int ctx_attribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, MIP_OPENGL_VERSION_MAJOR,
      WGL_CONTEXT_MINOR_VERSION_ARB, MIP_OPENGL_VERSION_MINOR,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0
    };

    MGLRC = wgl_CreateContextAttribsARB(MDC, NULL, ctx_attribs);
    if (!MGLRC) {
      MIP_Print("error creating context\n");
      wglMakeCurrent(MDC, NULL);
      wglDeleteContext(temp_ctx);
      exit(1);
    }
    else {
      //MIP_Print("context created\n");
    }

    //----------

    wglMakeCurrent(MDC,NULL);
    wglDeleteContext(temp_ctx);
    wglMakeCurrent(MDC, MGLRC);

    MIsCurrent = true;

  }

  //----------

  virtual ~MIP_WglPainter() {
    //MIP_PRINT;
    MIP_UnloadOpenGL();
    ReleaseDC(0,MDC);
    wglDeleteContext(MGLRC);
  }

//------------------------------
public:
//------------------------------

  void setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    glViewport(AXpos,AYpos,AWidth,AHeight);
  }

  //----------

  // w/h = viewport size (not update rect!)

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AMode) override {
    //MIP_Print("%i,%i - %i,%i\n",AXpos,AYpos,AWidth,AHeight);
    makeCurrent(AMode);
    //glViewport(0,0,AWidth,AHeight);
    setViewport(0,0,AWidth,AHeight);
  }

  //----------

  void endPaint(uint32_t AMode) override {
    //MIP_PRINT;
    swapBuffers(AMode);
    resetCurrent(AMode);
  }

//------------------------------
//protected:
public:
//------------------------------

  /*
    The wglMakeCurrent function makes a specified OpenGL rendering context the
    calling thread's current rendering context. All subsequent OpenGL calls
    made by the thread are drawn on the device identified by hdc. You can also
    use wglMakeCurrent to change the calling thread's current rendering context
    so it's no longer current.

    If hglrc is NULL, the function makes the calling thread's current rendering
    context no longer current, and releases the device context that is used by
    the rendering context. In this case, hdc is ignored.
  */

  bool makeCurrent(uint32_t AMode) override {
    //MIP_Print("\n");
    wglMakeCurrent(MDC, MGLRC);
    MIsCurrent = true;
    return true;
  }

  //----------

  bool resetCurrent(uint32_t AMode) override {
    //MIP_Print("\n");
    wglMakeCurrent(nullptr,nullptr);
    MIsCurrent = false;
    return true;
  }

  //----------

  /*
    The SwapBuffers function exchanges the front and back buffers if the
    current pixel format for the window referenced by the specified device
    context includes a back buffer.
  */

  void swapBuffers(uint32_t AMode) override {
    //MIP_Print("\n");
    SwapBuffers(MDC);
  }

//------------------------------
private:
//------------------------------

};

//----------------------------------------------------------------------
#endif



//HMODULE gl_module = LoadLibrary(TEXT("opengl32.dll"));
//wgl_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)GetProcAddress(gl_module,"wglGetExtensionsStringEXT");
//FreeLibrary(gl_module);


