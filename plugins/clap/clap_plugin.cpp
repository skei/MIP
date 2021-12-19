

// nc -U -l -k /tmp/mip.socket
//#define MIP_DEBUG_PRINT_SOCKET
#define MIP_NO_PLUGIN
#define MIP_NO_GUI
#include "mip.h"

#include "extern/clap/all.h"
#include "extern/clap/ext/draft/check-for-update.h"
#include "extern/clap/ext/draft/midi-mappings.h"

#include "x11_window.h"

//----------------------------------------------------------------------
//
// clap_plugin
//
//----------------------------------------------------------------------

class CLAP_Plugin {

//------------------------------
private:
//------------------------------

  clap_plugin*                  MPlugin               = nullptr;
  bool                          MAllocatedPlugin      = false;
  X11_Window*                   MWindow               = nullptr;

  // extensions

  clap_host_audio_ports*        MHostAudioPorts       = nullptr;
  clap_host_audio_ports_config* MHostAudioPortsConfig = nullptr;
//clap_host_check_for_update*   MHostCheckForUpdate   = nullptr;  // draft
  clap_host_event_filter*       MHostEventFilter      = nullptr;
  clap_host_fd_support*         MHostFdSupport        = nullptr;
  clap_host_file_reference*     MHostFileReference    = nullptr;  // draft
  clap_host_gui*                MHostGui              = nullptr;
  clap_host_latency*            MHostLatency          = nullptr;
  clap_host_log*                MHostLog              = nullptr;
  clap_host_midi_mappings*      MHostMidiMappings     = nullptr;  // draft
  clap_host_note_name*          MHostNoteName         = nullptr;
  clap_host_note_ports*         MHostNotePorts        = nullptr;  //
  clap_host_params*             MHostParams           = nullptr;
  clap_host_quick_controls*     MHostQuickControls    = nullptr;  // draft
  clap_host_state*              MHostState            = nullptr;
  clap_host_thread_check*       MHostThreadCheck      = nullptr;
  clap_host_thread_pool*        MHostThreadPool       = nullptr;  // draft
  clap_host_timer_support*      MHostTimerSupport     = nullptr;
  clap_host_track_info*         MHostTrackInfo        = nullptr;  // draft
  clap_host_tuning*             MHostTuning           = nullptr;  // draft

//------------------------------
protected:
//------------------------------

  float     MParameterValue     = 0.0;
  uint32_t  MSelectedPortConfig = 0;;

//------------------------------
public:
//------------------------------

  //CLAP_Plugin(const clap_plugin* plugin) {
  //  MPlugin = (clap_plugin*)plugin;
  //  MAllocatedPlugin = false;
  //}

  //----------

  CLAP_Plugin(const clap_plugin_descriptor* desc, const clap_host *host) {

    MPlugin           = (clap_plugin*)malloc(sizeof(clap_plugin));
    MAllocatedPlugin  = true;

    MPlugin->desc              = desc;
    MPlugin->plugin_data       = this;
    MPlugin->init              = clap_plugin_init_callback;
    MPlugin->destroy           = clap_plugin_destroy_callback;
    MPlugin->activate          = clap_plugin_activate_callback;
    MPlugin->deactivate        = clap_plugin_deactivate_callback;
    MPlugin->start_processing  = clap_plugin_start_processing_callback;
    MPlugin->stop_processing   = clap_plugin_stop_processing_callback;
    MPlugin->process           = clap_plugin_process_callback;
    MPlugin->get_extension     = clap_plugin_get_extension_callback;
    MPlugin->on_main_thread    = clap_plugin_on_main_thread_callback;

    // extensions

    MHostAudioPorts       = (clap_host_audio_ports*)host->get_extension(host,CLAP_EXT_AUDIO_PORTS);
    MHostAudioPortsConfig = (clap_host_audio_ports_config*)host->get_extension(host,CLAP_EXT_AUDIO_PORTS_CONFIG);
  //MHostCheckForUpdate   = (clap_host_check_for_update*)host->get_extension(host,CLAP_EXT_CHECK_FOR_UPDATE);
    MHostEventFilter      = (clap_host_event_filter*)host->get_extension(host,CLAP_EXT_EVENT_FILTER);
    MHostFdSupport        = (clap_host_fd_support*)host->get_extension(host,CLAP_EXT_FD_SUPPORT);
    MHostFileReference    = (clap_host_file_reference*)host->get_extension(host,CLAP_EXT_FILE_REFERENCE);
    MHostGui              = (clap_host_gui*)host->get_extension(host,CLAP_EXT_GUI);
    MHostLatency          = (clap_host_latency*)host->get_extension(host,CLAP_EXT_LATENCY);
    MHostLog              = (clap_host_log*)host->get_extension(host,CLAP_EXT_LOG);
    MHostMidiMappings     = (clap_host_midi_mappings*)host->get_extension(host,CLAP_EXT_MIDI_MAPPINGS);
    MHostNoteName         = (clap_host_note_name*)host->get_extension(host,CLAP_EXT_NOTE_NAME);
    MHostNotePorts        = (clap_host_note_ports*)host->get_extension(host,CLAP_EXT_NOTE_PORTS);
    MHostParams           = (clap_host_params*)host->get_extension(host,CLAP_EXT_PARAMS);
    MHostQuickControls    = (clap_host_quick_controls*)host->get_extension(host,CLAP_EXT_QUICK_CONTROLS);
    MHostState            = (clap_host_state*)host->get_extension(host,CLAP_EXT_STATE);
    MHostThreadCheck      = (clap_host_thread_check*)host->get_extension(host,CLAP_EXT_THREAD_CHECK);
    MHostThreadPool       = (clap_host_thread_pool*)host->get_extension(host,CLAP_EXT_THREAD_POOL);
    MHostTimerSupport     = (clap_host_timer_support*)host->get_extension(host,CLAP_EXT_TIMER_SUPPORT);
    MHostTrackInfo        = (clap_host_track_info*)host->get_extension(host,CLAP_EXT_TRACK_INFO);
    MHostTuning           = (clap_host_tuning*)host->get_extension(host,CLAP_EXT_TUNING);
  }

