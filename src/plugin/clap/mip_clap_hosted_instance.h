#ifndef mip_clap_hosted_instance_included
#define mip_clap_hosted_instance_included
//----------------------------------------------------------------------

/*
  (external) plugin instance
  see also: MIP_ClapHostedInstanceHost
*/

#include "mip.h"
#include "base/mip_library.h"
#include "plugin/clap/mip_clap.h"

class MIP_ClapHostedInstance
: public MIP_Library {

//------------------------------
private:
//------------------------------

  const char*         MFilename = nullptr;;
  const clap_plugin*  MPlugin   = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ClapHostedInstance(const char* AFilename)
  : MIP_Library() {
    MFilename = AFilename;
  }

  //----------

  virtual ~MIP_ClapHostedInstance() {
  }

//------------------------------
public:
//------------------------------

  bool on_plugin_init() {
    return true;
  }

  void on_plugin_deinit() {
  }

  bool on_plugin_activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) {
    return true;
  }

  void on_plugin_deactivate() {
  }

  bool on_plugin_startProcessing() {
    return true;
  }

  void on_plugin_stopProcessing() {
  }

  //----------

  uint32_t on_plugin_process(MIP_ProcessContext* AContext) {
    return CLAP_PROCESS_CONTINUE;
  }

  void on_plugin_parameter(uint32_t AIndex, float AValue) {
  }

  void on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
  }

  void on_plugin_expression(uint32_t AKey, uint32_t AExpression, float AValue) {
  }

  //----------

  bool on_plugin_openEditor(MIP_Window* AWindow) {
    return false;
  }

  void on_plugin_closeEditor() {
  }

  void on_plugin_updateEditor() {
  }

};

//----------------------------------------------------------------------
#endif
