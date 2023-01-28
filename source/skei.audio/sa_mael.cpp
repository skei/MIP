
//#define MIP_PLUGIN_CLAP
//#define MIP_PLUGIN_VST2
//#define MIP_PLUGIN_VST3

//----------

#include "base/mip.h"
#include "audio/mip_audio_math.h"
#include "audio/mip_audio_utils.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_voice_manager.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MY_PLUGIN_EDITOR_WIDTH    500
#define MY_PLUGIN_EDITOR_HEIGHT   530
#define MY_PLUGIN_MAX_VOICES      128

//----------

const clap_plugin_descriptor_t my_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "skei.audio/sa_mael/0.0.1",
  .name         = "sa_mael",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "...",
  .features     = (const char*[]){CLAP_PLUGIN_FEATURE_INSTRUMENT,nullptr}
};

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

  //----------

  double ph     = 0.0;
  double phadd  = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, MIP_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MIP_Assert(MContext);
  }

  //----------

  float getEnvLevel() {
    return 1.0;
  }

  //----------

  uint32_t noteOn(uint32_t AKey, double AVelocity) {
    float hz = MIP_NoteToHz(AKey);
    float srate = MContext->samplerate;
    ph = 0;
    MIP_Assert(srate > 0);
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

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    float* buffer = MContext->voice_buffer;
    MIP_Assert(buffer);
    buffer += (MIndex * MIP_AUDIO_MAX_BLOCK_SIZE);
    buffer += AOffset;
    if ((AState == MIP_VOICE_PLAYING) || (AState == MIP_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {
        ph = MIP_Fract(ph);
        float v = (ph * 2.0) - 1.0;
        *buffer++ = v * 0.1;
        ph += phadd;
      }
    }
    else {
      memset(buffer,0,ALength * sizeof(float));
    }
    return AState;
  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    float* buffer = MContext->voice_buffer;
    MIP_Assert(buffer);
    buffer += (MIndex * MIP_AUDIO_MAX_BLOCK_SIZE);
    buffer += AOffset;
    if ((AState == MIP_VOICE_PLAYING) || (AState == MIP_VOICE_RELEASED)) {
      for (uint32_t i=0; i<MIP_AUDIO_SLICE_SIZE; i++) {
        ph = MIP_Fract(ph);
        float v = (ph * 2.0) - 1.0;
        *buffer++ = v * 0.1;
        ph += phadd;
      }
    }
    else {
      memset(buffer,0,MIP_AUDIO_SLICE_SIZE * sizeof(float));
    }
    return AState;
  }

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

  myVoiceManager    MVoiceManager   = {};
  double            p_gain          = 0.0;
  MIP_Parameter*    par_gain        = nullptr;
  MIP_KnobWidget*   wdg_gain        = nullptr;
  MIP_VoicesWidget* wdg_voices      = nullptr;
  MIP_ValueWidget*  wdg_voices_txt1 = nullptr;
  MIP_ValueWidget*  wdg_voices_txt2 = nullptr;
  MIP_ValueWidget*  wdg_voices_txt3 = nullptr;

//------------------------------
public:
//------------------------------

  my_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    MIP_PRINT;
    setInitialEditorSize(MY_PLUGIN_EDITOR_WIDTH,MY_PLUGIN_EDITOR_HEIGHT);
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(MIP_VOICE_EVENT_MODE_QUANTIZED);
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    bool result = MIP_Plugin::init();
    if (result) {
      appendNoteInput();
      appendStereoOutput();
      par_gain = appendParameter( new MIP_Parameter( "Gain", 1, 0,1 ));
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
      MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count,&MParameters);
    }
    return result;
  }

  //----------

  bool gui_create(const char* api, bool is_floating) final {
    //MIsEditorOpen = false;
    //MEditor = new MIP_Editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters);
    MIP_Editor* editor = new MIP_Editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters);

    //if (MEditor) {
    if (editor) {

      setEditor(editor);

      MIP_PanelWidget* background = new MIP_PanelWidget(MIP_DRect(0,0, MY_PLUGIN_EDITOR_WIDTH, MY_PLUGIN_EDITOR_HEIGHT));
      /*MEditor*/editor->setRootWidget(background);

      wdg_voices = new MIP_VoicesWidget(MIP_DRect(50,20,400,20),MY_PLUGIN_MAX_VOICES);
      background->appendChildWidget(wdg_voices);

      {
        wdg_voices_txt1 = new MIP_ValueWidget(MIP_DRect(50,50,125,20),"Playing:",0);
        wdg_voices_txt1->setTextColor(0);
        wdg_voices_txt1->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
        wdg_voices_txt1->setNumDigits(0);
        wdg_voices_txt1->setFillBackground(true);
        wdg_voices_txt1->setBackgroundColor(0.45);
        background->appendChildWidget(wdg_voices_txt1);

        wdg_voices_txt2 = new MIP_ValueWidget(MIP_DRect(50+130,50,125,20),"Released:",0);
        wdg_voices_txt2->setTextColor(0);
        wdg_voices_txt2->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
        wdg_voices_txt2->setNumDigits(0);
        wdg_voices_txt2->setFillBackground(true);
        wdg_voices_txt2->setBackgroundColor(0.45);
        background->appendChildWidget(wdg_voices_txt2);

        wdg_voices_txt3 = new MIP_ValueWidget(MIP_DRect(50+260,50,125,20),"Total:",0);
        wdg_voices_txt3->setTextColor(0);
        wdg_voices_txt3->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
        wdg_voices_txt3->setNumDigits(0);
        wdg_voices_txt3->setFillBackground(true);
        wdg_voices_txt3->setBackgroundColor(0.45);
        background->appendChildWidget(wdg_voices_txt3);
      }

      wdg_gain = new MIP_KnobWidget(MIP_DRect(50,100,400,400));
      wdg_gain->setArcThickness(10);
      wdg_gain->setValueSize(100);
      wdg_gain->setDrawModulation(true);
      wdg_gain->setSnap(true);
      background->appendChildWidget(wdg_gain);

      /*MEditor*/editor->connect( wdg_gain, par_gain );

    }
    return (/*MEditor*/editor);
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
public:
//------------------------------

  void on_editor_timer() final {
    MIP_Editor* editor = getEditor();
    //if (MEditor && MEditor->isEditorOpen()) {
    if (editor && editor->isEditorOpen()) {
      uint32_t num = MVoiceManager.getMaxVoices();
      for (uint32_t i=0; i<num; i++) {
        uint32_t state = MVoiceManager.getVoiceState(i);
        wdg_voices->setVoiceState(i,state);
      }
      uint32_t num_playing = MVoiceManager.getNumPlayingVoices();
      uint32_t num_released = MVoiceManager.getNumReleasedVoices();
      wdg_voices_txt1->setValue(num_playing);
      wdg_voices_txt2->setValue(num_released);
      wdg_voices_txt3->setValue(num_playing + num_released);
      wdg_voices->redraw();
      wdg_voices_txt1->redraw();
      wdg_voices_txt2->redraw();
      wdg_voices_txt3->redraw();
    }
    MIP_Plugin::on_editor_timer();
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
      case 0: p_gain = event->value; break;
    }
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
    uint32_t length = AContext->process->frames_count;
    float** buffer = AContext->process->audio_outputs[0].data32;
    MIP_ScaleStereoBuffer(buffer,p_gain,length);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/mip_entry.h"
MIP_DEFAULT_ENTRY(my_descriptor,my_plugin);

