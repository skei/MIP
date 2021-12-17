#ifndef mip_clap_included
#define mip_clap_included
//----------------------------------------------------------------------

#ifdef MIP_DEBUG_CLAP
  #define MIP_CLAPPRINT   MIP_PRINT
  #define MIP_ClapPrint   MIP_Print
  #define MIP_ClapDPrint  MIP_DPrint
#else
  #define MIP_CLAPPRINT
  #define MIP_ClapPrint   MIP_NoPrint
  #define MIP_ClapDPrint  MIP_NoPrint
#endif

#include "mip.h"

#include "extern/clap/all.h"
#include "extern/clap/ext/draft/check-for-update.h"
#include "extern/clap/ext/draft/midi-mappings.h"

#include "plugin/clap/mip_clap_plugin.h"
#include "plugin/clap/mip_clap_utils.h"

//----------

//#define MIP_CLAP_MAIN(D,I,E)


//----------------------------------------------------------------------
#endif
