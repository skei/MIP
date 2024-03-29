#ifndef mip_voice_manager_incuded
#define mip_voice_manager_incuded
//----------------------------------------------------------------------

/*
*/

//----------------------------------------------------------------------

#include "plugin/mip_note.h"
#include "plugin/mip_voice.h"

//----------

typedef MIP_Queue<MIP_Note,MIP_VOICE_MANAGER_MAX_EVENTS_PER_BLOCK> MIP_NoteQueue;

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

  __MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
  float MVoiceBuffer[COUNT * MIP_AUDIO_MAX_BLOCK_SIZE]  = {0};

  uint32_t                      MNumPlayingVoices       = 0;
  uint32_t                      MNumReleasedVoices      = 0;

  MIP_NoteQueue                 MNoteEndQueue           = {};

  const clap_plugin_t*          MClapPlugin             = nullptr;
  const clap_host_t*            MClapHost               = nullptr;
  const clap_host_thread_pool*  MThreadPool             = nullptr;

  bool                          MProcessThreaded        = false;
  uint32_t                      MNumActiveVoices        = 0;
  uint32_t                      MActiveVoices[COUNT]    = {};

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

  void setEventMode(uint32_t AMode) {
    for (uint32_t i=0; i<COUNT; i++) {
      MVoices[i].event_mode = AMode;
    }
  }

  uint32_t getMaxVoices() {
    return COUNT;
  }

  uint32_t getNumPlayingVoices() {
    return MNumPlayingVoices;
  }

  uint32_t getNumReleasedVoices() {
    return MNumReleasedVoices;
  }

  uint32_t getVoiceState(uint32_t AIndex) {
    return MVoices[AIndex].state;
  }

//------------------------------
public:
//------------------------------

  void init(const clap_plugin_t* APlugin, const clap_host_t* AHost) {
    MClapPlugin = APlugin;
    MClapHost = AHost;
    if (MClapHost) {
      MThreadPool = (const clap_host_thread_pool*)MClapHost->get_extension(MClapHost,CLAP_EXT_THREAD_POOL);
      LOG.print("VOICE MANAGER: thread-pool: %s\n", MThreadPool ? "true" : "false" );
    }
  }

  //----------

  void activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count, MIP_ParameterArray* AParameters) {
    //MIP_Print("AParameters %p\n",AParameters);
    MVoiceContext.process_context   = nullptr; //
    MVoiceContext.voice_buffer      = MVoiceBuffer;
    MVoiceContext.min_frames_count  = min_frames_count;
    MVoiceContext.max_frames_count  = max_frames_count;
    MVoiceContext.samplerate        = sample_rate;
// !!
    MVoiceContext.parameters        = AParameters; // already in MIP_ProcessContext?
//
    for (uint32_t i=0; i<COUNT; i++) {
      MVoices[i].init(i,&MVoiceContext);
    }
  }

