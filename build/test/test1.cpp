
#include "gui/mip_window.h"
#include "gui/mip_widgets.h"

const char* brtxt[] = { "one", "2", "3", "IV", "V", "six" };

//----------

int main(int argc, char** argv) {
  MIP_Window* window = new MIP_Window(640,480);
  MIP_PanelWidget* background = new MIP_PanelWidget(MIP_DRect(0,0,640,480));
  window->setRootWidget(background,window);

  MIP_MenuWidget* menu = new MIP_MenuWidget(MIP_DRect(0,0,100,60), nullptr);
    menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,100,15), "Item1" ) );
    menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,15,100,15), "Item2" ) );
    menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,30,100,15), "Item3" ) );
    menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,45,100,15), "Item4" ) );

  MIP_TextWidget* text = new MIP_TextWidget( MIP_DRect(100,50,200,50), "Hello world!" );
  background->appendChildWidget(text);
  text->setTextSize(30);

  MIP_ValueWidget* value = new MIP_ValueWidget( MIP_DRect(100,110,200,20), "Value", 0.5 );
  background->appendChildWidget(value);

  MIP_ButtonWidget* button = new MIP_ButtonWidget( MIP_DRect(100,140,200,20), "Button", 0.0 );
  background->appendChildWidget(button);

  MIP_DragValueWidget* dragvalue = new MIP_DragValueWidget( MIP_DRect(100,170,200,20), "DragValue", 0.5 );
  background->appendChildWidget(dragvalue);

  MIP_SliderWidget* slider = new MIP_SliderWidget( MIP_DRect(100,200,200,20), "Slider", 0.5 );
  background->appendChildWidget(slider);

  MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_DRect(100,230,50,50), "Knob", 0.5 );
  background->appendChildWidget(knob);

  MIP_MenuItemWidget* menuitem = new MIP_MenuItemWidget( MIP_DRect(100,300,200,20), "menuitem" );
  background->appendChildWidget(menuitem);

  MIP_SelectorWidget* selector = new MIP_SelectorWidget( MIP_DRect(100,330,200,20), "selector", menu );
  background->appendChildWidget(selector);


  MIP_GridWidget* grid = new MIP_GridWidget( MIP_DRect(400,100,200,200), 6, 6 );
  background->appendChildWidget(grid);

  MIP_ButtonRowWidget* buttonrow = new MIP_ButtonRowWidget( MIP_DRect(400,320,200,20), 5, brtxt, MIP_BUTTON_ROW_SINGLE );
  background->appendChildWidget(buttonrow);

  MIP_ButtonRowWidget* buttonrow2 = new MIP_ButtonRowWidget( MIP_DRect(400,350,200,20), 6, brtxt, MIP_BUTTON_ROW_MULTI );
  background->appendChildWidget(buttonrow2);

  background->appendChildWidget(menu);

  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
