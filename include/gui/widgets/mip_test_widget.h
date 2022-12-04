#ifndef mip_test_widget_included
#define mip_test_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_color_widget.h"


class MIP_TestWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  int32_t         MImage      = -1;
  //MIP_Surface*    MSurface    = nullptr;
  //MIP_XcbSurface* MXcbSurface = nullptr;

//------------------------------
public:
//------------------------------

  MIP_TestWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
    //MIP_Print("\n");

    for (uint32_t y=0; y<5; y++) {
      for (uint32_t x=0; x<5; x++) {
        if ((x==0) && (y==0)) {
          MIP_ColorWidget* cw   = new MIP_ColorWidget(MIP_DRect(x*80,   y*80,   70,70),MIP_Color(1,1,1,0.5));
            MIP_ColorWidget* c1 = new MIP_ColorWidget(MIP_DRect(x*80,   y*80,   30,30),MIP_Color(1,1,1,0.5));
            MIP_ColorWidget* c2 = new MIP_ColorWidget(MIP_DRect(x*80+40,y*80,   30,30),MIP_Color(1,1,1,0.5));
            MIP_ColorWidget* c3 = new MIP_ColorWidget(MIP_DRect(x*80,   y*80+40,30,30),MIP_Color(1,1,1,0.5));
            cw->appendChildWidget(c1);
            cw->appendChildWidget(c2);
            cw->appendChildWidget(c3);
          appendChildWidget(cw);
        }
        else {
          MIP_ColorWidget* cw = new MIP_ColorWidget(MIP_DRect(x*80,y*80,40,40),MIP_Color((float)x*0.25,(float)y*0.25,0,0.5));
          appendChildWidget(cw);
        }
      }
    }

  }

  //----------

  virtual ~MIP_TestWidget() {
    //MIP_Print("\n");
  }

//------------------------------
public:
//------------------------------

  void open(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) final {
    //MIP_Print("\n");
    MIP_Widget::open(AOwnerWindow,ARecursive);
    MIP_Window* window = (MIP_Window*)AOwnerWindow;
    MIP_Painter* painter = window->getWindowPainter();
    painter->makeCurrent();
    MImage = painter->loadImage("/home/skei/Pictures/skei_256.png");
    painter->resetCurrent();

    //MSurface = new MIP_Surface(window,256,256);
    //painter = new MIP_Painter(MSurface);
    //painter->setFillColor(MIP_COLOR_CYAN2);
    //painter->fillRect(0,0,256,256);
    //painter->flush();
    //delete painter;

    //MXcbSurface = new MIP_XcbSurface(window,256,256);
    //MIP_XcbPainter* xcb_painter = new MIP_XcbPainter(MSurface);
    //xcb_painter->setFillColor(MIP_COLOR_CYAN2);
    //xcb_painter->fillRect(0,0,256,256);
    //xcb_painter->flush();
    //delete xcb_painter;

  }

  //----------

  void close(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) final {
    //MIP_Print("\n");
    MIP_Window* window = (MIP_Window*)AOwnerWindow;
    MIP_Painter* painter = window->getWindowPainter();
    painter->makeCurrent();
    painter->deleteImage(MImage);
    painter->resetCurrent();

    //if (MSurface) delete MSurface;
    //if (MXcbSurface) delete MXcbSurface;

    MIP_Widget::close(AOwnerWindow,ARecursive);
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) final {
    //MIP_Print("\n");
    //do_widget_set_hint(this,"painting");

    MIP_Painter*    painter = AContext->painter;
    MIP_DRect       mrect   = getRect();
    MIP_Window* window  = (MIP_Window*)getOwnerWindow();

    double X = mrect.x;
    double Y = mrect.y;

    // getScale should be in window, not in basewindow..
    double S = window->getWindowScale();

    /*
      no scaling, because the wqindow system have already set the root widget
      (this widget) size for us..
    */

    painter->setFillColor(MIP_COLOR_GRAY);
    painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);

    /*
      ..but the rest must be scaled..
    */

    painter->setFillGradient(X+0,Y+20.0*S,0,80.0*S,MIP_COLOR_BRIGHT_RED,MIP_COLOR_BRIGHT_GREEN);
    painter->fillRect(X+20.0*S,Y+20.0*S,236.0*S,60.0*S);

    painter->setLineWidth(1.0*S);
    painter->setDrawColor(MIP_COLOR_WHITE);
    painter->drawLine(X+10.0*S,Y+10.0*S,X+265.0*S,Y+89.0*S);

    painter->setLineWidth(1.0*S);
    painter->setDrawColor(MIP_COLOR_DARK_GRAY);
    painter->drawRect(X+10.0*S,Y+10.0*S,256.0*S,256.0*S);

    painter->setLineWidth(20.0*S);
    painter->setDrawColor(MIP_COLOR_BLUE2);
    painter->drawLine(X+20.0*S,Y+20.0*S,X+216.0*S,Y+216.0*S);

    painter->setLineWidth(10.0*S);
    painter->setDrawColor(MIP_COLOR_WHITE);
    painter->drawArc(X+128.0*S,Y+128.0*S,50.0*S,0,MIP_PI * 1.75);

    painter->setTextColor(MIP_COLOR_BRIGHT_YELLOW);
    painter->setTextSize(32.0*S);
    painter->drawText(X+100.0*S,Y+150.0*S,"Hello world!");

    painter->setTextColor(MIP_COLOR_BLACK);
    painter->setTextSize(7.0*S);
    painter->drawText(X+300.0*S,Y+10.0*S,"Hello world 7");

    painter->setTextSize(8.0*S);
    painter->drawText(X+300.0*S,Y+20.0*S,"Hello world 8");

    painter->setTextSize(9.0*S);
    painter->drawText(X+300.0*S,Y+30.0*S,"Hello world 9");

    painter->setTextSize(10.0*S);
    painter->drawText(X+300.0*S,Y+40.0*S,"Hello world 10");

    painter->setFillImage(MImage,  X+128.0*S,  Y+128.0*S,   1.0*S,   1.0*S,  0.7, 0.1 );
    painter->fillRect(             X+128.0*S,  Y+128.0*S, 256.0*S, 256.0*S            );

//    MIP_XcbPainter* xcb_painter = new MIP_XcbPainter((MIP_Window*)window);
//    xcb_painter->blit(64,64,MXcbSurface,0,0,256,256);
//    xcb_painter->flush();
//    delete xcb_painter;

    paintChildWidgets(AContext);

  }

  //----------

//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
//    MIP_PRINT;
//  }
//
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {
//    MIP_PRINT;
//  }
//
//  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
//    MIP_PRINT;
//  }
//
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
//    MIP_PRINT;
//  }
//
//  void on_widget_mouse_dblclick(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
//    MIP_PRINT;
//  }
//
//  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
//    MIP_PRINT;
//  }
//
//  void on_widget_enter(double AXpos, double AYpos) override {
//    MIP_Print("test. index %i\n",getIndex());
//  }
//
//  void on_widget_leave(double AXpos, double AYpos) override {
//    MIP_Print("test. index %i\n",getIndex());
//  }


};


//----------------------------------------------------------------------
#endif