//------------------------------
public:
//------------------------------

  /*
    TODO:
    - redo the noteid/pck/global selector..
      any -1 means 'don't care about it'
      anytihng else = 'i DO care about it'
      check passes if both are equal or 'selector' is -1
  */

  //----------

  bool isGlobal(uint32_t index, int32_t noteid, int32_t port, int32_t channel, int32_t key) {
    if ((noteid  == -1) && (port == -1) && (channel == -1) && (key == -1)) return true;
    return false;
  }

  //----------

  bool isTargeted(uint32_t index, int32_t noteid, int32_t port, int32_t channel, int32_t key) {
    if ( ((noteid  == -1) || (MVoices[index].note.noteid  == noteid))
      && ((port    == -1) || (MVoices[index].note.port    == port))
      && ((channel == -1) || (MVoices[index].note.channel == channel))
      && ((key     == -1) || (MVoices[index].note.key     == key)) ) {
      return true;
    }
    return false;
  }

  //----------

  // process -> prepare

  void processNoteOn(const clap_event_note_t* event) {
    int32_t voice = findFreeVoice(MIP_VOICE_MANAGER_STEAL_VOICES);
    if (voice >= 0) {
      MVoices[voice].state        = MIP_VOICE_WAITING;
      MVoices[voice].note.port    = event->port_index;
      MVoices[voice].note.channel = event->channel;
      MVoices[voice].note.key     = event->key;
      MVoices[voice].note.noteid  = event->note_id;
      MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_ON, event->header.time, event->key,event->velocity);
      MVoices[voice].events.write(ve);
    }
  }

  //----------

  void processNoteOff(const clap_event_note_t* event) {

    bool has_noteid = (event->note_id != -1);
    bool has_pck = ((event->port_index != -1) && (event->channel != -1) && (event->key != -1));

    for (int32_t voice=0; voice<COUNT; voice++) {

      //if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
      //  MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_OFF, event->header.time, event->key, event->velocity);
      //  MVoices[voice].events.write(ve);
      //}

      if (has_noteid && (event->note_id == MVoices[voice].note.noteid)) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_OFF, event->header.time, event->key, event->velocity);
        MVoices[voice].events.write(ve);
      }
      else if (has_pck && ((event->port_index == MVoices[voice].note.port) && (event->channel == MVoices[voice].note.channel) && (event->key == MVoices[voice].note.key))) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_OFF, event->header.time, event->key, event->velocity);
        MVoices[voice].events.write(ve);
      }

      //else {
      //  MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_OFF, event->header.time, event->key, event->velocity);
      //  MVoices[voice].events.write(ve);
      //}

    }
  }

  //----------

  void processNoteChoke(const clap_event_note_t* event) {
    bool has_noteid = (event->note_id != -1);
    bool has_pck = ((event->port_index != -1) && (event->channel != -1) && (event->key != -1));
    for (int32_t voice=0; voice<COUNT; voice++) {

      //if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
      //  MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_CHOKE, event->header.time, event->key, event->velocity);
      //  MVoices[voice].events.write(ve);
      //}

      if (has_noteid && (event->note_id == MVoices[voice].note.noteid)) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_CHOKE, event->header.time, event->key, event->velocity);
        MVoices[voice].events.write(ve);
      }
      else if (has_pck && ((event->port_index == MVoices[voice].note.port) && (event->channel == MVoices[voice].note.channel) && (event->key == MVoices[voice].note.key))) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_CHOKE, event->header.time, event->key, event->velocity);
        MVoices[voice].events.write(ve);
      }

      //else {
      //  MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_CHOKE, event->header.time, event->key, event->velocity);
      //  MVoices[voice].events.write(ve);
      //}
    }
  }

  //----------

  void processNoteExpression(const clap_event_note_expression_t* event) {
    bool has_noteid = (event->note_id != -1);
    bool has_pck = ((event->port_index != -1) && (event->channel != -1) && (event->key != -1));
    for (int32_t voice=0; voice<COUNT; voice++) {

      //if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
      //  MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_EXPRESSION, event->header.time, event->expression_id, event->value);
      //  MVoices[voice].events.write(ve);
      //}

      if (has_noteid && (event->note_id == MVoices[voice].note.noteid)) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_EXPRESSION, event->header.time, event->expression_id, event->value);
        MVoices[voice].events.write(ve);
      }
      else if (has_pck && ((event->port_index == MVoices[voice].note.port) && (event->channel == MVoices[voice].note.channel) && (event->key == MVoices[voice].note.key))) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_EXPRESSION, event->header.time, event->expression_id, event->value);
        MVoices[voice].events.write(ve);
      }

      //else {
      //  MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_EXPRESSION, event->header.time, event->expression_id, event->value);
      //  MVoices[voice].events.write(ve);
      //}
    }
  }

  //----------

  void processParamValue(const clap_event_param_value_t* event) {
    bool has_noteid = (event->note_id != -1);
    bool has_pck = ((event->port_index != -1) && (event->channel != -1) && (event->key != -1));
    for (int32_t voice=0; voice<COUNT; voice++) {

      //if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
      //  MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_NOTE_EXPRESSION, event->header.time, event->expression_id, event->value);
      //  MVoices[voice].events.write(ve);
      //}

      if (has_noteid && (event->note_id == MVoices[voice].note.noteid)) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_PARAM_VALUE, event->header.time, event->param_id, event->value);
        MVoices[voice].events.write(ve);
      }
      else if (has_pck && ((event->port_index == MVoices[voice].note.port) && (event->channel == MVoices[voice].note.channel) && (event->key == MVoices[voice].note.key))) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_PARAM_VALUE, event->header.time, event->param_id, event->value);
        MVoices[voice].events.write(ve);
      }

      #ifdef MIP_VOICE_MANAGER_SEND_GLOBAL_PARAMS_TO_ALL_VOICES
      else {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_PARAM_VALUE, event->header.time, event->param_id, event->value);
        MVoices[voice].events.write(ve);
      }
      #endif

    }
  }

  //----------

  /*
    see param_value
  */

  void processParamMod(const clap_event_param_mod_t* event) {
    bool has_noteid = (event->note_id != -1);
    bool has_pck = ((event->port_index != -1) && (event->channel != -1) && (event->key != -1));
    for (int32_t voice=0; voice<COUNT; voice++) {

      //if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
      //  MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_PARAM_MOD, event->header.time, event->param_id, event->amount);
      //  MVoices[voice].events.write(ve);
      //}


      if (has_noteid) {
        if (event->note_id == MVoices[voice].note.noteid) {
          MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_PARAM_MOD, event->header.time, event->param_id, event->amount);
          MVoices[voice].events.write(ve);
        }
      }
      else if (has_pck && ((event->port_index == MVoices[voice].note.port) && (event->channel == MVoices[voice].note.channel) && (event->key == MVoices[voice].note.key))) {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_PARAM_MOD, event->header.time, event->param_id, event->amount);
        MVoices[voice].events.write(ve);
      }

      #ifdef MIP_VOICE_MANAGER_SEND_GLOBAL_MODS_TO_ALL_VOICES
      else {
        MIP_VoiceEvent ve = MIP_VoiceEvent(CLAP_EVENT_PARAM_MOD, event->header.time, event->param_id, event->amount);
        MVoices[voice].events.write(ve);
      }
      #endif

    }
  }

  //----------

  void processMidi(const clap_event_midi_t* event) {
    #ifdef MIP_VOICE_MANAGER_CONVERT_MIDI
    uint8_t msg   = event->data[0] & 0xf0;
    uint8_t chan  = event->data[0] & 0x0f;
    uint8_t index = event->data[1]; // & 0x7f;
    uint8_t val   = event->data[2]; // & 0x7f;
    switch (msg) {
      case MIP_MIDI_NOTE_OFF:
        MIP_Print("MIDI NOTE OFF chan %i index %i val %i\n",chan,index,val);
        //processNoteOff
        break;
      case MIP_MIDI_NOTE_ON:
        MIP_Print("MIDI NOTE ON chan %i index %i val %i\n",chan,index,val);
        //processNoteOn
        break;
      case MIP_MIDI_POLY_AFTERTOUCH:
        MIP_Print("MIDI POLY AFTERTOUCH chan %i index %i val %i\n",chan,index,val);
        //processNoteExpression
        break;
      case MIP_MIDI_CONTROL_CHANGE:
        MIP_Print("MIDI CONTROL CHANGE chan %i index %i val %i\n",chan,index,val);
        // 74 : processNoteExpression
        // midi mapping
        break;
      case MIP_MIDI_PROGRAM_CHANGE:
        MIP_Print("MIDI PROGRAM CHANGE chan %i index %i val %i\n",chan,index,val);
        // ?
        break;
      case MIP_MIDI_CHANNEL_AFTERTOUCH:
        MIP_Print("MIDI CHANNEL AFTERTOUCH chan %i index %i val %i\n",chan,index,val);
        //processNoteExpression
        break;
      case MIP_MIDI_PITCHBEND:
        MIP_Print("MIDI PITCHBEND chan %i index %i val %i\n",chan,index,val);
        //processNoteExpression
        break;
      case MIP_MIDI_SYS:
        MIP_Print("MIDI SYS chan %i index %i val %i\n",chan,index,val);
        //processMidiSysex
        break;
    }
    #endif
  }

  //----------

  void processMidiSysex(const clap_event_midi_sysex_t* event) {
  }

  //----------

  void processMidi2(const clap_event_midi2_t* event) {
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
        queueNoteEnd(MVoices[i].note);      // ???
        //MIP_Print("voice %i -> OFF\n",i);
      }
      if (MVoices[i].state == MIP_VOICE_FINISHED) {
        MVoices[i].state = MIP_VOICE_OFF;
        queueNoteEnd(MVoices[i].note);
      }
    }
    flushNoteEnds(out_events);
  }

