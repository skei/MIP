#ifndef mip_clap_plugin_included
#define mip_clap_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin_host.h"
#include "plugin/clap/mip_clap_instance.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapPlugin {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  MIP_ClapPlugin() {
  }

  //----------

  ~MIP_ClapPlugin() {
  };

//------------------------------
public:
//------------------------------

  /*
    This interface is the entry point of the dynamic library.

    There is an invalidation mechanism for the set of plugins which is based on
    files. The host can watch the plugin DSO's mtime and a set of files's mtime
    provided by get_clap_invalidation_source().

    The set of plugins must not change, except during a call to refresh() by
    the host.

    Every methods must be thread-safe.
  */

  //----------

  bool clap_entry_init(const char *plugin_path) {
    //MIP_RegisterPlugins()
    uint32_t num = MIP_GetNumPlugins();
    for (uint32_t i=0; i<num; i++) {
      MIP_PluginInfo* info = MIP_GetPluginInfo(i);
      clap_plugin_descriptor* clapdescriptor = (clap_plugin_descriptor*)malloc(sizeof(clap_plugin_descriptor)); // who deletes this?
      clapdescriptor->clap_version = CLAP_VERSION;
      clapdescriptor->id           = info->desc->getIdString();
      clapdescriptor->name         = info->desc->getName();
      clapdescriptor->vendor       = info->desc->getAuthor();
      clapdescriptor->url          = info->desc->getUrl();
      clapdescriptor->manual_url   = "";
      clapdescriptor->support_url  = "";
      clapdescriptor->version      = info->desc->getVersionString();
      clapdescriptor->description  = "";
      clapdescriptor->keywords     = "";
      if (info->desc->isSynth()) clapdescriptor->plugin_type = CLAP_PLUGIN_INSTRUMENT;
      else clapdescriptor->plugin_type  = CLAP_PLUGIN_AUDIO_EFFECT; //CLAP_PLUGIN_EVENT_EFFECT, CLAP_PLUGIN_ANALYZER
      info->ptr = clapdescriptor;
    }
    return true;
  }

  //----------

  /*
  */

  void clap_entry_deinit(void) {
    uint32_t num = MIP_GetNumPlugins();
    for (uint32_t i=0; i<num; i++) {
      MIP_PluginInfo* info = MIP_GetPluginInfo(i);
      free(info->ptr);
    }
  }

  //----------

  /*
    Get the number of plugins available.
    [thread-safe]
  */

  uint32_t clap_entry_get_plugin_count() {
    return MIP_GetNumPlugins();
  }

  //----------

  /*
    Retrieves a plugin descriptor by its index.
    Returns null in case of error.
    The descriptor does not need to be freed.
    [thread-safe]
  */

  const clap_plugin_descriptor* clap_entry_get_plugin_descriptor(uint32_t index) {
    MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.getPlugin(index);
    return (clap_plugin_descriptor*)info->ptr;
  }

  //----------

  /*
    Create a clap_plugin by its plugin_id.
    The returned pointer must be freed by calling plugin->destroy(plugin);
    The plugin is not allowed to use the host callbacks in the create method.
    Returns null in case of error.
    [thread-safe]
  */

  const clap_plugin* clap_entry_create_plugin(const clap_host* host, const char* plugin_id) {
    uint32_t index = MIP_GLOBAL_PLUGIN_LIST.findPluginByIdString(plugin_id);//)  0;
    //MIP_Print("*** index = %i\n",index);
    MIP_PluginInfo*         info            = MIP_GetPluginInfo(index);
    MIP_Descriptor*         descriptor      = info->desc;
    MIP_Instance*           instance        = MIP_CreateInstance(index,descriptor);           // deleted by MIP_ClapInstance destructor
    MIP_ClapPluginHost*     claphost        = new MIP_ClapPluginHost();
    MIP_ClapInstance*       clapinstance    = new MIP_ClapInstance(index,instance,claphost);
    clap_plugin*            plugin          = (clap_plugin*)malloc(sizeof(clap_plugin));

    //clap_plugin_descriptor* clapdescriptor  = (clap_plugin_descriptor*)malloc(sizeof(clap_plugin_descriptor));
    clap_plugin_descriptor* clapdescriptor  = (clap_plugin_descriptor*)info->ptr;

    instance->setPluginFormat(MIP_PLUGIN_FORMAT_CLAP);
    plugin->desc              = clapdescriptor;
    plugin->plugin_data       = clapinstance;
    plugin->init              = clap_instance_init_callback;
    plugin->destroy           = clap_instance_destroy_callback;
    plugin->activate          = clap_instance_activate_callback;
    plugin->deactivate        = clap_instance_deactivate_callback;
    plugin->start_processing  = clap_instance_start_processing_callback;
    plugin->stop_processing   = clap_instance_stop_processing_callback;
    plugin->process           = clap_instance_process_callback;
    plugin->get_extension     = clap_instance_get_extension_callback;
    plugin->on_main_thread    = clap_instance_on_main_thread_callback;
    return plugin;
  }

  //----------

  /*
    Get the number of invalidation source.
  */

  uint32_t clap_entry_get_invalidation_sources_count(void) {
    return 0;
  }

  //----------

  /*
    Get the invalidation source by its index.
    [thread-safe]
  */

  const clap_plugin_invalidation_source* clap_entry_get_invalidation_sources(uint32_t index) {
    return nullptr;
  }

  //----------

  /*
    In case the host detected a invalidation event, it can call refresh() to
    let the plugin_entry scan the set of plugins available.
  */

  void clap_entry_refresh(void) {
  }

