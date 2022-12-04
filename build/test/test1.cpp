
#include "gui/mip_window.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_text_widget.h"
#include "gui/widgets/mip_value_widget.h"
#include "gui/widgets/mip_button_widget.h"
#include "gui/widgets/mip_drag_value_widget.h"
#include "gui/widgets/mip_slider_widget.h"

//----------

int main(int argc, char** argv) {
  MIP_Window* window = new MIP_Window(640,480);
  MIP_PanelWidget* background = new MIP_PanelWidget(MIP_DRect(0,0,640,480));
  window->setRootWidget(background,window);
  MIP_TextWidget* text = new MIP_TextWidget( MIP_DRect(100,50,200,50), "Hello world!" );
  text->setTextSize(30);
  background->appendChildWidget(text);
  MIP_ValueWidget* value = new MIP_ValueWidget( MIP_DRect(100,110,200,20), "Value", 0.5 );
  background->appendChildWidget(value);
  MIP_ButtonWidget* button = new MIP_ButtonWidget( MIP_DRect(100,140,200,20), "Button", 0.0 );
  background->appendChildWidget(button);
  MIP_DragValueWidget* dragvalue = new MIP_DragValueWidget( MIP_DRect(100,170,200,20), "DragValue", 0.5 );
  background->appendChildWidget(dragvalue);
  MIP_SliderWidget* slider = new MIP_SliderWidget( MIP_DRect(100,200,200,20), "Slider", 0.5 );
  background->appendChildWidget(slider);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
