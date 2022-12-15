#ifndef sa_botage_parameters_included
#define sa_botage_parameters_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/mip_parameter.h"
#include "plugin/mip_plugin.h"

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

  //APlugin->appendParameter( new MIP_Parameter("Gain", 1.0) );
  //APlugin->appendParameter( new MIP_Parameter("Left", 1.0) );
  //APlugin->appendParameter( new MIP_Parameter("Right",1.0) );

  MIP_IntParameter* beats  = new MIP_IntParameter("Beats", 4,1,8);
  APlugin->appendParameter(beats);
  //beats->setValueRange(1,8);

  MIP_IntParameter* slices = new MIP_IntParameter("Subdiv", 2,1,8);
  APlugin->appendParameter(slices);
  //slices->setValueRange(1,8);

  sa_botage_range_parameter* range1 = new sa_botage_range_parameter("%",0,-1,1);
  APlugin->appendParameter(range1);

  sa_botage_range_parameter* range2 = new sa_botage_range_parameter("%",0,-1,1);
  APlugin->appendParameter(range2);

  sa_botage_percent_parameter* percent1 = new sa_botage_percent_parameter("%",0,0,1);
  APlugin->appendParameter(percent1);

  sa_botage_percent_parameter* percent2 = new sa_botage_percent_parameter("%",0,0,1);
  APlugin->appendParameter(percent2);

}

//----------------------------------------------------------------------
#endif
