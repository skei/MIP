#ifndef mip_widget_included
#define mip_widget_included
//----------------------------------------------------------------------

// widget values are 0..1

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
  virtual void do_widget_set_cursor_pos(MIP_Widget* AWidget, int32_t AXpos, int32_t AYpos) {}
  virtual void do_widget_set_key_capture(MIP_Widget* AWidget, uint32_t AMode) {}
  virtual void do_widget_set_modal(MIP_Widget* AWidget, uint32_t AMode) {}
  virtual void do_widget_set_hint(MIP_Widget* AWidget, const char* AHint) {}
  //virtual void do_widget_set_timer(MIP_Widget* AWidget, uint32_t ATime) {}
  virtual void do_widget_notify(MIP_Widget* AWidget, int32_t AValue) {}
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

//  MIP_Widget*       MParent           = nullptr;
  MIP_WidgetListener* MListener         = nullptr;
  MIP_Widgets         MChildren         = {};
  MIP_BaseWindow*     MOwnerWindow      = nullptr;
  int32_t             MIndex            = -1;
  MIP_DRect           MRect             = {};
  MIP_DRect           MBaseRect         = {};
  int32_t             MCursor           = MIP_CURSOR_DEFAULT;

//  MIP_Parameter*      MParameter        = nullptr;

  MIP_Parameter*      MParameters[16]   = {0};
  uint32_t            MNumParameters    = 0;

//------------------------------
protected:
//------------------------------

//  double              MValue            = 0.0;
//  double              MModulation       = 0.0;

  double              MValues[16]       = {0};
  double              MModulations[16]  = {0};

  bool                MIsVisible        = true;
  bool                MIsActive         = true;
  bool                MIsInteracting    = false;

  bool                MIsDisabled       = false;
  bool                MIsFocused        = false;
  bool                MIsHovering       = false;

  bool                MAutoSetCursor    = true;
  bool                MAutoHideCursor   = false;
  bool                MAutoLockCursor   = false;
  bool                MAutoSetHint      = true;
  const char*         MHint             = "";

  MIP_Color           MDisabledColor    = MIP_Color(0.5);
  double              MDisabledAlpha    = 0.5;

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

//virtual MIP_Widget*     getParent()                       { return MParent; }

  virtual MIP_BaseWindow* getOwnerWindow()                  { return MOwnerWindow; }
  virtual int32_t         getIndex()                        { return MIndex; }
  virtual MIP_DRect       getRect()                         { return MRect; }
  virtual double          getWidth()                        { return MRect.w; }
  virtual double          getHeight()                       { return MRect.h; }

//virtual MIP_Parameter*  getParameter()                    { return MParameter; }
  virtual MIP_Parameter*  getParameter(uint32_t AIndex=0)   { return MParameters[AIndex]; }

  virtual uint32_t        getNumParameters()                { return MNumParameters; }

//virtual double          getValue()                        { return MValue; }
//virtual double          getModulation()                   { return MModulation; }

  virtual double          getValue(uint32_t AIndex=0)       { return MValues[AIndex]; }
  virtual double          getModulation(uint32_t AIndex=0)  { return MModulations[AIndex]; }

  virtual int32_t         getCursor()                       { return MCursor; }

  virtual MIP_Color   getDisabledColor() { return MDisabledColor; }
  virtual double      getDisabledAlpha() { return MDisabledAlpha; }

  //virtual uint32_t        getFlags()                      { return MFlags; }
  //virtual bool            hasFlag(uint32_t AFlag)         { return MFlags & AFlag; }

  virtual bool isActive()       { return MIsActive; }
  virtual bool isVisible()      { return MIsVisible; }
  virtual bool isInteracting()  { return MIsInteracting; }

  virtual bool isDisabled()     { return MIsDisabled; }
  virtual bool isFocused()      { return MIsFocused; }
  virtual bool isHovering()     { return MIsHovering; }

  virtual bool autoSetCursor()  { return MAutoSetCursor; }
  virtual bool autoHideCursor() { return MAutoHideCursor; }
  virtual bool autoLockCursor() { return MAutoLockCursor; }

  virtual void setActive(bool AActive=true)               { MIsActive = AActive; }
  virtual void setInteracting(bool AInteracting=true)     { MIsInteracting = AInteracting; }
  virtual void setVisible(bool AVisible=true)             { MIsVisible = AVisible; }

  virtual void setDisabled(bool ADisabled=true)           { MIsDisabled = ADisabled; }
  virtual void setFocused(bool AFocused=true)             { MIsFocused = AFocused; }
  virtual void setHovering(bool AHovering=true)           { MIsHovering = AHovering; }

  virtual void setAutoSetCursor(bool AAuto=true)          { MAutoSetCursor = AAuto; }
  virtual void setAutoHideCursor(bool AAuto=true)         { MAutoHideCursor = AAuto; }
  virtual void setAutoLockCursor(bool AAuto=true)         { MAutoLockCursor = AAuto; }

  virtual void setListener(MIP_WidgetListener* AListener) { MListener = AListener; }
  virtual void setParameter(MIP_Parameter* AParameter)                  { MParameters[0] = AParameter; }
  virtual void setParameter(uint32_t AIndex, MIP_Parameter* AParameter) { MParameters[AIndex] = AParameter; }

  virtual void setNumParameters(uint32_t ANum)            { MNumParameters = ANum; }

