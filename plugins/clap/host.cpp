
// nc -U -l -k /tmp/mip.socket
//#define MIP_DEBUG_PRINT_SOCKET

#define MIP_NO_PLUGIN
#define MIP_NO_GUI
#include "mip.h"

#include <dlfcn.h>

#include "extern/clap/all.h"
#include "extern/clap/ext/draft/check-for-update.h"
#include "extern/clap/ext/draft/midi-mappings.h"
#include "x11_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// CLAP_Host

class CLAP_Host {

//------------------------------
private:
//------------------------------

  const char*                     MPluginPath     = {0};
  char                            MPathOnly[1024] = {0};
  void*                           MLibHandle      = nullptr;
  const struct clap_plugin_entry* MClapEntry      = nullptr;

//------------------------------
public:
//------------------------------

  CLAP_Host() {
  }

  //----------

  virtual ~CLAP_Host() {
  }

//------------------------------
public:
//------------------------------

  const clap_host* getClapHost() {
    return &MClapHost;
  }

  //----------

  bool loadPlugin(const char* path) {
    MPluginPath = path;
    MLibHandle = dlopen(path,RTLD_LAZY|RTLD_LOCAL); // RTLD_NOW
    if (!MLibHandle) {
      MIP_Print("couldn't load %s\n",path);
      return false;
    }
    MClapEntry = (struct clap_plugin_entry*)dlsym(MLibHandle,"clap_plugin_entry");
    if (!MClapEntry) {
      MIP_Print("couldn't find 'clap_plugin_entry'\n");
      dlclose(MLibHandle);
      MLibHandle = nullptr;
      return false;
    }
    get_path_only(MPathOnly,path);
    return MClapEntry->init(MPathOnly);
  }

  //----------

  void unoadPlugin() {
    if (MClapEntry) MClapEntry->deinit();
    if (MLibHandle) dlclose(MLibHandle);
  }

  //----------

  const clap_plugin* instantiatePlugin(const char* path, uint32_t index) {
    if (!MClapEntry) return nullptr;
    if (!path) return nullptr;
    if (index >= MClapEntry->get_plugin_count()) return nullptr;
    const clap_plugin_descriptor* descriptor = MClapEntry->get_plugin_descriptor(index);
    if (!descriptor) {
      MIP_Print("couldn't get descriptor\n");
      return nullptr;
    }
    const clap_plugin* plugin = MClapEntry->create_plugin( &MClapHost, descriptor->id );
    if (!plugin) {
      MIP_Print("couldn't create plugin\n");
      return nullptr;
    }
    bool result = plugin->init(plugin);
    if (result) {
      return plugin;
    }
    else {
      MIP_Print("couldn't init plugin\n");
      plugin->destroy(plugin);
      return nullptr;
    }
  }

  //----------

  //void destroyPlugin(const clap_plugin* plugin) {
  //  if (plugin) plugin->destroy(plugin);
  //}

//------------------------------
private:
//------------------------------

  // these are a bit hacky..

  //----------

  // /home/skei/test.so -> test.so
  // returns ptr to first character after last /

  const char* get_filename_from_path(const char* path) {
    if (path) {
      const char* slash     = strrchr(path,'/');
      const char* backslash = strrchr(path,'\\');
      if (slash) {
        return slash + 1;
      }
      else if (backslash) {
        return backslash + 1;
      }
    }
    return NULL;
  }

  //----------

  // /home/skei/test.so -> /home/skei/
  // inserts a 0 after the last /

  char* get_path_only(char* dst, const char* src) {
    if (dst && src) {
      strcpy(dst,src);
      char* slash     = strrchr(dst,'/');
      char* backslash = strrchr(dst,'\\');
      if (slash) {
        slash[1] = 0;
        return dst;
      }
      else if (backslash) {
        backslash[1] = 0;
        return dst;
      }
    }
    return NULL;
  }

//------------------------------
public: // host
//------------------------------

