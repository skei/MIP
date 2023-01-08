#ifndef mip_voice_included
#define mip_voice_included
//----------------------------------------------------------------------

#include "plugin/mip_process_context.h"
#include "plugin/mip_voice_context.h"

//----------------------------------------------------------------------

#define MIP_VOICE_MAX_EVENTS_PER_BLOCK 256
typedef MIP_Queue<const clap_event_header_t*,MIP_VOICE_MAX_EVENTS_PER_BLOCK> MIP_ClapEventQueue;

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

  MYVOICE   voice     = {};
  uint32_t  index     = 0;
  uint32_t  state     = MIP_VOICE_OFF;
  MIP_Note  note      = {0};
  double    env_level = 0.0;

  double velocity     = 0.0;
  double pressure     = 0.0;
  double brightness   = 0.0;
  double tuning       = 0.0;

  MIP_ClapEventQueue  events = {};

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

  void handleAllEvents() {
    const clap_event_header_t* header = nullptr;
    while (events.read(&header)) {
      switch(header->type) {

        case CLAP_EVENT_NOTE_ON: {
          //MIP_Print("%i NOTE_ON\n",index);
          const clap_event_note_t* note_event = (const clap_event_note_t*)header;
          state = voice.noteOn(note_event->key,note_event->velocity);
          break;
        }

        case CLAP_EVENT_NOTE_OFF: {
          //MIP_Print("%i NOTE_OFF\n",index);
          const clap_event_note_t* note_event = (const clap_event_note_t*)header;
          state = voice.noteOff(note_event->key,note_event->velocity);
          break;
        }

        case CLAP_EVENT_NOTE_CHOKE: {
          //MIP_Print("%i NOTE_CHOKE\n",index);
          const clap_event_note_t* note_event = (const clap_event_note_t*)header;
          voice.noteChoke(note_event->key,note_event->velocity);
          state = MIP_VOICE_OFF;
          break;
        }

        //case CLAP_EVENT_NOTE_END: {
        //  MIP_Print("%i NOTE_END\n",index);
        //  break;
        //}

        case CLAP_EVENT_NOTE_EXPRESSION: {
          //MIP_Print("%i NOTE_EXPRESSION\n",index);
          const clap_event_note_expression_t* expression_event = (const clap_event_note_expression_t*)header;
          voice.noteExpression(expression_event->expression_id,expression_event->value);
          break;
        }

        case CLAP_EVENT_PARAM_VALUE: {
          //MIP_Print("%i PARAM_VALUE\n",index);
          const clap_event_param_value_t* param_event = (const clap_event_param_value_t*)header;
          voice.parameter(param_event->param_id,param_event->value); // cookie?
          break;
        }

        case CLAP_EVENT_PARAM_MOD: {
          //MIP_Print("%i PARAM_MOD\n",index);
          const clap_event_param_mod_t* mod_event = (const clap_event_param_mod_t*)header;
          voice.modulation(mod_event->param_id,mod_event->amount);
          break;
        }

        //case CLAP_EVENT_PARAM_GESTURE_BEGIN:
        //  MIP_Print("%i PARAM_GESTURE_BEGIN\n",index);
        //  break;
        //}

        //case CLAP_EVENT_PARAM_GESTURE_END: {
        //  MIP_Print("%i PARAM_GESTURE_END\n",index);
        //  break;
        //}

        //case CLAP_EVENT_TRANSPORT: {
        //  MIP_Print("%i TRANSPORT\n",index);
        //  break;
        //}

        //case CLAP_EVENT_MIDI: {
        //  MIP_Print("%i MIDI\n",index);
        //  break;
        //}

        //case CLAP_EVENT_MIDI_SYSEX: {
        //  MIP_Print("%i MIDI_SYSEX\n",index);
        //  break;
        //}

        //case CLAP_EVENT_MIDI2: {
        //  MIP_Print("%i MIDI2\n",index);
        //  break;
        //}

      }
    }
  }

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, MIP_VoiceContext* AContext) {
    voice.init(AIndex,AContext);
  }



  void process() {
    handleAllEvents();
    voice.process();
  }

};

//----------------------------------------------------------------------
#endif
