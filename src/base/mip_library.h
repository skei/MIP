#ifndef mip_library_included
#define mip_library_included
//----------------------------------------------------------------------

#include "mip.h"

//----------------------------------------------------------------------

class MIP_Library {

//------------------------------
private:
//------------------------------

  void* MLibHandle = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Library() {
  }

  //----------

  virtual ~MIP_Library() {
  }

//------------------------------
public:
//------------------------------

  bool loadLib(const char* APath) {
  //MLibHandle = dlopen();
    return false;
  }

  //----------

  void closeLib() {
    //dlclose(MLibHandle);
  }

  //----------

  void* findLibSymbol(const char* ASymbol) {
    // dlsym
    return nullptr;
  }

};

//----------------------------------------------------------------------
#endif
