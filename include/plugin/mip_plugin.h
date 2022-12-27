#ifndef mip_plugin_included
#define mip_plugin_included
//----------------------------------------------------------------------

// TODO: #ifndef MIP_NO_GUI around editor stuff

#include "base/mip.h"
#include "base/system/mip_timer.h"
#include "plugin/mip_parameter.h"
#include "plugin/mip_audio_port.h"
#include "plugin/mip_note_port.h"
#include "plugin/mip_process_context.h"
#include "plugin/clap/mip_clap_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_PLUGIN_MAX_GUI_EVENTS       32
#define MIP_PLUGIN_MAX_PARAM_EVENTS     4096
#define MIP_PLUGIN_GENERIC_EDITOR_WIDTH 500

#include "plugin/mip_editor.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------

class MIP_Plugin
: public MIP_ClapPlugin
, public MIP_TimerListener
, public MIP_EditorListener {

  //friend class MIP_ExeWindow;

//------------------------------
private:
//------------------------------

  uint32_t MProcessCount = 0;

//------------------------------
protected:
//------------------------------

  MIP_ProcessContext    MProcessContext           = {};
  MIP_ParameterArray    MParameters               = {};
  MIP_AudioPortArray    MAudioInputPorts          = {};
  MIP_AudioPortArray    MAudioOutputPorts         = {};
  MIP_NotePortArray     MNoteInputPorts           = {};
  MIP_NotePortArray     MNoteOutputPorts          = {};

  MIP_Editor*           MEditor                   = nullptr;
  MIP_Timer             MGuiTimer                 = MIP_Timer(this);

  // write values before indexes
  // read indexes before values

  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_PARAM_EVENTS> MProcessParamQueue        = {};
  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_PARAM_EVENTS> MProcessModQueue          = {};
  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_PARAM_EVENTS> MGuiParamQueue            = {};
  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_PARAM_EVENTS> MGuiModQueue              = {};
  MIP_Queue<uint32_t,MIP_PLUGIN_MAX_GUI_EVENTS>   MHostParamQueue           = {};

  MIP_Queue<double,MIP_PLUGIN_MAX_PARAM_EVENTS>   MQueuedProcessParamValues = {};
  MIP_Queue<double,MIP_PLUGIN_MAX_PARAM_EVENTS>   MQueuedProcessModValues   = {};
  MIP_Queue<double,MIP_PLUGIN_MAX_PARAM_EVENTS>   MQueuedGuiParamValues     = {};
  MIP_Queue<double,MIP_PLUGIN_MAX_PARAM_EVENTS>   MQueuedGuiModValues       = {};
  MIP_Queue<double,MIP_PLUGIN_MAX_GUI_EVENTS>     MQueuedHostParamValues    = {};

  bool                  MIsInitialized            = false;
  bool                  MIsActivated              = false;
  bool                  MIsProcessing             = false;
  bool                  MIsEditorOpen             = false;
//bool                  MIsEditorBusy             = false;

  uint32_t              MSelectedAudioPortsConfig = 0;
  int32_t               MRenderMode               = CLAP_RENDER_REALTIME;
  uint32_t              MInitialEditorWidth       = 256;
  uint32_t              MInitialEditorHeight      = 256;

//------------------------------
public:
//------------------------------

  MIP_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_ClapPlugin(ADescriptor) {
    //MIP_PRINT;
  }

  //----------

  virtual ~MIP_Plugin() {
    //MIP_PRINT;
    #ifndef MIP_NO_AUTODELETE
      deleteParameters();
      deleteAudioInputPorts();
      deleteAudioOutputPorts();
      deleteNoteInputPorts();
      deleteNoteOutputPorts();
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual void setInitialEditorSize(uint32_t AWidth, uint32_t AHeight) {
    MInitialEditorWidth  = AWidth;
    MInitialEditorHeight = AHeight;
    //MEditor.setInitialSize(AWidth,AHeight); // ->gui_create
  }

  virtual MIP_Editor* getEditor() { return MEditor; }

//------------------------------
public: // plugin
//------------------------------

  bool init() override {
    //MIP_Assert(!MIsInitialized);
    if (!MIsInitialized) {
      MIsInitialized = true;
    }
    return true;
  }

  //----------

  void destroy() override {
    //MIP_Assert(MIsInitialized);
    //if (MIsInitialized)
    MIsInitialized = false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    //MIP_PRINT;
    //MIP_Assert(!MIsActivated);
    if (!MIsActivated) {
      setDefaultParameterValues();
      MProcessContext.samplerate = sample_rate;
      MIsActivated = true;
    }
    return true;
  }

  //----------

  void deactivate() override {
    //MIP_Assert(MIsActivated);
    //if (MIsActivated)
    MIsActivated = false;
  }

  //----------

  bool start_processing() override {
    //MIP_Assert(!MIsProcessing);
    if (!MIsProcessing) {
      MProcessContext.counter = 0;
      MIsProcessing = true;
    }
    return true;
  }

  //----------

  void stop_processing() override {
    //MIP_Assert(MIsProcessing);
    //if (MIsProcessing)
    MIsProcessing = false;
  }

  //----------

  void reset() override {
  }

  //----------

  clap_process_status process(const clap_process_t *process) override {
    MProcessContext.counter += 1;
    MProcessContext.process = process;
    processTransport(process->transport);
    preProcessEvents(process->in_events,process->out_events);
    processEvents(process->in_events,process->out_events);
    flushProcessParams();
    processAudioBlock(&MProcessContext);
    postProcessEvents(process->in_events,process->out_events);
    flushHostParams(process->out_events);
    MProcessContext.process = nullptr;
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    if (strcmp(id,CLAP_EXT_AMBISONIC)           == 0) return &MAmbisonic;       // draft
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG)  == 0) return &MAudioPortsConfig;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS)         == 0) return &MAudioPorts;
    if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE)    == 0) return &MCheckForUpdate;  // draft
    if (strcmp(id,CLAP_EXT_CV)                  == 0) return &MCV;              // draft
    if (strcmp(id,CLAP_EXT_FILE_REFERENCE)      == 0) return &MFileReference;   // draft
    #ifndef MIP_NO_GUI
    if (strcmp(id,CLAP_EXT_GUI)                 == 0) return &MGui;
    #endif
    if (strcmp(id,CLAP_EXT_LATENCY)             == 0) return &MLatency;
    if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS)       == 0) return &MMidiMappings;    // draft
    if (strcmp(id,CLAP_EXT_NOTE_NAME)           == 0) return &MNoteName;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS)          == 0) return &MNotePorts;
    if (strcmp(id,CLAP_EXT_PARAMS)              == 0) return &MParams;
    if (strcmp(id,CLAP_EXT_PRESET_LOAD)         == 0) return &MPresetLoad;      // draft
    if (strcmp(id,CLAP_EXT_QUICK_CONTROLS)      == 0) return &MQuickControls;   // draft
    if (strcmp(id,CLAP_EXT_RENDER)              == 0) return &MRender;
    if (strcmp(id,CLAP_EXT_STATE)               == 0) return &MState;
    if (strcmp(id,CLAP_EXT_SURROUND)            == 0) return &MSurround;        // draft
    if (strcmp(id,CLAP_EXT_THREAD_POOL)         == 0) return &MThreadPool;
    if (strcmp(id,CLAP_EXT_TIMER_SUPPORT)       == 0) return &MTimerSupport;
    if (strcmp(id,CLAP_EXT_TRACK_INFO)          == 0) return &MTrackInfo;       // draft
    if (strcmp(id,CLAP_EXT_TUNING)              == 0) return &MTuning;          // draft
    if (strcmp(id,CLAP_EXT_VOICE_INFO)          == 0) return &MVoiceInfo;       // draft
    return nullptr;
  }

  //----------

 void on_main_thread() override {
 }

