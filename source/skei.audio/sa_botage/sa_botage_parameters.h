#ifndef sa_botage_parameters_included
#define sa_botage_parameters_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/mip_parameter.h"
#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------

enum sa_botage_params_e {

  PAR_NUM_BEATS = 0,
  PAR_NUM_SLICES,

  PAR_RANGE_PROB,
  PAR_RANGE_SLICE_COUNT,
  PAR_RANGE_LOOP_COUNT,

  PAR_LOOP_ENV_ATTACK,
  PAR_LOOP_ENV_DECAY,
  PAR_SLICE_ENV_ATTACK,
  PAR_SLICE_ENV_DECAY,

  PAR_FX_FILTER_PROB,
  PAR_FX_FILTER_TYPE,
  PAR_FX_FILTER_FREQ,
  PAR_FX_FILTER_BW,

  PAR_PROB_SIZE_PROB_RANGE,
  PAR_PROB_SIZE_MIN_RANGE,
  PAR_PROB_SIZE_MAX_RANGE,
  PAR_PROB_SIZE_PROB_LOOP,
  PAR_PROB_SIZE_MIN_LOOP,
  PAR_PROB_SIZE_MAX_LOOP,

  PAR_PROB_SPEED_PROB_RANGE,
  PAR_PROB_SPEED_MIN_RANGE,
  PAR_PROB_SPEED_MAX_RANGE,
  PAR_PROB_SPEED_PROB_LOOP,
  PAR_PROB_SPEED_MIN_LOOP,
  PAR_PROB_SPEED_MAX_LOOP,

  PAR_PROB_OFFSET_PROB_RANGE,
  PAR_PROB_OFFSET_MIN_RANGE,
  PAR_PROB_OFFSET_MAX_RANGE,
  PAR_PROB_OFFSET_PROB_LOOP,
  PAR_PROB_OFFSET_MIN_LOOP,
  PAR_PROB_OFFSET_MAX_LOOP,

  PAR_PROB_REVERSE_PROB_RANGE,
  PAR_PROB_REVERSE_PROB_LOOP,

  PAR_PROB_FX_PROB_RANGE,
  PAR_PROB_FX_MIN_RANGE,
  PAR_PROB_FX_MAX_RANGE,
  PAR_PROB_FX_PROB_LOOP,
  PAR_PROB_FX_MIN_LOOP,
  PAR_PROB_FX_MAX_LOOP,

  SA_BOTAGE_PARAM_COUNT
};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_range_parameter
: public MIP_Parameter {

public:

  sa_botage_range_parameter(const char* AName="", double AValue=0.0, double AMin=-1.0, double AMax=1.0)
  : MIP_Parameter(AName,AValue,AMin,AMax) {
  }

  bool valueToText(double value, char *display, uint32_t size) override {
    double v = powf(0.5,-value);
    v *= 100.0;
    //sprintf(display,"%.2f",v);
    uint32_t iv = MIP_Trunc(v);
    sprintf(display,"%i",iv);
    return true;
  }

};

//----------

