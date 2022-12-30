
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------

#include "plugin/mip_plugin.h"
#include "audio/mip_audio_utils.h"

#ifndef MIP_NO_GUI
  #include "gui/mip_widgets.h"
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

  const char* brtxt[6] = { "one", "2", "3", "IV", "V", "six" };

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
      MIP_Window* window = MEditor->getWindow();
      MIP_Assert(window);
      MRootWidget = new MIP_PanelWidget(MIP_DRect(0,0,640,480));
      MEditor->setRootWidget(MRootWidget);//,window);
      MIP_MenuWidget* menu = new MIP_MenuWidget(MIP_DRect(0,0,100,60), nullptr);
      menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,100,15), "Item1" ) );
      menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,15,100,15), "Item2" ) );
      menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,30,100,15), "Item3" ) );
      menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,45,100,15), "Item4" ) );
      MIP_TextWidget* text = new MIP_TextWidget( MIP_DRect(100,50,200,50), "Hello world!" );
      text->setTextSize(30);
      MRootWidget->appendChildWidget(text);
      MIP_ValueWidget* value = new MIP_ValueWidget( MIP_DRect(100,110,200,20), "Value", 0.5 );
      MRootWidget->appendChildWidget(value);
      MIP_ButtonWidget* button = new MIP_ButtonWidget( MIP_DRect(100,140,200,20), "Button", 0.0 );
      MRootWidget->appendChildWidget(button);
      MIP_DragValueWidget* dragvalue = new MIP_DragValueWidget( MIP_DRect(100,170,200,20), "DragValue", 0.5 );
      MRootWidget->appendChildWidget(dragvalue);
//      dragvalue->clearFlag(MIP_WIDGET_FLAG_AUTO_HIDE_CURSOR);
      MIP_SliderWidget* slider = new MIP_SliderWidget( MIP_DRect(100,200,200,20), "Slider", 0.5 );
      MRootWidget->appendChildWidget(slider);
      MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_DRect(100,230,50,50), "Knob", 0.5 );
      MRootWidget->appendChildWidget(knob);
      MIP_MenuItemWidget* menuitem = new MIP_MenuItemWidget( MIP_DRect(100,300,200,20), "menuitem" );
      MRootWidget->appendChildWidget(menuitem);
      MIP_SelectorWidget* selector = new MIP_SelectorWidget( MIP_DRect(100,330,200,20), "selector", menu );
      MRootWidget->appendChildWidget(selector);
      MIP_GridWidget* grid = new MIP_GridWidget( MIP_DRect(400,100,200,200), 6, 6 );
      MRootWidget->appendChildWidget(grid);
      MIP_ButtonRowWidget* buttonrow = new MIP_ButtonRowWidget( MIP_DRect(400,320,200,20), 5, brtxt, MIP_BUTTON_ROW_SINGLE );
      MRootWidget->appendChildWidget(buttonrow);
      MIP_ButtonRowWidget* buttonrow2 = new MIP_ButtonRowWidget( MIP_DRect(400,350,200,20), 6, brtxt, MIP_BUTTON_ROW_MULTI );
      MRootWidget->appendChildWidget(buttonrow2);
      MRootWidget->appendChildWidget(menu);
      MEditor->connect( knob,      getParameter(0) );
      MEditor->connect( slider,    getParameter(1) );
      MEditor->connect( dragvalue, getParameter(2) );
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