  //----------

  virtual ~CLAP_Plugin() {
    if (MAllocatedPlugin && MPlugin) free(MPlugin);
  }

//------------------------------
public:
//------------------------------

  const clap_plugin* getClapPlugin() { return MPlugin; }

//------------------------------
public: // plugin
//------------------------------

  bool init() {
    MIP_PRINT;
    return true;
  }

  //----------

  void destroy() {
    MIP_PRINT;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    MIP_PRINT;
    return true;
  }

  //----------

  void deactivate() {
    MIP_PRINT;
  }

  //----------

  bool start_processing() {
    MIP_PRINT;
    return true;
  }

  //----------

  void stop_processing() {
    MIP_PRINT;
  }

  //----------

  clap_process_status process(const clap_process *process) {
    //MIP_PRINT;

    // events

    if (process->in_events) {
      uint32_t num_events = process->in_events->size(process->in_events);
      for (uint32_t i=0; i<num_events; i++) {
        const clap_event* event = process->in_events->get(process->in_events,i);
        if (event) {
          switch (event->type) {

            case CLAP_EVENT_PARAM_VALUE:
              //handleParamValue(event);
              {
                uint32_t i = event->param_value.param_id;
                float v = event->param_value.value;
                if (i == 0) MParameterValue = (v*v);
              }
              break;

            //case CLAP_EVENT_NOTE_ON:          handleNoteOn(event);          break;
            //case CLAP_EVENT_NOTE_OFF:         handleNoteOff(event);         break;
            //case CLAP_EVENT_NOTE_END:         handleNoteEnd(event);         break;
            //case CLAP_EVENT_NOTE_CHOKE:       handleNoteChoke(event);       break;
            //case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpression(event);  break;
            //case CLAP_EVENT_NOTE_MASK:        handleNoteMask(event);        break;
            //case CLAP_EVENT_PARAM_MOD:        handleParamMod(event);        break;
            //case CLAP_EVENT_TRANSPORT:        handleTransport(event);       break;
            //case CLAP_EVENT_MIDI:             handleMidi(event);            break;
            //case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysex(event);       break;
          }
        }
      }
    }

    // process

    float* in0 = process->audio_inputs[0].data32[0];
    float* in1 = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<process->frames_count; i++) {
      *out0++ = *in0++ * MParameterValue;
      *out1++ = *in1++ * MParameterValue;
    }

    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  // return NULL for extensions you don't suooprt/want

