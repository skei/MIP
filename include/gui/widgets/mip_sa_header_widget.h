#ifndef mip_sa_header_widget_included
#define mip_sa_header_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_image_widget.h"
#include "gui/widgets/mip_panel_widget.h"

//----------

#include "../../../data/img/sa_img.h"
//#include "../../../data/img/botage_img.h"
#include "../../../data/img/mip_img.h"

//#define MIP_SA_HEADER_WIDTH 80
//#define MIP_SA_HEADER_HEIGHT 20

//----------------------------------------------------------------------

class MIP_HeaderTextWidget
: public MIP_TextWidget {

public:

  MIP_HeaderTextWidget(MIP_DRect ARect, const char* AText="")
  : MIP_TextWidget(ARect,AText) {
  }

  virtual ~MIP_HeaderTextWidget() {
  }

public:

  void drawText(MIP_PaintContext* AContext) override {
    MIP_Painter* painter = AContext->painter;
    int font;
    font = painter->getHeaderFont();
    painter->selectFont(font);
    MIP_TextWidget::drawText(AContext);
    font = painter->getDefaultFont();
    painter->selectFont(font);
  }



};

//----------------------------------------------------------------------

class MIP_SAHeaderWidget
: public MIP_PanelWidget {

private:

  int m_sa      = 0;
  //int m_botage  = 0;
  int m_mip     = 0;

  MIP_ImageWidget*      MSaImage          = nullptr;
  //MIP_ImageWidget*    MPluginLogoImage  = nullptr;
  MIP_ImageWidget*      MMipImage         = nullptr;
  MIP_HeaderTextWidget* MNameText         = nullptr;
  MIP_TextWidget*       MVersionText      = nullptr;
  MIP_TextWidget*       MClapText         = nullptr;

  char                  clap_string[32]   = {0};

public:

  //MIP_SAHeaderWidget(MIP_DRect ARect, const char* AVersion, void* logo, uint32_t logosize)
  MIP_SAHeaderWidget(MIP_DRect ARect, const char* AName, const char* AVersion)
  : MIP_PanelWidget(ARect) {

    setFillBackground(true);
    setBackgroundColor(0.25);

    // sa

    MSaImage = new MIP_ImageWidget(MIP_DRect(0,0,80,80),(void*)sa_img,sa_img_size);
    appendChildWidget(MSaImage);
    MSaImage->setHint("skei.audio");

    // plugin logo

//    if (logo && (logosize > 0)) {
//      MPluginLogoImage = new MIP_ImageWidget(MIP_DRect(80,0,160,80),logo,logosize);
//      appendChildWidget(MPluginLogoImage);
//      MPluginLogoImage->setHint("sa_botage");
//    }

    MNameText = new MIP_HeaderTextWidget(MIP_DRect(80,7,(MIP_PLUGIN_GENERIC_EDITOR_WIDTH-230),80),AName);
    appendChildWidget(MNameText);
    MNameText->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MNameText->setTextSize(48);
    MNameText->setTextColor(1.0);
    MNameText->setFillBackground(false);
    MNameText->setDrawBorder(false);

    // version

    MVersionText = new MIP_TextWidget(MIP_DRect(84,50,60,20),AVersion);
    appendChildWidget(MVersionText);
    MVersionText->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MVersionText->setTextSize(8);
    MVersionText->setTextColor(0.4);
    MVersionText->setFillBackground(false);
    MVersionText->setDrawBorder(false);

    // mip

    MMipImage = new MIP_ImageWidget(MIP_DRect((ARect.w - 120),0,120,80),(void*)mip_img,mip_img_size);
    appendChildWidget(MMipImage);
    MMipImage->setAlpha(0.6);
    MMipImage->setHint("Multum In Parvo");

    // mip version

    MVersionText = new MIP_TextWidget(MIP_DRect((ARect.w - 110),55,60,10),MIP_VERSION);
    appendChildWidget(MVersionText);
    MVersionText->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MVersionText->setTextSize(8);
    MVersionText->setTextColor(0.4);
    MVersionText->setFillBackground(false);
    MVersionText->setDrawBorder(false);

    // clap

//    sprintf(clap_string,"CLAP %i.%i.%i",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);
//    MClapText = new MIP_TextWidget(MIP_DRect((ARect.w - 110),65,60,10),clap_string);
//    appendChildWidget(MClapText);
//    MClapText->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
//    MClapText->setTextSize(9);
//    MClapText->setTextColor(0.4);
//    MClapText->setFillBackground(false);
//    MClapText->setDrawBorder(false);

  }

  virtual ~MIP_SAHeaderWidget() {
  }

//  virtual void open(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) {
//    MIP_Widget::open(AOwnerWindow,ARecursive);
//    //MIP_Window* window = AEditor->getWindow();
//    MIP_Window* window = (MIP_Window*)AOwnerWindow;
//    MIP_Painter* painter = window->getWindowPainter();
//    int m_sa      = painter->loadImage((void*)sa_img,    sa_img_size);
//    int m_botage  = painter->loadImage((void*)botage_img,botage_img_size);
//    int m_mip     = painter->loadImage((void*)mip_img,   mip_img_size);
//  }

};

//----------------------------------------------------------------------
#endif


