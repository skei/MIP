#ifndef mip_voice_manager_incuded
#define mip_voice_manager_incuded
//----------------------------------------------------------------------

#include "plugin/mip_note.h"
#include "plugin/mip_voice.h"

//typedef MIP_Queue<MIP_Note,MIP_VOICE_MANAGER_MAX_EVENTS_PER_BLOCK> MIP_NoteQueue;
typedef MIP_Queue<MIP_Note,MIP_PLUGIN_MAX_PARAM_EVENTS> MIP_NoteQueue;

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

  MIP_Voice<VOICE>              MVoices[COUNT]        = {};
  MIP_VoiceContext              MVoiceContext         = {};
  float                         MVoiceBuffer[COUNT * MIP_VOICE_MANAGER_MAX_FRAME_BUFFER_SIZE] = {0};


  MIP_NoteQueue                 MNoteEndQueue         = {};

  const clap_plugin_t*          MClapPlugin           = nullptr;
  const clap_host_t*            MClapHost             = nullptr;
  const clap_host_thread_pool*  MThreadPool           = nullptr;

  bool                          MProcessThreaded      = false;
  uint32_t                      MNumActiveVoices      = 0;
  uint32_t                      MActiveVoices[COUNT]  = {};

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

  void setProcessThreaded(bool AThreaded=true) {
    MProcessThreaded = AThreaded;
  }

  //----------

  //#define MIP_VOICE_EVENT_MODE_BLOCK       0
  //#define MIP_VOICE_EVENT_MODE_INTERLEAVED 1
  //#define MIP_VOICE_EVENT_MODE_QUANTIZED   2

  void setEventMode(uint32_t AMode) {
    for (uint32_t i=0; i<COUNT; i++) {
      MVoices[i].event_mode = AMode;
    }
  }

//------------------------------
public:
//------------------------------

  void init(const clap_plugin_t* APlugin, const clap_host_t* AHost) {
    MClapPlugin = APlugin;
    MClapHost = AHost;
    if (MClapHost) {
      MThreadPool = (const clap_host_thread_pool*)MClapHost->get_extension(MClapHost,CLAP_EXT_THREAD_POOL);
    }
  }

  //----------

  void activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    MVoiceContext.process_context   = nullptr; //
    MVoiceContext.buffer            = MVoiceBuffer;
    MVoiceContext.min_frames_count  = min_frames_count;
    MVoiceContext.max_frames_count  = max_frames_count;
    MVoiceContext.samplerate        = sample_rate;
    for (uint32_t i=0; i<COUNT; i++) {
      MVoices[i].init(i,&MVoiceContext);
    }
  }

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

  void processNoteOn(const clap_event_note_t* event) {
    int32_t voice = findFreeVoice();
    if (voice >= 0) {
      //MIP_Print("voice %i\n",voice);
      MVoices[voice].state = MIP_VOICE_WAITING;
      MVoices[voice].env_level = 0.0;
      MVoices[voice].events.write(&event->header);
      MVoices[voice].note.port = event->port_index;
      MVoices[voice].note.channel = event->channel;
      MVoices[voice].note.key = event->key;
      MVoices[voice].note.noteid = event->note_id;
    }
  }

  //----------

  void processNoteOff(const clap_event_note_t* event) {
    int32_t voice = findVoice(event->port_index,event->channel,event->key,event->note_id);
    if (voice >= 0) {
      //MIP_Print("voice %i\n",voice);
      MVoices[voice].events.write(&event->header);
    }
  }

  //----------

  void processNoteChoke(const clap_event_note_t* event) {
    int32_t voice = findVoice(event->port_index,event->channel,event->key,event->note_id);
    if (voice >= 0) {
      //MIP_Print("voice %i\n",voice);
      MVoices[voice].events.write(&event->header);
    }
  }

  //----------

  //void processNoteEnd(const clap_event_note_t* event) {
  //}

  //----------

  void processNoteExpression(const clap_event_note_expression_t* event) {
    int32_t voice = findVoice(event->port_index,event->channel,event->key,event->note_id);
    if (voice >= 0) {
      //MIP_Print("voice %i\n",voice);
      MVoices[voice].events.write(&event->header);
    }
  }

  //----------

  void processParamValue(const clap_event_param_value_t* event) {
    int32_t voice = findVoice(event->port_index,event->channel,event->key,event->note_id);
    //MIP_Print("voice %i\n",voice);
    if (voice >= 0) {
      MVoices[voice].events.write(&event->header);
    }
  }

  //----------

  void processParamMod(const clap_event_param_mod_t* event) {
    int32_t voice = findVoice(event->port_index,event->channel,event->key,event->note_id);
    if (voice >= 0) {
      //MIP_Print("voice %i\n",voice);
      MVoices[voice].events.write(&event->header);
    }
  }

  //----------

  //void processParamGestureBegin(const clap_event_param_gesture_t* event) {
  //  MIP_PRINT;
  //}

  //----------

  //void processParamGestureEnd(const clap_event_param_gesture_t* event) {
  //  MIP_PRINT;
  //}

  //----------

  //void processTransport(const clap_event_transport_t* event) {
  //  //MIP_PRINT;
  //}

  //----------

  void processMidi(const clap_event_midi_t* event) {
    MIP_PRINT;
  }

  //----------

  void processMidiSysex(const clap_event_midi_sysex_t* event) {
    MIP_PRINT;
  }

  //----------

  void processMidi2(const clap_event_midi2_t* event) {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    for (uint32_t i=0; i<COUNT; i++) {
      if (MVoices[i].state == MIP_VOICE_WAITING) {
        // still waiting, not started? something might be wrong..
        MVoices[i].state = MIP_VOICE_OFF;
      }
      if (MVoices[i].state == MIP_VOICE_FINISHED) {
        MVoices[i].state = MIP_VOICE_OFF;
        queueNoteEnd(MVoices[i].note);
        //MNumReleasedVoices -= 1;
      }
    }
    flushNoteEnds(out_events);
  }

