#ifndef mip_gdi_surface_included
#define mip_gdi_surface_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/base/mip_base_surface.h"
#include "gui/mip_paint_source.h"
#include "gui/mip_paint_target.h"
#include "gui/gdi/mip_gdi.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_GdiSurface
: public MIP_BaseSurface
, public MIP_PaintSource
, public MIP_PaintTarget {

//------------------------------
private:
//------------------------------

//  HDC       MHandle = nullptr;
  HBITMAP   MHandle = nullptr;
  int32_t   MWidth  = 0;
  int32_t   MHeight = 0;
  int32_t   MDepth  = 0;

//------------------------------
public:
//------------------------------

  /*
    https://msdn.microsoft.com/en-us/library/windows/desktop/dd183488%28v=vs.85%29.aspx
    CreateCompatibleBitmap
    This bitmap can be selected into any memory device context that is
    compatible with the original device.
    Note: When a memory device context is created, it initially has a 1-by-1
    monochrome bitmap selected into it. If this memory device context is used
    in CreateCompatibleBitmap, the bitmap that is created is a monochrome
    bitmap. To create a color bitmap, use the HDC that was used to create the
    memory device context, as shown in the following code:
      HDC memDC = CreateCompatibleDC ( hDC );
      HBITMAP memBM = CreateCompatibleBitmap ( hDC, nWidth, nHeight );
      SelectObject ( memDC, memBM );
  */

  /*
    ADepth
      1   monochrome
      8   greyscale
      16
      24
      32  rgba
  */

  // ATarget = window

  MIP_GdiSurface(MIP_PaintTarget* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : MIP_BaseSurface(ATarget,AWidth,AHeight,ADepth) {
    //HDC tempdc = GetDC(nullptr); // dc for the entire screen
    if (ATarget->tgtIsWindow()) {
      //HWND hwnd = ATarget->tgtGetHwnd();
      //HDC tempdc = GetDC(hwnd);
      HDC tempdc = ATarget->tgtGetHDC();
      MWidth  = AWidth;
      MHeight = AHeight;
      if (ADepth == 0) {

        MDepth  = GetDeviceCaps(tempdc,BITSPIXEL) * GetDeviceCaps(tempdc,PLANES);

        // The CreateCompatibleBitmap function creates a bitmap compatible with
        // the device that is associated with the specified device context.

        MHandle = CreateCompatibleBitmap(tempdc,MWidth,MHeight);

        // The CreateDIBSection function creates a DIB that applications can
        // write to directly. The function gives you a pointer to the location
        // of the bitmap bit values.

        //HBITMAP CreateDIBSection(
        //  [in]  HDC              hdc,
        //  [in]  const BITMAPINFO *pbmi,
        //  [in]  UINT             usage,       // DIB_RGB_COLORS
        //  [out] VOID             **ppvBits,   // &ourBuffer
        //  [in]  HANDLE           hSection,    // NULL, If hSection is NULL, the system allocates memory for the DIB
        //  [in]  DWORD            offset       // 0
        //);

      }
      else {

        MDepth  = ADepth;

        // The CreateBitmap function creates a device-dependent bitmap.

        MHandle = CreateBitmap(
          MWidth,   // nWidth,
          MHeight,  // nHeight,
          1,        // cPlanes,
          MDepth,   // cBitsPerPel,
          nullptr   // VOID* lpvBits
        );

      }
      //ReleaseDC(hwnd,tempdc);
    }
    MIP_Assert(MHandle);

  }

  //----------

  /*
    see also:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183485%28v=vs.85%29.aspx
    MHandle = CreateBitmap(
      MWidth, MHeight,
      1,      // planes
      32,     // bits per pixel
      S3_NULL // data
    );
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183486%28v=vs.85%29.aspx
    MHandle = CreateBitmapIndirect( ABitmap->hbitmap() );
  */

  //----------

  virtual ~MIP_GdiSurface() {
    DeleteObject(MHandle);
  }

//------------------------------
public: // paint target
//------------------------------

  // src

  bool    srcIsSurface()  override { return true; }
  int32_t srcGetWidth()   override { return MWidth; }
  int32_t srcGetHeight()  override { return MHeight; }
  int32_t srcGetDepth()   override { return MDepth; }
  HBITMAP srcGetHBITMAP() override { return MHandle; }

  // tgt

  bool    tgtIsSurface()  override { return true; }
  int32_t tgtGetWidth()   override { return MWidth; }
  int32_t tgtGetHeight()  override { return MHeight; }
  int32_t tgtGetDepth()   override { return MDepth; }
  HBITMAP tgtGetHBITMAP() override { return MHandle; }

};

//----------------------------------------------------------------------
#endif
