
// nc -U -l -k /tmp/mip.socket

//#define MIP_DEBUG_CLAP
//#define MIP_DEBUG_PRINT_SOCKET
//#define MIP_DEBUG_PRINT_THREAD
//#define MIP_DEBUG_PRINT_TIME

//#define MIP_LIB
//#define MIP_EXE

#define MIP_GUI_XCB
#define MIP_USE_CAIRO

//#define MIP_PLUGIN_ALL
#define MIP_PLUGIN_EXE
//#define MIP_PLUGIN_REGISTER_PLUGINS

#include "mip.h"
#include "base/utils/mip_arguments.h"

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
    MHasEditor = true;
    setEditorSize(640,480);
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
    MIP_PRINT;
  }

  void on_editor_open() final {
    MIP_PRINT;
  }

  void on_editor_close() final {
    MIP_PRINT;
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
  }

  virtual ~myInstance() {
  }

//------------------------------
public:
//------------------------------

  bool on_plugin_init() final {
    return true;
  }

  void on_plugin_deinit() final {
  }

  bool on_plugin_activate(float ASampleRate, uint32_t AMinFrames, uint32_t AMaxFrames) final {
    return true;
  }

  void on_plugin_deactivate() final {
  }

  bool on_plugin_startProcessing() final {
    return true;
  }

  void on_plugin_stopProcessing() final {
  }

  //----------

  uint32_t on_plugin_process(MIP_ProcessContext* AContext) final {
    return 1; //CLAP_PROCESS_CONTINUE;
  }

  void on_plugin_parameter(uint32_t AIndex, float AValue) final {
  }

  void on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
  }

  void on_plugin_expression(uint32_t AKey, uint32_t AExpression, float AValue) final {
  }

  //----------

  bool on_plugin_openEditor(MIP_Window* AWindow) final {
    return false;;
  }

  void on_plugin_closeEditor() final {
  }

  void on_plugin_updateEditor() final {
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
