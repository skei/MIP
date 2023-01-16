
#include "audio/mip_audio_utils.h"

#include "plugin/mip_note.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_voice.h"
#include "plugin/mip_voice_manager.h"

#ifndef MIP_NO_GUI
  #include "plugin/mip_editor.h"
#endif

//----------------------------------------------------------------------

#define SA_TYR_EDITOR_WIDTH   836
#define SA_TYR_EDITOR_HEIGHT  703
#define SA_TYR_NUM_VOICES     256
#define SA_TYR_PLUGIN_NAME    "sa_tyr"
#define SA_TYR_PLUGIN_VERSION "0.0.12"

#include "sa_tyr/sa_tyr_parameters.h"
#include "sa_tyr/sa_tyr_voice.h"

typedef MIP_VoiceManager<sa_tyr_voice,SA_TYR_NUM_VOICES> sa_tyr_voice_manager;

#ifndef MIP_NO_GUI
  #include "sa_tyr/sa_tyr_editor.h"
#endif



//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_tyr_descriptor = {
  .clap_version  = CLAP_VERSION,
  //.id            = "skei.audio/sa_tyr/0.0.12",
  .id            = "skei.audio/" SA_TYR_PLUGIN_NAME "/" SA_TYR_PLUGIN_VERSION,
  .name          = SA_TYR_PLUGIN_NAME,
  .vendor        = "skei.audio",
  .url           = "https://torhelgeskei.com",
  .manual_url    = "",
  .support_url   = "",
  .version       = SA_TYR_PLUGIN_VERSION,
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

  sa_tyr_voice_manager MVoiceManager = {};

//------------------------------
public:
//------------------------------

  /*
    ---           without   with thread-pool:
    reaper/linux  45        60
    bitwig/linux  45        60
    reaper/wine   35 *      180 *
  */

  //----------

  sa_tyr_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    setInitialEditorSize(SA_TYR_EDITOR_WIDTH,SA_TYR_EDITOR_HEIGHT);
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(MIP_VOICE_EVENT_MODE_INTERLEAVED);
    //MVoiceManager.setEventVoiceSplit(true);
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
      //appendAudioInputPort( new MIP_AudioPort() );
      //appendAudioOutputPort( new MIP_AudioPort() );
      //appendNoteInputPort( new MIP_NotePort() );
      //appendNoteOutputPort( new MIP_NotePort() );
      appendStereoInput();
      appendStereoOutput();
      appendNoteInput();
      appendNoteOutput();
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
    //return true;
  }

  //#endif

  //----------

  void on_editor_timer() override {
    if (MEditor && MEditor->isEditorOpen()) {
      sa_tyr_editor* editor = (sa_tyr_editor*)MEditor;
      editor->timer_update(&MVoiceManager);
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

    float** buffer  = AContext->process->audio_outputs[0].data32;
    uint32_t length = AContext->process->frames_count;

    MIP_ClearStereoBuffer(buffer,length);
    MVoiceManager.processAudioBlock(AContext);

    // gain & pan
    double gain     = MParameters[PAR_MASTER_VOL]->getValue();
    double pan      = MParameters[PAR_MASTER_PAN]->getValue();
    double left     = (1.0 - pan) * gain;
    double right    = (0.0 + pan) * gain;

    MIP_ScaleStereoBuffer(buffer,left,right,length);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/mip_entry.h"
MIP_DEFAULT_ENTRY(sa_tyr_descriptor,sa_tyr_plugin);
