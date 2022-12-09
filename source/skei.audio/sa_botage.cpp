
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------

#include "audio/mip_audio_utils.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_plugin.h"

//----------

#define SA_BOTAGE_EDITOR_WIDTH  640
#define SA_BOTAGE_EDITOR_HEIGHT 480

#include "sa_botage/sa_botage_editor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_botage_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "skei.audio/sa_botage/v0.1.0",
  .name         = "sa_botage",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.1.0",
  .description  = "sabotages your audio",
  .features     = (const char*[]){"audio-effect",nullptr}
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_botage_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  MIP_PanelWidget* MRootWidget = nullptr;

  // parameters
  double gain  = 0.0;
  double left  = 0.0;
  double right = 0.0;

  //const char* brtxt[6] = { "one", "2", "3", "IV", "V", "six" };

//------------------------------
public:
//------------------------------

  sa_botage_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    setInitialEditorSize(SA_BOTAGE_EDITOR_WIDTH,SA_BOTAGE_EDITOR_HEIGHT);
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

  bool gui_create(const char* api, bool is_floating) final {
    bool result = MIP_Plugin::gui_create(api,is_floating);
    if (result) {
      MRootWidget = new MIP_PanelWidget(MIP_DRect(0,0,SA_BOTAGE_EDITOR_WIDTH,SA_BOTAGE_EDITOR_HEIGHT));
      MEditor.setRootWidget(MRootWidget);
      sa_botage_setup_editor(&MEditor,MRootWidget);
    }
    return result;
  }

  //----------

  void gui_destroy() final {
    MIP_Plugin::gui_destroy();
    if (MRootWidget) delete MRootWidget;
  }

  //----------

  //void processParamValue(const clap_event_param_value_t* event) final {
  //  switch (event->param_id) {
  //    case 0: gain  = event->value; break;
  //    case 1: left  = event->value; break;
  //    case 2: right = event->value; break;
  //  }
  //}

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** inputs  = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;

    gain   = MParameters[0]->getValue();
    left   = MParameters[1]->getValue();
    right  = MParameters[2]->getValue();

    MIP_CopyStereoBuffer(outputs,inputs,length);
    MIP_ScaleStereoBuffer(outputs,left*gain,right*gain,length);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef MIP_NO_ENTRY

  #include "plugin/mip_entry.h"
  MIP_DEFAULT_ENTRY(sa_botage_descriptor,sa_botage_plugin);

#endif // MIP_NO_ENTRY