  const void* get_extension(const char *id) {
    MIP_Print("id: %s\n",id);

    if (strcmp(id, CLAP_EXT_AUDIO_PORTS         ) == 0) return &MClapAudioPorts;
    if (strcmp(id, CLAP_EXT_AUDIO_PORTS_CONFIG  ) == 0) return &MClapAudioPortsConfig;
  //if (strcmp(id, CLAP_EXT_CHECK_FOR_UPDATE    ) == 0) return &MClapCheckForUpdate;
    if (strcmp(id, CLAP_EXT_EVENT_FILTER        ) == 0) return &MClapEventFilter;
    if (strcmp(id, CLAP_EXT_FD_SUPPORT          ) == 0) return &MClapFdSupport;
    if (strcmp(id, CLAP_EXT_FILE_REFERENCE      ) == 0) return &MClapFileReference;
    if (strcmp(id, CLAP_EXT_GUI                 ) == 0) return &MClapGui;
    if (strcmp(id, CLAP_EXT_GUI_X11             ) == 0) return &MClapGuiX11;
    if (strcmp(id, CLAP_EXT_LATENCY             ) == 0) return &MClapLatency;
    if (strcmp(id, CLAP_EXT_MIDI_MAPPINGS       ) == 0) return &MClapMidiMappings;
    if (strcmp(id, CLAP_EXT_NOTE_NAME           ) == 0) return &MClapNoteName;
    if (strcmp(id, CLAP_EXT_NOTE_PORTS          ) == 0) return &MClapNotePorts;
    if (strcmp(id, CLAP_EXT_PARAMS              ) == 0) return &MClapParams;
    if (strcmp(id, CLAP_EXT_PRESET_LOAD         ) == 0) return &MClapPresetLoad;
    if (strcmp(id, CLAP_EXT_QUICK_CONTROLS      ) == 0) return &MClapQuickControls;
    if (strcmp(id, CLAP_EXT_RENDER              ) == 0) return &MClapRender;
    if (strcmp(id, CLAP_EXT_STATE               ) == 0) return &MClapState;
    if (strcmp(id, CLAP_EXT_TIMER_SUPPORT       ) == 0) return &MClapTimerSupport;
    if (strcmp(id, CLAP_EXT_THREAD_POOL         ) == 0) return &MClapThreadPool;
    if (strcmp(id, CLAP_EXT_TRACK_INFO          ) == 0) return &MClapTrackInfo;
    if (strcmp(id, CLAP_EXT_VST2_CONVERT        ) == 0) return &MClapVst2Convert;
    if (strcmp(id, CLAP_EXT_VST3_CONVERT        ) == 0) return &MClapVst3Convert;
    return nullptr;
  }

  //----------

  void on_main_thread() {
    MIP_PRINT;
  }

//------------------------------
public: // extensions
//------------------------------

  //------------------------------
  // audio ports
  //------------------------------

  uint32_t audio_ports_count(bool is_input) {
    MIP_PRINT;
    if (is_input) return 1;
    else return 1;
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info *info) {
    MIP_PRINT;
    if (is_input) {
      info->id            = 0;
      strncpy(info->name,"input",CLAP_NAME_SIZE-1);
      info->channel_count = 2;
      info->channel_map   = CLAP_CHMAP_STEREO;
      info->sample_size   = 32;
      info->is_main       = true;
      info->is_cv         = false;
      info->in_place      = true;
      return true;
    }
    else { // output
      info->id            = 0;
      strncpy(info->name,"output",CLAP_NAME_SIZE-1);
      info->channel_count = 2;
      info->channel_map   = CLAP_CHMAP_STEREO;
      info->sample_size   = 32;
      info->is_main       = true;
      info->is_cv         = false;
      info->in_place      = true;
      return true;
    }
    return false;
  }

  //------------------------------
  // audio ports config
  //------------------------------

  uint32_t audio_ports_config_count() {
    MIP_PRINT;
    return 1;
  }

