
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/mip_plugin.h"
#include "plugin/mip_editor.h"
#include "gui/mip_window.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin/0",
  .name         = "myPlugin",
  .vendor       = "me",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.0",
  .description  = "my plugin",
  .features     = (const char*[]){"audio-effect",nullptr}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myEditor
: public MIP_Editor {

//------------------------------
private:
//------------------------------

  MIP_PanelWidget* MRoot = nullptr;

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray* AParameters)
  : MIP_Editor(AListener,AWidth,AHeight,AParameters) {

    MRoot = new MIP_PanelWidget(MIP_DRect(0,0,640,480));
    //setRootWidget(panel,this);
    setRootWidget(MRoot);

    //MIP_Knob2Widget* knob2 = new MIP_Knob2Widget(MIP_DRect(10,10,200,250),"Knob",0.5);
    //MRoot->appendChildWidget(knob2);
    //knob2->setDrawBorder(true);

    MIP_SliderWidget* slider = new MIP_SliderWidget(MIP_DRect(10,10,200,20),"Slider",0.5);
    MRoot->appendChildWidget(slider);

    connect(slider,AParameters->getItem(0));

  }

  //----------

  virtual ~myEditor() {
    if (MRoot) delete MRoot;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

public:

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    appendStereoInput();
    appendStereoOutput();
    MIP_Parameter* param1 = new MIP_Parameter("param1",0.5,0,1);
    appendParameter(param1);
  }

  //----------

  virtual ~myPlugin() {
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    MIsEditorOpen = false;
    MEditor = new myEditor(this,640,480,&MParameters);
    return true;
  }

  //----------

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/mip_entry.h"
MIP_DEFAULT_ENTRY(myDescriptor,myPlugin);