class sa_botage_percent_parameter
: public MIP_Parameter {

public:

  sa_botage_percent_parameter(const char* AName="", double AValue=0.0, double AMin=-1.0, double AMax=1.0)
  : MIP_Parameter(AName,AValue,AMin,AMax) {
  }

  bool valueToText(double value, char *display, uint32_t size) override {
    double v = value;//powf(0.5,-value);
    v *= 100.0;
    //sprintf(display,"%.2f",v);
    uint32_t iv = MIP_Trunc(v);
    sprintf(display,"%i",iv);
    return true;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void sa_botage_init_parameters(MIP_Plugin* APlugin) {

  /* PAR_NUM_BEATS  */              APlugin->appendParameter(new MIP_IntParameter(  "Beats",                4,          1, 8 ));
  /* PAR_NUM_SLICES */              APlugin->appendParameter(new MIP_IntParameter(  "Slices",               2,          1, 8 ));

  // ----- slice

  /* PAR_RANGE_PROB */              APlugin->appendParameter(new MIP_Parameter(     "Range Prob",           0.25,       0, 1   ));
  /* PAR_RANGE_SLICE_COUNT  */      APlugin->appendParameter(new MIP_IntParameter(  "Range Slice Count",    0b00001111, 1, 255 ));
  /* PAR_RANGE_LOOP_COUNT  */       APlugin->appendParameter(new MIP_IntParameter(  "Range Loop Count",     0b00001111, 1, 255 ));

  // ----- env

  /* PAR_LOOP_ENV_ATTACK */         APlugin->appendParameter(new MIP_Parameter(     "Loop Env Attack",      1,          0, 25 ));
  /* PAR_LOOP_ENV_DECAY */          APlugin->appendParameter(new MIP_Parameter(     "Loop Env Decay",       24/*1*/,    0, 25 ));   // !!!!!
  /* PAR_SLICE_ENV_ATTACK */        APlugin->appendParameter(new MIP_Parameter(     "Slice Env Attack",     0,          0, 1 ));
  /* PAR_SLICE_ENV_DECAY */         APlugin->appendParameter(new MIP_Parameter(     "Slice Env Decay",      1/*0*/,     0, 1 ));    // !!!!!

//  // ----- fx filter

  /* PAR_FX_FILTER_PROB */          APlugin->appendParameter(new MIP_Parameter(     "FX Filter Prob",       0,          0, 1 ));
  /* PAR_FX_FILTER_TYPE */          APlugin->appendParameter(new MIP_IntParameter(  "FX Filter Type",       1,          0, 2 ));
  /* PAR_FX_FILTER_FREQ */          APlugin->appendParameter(new MIP_Parameter(     "FX Filter Frequency",  0.5,        0, 1 ));
  /* PAR_FX_FILTER_BW */            APlugin->appendParameter(new MIP_Parameter(     "FX Filter BandWidth",  0.5,        0, 1 ));

  // ----- loop size

  /* PAR_PROB_SIZE_PROB_RANGE */    APlugin->appendParameter(new MIP_Parameter(     "Size Prob Range",      0.25,       0, 1 ));
  /* PAR_PROB_SIZE_MIN_RANGE */     APlugin->appendParameter(new MIP_Parameter(     "Size Min Range",      -0.5,       -1, 1 ));
  /* PAR_PROB_SIZE_MAX_RANGE */     APlugin->appendParameter(new MIP_Parameter(     "Size Max Range",       0.5,         -1, 1 ));

  /* PAR_PROB_SIZE_PROB_LOOP */     APlugin->appendParameter(new MIP_Parameter(     "Size Prob Loop",       0.25,       0, 1 ));
  /* PAR_PROB_SIZE_MIN_LOOP */      APlugin->appendParameter(new MIP_Parameter(     "Size Min Loop",       -0.5,       -1, 1 ));
  /* PAR_PROB_SIZE_MAX_LOOP */      APlugin->appendParameter(new MIP_Parameter(     "Size Max Loop",        0.5,       -1, 1 ));

  // ----- loop speed

  /* PAR_PROB_SPEED_PROB_RANGE */   APlugin->appendParameter(new MIP_Parameter(     "Speed Prob Range",     0.25,       0, 1 ));
  /* PAR_PROB_SPEED_MIN_RANGE */    APlugin->appendParameter(new MIP_Parameter(     "Speed Min Range",     -0.5,       -1, 1 ));
  /* PAR_PROB_SPEED_MAX_RANGE */    APlugin->appendParameter(new MIP_Parameter(     "Speed Max Range",      0.5,       -1, 1 ));

  /* PAR_PROB_SPEED_PROB_LOOP */    APlugin->appendParameter(new MIP_Parameter(     "Speed Prob Loop",      0.25,       0, 1 ));
  /* PAR_PROB_SPEED_MIN_LOOP */     APlugin->appendParameter(new MIP_Parameter(     "Speed Min Loop",      -0.5,       -1, 1 ));
  /* PAR_PROB_SPEED_MAX_LOOP */     APlugin->appendParameter(new MIP_Parameter(     "Speed Max Loop",       0.5,       -1, 1 ));

  // ----- loop offset

  /* PAR_PROB_OFFSET_PROB_RANGE */  APlugin->appendParameter(new MIP_Parameter(     "Offset Prob Range",    0.25,       0, 1 ));
  /* PAR_PROB_OFFSET_MIN_RANGE */   APlugin->appendParameter(new MIP_Parameter(     "Offset Min Range",    -0.5,       -1, 1 ));
  /* PAR_PROB_OFFSET_MAX_RANGE */   APlugin->appendParameter(new MIP_Parameter(     "Offset Max Range",     0.5,       -1, 1 ));

  /* PAR_PROB_OFFSET_PROB_LOOP */   APlugin->appendParameter(new MIP_Parameter(     "Offset Prob Loop",     0.25,       0, 1 ));
  /* PAR_PROB_OFFSET_MIN_LOOP */    APlugin->appendParameter(new MIP_Parameter(     "Offset Min Loop",     -0.5,       -1, 1 ));
  /* PAR_PROB_OFFSET_MAX_LOOP */    APlugin->appendParameter(new MIP_Parameter(     "Offset Max Loop",      0.5,       -1, 1 ));

  // ----- loop reverse

  /* PAR_PROB_REVERSE_PROB_RANGE */ APlugin->appendParameter(new MIP_Parameter(     "Reverse Prob Range",   0.25,       0, 1 ));
  /* PAR_PROB_REVERSE_PROB_LOOP */  APlugin->appendParameter(new MIP_Parameter(     "Reverse Prob Loop",    0.25,       0, 1 ));

  // ----- fx

  /* PAR_PROB_FX_PROB_RANGE */      APlugin->appendParameter(new MIP_Parameter(     "FX Prob Range",        0.25,       0, 1 ));
  /* PAR_PROB_FX_MIN_RANGE */       APlugin->appendParameter(new MIP_Parameter(     "FX Min Range",        -0.5,       -1, 1 ));
  /* PAR_PROB_FX_MAX_RANGE */       APlugin->appendParameter(new MIP_Parameter(     "FX Max Range",         0.5,       -1, 1 ));

  /* PAR_PROB_FX_PROB_LOOP */       APlugin->appendParameter(new MIP_Parameter(     "FX Prob Loop",         0.25,       0, 1 ));
  /* PAR_PROB_FX_MIN_LOOP */        APlugin->appendParameter(new MIP_Parameter(     "FX Min Loop",         -0.5,       -1, 1 ));
  /* PAR_PROB_FX_MAX_LOOP */        APlugin->appendParameter(new MIP_Parameter(     "FX Max Loop",          0.5,       -1, 1 ));

}

//----------------------------------------------------------------------
#endif


#if 0


#include "mip.h"


//----------------------------------------------------------------------

const clap_param_info_t sa_botage_params[SA_BOTAGE_PARAM_COUNT] = {



};


#endif // 0
