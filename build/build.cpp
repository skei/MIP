
// nc -U -l -k /tmp/mip.socket

//#define MIP_DEBUG_CLAP
//#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_DEBUG_PRINT_THREAD
//#define MIP_DEBUG_PRINT_TIME

#define MIP_GUI_XCB
#define MIP_PLUGIN_ALL

#include "mip.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public MIP_PluginDescriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : MIP_PluginDescriptor() {
    MName = "myPlugin";
    MAuthor = "me";
    MVersion = 0x00000001;
    appendInputPort(  new MIP_PluginPort("input",  MIP_PLUGIN_PORT_AUDIO, 2, MIP_PLUGIN_PORT_INPUT) );
    appendOutputPort( "output");
    appendParameter(  new MIP_PluginParameter("param1") );
    appendParameter(  "param4", 0, -1, MIP_PI );
    //MHasEditor = true;
    //setEditorSize(640,480);
  }

  //----------

  virtual ~myDescriptor() {
  }

};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor
: public MIP_PluginEditor {

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, MIP_PluginDescriptor* ADescriptor)
  : MIP_PluginEditor(AListener,ADescriptor) {
  }

  virtual ~myEditor() {
  }

  void on_editor_attach() final {
  }

  void on_editor_open() final {
  }

  void on_editor_close() final {
  }

};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public MIP_PluginInstance {

//------------------------------
private:
//------------------------------

  myEditor* MEditor = nullptr;

//------------------------------
public:
//------------------------------

  myInstance(MIP_PluginDescriptor* ADescriptor)
  : MIP_PluginInstance(ADescriptor) {
    //MIP_Print("descriptor %p\n",ADescriptor);
  }

  virtual ~myInstance() {
    //MIP_Print("\n");
  }

//------------------------------
public:
//------------------------------

  bool on_plugin_init() final {
    //MIP_Print("-> true\n");
    return true;
  }

  void on_plugin_deinit() final {
    //MIP_Print("\n");
  }

  bool on_plugin_activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) final {
    //MIP_Print("samplerate %f minframes %i maxframes %i -> true\n",ASampleRate,AMinFrames,AMaxFrames);
    return true;
  }

  void on_plugin_deactivate() final {
    //MIP_Print("\n");
  }

  bool on_plugin_startProcessing() final {
    //MIP_Print("-> true\n");
    return true;
  }

  void on_plugin_stopProcessing() final {
    //MIP_Print("\n");
  }

  //----------

  uint32_t on_plugin_process(MIP_ProcessContext* AContext) final {
    //MIP_Print("-> 0\n");
    return 0;
  }

  void on_plugin_parameter(uint32_t AIndex, float AValue) final {
    //MIP_Print("index %i value %f\n",AIndex,AValue);
  }

  void on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
    //MIP_Print("%02X %02X %02X \n",AMsg1,AMsg2,AMsg3);
  }

  void on_plugin_expression(uint32_t AKey, uint32_t AExpression, float AValue) final {
    //MIP_Print("key %i expression %i value %f\n",AKey,AExpression,AValue);
  }

  //----------

  bool on_plugin_openEditor(MIP_Window* AWindow) final {
    //MIP_Print("window %p -> false\n");
    return false;
  }

  void on_plugin_closeEditor() final {
    //MIP_Print("\n");
  }

  void on_plugin_updateEditor() final {
    //MIP_Print("\n");
  }

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

//bool is_registered = false;

void MIP_RegisterPlugins() {
  //MIP_Print("\n");
  //if (is_registered) {
  //  MIP_Print("MIP_RegisterPlugins has already been called!\n");
  //}
  //else {
    MIP_PLUGIN_LIST.appendPlugin( new myDescriptor() );
  //}
  //is_registered = true;
}

//------------------------------
//
//------------------------------

MIP_PluginInstance* MIP_CreateInstance(uint32_t AIndex, MIP_PluginDescriptor* ADescriptor) {
  //MIP_Print("index %i descriptor %p\n",AIndex,ADescriptor);
  if (AIndex == 0) return new myInstance(ADescriptor);
  else return nullptr;
}

//----------

MIP_PluginEditor* MIP_CreateEditor(uint32_t AIndex, MIP_EditorListener* AListener, MIP_PluginDescriptor* ADescriptor) {
  //MIP_Print("index %i listener %i descriptor %p\n",AIndex,AListener,ADescriptor);
  if (AIndex == 0) return new myEditor(AListener,ADescriptor);
  else return nullptr;
}

//------------------------------
//
//------------------------------

MIP_MAIN;
