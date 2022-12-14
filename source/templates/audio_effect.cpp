
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------

#define MY_PLUGIN_NAME            "my_plugin"
#define MY_PLUGIN_VENDOR          "skei.audio"
#define MY_PLUGIN_VERSION         "0.3.0"
#define MY_PLUGIN_DESCRIPTION     "sabotage your audio!"
#define MY_PLUGIN_EDITOR_WIDTH    400
#define MY_PLUGIN_EDITOR_HEIGHT   500

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"
#include "gui/mip_widgets.h"
#include "audio/mip_audio_utils.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t my_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = MY_PLUGIN_VENDOR "/" MY_PLUGIN_NAME "/v" MY_PLUGIN_VERSION,
  .name         = MY_PLUGIN_NAME,
  .vendor       = MY_PLUGIN_VENDOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = MY_PLUGIN_VERSION,
  .description  = MY_PLUGIN_DESCRIPTION,
  .features     = (const char*[]){"audio-effect",nullptr}
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class my_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  //double gain = 0.0;

//------------------------------
public:
//------------------------------

  my_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    setInitialEditorSize(MY_PLUGIN_EDITOR_WIDTH,MY_PLUGIN_EDITOR_HEIGHT);
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    bool result = MIP_Plugin::init();
    if (result) {
      appendStereoInput();
      appendStereoOutput();
      appendParameter( new MIP_Parameter("Gain", 0.5) );
      //setDefaultParameterValues(); // called in activate()
    }
    return result;
  }

  //----------

  bool gui_create(const char* api, bool is_floating) final {
    MIsEditorOpen = false;
    //return MIP_Plugin::gui_create(api,is_floating);
    //MEditor = new my_editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters);
    MEditor = new MIP_Editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters);
    if (MEditor) {
      MIP_PanelWidget* background = new MIP_PanelWidget(MIP_DRect(0,0, MY_PLUGIN_EDITOR_WIDTH, MY_PLUGIN_EDITOR_HEIGHT));
      MEditor->setRootWidget(background);
      MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_DRect(50,50,300,400) );
      background->appendChildWidget(knob);
      knob->setArcThickness(40);
      MEditor->connect( knob, MParameters[0] );
    }
    return (MEditor);
  }

  //----------

  //void processParamValue(const clap_event_param_value_t* event) final {
  //  switch (event->param_id) {
  //    case 0: gain = event->value; break;
  //  } // switch
  //}

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** inputs  = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    double gain = MParameters[0]->getValue();
    MIP_CopyStereoBuffer(outputs,inputs,length);
    MIP_ScaleStereoBuffer(outputs,gain,length);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

//#ifndef MIP_NO_ENTRY

  #include "plugin/mip_entry.h"
  MIP_DEFAULT_ENTRY(my_descriptor,my_plugin);

//#endif // MIP_NO_ENTRY