//------------------------------
public:
//------------------------------

  void processAudioBlock(MIP_ProcessContext* AProcessContext) {

    MVoiceContext.process_context = AProcessContext;
    uint32_t blocksize = AProcessContext->block_length;
    float** output = AProcessContext->block_buffer;

    MIP_ClearStereoBuffer(output,blocksize);

    // set up active voices
    MNumActiveVoices = 0;
    uint32_t num_playing = 0;
    uint32_t num_released = 0;
    for (uint32_t i=0; i<COUNT; i++) {
      if ((MVoices[i].state != MIP_VOICE_OFF) && (MVoices[i].state != MIP_VOICE_FINISHED)) {
        MActiveVoices[MNumActiveVoices++] = i;
        if (MVoices[i].state == MIP_VOICE_PLAYING) num_playing += 1;
        if (MVoices[i].state == MIP_VOICE_RELEASED) num_released += 1;
      }
    }
    MNumPlayingVoices = num_playing;
    MNumReleasedVoices = num_released;

    // process active voices
    if (MNumActiveVoices > 0) {
      // thread-pool..
      bool processed = false;
      if (MProcessThreaded && MThreadPool) {
        processed = MThreadPool->request_exec(MClapHost,MNumActiveVoices);
        //MIP_Print("request_exec(%i) returned %s\n", MNumActiveVoices, processed ? "true" : "false" );
      }
      // ..or manually
      if (!processed) {
        for (uint32_t i=0; i<MNumActiveVoices; i++) {
          uint32_t v = MActiveVoices[i];
          process_voice(v);
        }
      }
      // mix, (post-process)

      for (uint32_t i=0; i<MNumActiveVoices; i++) {
        uint32_t voice = MActiveVoices[i];
        float* buffer = MVoiceBuffer;
        buffer += (voice * MIP_AUDIO_MAX_BLOCK_SIZE);// * MVoiceContext->); // * block_size? process->max_frames_count?
        MIP_AddMonoToStereoBuffer(output,buffer,blocksize);
      }

    } // num voices > 0
  }

