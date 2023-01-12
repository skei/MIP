

#define MIP_PLUGIN_CLAP
//#define MIP_PLUGIN_VST2
//#define MIP_PLUGIN_VST3

//#define MIP_NO_GUI
//#define MIP_NO_PAINTER

#ifndef MIP_NO_GUI
  #ifdef __gnu_linux__
    #define MIP_GUI_XCB
    #define MIP_PAINTER_NANOVG
    //#define MIP_PAINTER_XCB
    //#define MIP_WINDOW_BUFFERED
  #else
    #define MIP_GUI_WIN32
    #define MIP_PAINTER_NANOVG
    //#define MIP_PAINTER_GDI
  #endif
#endif

//#define MIP_PLUGIN_GENERIC_EDITOR

#ifndef MIP_EXE
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//#define MIP_LOG

//----------------------------------------------------------------------

#include "audio/mip_audio_utils.h"

#include "plugin/mip_plugin.h"
#include "plugin/mip_note.h"

//#include "audio/old/mip_voice_manager.h"
//#include "audio/old/mip_voice.h"

#include "plugin/mip_voice_manager.h"
#include "plugin/mip_voice.h"

#ifndef MIP_NO_GUI
  #include "plugin/mip_editor.h"
#endif

//#include "gui/widgets/mip_widgets.h"

//

#define SA_TYR_EDITOR_WIDTH   836
#define SA_TYR_EDITOR_HEIGHT  703
#define SA_TYR_NUM_VOICES     32

#define SA_TYR_PLUGIN_NAME    "sa_tyr"
#define SA_TYR_PLUGIN_VERSION "0.0.12"



#include "sa_tyr/sa_tyr_parameters.h"
#include "sa_tyr/sa_tyr_voice.h"

typedef MIP_VoiceManager<sa_tyr_voice<float>,SA_TYR_NUM_VOICES> sa_tyr_voice_manager;

#ifndef MIP_NO_GUI
  #include "sa_tyr/sa_tyr_editor.h"
#endif

//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_tyr_descriptor = {
  .clap_version  = CLAP_VERSION,
  .id            = "skei.audio/sa_tyr/0.0.9",
  .name          = SA_TYR_PLUGIN_NAME,
  .vendor        = "skei.audio",
  .url           = "https://torhelgeskei.com",
  .manual_url    = "",
  .support_url   = "",
  .version       = SA_TYR_PLUGIN_VERSION,//"0.0.9",
  .description   = "",
  .features      = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_tyr_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  //__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
  //MIP_VoiceManager<sa_tyr_voice<double>,SA_TYR_NUM_VOICES> MVoiceManager = {};

  //  sa_tyr_voice_manager MVoices = {};
  sa_tyr_voice_manager MVoiceManager = {};


  //---------------
  // parameters
  //---------------

  //enum gain_parameter_enums {
  //  PAR_GAIN = 0,
  //  PARAM_COUNT
  //};
  //
  //#define MP (CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID)
  //
  //const clap_param_info_t gain_parameters[PARAM_COUNT] = {
  //  { PAR_GAIN, MP, nullptr, "Gain", "", 0, 1, 0 }
  //};
  //
  //#undef MP

  //---------------
  // ports
  //---------------

//  const clap_audio_port_info_t gain_audioInputPorts[1] = {
//    { 0, "audio in 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
//  };
//
//  const clap_audio_port_info_t gain_audioOutputPorts[1] = {
//    { 0, "audio out 1", CLAP_AUDIO_PORT_IS_MAIN, 2, CLAP_PORT_STEREO, CLAP_INVALID_ID }
//  };
//
//  const clap_note_port_info_t gain_noteInputPorts[1] = {
//    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note in 1" }
//  };
//
//  const clap_note_port_info_t gain_noteOutputPorts[1] = {
//    { 0, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "note out 1" }
//  };

  //---------------
  //
  //---------------

//  double par_gain = 0.0;

//------------------------------
public:
//------------------------------

  sa_tyr_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    //MEditorWidth = SA_TYR_EDITOR_WIDTH;
    //MEditorHeight = SA_TYR_EDITOR_HEIGHT;
    setInitialEditorSize(SA_TYR_EDITOR_WIDTH,SA_TYR_EDITOR_HEIGHT);

    MVoiceManager.setProcessThreaded(true);
    //MVoiceManager.setEventVoiceSplit(true);
    MVoiceManager.setEventMode(MIP_VOICE_EVENT_MODE_QUANTIZED);

  }

  //----------

  virtual ~sa_tyr_plugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    bool result = MIP_Plugin::init();
    if (result) {
      //appendAudioInputPort( &gain_audioInputPorts[0] );
      //appendAudioOutputPort(&gain_audioOutputPorts[0]);
      //appendNoteInputPort(  &gain_noteInputPorts[0]  );
      //appendNoteOutputPort( &gain_noteOutputPorts[0] );
      appendAudioInputPort( new MIP_AudioPort() );
      appendAudioOutputPort( new MIP_AudioPort() );
      appendNoteInputPort( new MIP_NotePort() );
      appendNoteOutputPort( new MIP_NotePort() );
      for (uint32_t i=0; i<PARAM_COUNT; i++) {
        const char* name = sa_tyr_parameters[i].name;
        double val       = sa_tyr_parameters[i].default_value;
        double minval    = sa_tyr_parameters[i].min_value;
        double maxval    = sa_tyr_parameters[i].max_value;
        uint32_t flags   = sa_tyr_parameters[i].flags;
        MIP_Parameter* parameter = nullptr;
        if (flags & CLAP_PARAM_IS_STEPPED) parameter = new MIP_IntParameter(name,val,minval,maxval);
        else parameter = new MIP_Parameter(name,val,minval,maxval);
        parameter->setFlags(flags);
        appendParameter(parameter);
      }
      setDefaultParameterValues();
      MVoiceManager.init(getClapPlugin(),getClapHost());
    }
    return result;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
    MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count,&MParameters);
    return true;
  }

//------------------------------
public: // gui
//------------------------------

#ifndef MIP_NO_GUI

  //#ifndef MIP_PLUGIN_GENERIC_EDITOR

  bool gui_create(const char *api, bool is_floating) final {
    //MIP_Plugin::gui_create(api,is_floating);
    MIsEditorOpen = false;
    MEditor = new sa_tyr_editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters,getClapDescriptor());
    return (MEditor);
    //MEditor->setCanResizeEditor(true);
    //MEditor->setResizeProportional(true);
    //MEditor->setProportionalSize(EDITOR_WIDTH,EDITOR_HEIGHT);
    return true;
  }

  //#endif

  //----------

  void on_editor_timer() override {
    //MIP_PRINT;
    if (MEditor && MEditor->isEditorOpen()) {
      sa_tyr_editor* editor = (sa_tyr_editor*)MEditor;
      editor->timer_update(&MVoiceManager);
      //editor->updateWaveformWidget(&MProcessor);
      //editor->updateProbIndicators(&MProcessor);
    }
    MIP_Plugin::on_editor_timer();

  }


#endif

//------------------------------
public: // voice into
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = SA_TYR_NUM_VOICES;
    info->voice_capacity  = SA_TYR_NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

//------------------------------
public:
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
    MVoiceManager.processParamValue(event);
  }

  void processParamMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.processParamMod(event);
  }

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

#include "plugin/mip_entry.h"
MIP_DEFAULT_ENTRY(sa_tyr_descriptor,sa_tyr_plugin);