//------------------------------
public:
//------------------------------

  // trampolines to MIP_ClapInstance

  //----------

  static
  bool clap_instance_init_callback(const struct clap_plugin *plugin) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    return instance->clap_instance_init();
  }

  static
  void clap_instance_destroy_callback(const struct clap_plugin *plugin) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    instance->clap_instance_destroy();
    delete instance;
    //free(plugin);
  }

  static
  bool clap_instance_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t minframes, uint32_t maxframes) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    return instance->clap_instance_activate(sample_rate,minframes,maxframes);
  }

  static
  void clap_instance_deactivate_callback(const struct clap_plugin *plugin) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    instance->clap_instance_deactivate();
  }

  static
  bool clap_instance_start_processing_callback(const struct clap_plugin *plugin) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    return instance->clap_instance_start_processing();
  }

  static
  void clap_instance_stop_processing_callback(const struct clap_plugin *plugin) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    instance->clap_instance_stop_processing();
  }

  static
  clap_process_status clap_instance_process_callback(const struct clap_plugin *plugin, const clap_process *process) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    return instance->clap_instance_process(process);
  }

  static
  const void *clap_instance_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    return instance->clap_instance_get_extension(id);
  }

  static
  void clap_instance_on_main_thread_callback(const struct clap_plugin *plugin) {
    MIP_ClapInstance* instance = (MIP_ClapInstance*)plugin->plugin_data;
    instance->clap_instance_on_main_thread();
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define MIP_CLAP_MAIN                                                                                           \
                                                                                                                \
  MIP_ClapPlugin MIP_GLOBAL_CLAP_PLUGIN;                                                                        \
                                                                                                                \
  /*----------*/                                                                                                \
                                                                                                                \
  static bool clap_entry_init_callback(const char *plugin_path) {                                               \
    return MIP_GLOBAL_CLAP_PLUGIN.clap_entry_init(plugin_path);                                                 \
  }                                                                                                             \
                                                                                                                \
  static void clap_entry_deinit_callback() {                                                                    \
    MIP_GLOBAL_CLAP_PLUGIN.clap_entry_deinit();                                                                 \
  }                                                                                                             \
                                                                                                                \
  static uint32_t clap_entry_get_plugin_count_callback() {                                                      \
    return MIP_GLOBAL_CLAP_PLUGIN.clap_entry_get_plugin_count();                                                \
  }                                                                                                             \
                                                                                                                \
  static const clap_plugin_descriptor* clap_entry_get_plugin_descriptor_callback(uint32_t index) {              \
    return MIP_GLOBAL_CLAP_PLUGIN.clap_entry_get_plugin_descriptor(index);                                      \
  }                                                                                                             \
                                                                                                                \
  static const clap_plugin* clap_entry_create_plugin_callback(const clap_host* host, const char* plugin_id) {   \
    return MIP_GLOBAL_CLAP_PLUGIN.clap_entry_create_plugin(host,plugin_id);                                     \
  }                                                                                                             \
                                                                                                                \
  static uint32_t clap_entry_get_invalidation_sources_count_callback(void) {                                    \
    return MIP_GLOBAL_CLAP_PLUGIN.clap_entry_get_invalidation_sources_count();                                  \
  }                                                                                                             \
                                                                                                                \
  static const clap_plugin_invalidation_source* clap_entry_get_invalidation_sources_callback(uint32_t index) {  \
    return MIP_GLOBAL_CLAP_PLUGIN.clap_entry_get_invalidation_sources(index);                                   \
  }                                                                                                             \
                                                                                                                \
  static void clap_entry_refresh_callback(void) {                                                               \
    MIP_GLOBAL_CLAP_PLUGIN.clap_entry_refresh();                                                                \
  }                                                                                                             \
                                                                                                                \
  /*----------*/                                                                                                \
                                                                                                                \
  /*__attribute__((visibility("default")))*/                                                                    \
  __MIP_EXPORT                                                                                                  \
  struct clap_plugin_entry CLAP_ENTRY_STRUCT asm("clap_plugin_entry") = {                                       \
    CLAP_VERSION,                                                                                               \
    clap_entry_init_callback,                                                                                   \
    clap_entry_deinit_callback,                                                                                 \
    clap_entry_get_plugin_count_callback,                                                                       \
    clap_entry_get_plugin_descriptor_callback,                                                                  \
    clap_entry_create_plugin_callback,                                                                          \
    clap_entry_get_invalidation_sources_count_callback,                                                         \
    clap_entry_get_invalidation_sources_callback,                                                               \
    clap_entry_refresh_callback                                                                                 \
  };                                                                                                            \

//----------------------------------------------------------------------
#endif