//------------------------------
public:
//------------------------------

  void processAudioBlock(MIP_ProcessContext* AProcessContext) {
    MVoiceContext.process_context = AProcessContext;

    uint32_t len = AProcessContext->process->frames_count;
    //float** output = AProcessContext->process->audio_outputs[0].data32;
    //MIP_ClearStereoBuffer(output,len);
    float* out0 = AProcessContext->process->audio_outputs[0].data32[0];
    float* out1 = AProcessContext->process->audio_outputs[0].data32[1];
    MIP_ClearMonoBuffer(out0,len);
    MIP_ClearMonoBuffer(out1,len);

    // set up active voices

    MNumActiveVoices = 0;
    for (uint32_t i=0; i<COUNT; i++) {
      if ((MVoices[i].state == MIP_VOICE_WAITING) || (MVoices[i].state == MIP_VOICE_PLAYING) || (MVoices[i].state == MIP_VOICE_RELEASED)) {
        MActiveVoices[MNumActiveVoices++] = i;
        //MVoices[i].setVoiceBuffer();
      }
    }

    // process active voices

    if (MNumActiveVoices > 0) {

      // thread-pool

      bool processed = false;
      //if (MProcessThreaded && MThreadPool && MClapHost) {
      if (MProcessThreaded && MThreadPool) {
        processed = MThreadPool->request_exec(MClapHost,MNumActiveVoices);
        MIP_Print("request_exec(%i) returned %s\n", MNumActiveVoices, processed ? "true" : "false" );
      }

      // manually

      if (!processed) {
        for (uint32_t i=0; i<MNumActiveVoices; i++) {
          uint32_t v = MActiveVoices[i];
          process_voice(v);
        }
      }

      // mix

      for (uint32_t i=0; i<MNumActiveVoices; i++) {
        uint32_t index = MActiveVoices[i];
        //MVoices[i].getVoiceBuffer();
        float* ptr = MVoiceBuffer;
        ptr += (index * MIP_VOICE_MANAGER_MAX_FRAME_BUFFER_SIZE);
        MIP_AddMonoBuffer(out0,ptr,len);
        MIP_AddMonoBuffer(out1,ptr,len);
      }

    } // num voices > 0
  }

//------------------------------
//private:
public:
//------------------------------

  // (potentially) called in separate threads for each voice

  void process_voice(uint32_t i) {
    //MVoices[i].setVoiceBuffer();
    MVoices[i].process();
  };

  //----------

  // AIndex = task_index

  void thread_pool_exec(uint32_t AIndex) {
    uint32_t v = MActiveVoices[AIndex];
    process_voice(v);
  }

  //----------

  int32_t findVoice(int32_t port, int32_t chan, int32_t key, int32_t id) {
    //MIP_Print("port %i chan %i key %i id %i\n",port,chan,key,id);
    for (uint32_t i=0; i<COUNT; i++) {
      if ((port == -1) || (port == MVoices[i].note.port)) {
        if ((chan == -1) || (chan == MVoices[i].note.channel)) {
          if ((key == -1) || (key == MVoices[i].note.key)) {
            if ((id == -1) || (id == MVoices[i].note.noteid)) {
              return i;
            }
          }
        }
      }
    }
    return -1;
  }

  //----------

  int32_t findFreeVoice(bool AReleased=false) {
    for (uint32_t i=0; i<COUNT; i++) {
      if (MVoices[i].state == MIP_VOICE_OFF) return i;
    }
    if (AReleased) {
      int32_t lowest_index = -1;
      double  lowest_level = 666.0;
      for (uint32_t i=0; i<COUNT; i++) {
        if (MVoices[i].env_level < lowest_level) {
          lowest_index = i;
          lowest_level = MVoices[i].env_level;
        }
      }
      if (lowest_index >= 0) return lowest_index;
    }
    return -1;
  }

  //----------

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
