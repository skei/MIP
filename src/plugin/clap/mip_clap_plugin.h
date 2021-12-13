#ifndef mip_clap_plugin_included
#define mip_clap_plugin_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin_host.h"
#include "plugin/clap/mip_clap_instance.h"

void print_clap_plugin_entry();

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ClapPlugin {

//------------------------------
public:
//------------------------------

  MIP_ClapPlugin() {
    MIP_ClapPrint("\n");
    print_clap_plugin_entry();
  }

  //----------

  ~MIP_ClapPlugin() {
    MIP_ClapPrint("\n");
  };

//------------------------------
public:
//------------------------------


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
    uint32_t num = MIP_PLUGIN_LIST.getNumPlugins();
    MIP_ClapPrint("plugin_path '%s' -> %s\n",plugin_path, (num > 0) ? "true" : "false" );
    for (uint32_t i=0; i<num; i++) {
      MIP_PluginInfo* info = MIP_PLUGIN_LIST.getPluginInfo(i);
      clap_plugin_descriptor* clapdescriptor = (clap_plugin_descriptor*)malloc(sizeof(clap_plugin_descriptor));
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
      info->clap_desc = clapdescriptor;
    }
    //if (num > 0) return true;
    //else return false;
    return (num > 0);
  }

  //----------

  /*
  */

  void clap_entry_deinit(void) {
    MIP_ClapPrint("\n");
    uint32_t num = MIP_PLUGIN_LIST.getNumPlugins();
    for (uint32_t i=0; i<num; i++) {
      MIP_PluginInfo* info = MIP_PLUGIN_LIST.getPluginInfo(i);
      clap_plugin_descriptor* clapdescriptor = (clap_plugin_descriptor*)info->clap_desc;
      free(clapdescriptor);
      info->clap_desc = nullptr;
    }
  }

  //----------

  /*
    Get the number of plugins available.
    [thread-safe]
  */

  uint32_t clap_entry_get_plugin_count() {
    uint32_t num = MIP_PLUGIN_LIST.getNumPlugins();
    MIP_ClapPrint("-> %i\n",num);
    return num;

  }

  //----------

  /*
    Retrieves a plugin descriptor by its index.
    Returns null in case of error.
    The descriptor does not need to be freed.
    [thread-safe]
  */

  const clap_plugin_descriptor* clap_entry_get_plugin_descriptor(uint32_t index) {
    MIP_PluginInfo* info = MIP_PLUGIN_LIST.getPluginInfo(index);
    const clap_plugin_descriptor* desc = (clap_plugin_descriptor*)info->clap_desc;
    MIP_ClapPrint("index %i -> %p\n",index,desc);
    MIP_ClapDPrint("  clap_version: %i.%i.%i\n",desc->clap_version.major,desc->clap_version.minor,desc->clap_version.revision);
    MIP_ClapDPrint("  id:           %s\n",desc->id);
    MIP_ClapDPrint("  name:         %s\n",desc->name);
    MIP_ClapDPrint("  vendor:       %s\n",desc->vendor);
    MIP_ClapDPrint("  url:          %s\n",desc->url);
    MIP_ClapDPrint("  manual_url:   %s\n",desc->manual_url);
    MIP_ClapDPrint("  support_url:  %s\n",desc->support_url);
    MIP_ClapDPrint("  version:      %s\n",desc->version);
    MIP_ClapDPrint("  description:  %s\n",desc->description);
    MIP_ClapDPrint("  keywords:     %s\n",desc->keywords);
    MIP_ClapDPrint("  plugin_type:  %i\n",desc->plugin_type);
    return desc;
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
    MIP_ClapPrint("host %p plugin_id '%s' -> ...\n",host,plugin_id);
    MIP_ClapDPrint("  name:         %s\n",host->name);
    MIP_ClapDPrint("  version:      %s\n",host->version);
    MIP_ClapDPrint("  vendor:       %s\n",host->vendor);
    MIP_ClapDPrint("  url:          %s\n",host->url);
    MIP_ClapDPrint("  clap_version: %i.%i.%i\n",host->clap_version.major,host->clap_version.minor,host->clap_version.revision);
    MIP_ClapDPrint("  host_data:    %p\n",host->host_data);

    uint32_t          index         = MIP_PLUGIN_LIST.findPluginByIdString(plugin_id);//)  0;
    MIP_PluginInfo*   info          = MIP_PLUGIN_LIST.getPluginInfo(index);
    MIP_Descriptor*   descriptor    = info->desc;
    MIP_Instance*     instance      = MIP_CreateInstance(index,descriptor);           // deleted by MIP_ClapInstance destructor
    MIP_ClapInstance* clapinstance  = new MIP_ClapInstance(index,instance/*,claphost*/);

    instance->setPluginFormat(MIP_PLUGIN_FORMAT_CLAP);

    clap_plugin_descriptor* clapdesc = (clap_plugin_descriptor*)info->clap_desc;
    clap_plugin*            clapplug = (clap_plugin*)malloc(sizeof(clap_plugin));

    clapplug->desc              = clapdesc;
    clapplug->plugin_data       = clapinstance;
    clapplug->init              = clap_instance_init_callback;
    clapplug->destroy           = clap_instance_destroy_callback;
    clapplug->activate          = clap_instance_activate_callback;
    clapplug->deactivate        = clap_instance_deactivate_callback;
    clapplug->start_processing  = clap_instance_start_processing_callback;
    clapplug->stop_processing   = clap_instance_stop_processing_callback;
    clapplug->process           = clap_instance_process_callback;
    clapplug->get_extension     = clap_instance_get_extension_callback;
    clapplug->on_main_thread    = clap_instance_on_main_thread_callback;

    MIP_ClapPrint("... -> %p\n",clapplug);

    return clapplug;
  }

  //----------

  /*
    Get the number of invalidation source.
  */

  uint32_t clap_entry_get_invalidation_source_count(void) {
    MIP_ClapPrint("-> 0\n");
    return 0;
  }

  //----------

  /*
    Get the invalidation source by its index.
    [thread-safe]
  */

  const clap_plugin_invalidation_source* clap_entry_get_invalidation_source(uint32_t index) {
    MIP_ClapPrint("index %i -> NULL\n",index);
    return nullptr;
  }

  //----------

  /*
    In case the host detected a invalidation event, it can call refresh() to
    let the plugin_entry scan the set of plugins available.
  */

  void clap_entry_refresh(void) {
    MIP_ClapPrint("\n");
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
  static uint32_t clap_entry_get_invalidation_source_count_callback(void) {                                     \
    return MIP_GLOBAL_CLAP_PLUGIN.clap_entry_get_invalidation_source_count();                                   \
  }                                                                                                             \
                                                                                                                \
  static const clap_plugin_invalidation_source* clap_entry_get_invalidation_source_callback(uint32_t index) {   \
    return MIP_GLOBAL_CLAP_PLUGIN.clap_entry_get_invalidation_source(index);                                    \
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
    clap_entry_get_invalidation_source_count_callback,                                                          \
    clap_entry_get_invalidation_source_callback,                                                                \
    clap_entry_refresh_callback                                                                                 \
  };                                                                                                            \
                                                                                                                \
  /*----------*/                                                                                                \
                                                                                                                \
  void print_clap_plugin_entry() {                                                                              \
    MIP_ClapPrint("clap_plugin_entry:\n");                                                                      \
    MIP_ClapDPrint("  clap_version:                   %i.%i.%i\n",CLAP_ENTRY_STRUCT.clap_version.major,CLAP_ENTRY_STRUCT.clap_version.minor,CLAP_ENTRY_STRUCT.clap_version.revision); \
    MIP_ClapDPrint("  init                            %p\n", CLAP_ENTRY_STRUCT.init);                           \
    MIP_ClapDPrint("  deinit                          %p\n", CLAP_ENTRY_STRUCT.deinit);                         \
    MIP_ClapDPrint("  get_plugin_count                %p\n", CLAP_ENTRY_STRUCT.get_plugin_count);               \
    MIP_ClapDPrint("  get_plugin_descriptor           %p\n", CLAP_ENTRY_STRUCT.get_plugin_descriptor);          \
    MIP_ClapDPrint("  create_plugin                   %p\n", CLAP_ENTRY_STRUCT.create_plugin);                  \
    MIP_ClapDPrint("  get_invalidation_source_count   %p\n", CLAP_ENTRY_STRUCT.get_invalidation_source_count); \
    MIP_ClapDPrint("  get_invalidation_source         %p\n", CLAP_ENTRY_STRUCT.get_invalidation_source);        \
    MIP_ClapDPrint("  refresh                         %p\n", CLAP_ENTRY_STRUCT.refresh);                        \
  };                                                                                                            \

//----------------------------------------------------------------------
#endif

