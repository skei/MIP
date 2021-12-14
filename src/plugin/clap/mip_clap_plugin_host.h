#ifndef mip_clap_plugin_host_cinluded
#define mip_clap_plugin_host_cinluded
//----------------------------------------------------------------------

#include "mip.h"
#include "extern/clap/all.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------

class MIP_ClapPluginHost {

//------------------------------
private:
//------------------------------

  const clap_host*  MClapHost     = nullptr;
  void*             MHostData     = nullptr;
  const char*       MHostName     = "";
  const char*       MHostVendor   = "";
  const char*       MHostUrl      = "";
  const char*       MHostVersion  = "";

  // extensions
  clap_host_audio_ports_config* MHostAudioPortsConfig = nullptr;
  clap_host_audio_ports*        MHostAudioPorts       = nullptr;
  clap_host_event_filter*       MHostEventFilter      = nullptr;
  clap_host_fd_support*         MHostFdSupport        = nullptr;
  clap_host_gui*                MHostGui              = nullptr;
  clap_host_latency*            MHostLatency          = nullptr;
  clap_host_log*                MHostLog              = nullptr;
  clap_host_note_name*          MHostNoteName         = nullptr;
  clap_host_params*             MHostParams           = nullptr;
  clap_host_state*              MHostState            = nullptr;
  clap_host_thread_check*       MHostThreadCheck      = nullptr;
  clap_host_timer_support*      MHostTimerSupport     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapPluginHost(const clap_host* AHost) {
    //MIP_ClapPrint("AHost %p\n",AHost);

    MClapHost     = AHost;
    MHostData     = AHost->host_data;
    MHostName     = AHost->name;
    MHostVendor   = AHost->vendor;
    MHostUrl      = AHost->url;
    MHostVersion  = AHost->version;

    //MIP_ClapPrint("- name: %s\n",MHostName);
    //MIP_ClapPrint("- vendor: %s\n",MHostVendor);
    //MIP_ClapPrint("- url: %s\n",MHostUrl);
    //MIP_ClapPrint("- version: %s\n",MHostVersion);
    //MIP_ClapPrint("- host_data: %p\n",MHostData);

    init_extensions();
  }

//------------------------------
public:
//------------------------------

  void init_extensions() {
    MHostAudioPortsConfig = (clap_host_audio_ports_config*)get_extension(CLAP_EXT_AUDIO_PORTS_CONFIG);
    MHostAudioPorts       = (clap_host_audio_ports*)get_extension(CLAP_EXT_AUDIO_PORTS);
    MHostEventFilter      = (clap_host_event_filter*)get_extension(CLAP_EXT_EVENT_FILTER);
    MHostFdSupport        = (clap_host_fd_support*)get_extension(CLAP_EXT_FD_SUPPORT);
    MHostGui              = (clap_host_gui*)get_extension(CLAP_EXT_GUI);
    MHostLatency          = (clap_host_latency*)get_extension(CLAP_EXT_LATENCY);
    MHostLog              = (clap_host_log*)get_extension(CLAP_EXT_LOG);
    MHostNoteName         = (clap_host_note_name*)get_extension(CLAP_EXT_NOTE_NAME);
    MHostParams           = (clap_host_params*)get_extension(CLAP_EXT_PARAMS);
    MHostState            = (clap_host_state*)get_extension(CLAP_EXT_STATE);
    MHostThreadCheck      = (clap_host_thread_check*)get_extension(CLAP_EXT_THREAD_CHECK);
    MHostTimerSupport     = (clap_host_timer_support*)get_extension(CLAP_EXT_TIMER_SUPPORT);
  }

//------------------------------
public:
//------------------------------

  /*
    Query an extension.
    [thread-safe]
  */

  const void* get_extension(const char *extension_id) {
    const void* ptr = nullptr;
    if (MClapHost) {
      ptr = MClapHost->get_extension(MClapHost,extension_id);
      MIP_ClapPrint("extension_id %s -> %p\n",extension_id, ptr);
    }
    return ptr;
  }

  //----------

  /*
    Request the host to deactivate and then reactivate the plugin.
    The operation may be delayed by the host.
    [thread-safe]
  */

  void request_restart() {
    if (MClapHost) {
      //MIP_ClapPrint("\n");
      MClapHost->request_restart(MClapHost);
    }
  }

  //----------

  /*
    Request the host to activate and start processing the plugin.
    This is useful if you have external IO and need to wake up the plugin from
    "sleep".
    [thread-safe]
  */

  void request_process() {
    if (MClapHost) {
      //MIP_ClapPrint("\n");
      MClapHost->request_process(MClapHost);
    }
  }

  //----------

  /*
    Request the host to schedule a call to plugin->on_main_thread(plugin) on
    the main thread.
    [thread-safe]
  */

