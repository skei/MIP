
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------

#define SA_BOTAGE_EDITOR_WIDTH  640
#define SA_BOTAGE_EDITOR_HEIGHT 480

#ifdef MIP_DEBUG
  #define SA_BOTAGE_NAME        "sa_botage (debug)"
#else
  #define SA_BOTAGE_NAME        "sa_botage"
#endif

#define SA_BOTAGE_VENDOR        "skei.audio"
#define SA_BOTAGE_VERSION       "0.3.0"

//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"
#include "audio/mip_audio_utils.h"

//----------

#include "sa_botage/sa_botage_editor.h"
#include "sa_botage/sa_botage_parameters.h"
#include "sa_botage/sa_botage_processor.h"
#include "sa_botage/sa_botage_widgets.h"

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

  //double gain  = 0.0;
  //double left  = 0.0;
  //double right = 0.0;

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
    setDefaultParameterValues();
    return result;
  }

  //----------

  bool gui_create(const char* api, bool is_floating) final {
    //MIP_Print("\n");
    MIsEditorOpen = false;
    MEditor = new sa_botage_editor(this,MInitialEditorWidth,MInitialEditorHeight,&MParameters);
    return (MEditor);
  }

  //----------

  //void gui_destroy() final {
  //  MIP_Plugin::gui_destroy();
  //}

  //----------

  void processParamValue(const clap_event_param_value_t* event) final {

    switch (event->param_id) {

      case 0: {
        uint32_t beats = (uint32_t)event->value;
        if (MIsEditorOpen) {
          sa_botage_editor* editor = (sa_botage_editor*)MEditor;
          if (editor) {
            MIP_WaveformWidget* waveform = editor->MWaveformWidget;
            if (waveform) {
              waveform->setNumGrid(beats);
              //editor->redraw(waveform);
              waveform->redraw();
            }
          }
        }
        break;
      }

      case 1: {
        uint32_t slices = (uint32_t)event->value;
        if (MIsEditorOpen) {
          sa_botage_editor* editor = (sa_botage_editor*)MEditor;
          if (editor) {
            MIP_WaveformWidget* waveform = editor->MWaveformWidget;
            if (waveform) {
              waveform->setNumSubGrid(slices);
              //editor->redraw(waveform);
              waveform->redraw();
            }
          }
        }
        break;
      }

    } // switch
  }

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** inputs  = process->audio_inputs[0].data32;
    float** outputs = process->audio_outputs[0].data32;
    //gain   = 1.0; // MParameters[0]->getValue();
    //left   = 1.0; // MParameters[1]->getValue();
    //right  = 1.0; // MParameters[2]->getValue();
    MIP_CopyStereoBuffer(outputs,inputs,length);
    //MIP_ScaleStereoBuffer(outputs,left*gain,right*gain,length);
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

