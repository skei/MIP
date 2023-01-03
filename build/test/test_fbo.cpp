
#include "base/mip.h"
#include "gui/mip_widget.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------

class myWidget
: public MIP_Widget {

public:

  //void* MRenderBuffer = nullptr;

public:

  myWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
  }

public:

//  void open(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) override{
//    MIP_Widget::open(AOwnerWindow,ARecursive);
//    MIP_Window* window = (MIP_Window*)AOwnerWindow;
//    MIP_Painter* painter = window->getWindowPainter();
//    painter->makeCurrent(0);
//    // create fbo
//    MRenderBuffer = painter->createRenderBuffer(256,256);
//    // render into fbo
//    painter->selectRenderBuffer(MRenderBuffer);
//    painter->setViewport(0,0,256,256);
//    painter->beginFrame(256,256,1);
//    // background
//    painter->setFillColor(MIP_COLOR_DARK_CYAN2);
//    painter->fillRect(0,0,256,256);
//    // middle square
//    painter->setFillColor(MIP_COLOR_LIGHT_BLUE2);
//    painter->fillRect(64,64,128,128);
//    // finish
//    painter->endFrame();
//    painter->selectRenderBuffer(nullptr);
//    painter->resetCurrent(0);
//  }

  //----------

  // delete fbo

//  void close(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) final {
//    MIP_Window* window = (MIP_Window*)AOwnerWindow;
//    MIP_Painter* painter = window->getWindowPainter();
//    painter->deleteRenderBuffer(MRenderBuffer);
//    MIP_Widget::close(AOwnerWindow,ARecursive);
//  }

  //----------

  // called before the 'real' widget painting
  // gl context is current
  // no clipping, culling, etc, has been done..
  // - draw an additional line into fbo

//  void on_widget_preFrame(MIP_PaintContext* AContext) final {
//    MIP_Painter* painter = AContext->painter;
//    // render to fbo
//    painter->selectRenderBuffer(MRenderBuffer);
//    painter->setViewport(0,0,256,256);
//    painter->beginFrame(256,256,1.0);
//    // random color
//    double r = MIP_Random();
//    double g = MIP_Random();
//    double b = MIP_Random();
//    painter->setDrawColor(MIP_Color(r,g,b));
//    // random line coords
//    double x1 = MIP_RandomRange(0,256);
//    double y1 = MIP_RandomRange(0,256);
//    double x2 = MIP_RandomRange(0,256);
//    double y2 = MIP_RandomRange(0,256);
//    painter->setLineWidth(3);
//    painter->drawLine(x1,y1,x2,y2);
//    // end frame
//    painter->endFrame();
//    painter->selectRenderBuffer(nullptr);
//    //and continue..
//    MIP_Widget::on_widget_preFrame(AContext);
//  }

  //----------

  void on_widget_paint(MIP_PaintContext* AContext) final {
    MIP_Painter* painter = AContext->painter;

//    // fill background
//    MIP_DRect rect = getRect();
//    painter->setFillColor(MIP_COLOR_RED);
//    painter->fillRect(rect.x,rect.y,rect.w,rect.h);

//    // draw fbo
//    MIP_Window* window = (MIP_Window*)getOwnerWindow();
//    void* fbo = window->getRenderBuffer();
//    int32_t image = painter->getImageFromRenderBuffer(fbo);
//    painter->setFillImage( image,10,10,1,1,1,0);
//    painter->fillRect(10,10,256,256);

    // circle on top
    painter->setDrawColor(MIP_COLOR_WHITE);
    painter->setLineWidth(10);
    painter->drawArc(128,128,50,0,MIP_PI2);
  }

};

//----------------------------------------------------------------------

int main() {
  MIP_Window* window = new MIP_Window(640,480);
  myWidget* widget = new myWidget(MIP_DRect(0,0,640,480));
  window->setRootWidget(widget);
  window->setFillBackground(true);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
