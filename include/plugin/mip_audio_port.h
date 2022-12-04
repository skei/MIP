#ifndef mip_audio_port_included
#define mip_audio_portincluded
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"

class MIP_AudioPort;
typedef MIP_Array<MIP_AudioPort*> MIP_AudioPortArray;

class MIP_AudioPort {

  friend class MIP_Plugin;

private:

  clap_audio_port_info_t MAudioPortInfo = {
    /*clap_id     id                    =*/ 0,
    /*char        name[CLAP_NAME_SIZE]  =*/ "Audio",
    /*uint32_t    flags                 =*/ CLAP_AUDIO_PORT_IS_MAIN,
    /*uint32_t    channel_count         =*/ 2,
    /*const char* port_type             =*/ CLAP_PORT_STEREO,
    /*clap_id     in_place_pair         =*/ CLAP_INVALID_ID
  };

public:
  clap_audio_port_info_t* getInfo() { return &MAudioPortInfo; }
};

//----------------------------------------------------------------------
#endif
