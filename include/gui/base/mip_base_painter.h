#ifndef mip_base_painter_included
#define mip_base_painter_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_paint_source.h"
#include "gui/mip_paint_target.h"

class MIP_BasePainter {
public:
  MIP_BasePainter(MIP_PaintTarget* ATarget) {}
  virtual ~MIP_BasePainter() {}
public:
  virtual void flush(void) {}

  virtual bool makeCurrent() { return false; }
  virtual bool resetCurrent() { return false; }
  virtual void swapBuffers() {}

  virtual void setClip(MIP_DRect ARect) {}
  virtual void resetClip() {}

  virtual void setDrawColor(MIP_Color color) {}
  virtual void setFillColor(MIP_Color color) {}
  virtual void setTextColor(MIP_Color color) {}

  virtual bool getTextBounds(const char* AText, double* ABounds) { return false; }

  virtual void setTextSize(double ASize) {}
  virtual void setLineWidth(double AWidth) {}
  virtual void drawLine(double x1, double y1, double x2, double y2) {}
  virtual void drawLines(uint32_t num, double* coords) {}
  virtual void drawLineStrip(uint32_t num, double* coords) {}
  virtual void drawArc(double cx, double cy, double r, double a1, double a2) {}
  virtual void drawRect(double x, double y, double w, double h) {}
  virtual void fillRect(double x, double y, double w, double h) {}

  //virtual void fillRectImage(double x, double y, double w, double h, int32_t AImage, double iw, double ih) {}

  virtual void drawText(double x, double y, const char* text) {}
  virtual void blit(double ADstX, double ADstY, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) {}
  virtual void stretch(double ADstX, double ADstY, double ADstW, double ADstH, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) {}
  virtual void blend(double ADstX, double ADstY, double ADstW, double ADstH, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) {}

  virtual void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {} // viewport
  virtual void endPaint() {}

  virtual void    setFillGradient(double AX1, double AY1, double AX2, double AY2, MIP_Color AColor1, MIP_Color AColor2) {}

  virtual int32_t loadFont(const char* ANAme, const char* AFilename) { return -1; }
  virtual int32_t loadFont(const char* ANAme, void* ABuffer, uint32_t ASize) { return -1; }
  virtual void    deleteFont(int32_t AFont) {}
  virtual void    selectFont(int32_t AFont) {}

  virtual int32_t loadImage(const char* AFilename) { return -1; }
  virtual int32_t loadImage(void* ABuffer, uint32_t ASize) { return -1; }
  virtual int32_t loadImage(int32_t AWidth, int32_t AHeight, void* ABuffer) { return -1; }
  virtual void    deleteImage(int32_t AImage) {}
  //virtual void    setFillImage(int32_t AImage, double alpha=1.0, double angle=0.0) {}
  virtual void    setFillImage(int32_t AImage, double xofs, double yofs, double xscale, double yscale, double AAlpha=1.0, double AAngle=0.0) {}


};

//----------------------------------------------------------------------
#endif