//------------------------------
public: // draft ambisonic
//------------------------------

  bool ambisonic_get_info(bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) override {
    return false;
  }

//------------------------------
public: // ext audio-ports-config
//------------------------------

  uint32_t audio_ports_config_count() override {
    return 1;
  }

  //----------

  //TODO: don't assume stereo in/out, check MAudioInput/OutputPorts

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) override {
    MIP_Assert(index == 0);
    config->id                         = 0;
    strncpy(config->name,"config-name",CLAP_NAME_SIZE);
    config->input_port_count           = 1;
    config->output_port_count          = 1;
    config->has_main_input             = true;
    config->main_input_channel_count   = 2;
    config->main_input_port_type       = CLAP_PORT_STEREO;
    config->has_main_output            = true;
    config->main_output_channel_count  = 2;
    config->main_output_port_type      = CLAP_PORT_STEREO;
    return false;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) override {
    MIP_Assert(config_id == 0);
    MSelectedAudioPortsConfig = config_id;
    return false;
  }

//------------------------------
public: // ext audio-ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    if (is_input) return MAudioInputPorts.size();
    else return MAudioOutputPorts.size();
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) override {
    if (is_input) {
      memcpy(info,MAudioInputPorts[index]->getInfo(),sizeof(clap_audio_port_info_t));
      return true;
    }
    else { // output
      memcpy(info,MAudioOutputPorts[index]->getInfo(),sizeof(clap_audio_port_info_t));
      return true;
    }
    return false;  }

//------------------------------
public: // draft check-for-updates
//------------------------------

  void check_for_updates_check(bool include_preview) override {
  }

//------------------------------
public: // draft cv
//------------------------------

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) override {
    return false;
  }

//------------------------------
public: // draft file-reference
//------------------------------

  uint32_t file_reference_count() override {
    return 0;
  }

  //----------

  bool file_reference_get(uint32_t index, clap_file_reference_t *file_reference) override {
    return false;
  }

  //----------

  bool file_reference_get_blake3_digest(clap_id resource_id, uint8_t *digest) override {
    return false;
  }

  //----------

  bool file_reference_get_file_size(clap_id resource_id, uint64_t *size) override {
    return false;
  }

  //----------

  bool file_reference_update_path(clap_id resource_id, const char *path) override {
    return false;
  }

  //----------

  bool file_reference_save_resources() override {
    return false;
  }

