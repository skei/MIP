#ifndef mip_arguments_included
#define mip_arguments_included
//----------------------------------------------------------------------

#include "mip.h"

class MIP_Arguments {

//------------------------------
public:
//------------------------------

  int     MArgc = 0;
  char**  MArgv = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Arguments() {
  }

  //----------

  MIP_Arguments(int argc, char** argv) {
    init(argc,argv);
  }

  //----------

  ~MIP_Arguments() {
  }

//------------------------------
public:
//------------------------------

  void init(int argc, char** argv) {
    MArgc = argc;
    MArgv = argv;
  }

  //----------

  const char* getArgStr(int i) {
    if ((i < 0) || (i >= MArgc)) return "";
    return MArgv[i];
  }

  //----------

  int getArgInt(int i) {
    if ((i < 0) || (i >= MArgc)) return 0;
    char* e;
    int result = strtol( MArgv[i], &e, 10 ); // base 10
    if (e == MArgv[i]) {
      MIP_Print("Error parsing argument '%s'. expected int\n",MArgv[i]);
      return 0;
    }
    return result;
  }

  //----------

  float getArgFloat(int i) {
    if ((i < 0) || (i >= MArgc)) return 0.0;
    char* e;
    float result = strtof(MArgv[i],&e);
    if (e == MArgv[i]) {
      MIP_Print("Error parsing argument '%s'. expected float\n",MArgv[i]);
      return 0.0;
    }
    return result;
  }

  //----------

  bool isOption(int i) {
    if ((i < 0) || (i >= MArgc)) return false;
    char* str = MArgv[i];
    if (str[0] == '-') return true;
    return false;
  }

  //----------

  int findArg(const char* arg, const char* arg2=nullptr) {
    for (int i=0; i<MArgc; i++) {
      if (strcmp(arg,MArgv[i]) == 0) return i;
      if (arg2) if (strcmp(arg2,MArgv[i]) == 0) return i;
    }
    return -1;
  }

//------------------------------
public:
//------------------------------

  // "-i ./test"

  const char* getValStr(const char* arg, const char* arg2=nullptr) {
    int i = findArg(arg);
    if (arg2 && (i < 0)) i = findArg(arg2);
    if (i >= 0) {
      i += 1;
      if (i < MArgc) {
        return getArgStr(i);
      }
    }
    return "";
  }

  //----------

  // "-b 128"

  int getValInt(const char* arg, const char* arg2=nullptr) {
    int i = findArg(arg);
    if (arg2 && (i < 0)) i = findArg(arg2);
    if (i >= 0) {
      i += 1;
      if (i < MArgc) {
        return getArgInt(i);
      }
    }
    return 0.0;
  }

  //----------

  // "-r 1.5"

  float getValFloat(const char* arg, const char* arg2=nullptr) {
    int i = findArg(arg);
    if (arg2 && (i < 0)) i = findArg(arg2);
    if (i >= 0) {
      i += 1;
      if (i < MArgc) {
        return getArgFloat(i);
      }
    }
    return 0.0;
  }

//------------------------------
public:
//------------------------------

  // "-i test:abc"

  const char* getValAfterStr(const char* arg, char divider, const char* arg2=nullptr) {
    int i = findArg(arg);
    if (arg2 && (i < 0)) i = findArg(arg2);
    if (i >= 0) {
      const char* text = getArgStr(i+1);
      const char* text2 = strchr(text,divider);
      if (text2) {
        text2 += 1;
        return text2;
      }
    }
    return "";
  }

  //----------

  // "-i test:1"

  int getValAfterInt(const char* arg, char divider, const char* arg2=nullptr) {
    int i = findArg(arg);
    if (arg2 && (i < 0)) i = findArg(arg2);
    if (i >= 0) {
      const char* text = getArgStr(i+1);
      const char* text2 = strchr(text,divider);
      if (text2) {
        text2 += 1;
        char* e;
        int result = strtol(text2, &e, 10); // base 10
        if (e == text2) {
          MIP_Print("Error parsing argument '%s'. expected int\n",text2);
          return 0;
        }
        return result;
      }
    }
    return 0;
  }

  //----------

  // "-i test:0.75"

  float getValAfterFloat(const char* arg, char divider, const char* arg2=nullptr) {
    int i = findArg(arg);
    if (arg2 && (i < 0)) i = findArg(arg2);
    if (i >= 0) {
      const char* text = getArgStr(i+1);
      const char* text2 = strchr(text,divider);
      if (text2) {
        text2 += 1;
        char* e;
        float result = strtof(text2, &e); // base 10
        if (e == text2) {
          MIP_Print("Error parsing argument '%s'. expected int\n",text2);
          return 0;
        }
        return result;
      }
    }
    return 0.0;
  }


};

//----------------------------------------------------------------------
#endif
