
#include "gui/mip_window.h"

//----------------------------------------------------------------------

class myWidget
: public MIP_Widget {

public:

  myWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
  }

public:

  void on_widget_paint(MIP_PaintContext* AContext) final {

    MIP_Window* owner = (MIP_Window*)getOwnerWindow();
    double S = owner->getWindowScale();

    MIP_DRect rect = getRect();
    MIP_DRect updaterect = AContext->updateRect;
    updaterect.overlap(rect);
    AContext->painter->setFillColor(MIP_COLOR_GRAY);
    AContext->painter->setLineWidth(10.0 * S);
    AContext->painter->fillRect(rect.x,rect.y,rect.w,rect.h);

    AContext->painter->setDrawColor(MIP_COLOR_RED);
    AContext->painter->drawArc(300*S,200*S,50*S,0,MIP_PI);


    paintChildWidgets(AContext);
  }

};

//----------------------------------------------------------------------

int main() {
  MIP_Window* window = new MIP_Window(600,400);
  myWidget* widget = new myWidget(MIP_DRect(0,0,600,400));
  window->setTitle("myWindow");
  window->setRootWidget(widget);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
