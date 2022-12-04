#ifndef mip_gdi_painter_included
#define mip_gdi_painter_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/base/mip_base_painter.h"
#include "gui/gdi/mip_gdi.h"
#include "gui/mip_paint_target.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_GdiPainter
: public MIP_BasePainter {

//------------------------------
private:
//------------------------------

  HDC               MHandle         = nullptr;

  HPEN              MPenHandle      = nullptr;
  HBRUSH            MBrushHandle    = nullptr;
  HFONT             MFontHandle     = nullptr;
  //HBITMAP         MBitmapHandle   = nullptr;

  HGDIOBJ           MDefaultPen     = nullptr;
  HGDIOBJ           MDefaultBrush   = nullptr;
  HGDIOBJ           MDefaultFont    = nullptr;
  //HGDIOBJ         MDefaultBitmap  = nullptr;

  HPEN              MNullPen        = nullptr;
  HBRUSH            MNullBrush      = nullptr;

//------------------------------
protected:
//------------------------------

  MIP_PaintTarget*  MPaintTarget    = nullptr;

  MIP_Color         MDrawColor      = 1.0;
  MIP_Color         MFillColor      = 0.5;
  MIP_Color         MTextColor      = 0.0;

  double            MLineWidth      = 1.0;
  double            MTextSize       = 10.0;
  const char*       MTextFontName   = "Arial";

//------------------------------
public:
//------------------------------

  MIP_GdiPainter(MIP_PaintTarget* ATarget)
  : MIP_BasePainter(ATarget) {
    MPaintTarget = ATarget;

    if (ATarget->tgtIsWindow()) {
      MHandle  = ATarget->tgtGetHDC();
    }
    else if (ATarget->tgtIsSurface()) {
      HDC tempdc = GetDC(0);
      MHandle = CreateCompatibleDC(tempdc);
      MIP_Assert(MHandle);
      ReleaseDC(0,tempdc);
      HGDIOBJ bitmap = ATarget->tgtGetHBITMAP();
      MIP_Assert(bitmap);
      SelectObject(MHandle,bitmap);
      //HGDIOBJ old_bitmap = SelectObject(MHandle,bitmap);
      //DeleteObject(old_bitmap);
    }
    MIP_Assert(MHandle);

    //MPenHandle = CreatePen(PS_SOLID,(int)MLineWidth,MIP_GdiColor(MDrawColor));
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = MIP_GdiColor(MDrawColor);
    MPenHandle = ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, MLineWidth, &lb, 0, nullptr);

    MDefaultPen = SelectObject(MHandle,MPenHandle);

    MBrushHandle = CreateSolidBrush(MIP_GdiColor(MFillColor));
    MDefaultBrush = SelectObject(MHandle,MBrushHandle);

    MFontHandle = loadFont(MTextFontName,MTextSize);
    MDefaultFont = SelectObject(MHandle,MFontHandle);

    //MDefaultBitmap = SelectObject(AHandle,MBitmapHandle);

    MNullPen = CreatePen(PS_NULL,0,0);

    LOGBRUSH lbrush;
    lbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ..
    lbrush.lbColor = 0;       // ignored if null
    lbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
    MNullBrush = CreateBrushIndirect(&lbrush);

  }

  //----------

  // It is not necessary (but it is not harmful) to delete stock objects
  // by calling DeleteObject.
  //
  // Class and private DCs do not have to be released.

  virtual ~MIP_GdiPainter() {

    HGDIOBJ pen = SelectObject(MHandle,MDefaultPen);
    DeleteObject(pen);

    HGDIOBJ brush = SelectObject(MHandle,MDefaultBrush);
    DeleteObject(brush);

    HGDIOBJ font = SelectObject(MHandle,MDefaultFont);
    DeleteObject(font);

    //HGDIObj bitmap = SelectObject(MHandle,MDefaultBitmap);
    //DeleteObject(bitmap);

    DeleteObject(MNullPen);
    DeleteObject(MNullBrush);

    if (MPaintTarget->tgtIsWindow()) {
      HWND hwnd = MPaintTarget->tgtGetHWND();
      ReleaseDC(hwnd,MHandle);
    }
    else if (MPaintTarget->tgtIsSurface()) {
      //ReleaseDC(0,MHandle);
      DeleteDC(MHandle);
    }

  }

