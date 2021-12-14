#ifndef mip_plugin_descriptor_included
#define mip_plugin_descriptor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin_parameter.h"
#include "base/mip_rect.h"
#include "base/mip_utils.h"

//----------------------------------------------------------------------

struct MIP_PluginPort {
  const char* name      = "";
  uint32_t    type      = MIP_PLUGIN_PORT_AUDIO;
  uint32_t    channels  = 2;
  uint32_t    direction = MIP_PLUGIN_PORT_OUTPUT;
  MIP_PluginPort(const char* AName="", uint32_t AType=MIP_PLUGIN_PORT_AUDIO, uint32_t AChannels=2, uint32_t ADirection=MIP_PLUGIN_PORT_OUTPUT) {
    name      = AName;
    type      = AType;
    channels  = AChannels;
    direction = ADirection;
  }
};

typedef std::vector<MIP_PluginPort*> MIP_PluginPorts;

//----------------------------------------------------------------------

class MIP_PluginDescriptor {

//------------------------------
private:
//------------------------------

  uint8_t   MLongId[16]         = {0};
  uint8_t   MLongEditorId[16]   = {0};
  char      MVersionString[32]  = {0};
  char      MIdString[256]      = {0};

//------------------------------
protected:
//------------------------------

  const char*           MName             = "";
  const char*           MAuthor           = "";
  uint32_t              MVersion          = 0;
  const char*           MVersionText      = "";
  const char*           MDescription      = "";
  const char*           MLicense          = "";
  const char*           MUrl              = "";
  const char*           MManualUrl        = "";
  const char*           MSupportUrl       = "";
  MIP_PluginParameters  MParameters       = {};
  MIP_PluginPorts       MInputPorts       = {};
  MIP_PluginPorts       MOutputPorts      = {};
  bool                  MIsSynth          = false;
  bool                  MHasEditor        = false;
  bool                  MCanResizeEditor  = false;
  MIP_FRect             MEditorRect       = {};

//------------------------------
public:
//------------------------------

  MIP_PluginDescriptor() {
  }

  //----------

  virtual ~MIP_PluginDescriptor() {
    deleteParameters();
  }

//------------------------------
public:
//------------------------------

  const char*           getName()                 { return MName; }
  const char*           getAuthor()               { return MAuthor; }
  uint32_t              getVersion()              { return MVersion; }
  const char*           getVersionText()          { return MVersionText; }
  const char*           getDescription()          { return MDescription; }
  const char*           getLicense()              { return MLicense; }
  const char*           getUrl()                  { return MUrl; }
  const char*           getManualUrl()            { return MManualUrl; }
  const char*           getSupportUrl()           { return MSupportUrl; }

  uint32_t              getNumInputPorts()        { return MInputPorts.size(); }
  MIP_PluginPort*       getInputPort(uint32_t i)  { return MInputPorts[i]; }
  uint32_t              getNumOutputPorts()       { return MOutputPorts.size(); }
  MIP_PluginPort*       getOutputPort(uint32_t i) { return MOutputPorts[i]; }
  uint32_t              getNumParameters()        { return MParameters.size(); }
  MIP_PluginParameter*  getParameter(uint32_t i)  { return MParameters[i]; }

  MIP_FRect             getEditorRect()           { return MEditorRect; }

  bool                  hasEditor()               { return MHasEditor; }
  bool                  canResizeEditor()         { return MCanResizeEditor; }
  bool                  isSynth()                 { return MIsSynth; }
//bool                  canSendMidi()             { return MCanSendMidi; }
//bool                  canReceiveMidi()          { return MCanReceiveMidi; }

  //----------

  void setEditorSize(int32_t AWidth, int32_t AHeight) {
    MEditorRect.w = AWidth;
    MEditorRect.h = AHeight;
  }

//------------------------------
public:
//------------------------------

  uint8_t* getLongId() {
    uint32_t* ptr = (uint32_t*)MLongId;
    ptr[0] = MIP_MAGIC_K_PL;
    ptr[1] = MIP_HashString(MName);
    ptr[2] = MIP_HashString(MAuthor);
    ptr[3] = MVersion;
    return MLongId;
  }

  //----------

  uint8_t* getLongEditorId() {
    uint32_t* ptr = (uint32_t*)MLongEditorId;
    ptr[0] = MIP_MAGIC_K_ED;
    ptr[1] = MIP_HashString(MName);
    ptr[2] = MIP_HashString(MAuthor);
    ptr[3] = MVersion;
    return MLongEditorId;
  }

  //----------

  // 0x03030001 -> "3.3.1"
  // buffer = 32 bytes, maxlen = 31
  // '255.255.65535' + 0 = 14 chars..

  //char* getVersionString(char* buffer) {
  char* getVersionString() {
    char* buffer = MVersionString;
    snprintf( buffer,
              31, // safety...
              "%i.%i.%i",
              ((MVersion & 0xff000000) >> 24),
              ((MVersion & 0x00ff0000) >> 16),
               (MVersion & 0x0000ffff)
    );
    return buffer;
  }

  //----------

  /*
    warning: ‘char* __builtin___strcat_chk(char*, const char*, long unsigned int)’
    accessing 33 or more bytes at offsets 72 and 40 may overlap 1 byte at offset 72
    [-Wrestrict]|
  */

  //char* getIdString(char* buffer) {
  char* getIdString() {
    char* buffer = MIdString;
    char* ver    = getVersionString();
    //strcpy(buffer,name);
    //strcat(buffer,"/");
    //strcat(buffer,author);
    //strcat(buffer,"/");
    //strcat(buffer,ver);
    snprintf( buffer,
              255, // safety...
              "%s/%s/%s",MName,MAuthor,ver
    );
    return buffer;
  }

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

  void appendInputPort(MIP_PluginPort* APort) {
    MInputPorts.push_back(APort);
  }

  void appendInputPort(const char* AName="", uint32_t AChannels=2) {
    MIP_PluginPort* port = new MIP_PluginPort(AName,MIP_PLUGIN_PORT_AUDIO,AChannels,MIP_PLUGIN_PORT_INPUT);
    MInputPorts.push_back(port);
  }

  void deleteInputPorts() {
    for (uint32_t i=0; i<MInputPorts.size(); i++) {
      delete MInputPorts[i];
      MInputPorts[i] = nullptr;
    }
  }

  //----------

  void appendOutputPort(MIP_PluginPort* APort) {
    MOutputPorts.push_back(APort);
  }

  void appendOutputPort(const char* AName, uint32_t AChannels=2) {
    MIP_PluginPort* port = new MIP_PluginPort(AName,MIP_PLUGIN_PORT_AUDIO,AChannels,MIP_PLUGIN_PORT_OUTPUT);
    MOutputPorts.push_back(port);
  }

  void deleteOutputPorts() {
    for (uint32_t i=0; i<MOutputPorts.size(); i++) {
      delete MOutputPorts[i];
      MOutputPorts[i] = nullptr;
    }
  }

  //----------

  void appendParameter(MIP_PluginParameter* AParameter) {
    uint32_t index = MParameters.size();
    AParameter->MIndex = index;
    MParameters.push_back(AParameter);
  }

  void appendParameter(const char* AName="", float ADefValue=0.0, float AMinValue=0.0, float AMaxValue=1.0, uint32_t ANumSteps=0) {
    MIP_PluginParameter* param = new MIP_PluginParameter(AName,ADefValue,AMinValue,AMaxValue,ANumSteps);
    appendParameter(param);
  }

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      if (MParameters[i]) delete MParameters[i];
      MParameters[i] = nullptr;
    }
    MParameters.clear();
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