  const void* get_extension(const char *extension_id) {
    MIP_Print("extension id: %s\n",extension_id);
    if (strcmp(extension_id, CLAP_EXT_AUDIO_PORTS         ) == 0) return &MClapHostAudioPorts;
    if (strcmp(extension_id, CLAP_EXT_AUDIO_PORTS_CONFIG  ) == 0) return &MClapHostAudioPortsConfig;
    if (strcmp(extension_id, CLAP_EXT_CHECK_FOR_UPDATE    ) == 0) return &MClapHostChekForUpdate;
    if (strcmp(extension_id, CLAP_EXT_EVENT_FILTER        ) == 0) return &MClapHostEventFilter;
    if (strcmp(extension_id, CLAP_EXT_FD_SUPPORT          ) == 0) return &MClapHostFdSupport;
    if (strcmp(extension_id, CLAP_EXT_FILE_REFERENCE      ) == 0) return &MClapHostFileReference;
    if (strcmp(extension_id, CLAP_EXT_GUI                 ) == 0) return &MClapHostGui;
    if (strcmp(extension_id, CLAP_EXT_LATENCY             ) == 0) return &MClapHostLatency;
    if (strcmp(extension_id, CLAP_EXT_LOG                 ) == 0) return &MClapHostLog;
    if (strcmp(extension_id, CLAP_EXT_MIDI_MAPPINGS       ) == 0) return &MClapHostMidiMappings;
    if (strcmp(extension_id, CLAP_EXT_NOTE_NAME           ) == 0) return &MClapHostNoteName;
    if (strcmp(extension_id, CLAP_EXT_NOTE_PORTS          ) == 0) return &MClapHostNotePorts;
    if (strcmp(extension_id, CLAP_EXT_PARAMS              ) == 0) return &MClapHostParams;
    if (strcmp(extension_id, CLAP_EXT_QUICK_CONTROLS      ) == 0) return &MClapHostQuickControls;
    if (strcmp(extension_id, CLAP_EXT_STATE               ) == 0) return &MClapHostState;
    if (strcmp(extension_id, CLAP_EXT_THREAD_CHECK        ) == 0) return &MClapHostThreadCheck;
    if (strcmp(extension_id, CLAP_EXT_THREAD_POOL         ) == 0) return &MClapHostThreadPool;
    if (strcmp(extension_id, CLAP_EXT_TIMER_SUPPORT       ) == 0) return &MClapHostTimerSupport;
    if (strcmp(extension_id, CLAP_EXT_TRACK_INFO          ) == 0) return &MClapHostTrackInfo;
    if (strcmp(extension_id, CLAP_EXT_TUNING              ) == 0) return &MClapHostTuning;
    return nullptr;
  }

  //----------

  void request_restart() {
    MIP_PRINT;
  }

  //----------

  void request_process() {
    MIP_PRINT;
  }

  //----------

  void request_callback() {
    MIP_PRINT;
  }

//------------------------------
public: // extensions
//------------------------------

  //------------------------------
  // audio-ports
  //------------------------------

  uint32_t audio_ports_get_preferred_sample_size() {
    MIP_PRINT;
    return 32;
  }

  //----------

  void audio_ports_rescan(uint32_t flags) {
    MIP_Print("flags: %i\n",flags);
  }

  //------------------------------
  // audio-ports-config
  //------------------------------

  void audio_ports_config_rescan() {
    MIP_PRINT;
  }

  //------------------------------
  // check-for-update.draft/0
  //------------------------------

  void check_for_update_on_new_version(const clap_check_for_update_info *update_info) {
    MIP_PRINT;
  }

  //------------------------------
  // event-filter
  //------------------------------

  void event_filter_changed() {
    MIP_PRINT;
  }

  //------------------------------
  // fd-support
  //------------------------------

