#ifndef mip_config_included
#define mip_config_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// os
//
//----------------------------------------------------------------------

#ifdef __gnu_linux__
  #define MIP_LINUX
#endif

#if defined _WIN32 || defined __CYGWIN__
  #define  MIP_WIN32
#endif

//----------------------------------------------------------------------
//
// numeric, magic values, ..
//
//----------------------------------------------------------------------

  #define MIP_ALIGNMENT_SIMD                        16
  #define MIP_ALIGNMENT_CACHE                       64
  #define MIP_AUDIO_SLICE_SIZE                      32
  #define MIP_AUDIO_MAX_BLOCK_SIZE                  (4096 * 4)
  #define MIP_CLIP_RECT_STACK_SIZE                  1024
  #define MIP_LOGFILE_NAME                          "mip.log"
  #define MIP_MAX_PATH_LENGTH                       512
  #define MIP_MAX_STRING_LENGTH                     256
  #define MIP_OPENGL_VERSION_MAJOR                  3
  #define MIP_OPENGL_VERSION_MINOR                  2
  #define MIP_PLUGIN_MAX_GUI_EVENTS                 32
  #define MIP_PLUGIN_MAX_PARAM_EVENTS               4096
  #define MIP_PLUGIN_GENERIC_EDITOR_WIDTH           500
  #define MIP_PLUGIN_VST3_TIMER_MS                  50
  #define MIP_PLUGIN_VST3_MAX_AUDIO_BLOCK_SIZE      4096
  #define MIP_PLUGIN_VST3_MAX_EVENTS_PER_BLOCK      4096
  #define MIP_PLUGIN_VST3_MAX_EVENT_SIZE            256
  #define MIP_PLUGIN_VST3_MAX_NOTE_IDS              32
  #define MIP_PLUGIN_VST3_MAX_GUI_EVENTS            32
  #define MIP_VOICE_MAX_EVENTS_PER_BLOCK            4096
  #define MIP_VOICE_MANAGER_MAX_EVENTS_PER_BLOCK    4096
  #define MIP_VOICE_MANAGER_STEAL_VOICES            true
  #define MIP_WINDOW_MAX_DIRTY_WIDGETS              1024
  #define MIP_WINDOW_TIMER_MS                       30
  #define MIP_WINDOW_TIMER_ID                       333

//----------------------------------------------------------------------
//
// general
//
//----------------------------------------------------------------------

//#define MIP_EXECUTABLE_SHARED_LIBRARY
//#define MIP_FAST_MATH_EXTRA_PRECISION
  #define MIP_LOGGING
  #define MIP_LOG_APPEND
//#define MIP_NO_AUTODELETE
//#define MIP_NO_DEFAULT_WARNINGS
//#define MIP_NO_DMC_COMPRESS
//#define MIP_NO_ENTRY
//#define MIP_NO_STDLIB
  #define MIP_RANDOM_LCG
//#define MIP_RANDOM_SYSTEM
//#define MIP_UNIT_TESTING
//#define MIP_VOICE_ADD_TO_BUFFER

//----------------------------------------------------------------------
//
// debug
//
//----------------------------------------------------------------------

#ifdef MIP_DEBUG
  #define MIP_DEBUG_ASSERT
  #define MIP_DEBUG_CALLSTCAK
  #define MIP_DEBUG_CRASHHANDLER
//#define MIP_DEBUG_MEMTRACE
//#define MIP_DEBUG_OBSERVER
//#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_DEBUG_PRINT_THREAD
//#define MIP_DEBUG_PRINT_TIME
//#define MIP_DEBUG_WINDOW
#else
  #undef MIP_DEBUG_ASSERT
  #undef MIP_DEBUG_CALLSTCAK
  #undef MIP_DEBUG_CRASHHANDLER
  #undef MIP_DEBUG_MEMTRACE
  #undef MIP_DEBUG_OBSERVER
  #undef MIP_DEBUG_PRINT_SOCKET
  #undef MIP_DEBUG_PRINT_THREAD
  #undef MIP_DEBUG_PRINT_TIME
  #undef MIP_DEBUG_WINDOW
#endif

//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------

//#define MIP_GUI_WIN32
//#define MIP_GUI_XCB
//#define MIP_MOUSE_DOUBLECLICK
//#define MIP_NO_BITMAP_PNG
//#define MIP_NO_BITMAP_PNG_FILE
//#define MIP_NO_BITMAP_DRAW
//#define MIP_NO_BITMAP_FONT
//#define MIP_NO_BITMAP_PROC
//#define MIP_NO_BITMAP_GEN
//#define MIP_NO_GUI
//#define MIP_NO_PAINTER
//#define MIP_PAINTER_CAIRO
//#define MIP_PAINTER_GDI
//#define MIP_PAINTER_GLX
//#define MIP_PAINTER_NVG
//#define MIP_PAINTER_WGL
//#define MIP_PAINTER_XCB
//#define MIP_USE_CAIRO
//#define MIP_USE_GDI
//#define MIP_USE_GLX
//#define MIP_USE_NVG
//#define MIP_USE_WGL
//#define MIP_USE_XCB
//#define MIP_WIN32_GDIPLUS
//#define MIP_WIN32_INVALIDATE_WHEN_RESIZING
//#define MIP_WINDOW_BUFFERED
//#define MIP_WINDOW_COMBINE_UPDATE_RECTS
  #define MIP_WINDOW_FULL_UPDATE_RECT
//#define MIP_XCB_USE_STB_TRUETYPE
  #define MIP_XCB_WAIT_FOR_MAPNOTIFY

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

