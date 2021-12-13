#ifndef mip_plugin_list_included
#define mip_plugin_list_included
//----------------------------------------------------------------------

/*
  your plugin must implement this function..
  call MIP_PLUGIN_LIST.appendPlugin() for each of your (sub-) plugins
*/

extern void MIP_RegisterPlugins();

//----------------------------------------------------------------------

struct MIP_PluginInfo {
  MIP_PluginDescriptor* desc      = nullptr;
  void*           clap_desc = nullptr; // clap_plugin_descriptor* / ...
  // vst3, vst2, lv2
  MIP_PluginInfo(MIP_PluginDescriptor* d) {
    desc = d;
  }
};

//----------

typedef std::vector<MIP_PluginInfo*> MIP_PluginInfos;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_PluginList {

//------------------------------
private:
//------------------------------

  MIP_PluginInfos MPlugins = {};

//------------------------------
public:
//------------------------------

  MIP_PluginList() {
    MIP_RegisterPlugins();
  }

  //----------

  ~MIP_PluginList() {
    deletePlugins();
  }

public:

  void appendPlugin(MIP_PluginDescriptor* desc) {
    MIP_PluginInfo* info = new MIP_PluginInfo(desc);
    MPlugins.push_back(info);
  }

  //----------

  void deletePlugins() {
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      if (MPlugins[i]->desc) delete MPlugins[i]->desc;
      // each format should have deleted its own data
      delete MPlugins[i];
    }
    MPlugins.clear();
  }

  //----------

  uint32_t getNumPlugins() {
    return MPlugins.size();
  }

  //----------

  MIP_PluginInfo* getPluginInfo(uint32_t index) {
    return MPlugins[index];
  }

  //----------

  int32_t findPluginByIdString(const char* id) {
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      const char* pluginid = MPlugins[i]->desc->getIdString();
      if (strcmp(id, pluginid) == 0) return i;
    }
    return -1;
  };

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

MIP_PluginList MIP_PLUGIN_LIST = {};

//----------------------------------------------------------------------
#endif
