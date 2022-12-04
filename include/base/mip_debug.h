#ifndef mip_debug_included
#define mip_debug_included
//----------------------------------------------------------------------

/*
  ugh.. we need #ifdef's all over, or abstract this in some way..

  //#define MIP_DEBUG_PRINT_THREAD
  //#define MIP_DEBUG_PRINT_TIME
  //#define MIP_DEBUG_PRINT_SOCKET

  view socket output: nc -U -l -k /tmp/mip.socket
  (we could have several, for different severities/logging, etc?)

  todo:
  - observer
    MIP_GLOBAL_DEBUG.observe("myVariable",&myVariable,MIP_TYPE_BOOL,?);

*/

//----------------------------------------------------------------------

#ifdef MIP_DEBUG_PRINT_SOCKET
  #include <unistd.h>
  #ifdef MIP_LINUX
    #include <sys/socket.h>
    #include <sys/un.h>
  #endif
  #ifdef MIP_WIN32
  #endif
#endif

//----------

#ifdef MIP_DEBUG_PRINT_THREAD
  #ifdef MIP_LINUX
    //#include <sys/syscall.h>
    //#include <sys/unistd.h>
    //#define gettid() syscall(SYS_gettid)
  #endif
  #ifdef MIP_WIN32
    #include <processthreadsapi.h>
  #endif
#endif

//----------

#ifdef MIP_DEBUG_PRINT_TIME
  #include <sys/time.h> // gettimeofday
#endif

//----------

#include "base/debug/mip_assert.h"
#include "base/debug/mip_call_stack.h"
#include "base/debug/mip_crash_handler.h"
#include "base/debug/mip_mem_trace.h"

#include "base/debug/mip_observer.h"
#include "base/debug/mip_debug_window.h"

//#include "base/utils/mip_strutils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef MIP_DEBUG

#include <stdarg.h>   // va_
#include <sys/unistd.h>

class MIP_Debug {

//------------------------------
private:
//------------------------------

  char MPrefixBuffer[256] = {0};
  char MPrintBuffer[256] = {0};

  #ifdef MIP_DEBUG_PRINT_SOCKET
    int MSocketHandle = 0;
  #endif

  #ifdef MIP_DEBUG_PRINT_TIME
    double MStartTime = 0.0;
  #endif

//------------------------------
public:
//------------------------------

MIP_Debug() {
  #ifdef MIP_DEBUG_PRINT_SOCKET
    socket_init();
  #endif
  #ifdef MIP_DEBUG_PRINT_TIME
    time_start();
  #endif
}

//----------

~MIP_Debug() {
  #ifdef MIP_DEBUG_PRINT_SOCKET
    socket_close();
  #endif
}

//------------------------------
private:
//------------------------------

  // /home/skei/test -> test
  // returns everything after the last /

  const char* strip_path(const char* buffer) {
    const char *slash;
    slash = strrchr(buffer,'/');
    if (slash) {
      return slash + 1;
    }
    return buffer;
  }

  //----------

  #ifdef MIP_DEBUG_PRINT_TIME

  void time_start() {
    struct timeval time;
    gettimeofday(&time,NULL);
    MStartTime = (double)time.tv_sec + (double)time.tv_usec * .000001;
  }

  //----------

  double time_elapsed() {
    struct timeval time;
    gettimeofday(&time,NULL);
    double currenttime = (double)time.tv_sec + (double)time.tv_usec * .000001;
    double elapsed = currenttime - MStartTime;
    return elapsed;
  }

  #endif // MIP_DEBUG_PRINT_TIME

  //----------

  //#ifdef MIP_DEBUG_PRINT_THREAD
  //
  //  #define gettid() syscall(SYS_gettid)
  //
  //  pid_t MIP_GetThreadId() {
  //    return gettid();
  //  }
  //
  //#endif // MIP_DEBUG_PRINT_THREAD

  //----------

  #ifdef MIP_DEBUG_PRINT_SOCKET

  void socket_init() {
    #ifdef MIP_LINUX
      MSocketHandle = socket(PF_UNIX,SOCK_STREAM,0);
      sockaddr_un address;
      memset(&address,0,sizeof(sockaddr_un));
      address.sun_family = AF_UNIX;
      snprintf(address.sun_path,108,"/tmp/mip.socket"); // max 108?
      connect(MSocketHandle,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
    #endif
  }

  //----------

  void socket_close() {
    #ifdef MIP_LINUX
      close(MSocketHandle);
    #endif
  }

  //----------

  void socket_print(char* prefix_buffer, char* print_buffer) {
    #ifdef MIP_LINUX
      dprintf(MSocketHandle,"%s %s",prefix_buffer, print_buffer);
    #else
      printf("%s %s",prefix_buffer, print_buffer);
    #endif
  }

  #endif // MIP_DEBUG_PRINT_SOCKET

//------------------------------
public:
//------------------------------

  void set_prefix(const char* file, const char* func, uint32_t line) {
    __MIP_UNUSED char buffer[256];
    strcpy(MPrefixBuffer,"[");
    #ifdef MIP_DEBUG_PRINT_TIME
      double time = time_elapsed();
      sprintf(buffer,"%.6f:",time);
      strcat(MPrefixBuffer,buffer);
    #endif
    #ifdef MIP_DEBUG_PRINT_THREAD
      #ifdef MIP_LINUX
        uint32_t thread_id = gettid();
      #endif
      #ifdef MIP_WIN32
        uint32_t thread_id = GetCurrentThreadId();
      #endif
      sprintf(buffer,"%08x:",thread_id);
      strcat(MPrefixBuffer,buffer);
    #endif
    strcat(MPrefixBuffer,strip_path(file));
    strcat(MPrefixBuffer,":");
    strcat(MPrefixBuffer,func);
    strcat(MPrefixBuffer,":");
    char line_str[16] = {0};
    //itoa(line,line_str,10);
    sprintf(line_str,"%i",line);
    strcat(MPrefixBuffer,line_str);
    strcat(MPrefixBuffer,"]");
  }

  //----------

  void clear_prefix() {
    MPrefixBuffer[0] = 0;
  }

  //----------

  void print(const char* format, ...) {
    //printf("%s %s",MPrefixBuffer,..);
    va_list args;
    va_start(args,format);
    vsprintf(MPrintBuffer,format,args);
    #ifdef MIP_DEBUG_PRINT_SOCKET
      socket_print(MPrefixBuffer,MPrintBuffer);
    #else
      printf("%s %s",MPrefixBuffer,MPrintBuffer);
    #endif
    //flush_stdout
    va_end(args); // move this up?
    clear_prefix();
  }

  //----------

//  void dprint(const char* format, ...) {
//  }


};

#endif // MIP_DEBUG

//----------------------------------------------------------------------
//
// global
//
//----------------------------------------------------------------------

#ifdef MIP_DEBUG

  MIP_Debug MIP_GLOBAL_DEBUG = {};

  #define MIP_Print \
    MIP_GLOBAL_DEBUG.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    MIP_GLOBAL_DEBUG.print

  #define MIP_DPrint \
    MIP_GLOBAL_DEBUG.clear_prefix(); \
    MIP_GLOBAL_DEBUG.print

  #define MIP_PRINT \
    MIP_GLOBAL_DEBUG.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    MIP_GLOBAL_DEBUG.print("\n")

#else // MIP_DEBUG

  void MIP_NoPrint(const char*,...) {}
  #define MIP_Print    MIP_NoPrint
  #define MIP_DPrint   MIP_NoPrint
  #define MIP_PRINT {}

  //...

#endif

//----------------------------------------------------------------------
#endif
