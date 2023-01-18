#ifndef mip_voice_included
#define mip_voice_included
//----------------------------------------------------------------------

#include "plugin/mip_process_context.h"
#include "plugin/mip_voice_context.h"

//----------------------------------------------------------------------

struct MIP_VoiceEvent {
  uint32_t time   = 0;
  uint32_t type   = 0;
  uint32_t index  = 0;
  double   value  = 0.0;
  MIP_VoiceEvent(uint32_t ty=0, uint32_t ti=0, uint32_t in=0, double va=0.0) {
    type  = ty;
    time  = ti;
    index = in;
    value = va;
  }
};

typedef MIP_Queue<MIP_VoiceEvent,MIP_VOICE_MAX_EVENTS_PER_BLOCK> MIP_VoiceEventQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE>
class MIP_Voice {

//------------------------------
public:
//------------------------------

  __MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
  VOICE               voice       = {};

  MIP_VoiceContext*   context     = nullptr;
  MIP_Note            note        = {};
  uint32_t            state       = MIP_VOICE_OFF;
  uint32_t            event_mode  = MIP_VOICE_EVENT_MODE_BLOCK;
  MIP_VoiceEventQueue events      = {};

//MIP_ParameterArray* parameters  = nullptr;

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
    //parameters = AParameters;
    context = AContext;
    voice.init(AIndex,context);
  }

  //----------

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

  //----------

  float getEnvLevel() {
    return voice.getEnvLevel();
  }

//------------------------------
private:
//------------------------------

  void handleEvent(MIP_VoiceEvent event) {
    switch(event.type) {
      case CLAP_EVENT_NOTE_ON: {
        state = voice.noteOn(event.index,event.value);
        break;
      }
      case CLAP_EVENT_NOTE_OFF: {
        state = voice.noteOff(event.index,event.value);
        break;
      }
      case CLAP_EVENT_NOTE_CHOKE: {
        /*state =*/ voice.noteChoke(event.index,event.value);
        break;
      }
      //case CLAP_EVENT_NOTE_END:
      case CLAP_EVENT_NOTE_EXPRESSION: {
        /*state =*/ voice.noteExpression(event.index,event.value);
        break;
      }
      case CLAP_EVENT_PARAM_VALUE: {
        voice.parameter(event.index,event.value);
        break;
      }
      case CLAP_EVENT_PARAM_MOD: {
        voice.modulation(event.index,event.value);
        break;
      }
      //case CLAP_EVENT_PARAM_GESTURE_BEGIN:
      //case CLAP_EVENT_PARAM_GESTURE_END:
      //case CLAP_EVENT_TRANSPORT:
      //case CLAP_EVENT_MIDI:
      //case CLAP_EVENT_MIDI_SYSEX:
      //case CLAP_EVENT_MIDI2:
    } // switch
  }

  //----------

  void handleAllEvents() {
    MIP_VoiceEvent event;
    while (events.read(&event)) handleEvent(event);

    uint32_t length = context->process_context->block_length;

    state = voice.process(state,0,length);
  }

  //----------

  void handleInterleavedEvents() {
    uint32_t current_time = 0;

    uint32_t remaining = context->process_context->block_length;

    MIP_VoiceEvent event = {};
    while (remaining > 0) {
      if (events.read(&event)) {
        //MIP_Print("event\n");
        int32_t length = event.time - current_time;
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
    //MIP_Assert( events.read(&event) == false );
  }

  //----------

  void handleQuantizedEvents() {

    uint32_t buffer_length = context->process_context->block_length;

    uint32_t        current_time  = 0;
    uint32_t        remaining     = buffer_length;
    uint32_t        next_event    = 0;
    MIP_VoiceEvent  event         = {};
    if (events.read(&event)) {
      next_event = event.time;
      do {
        // process events for next slice
        while (next_event < (current_time + MIP_AUDIO_SLICE_SIZE)) {
          handleEvent(event);
          if (events.read(&event)) next_event = event.time;
          else next_event = buffer_length; // ???
        }
        // process next slice
        if (remaining < MIP_AUDIO_SLICE_SIZE) {
          state = voice.process(state,current_time,remaining);
          current_time += remaining;
          remaining = 0;
        }
        else {
          state = voice.processSlice(state,current_time);
          current_time += MIP_AUDIO_SLICE_SIZE;
          remaining -= MIP_AUDIO_SLICE_SIZE;
        }
      } while (remaining > 0);
    }
    else {
      // no events..
      do {
        if (remaining < MIP_AUDIO_SLICE_SIZE) state = voice.process(state,current_time,remaining);
        else state = voice.processSlice(state,current_time);
        current_time += MIP_AUDIO_SLICE_SIZE;
        remaining -= MIP_AUDIO_SLICE_SIZE;
      } while (remaining > 0);
    }
    //MIP_Assert( events.read(&event) == false );
  }

  //----------

};

//----------------------------------------------------------------------
#endif
