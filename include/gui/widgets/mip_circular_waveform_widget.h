#ifndef mip_circular_waveform_widget_included
#define mip_circular_waveform_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "base/utils/mip_interpolation.h"
#include "gui/mip_paint_context.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------

#define MAX_WAVEFORM_LINES    4096
#define MAX_WAVEFORM_AREAS    16
#define MAX_WAVEFORM_MARKERS  16

#define DEFAULT_NUM_LINES     360

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct MIP_WaveformArea {
  bool      active  = false;
  double    size    = 0.0;
  double    pos     = 0.0;
  MIP_Color color   = MIP_COLOR_GREEN;
};

//----------------------------------------------------------------------

class MIP_CircularWaveformWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  uint32_t  MNumWaveformLineCoords                          = 0;
  double    MWaveformLineCoords[MAX_WAVEFORM_LINES * 4]     = {0};
  double    MWaveformDrawCoords[MAX_WAVEFORM_LINES * 4]     = {0};

//------------------------------
protected:
//------------------------------

  uint32_t  MAudioBufferSize                        = 0;
  float*    MAudioBuffer                            = nullptr;
  bool      MAudioBufferIsStereo                    = true;

  double    MOuterRadius                            = 1.0;
  double    MInnerRadius                            = 0.5;
  bool      MBipolar                                = true; // -> MIP_WaveformWidget?

  MIP_Color MWaveformColor                          = MIP_Color(1.0);
  double    MWaveformWidth                          = 1.0;
  MIP_Color MWaveformBackgroundColor                = MIP_Color(0.25);

  MIP_Color MZeroLineColor                          = MIP_Color(0.6);
  double    MZeroLineWidth                          = 1.0;

  uint32_t  MNumGrid                                = 4;
  uint32_t  MNumSubGrid                             = 4;
  double    MGridWidth                              = 0.002;
  double    MSubGridWidth                           = 0.001;
  MIP_Color MGridColor                              = MIP_Color(0.1);
  MIP_Color MSubGridColor                           = MIP_Color(0.2);

  //uint32_t  MNumMarkers                             = 0;
  //uint32_t  MNumAreas                               = 0;

  MIP_WaveformArea  MMarkers[MAX_WAVEFORM_MARKERS]  = {};
  MIP_WaveformArea  MAreas[MAX_WAVEFORM_AREAS]      = {};

//------------------------------
public:
//------------------------------

  MIP_CircularWaveformWidget(MIP_DRect ARect, uint32_t ANumLines=DEFAULT_NUM_LINES)
  : MIP_PanelWidget(ARect) {
    setFillBackground(true);
    setBackgroundColor(0.2);
    setDrawBorder(false);
    precalcLineCoords(ANumLines,ARect);
  }

  //----------

  virtual ~MIP_CircularWaveformWidget() {
    //free(MAudioBuffer);
  }

//------------------------------
public:
//------------------------------

  virtual void setAudioBuffer(uint32_t ASize, float* ABuffer, bool AStereo=true) {
    MAudioBufferSize = ASize;
    MAudioBuffer = ABuffer;
    MAudioBufferIsStereo = AStereo;
  }

  virtual void setRadius(double AOuter, double AInner) {
    MOuterRadius = AOuter;
    MInnerRadius = AInner;
  }

  virtual void setBipolar(bool ABipolar) {
    MBipolar = ABipolar;
  }

  virtual void setWaveformColor(MIP_Color AColor)           { MWaveformColor = AColor; }
  virtual void setWaveformWidth(double AWidth)              { MWaveformWidth = AWidth; }
  virtual void setWaveformBackgroundColor(MIP_Color AColor) { MWaveformBackgroundColor = AColor; }

  virtual void setZeroLineColor(MIP_Color AColor)           { MZeroLineColor = AColor; }
  virtual void setZeroLineWidth(double AWidth)              { MZeroLineWidth = AWidth; }

  virtual void setGrid(uint32_t AGrid, uint32_t ASubGrid) {
    MNumGrid = AGrid;
    MNumSubGrid = ASubGrid;
  }

  virtual void setGrid(uint32_t AGrid) {
    MNumGrid = AGrid;
  }

  virtual void setSubGrid(uint32_t ASubGrid) {
    MNumSubGrid = ASubGrid;
  }

  virtual void setGridColor(MIP_Color AGrid, MIP_Color ASubGrid) {
    MGridColor = AGrid;
    MSubGridColor = ASubGrid;
  }

  virtual void setGridWidth(double AGrid=0.002, double ASubGrid=0.001) {
    MGridWidth = AGrid;
    MSubGridWidth = ASubGrid;
  }

  //virtual void setNumAreas(uint32_t ANum) {
  //  MNumAreas = ANum;
  //}

  virtual void setAreaActive(uint32_t AIndex, bool AActive=true) {
    MAreas[AIndex].active = AActive;
  }

  virtual void setAreaPos(uint32_t AIndex, double APos, double ASize) {
    MAreas[AIndex].pos = APos;
    MAreas[AIndex].size = ASize;
  }

  virtual void setAreaColor(uint32_t AIndex, MIP_Color AColor) {
    MAreas[AIndex].color = AColor;
  }

  //virtual void setNumMarkers(uint32_t ANum) {
  //  MNumMarkers = ANum;
  //}

  virtual void setMarkerActive(uint32_t AIndex, bool AActive=true) {
    MMarkers[AIndex].active = AActive;
  }

  virtual void setMarkerPos(uint32_t AIndex, double APos, double ASize=0.0025) {
    MMarkers[AIndex].pos = APos;
    MMarkers[AIndex].size = ASize;
  }

  virtual void setMarkerColor(uint32_t AIndex, MIP_Color AColor) {
    MMarkers[AIndex].color =AColor;
  }