//virtual void setValue(double AValue)                    { MValue = AValue; }
//virtual void setModulation(double AValue)               { MModulation = AValue; }

  virtual void setValue(double AValue)                    { MValues[0] = AValue; }
  virtual void setModulation(double AValue)               { MModulations[0] = AValue; }

  virtual void setValue(uint32_t AIndex,double AValue)      { MValues[AIndex] = AValue; }
  virtual void setModulation(uint32_t AIndex,double AValue) { MModulations[AIndex] = AValue; }

  virtual void setCursor(int32_t ACursor)                   { MCursor = ACursor; }

  virtual void setHint(const char* AHint)                   { MHint = AHint; }

  //virtual void setFlags(uint32_t AFlags)                  { MFlags = AFlags; }
  //virtual void setFlag(uint32_t AFlag)                    { MFlags |= AFlag; }
  //virtual void clearFlag(uint32_t AFlag)                  { MFlags &= ~AFlag; }

  virtual void setDisabledColor(MIP_Color AColor) { MDisabledColor = AColor; }
  virtual void setDisabledAlpha(double AAlpha)      { MDisabledAlpha = AAlpha; }

//------------------------------
public:
//------------------------------

  virtual void setBasePos(double AXpos, double AYpos) {
    MBaseRect.x = AXpos;
    MBaseRect.y = AYpos;
  }

  //----------

  virtual void setBaseSize(double AWidth, double AHeight) {
    MBaseRect.w = AWidth;
    MBaseRect.h = AHeight;
  }

  //----------

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

    //for (uint32_t i=0; i<MChildren.size(); i++) {
    uint32_t num = MChildren.size();
    if (num > 0) {
      for (int32_t i=num-1; i>=0; i--) {
        MIP_Widget* widget = MChildren[i];
        //if (widget->isVisible()) {
        if (widget->isActive()) {
          MIP_DRect rect = widget->getRect();
          if (rect.contains(AXpos,AYpos)) {
            MIP_Widget* child = widget;
            if (ARecursive) {
              child = widget->findChildWidget(AXpos,AYpos,ARecursive);
            }
            return child;
          }
        }
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
    MIP_DRect updaterect = AContext->updateRect;
    MIP_Painter* painter = AContext->painter;
    MIP_DRect mrect = getRect();

    uint32_t num = MChildren.size();
    if (num > 0) {

      MIP_DRect cliprect = mrect;
      cliprect.overlap(updaterect);

//      for (uint32_t i=0; i<num; i++) {
//        MIP_Widget* widget = MChildren[i];
//        if (widget->isVisible()) {
//          MIP_DRect widgetrect = widget->getRect();
//          if (widgetrect.intersects(updaterect)) {
//            //painter->pushClip(widgetrect);
//            widget->on_widget_paint(AContext);
//            //painter->popClip();
//            if (ARecursive) widget->paintChildWidgets(AContext,ARecursive);
//          } // intersect
//        } // visible
//      } // for

      for (uint32_t i=0; i<num; i++) {
        MIP_Widget* widget = MChildren[i];
        if (widget->isVisible()) {
          MIP_DRect widgetrect = widget->getRect();
          widgetrect.overlap(cliprect);
          if (widgetrect.isNotEmpty()) {
            painter->pushOverlapClip(widgetrect);
            widget->on_widget_paint(AContext);
            painter->popClip();
          } // intersect
        } // visible
      } // for

    } // num > 0

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

  virtual void moveChildWidgets(double ADeltaX, double ADeltaY, bool ARecursive=true) {
   for (uint32_t i=0; i<getNumChildWidgets(); i++) {
      MIP_Widget* widget = getChildWidget(i);
      MIP_DRect rect = widget->getRect();
      double x = rect.x + ADeltaX;
      double y = rect.y + ADeltaY;
      widget->setPos(x,y);
      //widget->setBasePos(x,y);
      if (ARecursive) widget->moveChildWidgets(ADeltaX,ADeltaY,ARecursive);
    }
  }

  //----------

//------------------------------
public:
//------------------------------

  virtual void on_widget_connect(MIP_Parameter* AParameter) {
    double value = AParameter->getValue();
    for (uint32_t i=0; i<MNumParameters; i++) {
      setValue(i,value);
    }
    //const char* name = AParameter->getName();
    //setText(name);
  }

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
    //if (hasFlag(MIP_WIDGET_FLAG_AUTO_SET_CURSOR)) {
    if (MAutoSetCursor) {
      do_widget_set_cursor(this,MCursor);
    }
    if (MAutoSetHint) {
      do_widget_set_hint(this,MHint);
    }
  }

  virtual void on_widget_leave(double AXpos, double AYpos) {
    if (MAutoSetCursor) {
      do_widget_set_cursor(this,MIP_CURSOR_DEFAULT);
    }
    if (MAutoSetHint) {
      do_widget_set_hint(this,"");
    }
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

  void do_widget_set_cursor_pos(MIP_Widget* AWidget, int32_t AXpos, int32_t AYpos) override {
    if (MListener) MListener->do_widget_set_cursor_pos(AWidget,AXpos,AYpos);
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

  void do_widget_notify(MIP_Widget* AWidget, int32_t AValue) override {
    if (MListener) MListener->do_widget_notify(AWidget,AValue);
  }

};

//----------------------------------------------------------------------
#endif
