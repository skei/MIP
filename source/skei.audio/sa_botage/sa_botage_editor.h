#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

//----------

#include "sa_botage_widgets.h"

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

//----------------------------------------------------------------------

bool sa_botage_setup_editor(MIP_Editor* AEditor, MIP_Widget* ARootWidget) {

  MIP_Window* window = AEditor->getWindow();
  MIP_Assert(window);

  // header

  double width  = ARootWidget->getWidth();
  sa_botage_header* header  = new sa_botage_header(MIP_DRect(0,0,width,SA_BOTAGE_HEADER_HEIGHT));
  ARootWidget->appendChildWidget(header);

  // waveform

  MIP_PanelWidget* waveform = new MIP_PanelWidget( MIP_DRect(10,90,(SA_BOTAGE_EDITOR_WIDTH - 20),60) );
  ARootWidget->appendChildWidget(waveform);

  // beats, slices

  double __MIP_UNUSED x,y,w,h = 0;

  w = ((SA_BOTAGE_EDITOR_WIDTH - 30) / 2);

  MIP_SliderWidget* beats = new MIP_SliderWidget( MIP_DRect(10,160,w,20),"Beats");
  ARootWidget->appendChildWidget(beats);

  x = 10 + w + 10;

  MIP_SliderWidget* slices = new MIP_SliderWidget( MIP_DRect(x,160,w,20),"Slices");
  ARootWidget->appendChildWidget(slices);


  // footer

  //double height = ARootWidget->getHeight();
  //sa_botage_footer* footer  = new sa_botage_footer(MIP_DRect(0,(height - SA_BOTAGE_FOOTER_HEIGHT),width,SA_BOTAGE_FOOTER_HEIGHT));
  //ARootWidget->appendChildWidget(footer);

  return true;
}

//----------------------------------------------------------------------
#endif