//------------------------------
public:
//------------------------------

  void flush(void) override {
    GdiFlush();
  }

//------------------------------
public: // clipping
//------------------------------

  // http://www.codeproject.com/Articles/2095/A-Guide-to-WIN32-Clipping-Regions

  /*
    CreateRectRgn
    http://msdn.microsoft.com/en-us/library/windows/desktop/dd183514%28v=vs.85%29.aspx

    Regions created by the Create<shape>Rgn methods (such as CreateRectRgn and
    CreatePolygonRgn) only include the interior of the shape; the shape's outline
    is excluded from the region. This means that any point on a line between two
    sequential vertices is not included in the region. If you were to call
    PtInRegion for such a point, it would return zero as the result.
    When you no longer need the HRGN object, call the DeleteObject function to
    delete it.

    SelectClipRgn
    http://msdn.microsoft.com/en-us/library/windows/desktop/dd162955%28v=vs.85%29.aspx

    The SelectClipRgn function selects a region as the current clipping region for
    the specified device context.
    To remove a device-context's clipping region, specify a NULL region handle.
    Only a copy of the selected region is used. The region itself can be selected
    for any number of other device contexts or it can be deleted.
    "When you call SelectClipRgn() within a BeginPaint()/EndPaint() block in an
    application's WM_PAINT case, the maximum size to which you can set your
    clipping region is the size of the update region of your paint structure. In
    other words you can use SelectClipRgn() to shrink your update region but not
    to grow it".

    The BeginPaint function automatically sets the clipping region of the device
    context to exclude any area outside the update region.

    http://books.google.no/books?id=-O92IIF1Bj4C&pg=PA427&lpg=PA427&dq=SelectClipRgn&source=bl&ots=Sx2GC35hp7&sig=NFtTCTcuzrcBMoHaTJYY998CJww&hl=no&sa=X&ei=3ZvrUpOgJ4bhywOH74DAAg&redir_esc=y#v=onepage&q=SelectClipRgn&f=false

    For a device context returned by BeginPaint, GetDC, or CreateDC, the clipping
    region is NULL

    --------------------------------------------------

    http://msdn.microsoft.com/en-us/library/windows/desktop/dd183439%28v=vs.85%29.aspx

    if you obtain a device context handle from the BeginPaint function, the DC
    contains a predefined rectangular clipping region that corresponds to the
    invalid rectangle that requires repainting. However, when you obtain a device
    context handle by calling the GetDC function with a NULLhWnd parameter, or by
    calling the CreateDC function, the DC does not contain a default clipping
    region.

    ExcludeClipRect
    IntersectClipRect

    invalidate -> GetUpdateRect

  */

  //  virtual // MIP_Painter_Base
  //  void clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  //    //self.noClip;
  //    HRGN rgn = CreateRectRgn( AX1, AY1, AX2+1, AY2+1 );
  //    SelectClipRgn(MHandle,rgn);
  //    DeleteObject(rgn);
  //    //  IntersectClipRect(FDC,ARect.x-0{1}, ARect.y-0{1}, ARect.x2+1, ARect.y2+1 );
  //  }

  void setClip(MIP_DRect ARect) override {
    HRGN rgn = CreateRectRgn(
      ARect.x,//AX1,
      ARect.y,//AY1,
      ARect.x2() + 1,
      ARect.y2() + 1
    );
    SelectClipRgn(MHandle,rgn);
    DeleteObject(rgn);
  }

  //----------

  // To remove a device-context's clipping region, specify a NULL region handle.

  void resetClip() override {
    SelectClipRgn(MHandle,nullptr);
  }

