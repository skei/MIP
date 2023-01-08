#ifndef mip_parameter_manager_included
#define mip_parameter_manager_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_editor.h"
#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ParameterManager {

//------------------------------
public:
//------------------------------

  void appendParameter(MIP_Parameter* AParameter) {
  }

  void appendClapParameters(uint32_t ACount, clap_param_info_t* AInfo) {
  }

  uint32_t getNumParameters() {
    return 0;
  }

  MIP_Parameter* getParameter(uint32_t AIndex) {
    return nullptr;
  }

  double getParameterValue(uint32_t AIndex) {
    return 0.0;
  }

  double getParameterModulation(uint32_t AIndex) {
    return 0.0;
  }

  void setParameterValue(uint32_t AIndex, double AValue) {
  }

  void setParameterModulation(uint32_t AIndex, double AValue) {
  }

  void updateEditor(MIP_Editor* AEditor) {
  }

//----------

};

//----------------------------------------------------------------------
#endif
