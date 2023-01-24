
#include "gui/mip_window.h"

//----------------------------------------------------------------------

class myWidget
: public MIP_Widget {

private:

  //int32_t image = -1;

public:

  myWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
  }

public:

//  void open(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) {
//    MIP_Widget::open(AOwnerWindow,ARecursive);
//    MIP_Window* window = (MIP_Window*)AOwnerWindow;
//    MIP_Painter* painter = window->getWindowPainter();
//    image = painter->loadImage("/DISKS/sda2/code/git/MIP/data/img/sa_60x60.png");
//    MIP_Print("image %i\n",image);
//  }
//
//  void close(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) {
//    MIP_Widget::close(AOwnerWindow,ARecursive);
//    MIP_Window* window = (MIP_Window*)AOwnerWindow;
//    MIP_Painter* painter = window->getWindowPainter();
//    painter->deleteImage(image);
//  }


  void on_widget_paint(MIP_PaintContext* AContext) final {
    MIP_Window* owner = (MIP_Window*)getOwnerWindow();
    double S = owner->getWindowScale();
    MIP_DRect rect = getRect();
    MIP_DRect updaterect = AContext->updateRect;
    updaterect.overlap(rect);

    MIP_Painter* painter = AContext->painter;

    // background
    painter->setFillColor(MIP_COLOR_GRAY);
    painter->fillRect(rect.x,rect.y,rect.w,rect.h);

    // logo
//    int32_t image = painter->loadImage("/DISKS/sda2/code/git/MIP/data/img/sa_60x60.png");
//    MIP_Print("image %i\n",image);
//    painter->setFillImage(image,0,0, 1,1, 1,0);
//    painter->fillRect(0,0,60,60);
//    painter->deleteImage(image);

    // arc
    painter->setDrawColor(MIP_COLOR_RED);
    painter->setLineWidth(10.0 * S);
    painter->drawArc(300*S,200*S,50*S,0,MIP_PI);

    //paintChildWidgets(AContext);
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
