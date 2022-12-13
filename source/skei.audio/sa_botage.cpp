
#ifdef MIP_PLUGIN
  #define MIP_DEBUG_PRINT_SOCKET
  // nc -U -l -k /tmp/mip.socket
#endif

//----------

#include "plugin/mip_plugin.h"
#include "audio/mip_audio_utils.h"

//----------

#define SA_BOTAGE_EDITOR_WIDTH  640
#define SA_BOTAGE_EDITOR_HEIGHT 480

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

  double gain  = 0.0;
  double left  = 0.0;
  double right = 0.0;

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

  bool gui_show() final {
    //MIP_Print("\n");

    //    MIP_Window* window = MEditor->getWindow();
    //    MIP_Painter* painter = MWindow->getWindowPainter();
    //    painter->setClipRect(MIP_DRect(0,0,width,height));

    bool result = MIP_Plugin::gui_show();

    //    uint32_t width = MEditor->getWidth();
    //    uint32_t height = MEditor->getHeight();
    //    MIP_Print("window width %i window height %i\n",width,height);

    //    MIP_Window* window = MEditor->getWindow();
    //    window->invalidate(0,0,width,height);

    //    MIP_Painter* painter = window->getWindowPainter();
    //    MIP_DRect cliprect = painter->getClipRect();
    //    MIP_Print("cliprect %.1f,%.1f,%.1f,%.1f\n",cliprect.x,cliprect.y,cliprect.w,cliprect.h);

    return result;
  }

  //----------

  void gui_destroy() final {
    //MIP_Print("\n");
    MIP_Plugin::gui_destroy();
    if (MRootWidget) delete MRootWidget;
  }

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

    gain   = 1.0; // MParameters[0]->getValue();
    left   = 1.0; // MParameters[1]->getValue();
    right  = 1.0; // MParameters[2]->getValue();

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

