#ifndef mip_clap_preset_discovery_included
#define mip_clap_preset_discovery_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_registry.h"
#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------
//
// discovery provider
//
//----------------------------------------------------------------------

const clap_preset_discovery_provider_descriptor_t MIP_CLAP_PRESET_DISCOVERY_DESCRIPTOR {
  CLAP_VERSION,
  "skei.audio/clap_preset_discovery_provider/0.0.0",
  "skei.audio preset provider",
  "skei.audio"
};

//----------

bool clap_preset_discovery_provider_init_callback(const struct clap_preset_discovery_provider *provider) {
  MIP_PRINT;
  return false;
}

//----------

void clap_preset_discovery_provider_destroy_callback(const struct clap_preset_discovery_provider *provider) {
  MIP_PRINT;
}

//----------

bool clap_preset_discovery_provider_get_metadata_callback(const struct clap_preset_discovery_provider *provider, const char *uri, const clap_preset_discovery_metadata_receiver_t *metadata_receiver) {
  MIP_PRINT;
  //metadata_receiver->begin_preset(metadata_receiver,"preset","load_key");
  //metadata_receiver->add_plugin_id(metadata_receiver,"preset","plugin_id");
  return false;
}

//----------

const void* clap_preset_discovery_provider_get_extension_callback(const struct clap_preset_discovery_provider *provider, const char *extension_id) {
  MIP_PRINT;
  return "";
}

//----------------------------------------------------------------------

const clap_preset_discovery_provider_t MIP_CLAP_PRESET_DISCOVERY_PROVIDER {
  .desc           = &MIP_CLAP_PRESET_DISCOVERY_DESCRIPTOR,
  .provider_data  = nullptr,
  .init           = clap_preset_discovery_provider_init_callback,
  .destroy        = clap_preset_discovery_provider_destroy_callback,
  .get_metadata   = clap_preset_discovery_provider_get_metadata_callback,
  .get_extension  = clap_preset_discovery_provider_get_extension_callback
};

//----------------------------------------------------------------------
//
// discovery factory
//
//----------------------------------------------------------------------

uint32_t clap_preset_discovery_count_callback(const struct clap_preset_discovery_factory* factory) {
  MIP_PRINT;
  return 1;
}

//----------

const clap_preset_discovery_provider_descriptor_t* clap_preset_discovery_get_descriptor_callback(const struct clap_preset_discovery_factory* factory, uint32_t index) {
  MIP_PRINT;
  return &MIP_CLAP_PRESET_DISCOVERY_DESCRIPTOR;
}

//----------

const clap_preset_discovery_provider_t* clap_preset_discovery_create_callback(const struct clap_preset_discovery_factory* factory, const clap_preset_discovery_indexer_t* indexer, const char* provider_id) {
  MIP_Print("provider_id      %s\n",provider_id);
  MIP_Print("indexer.name     %s\n",indexer->name);
  MIP_Print("indexer.vendor   %s\n",indexer->vendor);
  MIP_Print("indexer.url      %s\n",indexer->url);
  MIP_Print("indexer.version  %s\n",indexer->version);
  //indexer->declare_filetype()
  //indexer->declare_location()
  //indexer->declare_soundpack()
  //indexer->get_extension()
  return &MIP_CLAP_PRESET_DISCOVERY_PROVIDER;
}

//----------------------------------------------------------------------

const clap_preset_discovery_factory_t MIP_CLAP_PRESET_DISCOVERY {
  .count          = clap_preset_discovery_count_callback,
  .get_descriptor = clap_preset_discovery_get_descriptor_callback,
  .create         = clap_preset_discovery_create_callback
};

//----------------------------------------------------------------------
#endif