//------------------------------
public: // ext gui
//------------------------------

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    //MIP_Print("\n");
    #ifdef MIP_LINUX
      if (!is_floating && (strcmp(api,CLAP_WINDOW_API_X11) == 0)) return true;
    #endif
    #ifdef MIP_WIN32
      if (!is_floating && (strcmp(api,CLAP_WINDOW_API_WIN32) == 0)) return true;
    #endif
    return false;
  }

  //----------

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    //MIP_Print("\n");
    #ifdef MIP_LINUX
      *api = CLAP_WINDOW_API_X11;
      *is_floating = false;
    #endif
    #ifdef MIP_WIN32
      *api = CLAP_WINDOW_API_WIN32;
      *is_floating = false;
    #endif
    return true;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    //MIP_Print("\n");
    MIsEditorOpen = false;
    MEditor = new MIP_Editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters);
    //MIP_Assert(MEditor);
    return true;
  }

  //----------

  void gui_destroy() override {
    //MIP_Print("\n");
    //MIP_Assert(MEditor);
    if (MEditor) {
      if (MIsEditorOpen) gui_hide();
      MIsEditorOpen = false;
      if (MIsEditorOpen) {
        MEditor->hide();
      }
      delete MEditor;
    }
  }

  //----------

  bool gui_set_scale(double scale) override {
    //MIP_Print("%f\n",scale);
    //MIP_Assert(MEditor);
    if (MEditor) return MEditor->setScale(scale);
    else return 1.0;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    //MIP_Assert(MEditor);
    if (MEditor) {
      bool result = MEditor->getSize(width,height);
      //MIP_Print("-> %i,%i\n",*width,*height);
      return result;
    }
    else {
      *width = MInitialEditorWidth;
      *height = MInitialEditorHeight;
      return true;
    }
  }

  //----------

  bool gui_can_resize() override {
    //MIP_Print("\n");
    //MIP_Assert(MEditor);
    if (MEditor) return MEditor->canResize();
    else return false;
  }

  //----------

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    //MIP_Print("\n");
    //MIP_Assert(MEditor);
    if (MEditor) return MEditor->getResizeHints(hints);
    else {
      return false;
    }
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    MIP_Assert(MEditor);
    //uint32_t w = *width;
    //uint32_t h = *height;
    bool result = MEditor->adjustSize(width,height);
    //MIP_Print("%i,%i -> %i,%i\n",w,h,*width,*height);
    return result;
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    //MIP_Print("%i,%i\n",width,height);
    MIP_Assert(MEditor);
    //MIsEditorBusy = true;
    bool result = MEditor->setSize(width,height);
    //MIsEditorBusy = false;
    return result;
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    //MIP_Print("\n");
    MIP_Assert(MEditor);
    return MEditor->setParent(window);
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    //MIP_Print("\n");
    MIP_Assert(MEditor);
    return MEditor->setTransient(window);
  }

  //----------

  void gui_suggest_title(const char *title) override {
    //MIP_Print("\n");
    MIP_Assert(MEditor);
    MEditor->suggestTitle(title);
  }

  //----------

  bool gui_show() override {
    //MIP_Print("\n");
    MIP_Assert(MEditor);
    updateEditorParameterValues();
    bool result = MEditor->show();
    if (result) {
      #ifdef MIP_LINUX
        MGuiTimer.start(MIP_EDITOR_TIMER_MS);
      #endif
      #ifdef MIP_WIN32
        MIP_Win32Window* window = MEditor->getWindow();
        HWND hwnd = window->getHandle();
        MGuiTimer.start(MIP_EDITOR_TIMER_MS,hwnd);
      #endif
      MIsEditorOpen = true;
    }
    return result;
  }

  //----------

  bool gui_hide() override {
    //MIP_Print("\n");
    MIP_Assert(MEditor);
    MGuiTimer.stop();
    MIsEditorOpen = false;
    bool result = MEditor->hide();
    return result;
  }

//------------------------------
public: // ext latency
//------------------------------

  uint32_t latency_get() override {
    return 0;
  }

//------------------------------
public: // draft midi-mappings
//------------------------------

  uint32_t midi_mappings_count() override {
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) override {
    //mapping->channel = -1;
    //mapping->number = -1;
    //mapping->param_id = -1;
    return false;
  }

//------------------------------
public: // ext note-names
//------------------------------

  uint32_t note_names_count() override {
    return 0;
  }

  //----------

  bool note_names_get(uint32_t index, clap_note_name_t *note_name) override {
   //strncpy(note_name->name,"name",CLAP_NAME_SIZE);
   //note_name->port = -1;
   //note_name->key = -1;
   //note_name->channel = -1;
   return false;
  }

//------------------------------
public: // ext note-ports
//------------------------------

  uint32_t note_ports_count(bool is_input) override {
    if (is_input) return MNoteInputPorts.size();
    else return MNoteOutputPorts.size();
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) override {
    if (is_input) {
      memcpy(info,MNoteInputPorts[index]->getInfo(),sizeof(clap_note_port_info_t));
      return true;
    }
    else { // output
      memcpy(info,MNoteOutputPorts[index]->getInfo(),sizeof(clap_note_port_info_t));
      return true;
    }
    return false;
  }

//------------------------------
public: // ext params
//------------------------------

  uint32_t params_count() override {
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) override {
    MIP_Parameter* parameter  = MParameters[param_index];
    param_info->id            = param_index;
    param_info->flags         = parameter->getFlags();
    param_info->cookie        = parameter;
    param_info->min_value     = parameter->getMinValue();
    param_info->max_value     = parameter->getMaxValue();
    param_info->default_value = parameter->getDefValue();
    strcpy(param_info->name,parameter->getName());
    strcpy(param_info->module,parameter->getModule());
    return true;  }

  //----------

  bool params_get_value(clap_id param_id, double *value) override {
    MIP_Parameter* parameter = MParameters[param_id];
    *value = parameter->getValue();
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) override {
    MIP_Parameter* parameter = MParameters[param_id];
    parameter->valueToText(value,display,size);
    return true;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char *display, double *value) override {
    MIP_Parameter* parameter = MParameters[param_id];
    parameter->textToValue(display,value);
    return false;
  }

  //----------

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) override {
    preProcessEvents(in,out);
    processEvents(in,out);
    postProcessEvents(in,out);
    flushHostParams(out);
  }

