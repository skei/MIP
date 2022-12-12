#ifndef mip_image_widget_included
#define mip_image_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_ImageWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool        MDrawImage        = true;
  MIP_DRect   MImageOffset      = MIP_DRect(0);

//  double      MTextSize       = 14.0;
//  MIP_Color   MTextColor      = MIP_COLOR_DARK_GRAY;
//  const char* MText           = "";
//  uint32_t    MTextAlignment  = MIP_TEXT_ALIGN_CENTER;

  bool MInitialized     = false;

  int32_t   MImage      = -1;
  void*     MBuffer     = nullptr;
  uint32_t  MBufferSize = 0;

  double    MAlpha      = 1.0;
  double    MAngle      = 0.0;

//------------------------------
public:
//------------------------------

  MIP_ImageWidget(MIP_DRect ARect, void* ABuffer, uint32_t ASize)
  : MIP_PanelWidget(ARect) {

    setFillBackground(false);
    //setBackgroundColor(MIP_COLOR_MAGENTA);

    MBuffer = ABuffer;
    MBufferSize = ASize;
  }

  //----------

  virtual ~MIP_ImageWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawImage(bool ADraw=true)            { MDrawImage = ADraw; }
  virtual void setImage(char* ABuffer, uint32_t ASize)  { MBuffer = ABuffer; MBufferSize = ASize; }
  virtual void setImageOffset(MIP_DRect AOffset)        { MImageOffset = AOffset; }
  virtual void setAlpha(double AAlpha)                  { MAlpha = AAlpha; }
  virtual void setAngle(double AAngle)                  { MAlpha = AAngle; }

//------------------------------
public:
//------------------------------

//  void open(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) override {
//    MIP_PRINT;
//    MIP_PanelWidget::open(AOwnerWindow,ARecursive);
//    if (!MInitialized) {
//      MIP_Window* window = (MIP_Window*)AOwnerWindow;
//      MIP_Painter* painter = window->getWindowPainter();
//      MImage = painter->loadImage(MBuffer,MBufferSize);
//      MInitialized = true;
//    }
//  }

  void close(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) override {
    if (MInitialized) {
      MIP_Window* window = (MIP_Window*)AOwnerWindow;
      MIP_Painter* painter = window->getWindowPainter();
      painter->deleteImage(MImage);
      MInitialized = false;
    }
    MIP_PanelWidget::close(AOwnerWindow,ARecursive);
  }


//------------------------------
public:
//------------------------------

  virtual void drawImage(MIP_PaintContext* AContext) {

    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();

    if (!MInitialized) {
      MImage = painter->loadImage(MBuffer,MBufferSize);
      MInitialized = true;
    }

    painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
    painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    if (MDrawImage) drawImage(AContext);
    paintChildWidgets(AContext);
    if (MDrawBorder) drawBorder(AContext);

  }

  //----------




};

//----------------------------------------------------------------------
#endif



