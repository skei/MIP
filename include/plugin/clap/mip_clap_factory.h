#ifndef mip_clap_factory_included
#define mip_clap_factory_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_registry.h"
#include "plugin/mip_plugin.h"

extern MIP_Plugin* MIP_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) __MIP_WEAK;

//----------------------------------------------------------------------

uint32_t clap_plugin_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  //MIP_Print("\n");
  int32_t num = MIP_REGISTRY.getNumDescriptors();
  LOG.print("CLAP FACTORY: get_plugin_count\n");
  return num;
}

//----------

const clap_plugin_descriptor_t* clap_plugin_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  //MIP_Print("index: %i\n",index);
  LOG.print("CLAP_FACTORY: get_plugin_descriptor (%i)\n",index);
  return MIP_REGISTRY.getDescriptor(index);
}

//----------

const clap_plugin_t* clap_plugin_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  //MIP_Print("plugin_id: %s\n",plugin_id);
  LOG.print("CLAP_FACTORY: create_plugin (%s)\n",plugin_id);
  if (MIP_CreatePlugin) {
    int32_t index = MIP_REGISTRY.findDescriptorById(plugin_id);
    //LOG.print("  - index %i\n",index);
    if (index >= 0) {
      const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
      MIP_Plugin* plugin = MIP_CreatePlugin(index,descriptor,host);
      return plugin->getClapPlugin();
    }
  }
  return nullptr;
}

//----------------------------------------------------------------------

const clap_plugin_factory_t MIP_CLAP_FACTORY {
  .get_plugin_count      = clap_plugin_factory_get_plugin_count_callback,
  .get_plugin_descriptor = clap_plugin_factory_get_plugin_descriptor_callback,
  .create_plugin         = clap_plugin_factory_create_plugin_callback
};

//----------------------------------------------------------------------
#endif
