#ifndef mip_exe_entry
#define mip_exe_entry
//----------------------------------------------------------------------

#include "base/mip.h"
#include "base/utils/mip_arguments.h"
#include "plugin/exe/mip_exe_window.h"

//----------------------------------------------------------------------

MIP_Arguments Arguments = {};

// hack to make it compile,..
const clap_host_t*              claphost        = nullptr;
const clap_plugin_descriptor_t* clapdescriptor  = nullptr;
const clap_plugin_t*            clapplugin      = nullptr;

MIP_Plugin*                     plugin          = nullptr;

//----------------------------------------------------------------------

/*

  #ifndef MIP_NO_GUI





  }

*/

void mip_exe_open_editor() {
  MIP_DPrint("Loading editor\n");

  clap_plugin_gui_t* gui = (clap_plugin_gui_t*)plugin->get_extension(CLAP_EXT_GUI);
  if (gui) {

    #ifdef MIP_LINUX
    if (gui->is_api_supported(clapplugin,CLAP_WINDOW_API_X11,false)) {
      gui->create(clapplugin,CLAP_WINDOW_API_X11,false);
    #endif

    #ifdef MIP_WIN32
    if (gui->is_api_supported(clapplugin,CLAP_WINDOW_API_WIN32,false)) {
      gui->create(clapplugin,CLAP_WINDOW_API_WIN32,false);
    #endif

      gui->set_scale(clapplugin,1.0);

      //bool can_resize = gui->can_resize(clapplugin);
      //if (can_resize) {
      // plugin->MEditorWidth/Height
      uint32_t width, height;
      gui->get_size(clapplugin,&width,&height);
      //}
      //else {
      //}
      //gui->set_size(clapplugin,width,height);

      //MIP_Window* window = new MIP_Window(width,height);
      MIP_ExeWindow* window = new MIP_ExeWindow(width,height,clapplugin,gui);
      if (window) {

        window->open();

        clap_window_t clap_window;
        #ifdef MIP_LINUX
          clap_window.api = CLAP_WINDOW_API_X11;
          clap_window.x11 = window->tgtGetWindow();
        #endif
        #ifdef MIP_WIN32
          clap_window.api = CLAP_WINDOW_API_WIN32;
          clap_window.win32 = window->tgtGetWindow();
        #endif

        gui->set_parent(clapplugin,&clap_window);
        gui->show(clapplugin);
        //redraw?
        gui->set_size(clapplugin,width,height);

        // when window resizes, we need to tell the editor something...

        window->eventLoop();

        gui->hide(clapplugin);
        gui->destroy(clapplugin);

        window->close();
        delete window;

      } // window

    } // api supported

  } // gui

}

//----------------------------------------------------------------------

int mip_exe_load_plugin(uint32_t AIndex) {
  MIP_DPrint("Loading plugin %i\n",AIndex);
  uint32_t num = MIP_REGISTRY.getNumDescriptors();
  if (AIndex >= num) {
    MIP_DPrint("Plugin index out of bounds\n");
    return -1;
  }

  claphost = nullptr;
  clapdescriptor = MIP_REGISTRY.getDescriptor(AIndex);

  MIP_DPrint("  clap_version    %i.%i.%i\n",clapdescriptor->clap_version.major,clapdescriptor->clap_version.minor,clapdescriptor->clap_version.revision);
  MIP_DPrint("  id              %s\n",clapdescriptor->id);
  MIP_DPrint("  name            %s\n",clapdescriptor->name);
  MIP_DPrint("  vendor          %s\n",clapdescriptor->vendor);
  MIP_DPrint("  url             %s\n",clapdescriptor->url);
  MIP_DPrint("  manual_url      %s\n",clapdescriptor->manual_url);
  MIP_DPrint("  support_url     %s\n",clapdescriptor->support_url);
  MIP_DPrint("  version         %s\n",clapdescriptor->version);
  MIP_DPrint("  description     %s\n",clapdescriptor->description);
  MIP_DPrint("  features       ");
  const char** ptr = clapdescriptor->features;
  while (*ptr) {
    MIP_DPrint("%s ",*ptr);
    ptr += 1;
  }
  MIP_DPrint("\n");

  plugin = MIP_CreatePlugin(AIndex,clapdescriptor,claphost);
  clapplugin = plugin->getClapPlugin();
  plugin->init();
  plugin->activate(44100,256,1024);
  plugin->start_processing();

  /*if (Arguments.hasOption("-e"))*/ mip_exe_open_editor();

  plugin->stop_processing();
  plugin->deactivate();
  plugin->destroy();

  delete plugin;
  return 0;
}

//----------------------------------------------------------------------


int main(int argc, char** argv) {
  int result = 0;
  Arguments.init(argc,argv);
  if (Arguments.hasOption("-?")) {
    MIP_DPrint("arguments:\n");
    MIP_DPrint("  -i n   plugin index (default: 0)\n");
    MIP_DPrint("  -e     open editor\n");
  }
  else {
    int index = Arguments.getArgInt("-i");
    result = mip_exe_load_plugin(index);
  }
  return result;
}

//----------------------------------------------------------------------
#endif
