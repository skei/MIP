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

  bool        MInitialized    = false;
  bool        MDrawImage      = true;
  MIP_DRect   MImageOffset    = MIP_DRect(0);
  int32_t     MImage          = -1;
  void*       MBuffer         = nullptr;
  uint32_t    MBufferSize     = 0;
  const char* MFilename       = nullptr;
  double      MAlpha          = 1.0;
  double      MAngle          = 0.0;
  bool        MMipMapped      = false;

//------------------------------
public:
//------------------------------

  MIP_ImageWidget(MIP_DRect ARect, void* ABuffer, uint32_t ASize)
  : MIP_PanelWidget(ARect) {
    setFillBackground(false);
    MBuffer = ABuffer;
    MBufferSize = ASize;
  }

  //----------

  MIP_ImageWidget(MIP_DRect ARect, const char* AFilename)
  : MIP_PanelWidget(ARect) {
    setFillBackground(false);
    MFilename = AFilename;
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
      if (MBuffer && (MBufferSize > 0)) {
        MImage = painter->loadImage(MBuffer,MBufferSize);
        MInitialized = true;
      }
      else if (MFilename) {
        MImage = painter->loadImage(MFilename);
        MInitialized = true;
      }
    }

    if (MMipMapped) {
    }
    else {
      painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
      painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
    }
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

};

//----------------------------------------------------------------------
#endif



