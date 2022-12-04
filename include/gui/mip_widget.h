#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

/*

  ScaledWidget
   ChildWidget
    ParentWidget
     ContainerWidget
      LayoutWidget

      Clickable
      Draggable
      Movable
      Sizable
      Closable
      Scrollable

*/

// base widget = the minimal to receive events, etc..

#include "base/mip.h"
#include "gui/base/mip_base_window.h"
#include "gui/mip_paint_context.h"

#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Widget;
typedef MIP_Array<MIP_Widget*> MIP_Widgets;

//----------------------------------------------------------------------

class MIP_WidgetListener {
public:
  virtual void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) {}
  virtual void do_widget_redraw(MIP_Widget* AWidget, uint32_t AMode=0) {}
  virtual void do_widget_set_cursor(MIP_Widget* AWidget, int32_t ACursor) {}
  virtual void do_widget_set_key_capture(MIP_Widget* AWidget, uint32_t AMode) {}
  virtual void do_widget_set_modal(MIP_Widget* AWidget, uint32_t AMode) {}
  virtual void do_widget_set_hint(MIP_Widget* AWidget, const char* AHint) {}
  //virtual void do_widget_set_timer(MIP_Widget* AWidget, uint32_t ATime) {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Widget
: public MIP_WidgetListener {

//------------------------------
private:
//------------------------------

//  MIP_Widget*       MParent       = nullptr;
  MIP_WidgetListener* MListener     = nullptr;
  MIP_Widgets         MChildren     = {};
  MIP_BaseWindow*     MOwnerWindow  = nullptr;
  int32_t             MIndex        = -1;
  MIP_DRect           MRect         = {};
  MIP_DRect           MBaseRect     = {};

  MIP_Parameter*      MParameter    = nullptr;

//------------------------------
protected:
//------------------------------

  double              MValue        = 0.0;
  double              MModulation   = 0.0;

//------------------------------
public:
//------------------------------

  MIP_Widget(MIP_DRect ARect) {
    MRect     = ARect;
    MBaseRect = ARect;
  }

  //----------

  virtual ~MIP_Widget() {
    #ifndef MIP_NO_AUTODELETE
    deleteChildWidgets();
    #endif
  }

//------------------------------
public:
//------------------------------

//virtual MIP_Widget*     getParent()           { return MParent; }

  virtual MIP_BaseWindow* getOwnerWindow()      { return MOwnerWindow; }
  virtual int32_t         getIndex()            { return MIndex; }
  virtual MIP_DRect       getRect()             { return MRect; }
  virtual double          getWidth()            { return MRect.w; }
  virtual double          getHeight()           { return MRect.h; }

  virtual void setListener(MIP_WidgetListener* AListener) { MListener = AListener; }

  virtual void            setParameter(MIP_Parameter* AParameter) { MParameter = AParameter; }
  virtual MIP_Parameter*  getParameter()                          { return MParameter; }

  virtual void            setValue(double AValue) { MValue = AValue; }
  virtual double          getValue()              { return MValue; }

  virtual void            setModulation(double AValue) { MModulation = AValue; }
  virtual double          getModulation()              { return MModulation; }

//------------------------------
public:
//------------------------------

  virtual void setPos(double AXpos, double AYpos) {
    MRect.x = AXpos;
    MRect.y = AYpos;
  }

  //----------

  virtual void setSize(double AWidth, double AHeight) {
    MRect.w = AWidth;
    MRect.h = AHeight;
  }

  //----------

  /*
    called last in MIP_Window.open()
  */

  virtual void open(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) {
    MOwnerWindow = AOwnerWindow;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->open(AOwnerWindow,ARecursive);
      }
    }
  }

  //----------

  /*
    called first in MIP_Window.close()
  */

  virtual void close(MIP_BaseWindow* AOwnerWindow, bool ARecursive=true) {
    //MOwnerWindow = AOwnerWindow;
    if (ARecursive) {
      for (uint32_t i=0; i<MChildren.size(); i++) {
        MChildren[i]->close(AOwnerWindow,ARecursive);
      }
    }
  }

  //----------

  virtual void redraw() {
    if (MListener) MListener->do_widget_redraw(this);
  }

