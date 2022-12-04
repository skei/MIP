#ifndef mip_note_port_included
#define mip_note_port_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"

class MIP_NotePort;
typedef MIP_Array<MIP_NotePort*> MIP_NotePortArray;

class MIP_NotePort {

  friend class MIP_Plugin;

private:

  clap_note_port_info_t MNotePortInfo = {
    /*clap_id  .id                    =*/ 0,
    /*uint32_t .supported_dialects    =*/ CLAP_NOTE_DIALECT_CLAP,
    /*uint32_t .preferred_dialect     =*/ CLAP_NOTE_DIALECT_CLAP,
    /*char     .name[CLAP_NAME_SIZE]  =*/ "Notes"
  };

public:

  clap_note_port_info_t* getInfo() { return &MNotePortInfo; }

};


//----------------------------------------------------------------------
#endif
