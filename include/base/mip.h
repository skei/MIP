#ifndef mip_included
#define mip_included
//----------------------------------------------------------------------

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>

//------------------------------
//
//------------------------------

#include "base/mip_config.h"
#include "base/mip_attributes.h"
#include "base/mip_const.h"
#include "base/mip_enums.h"
#include "base/mip_types.h"
//#include "base/mip_system.h"

//------------------------------
//
//------------------------------

// nc -U -l -k /tmp/mip.socket
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
#endif

//------------------------------
//
//------------------------------

#ifdef MIP_LINUX
  #include "mip_linux.h"
#endif

//----------

#ifdef MIP_WIN32
  #include "mip_win32.h"
#endif

//------------------------------
//
//------------------------------

// nc -U -l -k /tmp/mip.socket

#ifdef MIP_PLUGIN
  #ifdef MIP_LINUX
    #define MIP_DEBUG_PRINT_SOCKET
  #endif
#endif

#include "base/mip_debug.h"
#include "base/utils/mip_logfile.h"
#include "base/mip_test.h"

#ifdef MIP_UNIT_TESTING
  MIP_TEST_STATE();
#endif

#include "plugin/mip_registry.h"

//#include "plugin/mip_entry.h"

//----------------------------------------------------------------------
#endif