//------------------------------
public:
//------------------------------

  // called by host
  // separate thread for each task/voice

  void thread_pool_exec(uint32_t AIndex) {
    uint32_t v = MActiveVoices[AIndex];
    process_voice(v);
  }

//------------------------------
//public:
private:
//------------------------------

  // if threaded: separate threads for each voice
  // else:        one voice after another

  void process_voice(uint32_t i) {
    MVoices[i].process();
  };

  //----------

  int32_t findFreeVoice(bool AReleased=false) {
    for (uint32_t i=0; i<COUNT; i++) {
      if ((MVoices[i].state == MIP_VOICE_OFF) /*|| (MVoices[i].state == MIP_VOICE_FINISHED)*/) {
        return i;
      }
    }
    if (AReleased) {
      int32_t lowest_index = -1;
      double  lowest_level = 666.0;
      for (uint32_t i=0; i<COUNT; i++) {
        if (MVoices[i].state == MIP_VOICE_RELEASED) {
          float env_level = MVoices[i].getEnvLevel();
          if (env_level < lowest_level) {
            lowest_index = i;
            lowest_level = env_level;
          }
        }
      }
      if (lowest_index >= 0) {
        // kill released note..
        queueNoteEnd( MVoices[lowest_index].note );
        return lowest_index;
      }
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
