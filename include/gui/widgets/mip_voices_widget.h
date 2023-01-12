#ifndef mip_voices_widget_included
#define mip_voices_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_panel_widget.h"

#define MAX_VOICES 1024

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_VoicesWidget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool      MDrawVoices               = true;
  uint32_t  MMaxVoices                = 0;
  MIP_Color MPlayingColor             = MIP_Color(0.8);
  MIP_Color MReleasedColor            = MIP_Color(0.5);
  MIP_Color MOffColor                 = MIP_Color(0.2);
  uint32_t  MVoiceStates[MAX_VOICES]  = {0};

//------------------------------
public:
//------------------------------

  MIP_VoicesWidget(MIP_DRect ARect, uint32_t AMaxVoices)
  : MIP_PanelWidget(ARect) {
    MMaxVoices = AMaxVoices;
    setFillBackground(false);
    setDrawBorder(false);
  }

  //----------

  virtual ~MIP_VoicesWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setMaxVoices(uint32_t AValue)      { MMaxVoices = AValue; }
  virtual void setPlayingColor(MIP_Color AColor)  { MPlayingColor = AColor; }
  virtual void setReleasedColor(MIP_Color AColor) { MReleasedColor = AColor; }
  virtual void setOffColor(MIP_Color AColor)      { MOffColor = AColor; }

  virtual void setVoiceState(uint32_t AIndex, uint32_t AState) {
    MVoiceStates[AIndex] = AState;
  }

//------------------------------
public:
//------------------------------

  virtual void drawVoices(MIP_PaintContext* AContext) {
    if (MDrawVoices) {
      if (MMaxVoices > 0) {
        //MIP_Window* window = (MIP_Window*)getOwnerWindow();
        //double S = window->getWindowScale();
        MIP_Painter* painter = AContext->painter;
        MIP_DRect mrect = getRect();

        double width = mrect.w / MMaxVoices;

        for (uint32_t i=0; i<MMaxVoices; i++) {

          double x = mrect.x + (width * i);
          double y = mrect.y;
          double w = width;
          double h = mrect.h;

          MIP_Color color = MIP_COLOR_BLACK;
          switch (MVoiceStates[i]) {
            case MIP_VOICE_OFF:       { color = MOffColor; break; }
            case MIP_VOICE_PLAYING:   { color = MPlayingColor; break; }
            case MIP_VOICE_RELEASED:  { color = MReleasedColor; break; }
          }

          painter->setFillColor(color);
          painter->fillRect(x,y,w,h);

        }
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    if (MFillBackground) fillBackground(AContext);
    //paintChildWidgets(AContext);
    if (MDrawVoices) drawVoices(AContext);
    if (MDrawBorder) drawBorder(AContext);

  }

  //----------


};

#undef MAX_VOICES

//----------------------------------------------------------------------
#endif

