#ifndef mip_exe_included
#define mip_exe_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//template <class DESC, class INST, class EDIT>
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
public:
//------------------------------

  //TODO: error checking, safety..

  int main(int argc, char** argv) {
    //DESC* descriptor = new DESC();

    //MIP_Descriptor* descriptor = MIP_CreateDescriptor(0);
    MIP_PluginInfo* info = MIP_GLOBAL_PLUGIN_LIST.getPlugin(0);
    MIP_Descriptor* descriptor = info->desc;

    //INST* instance = new INST(descriptor);
    MIP_Instance* instance = MIP_CreateInstance(0,descriptor);

    instance->on_plugin_init();
    instance->on_plugin_activate(44100.0,128,128);
    instance->on_plugin_startProcessing();
    if (descriptor->hasEditor()) {
      //EDIT* editor = new EDIT(instance,descriptor);
      MIP_Editor* editor = MIP_CreateEditor(0,instance,descriptor);
      editor->attach("",nullptr);
      MIP_Window* window = editor->getWindow();
      instance->on_plugin_openEditor(window);
      editor->open();
      window->eventLoop();
      instance->on_plugin_closeEditor();
      editor->close();
      delete editor;
    }
    delete instance;
    //delete descriptor;
    return 0;
  }

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

/* #define MIP_EXE_MAIN(D,I,E) */

#define MIP_EXE_MAIN                              \
                                                  \
  MIP_Exe MIP_GLOBAL_EXE;                         \
                                                  \
  int main(int argc, char** argv) {               \
    return MIP_GLOBAL_EXE.main(argc,argv);        \
  }                                               \
                                                  \
  /* MIP_Exe<D,I,E> MIP_GLOBAL_EXE;           */  \
  /*                                          */  \
  /* int main(int argc, char** argv) {        */  \
  /*   MIP_PRINT;                             */  \
  /*   return MIP_GLOBAL_EXE.main(argc,argv); */  \
  /* }                                        */  \


//----------------------------------------------------------------------
#endif
