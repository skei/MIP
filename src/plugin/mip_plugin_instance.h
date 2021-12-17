#ifndef mip_plugin_instance_included
#define mip_plugin_instance_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_plugin_editor.h"
#include "plugin/mip_process_context.h"

//----------------------------------------------------------------------

class MIP_PluginInstance
: public MIP_EditorListener {

//------------------------------
protected:
//------------------------------

  MIP_PluginDescriptor* MDescriptor       = nullptr;
  MIP_PluginEditor*     MEditor           = nullptr;
  uint32_t              MPluginFormat     = MIP_PLUGIN_FORMAT_NONE;
  float*                MParameterValues  = nullptr;

  //void*                 MFormatSpecificInstance = nullptr;
  //void*                 MFormatSpecificHost     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_PluginInstance(MIP_PluginDescriptor* ADescriptor) {
    MDescriptor = ADescriptor;
    uint32_t num = MDescriptor->getNumParameters();
    MParameterValues = (float*)malloc(num * sizeof(float));
  }

  //----------

  virtual ~MIP_PluginInstance() {
    free(MParameterValues);
  }

//------------------------------
public:
//------------------------------

  void                  setPluginFormat(uint32_t AFormat) { MPluginFormat = AFormat; }
  MIP_PluginDescriptor* getDescriptor()                   { return MDescriptor; }

  //void  setFormatSpecificInstance(void* ptr) { MFormatSpecificInstance = ptr; }
  //void* getFormatSpecificInstance() { return MFormatSpecificInstance; }

  //void  setFormatSpecificHost(void* ptr) { MFormatSpecificHost = ptr; }
  //void* getFormatSpecificHost() { return MFormatSpecificHost; }

//------------------------------
public:
//------------------------------

  void setParameterValue(uint32_t AIndex, float AValue) {
    MParameterValues[AIndex] = AValue;
  }

  float getParameterValue(uint32_t AIndex) {
    return MParameterValues[AIndex];
  }

  void setDefaultParameterValues() {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      MIP_PluginParameter* param = MDescriptor->getParameter(i);
      float value = param->getDefaultValue();
      setParameterValue(i,value);
    }
  }

  void updateAllParameters() {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      MIP_PluginParameter* parameter = MDescriptor->getParameter(i);
      //float value = param->getDefaultValue();
      float value = parameter->getDefaultValue();
      //setParameterValue(i,value);
      on_plugin_parameter(i,value);
    }
  }

  void updateAllEditorParameters(MIP_PluginEditor* AEditor, bool ARedraw) {
    if (MEditor) {
      uint32_t num = MDescriptor->getNumParameters();
      for (uint32_t i=0; i<num; i++) {
        MIP_PluginParameter* parameter = MDescriptor->getParameter(i);
        //float value = param->getDefaultValue();
        float value = parameter->getDefaultValue();
        //setParameterValue(i,value);
        //on_plugin_parameter(i,value);
        MEditor->updateParameter(i,value,ARedraw);
      }
    }
  }

//------------------------------
public:
//------------------------------

  virtual bool      on_plugin_init() { return true; }
  virtual void      on_plugin_deinit() {}
  virtual bool      on_plugin_activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) { return false; }
  virtual void      on_plugin_deactivate() {}
  virtual bool      on_plugin_startProcessing() { return true; }
  virtual void      on_plugin_stopProcessing() {}
  virtual uint32_t  on_plugin_process(MIP_ProcessContext* AContext) { return 1; }
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
    //TODO
  }

  //----------

  void on_editor_resize(uint32_t AWidth, uint32_t AHeight) final {
    //TODO
  }

};

//----------------------------------------------------------------------
#endif
