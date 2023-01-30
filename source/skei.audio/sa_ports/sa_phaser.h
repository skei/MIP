#ifndef sa_phaser_included
#define sa_phaser_included
//----------------------------------------------------------------------

// based on:
// https://github.com/Themaister/RetroArch-DSP-plugins/blob/master/phaser.cpp
//

#define MIP_PLUGIN_GENERIC_EDITOR

#include "plugin/mip_plugin.h"
#include "audio/mip_audio_math.h"

//#include "plugin/clap/mip_clap_preset_discovery.h"

#define phaserlfoshape  4.0
#define lfoskipsamples  20
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* sa_phaser_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_phaser_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_phaser/0.0.1",
  "sa_phaser",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.3.1",
  "",
  sa_phaser_features
};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Phaser {
private:
  float         freq;
  float         startphase;
  float         fb;
  int           depth;
  int           stages;
  int           drywet;
  unsigned long skipcount;
  float         old[24];
  float         gain;
  float         fbout;
  float         lfoskip;
  float         phase;

public:

  MIP_Phaser() {
  }

  ~MIP_Phaser() {
    for (int j=0; j<stages; j++) old[j] = 0;
  }

public:

  void SetLFOFreq(float val) {
    freq = val;
  }

  void SetLFOStartPhase(float val) {
    startphase = val;
  }

  void SetFeedback(float val) {
    fb = val;
  }

  void SetDepth(float val) {
    depth = val;
  }

  void SetStages(float val) {
    stages = val;
  }

  void SetDryWet(float val) {
    drywet = val;
  }

  void init(float samplerate) {
    skipcount = 0;
    gain      = 0.0;
    fbout     = 0.0;
    lfoskip   = freq * 2.0 * MIP_PI / samplerate;
    phase     = startphase * MIP_PI / 180.0;
    for (int j=0; j<stages; j++) old[j] = 0;
  }

  float process(float in) {
    float m, tmp, out;
    int j;
    m = in + fbout * fb / 100.0;
    if (((skipcount++) % lfoskipsamples) == 0) {
      gain = (1.0 + cos(skipcount * lfoskip + phase)) / 2.0;
      gain =(exp(gain * phaserlfoshape) - 1.0) / (exp(phaserlfoshape) - 1.0);
      gain = 1 - gain / 255.0 * depth;
    }
    for (j = 0; j < stages; j++) {
      tmp = old[j];
      old[j] = gain * tmp + m;
      m = tmp - gain * old[j];
    }
    fbout = m;
    out = (m * drywet + in * (255.0 - drywet)) / 255.0;
    if (out < -1.0) out = -1.0;
    if (out > 1.0) out = 1.0;
    return out;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_phaser_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool          MNeedRecalc = false;
  float         MSampleRate = 0.0;

  MIP_Phaser    MPhaserL = {};
  MIP_Phaser    MPhaserR = {};

//------------------------------
public:
//------------------------------

  sa_phaser_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
    uint32_t w = getGenericWidth();
    uint32_t h = calcGenericHeight(5);
    setInitialEditorSize(w,h);
  }

  //----------

  virtual ~sa_phaser_plugin() {
  }

//------------------------------
private:
//------------------------------

  //void precalc() {
  //}

  //----------

  void recalc(float srate) {
    MNeedRecalc = false;
  }

//------------------------------
private:
//------------------------------

  void processParamValue(const clap_event_param_value_t* param_value) final {
    uint32_t index = param_value->param_id;
    double   value = param_value->value;
    switch (index) {
      case 0: MPhaserL.SetLFOFreq(value);       MPhaserR.SetLFOFreq(value);       break;
      case 1: MPhaserL.SetLFOStartPhase(value); MPhaserR.SetLFOStartPhase(value); break;
      case 2: MPhaserL.SetFeedback(value);      MPhaserR.SetFeedback(value);      break;
      case 3: MPhaserL.SetDepth(value);         MPhaserR.SetDepth(value);         break;
      case 4: MPhaserL.SetStages(value);        MPhaserR.SetStages(value);        break;
      case 5: MPhaserL.SetDryWet(value);        MPhaserR.SetDryWet(value);        break;
    }
    //MNeedRecalc = true;
  }

  //----------

  void processAudioBlock(MIP_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    if (MNeedRecalc) recalc(MSampleRate);
    uint32_t len = process->frames_count;
    float* in0  = process->audio_inputs[0].data32[0];
    float* in1  = process->audio_inputs[0].data32[1];
    float* out0 = process->audio_outputs[0].data32[0];
    float* out1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      spl0 = MPhaserL.process(spl0);
      spl1 = MPhaserR.process(spl1);
      *out0++ = spl0;
      *out1++ = spl1;
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendStereoInput();
    appendStereoOutput();
    appendParameter( new MIP_Parameter(   "LFO Freq",       0,    0,    127 ));
    appendParameter( new MIP_Parameter(   "LFO StartPhase", 0,    0,    127 ));
    appendParameter( new MIP_Parameter(   "Feedback",       0,    0,    127 ));
    appendParameter( new MIP_Parameter(   "Depth",          100,  0,    127 ));
    appendParameter( new MIP_IntParameter("Stages",         2,    0,    10  ));
    appendParameter( new MIP_Parameter(   "Dry / Wet",      128,  0,    127 ));
    MNeedRecalc = true;
    bool result = MIP_Plugin::init();
    return result;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    MPhaserL.init(sample_rate);
    MPhaserR.init(sample_rate);
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    bool result = MIP_Plugin::gui_create(api,is_floating);
    if (result) {
      MIP_Widget* panel = setupGenericEditor();
      MEditor->setRootWidget(panel);
    }
    return result;
  }

  //----------

  //const void* get_extension(const char *id) final {
  //  const void* ext = MIP_Plugin::get_extension(id);
  //  if (!ext) {
  //    if (strcmp(id,CLAP_EXT_GUI) == 0)             return &MGui;
  //    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)     return &MAudioPorts;
  //    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)      return &MNotePorts;
  //    if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)  return &MQuickControls;
  //  }
  //  return ext;
  //}

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef MIP_NO_ENTRY

  #include "plugin/mip_entry.h"
  MIP_DEFAULT_ENTRY(sa_phaser_descriptor,sa_phaser_plugin)

#endif // MIP_NO_ENTRY

//----------------------------------------------------------------------
#endif