//------------------------------
public: // ext posix-fd-support
//------------------------------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
  }

//------------------------------
public: // draft preset-load
//------------------------------

  bool preset_load_from_file(const char *path) override {
    return false;
  }

//------------------------------
public: // draft quick-controls
//------------------------------

  uint32_t quick_controls_count() override {
    return 1;
  }

  //----------

  bool quick_controls_get(uint32_t page_index, clap_quick_controls_page_t *page) override {
    switch (page_index) {
      case 0:
        page->id = 0;
        strcpy(page->name,"page1");
        for (uint32_t i=0; i<CLAP_QUICK_CONTROLS_COUNT; i++) {
          page->param_ids[i] = i;
        }
        return true;
    }
    return false;
  }

//------------------------------
public: // ext render
//------------------------------

  bool render_has_hard_realtime_requirement() override {
    return false;
  }

  //----------

  bool render_set(clap_plugin_render_mode mode) override {
    //MRenderMode = mode;
    return false;
  }

//------------------------------
public: // ext state
//------------------------------

  bool state_save(const clap_ostream_t *stream) override {
    //MIP_PRINT;
    uint32_t written = 0;
    uint32_t version = 0;
    uint32_t num_params = MParameters.size();
    // version
    written = stream->write(stream,&version,sizeof(uint32_t));
    if (written != sizeof(uint32_t)) {
      MIP_Print("state_save: error writing version\n");
      return false;
    }
    // num params
    written = stream->write(stream,&num_params,sizeof(uint32_t));
    if (written != sizeof(uint32_t)) {
      MIP_Print("state_save: error writing parameter count\n");
      return false;
    }
    // param values
    for (uint32_t i=0; i<num_params; i++) {
      double value = MParameters[i]->getValue();
      written = stream->write(stream,&value,sizeof(double));
      if (written != sizeof(double)) {
        MIP_Print("state_load: error writing parameter %i\n",i);
        return false;
      }
    }
    return true;
  }

  //----------

  bool state_load(const clap_istream_t *stream) override {
    //MIP_PRINT;
    uint32_t read = 0;
    uint32_t version = 0;
    uint32_t num_params = 0;
    // version
    read = stream->read(stream,&version,sizeof(uint32_t));
    if (read != sizeof(uint32_t)) {
      MIP_Print("state_load: error reading version\n");
      return false;
    }
    //TODO: check version
    // num params
    read = stream->read(stream,&num_params,sizeof(uint32_t));
    if (read != sizeof(uint32_t)) {
      MIP_Print("state_load: error reading parameter count\n");
      return false;
    }
    //TODO: check num params = marameters.size
    if (num_params != MParameters.size()) {
      MIP_Print("state_load: wrong parameter count\n");
      return false;
    }
    // param values
    for (uint32_t i=0; i<num_params; i++) {
      double value = 0.0;
      read = stream->read(stream,&value,sizeof(double));
      if (read != sizeof(double)) {
        MIP_Print("state_load: error reading parameter %i\n",i);
        return false;
      }
      MParameters[i]->setValue(value);
    }
    updateParameterValues();
    return true;
  }

//------------------------------
public: // draft surround
//------------------------------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) override {
    return 0;
  }

  //----------

  void surround_changed() override {
  }

//------------------------------
public: // ext tail
//------------------------------

  uint32_t tail_get() override {
    return 0;
  }

//------------------------------
public: // ext thread-pool
//------------------------------

  void thread_pool_exec(uint32_t task_index) override {
  }

//------------------------------
public: // ext timer-support
//------------------------------

  void timer_support_on_timer(clap_id timer_id) override {
  }

//------------------------------
public: // draft track-info
//------------------------------

  void track_info_changed() override {
  }

//------------------------------
public: // draft tuning
//------------------------------

  void tuning_changed() override {
  }

//------------------------------
public: // ext voice-info
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    /*
    info->voice_count     = 0;
    info->voice_capacity  = 0;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    */
    return false;
  }

//------------------------------
private: // editor listener
//------------------------------

  void on_editor_parameter_change(uint32_t AIndex, double AValue) override {
    //MIP_Print("%i = %f\n",AIndex,AValue);
    queueHostParam(AIndex,AValue);
    queueProcessParam(AIndex,AValue);
  }

  //----------

  void on_editor_editor_resize() override  {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    //clap_host_gui_t* host_gui = (clap_host_gui_t*)MClapHost->get_extension(MClapHost,CLAP_EXT_GUI);
    //if (host_gui) {
    //  host_gui->request_resize(MClapHost,AWidth,AHeight);
    //  gui_set_size(AWidth,AHeight);
    //}
  }

  //----------

  //void on_editor_gesture_begin() override {
  //}

  //----------

  //void on_editor_gesture_end() override {
  //}

//------------------------------
public: // timer listener
//------------------------------

  /*
    called from our (separate) timer thread..
  */

  #ifndef MIP_NO_GUI

  // added override.. :-/

  void on_timer_callback(MIP_Timer* ATimer) override {
    //MIP_Print("1\n");
    if (ATimer == &MGuiTimer) {
      if (MIsEditorOpen) { //  && !MIsEditorBusy) {

//        MEditor->updateTimer();
//        MEditor.updateWaveformWidget(sa_botage_processor* process) {

        flushGuiParams();
        flushGuiMods();

      }
    }
    //MIP_PRINT;
  }

  #endif

