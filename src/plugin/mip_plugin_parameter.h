#ifndef mip_plugin_parameter_included
#define mip_plugin_parameter_included
//----------------------------------------------------------------------

#include "mip.h"

//#include <vector>
class MIP_PluginParameter;
typedef std::vector<MIP_PluginParameter*> MIP_PluginParameters;

//----------------------------------------------------------------------

class MIP_PluginParameter {

  friend class MIP_PluginDescriptor;

//------------------------------
private:
//------------------------------

  int32_t     MIndex      = -1;

//------------------------------
protected:
//------------------------------

  const char* MName       = "";
  const char* MLabel      = "";
  float       MDefValue   = 0.0;
  float       MMinValue   = 0.0;
  float       MMaxValue   = 0.0;
  uint32_t    MNumSteps   = 0;

  bool MIsHidden      = false;
  bool MIsReadOnly    = false;
  bool MCanAutomate   = true;
  bool MCanModulate   = false;

//------------------------------
public:
//------------------------------

  MIP_PluginParameter(const char* AName="", float ADefValue=0.0, float AMinValue=0.0, float AMaxValue=1.0, uint32_t ANumSteps=0) {
    MName = AName;
    MDefValue = ADefValue;
    MMinValue = AMinValue;
    MMaxValue = AMaxValue;
    MNumSteps = ANumSteps;
  }

  //----------

  virtual ~MIP_PluginParameter() {
  }

//------------------------------
public:
//------------------------------

  int32_t             getIndex()        { return MIndex; }

  virtual const char* getName()         { return MName; }
  virtual const char* getLabel()        { return MLabel; }
  virtual float       getDefaultValue() { return MDefValue; }
  virtual float       getMinValue()     { return MMinValue; }
  virtual float       getMaxValue()     { return MMaxValue; }
  virtual uint32_t    getNumSteps()     { return MNumSteps; }

  virtual bool isHidden()     { return MIsHidden; }
  virtual bool isReadOnly()   { return MIsReadOnly; }
  virtual bool canAutomate()  { return MCanAutomate; }
  virtual bool canModulate()  { return MCanModulate; }


  //----------

  virtual float from01(float AValue) {
    return AValue;
  }

  //----------

  virtual float to01(float AValue) {
    return AValue;
  }

  //----------

  virtual const char* getDisplayText(float AValue, char* ABuffer, uint32_t APrecision=2) {
    char format[5];
    format[0] = '%';
    format[1] = '.';
    format[2] = 48 + APrecision;
    format[3] = 'f';
    format[4] = 0;
    sprintf(ABuffer,format,AValue);
    return ABuffer;
  }

};

//----------------------------------------------------------------------
#endif