  //----------

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config *config) {
    MIP_PRINT;
    switch(index) {
      case 0:
        config->id                    = 0;
        strncpy(config->name,"port config 1",CLAP_NAME_SIZE-1);
        config->input_channel_count   = 2;
        config->input_channel_map     = CLAP_CHMAP_STEREO;
        config->output_channel_count  = 2;
        config->output_channel_map    = CLAP_CHMAP_STEREO;
        return true;
    }
    return false;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) {
    MIP_PRINT;
    if (config_id == 0) {
      MSelectedPortConfig = config_id;
    }
    return true;
  }

  //------------------------------
  // check for update
  //------------------------------

  void check_for_update_check(bool include_beta) {
    MIP_PRINT;
  }

  //------------------------------
  // event filter
  //------------------------------

  // return false for events you don't want

  bool event_filter_accepts(clap_event_type event_type) {
    MIP_PRINT;
    switch (event_type) {
      case CLAP_EVENT_NOTE_ON:          return true;
      case CLAP_EVENT_NOTE_OFF:         return true;
      case CLAP_EVENT_NOTE_END:         return true;
      case CLAP_EVENT_NOTE_CHOKE:       return true;
      case CLAP_EVENT_NOTE_EXPRESSION:  return true;
      case CLAP_EVENT_NOTE_MASK:        return true;
      case CLAP_EVENT_PARAM_VALUE:      return true;
      case CLAP_EVENT_PARAM_MOD:        return true;
      case CLAP_EVENT_TRANSPORT:        return true;
      case CLAP_EVENT_MIDI:             return true;
      case CLAP_EVENT_MIDI_SYSEX:       return true;
    }
    return false;
  }

  //------------------------------
  // fd support
  //------------------------------

  void fd_support_on_fd(clap_fd fd, clap_fd_flags flags) {
    MIP_PRINT;
  }

  //------------------------------
  // file reference
  //------------------------------

  uint32_t file_reference_count() {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool file_reference_get(uint32_t index, clap_file_reference *file_reference) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool file_reference_get_hash(clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool file_reference_update_path(clap_id resource_id, const char *path) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool file_reference_save_resources() {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // gui
  //------------------------------

  bool gui_create() {
    MIP_PRINT;
    MWindow = new X11_Window();
    return true;
  }

  //----------

  void gui_destroy() {
    MIP_PRINT;
    delete MWindow;
  }

  //----------

  void gui_set_scale(double scale) {
    MIP_PRINT;
    MWindow->set_scale(scale);
  }

  //----------

  bool gui_get_size(uint32_t* width, uint32_t* height) {
    MIP_PRINT;
    *width = MWindow->get_width();
    *width = MWindow->get_height();
    return true;
  }

  //----------

  bool gui_can_resize() {
    MIP_PRINT;
    return false;
  }

  //----------

  void gui_round_size(uint32_t* width, uint32_t* height) {
    MIP_PRINT;
    *width = MWindow->get_width();
    *width = MWindow->get_height();
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) {
    MIP_PRINT;
    MWindow->set_width(width);
    MWindow->set_height(height);
    return true;
  }

  //----------

  void gui_show() {
    MIP_PRINT;
    MWindow->show();
  }

  //----------

  void gui_hide() {
    MIP_PRINT;
    MWindow->hide();
  }

  //------------------------------
  // gui x11
  //------------------------------

  bool gui_x11_attach(const char* display_name, unsigned long window) {
    MIP_PRINT;
    return MWindow->attach(display_name,window);
  }

  //------------------------------
  // latency
  //------------------------------

  uint32_t latency_get() {
    MIP_PRINT;
    return 0;
  }

  //------------------------------
  // midi mappings
  //------------------------------

  uint32_t midi_mappings_count() {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping* mapping) {
    MIP_PRINT;
    //mapping->channel  =
    //mapping->number   =
    //mapping->param_id =
    return false;
  }

  //------------------------------
  // note name
  //------------------------------

  uint32_t note_name_count() {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool note_name_get(uint32_t index, clap_note_name* note_name) {
    MIP_PRINT;
    //strncpy(note_name->name,"note",CLAP_NAME_SIZE-1);
    //note_name->port     =
    //ote_name->key      =
    //note_name->channel  =
    return false;
  }

  //------------------------------
  // note-ports
  //------------------------------

  uint32_t note_ports_count(bool is_input) {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info* info) {
    MIP_PRINT;
    //info->id = 0;
    //strncpy(info->name,"port",CLAP_NAME_SIZE-1);
    return false;
  }

  //------------------------------
  // params
  //------------------------------

  uint32_t params_count() {
    MIP_PRINT;
    return 1;
  }

  //----------

  bool params_get_info(int32_t param_index, clap_param_info *param_info) {
    MIP_PRINT;
    if (param_index == 0) {
      param_info->id            = param_index;
      param_info->flags         = 0; // CLAP_PARAM_IS_HIDDEN, CLAP_PARAM_IS_READONLY, CLAP_PARAM_IS_MODULATABLE, CLAP_PARAM_IS_STEPPED, ..
      param_info->cookie        = nullptr;
      strncpy(param_info->name,"parameter",CLAP_NAME_SIZE-1);
      strncpy(param_info->module,"",CLAP_MODULE_SIZE-1);
      param_info->min_value     = 0.0;
      param_info->max_value     = 1.0;
      param_info->default_value = 0.5;
      return true;
    }
    return false;
  }

  //----------

  bool params_get_value(clap_id param_id, double* value) {
    MIP_PRINT;
    *value = MParameterValue;;
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char* display, uint32_t size) {
    MIP_PRINT;
    strncpy(display,"0.0 %",CLAP_NAME_SIZE-1);
    return false;
  }

  //----------

  bool params_text_to_value(clap_id param_id, const char* display, double* value) {
    MIP_PRINT;
    float f = atof(display);
    *value = f;
    return true;
  }

  //----------

  void params_flush(const clap_event_list *input_parameter_changes, const clap_event_list *output_parameter_changes) {
    MIP_PRINT;
  }

  //------------------------------
  // preset load
  //------------------------------

  bool preset_load_from_file(const char *path) {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // quick controls
  //------------------------------

  uint32_t quick_controls_count() {
    MIP_PRINT;
    return 0;
  }

  //----------

//typedef struct clap_quick_controls_page {
//   clap_id id;
//   char    name[CLAP_NAME_SIZE];
//   char    keywords[CLAP_KEYWORDS_SIZE];
//   clap_id param_ids[CLAP_QUICK_CONTROLS_COUNT];
//} clap_quick_controls_page;

  bool quick_controls_get(uint32_t page_index, clap_quick_controls_page *page) {
    MIP_PRINT;
    return false;
  }

  //----------

  void quick_controls_select(clap_id page_id) {
    MIP_PRINT;
  }

  //----------

  clap_id quick_controls_get_selected() {
    MIP_PRINT;
    return 0;
  }

  //------------------------------
  // render
  //------------------------------

  //CLAP_RENDER_REALTIME
  //CLAP_RENDER_OFFLINE

  void render_set(clap_plugin_render_mode mode) {
    MIP_PRINT;
  }

  //------------------------------
  // state
  //------------------------------

  //typedef struct clap_ostream {
  //   void *ctx;
  //   int64_t (*write)(struct clap_ostream *stream, const void *buffer, uint64_t size);
  //} clap_ostream

  bool state_save(clap_ostream *stream) {
    MIP_PRINT;
    return true;
  }

  //----------

  //typedef struct clap_istream {
  //  void *ctx;
  //  int64_t (*read)(struct clap_istream *stream, void *buffer, uint64_t size);
  //} clap_istream;

  bool state_load(clap_istream *stream) {
    MIP_PRINT;
    return true;
  }

  //------------------------------
  // thread pool
  //------------------------------

  void thread_pool_exec(uint32_t task_index) {
    MIP_PRINT;
  }

  //------------------------------
  // timer support
  //------------------------------

  void timer_support_on_timer(clap_id timer_id) {
    MIP_PRINT;
  }

  //------------------------------
  // track info
  //------------------------------

  void track_info_changed() {
    MIP_PRINT;
  }

  //------------------------------
  // vst2 convert
  //------------------------------

  int32_t vst2_convert_get_vst2_plugin_id(uint32_t* vst2_plugin_id, char* name, uint32_t name_size) {
    MIP_PRINT;
    return 0;
  }

  //----------

  bool vst2_convert_restore_vst2_state(clap_istream *stream) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool vst2_convert_convert_normalized_value(uint32_t vst2_param_id, double vst2_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool vst2_convert_convert_plain_value(uint32_t vst2_param_id, double vst2_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // vst3 convert
  //------------------------------

  void vst3_convert_get_vst3_plugin_id(const clap_plugin *plugin, uint8_t *vst3_plugin_uuid) {
    MIP_PRINT;
  }

  //----------

  bool vst3_convert_restore_vst3_state(const clap_plugin *plugin, clap_istream *stream) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool vst3_convert_convert_normalized_value(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool vst3_convert_convert_plain_value(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_PRINT;
    return false;
  }

//------------------------------
private: // callbacks
//------------------------------

  static bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->init();
  }

  static void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->destroy();
  }

  static bool clap_plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static void clap_plugin_deactivate_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->deactivate();
  }

  static bool clap_plugin_start_processing_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static void clap_plugin_stop_processing_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->stop_processing();
  }

  static clap_process_status clap_plugin_process_callback(const struct clap_plugin *plugin, const clap_process *process) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static const void* clap_plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static void clap_plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->on_main_thread();
  }

//------------------------------
private: // extension callbacks
//------------------------------

  // clap.audio-ports-config

  struct clap_plugin_audio_ports_config MClapAudioPortsConfig = {
    clap_audio_ports_config_count_callback,
    clap_audio_ports_config_get_callback,
    clap_audio_ports_config_select_callback
  };

  static uint32_t clap_audio_ports_config_count_callback(const clap_plugin* plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_config_count();
  }

  static bool clap_audio_ports_config_get_callback(const clap_plugin* plugin, uint32_t index, clap_audio_ports_config *config) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_config_get(index,config);
  }

  static bool clap_audio_ports_config_select_callback(const clap_plugin* plugin, clap_id config_id) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_config_select(config_id);
  }

  // clap.audio-ports

  struct clap_plugin_audio_ports MClapAudioPorts =  {
    clap_audio_ports_count_callback,
    clap_audio_ports_get_callback
  };

  static uint32_t clap_audio_ports_count_callback(const clap_plugin *plugin, bool is_input) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_count(is_input);

  }

  static bool clap_audio_ports_get_callback(const clap_plugin* plugin, uint32_t index, bool is_input, clap_audio_port_info *info) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->audio_ports_get(index,is_input,info);
  };

  // clap.check-for-updates

  //struct clap_plugin_check_for_update MClapCheckForUpdate = {
  //  clap_check_for_update_check_callback
  //};
  //
  //static void clap_check_for_update_check_callback(const clap_host *host, bool include_beta) {
  //  CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
  //}

  // clap.event-filter

  struct clap_plugin_event_filter MClapEventFilter = {
    clap_event_filter_accepts_callback
  };

  static bool clap_event_filter_accepts_callback(const clap_plugin *plugin, clap_event_type event_type) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->event_filter_accepts(event_type);
  }

  // clap.fd-support

  struct clap_plugin_fd_support MClapFdSupport = {
    clap_fd_support_on_fd_callback
  };

  static void clap_fd_support_on_fd_callback(const clap_plugin *plugin, clap_fd fd, clap_fd_flags flags) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->fd_support_on_fd(fd,flags);
  }

  // clap.file-reference

  struct clap_plugin_file_reference MClapFileReference = {
    clap_file_reference_count_callback,
    clap_file_reference_get_callback,
    clap_file_reference_get_hash_callback,
    clap_file_reference_update_path_callback,
    clap_file_reference_save_resources_callback
  };

  static uint32_t clap_file_reference_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_count();
  }

  static bool clap_file_reference_get_callback(const clap_plugin *plugin, uint32_t index, clap_file_reference *file_reference) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_get(index,file_reference);
  }

  static bool clap_file_reference_get_hash_callback(const clap_plugin *plugin, clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_get_hash(resource_id,hash,digest,digest_size);
  }

  static bool clap_file_reference_update_path_callback(const clap_plugin *plugin,  clap_id resource_id, const char *path) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_update_path(resource_id,path);
  }

  static bool clap_file_reference_save_resources_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->file_reference_save_resources();
  }

  // clap.gui

  struct clap_plugin_gui MClapGui = {
    clap_gui_create_callback,
    clap_gui_destroy_callback,
    clap_gui_set_scale_callback,
    clap_gui_get_size_callback,
    clap_gui_can_resize_callback,
    clap_gui_round_size_callback,
    clap_gui_set_size_callback,
    clap_gui_show_callback,
    clap_gui_hide_callback
  };

  static bool clap_gui_create_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_create();
  }

  static void clap_gui_destroy_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_destroy();
  }

  static void clap_gui_set_scale_callback(const clap_plugin *plugin, double scale) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_set_scale(scale);
  }

  static bool clap_gui_get_size_callback(const clap_plugin *plugin, uint32_t *width, uint32_t *height) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_get_size(width,height);
  }

  static bool clap_gui_can_resize_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_can_resize();
  }

  static void clap_gui_round_size_callback(const clap_plugin *plugin, uint32_t *width, uint32_t *height) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_round_size(width,height);
  }

  static bool clap_gui_set_size_callback(const clap_plugin *plugin, uint32_t width, uint32_t height) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_set_size(width,height);
  }

  static void clap_gui_show_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_show();
  }

  static void clap_gui_hide_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->gui_hide();
  }

  // clap.gui-x11

  struct clap_plugin_gui_x11 MClapGuiX11 = {
    clap_gui_x11_attach_callback
  };

  static bool clap_gui_x11_attach_callback(const clap_plugin * plugin, const char* display_name, unsigned long window) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->gui_x11_attach(display_name,window);
  }

  // clap.latency

  struct clap_plugin_latency MClapLatency = {
    clap_latency_get_callback
  };

  static uint32_t clap_latency_get_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->latency_get();
  }

  // clap.midi-mappings

  struct clap_plugin_midi_mappings MClapMidiMappings = {
    clap_midi_mappings_count_callback,
    clap_midi_mappings_get_callback
  };

  static uint32_t clap_midi_mappings_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->midi_mappings_count();
  }

  static bool clap_midi_mappings_get_callback(const clap_plugin *plugin, uint32_t index, clap_midi_mapping *mapping) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->midi_mappings_get(index,mapping);
  }

  // clap.note-name

  struct clap_plugin_note_name MClapNoteName = {
    clap_note_name_count_callback,
    clap_note_name_get_callback
  };

  static uint32_t clap_note_name_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->note_name_count();
  }

  static bool clap_note_name_get_callback(const clap_plugin *plugin, uint32_t index, clap_note_name *note_name) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->note_name_get(index,note_name);
  }

  // clap.note-ports

  struct clap_plugin_note_ports MClapNotePorts = {
    clap_note_ports_count_callback,
    clap_note_ports_get_callback
  };

  static uint32_t clap_note_ports_count_callback(const clap_plugin *plugin, bool is_input) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->note_ports_count(is_input);
  }

  static bool clap_note_ports_get_callback(const clap_plugin* plugin, uint32_t index, bool is_input, clap_note_port_info *info) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->note_ports_get(index,is_input,info);
  }

  // clap.params

  struct clap_plugin_params MClapParams = {
    clap_params_count_callback,
    clap_params_get_info_callback,
    clap_params_get_value_callback,
    clap_params_value_to_text_callback,
    clap_params_text_to_value_callback,
    clap_params_flush_callback
  };

  static uint32_t clap_params_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static bool clap_params_get_info_callback(const clap_plugin *plugin, int32_t param_index, clap_param_info *param_info) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static bool clap_params_get_value_callback(const clap_plugin *plugin, clap_id param_id, double *value) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,value);
  }

  static bool clap_params_value_to_text_callback(const clap_plugin *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,display,size);
  }

  static bool clap_params_text_to_value_callback(const clap_plugin *plugin, clap_id param_id, const char* display, double* value) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,display,value);
  }

  static void clap_params_flush_callback(const clap_plugin* plugin, const clap_event_list *input_parameter_changes, const clap_event_list *output_parameter_changes) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->params_flush(input_parameter_changes,output_parameter_changes);
  }

  // clap.preset-load

  struct clap_plugin_preset_load MClapPresetLoad = {
    clap_preset_load_from_file_callback
  };

  static bool clap_preset_load_from_file_callback(const clap_plugin *plugin, const char *path) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->preset_load_from_file(path);
  }

  // clap.quick-controls

  struct clap_plugin_quick_controls MClapQuickControls = {
    clap_quick_controls_count_callback,
    clap_quick_controls_get_callback,
    clap_quick_controls_select_callback,
    clap_quick_controls_get_selected_callback
  };

  static uint32_t clap_quick_controls_count_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->quick_controls_count();
  }

  static bool clap_quick_controls_get_callback(const clap_plugin *plugin, uint32_t page_index, clap_quick_controls_page *page) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->quick_controls_get(page_index,page);
  }

  static void clap_quick_controls_select_callback(const clap_plugin *plugin, clap_id page_id) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->quick_controls_select(page_id);
  }

  static clap_id clap_quick_controls_get_selected_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->quick_controls_get_selected();
  }

  // clap.render

  struct clap_plugin_render MClapRender = {
    clap_render_set_callback
  };

  static void clap_render_set_callback(const clap_plugin *plugin, clap_plugin_render_mode mode) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->render_set(mode);
  }

  // clap.state

  struct clap_plugin_state MClapState = {
    clap_state_save_callback,
    clap_state_load_callback
  };

  static bool clap_state_save_callback(const clap_plugin *plugin, clap_ostream *stream) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->state_save(stream);
  }

  static bool clap_state_load_callback(const clap_plugin *plugin, clap_istream *stream) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->state_load(stream);
  }

  // clap.thread-pool

  struct clap_plugin_thread_pool MClapThreadPool = {
    clap_thread_pool_exec_callback
  };

  static void clap_thread_pool_exec_callback(const clap_plugin *plugin, uint32_t task_index) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->thread_pool_exec(task_index);
  }

  // clap.timer-support

  struct clap_plugin_timer_support MClapTimerSupport = {
    clap_timer_support_on_timer_callback
  };

  static void clap_timer_support_on_timer_callback(const clap_plugin *plugin, clap_id timer_id) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->timer_support_on_timer(timer_id);
  }

  // clap.track-info

  struct clap_plugin_track_info MClapTrackInfo = {
    clap_track_info_changed_callback
  };

  static void clap_track_info_changed_callback(const clap_plugin *plugin) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    plug->track_info_changed();
  }

  // clap.vst2-convert

  struct clap_plugin_vst2_convert MClapVst2Convert = {
    clap_vst2_convert_get_vst2_plugin_id_callback,
    clap_vst2_convert_restore_vst2_state_callback,
    clap_vst2_convert_convert_normalized_value_callback,
    clap_vst2_convert_convert_plain_value_callback
  };

  static int32_t clap_vst2_convert_get_vst2_plugin_id_callback(const clap_plugin *plugin, uint32_t* vst2_plugin_id, char* name, uint32_t name_size) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->vst2_convert_get_vst2_plugin_id(vst2_plugin_id,name,name_size);
  }

  static bool clap_vst2_convert_restore_vst2_state_callback(const clap_plugin *plugin, clap_istream *stream) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->vst2_convert_restore_vst2_state(stream);
  }

  static bool clap_vst2_convert_convert_normalized_value_callback(const clap_plugin *plugin, uint32_t vst2_param_id, double vst2_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->vst2_convert_convert_normalized_value(vst2_param_id,vst2_normalized_value,clap_param_id,clap_plain_value);
  }

  static bool clap_vst2_convert_convert_plain_value_callback(const clap_plugin *plugin, uint32_t vst2_param_id, double vst2_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    return plug->vst2_convert_convert_plain_value(vst2_param_id,vst2_plain_value,clap_param_id,clap_plain_value);
  }

  // clap.vst3-convert

  struct clap_plugin_vst3_convert MClapVst3Convert = {
    clap_vst3_convert_get_vst3_plugin_id_callback,
    clap_vst3_convert_restore_vst3_state_callback,
    clap_vst3_convert_convert_normalized_value_callback,
    clap_vst3_convert_convert_plain_value_callback
  };

  static void clap_vst3_convert_get_vst3_plugin_id_callback(const clap_plugin *plugin, uint8_t *vst3_plugin_uuid) {
    //CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    //plug->vst3_convert_get_vst3_plugin_id(vst3_plugin_uuid);
  }

  static bool clap_vst3_convert_restore_vst3_state_callback(const clap_plugin *plugin, clap_istream *stream) {
    //CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    //return plug->vst3_convert_restore_vst3_state(stream);
    return false;
  }

  static bool clap_vst3_convert_convert_normalized_value_callback(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    //CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    //return plug->vst3_convert_convert_normalized_value_callback(vst3_param_id,vst3_normalized_value,clap_param_id,clap_plain_value);
    return false;
  }

  static bool clap_vst3_convert_convert_plain_value_callback(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    //CLAP_Plugin* plug = (CLAP_Plugin*)plugin->plugin_data;
    //return->vst3_convert_convert_plain_value(vst3_param_id,vst3_plain_value,clap_param_id,clap_plain_value);
    return false;
  }

};