//------------------------------
private:
//------------------------------

  void drawSegment(MIP_Painter* APainter, double cx, double cy, double r, double v1, double v2) {
    if (v2 > 0.0) {
      double a1 = v1 + 0.75 + v2;
      double a2 = v2;
      a1 *= MIP_PI2;
      a2 *= MIP_PI2;
      APainter->drawArc(cx,cy,r,a1,a2);
    }
  }

//------------------------------
public:
//------------------------------

  virtual void precalcLineCoords(uint32_t ANum, MIP_DRect ARect) {
    MNumWaveformLineCoords = ANum;
    if (MNumWaveformLineCoords > 0) {
      double size = MIP_Min(ARect.w,ARect.h) * 0.5;
      double center_x = ARect.x + size;
      double center_y = ARect.y + size;
      double angle = -0.25; // start at 12 o'clock
      double angle_add = 1.0f / (double)MNumWaveformLineCoords;
      for (uint32_t i=0; i<MNumWaveformLineCoords; i++) {
        //double a1 = angle - (angle_add * 0.5);
        //double a2 = angle + (angle_add * 0.5);
        //a1 = MIP_Fract(a1);
        //a2 = MIP_Fract(a2);
        angle = MIP_Fract(angle);
        double cos_angle = cosf(angle * MIP_PI2);
        double sin_angle = sinf(angle * MIP_PI2);
        double x1 = center_x + (cos_angle * (size * MInnerRadius));
        double y1 = center_y + (sin_angle * (size * MInnerRadius));
        double x2 = center_x + (cos_angle * (size * MOuterRadius));
        double y2 = center_y + (sin_angle * (size * MOuterRadius));
        //APainter->drawLine(xcenter+prevx,ycenter+prevy,xcenter+x,ycenter+y,MLineColor);
        //APainter->lineTo(xcenter+x,ycenter+y);
        MWaveformLineCoords[(i * 4)    ] = x1;
        MWaveformLineCoords[(i * 4) + 1] = y1;
        MWaveformLineCoords[(i * 4) + 2] = x2;
        MWaveformLineCoords[(i * 4) + 3] = y2;
        angle += angle_add;
      }
    }
  }

  //----------

  virtual void drawAreas(MIP_PaintContext* AContext) {
    //if (MNumAreas > 0) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect mrect = getRect();
      //MIP_Window* window = (MIP_Window*)getOwnerWindow();
      //double S = window->getWindowScale();
      double cx = mrect.x + (mrect.w * 0.5);
      double cy = mrect.y + (mrect.h * 0.5);
      double size = MIP_Min(mrect.w,mrect.h) * 0.5;
      double ri = size * MInnerRadius;
      double ro = size * MOuterRadius;
      double rw = ro - ri;
      double r  = ri + (rw * 0.5);
      painter->setLineWidth(rw);
      painter->setDrawColor( MWaveformBackgroundColor );
      drawSegment(painter,cx,cy,r,0,MIP_PI2);
      painter->setLineWidth(rw);
      //for (uint32_t i=0; i<MNumAreas; i++) {
      for (uint32_t i=0; i<MAX_WAVEFORM_AREAS; i++) {
        if (MAreas[i].active) {
          double m1 = MAreas[i].pos;
          double m2 = MAreas[i].size;
          painter->setDrawColor(MAreas[i].color);
          drawSegment(painter,cx,cy,r,m1,m2);
        }
      }
    //}
  }

  //----------

  virtual void drawMarkers(MIP_PaintContext* AContext) {
    //if (MNumMarkers > 0) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect mrect = getRect();
      MIP_Window* window = (MIP_Window*)getOwnerWindow();
      double S = window->getWindowScale();
      double cx = mrect.x + (mrect.w * 0.5);
      double cy = mrect.y + (mrect.h * 0.5);
      double size = MIP_Min(mrect.w,mrect.h) * 0.5;
      double ri = size * MInnerRadius;
      double ro = size * MOuterRadius;
      double rw = ro - ri;
      double r  = ri + (rw * 0.5);
      painter->setLineWidth(rw);
      //for (uint32_t i=0; i<MNumMarkers; i++) {
      for (uint32_t i=0; i<MAX_WAVEFORM_MARKERS; i++) {
        if (MMarkers[i].active) {
          double m1 = MMarkers[i].pos;
          double m2 = MMarkers[i].size * S;
          painter->setDrawColor(MMarkers[i].color);
          drawSegment(painter,cx,cy,r,m1,m2);
        }
      }
    //}
  }

  //----------

  virtual void drawGrid(MIP_PaintContext* AContext) {
    uint32_t num = MNumGrid * MNumSubGrid;
    if (num > 1) {
      MIP_Painter* painter = AContext->painter;
      MIP_DRect mrect = getRect();
      //MIP_Window* window = (MIP_Window*)getOwnerWindow();
      //double S = window->getWindowScale();
      double cx = mrect.x + (mrect.w * 0.5);
      double cy = mrect.y + (mrect.h * 0.5);
      double size = MIP_Min(mrect.w,mrect.h) * 0.5;
      double ri = size * MInnerRadius;
      double ro = size * MOuterRadius;
      double rw = ro - ri;
      double r  = ri + (rw * 0.5);
      painter->setLineWidth(rw);
      double pos = 0.0;
      double adder = 1.0 / (double)num;
      for (uint32_t i=0; i<num; i++) {
        if ((i % MNumSubGrid) == 0) {
          painter->setDrawColor(MGridColor);
          drawSegment(painter,cx,cy,r,pos,MGridWidth);
        }
        else {
          painter->setDrawColor(MSubGridColor);
          drawSegment(painter,cx,cy,r,pos,MSubGridWidth);
        }
        pos += adder;
      }

    }
  }

  //----------

  virtual void drawWaveform(MIP_PaintContext* AContext) {
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();
    if (MAudioBuffer && (MAudioBufferSize > 0)) {
      // waveform
      painter->setDrawColor(MWaveformColor);
      painter->setLineWidth(MWaveformWidth * S);
      double bufferpos = 0.0;
      double bufferadd = (double)MAudioBufferSize / (double)MNumWaveformLineCoords;
      for (uint32_t i=0; i<MNumWaveformLineCoords; i++) {
        uint32_t ip = MIP_Trunc(bufferpos);
        bufferpos += bufferadd;
        float vv = 0.0;
        if (MAudioBufferIsStereo) {
          float v1 = MAudioBuffer[(ip * 2)    ]; // left
          float v2 = MAudioBuffer[(ip * 2) + 1]; // right
          vv = (v1 + v2) * 0.5;
        }
        else {
          vv = MAudioBuffer[ip];
        }
        MIP_Clamp(vv,-1,1);
        float v  = (vv * 0.5) + 0.5;            //  0..1
        double x1 = MWaveformLineCoords[(i * 4)    ] * S;
        double y1 = MWaveformLineCoords[(i * 4) + 1] * S;
        double x2 = MWaveformLineCoords[(i * 4) + 2] * S;
        double y2 = MWaveformLineCoords[(i * 4) + 3] * S;
        double cx = MIP_Interpolate_Linear(0.5,x1,x2);
        double cy = MIP_Interpolate_Linear(0.5,y1,y2);
        double xx = MIP_Interpolate_Linear(v,x1,x2);
        double yy = MIP_Interpolate_Linear(v,y1,y2);
        painter->drawLine(cx,cy,xx,yy);
      }
    }
    // zero-line
    double cx = mrect.x + (mrect.w * 0.5);
    double cy = mrect.y + (mrect.h * 0.5);
    double size = MIP_Min(mrect.w,mrect.h) * 0.5;
    double ri = size * MInnerRadius;
    double ro = size * MOuterRadius;
    double rw = ro - ri; //MOuterRadius - MInnerRadius;
    double r  = ri + (rw * 0.5);
    painter->setDrawColor(MZeroLineColor);
    painter->setLineWidth(MZeroLineWidth * S);
    drawSegment(painter,cx,cy,r,0,MIP_PI2);
  }

  //----------

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    fillBackground(AContext);
    drawAreas(AContext);
    drawMarkers(AContext);
    drawGrid(AContext);
    drawWaveform(AContext);
    //paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------

#undef MAX_WAVEFORM_LINES
#undef MAX_WAVEFORM_AREAS
#undef MAX_WAVEFORM_MARKERS
#undef NUM_TEST_SAMPLES
#undef DEFAULT_NUM_LINES

//----------------------------------------------------------------------
#endif

