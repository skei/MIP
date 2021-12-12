#ifndef mip_descriptor_included
#define mip_descriptor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_parameter.h"
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

class MIP_Descriptor {

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

  const char*     MName             = "";
  const char*     MAuthor           = "";
  uint32_t        MVersion          = 0;
  const char*     MVersionText      = "";
  const char*     MUrl              = "";
  const char*     MManualUrl        = "";
  const char*     MSupportUrl       = "";
  MIP_Parameters  MParameters       = {};
  MIP_PluginPorts MInputs           = {};
  MIP_PluginPorts MOutputs          = {};
  bool            MIsSynth          = false;
  bool            MHasEditor        = false;
  bool            MCanResizeEditor  = false;
  MIP_FRect       MEditorRect       = {};

//------------------------------
public:
//------------------------------

  MIP_Descriptor() {
    MIP_PRINT;
  }

  //----------

  virtual ~MIP_Descriptor() {
    MIP_PRINT;
    deleteParameters();
  }

//------------------------------
public:
//------------------------------

  const char*     getName()                 { return MName; }
  const char*     getAuthor()               { return MAuthor; }
  uint32_t        getVersion()              { return MVersion; }
  const char*     getVersionText()          { return MVersionText; }
  const char*     getUrl()                  { return MUrl; }
  const char*     getManualUrl()            { return MManualUrl; }
  const char*     getSuportUrl()            { return MSupportUrl; }

  uint32_t        getNumInputs()            { return MInputs.size(); }
  MIP_PluginPort* getInput(uint32_t i)      { return MInputs[i]; }
  uint32_t        getNumOutputs()           { return MOutputs.size(); }
  MIP_PluginPort* getOutput(uint32_t i)     { return MOutputs[i]; }
  uint32_t        getNumParameters()        { return MParameters.size(); }
  MIP_Parameter*  getParameter(uint32_t i)  { return MParameters[i]; }

  MIP_FRect       getEditorRect()           { return MEditorRect; }

  bool            hasEditor()               { return MHasEditor; }
  bool            canResizeEditor()         { return MCanResizeEditor; }
  bool            isSynth()                 { return MIsSynth; }
//bool            canSendMidi()             { return MCanSendMidi; }
//bool            canReceiveMidi()          { return MCanReceiveMidi; }

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

  void appendInput(MIP_PluginPort* APort) {
    MInputs.push_back(APort);
  }

  void appendInput(const char* AName) {
    MIP_PluginPort* port = new MIP_PluginPort(AName,MIP_PLUGIN_PORT_AUDIO,1,MIP_PLUGIN_PORT_INPUT);
    MInputs.push_back(port);
  }

  void deleteInputs() {
    for (uint32_t i=0; i<MInputs.size(); i++) {
      delete MInputs[i];
      MInputs[i] = nullptr;
    }
  }

  //----------

  void appendOutput(MIP_PluginPort* APort) {
    MOutputs.push_back(APort);
  }

  void appendOutput(const char* AName) {
    MIP_PluginPort* port = new MIP_PluginPort(AName,MIP_PLUGIN_PORT_AUDIO,1,MIP_PLUGIN_PORT_OUTPUT);
    MOutputs.push_back(port);
  }

  void deleteOutputs() {
    for (uint32_t i=0; i<MOutputs.size(); i++) {
      delete MOutputs[i];
      MOutputs[i] = nullptr;
    }
  }

  //----------

  void appendParameter(MIP_Parameter* AParameter) {
    uint32_t index = MParameters.size();
    AParameter->MIndex = index;
    MParameters.push_back(AParameter);
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