  void request_callback() {
    if (MClapHost) {
      //MIP_ClapPrint("\n");
      MClapHost->request_callback(MClapHost);
    }
  }

//------------------------------
public: // extensions
//------------------------------

  //------------------------------
  // audio ports config
  //------------------------------

  /*
    This extension provides a way for the plugin to describe possible ports
    configurations, for example mono, stereo, surround, ...
    and a way for the host to select a configuration.
    ...
    Plugin with very complex configuration possibilities should let the user
    configure the ports from the plugin GUI, and call
    clap_host_audio_ports.rescan(CLAP_AUDIO_PORTS_RESCAN_ALL).
  */

  //----------

  /*
    Rescan the full list of configs.
    [main-thread]
  */

  void audio_ports_config_rescan() {
    if (MHostAudioPortsConfig) {
      //MIP_ClapPrint("\n");
      MHostAudioPortsConfig->rescan(MClapHost);
    }
  }

  //------------------------------
  // audio ports
  //------------------------------

  /*
    This extension provides a way for the plugin to describe its current audio
    ports.
    The plugin is only allowed to change its ports configuration while it is
    deactivated.
  */

  //----------

  /*
    [main-thread]
  */

  uint32_t audio_ports_get_preferred_sample_size() {
    if (MHostAudioPorts) {
      uint32_t i = MHostAudioPorts->get_preferred_sample_size(MClapHost);
      //MIP_ClapPrint("-> %i\n",i);
      return i;
    }
    return 0;
  }

  //----------

  /*
    Rescan the full list of audio ports according to the flags.
    [main-thread]

    CLAP_AUDIO_PORTS_RESCAN_ALL
    CLAP_AUDIO_PORTS_RESCAN_NAMES

   The ports have changed, the host shall perform a full scan of the ports.
   This flag can only be used if the plugin is not active.
   If the plugin active, call host->request_restart() and then call rescan()
   when the host calls deactivate()
  */

  void audio_ports_rescan(uint32_t flags) {
    if (MHostAudioPorts) {
      //MIP_ClapPrint("flags %i\n",flags);
      MHostAudioPorts->rescan(MClapHost,flags);
    }
  }

  //------------------------------
  // event filter
  //------------------------------

  /*
    This extension lets the host know which event types the plugin is
    interested in.
  */

  //----------

  /*
    [main-thread]
  */

  void event_filter_changed() {
    if (MHostEventFilter) {
      //MIP_ClapPrint("\n");
      MHostEventFilter->changed(MClapHost);
    }
  }

  //------------------------------
  // fd support
  //------------------------------

  /*
    [main-thread]

    CLAP_FD_READ
    CLAP_FD_WRITE
    CLAP_FD_ERROR
  */

  bool fd_support_register_fd(clap_fd fd, clap_fd_flags flags) {
    bool result = false;
    if (MHostFdSupport) {
      result = MHostFdSupport->register_fd(MClapHost,fd,flags);
      //MIP_ClapPrint("fd %i flags %i -> %s\n",fd,flags, result ? "true" : "false" );
    }
    return result;
  }

  //----------

  /*
    [main-thread]
  */

  bool fd_support_modify_fd(clap_fd fd, clap_fd_flags flags) {
    bool result = false;
    if (MHostFdSupport) {
      result = MHostFdSupport->modify_fd(MClapHost,fd,flags);
      //MIP_ClapPrint("fd %i flags %i -> %s\n",fd,flags, result ? "true" : "false" );
    }
    return result;
  }

  //----------

  /*
    [main-thread]
  */

  bool fd_support_unregister_fd(clap_fd fd) {
    bool result = false;
    if (MHostFdSupport) {
      result = MHostFdSupport->unregister_fd(MClapHost,fd);
      //MIP_ClapPrint("fd %i -> %s\n",fd, result ? "true" : "false" );
    }
    return result;
  }

  //------------------------------
  // gui
  //------------------------------

  /*
    Request the host to resize the client area to width, height.
    Return true on success, false otherwise.
     [thread-safe]
  */

  bool gui_resize(uint32_t width, uint32_t height) {
    bool result = false;
    if (MHostGui) {
      result = MHostGui->resize(MClapHost,width,height);
      //MIP_ClapPrint("width %i height %i -> %s\n",width,height, result ? "true" : "false" );
    }
    return result;
  }

  //------------------------------
  // latency
  //------------------------------

  /*
    Tell the host that the latency changed.
    The latency is only allowed to change if the plugin is deactivated.
    If the plugin is activated, call host->request_restart()
    [main-thread]
  */

  void latency_changed() {
    if (MHostLatency) {
      //MIP_ClapPrint("\n");
      MHostLatency->changed(MClapHost);
    }
  }

