#ifndef mip_plugin_list_included
#define mip_plugin_list_included
//----------------------------------------------------------------------

extern void MIP_RegisterPlugins();

//----------------------------------------------------------------------

struct MIP_PluginInfo {
  MIP_Descriptor* desc      = nullptr;
  void*           clap_desc = nullptr; // clap_plugin_descriptor* / ...
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

  void appendPlugin(MIP_Descriptor* desc) {
    MIP_PluginInfo* info = (MIP_PluginInfo*)malloc(sizeof(MIP_PluginInfo));
    memset((void*)info,0,sizeof(MIP_PluginInfo));
    info->desc = desc;
    MPlugins.push_back(info);
  }

  //----------

  void deletePlugins() {
    for (uint32_t i=0; i<MPlugins.size(); i++) {
      if (MPlugins[i]) {
        if (MPlugins[i]->desc) delete MPlugins[i]->desc;
        free(MPlugins[i]);
      }
      MPlugins[i] = nullptr;
    }
    MPlugins.clear();
  }

  //----------

  uint32_t getNumPlugins() {
    return MPlugins.size();
    MIP_PRINT;
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