//----------------------------------------------------------------------
//
// clap_host
//
//----------------------------------------------------------------------

//...


//----------------------------------------------------------------------
//
// clap_plugin_entry
//
//----------------------------------------------------------------------

class CLAP_PluginEntry {

//------------------------------
private:
//------------------------------

  clap_plugin_descriptor  MDescriptor = {
    CLAP_VERSION,
    "CLAP_Plugin",              // id
    "CLAP_Plugin",              // name
    "Tor-Helge Skei",           // vendor
    "https://torhelgeskei.com", // url
    "manual_url",               // manual url
    "support_url",              // support url
    "0.0.1",                    // version
    "clap test plugin",         // description
    "clap;test;plugin",         // keywords
    CLAP_PLUGIN_AUDIO_EFFECT
  };

//------------------------------
public:
//------------------------------

  CLAP_PluginEntry() {
  }

  virtual ~CLAP_PluginEntry() {
  }

//------------------------------
public: // your plugin
//------------------------------

  bool init(const char *plugin_path) {
    MIP_PRINT;
    return true;
  }

  void deinit(void) {
    MIP_PRINT;
  }

  uint32_t get_plugin_count(void) {
    MIP_PRINT;
    return 1;
  }

  const clap_plugin_descriptor* get_plugin_descriptor(uint32_t index) {
    MIP_PRINT;
    return &MDescriptor;
  }

