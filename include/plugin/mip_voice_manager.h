#ifndef mip_voice_manager_incuded
#define mip_voice_manager_incuded
//----------------------------------------------------------------------

#include "plugin/mip_note.h"
#include "plugin/mip_voice.h"

typedef MIP_Queue<MIP_Note,1024> MIP_NoteQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int COUNT>
class MIP_VoiceManager {

//------------------------------
private:
//------------------------------

  MIP_Voice<VOICE>              MVoices[COUNT]          = {};
  MIP_VoiceContext              MVoiceContext           = {};
  MIP_NoteQueue                 MNoteEndQueue           = {};

  const clap_plugin_t*          MClapPlugin             = nullptr;
  const clap_host_t*            MClapHost               = nullptr;
  const clap_host_thread_pool*  MThreadPool             = nullptr;

  uint32_t                      MNumPlayingVoices       = 0;
  uint32_t                      MNumReleasedVoices      = 0;

  bool                          MProcessThreaded        = false;
  uint32_t                      MNumThreadedVoices      = 0;
  uint32_t                      MThreadedVoices[COUNT]  = {};

//------------------------------
public:
//------------------------------

  MIP_VoiceManager() {
  }

  //----------

  ~MIP_VoiceManager() {
  }

//------------------------------
public:
//------------------------------

  void init(const clap_plugin_t* APlugin, const clap_host_t* AHost) {
    MClapPlugin = APlugin;
    MClapHost = AHost;
  }

  //----------

  void activate(double ASampleRate, uint32_t AMinFrameSize, uint32_t AMaxFrameSize) {
  }

  //------------------------------
  //
  //------------------------------

  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  void processNoteOn(const clap_event_note_t* event) {}
  void processNoteOff(const clap_event_note_t* event) {}
  void processNoteChoke(const clap_event_note_t* event) {}
  void processNoteEnd(const clap_event_note_t* event) {}
  void processNoteExpression(const clap_event_note_expression_t* event) {}
  void processParamValue(const clap_event_param_value_t* event) {}
  void processParamMod(const clap_event_param_mod_t* event) {}
  void processParamGestureBegin(const clap_event_param_gesture_t* event) {}
  void processParamGestureEnd(const clap_event_param_gesture_t* event) {}
  void processTransport(const clap_event_transport_t* event) {}
  void processMidi(const clap_event_midi_t* event) {}
  void processMidiSysex(const clap_event_midi_sysex_t* event) {}
  void processMidi2(const clap_event_midi2_t* event) {}

  //----------

  //void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  //  uint32_t num_events = in_events->size(in_events);
  //  for (uint32_t i=0; i<num_events; i++) {
  //    const clap_event_header_t* header = in_events->get(in_events,i);
  //    if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
  //      processEvent(header);
  //    }
  //  }
  //}

  //void processEvent(const clap_event_header_t* header) {
  //  switch (header->type) {
  //    case CLAP_EVENT_NOTE_ON:              processNoteOnEvent(             (const clap_event_note_t*)            header  );  break;
  //    case CLAP_EVENT_NOTE_OFF:             processNoteOffEvent(            (const clap_event_note_t*)            header  );  break;
  //    case CLAP_EVENT_NOTE_CHOKE:           processNoteChokeEvent(          (const clap_event_note_t*)            header  );  break;
  //    case CLAP_EVENT_NOTE_END:             processNoteEndEvent(            (const clap_event_note_t*)            header  );  break;
  //    case CLAP_EVENT_NOTE_EXPRESSION:      processNoteExpressionEvent(     (const clap_event_note_expression_t*) header  );  break;
  //    case CLAP_EVENT_PARAM_VALUE:          processParamValueEvent(         (const clap_event_param_value_t*)     header  );  break;
  //    case CLAP_EVENT_PARAM_MOD:            processParamModEvent(           (const clap_event_param_mod_t*)       header  );  break;
  //    case CLAP_EVENT_PARAM_GESTURE_BEGIN:  processParamGestureBeginEvent(  (const clap_event_param_gesture_t*)   header  );  break;
  //    case CLAP_EVENT_PARAM_GESTURE_END:    processParamGestureEndEvent(    (const clap_event_param_gesture_t*)   header  );  break;
  //    case CLAP_EVENT_TRANSPORT:            processTransportEvent(          (const clap_event_transport_t*)       header  );  break;
  //    case CLAP_EVENT_MIDI:                 processMidiEvent(               (const clap_event_midi_t*)            header  );  break;
  //    case CLAP_EVENT_MIDI_SYSEX:           processMidiSysexEvent(          (const clap_event_midi_sysex_t*)      header  );  break;
  //    case CLAP_EVENT_MIDI2:                processMidi2Event(              (const clap_event_midi2_t*)           header  );  break;
  //  }
  //}


  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    for (uint32_t i=0; i<COUNT; i++) {
      //if (MVoices[i].state == MIP_VOICE_WAITING) {
      //}
      if (MVoices[i].state == MIP_VOICE_FINISHED) {
        MVoices[i].state = MIP_VOICE_OFF;
        MThreadedVoices[MNumThreadedVoices++] = i;
      }
    }
    flushNoteEnds(out_events);
  }

  //------------------------------
  //
  //------------------------------

  void processAudioBlock(MIP_ProcessContext* AProcessContext) {
    MVoiceContext.process_context = AProcessContext;

    // set up threaded voices

    MNumThreadedVoices = 0;
    for (uint32_t i=0; i<COUNT; i++) {
      if ((MVoices[i].state == MIP_VOICE_WAITING) || (MVoices[i].state == MIP_VOICE_PLAYING) || (MVoices[i].state == MIP_VOICE_RELEASED)) {
        MThreadedVoices[MNumThreadedVoices++] = i;
      }
    }

    // process threaded voices

    bool processed = false;
    if (MProcessThreaded) {
      // thread pool
      processed = MThreadPool->request_exec(MClapHost,MNumThreadedVoices);
    }
    if (!processed) {
      // manual
      for (uint32_t i=0; i<MNumThreadedVoices; i++) {
        uint32_t v = MThreadedVoices[i];
        process_voice(v);
      }
    }

    //post process

    post_process_voices();
  }

//------------------------------
public: // thread-pool
//------------------------------

  // AIndex = task_index

  void thread_pool_exec(uint32_t AIndex) {
    uint32_t v = MThreadedVoices[AIndex];
    process_voice(v);
  }

//------------------------------
private:
//------------------------------

  // (potentially) called in separate threads for each voice

  void process_voice(uint32_t i) {
    MVoices[i].process(&MVoiceContext);
  };

  //----------

  void post_process_voices() {
    // mix
  };

//------------------------------
private:
//------------------------------

  void queueNoteEnd(MIP_Note ANote) {
    MNoteEndQueue.write(ANote);
  }

  //----------

  void flushNoteEnds(const clap_output_events_t* out_events) {
    MIP_Note note = {0};
    while (MNoteEndQueue.read(&note)) {
      clap_event_note_t note_event;
      note_event.header.flags     = 0;
      note_event.header.size      = sizeof(clap_event_note_t);
      note_event.header.space_id  = CLAP_CORE_EVENT_SPACE_ID;
      note_event.header.time      = 0;
      note_event.header.type      = CLAP_EVENT_NOTE_END;
      note_event.port_index       = note.port;
      note_event.channel          = note.channel;
      note_event.key              = note.key;
      note_event.note_id          = note.noteid;
      note_event.velocity         = 0.0;
      out_events->try_push(out_events,&note_event.header);
    }
  }

};

//----------------------------------------------------------------------
#endif