//------------------------------
public:
//------------------------------

  virtual void appendChildWidget(MIP_Widget* AWidget, MIP_WidgetListener* AListener=nullptr) {
    int32_t index = MChildren.size();
    //AWidget->MParent = this;
    if (AListener) AWidget->MListener = AListener;
    else AWidget->MListener = this;
    AWidget->MIndex = index;
    MChildren.append(AWidget);
  }

  //----------

  virtual void deleteChildWidgets() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      if (MChildren[i]) delete MChildren[i];
      MChildren[i] = nullptr;
    }
  }

  //----------

  // returns (sub_) child at (x/y)
  // or self/this if no child widgets at that point

  virtual MIP_Widget* findChildWidget(double AXpos, double AYpos, bool ARecursive=true) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      MIP_Widget* widget = MChildren[i];
      MIP_DRect rect = widget->getRect();
      if (rect.contains(AXpos,AYpos)) {
        MIP_Widget* child = widget;
        if (ARecursive) {
          child = widget->findChildWidget(AXpos,AYpos,ARecursive);
        }
        return child;
      }
    }
    return this;
  }

  //----------

  virtual uint32_t getNumChildWidgets() {
    return MChildren.size();
  }

  //----------

  virtual MIP_Widget* getChildWidget(uint32_t AIndex) {
    return MChildren[AIndex];
  }

  //----------

  //virtual void setOwnerWindow(MIP_BaseWindow* AWindow, bool ARecursive=true) {
  //  MOwnerWindow = AWindow;
  //  if (ARecursive) {
  //    for (uint32_t i=0; i<MChildren.size(); i++) {
  //      //MIP_Widget* widget = MChildren[i];
  //      MChildren[i]->setOwnerWindow(AWindow,ARecursive);
  //    }
  //  }
  //}

  //----------

  virtual void scaleChildWidgets(double AScale, bool ARecursive=true) {
    //MIP_Print("AScale %f\n",AScale);
    //MRect.x = MBaseRect.x * AScale;
    //MRect.y = MBaseRect.y * AScale;
    //MRect.w = MBaseRect.w * AScale;
    //MRect.h = MBaseRect.h * AScale;
    for (uint32_t i=0; i<MChildren.size(); i++) {
      MIP_Widget* widget = MChildren[i];
      widget->MRect.x = widget->MBaseRect.x * AScale;
      widget->MRect.y = widget->MBaseRect.y * AScale;
      widget->MRect.w = widget->MBaseRect.w * AScale;
      widget->MRect.h = widget->MBaseRect.h * AScale;
      if (ARecursive) MChildren[i]->scaleChildWidgets(AScale,ARecursive);
    }
  }

  //----------

  virtual void paintChildWidgets(MIP_PaintContext* AContext, bool ARecursive=true) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      MIP_Widget* widget = MChildren[i];
      widget->on_widget_paint(AContext);
      if (ARecursive) widget->paintChildWidgets(AContext,ARecursive);
    }
  }

  //----------

  virtual void alignChildWidgets(bool ARecursive=true) {
    //for (uint32_t i=0; i<MChildren.size(); i++) {
    //  MIP_Widget* widget = MChildren[i];
    //  widget->MRect.x = widget->MBaseRect.x + MRect.x;
    //  widget->MRect.y = widget->MBaseRect.y + MRect.y;
    //  if (ARecursive) widget->realignChildWidgets();
    //}
  }

  //----------

//------------------------------
public:
//------------------------------

  virtual void on_widget_move(double AXpos, double AYpos) {
  }

  virtual void on_widget_resize(double AWidth, double AHeight) {
  }

  virtual void on_widget_paint(MIP_PaintContext* AContext) {
    paintChildWidgets(AContext);
  }

  virtual void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) {
  }

  virtual void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
  }

  virtual void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
  }

  virtual void on_widget_mouse_dblclick(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
  }

  virtual void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) {
  }

  virtual void on_widget_enter(double AXpos, double AYpos) {
  }

  virtual void on_widget_leave(double AXpos, double AYpos) {
  }


//------------------------------
public:
//------------------------------

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
    if (MListener) MListener->do_widget_update(AWidget,AMode);
  }

  void do_widget_redraw(MIP_Widget* AWidget, uint32_t AMode=0) override {
    if (MListener) MListener->do_widget_redraw(AWidget,AMode);
  }

  void do_widget_set_cursor(MIP_Widget* AWidget, int32_t ACursor) override {
    if (MListener) MListener->do_widget_set_cursor(AWidget,ACursor);
  }

  void do_widget_set_key_capture(MIP_Widget* AWidget, uint32_t AMode) override {
    if (MListener) MListener->do_widget_set_key_capture(AWidget,AMode);
  }

  void do_widget_set_modal(MIP_Widget* AWidget, uint32_t AMode) override {
    if (MListener) MListener->do_widget_set_modal(AWidget,AMode);
  }

  void do_widget_set_hint(MIP_Widget* AWidget, const char* AHint) override {
    if (MListener) MListener->do_widget_set_hint(AWidget,AHint);
  }

//  void do_widget_set_timer(MIP_Widget* AWidget, uint32_t ATime) override {
//    if (MListener) MListener->do_widget_set_timer(AWidget,ATime);
//  }

};

//----------------------------------------------------------------------
#endif