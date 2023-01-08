
// nc -U -l -k /tmp/mip.socket
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
#endif

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
  .id           = "me/my_plugin/0.0.0",
  .name         = "my_plugin",
  .vendor       = "me",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.0",
  .description  = "my amazing plugin!",
  .features     = (const char*[]){"audio-effect",nullptr}
};

#define MY_PLUGIN_EDITOR_WIDTH    400
#define MY_PLUGIN_EDITOR_HEIGHT   500

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
    }
    return result;
  }

  //----------

  bool gui_create(const char* api, bool is_floating) final {
    MIsEditorOpen = false;
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