  bool fd_support_register_fd(clap_fd fd, clap_fd_flags flags) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool fd_support_modify_fd(clap_fd fd, clap_fd_flags flags) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool fd_support_unregister_fd(clap_fd fd) {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // file-reference.draft/0
  //------------------------------

  void file_reference_changed() {
    MIP_PRINT;
  }

  //----------

  void file_reference_set_dirty(clap_id resource_id) {
    MIP_PRINT;
  }

  //------------------------------
  // gui
  //------------------------------

  bool gui_resize(uint32_t width, uint32_t height) {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // latency
  //------------------------------

  void latency_changed() {
    MIP_PRINT;
  }

  //------------------------------
  // log
  //------------------------------

  void log_log(clap_log_severity severity, const char *msg) {
    MIP_PRINT;
  }

  //------------------------------
  // midi-mappings.draft/0
  //------------------------------

  void midi_mappings_changed() {
    MIP_PRINT;
  }

  //------------------------------
  // note-name
  //------------------------------

  void note_name_changed() {
    MIP_PRINT;
  }

  //------------------------------
  // note-ports
  //------------------------------

  void note_ports_rescan(uint32_t flags) {
    MIP_PRINT;
  }

  //------------------------------
  // params
  //------------------------------

  void params_rescan(clap_param_rescan_flags flags) {
    MIP_PRINT;
  }

  //----------

  void params_clear(clap_id param_id, clap_param_clear_flags flags) {
    MIP_PRINT;
  }

  //----------

  void params_request_flush() {
    MIP_PRINT;
  }

  //------------------------------
  // quick-controls.draft/0
  //------------------------------

  void quick_controls_changed(clap_quick_controls_changed_flags flags) {
    MIP_PRINT;
  }

  //------------------------------
  // state
  //------------------------------

  void state_mark_dirty() {
    MIP_PRINT;
  }

  //------------------------------
  // thread-check
  //------------------------------

  bool thread_check_is_main_thread() {
    MIP_PRINT;
    return false;
  }

  //----------

  bool thread_check_is_audio_thread() {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // thread-pool.draft/0
  //------------------------------

  bool thread_pool_request_exec(uint32_t num_tasks) {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // timer-support
  //------------------------------

  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) {
    MIP_PRINT;
    return false;
  }

  //----------

  bool timer_support_unregister_timer(clap_id timer_id) {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // track-info.draft/0
  //------------------------------

  bool track_info_get(clap_track_info *info) {
    MIP_PRINT;
    return false;
  }

  //------------------------------
  // tuning.draft/0
  //------------------------------

  double tuning_get(int32_t key, int32_t channel) {
    MIP_PRINT;
    return 0.0;
  }

//------------------------------
private: // callbacks
//------------------------------

  const clap_host MClapHost = {
    CLAP_VERSION,
    this,
    "CLAP_Host",
    "Tor-Helge Skei",
    "https://torhelgeskei.com",
    "0.0.1",
    clap_host_get_extension_callback,
    clap_host_request_restart_callback,
    clap_host_request_process_callback,
    clap_host_request_callback_callback
  };

  static const void* clap_host_get_extension_callback(const struct clap_host *host, const char *extension_id) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->get_extension(extension_id);
  }

  static void clap_host_request_restart_callback(const struct clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->request_restart();
  }

  static void clap_host_request_process_callback(const struct clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->request_process();
  }

  static void clap_host_request_callback_callback(const struct clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->request_callback();
  }

//------------------------------
private: // extension callbacks
//------------------------------

  // audio-ports

  clap_host_audio_ports MClapHostAudioPorts = {
    clap_host_audio_ports_get_preferred_sample_size_callback,
    clap_host_audio_ports_rescan_callback
  };

  static uint32_t clap_host_audio_ports_get_preferred_sample_size_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->audio_ports_get_preferred_sample_size();
  }

  static void clap_host_audio_ports_rescan_callback(const clap_host *host, uint32_t flags) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->audio_ports_rescan(flags);
  }

  // audio-ports-config

  clap_host_audio_ports_config MClapHostAudioPortsConfig = {
   clap_host_audio_ports_config_rescan_callback
  };

  static void clap_host_audio_ports_config_rescan_callback(const struct clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->audio_ports_config_rescan();
  }

  // check-for-update.draft/0

  clap_host_check_for_update MClapHostChekForUpdate = {
    clap_host_check_for_update_on_new_version_callback
  };

  static void clap_host_check_for_update_on_new_version_callback(const clap_host *host, const clap_check_for_update_info *update_info) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->check_for_update_on_new_version(update_info);
  }

  // event-filter

  clap_host_event_filter MClapHostEventFilter = {
    clap_host_event_filter_changed_callback
  };

