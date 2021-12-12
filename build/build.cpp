

#include "mip.h"


//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public MIP_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {
    MIP_PRINT;
    MName = "myPlugin";
    MAuthor = "me";
    MVersion = 0x00000001;
    MHasEditor = true;
    setEditorSize(640,480);
    appendInput("input1");
    appendInput("input2");
    appendOutput("input1");
    appendOutput("input2");
    appendParameter( new MIP_Parameter("param1"));
  }

  //----------

  virtual ~myDescriptor() {
    MIP_PRINT;
  }

};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public MIP_Editor {

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, MIP_Descriptor* ADescriptor)
  : MIP_Editor(AListener,ADescriptor) {
    MIP_PRINT;
  }

  virtual ~myEditor() {
    MIP_PRINT;
  }

};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public MIP_Instance {

//------------------------------
private:
//------------------------------

  myEditor* MEditor = nullptr;

//------------------------------
public:
//------------------------------

  myInstance(MIP_Descriptor* ADescriptor)
  : MIP_Instance(ADescriptor) {
    MIP_PRINT;
  }

  virtual ~myInstance() {
    MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  bool on_plugin_init() final {
    MIP_PRINT;
    return true;
  }

  void on_plugin_deinit() final {
    MIP_PRINT;
  }

  bool on_plugin_activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) final {
    MIP_PRINT;
    return true;
  }

  void on_plugin_deactivate() final {
    MIP_PRINT;
  }

  bool on_plugin_startProcessing() final {
    MIP_PRINT;
    return true;
  }

  void on_plugin_stopProcessing() final {
    MIP_PRINT;
  }

  //----------

  uint32_t on_plugin_process(MIP_ProcessContext* AContext) final {
    //MIP_PRINT;
    return 0;
  }

  void on_plugin_parameter(uint32_t AIndex, float AValue) final {
    MIP_PRINT;
  }

  void on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
    MIP_PRINT;
  }

  void on_plugin_expression(uint32_t AKey, uint32_t AExpression, float AValue) final {
    MIP_PRINT;
  }

  bool on_plugin_openEditor(MIP_Window* AWindow) final {
    MIP_PRINT;
    return false;
  }

  void on_plugin_closeEditor() final {
    MIP_PRINT;
  }

  void on_plugin_updateEditor() final {
    MIP_PRINT;
  }

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

MIP_MAIN(myDescriptor,myInstance,myEditor);
