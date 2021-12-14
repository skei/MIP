#ifndef mip_exe_included
#define mip_exe_included
//----------------------------------------------------------------------

#define MIP_EXE_AUDIO_SAMPLERATE  44100.0
#define MIP_EXE_AUDIO_BLOCKSIZE   256

//----------

#include "mip.h"
//#include "base/utils/mip_arguments.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Exe {

//------------------------------
private:
//------------------------------

  //MAudioIO* MAudioIO = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Exe() {
  }

  //----------

  ~MIP_Exe() {
  }

//------------------------------
private:
//------------------------------

  void start_audio(float ASampleRate=44100.0, uint32_t ABlockSize=256) {
  }

  //----------

  void stop_audio() {
  }

//------------------------------
public:
//------------------------------

  //TODO: error checking, safety..

  int main(int argc, char** argv) {

    //MIP_Arguments arg;
    //arg.init(argc,argv);
    //const char* s = arg.getValStr("-l","--long-string");
    //MIP_Print("s = %s\n",s);
    //int i = arg.getValInt("-p");
    //MIP_Print("i = %i\n",i);
    //float f = arg.getValAfterFloat("-p",':');
    //MIP_Print("f = %f\n",f);


    MIP_Print("Starting audio\n");
    start_audio(MIP_EXE_AUDIO_SAMPLERATE,MIP_EXE_AUDIO_BLOCKSIZE);
    MIP_Print("Audio started\n");

    MIP_Print("Initializing plugin\n");
    MIP_PluginInfo* info = MIP_PLUGIN_LIST.getPluginInfo(0);
    MIP_PluginDescriptor* descriptor = info->desc;
    MIP_PluginInstance* instance = MIP_CreateInstance(0,descriptor);
    instance->on_plugin_init();
    instance->on_plugin_activate(MIP_EXE_AUDIO_SAMPLERATE,MIP_EXE_AUDIO_BLOCKSIZE,MIP_EXE_AUDIO_BLOCKSIZE);
    instance->on_plugin_startProcessing();
    MIP_Print("Plugin initialized\n");

    if (descriptor->hasEditor()) {

      MIP_Print("Opening editor\n");
      MIP_PluginEditor* editor = MIP_CreateEditor(0,instance,descriptor);
      editor->attach("",nullptr);
      MIP_Window* window = editor->getWindow();
      instance->on_plugin_openEditor(window);
      editor->open();
      MIP_Print("Editor opened\n");

      window->eventLoop();

      MIP_Print("Closing editor\n");
      instance->on_plugin_closeEditor();
      editor->close();
      delete editor;
      MIP_Print("Editor closed\n");

    }
    MIP_Print("Shutting down plugin\n");
    instance->on_plugin_stopProcessing();
    instance->on_plugin_deactivate();
    instance->on_plugin_deinit();
    delete instance;
    MIP_Print("Plugin shut down\n");

    MIP_Print("Stopping audio\n");
    stop_audio();
    MIP_Print("Audio stopped\n");

    return 0;
  }

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

#define MIP_EXE_MAIN                              \
                                                  \
  MIP_Exe MIP_GLOBAL_EXE;                         \
                                                  \
  int main(int argc, char** argv) {               \
    return MIP_GLOBAL_EXE.main(argc,argv);        \
  }                                               \


//----------------------------------------------------------------------
#endif