  static void clap_host_event_filter_changed_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->event_filter_changed();
  }

  // fd-support

  clap_host_fd_support MClapHostFdSupport = {
    clap_host_fd_support_register_fd_callback,
    clap_host_fd_support_modify_fd_callback,
    clap_host_fd_support_unregister_fd_callback
  };

  static bool clap_host_fd_support_register_fd_callback(const clap_host *host, clap_fd fd, clap_fd_flags flags) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->fd_support_register_fd(fd,flags);
  }

  static bool clap_host_fd_support_modify_fd_callback(const clap_host *host, clap_fd fd, clap_fd_flags flags) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->fd_support_modify_fd(fd,flags);
  }

  static bool clap_host_fd_support_unregister_fd_callback(const clap_host *host, clap_fd fd) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->fd_support_unregister_fd(fd);
  }

  // file-reference.draft/0

  clap_host_file_reference MClapHostFileReference = {
    clap_host_file_reference_changed_callback,
    clap_host_file_reference_set_dirty_callback
  };

  static void clap_host_file_reference_changed_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->file_reference_changed();
  }

  static void clap_host_file_reference_set_dirty_callback(const clap_host *host, clap_id resource_id) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->file_reference_set_dirty(resource_id);
  }

  // gui

  clap_host_gui MClapHostGui = {
    clap_host_gui_resize_callback
  };

  static bool clap_host_gui_resize_callback(const clap_host *host, uint32_t width, uint32_t height) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->gui_resize(width,height);
  }

  // latency

  clap_host_latency MClapHostLatency = {
    clap_host_latency_changed_callback
  };

  static void clap_host_latency_changed_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->latency_changed();
  }

  // log

  clap_host_log MClapHostLog = {
    clap_host_log_log_callback
  };

  static void clap_host_log_log_callback(const clap_host *host, clap_log_severity severity, const char *msg) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->log_log(severity,msg);
  }

  // midi-mappings.draft/0

  clap_host_midi_mappings MClapHostMidiMappings = {
    clap_host_midi_mappings_changed_callback
  };

  static void clap_host_midi_mappings_changed_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->midi_mappings_changed();
  }

  // note-name

  clap_host_note_name MClapHostNoteName = {
    clap_host_note_name_changed_callback
  };

  static void clap_host_note_name_changed_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->note_name_changed();
  }

  // note-ports

  clap_host_note_ports MClapHostNotePorts = {
    clap_host_note_ports_rescan_callback
  };

  static void clap_host_note_ports_rescan_callback(const clap_host *host, uint32_t flags) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->note_ports_rescan(flags);
  }

  // params

  clap_host_params MClapHostParams = {
    clap_host_params_rescan_callback,
    clap_host_params_clear_callback,
    clap_host_params_request_flush_callback
  };

  static void clap_host_params_rescan_callback(const clap_host *host, clap_param_rescan_flags flags) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->params_rescan(flags);
  }

  static void clap_host_params_clear_callback(const clap_host *host, clap_id param_id, clap_param_clear_flags flags) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->params_clear(param_id,flags);
  }

  static void clap_host_params_request_flush_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->params_request_flush();
  }

  // quick-controls.draft/0

  clap_host_quick_controls MClapHostQuickControls = {
    clap_host_quick_controls_changed_callback
  };

  static void clap_host_quick_controls_changed_callback(const clap_host *host, clap_quick_controls_changed_flags flags) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->quick_controls_changed(flags);
  }

  // state

  clap_host_state MClapHostState = {
    clap_host_state_mark_dirty_callback
  };

  static void clap_host_state_mark_dirty_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    host_->state_mark_dirty();
  }

  // thread-check

  clap_host_thread_check MClapHostThreadCheck = {
    clap_host_thread_check_is_main_thread_callback,
    clap_host_thread_check_is_audio_thread_callback
  };

  static bool clap_host_thread_check_is_main_thread_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->thread_check_is_main_thread();
  }

  static bool clap_host_thread_check_is_audio_thread_callback(const clap_host *host) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->thread_check_is_audio_thread();
  }

  // thread-pool.draft/0

  clap_host_thread_pool MClapHostThreadPool = {
    clap_host_thread_pool_request_exec_callback
  };

  static bool clap_host_thread_pool_request_exec_callback(const clap_host *host, uint32_t num_tasks) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->thread_pool_request_exec(num_tasks);
  }

  // timer-support

  clap_host_timer_support MClapHostTimerSupport = {
    clap_host_timer_support_register_timer_callback,
    clap_host_timer_support_unregister_timer_callback
  };

  static bool clap_host_timer_support_register_timer_callback(const clap_host *host, uint32_t period_ms, clap_id *timer_id) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->timer_support_register_timer(period_ms,timer_id);
  }

  static bool clap_host_timer_support_unregister_timer_callback(const clap_host *host, clap_id timer_id) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->timer_support_unregister_timer(timer_id);
  }

  // track-info.draft/0

  clap_host_track_info MClapHostTrackInfo = {
    clap_host_track_info_get_callback
  };

  static bool clap_host_track_info_get_callback(const clap_host *host, clap_track_info *info) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->track_info_get(info);
  }

  // tuning.draft/0

  clap_host_tuning MClapHostTuning = {
    clap_host_tuning_get_callback
  };

  static double clap_host_tuning_get_callback(const clap_host *host, int32_t key, int32_t channel) {
    CLAP_Host* host_ = (CLAP_Host*)host->host_data;
    return host_->tuning_get(key,channel);
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main(int argc, char** argv) {
  CLAP_Host host;
  host.loadPlugin("/DISKS/sda2/code/git/MIP/bin/mip_debug.so");
  const clap_plugin* plugin = host.instantiatePlugin("./",0);
  if (plugin) {
    plugin->activate(plugin,44100,128,128);
    plugin->start_processing(plugin);
    plugin->stop_processing(plugin);
    plugin->deactivate(plugin);
    plugin->destroy(plugin);
    //host.destroyPlugin(plugin);
  }
  host.unoadPlugin();
  return 0;
}
