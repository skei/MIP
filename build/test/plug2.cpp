
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

#define NUM_TEST_SAMPLES 100000

//

class myEditor
: public MIP_Editor {

//------------------------------
private:
//------------------------------

  MIP_PanelWidget*  MRoot   = nullptr;
  float*            MBuffer = nullptr;

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray* AParameters)
  : MIP_Editor(AListener,AWidth,AHeight,AParameters) {

    MRoot = new MIP_PanelWidget(MIP_DRect(0,0,640,480));
    setRootWidget(MRoot);

    MRoot->setFillBackground(true);
    MRoot->setBackgroundColor(0.2);

    MIP_SliderWidget* slider = new MIP_SliderWidget(MIP_DRect(10,10,200,20),"Slider",0.5);
    MRoot->appendChildWidget(slider);
    connect(slider,AParameters->getItem(0));

    MIP_CircularWaveformWidget* waveform = new MIP_CircularWaveformWidget(MIP_DRect(100,100,300,300));
    MRoot->appendChildWidget(waveform);

    //-----

    MBuffer = (float *)malloc(NUM_TEST_SAMPLES * sizeof(float) * 2);
    for (uint32_t i=0; i<NUM_TEST_SAMPLES; i++) {
      MBuffer[(i * 2)    ] = MIP_RandomSigned();
      MBuffer[(i * 2) + 1] = MIP_RandomSigned();
    }

    waveform->setAudioBuffer(NUM_TEST_SAMPLES,MBuffer,true);

    waveform->setGrid(4,4);
    waveform->setGridColor( MIP_Color(0.8), MIP_Color(0.5) );


    waveform->setNumAreas(2);
    waveform->setArea( 0, 0, 0.375);
    waveform->setArea( 1, 0, 0.125);

    waveform->setAreaColor( 0, MIP_Color(0, 0.4, 0));
    waveform->setAreaColor( 1, MIP_Color(0, 0.5, 0));

    waveform->setNumMarkers(1);
    waveform->setMarker(0, 0.26, 0.0025);
    waveform->setMarkerColor(0,MIP_Color(0.8, 0, 0));

  }

  //----------

  virtual ~myEditor() {
    if (MRoot) delete MRoot;
    free(MBuffer);
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
