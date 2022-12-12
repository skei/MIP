#ifndef sa_botage_widgets_included
#define sa_botage_widgets_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_image_widget.h"
#include "gui/widgets/mip_panel_widget.h"

//----------

#include "../img/sa_img.h"
#include "../img/botage_img.h"
#include "../img/mip_img.h"

#define SA_BOTAGE_HEADER_HEIGHT 80
#define SA_BOTAGE_FOOTER_HEIGHT 20

//----------------------------------------------------------------------

class sa_botage_header
: public MIP_PanelWidget {

private:

  int m_sa      = 0;
  int m_botage  = 0;
  int m_mip     = 0;

  MIP_ImageWidget*  MSaImage      = nullptr;
  MIP_ImageWidget*  MBotageImage  = nullptr;
  MIP_ImageWidget*  MMipImage     = nullptr;

public:

  sa_botage_header(MIP_DRect ARect)
  : MIP_PanelWidget(ARect) {

    setFillBackground(true);
    setBackgroundColor(0.25);

    MSaImage = new MIP_ImageWidget(MIP_DRect(0,0,80,80),(void*)sa_img,sa_img_size);
    appendChildWidget(MSaImage);

    MBotageImage = new MIP_ImageWidget(MIP_DRect(80,0,160,80),(void*)botage_img,botage_img_size);
    appendChildWidget(MBotageImage);

    MMipImage = new MIP_ImageWidget(MIP_DRect((ARect.w - 120),0,120,80),(void*)mip_img,mip_img_size);
    MMipImage->setAlpha(0.6);
    appendChildWidget(MMipImage);

  }

  virtual ~sa_botage_header() {
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

//----------

//class sa_botage_footer
//: public MIP_PanelWidget {
//
//public:
//
//  sa_botage_footer(MIP_DRect ARect)
//  : MIP_PanelWidget(ARect) {
//
//    setFillBackground(true);
//    setBackgroundColor(0.25);
//  }
//
//  virtual ~sa_botage_footer() {
//  }
//
//};


//----------------------------------------------------------------------
#endif

