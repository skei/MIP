#ifndef mip_instance_included
#define mip_instance_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_process_context.h"

//----------------------------------------------------------------------

class MIP_Instance
: public MIP_EditorListener {

//------------------------------
protected:
//------------------------------

  MIP_Descriptor* MDescriptor   = nullptr;
  MIP_Editor*     MEditor       = nullptr;
  uint32_t        MPluginFormat = MIP_PLUGIN_FORMAT_NONE;

//------------------------------
public:
//------------------------------

  MIP_Instance(MIP_Descriptor* ADescriptor) {
    MIP_PRINT;
    MDescriptor = ADescriptor;
  }

  //----------

  virtual ~MIP_Instance() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  void            setPluginFormat(uint32_t AFormat) { MPluginFormat = AFormat; }
  MIP_Descriptor* getDescriptor()                   { return MDescriptor; }

//------------------------------
public:
//------------------------------

  void setParameterValue(uint32_t AIndex, float AValue) {
  }

  float getParameterValue(uint32_t AIndex) {
    return 0.0;
  }

  void setDefaultParameterValues() {
  }

  void updateAllParameters() {
  }

  void updateAllEditorParameters(MIP_Editor* AEditor, bool ARedraw) {
  }

//------------------------------
public:
//------------------------------

  virtual bool      on_plugin_init() { return false; }
  virtual void      on_plugin_deinit() {}
  virtual bool      on_plugin_activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) { return false; }
  virtual void      on_plugin_deactivate() {}
  virtual bool      on_plugin_startProcessing() { return false; }
  virtual void      on_plugin_stopProcessing() {}
  virtual uint32_t  on_plugin_process(MIP_ProcessContext* AContext) { return 0; }
  virtual void      on_plugin_parameter(uint32_t AIndex, float AValue) {}
  virtual void      on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}
  virtual void      on_plugin_expression(uint32_t AKey, uint32_t AExpression, float AValue) {}
  //virtual bool      on_plugin_createEditor(MIP_Window* AWindow) { return false; }
  //virtual void      on_plugin_destroyEditor() {}
  //virtual bool      on_plugin_attachEditor() { return false; }
  virtual bool      on_plugin_openEditor(MIP_Window* AWindow) { return false; }
  virtual void      on_plugin_closeEditor() {}
  virtual void      on_plugin_updateEditor() {}

//------------------------------
private: // editor listener
//------------------------------

  void on_editor_parameter(uint32_t AIndex, float AValue) final {
    MIP_PRINT;
    //TODO
  }

  //----------

  void on_editor_resize(uint32_t AWidth, uint32_t AHeight) final {
    MIP_PRINT;
    //TODO
  }

};

//----------------------------------------------------------------------
#endif
