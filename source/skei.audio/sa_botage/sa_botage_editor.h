#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

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

/*
      MIP_MenuWidget* menu = new MIP_MenuWidget(MIP_DRect(0,0,100,60), nullptr);
      menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,100,15), "Item1" ) );
      menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,15,100,15), "Item2" ) );
      menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,30,100,15), "Item3" ) );
      menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,45,100,15), "Item4" ) );
      MIP_TextWidget* text = new MIP_TextWidget( MIP_DRect(100,50,200,50), "Hello world!" );
      text->setTextSize(30);
      MRootWidget->appendChildWidget(text);
      MIP_ValueWidget* value = new MIP_ValueWidget( MIP_DRect(100,110,200,20), "Value", 0.5 );
      MRootWidget->appendChildWidget(value);
      MIP_ButtonWidget* button = new MIP_ButtonWidget( MIP_DRect(100,140,200,20), "Button", 0.0 );
      MRootWidget->appendChildWidget(button);
      MIP_DragValueWidget* dragvalue = new MIP_DragValueWidget( MIP_DRect(100,170,200,20), "DragValue", 0.5 );
      MRootWidget->appendChildWidget(dragvalue);
      dragvalue->clearFlag(MIP_WIDGET_FLAG_AUTO_HIDE_CURSOR);
      MIP_SliderWidget* slider = new MIP_SliderWidget( MIP_DRect(100,200,200,20), "Slider", 0.5 );
      MRootWidget->appendChildWidget(slider);
      MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_DRect(100,230,50,50), "Knob", 0.5 );
      MRootWidget->appendChildWidget(knob);
      MIP_MenuItemWidget* menuitem = new MIP_MenuItemWidget( MIP_DRect(100,300,200,20), "menuitem" );
      MRootWidget->appendChildWidget(menuitem);
      MIP_SelectorWidget* selector = new MIP_SelectorWidget( MIP_DRect(100,330,200,20), "selector", menu );
      MRootWidget->appendChildWidget(selector);
      MIP_GridWidget* grid = new MIP_GridWidget( MIP_DRect(400,100,200,200), 6, 6 );
      MRootWidget->appendChildWidget(grid);
      MIP_ButtonRowWidget* buttonrow = new MIP_ButtonRowWidget( MIP_DRect(400,320,200,20), 5, brtxt, MIP_BUTTON_ROW_SINGLE );
      MRootWidget->appendChildWidget(buttonrow);
      MIP_ButtonRowWidget* buttonrow2 = new MIP_ButtonRowWidget( MIP_DRect(400,350,200,20), 6, brtxt, MIP_BUTTON_ROW_MULTI );
      MRootWidget->appendChildWidget(buttonrow2);
      MRootWidget->appendChildWidget(menu);
      MEditor.connect( knob,      getParameter(0) );
      MEditor.connect( slider,    getParameter(1) );
      MEditor.connect( dragvalue, getParameter(2) );
*/

//----------

bool sa_botage_setup_editor(MIP_Editor* AEditor, MIP_Widget* ARootWidget) {

  double width  = ARootWidget->getWidth();
  sa_botage_header* header  = new sa_botage_header(MIP_DRect(0,0,width,SA_BOTAGE_HEADER_HEIGHT));
  ARootWidget->appendChildWidget(header);

//  double height = ARootWidget->getHeight();
//  sa_botage_footer* footer  = new sa_botage_footer(MIP_DRect(0,(height - SA_BOTAGE_FOOTER_HEIGHT),width,SA_BOTAGE_FOOTER_HEIGHT));
//  ARootWidget->appendChildWidget(footer);

  return true;
}

//----------------------------------------------------------------------
#endif