//#define MIP_NO_ENTRY
//#define MIP_NO_PLUGIN
//#define MIP_NO_REGISTRY
//#define MIP_PLUGIN_CLAP
//#define MIP_PLUGIN_EXE
//#define MIP_PLUGIN_VST2
//#define MIP_PLUGIN_VST3
//#define MIP_PLUGIN_GENERIC_EDITOR
//#define MIP_VOICE_MANAGER_CONVERT_MIDI
  #define MIP_VOICE_MANAGER_SEND_GLOBAL_PARAMS_TO_ALL_VOICES
  #define MIP_VOICE_MANAGER_SEND_GLOBAL_MODS_TO_ALL_VOICES
//#define MIP_VST2_FST
//#define MIP_VST2_SDK
//#define MIP_VST2_VESTIGE
//#define MIP_VST2_XAYMAR
//#define MIP_VST3_SDK
//#define MIP_VST3_TRAVESTY


//----------------------------------------------------------------------
//
// post-process the defines..
//
//----------------------------------------------------------------------





//------------------------------
// gui type
//------------------------------

#undef MIP_GUI_DEFINED

//----------

#ifdef MIP_NO_GUI
  #define MIP_GUI_DEFINED
#endif

#ifdef MIP_GUI_WIN32
  #define MIP_GUI_DEFINED
  #define MIP_USE_WIN32
  #define MIP_USE_GDI
#endif

#ifdef MIP_GUI_XCB
  #define MIP_GUI_DEFINED
  #define MIP_USE_XCB
#endif

//----------

#ifndef MIP_GUI_DEFINED
  #ifdef MIP_LINUX
    #ifndef MIP_NO_DEFAULT_WARNINGS
      #warning default gui: xcb
    #endif
    #define MIP_GUI_XCB
    #define MIP_USE_XCB
  #endif
  #ifdef MIP_WIN32
    #ifndef MIP_NO_DEFAULT_WARNINGS
      #warning default gui: win32
    #endif
    #define MIP_GUI_WIN32
    #define MIP_USE_WIN32
  #endif
#endif

//----------

#undef MIP_GUI_DEFINED

//------------------------------
// painter (type)
//------------------------------

#undef MIP_PAINTER_DEFINED

//----------

#ifdef MIP_NO_PAINTER
  #define MIP_PAINTER_DEFINED
#endif

#ifdef MIP_PAINTER_CAIRO
  #define MIP_PAINTER_DEFINED
  #define MIP_USE_CAIRO
#endif

#ifdef MIP_PAINTER_GDI
  #define MIP_PAINTER_DEFINED
  #define MIP_USE_GDI
  #define MIP_USE_WIN32
#endif

#ifdef MIP_PAINTER_GLX
  #define MIP_PAINTER_DEFINED
  #define MIP_USE_GLX
  #define MIP_USE_XCB
  #define MIP_USE_XLIB
#endif

#ifdef MIP_PAINTER_NVG
  #define MIP_PAINTER_DEFINED
  #define MIP_USE_NVG
  #ifdef MIP_LINUX
    #define MIP_USE_GLX
    #define MIP_USE_XCB
    #define MIP_USE_XLIB
  #endif
  #ifdef MIP_WIN32
    #define MIP_USE_WGL
    #define MIP_USE_WIN32
    //#define MIP_USE_GDI
  #endif
#endif

#ifdef MIP_PAINTER_WGL
  #define MIP_PAINTER_DEFINED
  #define MIP_USE_WGL
  #define MIP_USE_WIN32
  //#define MIP_USE_GDI
#endif

#ifdef MIP_PAINTER_XCB
  #define MIP_PAINTER_DEFINED
  #define MIP_USE_XCB
#endif

//----------

#ifndef MIP_PAINTER_DEFINED
  #ifdef MIP_LINUX
    #ifndef MIP_NO_DEFAULT_WARNINGS
      #warning default painter: nvg
    #endif
    //#define MIP_PAINTER_XCB
    #define MIP_PAINTER_NVG
    #define MIP_USE_GLX
    #define MIP_USE_NVG
    #define MIP_USE_XCB
    #define MIP_USE_XLIB
  #endif
  #ifdef MIP_WIN32
    #ifndef MIP_NO_DEFAULT_WARNINGS
      #warning default painter: nvg
    #endif
    //#define MIP_PAINTER_GDI
    #define MIP_PAINTER_NVG
    #define MIP_USE_NVG
    #define MIP_USE_WGL
    #define MIP_USE_WIN32
    //#define MIP_USE_GDI
  #endif
#endif

//----------

#undef MIP_PAINTER_DEFINED

//------------------------------
// plugin (type)
//------------------------------

#undef MIP_PLUGIN_DEFINED

//----------

#ifdef MIP_NO_PLUGIN
  #define MIP_PLUGIN_DEFINED
#endif

#ifdef MIP_PLUGIN_CLAP
  #define MIP_PLUGIN_DEFINED
#endif

#ifdef MIP_PLUGIN_EXE
  #define MIP_PLUGIN_DEFINED
#endif

#ifdef MIP_PLUGIN_VST2
  #define MIP_PLUGIN_DEFINED
#endif

#ifdef MIP_PLUGIN_VST3
  #define MIP_PLUGIN_DEFINED
#endif

//----------

#ifndef MIP_PLUGIN_DEFINED
  #ifdef MIP_PLUGIN
    #ifndef MIP_NO_DEFAULT_WARNINGS
      #warning default plugin: clap
    #endif
    #define MIP_PLUGIN_CLAP
  #endif
  #ifdef MIP_EXE
    #ifndef MIP_NO_DEFAULT_WARNINGS
      #warning default plugin: exe
    #endif
    #define MIP_PLUGIN_EXE
  #endif
#endif

//----------

#undef MIP_PLUGIN_DEFINED

//----------------------------------------------------------------------
#endif
