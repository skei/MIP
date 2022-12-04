#ifndef mip_parameter_included
#define mip_parameter_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"

//----------

class MIP_Widget;
class MIP_Parameter;
typedef MIP_Array<MIP_Parameter*> MIP_ParameterArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Parameter {

//------------------------------
protected:
//------------------------------

  clap_param_info_t MParamInfo = {
    /*.id             =*/ 0,
    /*.flags          =*/ CLAP_PARAM_IS_AUTOMATABLE,
    /*.cookie         =*/ this,
    /*.name           =*/ "param",
    /*.module         =*/ "",
    /*.min_value      =*/ 0.0,
    /*.max_value      =*/ 1.0,
    /*.default_value  =*/ 0.0
  };

  MIP_Widget*       MWidget           = nullptr;
  int32_t           MIndex            = -1;
  double            MValue            = 0.0;
  double            MModulation       = 0.0;

  double            MValueTarget      = 0.0;
  double            MValueFactor      = 0.0;
  double            MValueSmoothed    = 0.0;

  double            MModulationTarget = 0.0;
  double            MModulationSmooth = 0.0;
  double            MModulationFactor = 0.0;

  char              MValueText[256]   = {0};


//------------------------------
public:
//------------------------------

  MIP_Parameter(const char* AName, double AValue=0.0) {
    setName(AName);
    setValue(AValue);
    setDefValue(AValue);
  }

  //----------

  virtual ~MIP_Parameter() {
  }

//------------------------------
public:
//------------------------------

  virtual void        setWidget(MIP_Widget* AWidget)      { MWidget = AWidget; }
  virtual void        setIndex(int32_t AIndex)            { MIndex = AIndex; }

  virtual void        setValue(double AValue)             { MValue = AValue; }
  virtual void        setValueTarget(double AValue)       { MValueTarget = AValue; }
  virtual void        setValueFactor(double AValue)       { MValueFactor = AValue; }

  virtual void        setModulation(double AValue)        { MModulation = AValue; }
  virtual void        setModulationTarget(double AValue)  { MModulationTarget = AValue; }
  virtual void        setModulationFactor(double AValue)  { MModulationFactor = AValue; }

  // expressions?

  virtual void        setId(uint32_t AId)                 { MParamInfo.id = AId; }
  virtual void        setFlags(uint32_t AFlags)           { MParamInfo.flags = AFlags; }
  virtual void        setName(const char* AName)          { strncpy(MParamInfo.name,AName,CLAP_NAME_SIZE-1); }
  virtual void        setModule(const char* AModule)      { strncpy(MParamInfo.module,AModule,CLAP_PATH_SIZE-1); }
  virtual void        setDefValue(double AValue)          { MParamInfo.default_value = AValue; }
  virtual void        setMinValue(double AValue)          { MParamInfo.min_value = AValue; }
  virtual void        setMaxValue(double AValue)          { MParamInfo.max_value = AValue; }

//------------------------------
public:
//------------------------------

  virtual MIP_Widget* getWidget()           { return MWidget; }
  virtual int32_t     getIndex()            { return MIndex; }
  virtual double      getValue()            { return MValue; }
  virtual double      getValueSmoothed()    { return MValueSmoothed; }
  virtual double      getModulation()       { return MModulation; }
  virtual double      getModulationSmooth() { return MModulationSmooth; }

  virtual uint32_t    getId()               { return MParamInfo.id; }
  virtual uint32_t    getFlags()            { return MParamInfo.flags; }
  virtual const char* getName()             { return MParamInfo.name; }
  virtual const char* getModule()           { return MParamInfo.module; }
  virtual double      getDefValue()         { return MParamInfo.default_value; }
  virtual double      getMinValue()         { return MParamInfo.min_value; }
  virtual double      getMaxValue()         { return MParamInfo.max_value; }

//------------------------------
public:
//------------------------------

  //TODO: check size, etc..

  virtual bool valueToText(double value, char *display, uint32_t size) {
    sprintf(display,"%.3f",value);
    return true;
  }

  //----------

  virtual bool textToValue(const char *display, double *value) {
    *value = atoi(display);
    return false;
  }

  //----------

  virtual double normalizeValue(double AValue) {
    double range = MParamInfo.max_value - MParamInfo.min_value;
    if (range > 0.0) {
      return (AValue - MParamInfo.min_value) / range;
    }
    return 0.0;
  }

  //----------

  virtual double denormalizeValue(double AValue) {
    double range = MParamInfo.max_value - MParamInfo.min_value;
    return MParamInfo.min_value + (AValue * range);
  }

};

//----------------------------------------------------------------------
//
// int
//
//----------------------------------------------------------------------

class MIP_IntParameter
: public MIP_Parameter {

//------------------------------
public:
//------------------------------

  MIP_IntParameter(const char* AName, double AValue=0.0)
  : MIP_Parameter(AName,AValue) {
  }

  //----------

  virtual ~MIP_IntParameter() {
  }

//------------------------------
public:
//------------------------------

  double getValue() override {
    return (int32_t)MValue;
  }

  //----------

  bool valueToText(double value, char *display, uint32_t size) override {
    sprintf(display,"%i",(int)value);
    return true;
  }

};

//----------------------------------------------------------------------
//
// text
//
//----------------------------------------------------------------------

class MIP_TextParameter
: public MIP_IntParameter {

//------------------------------
private:
//------------------------------

  const char** MText = nullptr;

//------------------------------
public:
//------------------------------

  MIP_TextParameter(const char* AName, double AValue, const char** AText)
  : MIP_IntParameter(AName,AValue) {
    MText = AText;
  }

  //----------

  virtual ~MIP_TextParameter() {
  }

//------------------------------
public:
//------------------------------

  bool valueToText(double value, char *display, uint32_t size) override {
    uint32_t index = (int)value;
    if (index <= MParamInfo.max_value) {
      strcpy(display,MText[index]);
    }
    else {
      strcpy(display,"error");
    }
    return true;
  }

};

//----------------------------------------------------------------------

/*
  db
  hz, freq
  pow
*/

//----------------------------------------------------------------------
#endif