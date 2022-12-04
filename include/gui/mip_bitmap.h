#ifndef mip_bitmap_included
#define mip_bitmap_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_paint_source.h"
#include "gui/mip_paint_target.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Bitmap
: public MIP_PaintSource
, public MIP_PaintTarget {

//------------------------------
private:
//------------------------------

  uint32_t  MWidth      = 0;
  uint32_t  MHeight     = 0;
  uint32_t  MDepth      = 0;
  uint32_t  MStride     = 0;
  uint32_t* MBuffer     = nullptr;
  bool      MAllocated  = false;

//------------------------------
public:
//------------------------------

  MIP_Bitmap() {
  }

  //----------

  virtual ~MIP_Bitmap() {
    if (MAllocated && MBuffer) free(MBuffer);
  }

//------------------------------
public: // paint src/tgt
//------------------------------

  bool        tgtIsBitmap()   override { return true; }
  int32_t     tgtGetWidth()   override { return MWidth; }
  int32_t     tgtGetHeight()  override { return MHeight; }
  int32_t     tgtGetDepth()   override { return MDepth; }
  int32_t     tgtGetStride()  override { return MStride; }
  uint32_t*   tgtGetBuffer()  override { return MBuffer; }

  bool      srcIsBitmap()   override { return true; }
  int32_t   srcGetWidth()   override { return MWidth; }
  int32_t   srcGetHeight()  override { return MHeight; }
  int32_t   srcGetDepth()   override { return MDepth; }
  int32_t   srcGetStride()  override { return MStride; }
  uint32_t* srcGetBuffer()  override { return MBuffer; }

//------------------------------
public:
//------------------------------

  int32_t   getWidth()   { return MWidth; }
  int32_t   getHeight()  { return MHeight; }
  int32_t   getDepth()   { return MDepth; }
  int32_t   getStride()  { return MStride; }
  uint32_t* getBuffer()  { return MBuffer; }

  uint32_t* getPixelPtr(uint32_t x, uint32_t y) {
    return MBuffer + (y * MStride) + x;
  }

};

//----------------------------------------------------------------------
#endif