//------------------------------
public: // parameters
//------------------------------

  MIP_Parameter* appendParameter(MIP_Parameter* AParameter) {
    int32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    double value = AParameter->getDefValue();
    AParameter->setValue(value);
    return AParameter;
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      if (MParameters[i]) delete MParameters[i];
    }
    MParameters.clear();
  }

  //----------

  uint32_t getParameterCount() {
    return MParameters.size();
  }

  //----------

  MIP_Parameter* getParameter(uint32_t AIndex) {
    return MParameters[AIndex];
  }

  //----------

  double getParameterValue(uint32_t AIndex) {
    return MParameters[AIndex]->getValue();
  }

  //----------

  void setParameterValue(uint32_t AIndex, double AValue) {
    MParameters[AIndex]->setValue(AValue);
  }

  //----------

  void setDefaultParameterValues() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      double value = MParameters[i]->getDefValue();
      MParameters[i]->setValue(value);
      queueProcessParam(i,value);
    }
  }

  //----------

  void updateParameterValues() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      double value = MParameters[i]->getValue();
      //MParameters[i]->setValue(value);
      queueProcessParam(i,value);
    }
  }

  //----------

  void updateEditorParameterValues() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      double v = MParameters[i]->getValue();
      MEditor->updateParameter(i,v,false);
    }
  }

//------------------------------
public: // modulation
//------------------------------

  double  getParameterModulation(uint32_t AIndex) {
    return MParameters[AIndex]->getModulation();
  }

  //----------

  void setParameterModulation(uint32_t AIndex, double AValue) {
    MParameters[AIndex]->setModulation(AValue);
  }

//------------------------------
public: // audio ports
//------------------------------

  MIP_AudioPort* appendAudioInputPort(MIP_AudioPort* AAudioPort) {
    MAudioInputPorts.append(AAudioPort);
    return AAudioPort;
  }

  //----------

  MIP_AudioPort* appendStereoInput() {
    MIP_AudioPort* port = new MIP_AudioPort();
    MAudioInputPorts.append(port);
    return port;
  }

  //----------

  void deleteAudioInputPorts() {
    for (uint32_t i=0; i<MAudioInputPorts.size(); i++) {
      if (MAudioInputPorts[i]) delete MAudioInputPorts[i];
    }
    MAudioInputPorts.clear();
  }

  //----------

  const clap_audio_port_info_t* getAudioInputPort(uint32_t AIndex) {
    return MAudioInputPorts[AIndex]->getInfo();
  }

  //----------

  MIP_AudioPort* appendAudioOutputPort(MIP_AudioPort* AAudioPort) {
    MAudioOutputPorts.append(AAudioPort);
    return AAudioPort;
  }

  //----------

  MIP_AudioPort* appendStereoOutput() {
    MIP_AudioPort* port = new MIP_AudioPort();
    MAudioOutputPorts.append(port);
    return port;
  }

  //----------

  void deleteAudioOutputPorts() {
    for (uint32_t i=0; i<MAudioOutputPorts.size(); i++) {
      if (MAudioOutputPorts[i]) delete MAudioOutputPorts[i];
    }
    MAudioOutputPorts.clear();
  }

  //----------

  const clap_audio_port_info_t* getAudioOutputPort(uint32_t AIndex) {
    return MAudioOutputPorts[AIndex]->getInfo();
  }

//------------------------------
public: // note ports
//------------------------------

  MIP_NotePort* appendNoteInputPort(MIP_NotePort* ANotePort) {
    MNoteInputPorts.append(ANotePort);
    return ANotePort;
  }

  //----------

  MIP_NotePort* appendNoteInput() {
    MIP_NotePort* port = new MIP_NotePort();
    MNoteInputPorts.append(port);
    return port;
  }

  //----------

  void deleteNoteInputPorts() {
    for (uint32_t i=0; i<MNoteInputPorts.size(); i++) {
      if (MNoteInputPorts[i]) delete MNoteInputPorts[i];
    }
    MNoteInputPorts.clear();
  }

  //----------

  const clap_note_port_info_t* getNoteInputPort(uint32_t AIndex) {
    return MNoteInputPorts[AIndex]->getInfo();
  }

  //----------

  MIP_NotePort* appendNoteOutputPort(MIP_NotePort* ANotePort) {
    MNoteOutputPorts.append(ANotePort);
    return ANotePort;
  }

  //----------

  //MIP_NotePort* appendClapNoteOutput() {
  MIP_NotePort* appendNoteOutput() {
    MIP_NotePort* port = new MIP_NotePort();
    MNoteOutputPorts.append(port);
    return port;
  }

  //----------

  void deleteNoteOutputPorts() {
    for (uint32_t i=0; i<MNoteOutputPorts.size(); i++) {
      if (MNoteOutputPorts[i]) delete MNoteOutputPorts[i];
    }
    MNoteOutputPorts.clear();
  }

  //----------

  const clap_note_port_info_t* getNoteOutputPort(uint32_t AIndex) {
    return MNoteOutputPorts[AIndex]->getInfo();
  }

