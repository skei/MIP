#ifndef mip_exe_window_included
#define mip_exe_window_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/exe/mip_exe.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------

class MIP_ExeWindow
: public MIP_ImplementedWindow {

//------------------------------
private:
//------------------------------

  const clap_plugin_t*      plugin  = nullptr;
  const clap_plugin_gui_t*  gui     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ExeWindow(uint32_t AWidth, uint32_t AHeight, const clap_plugin_t* APlugin, const clap_plugin_gui_t* AGui)
  : MIP_ImplementedWindow(AWidth,AHeight) {
    plugin = APlugin;
    gui = AGui;
  }

//------------------------------
public:
//------------------------------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {
    gui->set_size(plugin,AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
#endif
