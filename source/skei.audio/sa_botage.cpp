
//#ifdef MIP_PLUGIN
//  #define MIP_DEBUG_PRINT_SOCKET
//  // nc -U -l -k /tmp/mip.socket
//#endif

//----------

#define MIP_LOGGING
#define MIP_LOG_APPEND

#define SA_BOTAGE_EDITOR_WIDTH  640
#define SA_BOTAGE_EDITOR_HEIGHT 480

#define SA_BOTAGE_NAME          "sa_botage"
#define SA_BOTAGE_VENDOR        "skei.audio"
#define SA_BOTAGE_VERSION       "0.1.7"

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"
#include "audio/mip_audio_utils.h"

//----------

#include "sa_botage/sa_botage_parameters.h"
#include "sa_botage/sa_botage_processor.h"
#include "sa_botage/sa_botage_editor.h"
//#include "sa_botage/sa_botage_widgets.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_botage_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SA_BOTAGE_VENDOR "/" SA_BOTAGE_NAME "/v" SA_BOTAGE_VERSION,
  .name         = SA_BOTAGE_NAME,
  .vendor       = SA_BOTAGE_VENDOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = SA_BOTAGE_VERSION,
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

  sa_botage_processor MProcessor;

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
    sa_botage_init_parameters(this);
    return result;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
    MProcessor.activate(sample_rate);
    return true;
  }

  //----------

  bool gui_create(const char* api, bool is_floating) final {
    //MIP_Print("\n");
    MIsEditorOpen = false;
    MEditor = new sa_botage_editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters,getClapDescriptor());
    return (MEditor);
  }

  //----------

  //void gui_destroy() final {
  //  MIP_Plugin::gui_destroy();
  //}

  //----------

  void processParamValue(const clap_event_param_value_t* event) final {
    MProcessor.setParamValue(event->param_id,event->value);
  }

  //----------

  void processTransport(const clap_event_transport_t* transport) final {
    MIP_Plugin::processTransport(transport);
    MProcessor.transport(transport->flags);
  }

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    MProcessor.processAudioBlock(AContext);
  }

//------------------------------
public:
//------------------------------

  void on_timer_callback(MIP_Timer* ATimer) override {
    if (MEditor && MEditor->isEditorOpen()) {
      sa_botage_editor* editor = (sa_botage_editor*)MEditor;
      editor->updateWaveformWidget(&MProcessor);
      editor->updateProbIndicators(&MProcessor);
    }
    MIP_Plugin::on_timer_callback(ATimer);
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

