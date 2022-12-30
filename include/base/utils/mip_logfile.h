#ifndef mip_logfile_included
#define mip_logfile_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "base/system/mip_file.h"
#include "base/system/mip_paths.h"
#include "base/system/mip_time.h"

#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_LOGGING

class MIP_LogFile {

//------------------------------
private:
//------------------------------

  MIP_File    MFile                             = {};
  const char* MLogfileName                      = nullptr;//"mip2_log.txt";
  bool        MEnabled                          = false;
  char        MDesktopPath[MIP_MAX_PATH_LENGTH] = {0};
  char        MFilename[MIP_MAX_PATH_LENGTH*2]  = {0};
  char        MTempBuffer[MIP_MAX_PATH_LENGTH]  = {0};
  //char      MHomePath[MIP_MAX_PATH_LENGTH]    = {0};

//------------------------------
public:
//------------------------------

  MIP_LogFile(const char* AName) {
    MEnabled = false;
    MLogfileName = AName;
    MIP_File file = {};
    MIP_GetDesktopPath(MDesktopPath);
    strcat(MDesktopPath,"skei.audio");
    if (file.direxists(MDesktopPath)) {

      #ifdef MIP_LINUX
        sprintf(MFilename,"%s/%s",MDesktopPath,MLogfileName);
        //snprintf(MFilename,MIP_MAX_STRING_LENGTH,"%s/%s",MDesktopPath,MLogfileName);
      #else
        sprintf(MFilename,"%s\\%s",MDesktopPath,MLogfileName);
      #endif

      #ifdef MIP_LOG_APPEND
        MFile.open(MFilename,MIP_FILE_APPEND_TEXT);
      #else
        MFile.open(MFilename,MIP_FILE_WRITE_TEXT);
      #endif

      if (!MFile.isOpen()) {
        printf("Log: couldn't open logfile '%s'\n",MFilename);
      }
      else {
        MEnabled = true;
        print("\n----------------------------------------------------------------------\n\n");
        print_header();
        print("\n----------\n\n");
      }

    }
    else {
      //printf("Log: directory '%s' doesn't exist\n",MDesktopPath);
    }

  }

  //----------

  ~MIP_LogFile() {
    if (MEnabled) {
      print("\n\n");
      MFile.close();
    }
  }

//------------------------------
public:
//------------------------------

  void print_header() {

    if (MEnabled) {

      MIP_CurrentTime time = {0};
      MIP_GetLocalTime(&time);

      //------------------------------
      //
      //------------------------------

      const char* gui = "";
      const char* painter = "";

      #ifdef MIP_NO_GUI
        gui = "MIP_NO_GUI";
      #endif

      #ifdef MIP_GUI_WIN32
        gui = "MIP_GUI_WIN32";
      #endif

      #ifdef MIP_GUI_XCB
        gui = "MIP_GUI_XCB";
      #endif

      #ifdef MIP_NO_PAINTER
        gui = "MIP_NO_PAINTER";
      #endif

      #ifdef MIP_PAINTER_GDI
        painter = "MIP_PAINER_GDI";
      #endif

      #ifdef MIP_PAINTER_GLX
        painter = "MIP_PAINER_GLX";
      #endif

      #ifdef MIP_PAINTER_NVG
        painter = "MIP_PAINER_NVG";
      #endif

      #ifdef MIP_PAINTER_WGL
        painter = "MIP_PAINER_WGL";
      #endif

      #ifdef MIP_PAINTER_XCB
        painter = "MIP_PAINER_XCB";
      #endif

      //------------------------------
      //
      //------------------------------

      print("Time: %02i.%02i.%04i (%02i.%02i.%02i)\n",time.day,time.month,time.year,time.hour,time.minutes,time.seconds);

      #ifdef MIP_DEBUG
        print("Build: Debug\n");
      #else
        print("Build: Release\n");
      #endif

      #ifdef MIP_LINUX
        print("OS: Linux\n");
      #endif

      #ifdef MIP_WIN32
        print("OS: Windows\n");
      #endif

      print("GUI: %s\n",gui);
      print("Painter: %s\n",painter);

      char buffer[MIP_MAX_PATH_LENGTH];
      print("Home path: %s\n",MIP_GetHomePath(buffer));
      print("Desktop path: %s\n",MIP_GetDesktopPath(buffer));
      print("Base filename: %s\n",MIP_GetBaseFilename(buffer));
      print("Base path: %s\n",MIP_GetBasePath(buffer));

      print("CLAP Version: %i.%i.%i\n",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);

      #ifdef MIP_PLUGIN_EXE
        print("Wrapper: EXE\n");
      #endif

      #ifdef MIP_PLUGIN_VST2
        print("Wrapper: VST2\n");
      #endif

      #ifdef MIP_PLUGIN_VST3
        print("Wrapper: VST3\n");
      #endif

      //------------------------------
      //
      //------------------------------

    } // enabled

  }

//------------------------------
public:
//------------------------------

  //void operator () (const char* format, ...) {}

  //----------

  void print_string(char* str) {
    if (MEnabled) {
      uint32_t len = strlen(str);
      MFile.write(str,len);
      MFile.flush();
    }
}

  //----------

  void print(const char* format, ...) {
    if (MEnabled) {
      if (MFile.isOpen()) {
        va_list args;
        va_start(args,format);
        vsprintf(MTempBuffer,format,args);
        va_end(args);
        print_string(MTempBuffer);
      }
    }
  }

  void print_if(bool cond, const char* format, ...) {
    if (MEnabled) {
      if (cond && MFile.isOpen()) {
        va_list args;
        va_start(args,format);
        vsprintf(MTempBuffer,format,args);
        va_end(args);
        print_string(MTempBuffer);
      }
    }
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------

#else // MIP_LOGGING

class MIP_LogFile {
public:
  MIP_LogFile(const char* AName) {}
  ~MIP_LogFile() {}
public:
  void print_header() {}
  void print_string(char* str) {}
  void print(const char* format, ...) {}
  void print_if(bool cond, const char* format, ...) {}
};

#endif // MIP_LOGGING

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_LogFile LOG = MIP_LogFile(MIP_LOGFILE_NAME);

//----------------------------------------------------------------------
#endif
