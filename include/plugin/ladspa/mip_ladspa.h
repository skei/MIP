#ifndef mip_ladspa_included
#define mip_ladspa_included
//----------------------------------------------------------------------

#include "base/mip.h"


#ifdef MIP_LINUX
  #include <ladspa.h>
#endif

#ifdef MIP_WIN32
  #include "/usr/include/ladspa.h"
#endif

#define MIP_PLUGIN_LADSPA_MAX_NAME_LENGTH      256
#define MIP_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH 64

//----------------------------------------------------------------------
#endif
