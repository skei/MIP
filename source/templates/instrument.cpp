
// nc -U -l -k /tmp/mip.socket
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
#endif

//----------------------------------------------------------------------

#include "audio/mip_audio_math.h"
#include "audio/mip_audio_utils.h"
#include "plugin/mip_plugin.h"
#include "gui/mip_widgets.h"
#include "audio/mip_audio_utils.h"

#include "plugin/mip_voice_manager.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t my_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/my_plugin/0.0.0",
  .name         = "my_plugin",
  .vendor       = "me",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.0",
  .description  = "my amazing plugin!",
  .features     = (const char*[]){CLAP_PLUGIN_FEATURE_INSTRUMENT,nullptr}
};

#define MY_PLUGIN_EDITOR_WIDTH    400
#define MY_PLUGIN_EDITOR_HEIGHT   500
#define MY_PLUGIN_MAX_VOICES      32

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {

//------------------------------
private:
//------------------------------

  uint32_t          MIndex    = 0;
  MIP_VoiceContext* MContext  = nullptr;

//------------------------------
private:
//------------------------------

  // 'synth'

  double ph = 0.0;
  double phadd = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, MIP_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
  }

  //----------

  uint32_t noteOn(uint32_t AKey, double AVelocity) {
    float hz = MIP_NoteToHz(AKey);
    float srate = MContext->samplerate;
    ph = 0;
    phadd = hz / srate;
    return MIP_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AKey, double AVelocity) {
    return MIP_VOICE_FINISHED;
  }

  //----------

  void noteChoke(uint32_t AKey, double AVelocity) {
  }

  //----------

  void noteExpression(uint32_t AExpression, double AValue) {
  }

  //----------

  void parameter(uint32_t AIndex, double AValue) {
  }

  //----------

  void modulation(uint32_t AIndex, double AValue) {
  }

  //----------

  // don't touch stuff other voices might be touching!
  // calculate to separate buffer!

  void process() {
    float* out = MContext->buffer;
    out += (MIndex * MIP_VOICE_MANAGER_MAX_FRAME_BUFFER_SIZE);
    uint32_t num = MContext->process_context->process->frames_count;
    for (uint32_t i=0; i<num; i++) {
      ph = MIP_Fract(ph);

      //float v = sin(ph * MIP_PI2);
      float v = (ph * 2.0) - 1.0;  // 0..1 -> -1..1

      { // test: waste cpu
        #define NUM 250
        float p = ph - (phadd * 0.5);
        float pa = (phadd / NUM);
        for (uint32_t j=0; j<NUM; j++) {
          v += sin(p * MIP_PI2) / NUM;
          p += pa;
        }
        #undef NUM
      }

      *out++ = v * 0.1;
      ph += phadd;
    }
  }

  //----------

};

//----------

typedef MIP_VoiceManager<myVoice,MY_PLUGIN_MAX_VOICES> myVoiceManager;

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class my_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  //double gain = 0.0;

  myVoiceManager  MVoiceManager = {};

//------------------------------
public:
//------------------------------

  my_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    setInitialEditorSize(MY_PLUGIN_EDITOR_WIDTH,MY_PLUGIN_EDITOR_HEIGHT);
    //MVoiceManager->setThreadPool(MThreadPool);
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    bool result = MIP_Plugin::init();
    if (result) {
      appendNoteInput();
      appendStereoOutput();

      MIP_Parameter* param = appendParameter( new MIP_Parameter("Gain", 0.5) );
      param->setFlag(CLAP_PARAM_IS_MODULATABLE);
      param->setFlag(CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID);

      MVoiceManager.init(getClapPlugin(),getClapHost());
    }
    return result;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    bool result = MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
    if (result) {
      MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    }
    return result;
  }

  //----------

  bool gui_create(const char* api, bool is_floating) final {
    MIsEditorOpen = false;
    MEditor = new MIP_Editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters);
    if (MEditor) {
      MIP_PanelWidget* background = new MIP_PanelWidget(MIP_DRect(0,0, MY_PLUGIN_EDITOR_WIDTH, MY_PLUGIN_EDITOR_HEIGHT));
      MEditor->setRootWidget(background);
      //MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_DRect(50,50,300,400) );
      //background->appendChildWidget(knob);
      //knob->setArcThickness(40);
      //knob->setValueSize(75);
      //MEditor->connect( knob, MParameters[0] );
    }
    return (MEditor);
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.thread_pool_exec(task_index);
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = MY_PLUGIN_MAX_VOICES;
    info->voice_capacity  = MY_PLUGIN_MAX_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

//------------------------------
public: // events
//------------------------------

  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.preProcessEvents(in_events,out_events);
  }

  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.postProcessEvents(in_events,out_events);
  }

  //----------

  void processNoteOn(const clap_event_note_t* event) final {
    MVoiceManager.processNoteOn(event);
  }

  void processNoteOff(const clap_event_note_t* event) final {
    MVoiceManager.processNoteOff(event);
  }

  void processNoteChoke(const clap_event_note_t* event) final {
    MVoiceManager.processNoteChoke(event);
  }

  //void processNoteEnd(const clap_event_note_t* event) final {
  //  MVoiceManager.processNoteEnd(event);
  //}

  void processNoteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.processNoteExpression(event);
  }

  void processParamValue(const clap_event_param_value_t* event) final {
    MVoiceManager.processParamValue(event);
  }

  void processParamMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.processParamMod(event);
  }

  //void processParamGestureBegin(const clap_event_param_gesture_t* event) final {
  //  MVoiceManager.processParamGestureBegin(event);
  //}

  //void processParamGestureEnd(const clap_event_param_gesture_t* event) final {
  //  MVoiceManager.processParamGestureEnd(event);
  //}

  //void processTransport(const clap_event_transport_t* event) final {
  //  MVoiceManager.processTransport(event);
  //}

  void processMidi(const clap_event_midi_t* event) final {
    MVoiceManager.processMidi(event);
  }

  void processMidiSysex(const clap_event_midi_sysex_t* event) final {
    MVoiceManager.processMidiSysex(event);
  }

  void processMidi2(const clap_event_midi2_t* event) final {
    MVoiceManager.processMidi2(event);
  }

//------------------------------
public: // audio
//------------------------------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    MVoiceManager.processAudioBlock(AContext);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

//#ifndef MIP_NO_ENTRY

  #include "plugin/mip_entry.h"
  MIP_DEFAULT_ENTRY(my_descriptor,my_plugin);

//#endif // MIP_NO_ENTRY

