#ifndef sa_botage_parameters_included
#define sa_botage_parameters_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/mip_plugin.h"

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

}

//----------------------------------------------------------------------
#endif