//------------------------------
public: // set, get..
//------------------------------

  /*
    SetDCPenColor function sets the current device context (DC) pen color to
    the specified color value.
    If the function succeeds, the return value specifies the previous DC pen
    color as a COLORREF value. If the function fails, the return value is
    CLR_INVALID.
    The function returns the previous DC_PEN color, even if the stock pen
    DC_PEN is not selected in the DC; however, this will not be used in
    drawing operations until the stock DC_PEN is selected in the DC.
    The GetStockObject function with an argument of DC_BRUSH or DC_PEN can be
    used interchangeably with the SetDCPenColor and SetDCBrushColor
    functions.
  */

  void setDrawColor(MIP_Color color) override {
    MDrawColor = color;

    //HPEN pen = CreatePen(PS_SOLID,MLineWidth,MIP_GdiColor(color));
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = MIP_GdiColor(color);
    HPEN pen = ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, MLineWidth, &lb, 0, nullptr);

    HGDIOBJ old_pen = SelectObject(MHandle,pen);
    DeleteObject(old_pen);

  }

  //----------

  void setFillColor(MIP_Color color) override {
    MFillColor = color;
    HBRUSH brush = CreateSolidBrush(MIP_GdiColor(color));
    HGDIOBJ old_brush = SelectObject(MHandle,brush);
    DeleteObject(old_brush);
  }

  //----------

  void setTextColor(MIP_Color color) override {
    MTextColor = color;
    SetTextColor(MHandle,MIP_GdiColor(color));
  }

  //----------

  void setTextSize(double ASize) override {
    MTextSize = ASize;
    HFONT font = loadFont(MTextFontName,ASize);
    HGDIOBJ old_font = SelectObject(MHandle,font);
    DeleteObject(old_font);
  }

  //----------

  void setLineWidth(double AWidth) override {
    MLineWidth = AWidth;

    //HPEN pen = CreatePen(PS_SOLID | PS_ENDCAP_FLAT,(int)AWidth,MIP_GdiColor(MDrawColor));
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = MIP_GdiColor(MDrawColor);
    HPEN pen = ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, MLineWidth, &lb, 0, nullptr);

    HGDIOBJ old_pen = SelectObject(MHandle,pen);
    DeleteObject(old_pen);
  }

//------------------------------
public: // draw
//------------------------------

  void drawLine(double x1, double y1, double x2, double y2) override {
    MoveToEx(MHandle,x1,y1,0);
    LineTo(MHandle,x2+1,y2);
  }

  //----------

  void drawLines(uint32_t num, double* coords) override {
    if (num >= 2) {
      for (uint32_t i=1; i<num; i++) {
        double x = *coords++;
        double y = *coords++;
        MoveToEx(MHandle,x,y,0);
        x = *coords++;
        y = *coords++;
        LineTo(MHandle,x,y);
      }
    }
  }

  //----------

  void drawLineStrip(uint32_t num, double* coords) override {
    if (num >= 2) {
      double x = *coords++;
      double y = *coords++;
      MoveToEx(MHandle,x,y,0);
      for (uint32_t i=1; i<num; i++) {
        x = *coords++;
        y = *coords++;
        LineTo(MHandle,x,y);
      }
    }
  }

  //----------

  // a1,a2 : start/end angle..
  // 0..PI2, starts art 3 o'clock, counterclockwise

  void drawArc(double cx, double cy, double r, double a1, double a2) override {
    float x1 = cx - r;
    float y1 = cy - r;
    float x2 = x1 + (r * 2.0);
    float y2 = y1 + (r * 2.0);
    float s = fmaxf((x2-x1),(y2-y1));
    float ax1 = cx + (cosf(-a1) * s);
    float ay1 = cy + (sinf(-a1) * s); // negative a -> counter clockwise
    float ax2 = cx + (cosf(-a2) * s);
    float ay2 = cy + (sinf(-a2) * s);
    SetArcDirection(MHandle,AD_COUNTERCLOCKWISE);
    Arc(MHandle,x1,y1,x2,y2,(int)ax1,(int)ay1,(int)ax2,(int)ay2);
  }

  //----------

  void drawRect(double x, double y, double w, double h) override {
    double x2 = x + w - 1;
    double y2 = y + h - 1;
    MoveToEx(MHandle,x, y, 0);
    LineTo(MHandle,  x2,y  );
    LineTo(MHandle,  x2,y2 );
    LineTo(MHandle,  x, y2 );
    LineTo(MHandle,  x, y  );
  }