  const clap_plugin* create_plugin(const clap_host *host, const char *plugin_id) {
    MIP_PRINT;
    CLAP_Plugin* plugin = new CLAP_Plugin(&MDescriptor,host);
    return plugin->getClapPlugin();
  }

  uint32_t get_invalidation_source_count(void) {
    MIP_PRINT;
    return 0;
  }

  const clap_plugin_invalidation_source* get_invalidation_source(uint32_t index) {
    MIP_PRINT;
    return nullptr;
  }

  void refresh(void) {
    MIP_PRINT;
  }

};

//----------------------------------------------------------------------

// our plugin entry

CLAP_PluginEntry GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT;

//----------------------------------------------------------------------

// clap callbacks
// trampoline into our entry class

static
bool clap_plugin_entry_init_callback(const char *plugin_path) {
  return GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT.init(plugin_path);
}

static
void clap_plugin_entry_deinit_callback(void) {
  GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT.deinit();
}

static
uint32_t clap_plugin_entry_get_plugin_count_callback(void) {
  return GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT.get_plugin_count();
}

static
const clap_plugin_descriptor *clap_plugin_entry_get_plugin_descriptor_callback(uint32_t index) {
  return GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT.get_plugin_descriptor(index);
}

static
const clap_plugin *clap_plugin_entry_create_plugin_callback(const clap_host *host, const char *plugin_id) {
  return GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT.create_plugin(host,plugin_id);
}

static
uint32_t clap_plugin_entry_get_invalidation_source_count_callback(void) {
  return GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT.get_invalidation_source_count();
}

static
const clap_plugin_invalidation_source *clap_plugin_entry_get_invalidation_source_callback(uint32_t index) {
  return GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT.get_invalidation_source(index);
}

static
void clap_plugin_entry_refresh_callback(void) {
  GLOBAL_CLAP_PLUGIN_ENTRY_STRUCT.refresh();
}

//----------------------------------------------------------------------

// exported struct

//__MIP_EXPORT
__attribute__((visibility("default")))
struct clap_plugin_entry ENTRY_STRUCT asm("clap_plugin_entry") = {
  CLAP_VERSION,
  clap_plugin_entry_init_callback,
  clap_plugin_entry_deinit_callback,
  clap_plugin_entry_get_plugin_count_callback,
  clap_plugin_entry_get_plugin_descriptor_callback,
  clap_plugin_entry_create_plugin_callback,
  clap_plugin_entry_get_invalidation_source_count_callback,
  clap_plugin_entry_get_invalidation_source_callback,
  clap_plugin_entry_refresh_callback
};
