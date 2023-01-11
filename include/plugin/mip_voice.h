#ifndef mip_voice_included
#define mip_voice_included
//----------------------------------------------------------------------

#include "plugin/mip_process_context.h"
#include "plugin/mip_voice_context.h"

//----------------------------------------------------------------------

struct MIP_VoiceEvent {
  //MIP_Note note   = {};
  uint32_t time   = 0;
  uint32_t type   = 0;//CLAP_EVENT_NOTE_ON;
  uint32_t index  = 0;
  double   value  = 0.0;
  MIP_VoiceEvent(/*MIP_Note n={},*/ uint32_t ty=0, uint32_t ti=0, uint32_t in=0, double va=0.0) {
    //note  = n;
    type  = ty;
    time  = ti;
    index = in;
    value = va;
  }
};

typedef MIP_Queue<MIP_VoiceEvent,MIP_VOICE_MAX_EVENTS_PER_BLOCK> MIP_VoiceEventQueue;

//----------

//typedef MIP_Queue<const clap_event_header_t*,MIP_VOICE_MAX_EVENTS_PER_BLOCK> MIP_ClapEventQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class MYVOICE>
class MIP_Voice {

//------------------------------
public:
//------------------------------

  MYVOICE             voice       = {};
  uint32_t            index       = 0;
  uint32_t            state       = MIP_VOICE_OFF;
  MIP_Note            note        = {};
  double              env_level   = 0.0;
  uint32_t            event_mode  = MIP_VOICE_EVENT_MODE_BLOCK;

  double              velocity    = 0.0;
  double              pressure    = 0.0;
  double              brightness  = 0.0;
  double              tuning      = 0.0;

  //MIP_ClapEventQueue  events      = {};
  MIP_VoiceEventQueue events      = {};
  MIP_VoiceContext*   context     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Voice() {
  }

  //----------

  ~MIP_Voice() {
  }

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, MIP_VoiceContext* AContext) {
    context = AContext;
    voice.init(AIndex,context);
  }

  //----------

  /*
    event handling
    1) all at beginning of buffer
    2) split at event offsets, process inbetween
    3) quantize to n samples, handla all events for current slice
       at beginning of slice..
       a) further: hardcoding..
  */

  void process() {
    switch (event_mode) {
      case MIP_VOICE_EVENT_MODE_BLOCK: {
        handleAllEvents();
        break;
      }
      case MIP_VOICE_EVENT_MODE_INTERLEAVED: {
        handleInterleavedEvents();
        break;
      }
      case MIP_VOICE_EVENT_MODE_QUANTIZED: {
        handleQuantizedEvents();
        break;
      }
    }
  }

//------------------------------
private:
//------------------------------

  //void handleEvent(const clap_event_header_t* header) {
  void handleEvent(MIP_VoiceEvent event) {
    switch(event.type) {
      case CLAP_EVENT_NOTE_ON: {
        //const clap_event_note_t* note_event = (const clap_event_note_t*)header;
        //state = voice.noteOn(note_event->key,note_event->velocity);
        state = voice.noteOn(event.index,event.value);
        break;
      }
      case CLAP_EVENT_NOTE_OFF: {
        //const clap_event_note_t* note_event = (const clap_event_note_t*)header;
        //state = voice.noteOff(note_event->key,note_event->velocity);
        state = voice.noteOff(event.index,event.value);
        break;
      }
      case CLAP_EVENT_NOTE_CHOKE: {
        //const clap_event_note_t* note_event = (const clap_event_note_t*)header;
        //state = voice.noteChoke(note_event->key,note_event->velocity);
        state = voice.noteChoke(event.index,event.value);
        break;
      }
      //case CLAP_EVENT_NOTE_END:
      case CLAP_EVENT_NOTE_EXPRESSION: {
        //const clap_event_note_expression_t* expression_event = (const clap_event_note_expression_t*)header;
        //voice.noteExpression(expression_event->expression_id,expression_event->value);
        voice.noteExpression(event.index,event.value);
        break;
      }
      case CLAP_EVENT_PARAM_VALUE: {
        //MIP_PRINT;
        //const clap_event_param_value_t* param_event = (const clap_event_param_value_t*)header;
        //voice.parameter(param_event->param_id,param_event->value); // cookie?
        voice.parameter(event.index,event.value);
        break;
      }
      case CLAP_EVENT_PARAM_MOD: {
        //const clap_event_param_mod_t* mod_event = (const clap_event_param_mod_t*)header;
        //voice.modulation(mod_event->param_id,mod_event->amount);
        voice.modulation(event.index,event.value);
        break;
      }
      //case CLAP_EVENT_PARAM_GESTURE_BEGIN:
      //case CLAP_EVENT_PARAM_GESTURE_END:
      //case CLAP_EVENT_TRANSPORT: {
      //case CLAP_EVENT_MIDI: {
      //case CLAP_EVENT_MIDI_SYSEX: {
      //case CLAP_EVENT_MIDI2: {
    } // switch
  }

  //----------

  void handleAllEvents() {
    //MIP_PRINT;
    //const clap_event_header_t* header = nullptr;
    //while (events.read(&header)) {
    //  handleEvent(header);
    //}
    MIP_VoiceEvent event;
    while (events.read(&event)) {
      handleEvent(event);
    }
    uint32_t length = context->process_context->process->frames_count;
    state = voice.process(state,0,length);
  }

  //----------

  void handleInterleavedEvents() {
    //MIP_PRINT;
    uint32_t current_time = 0;
    uint32_t remaining = context->process_context->process->frames_count;
    //const clap_event_header_t* header = nullptr;
    MIP_VoiceEvent event = {};
    while (remaining > 0) {
      //if (events.read(&header)) {
      if (events.read(&event)) {
        //MIP_Print("type %i time %i\n",header->type,header->time);
        MIP_Print("type %i time %i\n",event.type,event.time);
        int32_t length = event.time - current_time;
        //MIP_Print("length %i\n",length);
        if (length > 0) {
          state = voice.process(state,current_time,length);
          remaining -= length;
          current_time += length;
        }
        handleEvent(event);
      } // event
      else {
        // no more events
        int32_t length = remaining;
        state = voice.process(state,current_time,length);
        remaining -= length;
        current_time += length;
      } // !event
    } // remaining > 0

    MIP_Assert( events.read(&event) == false );

  }

  //----------

  void handleQuantizedEvents() {
    //MIP_PRINT;
    //const clap_event_header_t* header = nullptr;
    //while (events.read(&header)) {
    //  handleEvent(header);
    //}
  }

};

//----------------------------------------------------------------------
#endif