//------------------------------
public: // queues
//------------------------------

  /*
    gui -> audio

    when we are changing a parameter from the gui,
    we also need to tell the audio processor about the changed parameters,
    so we queue up the events, and flush them all at the beginning of the
    next process (or params_flush) call
  */

  void queueProcessParam(uint32_t AIndex, double AValue) {
    //MIP_PRINT; // from gui
    MQueuedProcessParamValues.write(AValue);
    MProcessParamQueue.write(AIndex);
  }

  //----------

  /*
    send 'fake' param value events to our plugin
    so we can handle them via the usual methods.. make sure you call
    processParamValueEvent yourself if you override processEvent/s

    hmmm... processParamValueEvent will queueGuiParam..
    but widget already redraws itself..
    call on_plugin_parameter() directly here,
     and in processParamValueEvent
  */

  void flushProcessParams() {
    uint32_t index = 0;
    double value = 0.0;
    while (MProcessParamQueue.read(&index)) {
      MQueuedProcessParamValues.read(&value);
      //if (value != MFlushedProcessParamValues[index])
      clap_event_param_value_t event;
      event.header.size     = sizeof(clap_event_param_value_t);
      event.header.time     = 0;
      event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
      event.header.type     = CLAP_EVENT_PARAM_VALUE;
      event.header.flags    = 0; //CLAP_EVENT_IS_LIVE; // _DONT_RECORD
      event.param_id        = index;
      event.cookie          = MParameters[index];
      event.note_id         = -1;
      event.port_index      = -1;
      event.channel         = -1;
      event.key             = -1;
      event.value           = value;
    //processParamValue/*Event*/(&event);
      processParamValueEvent(&event);
      //MAudioprocessor->updateParameter(index,value);

    }
  }

  //----------

  /*
    gui -> host

    ..and we also need to tell the host about the changed parameter
    we send all events at the end of process (or params_flush)
  */

  void queueHostParam(uint32_t AIndex, double AValue) {
    //MIP_Print("%i = %f\n",AIndex,AValue);
    MQueuedHostParamValues.write(AValue);
    MHostParamQueue.write(AIndex);
  }

  //----------

  void flushHostParams(const clap_output_events_t* out_events) {
    uint32_t index = 0;
    double value = 0.0;
    while (MHostParamQueue.read(&index)) {
      MQueuedHostParamValues.read(&value);
      //double value = MParameters[index]->getValue();
      //MIP_Print("%i = %.3f\n",index,value);
      clap_event_param_gesture_t begin_gesture;
      begin_gesture.header.size     = sizeof(clap_event_param_gesture_t);
      begin_gesture.header.time     = 0;
      begin_gesture.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
      begin_gesture.header.type     = CLAP_EVENT_PARAM_GESTURE_BEGIN;
      begin_gesture.header.flags    = 0;  // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
      begin_gesture.param_id        = index;
      out_events->try_push(out_events,(clap_event_header_t*)&begin_gesture);

      clap_event_param_value_t event;
      event.header.size     = sizeof(clap_event_param_value_t);
      event.header.time     = 0;
      event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
      event.header.type     = CLAP_EVENT_PARAM_VALUE;
      event.header.flags    = 0;  // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
      event.param_id        = index;
      event.cookie          = nullptr;
      event.note_id         = -1;
      event.port_index      = -1;
      event.channel         = -1;
      event.key             = -1;
      event.value           = value;
      out_events->try_push(out_events,(clap_event_header_t*)&event);

      clap_event_param_gesture_t end_gesture;
      end_gesture.header.size     = sizeof(clap_event_param_gesture_t);
      end_gesture.header.time     = 0;
      end_gesture.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
      end_gesture.header.type     = CLAP_EVENT_PARAM_GESTURE_END;
      end_gesture.header.flags    = 0;  // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
      end_gesture.param_id        = index;
      out_events->try_push(out_events,(clap_event_header_t*)&end_gesture);

    }
  }

  //----------

  //queueHostNoteEnd
  //queueHostBeginGesture
  //queueHostEndGesture

  //----------

  #ifndef MIP_NO_GUI

  /*
    host -> gui

    when a parameter is updated, we also want to update the gui..
    we queue the events, and flush them all in a timer callback
  */

  void queueGuiParam(uint32_t AIndex, double AValue) {
    //MIP_Print("%i = %.3f\n",AIndex,AValue);
    MQueuedGuiParamValues.write(AValue);
    MGuiParamQueue.write(AIndex);
  }

  //----------

  /*
    todo: setup list, check for, and remove duplicates..
    MIP_PLUGIN_MAX_PARAM_EVENTS
  */

  void flushGuiParams() {
    //if (!MSuppressGuiTimer) {
      //MIP_PRINT;
      uint32_t index = 0;
      double value = 0.0;
      while (MGuiParamQueue.read(&index)) {
        MQueuedGuiParamValues.read(&value);
        //double value = MParameters[index]->getValue();
        //MIP_Print("%i = %.3f\n",index,value);
        MEditor->updateParameter(index,value);
      }
    //}
  }

  //----------

  /*
    and same thing with modulators..
  */

  void queueGuiMod(uint32_t AIndex, double AValue) {
    MQueuedGuiModValues.write(AValue);
    MGuiModQueue.write(AIndex);
  }

  //----------

  /*

    todo: setup list, check for, and remove duplicates..
    we can get 750 of these per second = 15 per update tick (20ms)
    MIP_PLUGIN_MAX_PARAM_EVENTS


  */

  void flushGuiMods() {
    //MIP_PRINT;
    uint32_t index = 0;
    double value = 0.0;
    while (MGuiModQueue.read(&index)) {
      MQueuedGuiModValues.read(&value);
      //double value = MParameters[index]->getValue();
      //MIP_Print("%i = %.3f\n",index,value);
      //MEditor->updateParameter(index,value);
    //  double current_value = MParameters[index]->getModulation();
    //  if (value != current_value) {
        MEditor->updateModulation(index,value);
    //  }
    }
  }

  #endif // MIP_NO_GUI