  //------------------------------
  // log
  //------------------------------

  /*
    Log a message through the host.
    [thread-safe]

    CLAP_LOG_DEBUG
    CLAP_LOG_INFO
    CLAP_LOG_WARNING
    CLAP_LOG_ERROR
    CLAP_LOG_FATAL

  */

  void log(clap_log_severity severity, const char *msg) {
    if (MHostLog) {
      //MIP_ClapPrint("severity %i msg %s\n",severity,msg);
      MHostLog->log(MClapHost,severity,msg);
    }
  }

  //------------------------------
  // note name
  //------------------------------

  /*
    Informs the host that the note names has changed.
    [main-thread]
  */

  void note_name_changed() {
    if (MHostNoteName) {
      //MIP_ClapPrint("\n");
      MHostNoteName->changed(MClapHost);
    }
  }

  //------------------------------
  // params
  //------------------------------

  /*
    Rescan the full list of parameters according to the flags.
    [main-thread]

    CLAP_PARAM_RESCAN_VALUES
    CLAP_PARAM_RESCAN_TEXT
    CLAP_PARAM_RESCAN_INFO
    CLAP_PARAM_RESCAN_ALL
  */

  void params_rescan(clap_param_rescan_flags flags) {
    if (MHostParams) {
      //MIP_ClapPrint("flags %i\n",flags);
      MHostParams->rescan(MClapHost,flags);
    }
  }

  //----------

  /*
    Clears references to a parameter
    [main-thread]

    CLAP_PARAM_CLEAR_ALL
    CLAP_PARAM_CLEAR_AUTOMATIONS
    CLAP_PARAM_CLEAR_MODULATIONS
  */

  void params_clear(clap_id param_id, clap_param_clear_flags flags) {
    if (MHostParams) {
      //MIP_ClapPrint("param_id %i flags %i\n",param_id,flags);
      MHostParams->clear(MClapHost,param_id,flags);
    }
  }

  //----------

  /*
    Request the host to call clap_plugin_params->flush().
    This is useful if the plugin has parameters value changes to report to the
    host but the plugin is not processing.
    eg. the plugin has a USB socket to some hardware controllers and receives a
    parameter change while it is not processing.
    This must not be called on the [audio-thread].
    [thread-safe]
  */

  void params_request_flush() {
    if (MHostParams) {
      //MIP_ClapPrint("\n");
      MHostParams->request_flush(MClapHost);
    }
  }

  //------------------------------
  // state
  //------------------------------

  /*
    Tell the host that the plugin state has changed and should be saved again.
    If a parameter value changes, then it is implicit that the state is dirty.
    [main-thread]
  */

  void state_mark_dirty() {
    if (MHostState) {
      //MIP_ClapPrint("\n");
      MHostState->mark_dirty(MClapHost);
    }
  }

  //------------------------------
  // thread-check
  //------------------------------

  /*
    This interface is useful to do runtime checks and make
    sure that the functions are called on the correct threads.
    It is highly recommended to implement this extension
  */

  //----------

  /*
    Returns true if "this" thread is the main thread.
    [thread-safe]
  */

  bool thread_check_is_main_thread() {
    bool result = false;
    if (MHostThreadCheck) {
      result = MHostThreadCheck->is_main_thread(MClapHost);
      //MIP_ClapPrint("-> %s\n", result ? "true" : "false" );
    }
    return result;
  }

  //----------

  /*
    Returns true if "this" thread is one of the audio threads.
    [thread-safe]
  */

  bool thread_check_is_audio_thread() {
    bool result = false;
    if (MHostThreadCheck) {
      result = MHostThreadCheck->is_audio_thread(MClapHost);
      //MIP_ClapPrint("-> %s\n", result ? "true" : "false" );
    }
    return result;
  }

  //------------------------------
  // timer-support
  //------------------------------

  /*
    Registers a periodic timer.
    The host may adjust the period if it is under a certain threshold.
    30 Hz should be allowed.
    [main-thread]
  */

  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) {
    bool result = false;
    if (MHostTimerSupport) {
      result = MHostTimerSupport->register_timer(MClapHost,period_ms,timer_id);
      *timer_id = 0;
      //MIP_Print("period_ms %i -> %s (*timer_id %i\n",period_ms, result ? "true" : "false", *timer_id );
    }
    return result;
  }

  //----------

  /*
    [main-thread]
  */

  bool timer_support_unregister_timer(clap_id timer_id) {
    bool result = false;
    if (MHostTimerSupport) {
      result = MHostTimerSupport->unregister_timer(MClapHost,timer_id);
      //MIP_Print("timer_id %i -> %s \n",timer_id, result ? "true" : "false" );
    }
    return result;
  }

};

//----------------------------------------------------------------------
#endif
