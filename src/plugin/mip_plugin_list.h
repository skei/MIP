#ifndef mip_plugin_list_included
#define mip_plugin_list_included
//----------------------------------------------------------------------

extern void MIP_RegisterPlugins();

//----------------------------------------------------------------------

struct MIP_PluginInfo {
  uint32_t        index = 0;
  MIP_Descriptor* desc  = nullptr;
  void*           ptr   = nullptr;
};

typedef std::vector<MIP_PluginInfo*> MIP_PluginInfos;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//template <class DESC, class INST, class EDIT>
class MIP_PluginList {

//------------------------------
private:
//------------------------------

  MIP_PluginInfos MPlugins = {};

//------------------------------
public:
//------------------------------

  MIP_PluginList() {
    //MPlugins.clear();
    MIP_RegisterPlugins();
  }

  //----------

  ~MIP_PluginList() {
    deletePlugins();
  }

public:

  uint32_t appendPlugin(MIP_Descriptor* desc) {
    uint32_t index = MPlugins.size();
    MIP_PluginInfo* info = (MIP_PluginInfo*)malloc(sizeof(MIP_PluginInfo));
    memset((void*)info,0,sizeof(MIP_PluginInfo));
    info->index = index;
    info->desc = desc;
    MPlugins.push_back(info);
    return index;
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

  MIP_PluginInfo* getPlugin(uint32_t index) {
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

MIP_PluginList MIP_GLOBAL_PLUGIN_LIST = {};

//------------------------------

void MIP_RegisterPlugin(MIP_Descriptor* desc) {
  MIP_GLOBAL_PLUGIN_LIST.appendPlugin(desc);
}

//----------

uint32_t MIP_GetNumPlugins() {
  return MIP_GLOBAL_PLUGIN_LIST.getNumPlugins();
}

//----------

MIP_PluginInfo* MIP_GetPluginInfo(uint32_t i) {
  return MIP_GLOBAL_PLUGIN_LIST.getPlugin(i);
}

//----------------------------------------------------------------------
#endif