//------------------------------
public: // process audio
//------------------------------

  virtual void processAudioBlock(MIP_ProcessContext* AContext) {
  }

//------------------------------
public: // process events
//------------------------------

  virtual void processNoteOn(const clap_event_note_t* event) {}
  virtual void processNoteOff(const clap_event_note_t* event) {}
  virtual void processNoteChoke(const clap_event_note_t* event) {}
  virtual void processNoteEnd(const clap_event_note_t* event) {}
  virtual void processNoteExpression(const clap_event_note_expression_t* event) {}
  virtual void processParamValue(const clap_event_param_value_t* event) {}
  virtual void processParamMod(const clap_event_param_mod_t* event) {}
  virtual void processParamGestureBegin(const clap_event_param_gesture_t* event) {}
  virtual void processParamGestureEnd(const clap_event_param_gesture_t* event) {}
  virtual void processTransport(const clap_event_transport_t* event) {}
  virtual void processMidi(const clap_event_midi_t* event) {}
  virtual void processMidiSysex(const clap_event_midi_sysex_t* event) {}
  virtual void processMidi2(const clap_event_midi2_t* event) {}

//------------------------------
public: // process events
//------------------------------

  virtual void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    // setup voices
  }

  //----------

  virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    // cleanup voices, note ends..
  }

  //----------

  virtual void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        processEvent(header);
      }
    }
  }

  //----------

  void processEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:              processNoteOnEvent(             (const clap_event_note_t*)            header  );  break;
      case CLAP_EVENT_NOTE_OFF:             processNoteOffEvent(            (const clap_event_note_t*)            header  );  break;
      case CLAP_EVENT_NOTE_CHOKE:           processNoteChokeEvent(          (const clap_event_note_t*)            header  );  break;
      case CLAP_EVENT_NOTE_END:             processNoteEndEvent(            (const clap_event_note_t*)            header  );  break;
      case CLAP_EVENT_NOTE_EXPRESSION:      processNoteExpressionEvent(     (const clap_event_note_expression_t*) header  );  break;
      case CLAP_EVENT_PARAM_VALUE:          processParamValueEvent(         (const clap_event_param_value_t*)     header  );  break;
      case CLAP_EVENT_PARAM_MOD:            processParamModEvent(           (const clap_event_param_mod_t*)       header  );  break;
      case CLAP_EVENT_PARAM_GESTURE_BEGIN:  processParamGestureBeginEvent(  (const clap_event_param_gesture_t*)   header  );  break;
      case CLAP_EVENT_PARAM_GESTURE_END:    processParamGestureEndEvent(    (const clap_event_param_gesture_t*)   header  );  break;
      case CLAP_EVENT_TRANSPORT:            processTransportEvent(          (const clap_event_transport_t*)       header  );  break;
      case CLAP_EVENT_MIDI:                 processMidiEvent(               (const clap_event_midi_t*)            header  );  break;
      case CLAP_EVENT_MIDI_SYSEX:           processMidiSysexEvent(          (const clap_event_midi_sysex_t*)      header  );  break;
      case CLAP_EVENT_MIDI2:                processMidi2Event(              (const clap_event_midi2_t*)           header  );  break;
    }
  }

  //----------

  void processNoteOnEvent(const clap_event_note_t* event) {
    //MIP_Print("NOTE ON key %i note_id %i\n",event->key,event->note_id);
    processNoteOn(event);
  }

  //----------

  void processNoteOffEvent(const clap_event_note_t* event) {
    //MIP_Print("NOTE OFF key %i note_id %i\n",event->key,event->note_id);
    processNoteOff(event);
  }

  //----------

  void processNoteChokeEvent(const clap_event_note_t* event) {
    //MIP_Print("NOTE CHOKE key %i note_id %i\n",event->key,event->note_id);
    processNoteChoke(event);
  }

  //----------

  // not called.. (plugin -> host)
  void processNoteEndEvent(const clap_event_note_t* event) {
    //MIP_Print("NOTE END !\n");
    processNoteEnd(event);
  }

  //----------

  void processNoteExpressionEvent(const clap_event_note_expression_t* event) {
    //MIP_Print("NOTE EXPRESSION expr %i key %i note_id %i value %.3f\n",event->expression_id,event->key,event->note_id,event->value);
    processNoteExpression(event);
  }

  //----------

  void processParamValueEvent(const clap_event_param_value_t* event) {
    //MIP_Print("PARAM VALUE index %i value %.3f\n",event->param_id,event->value);
    uint32_t index = event->param_id;
    double value = event->value;
    setParameterValue(index,value);
    #ifndef MIP_NO_GUI
    //MIP_Print("%i = %f\n",index,value);
    if (MIsEditorOpen) queueGuiParam(index,value);
    #endif
    processParamValue(event);
  }

  //----------

  void processParamModEvent(const clap_event_param_mod_t* event) {
    //MIP_Print("PARAM MOD index %i value %.3f\n",event->param_id,event->amount);
    uint32_t index = event->param_id;
    double value = event->amount;
    setParameterModulation(index,value);
    #ifndef MIP_NO_GUI
    if (MIsEditorOpen) queueGuiMod(index,value);
    #endif
    processParamMod(event);
  }

  //----------

  // not called.. (plugin -> host)
  void processParamGestureBeginEvent(const clap_event_param_gesture_t* event) {
    //MIP_Print("PARAM GESTURE BEGIN\n");
    processParamGestureBegin(event);
  }

  //----------

  // not called.. (plugin -> host)
  void processParamGestureEndEvent(const clap_event_param_gesture_t* event) {
    //MIP_Print("PARAM GESTURE END\n");
    processParamGestureEnd(event);
  }

  //----------

  void processTransportEvent(const clap_event_transport_t* event) {
    //MIP_Print("TRANSPORT\n");
    processTransport(event);
  }

  //----------

  void processMidiEvent(const clap_event_midi_t* event) {
    //MIP_Print("MIDI\n");
    processMidi(event);
    /*
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
    */
  }

  //----------

  void processMidiSysexEvent(const clap_event_midi_sysex_t* event) {
    //MIP_Print("MIDI SYSEX\n");
    processMidiSysex(event);
  }

  //----------

  void processMidi2Event(const clap_event_midi2_t* event) {
    //MIP_Print("MIDI2\n");
    processMidi2(event);
  }

