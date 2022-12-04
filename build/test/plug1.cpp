#define MIP_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/mip.socket

//----------

#include "plugin/mip_plugin.h"
#include "audio/mip_audio_utils.h"

#ifndef MIP_NO_GUI
  #include "plugin/mip_editor.h"
  #include "gui/widgets/mip_panel_widget.h"
  #include "gui/widgets/mip_text_widget.h"
  #include "gui/widgets/mip_value_widget.h"
  #include "gui/widgets/mip_button_widget.h"
  #include "gui/widgets/mip_drag_value_widget.h"
  #include "gui/widgets/mip_slider_widget.h"
#endif

//----------------------------------------------------------------------
//
// descriptor
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
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  double gain  = 0.0;
  double left  = 0.0;
  double right = 0.0;

  MIP_PanelWidget* MRootWidget = nullptr;

//------------------------------
public:
//------------------------------

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    setInitialEditorSize(640,480);
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    bool result = MIP_Plugin::init();
    appendStereoInput();
    appendStereoOutput();
    appendParameter( new MIP_Parameter("Gain", 1.0) );
    appendParameter( new MIP_Parameter("Left", 1.0) );
    appendParameter( new MIP_Parameter("Right",1.0) );
    setDefaultParameterValues();
    return result;
  }

  //----------

  #ifndef MIP_NO_GUI

  bool gui_create(const char* api, bool is_floating) final {
    bool result = MIP_Plugin::gui_create(api,is_floating);
    if (result) {
      MIP_Window* window = MEditor.getWindow();
      MIP_Assert(window);
      MRootWidget = new MIP_PanelWidget(MIP_DRect(0,0,640,480));
      MEditor.setRootWidget(MRootWidget);//,window);
      MIP_TextWidget* text = new MIP_TextWidget( MIP_DRect(100,50,200,50), "Hello world!" );
      text->setTextSize(30);
      MRootWidget->appendChildWidget(text);
      MIP_ValueWidget* value = new MIP_ValueWidget( MIP_DRect(100,110,200,20), "Value", 0.5 );
      MRootWidget->appendChildWidget(value);
      MIP_ButtonWidget* button = new MIP_ButtonWidget( MIP_DRect(100,140,200,20), "Button", 0.0 );
      MRootWidget->appendChildWidget(button);
      MIP_DragValueWidget* dragvalue = new MIP_DragValueWidget( MIP_DRect(100,170,200,20), "DragValue", 0.5 );
      MRootWidget->appendChildWidget(dragvalue);
      MIP_SliderWidget* slider = new MIP_SliderWidget( MIP_DRect(100,200,200,20), "Slider", 0.5 );
      MRootWidget->appendChildWidget(slider);
    }
    return result;
  }

  #endif

  //----------

  void gui_destroy() final {
    MIP_Plugin::gui_destroy();
    if (MRootWidget) delete MRootWidget;
  }

  //----------

  void processParamValue(const clap_event_param_value_t* event) final {
    switch (event->param_id) {
      case 0: gain  = event->value; break;
      case 1: left  = event->value; break;
      case 2: right = event->value; break;
    }
  }

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** inputs  = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    MIP_CopyStereoBuffer(outputs,inputs,length);
    MIP_ScaleStereoBuffer(outputs,left*gain,right*gain,length);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/mip_entry.h"

MIP_DEFAULT_ENTRY(myDescriptor,myPlugin);

