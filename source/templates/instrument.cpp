
// nc -U -l -k /tmp/mip.socket
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
#endif

//----------------------------------------------------------------------

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

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {
};

typedef MIP_VoiceManager<myVoice,16> myVoiceManager;

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
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    bool result = MIP_Plugin::init();
    if (result) {
      appendNoteInput();
      appendStereoOutput();
      appendParameter( new MIP_Parameter("Gain", 0.5) );
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

//------------------------------
public: // gui
//------------------------------

  bool gui_create(const char* api, bool is_floating) final {
    MIsEditorOpen = false;
    MEditor = new MIP_Editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters);
    if (MEditor) {
      MIP_PanelWidget* background = new MIP_PanelWidget(MIP_DRect(0,0, MY_PLUGIN_EDITOR_WIDTH, MY_PLUGIN_EDITOR_HEIGHT));
      MEditor->setRootWidget(background);
      MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_DRect(50,50,300,400) );
      background->appendChildWidget(knob);
      knob->setArcThickness(40);
      MEditor->connect( knob, MParameters[0] );
    }
    return (MEditor);
  }


//------------------------------
public: // thread-pool
//------------------------------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.thread_pool_exec(task_index);
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

  //void processParamValue(const clap_event_param_value_t* event) final {
  //  switch (event->param_id) {
  //    case 0: gain = event->value; break;
  //  } // switch
  //}

  //----------

  void processNoteOn(const clap_event_note_t* event) final {}
  void processNoteOff(const clap_event_note_t* event) final {}
  void processNoteChoke(const clap_event_note_t* event) final {}
  void processNoteEnd(const clap_event_note_t* event) final {}
  void processNoteExpression(const clap_event_note_expression_t* event) final {}
  void processParamValue(const clap_event_param_value_t* event) final {}
  void processParamMod(const clap_event_param_mod_t* event) final {}
  void processParamGestureBegin(const clap_event_param_gesture_t* event) final {}
  void processParamGestureEnd(const clap_event_param_gesture_t* event) final {}
  void processTransport(const clap_event_transport_t* event) final {}
  void processMidi(const clap_event_midi_t* event) final {}
  void processMidiSysex(const clap_event_midi_sysex_t* event) final {}
  void processMidi2(const clap_event_midi2_t* event) final {}

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