//------------------------------
public: // generic gui
//------------------------------

  #ifndef MIP_NO_GUI


  uint32_t getGenericNumControls() {
    uint32_t num = 0;
    for (uint32_t i=0; i<MParameters.size(); i++) {
      MIP_Parameter* parameter = MParameters[i];
      if (parameter->isAutomatable())
        if (!parameter->isHidden())
          num += 1;
    }
    return num;
  }

  //----------

  uint32_t getGenericWidth() {
    return MIP_PLUGIN_GENERIC_EDITOR_WIDTH;
  }

  //----------

  uint32_t getGenericHeight() {
    uint32_t numparams = getGenericNumControls();
    //return 80 + 10 + (numparams * 20) + ((numparams-1) * 5) + 10 + 25;
    return calcGenericHeight(numparams);
  }

  //----------

  uint32_t calcGenericHeight(uint32_t ANumParams) {
    return 80 + 10 + (ANumParams * 20) + ((ANumParams-1) * 5) + 10;// + 25;
  }

  //----------

  MIP_Widget* setupGenericEditor() {

    uint32_t w = getGenericWidth();
    uint32_t h = getGenericHeight();

    //----- background -----

    MIP_PanelWidget* panel = new MIP_PanelWidget(MIP_DRect(w,h));
    //panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //panel->Layout.aspectRatio = (double)w / (double)h;
    panel->setFillBackground(true);
    panel->setDrawBorder(false);
    panel->setBackgroundColor(0.55);

    //----- sa header -----

    //const char* name = getClapDescriptor()->name;
    //if ((name[0] == 's') && (name[1] == 'a') && (name[2] == '_')) name += 3;
    //const char* version = getClapDescriptor()->version;

    //MIP_SAHeaderWidget* saheader = new MIP_SAHeaderWidget(MIP_DRect(0,0,w,80),name,version);
    MIP_SAHeaderWidget* saheader = new MIP_SAHeaderWidget(MIP_DRect(0,0,w,80),getClapDescriptor());
    panel->appendChildWidget(saheader);

//    saheader->Layout.scaleMode = MIP_WIDGET_SCALE_MODE_INITIAL_RATIO;
//    saheader->setPluginName(name);
//    saheader->setPluginVersion(version);
//    saheader->setPluginVersion(getDescriptor()->version);

    //----- footer -----

//    MIP_TextWidget* footer_panel = new MIP_TextWidget(MIP_DRect(0,(h-25),w,25), "footer" );
//    panel->appendChildWidget(footer_panel);
//    footer_panel->Layout.scaleMode = MIP_WIDGET_SCALE_MODE_INITIAL_RATIO;
//    footer_panel->Layout.scaleMode = MIP_WIDGET_SCALE_MODE_INITIAL_RATIO;
//    footer_panel->setFillBackground(true);
//    footer_panel->setBackgroundColor(0.4);
//    footer_panel->setDrawBorder(false);
//    footer_panel->setDrawText(true);
//    footer_panel->setTextColor(MIP_Color(0.75));
//    footer_panel->setTextSize(-0.5);
//    footer_panel->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
//    footer_panel->setTextOffset(MIP_DPoint(5,0));

    //----- parameters -----

    for (uint32_t i=0; i<MParameters.size(); i++) {
      MIP_Parameter* parameter = MParameters[i];
      if (parameter->isAutomatable()) {
        if (!parameter->isHidden()) {
          const char* name = parameter->getName();
          double value = parameter->getDefValue();
          MIP_SliderWidget* slider = new MIP_SliderWidget( MIP_DRect(10, 90 + (25 * i), w - 20, 20),name,value);
          panel->appendChildWidget(slider);
          //slider->Layout.scaleMode = MIP_WIDGET_SCALE_MODE_INITIAL_RATIO;
          //slider->setTextSize(-0.8);
          //slider->setValueSize(-0.8);
          slider->setText(name);
          MEditor->connect(slider,parameter);
        }
      }
    }

    return panel;

  }

  #endif // MIP_NO_GUI

};

//----------------------------------------------------------------------
#endif