//------------------------------
public: // fill
//------------------------------

  // +2 because of no borders..
  // we could set border color instead..

  void fillRect(double x, double y, double w, double h) override {
    double x2 = x + w - 1;
    double y2 = y + h - 1;
    HGDIOBJ old_pen = SelectObject(MHandle,MNullPen);
    Rectangle(MHandle,x,y,x2+2,y2+2);
    SelectObject(MHandle,old_pen);
  }

//------------------------------
public: // text
//------------------------------

  void drawText(double x, double y, const char* text) override {
    SetBkMode(MHandle,TRANSPARENT);
    RECT R;
    R.left    = x;
    R.top     = y;
    R.right   = x + 2000;
    R.bottom  = y + 2000;
    int flags = DT_LEFT | DT_TOP;
    DrawText(MHandle,text,-1,&R,flags|DT_NOCLIP|DT_SINGLELINE|DT_NOPREFIX);
  }

//------------------------------
public: // bitmaps
//------------------------------

  void blit(double ADstX, double ADstY, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
    HDC src_dc = CreateCompatibleDC(MHandle);
    HGDIOBJ bitmap = ASource->srcGetHBITMAP();
    SelectObject(src_dc,bitmap);
    BitBlt(MHandle,ADstX,ADstY,ASrcW,ASrcH,src_dc,ASrcX,ASrcY,SRCCOPY);
    DeleteObject(src_dc);
  }

  //----------

  void stretch(double ADstX, double ADstY, double ADstW, double ADstH, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
    HDC src_dc = CreateCompatibleDC(MHandle);
    HGDIOBJ bitmap = ASource->srcGetHBITMAP();
    SelectObject(src_dc,bitmap);
    SetStretchBltMode(MHandle,COLORONCOLOR);
    StretchBlt(MHandle,ADstX,ADstY,ADstW,ADstH,src_dc,ASrcX,ASrcY,ASrcW,ASrcH,SRCCOPY);
    DeleteObject(src_dc);
  }

  //----------

  void blend(double ADstX, double ADstY, double ADstW, double ADstH, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
    HDC src_dc = CreateCompatibleDC(MHandle);
    HGDIOBJ bitmap = ASource->srcGetHBITMAP();
    SelectObject(src_dc,bitmap);
    BLENDFUNCTION blend = {
      AC_SRC_OVER,
      0,
      0,            // whole-surface alpha
      AC_SRC_ALPHA  // AC_SRC_ALPHA = src has premultiplied alpha
    };
    AlphaBlend(MHandle,ADstX,ADstY,ADstW,ADstH,src_dc,ASrcX,ASrcY,ASrcW,ASrcH,blend);
    DeleteObject(src_dc);
  }

//------------------------------
private: //
//------------------------------

  HFONT loadFont(const char* AName, double ASize) {
    LOGFONT lfont;
    memset(&lfont,0,sizeof(lfont));
    strcpy(lfont.lfFaceName,AName);
    lfont.lfHeight = -MulDiv(ASize,GetDeviceCaps(MHandle,LOGPIXELSY),72); // 8
    return CreateFontIndirect(&lfont);
  }

  //----------

  void getTextBounds(double x, double y, const char* text, double* bounds) {
    SIZE S;
    GetTextExtentPoint32(MHandle,text,strlen(text),&S);
    bounds[0] = x;
    bounds[1] = y;
    bounds[2] = x + S.cx;
    bounds[3] = y + S.cy;
  }

};

//----------------------------------------------------------------------
#endif
