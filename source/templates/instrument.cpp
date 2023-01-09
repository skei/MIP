
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

#define MY_PLUGIN_EDITOR_WIDTH    500
#define MY_PLUGIN_EDITOR_HEIGHT   400
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

  uint32_t noteChoke(uint32_t AKey, double AVelocity) {
    return MIP_VOICE_OFF;

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

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {

    float* buffer = MContext->buffer;
    buffer += AOffset;
    buffer += (MIndex * MIP_VOICE_MANAGER_MAX_FRAME_BUFFER_SIZE);

    if ((AState == MIP_VOICE_PLAYING) || (AState == MIP_VOICE_RELEASED)) {

      for (uint32_t i=0; i<ALength; i++) {
        ph = MIP_Fract(ph);

        float v = sin(ph * MIP_PI2);
        //float v = (ph * 2.0) - 1.0;  // 0..1 -> -1..1

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

        *buffer++ = v * 0.1;
        ph += phadd;
      }

    } // playing

    else {
      memset(buffer,0,ALength * sizeof(float));
    }

    return AState;

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

  myVoiceManager        MVoiceManager = {};

  MIP_Parameter*        par_gain    = nullptr;
  MIP_Parameter*        par_thread  = nullptr;
  MIP_Parameter*        par_render  = nullptr;

  MIP_KnobWidget*       wdg_gain    = nullptr;
  MIP_DragValueWidget*  wdg_thread  = nullptr;
  MIP_DragValueWidget*  wdg_render  = nullptr;

  double    p_gain    = 0.0;
  uint32_t  p_thread  = 0;
  uint32_t  p_render  = 0;

//------------------------------
public:
//------------------------------

  my_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    setInitialEditorSize(MY_PLUGIN_EDITOR_WIDTH,MY_PLUGIN_EDITOR_HEIGHT);
    MVoiceManager.setProcessThreaded(true);
  }

//------------------------------
public:
//------------------------------

  const char* thread_text[2] = {
    "No",
    "Yes"
  };

  const char* render_text[3] = {
    "Block",
    "Interleaved",
    "Quantized"
  };

  bool init() final {
    bool result = MIP_Plugin::init();
    if (result) {
      appendNoteInput();
      appendStereoOutput();
      par_gain    = appendParameter( new MIP_Parameter(     "Gain",         1, 0,1) );
      par_thread  = appendParameter( new MIP_TextParameter( "Thread Pool",  0, 0,1, thread_text ));
      par_render  = appendParameter( new MIP_TextParameter( "Render Mode",  0, 0,2, render_text) );
      par_gain->setFlag(CLAP_PARAM_IS_MODULATABLE);
      par_gain->setFlag(CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID);
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

      wdg_gain   = new MIP_KnobWidget(   MIP_DRect(     10,10,  200,200 ));
      wdg_thread = new MIP_DragValueWidget( MIP_DRect(  220,10,  200, 20 ));
      wdg_render = new MIP_DragValueWidget( MIP_DRect(  220,40,  200, 20 ));

      background->appendChildWidget(wdg_gain);
      background->appendChildWidget(wdg_thread);
      background->appendChildWidget(wdg_render);

      wdg_gain->setArcThickness(40);
      wdg_gain->setValueSize(50);

      wdg_thread->setDrawBorder(true);
      wdg_thread->setBorderColor(0);
      wdg_thread->setText("ThreadPool");
      wdg_thread->setTextColor(0);
      wdg_thread->setValueColor(0);

      wdg_render->setDrawBorder(true);
      wdg_render->setBorderColor(0);
      wdg_render->setText("EventMode");
      wdg_render->setTextColor(0);
      wdg_render->setValueColor(0);

      MEditor->connect( wdg_gain,   MParameters[0] );
      MEditor->connect( wdg_thread, MParameters[1] );
      MEditor->connect( wdg_render, MParameters[2] );

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

  void processNoteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.processNoteExpression(event);
  }

  void processParamValue(const clap_event_param_value_t* event) final {
    switch (event->param_id) {
      case 0: p_gain   = event->value; break;
      case 1: p_thread = event->value; break;
      case 2: p_render = event->value; break;
    }
    MVoiceManager.processParamValue(event);
  }

  void processParamMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.processParamMod(event);
  }

  //void processMidi(const clap_event_midi_t* event) final {
  //  MVoiceManager.processMidi(event);
  //}

  //void processMidiSysex(const clap_event_midi_sysex_t* event) final {
  //  MVoiceManager.processMidiSysex(event);
  //}

  //void processMidi2(const clap_event_midi2_t* event) final {
  //  MVoiceManager.processMidi2(event);
  //}

//------------------------------
public: // audio
//------------------------------

  void processAudioBlock(MIP_ProcessContext* AContext) final {

    if (p_thread > 0.5) MVoiceManager.setProcessThreaded(true);
    else MVoiceManager.setProcessThreaded(false);

    MVoiceManager.setEventMode(p_render);

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

