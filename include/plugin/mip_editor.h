#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

/*

  open

   1. clap_plugin_gui->is_api_supported(), check what can work
   2. clap_plugin_gui->create(), allocates gui resources
   3. if the plugin window is floating
   4.    -> clap_plugin_gui->set_transient()
   5.    -> clap_plugin_gui->suggest_title()
   6. else
   7.    -> clap_plugin_gui->set_scale()
   8.    -> clap_plugin_gui->can_resize()
   9.    -> if resizable and has known size from previous session, clap_plugin_gui->set_size()
  10.    -> else clap_plugin_gui->get_size(), gets initial size
  11.    -> clap_plugin_gui->set_parent()
  12. clap_plugin_gui->show()
  13. clap_plugin_gui->hide()/show() ...
  14. clap_plugin_gui->destroy() when done with the gui

  resize (drag, if embedded)):

  1. Only possible if clap_plugin_gui->can_resize() returns true
  2. Mouse drag -> new_size
  3. clap_plugin_gui->adjust_size(new_size) -> working_size
  4. clap_plugin_gui->set_size(working_size)

  resize (initiated by the plugin, if embedded):

  1. Plugins calls clap_host_gui->request_resize()
  2. If the host returns true the new size is accepted,
     the host doesn't have to call clap_plugin_gui->set_size().
     If the host returns false, the new size is rejected.

*/

//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"
//#include "plugin/mip_editor_window.h"
#include "gui/mip_window.h"
#include "gui/mip_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_EditorListener {
public:
  virtual void on_editor_parameter_change(uint32_t AIndex, double AValue) {}
  virtual void on_editor_editor_resize() {}
  //virtual void on_editor_gesture_begin() {}
  //virtual void on_editor_gesture_end() {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_WidgetListener {

  friend class MIP_Plugin;

//------------------------------
private:
//------------------------------

//------------------------------
protected:
//------------------------------

  MIP_Window*         MWindow         = nullptr;
  MIP_EditorListener* MListener       = nullptr;

  intptr_t            MParentWindow   = 0;
  bool                MIsEditorOpen   = false;

  uint32_t            MEditorWidth    = 0;
  uint32_t            MEditorHeight   = 0;
  double              MEditorScale    = 1.0;

  uint32_t            MInitialWidth   = 0;
  uint32_t            MInitialHeight  = 0;
  double              MAspectRatio    = 0.0;

  MIP_ParameterArray* MParameters     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray* AParameters) {
    //MIP_Print("%i,%i\n",AWidth,AHeight);
    MListener       = AListener;
    MInitialWidth   = AWidth;
    MInitialHeight  = AHeight;
    MEditorWidth    = AWidth;
    MEditorHeight   = AHeight;
    MParameters     = AParameters;
    MAspectRatio    = (double)AWidth / (double)AHeight;
    MWindow         = new MIP_Window(AWidth,AHeight);

    // setup parameters & widgets

  }

  //----------

  virtual ~MIP_Editor() {
    if (MWindow) {
      delete MWindow;
    }
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t    getWidth()        { return MEditorWidth; }
  virtual uint32_t    getHeight()       { return MEditorHeight; }
  virtual double      getScale()        { return MEditorScale; }
  virtual double      getAspectRatio()  { return MAspectRatio; }
  virtual MIP_Window* getWindow()       { return MWindow; }
  virtual bool        isEditorOpen()    { return MIsEditorOpen; }

//------------------------------
public:
//------------------------------

  //  virtual void setWidth(uint32_t AWidth) { MWidth = AWidth; }
  //  virtual void setHeight(uint32_t AHeight) { MHeight = AHeight; }
  //  virtual void setSize(uint32_t AWidth, uint32_t AHeight) { MWidth = AWidth; MHeight = AHeight; }
  //  virtual void setScale(double AScale) { MScale = AScale; }

  //----------

  virtual void setListener(MIP_EditorListener* AListener) {
    MListener = AListener;
  }

  //----------

  virtual void setParent(intptr_t AParent) {
    MParentWindow = AParent;
  }

  //----------

  // setEditorWidget?

  virtual void setRootWidget(MIP_Widget* AWidget, MIP_WidgetListener* AListener=nullptr) {
    if (MWindow) {
      if (AListener) MWindow->setRootWidget(AWidget,AListener);
      else MWindow->setRootWidget(AWidget,this);
    }
  }

  //----------

  virtual void connect(MIP_Widget* AWidget, MIP_Parameter* AParameter) {
    //MIP_Print("AWidget %p AParameter %p\n",AWidget,AParameter);
    AWidget->setParameter(AParameter);
    AParameter->setWidget(AWidget);
    double value = AParameter->getValue();
    AWidget->setValue(value);
    AWidget->on_widget_connect(AParameter);
    //double nv = AParameter->normalizeValue(v);
    //AWidget->setValue(nv);
  }

  //----------

  virtual void setParameters(MIP_ParameterArray* AParameters) {
    MParameters = AParameters;
  }

  //----------

  virtual void redraw(MIP_Widget* AWidget) {
    if (MWindow && MIsEditorOpen) {
      MIP_PRINT;
    }
  }

//------------------------------
public:
//------------------------------

  virtual MIP_ParameterArray* getParameters() {
    return MParameters;
  }


//------------------------------
public:
//------------------------------

  // called by plugin

  virtual void updateParameter(uint32_t AIndex, double AValue, bool ARedraw=true) {
    MIP_Parameter* parameter = MParameters->getItem(AIndex);
    if (parameter) {
      MIP_Widget* widget = parameter->getWidget();
      if (widget) {
        // [de-]normalize value
        //double v = parameter->normalizeValue(AValue);
        widget->setValue(AValue);
        if (ARedraw) widget->redraw(/*MIP_WIDGET_REDRAW_VALUE*/);
      }
    }
  }

  //----------

  virtual void updateModulation(uint32_t AIndex, double AValue, bool ARedraw=true) {
    MIP_Parameter* parameter = MParameters->getItem(AIndex);
    if (parameter) {
      MIP_Widget* widget = parameter->getWidget();
      if (widget) {
        // [de-]normalize value
        widget->setModulation(AValue);
        if (ARedraw) widget->redraw(/*MIP_WIDGET_REDRAW_MODULATION*/);
      }
    }
  }

//------------------------------
public: // clap.gui
//------------------------------

  virtual bool setScale(double scale) {
    //MIP_Print("scale %.3f -> true\n",scale);
    MEditorScale = scale;
    return true;
  }

  //----------

  virtual bool getSize(uint32_t *width, uint32_t *height) {
    //MIP_Print("editor %i,%i\n",MEditorWidth,MEditorHeight);
    *width = MEditorWidth;
    *height = MEditorHeight;
    return true;
  }

  //----------

  virtual bool canResize() {
    //MIP_Print("-> true\n");
    return true;
  }

  //----------

  virtual bool getResizeHints(clap_gui_resize_hints_t *hints) {
    //MIP_Print(".. -> true\n");
    hints->can_resize_horizontally  = true;
    hints->can_resize_vertically    = true;
    hints->preserve_aspect_ratio    = false;
    hints->aspect_ratio_width       = MInitialWidth;
    hints->aspect_ratio_height      = MInitialHeight;
    return true;
  }

  //----------

  // see also: MIP_Window::on_window_resize

  virtual bool adjustSize(uint32_t *width, uint32_t *height) {
    /*
    double w = *width;
    double h = *height;
    if (MAspectRatio > 0) {
      if (h > 0) {
        double aspect = w / h;
        if (aspect >= MAspectRatio) w = h * MAspectRatio;
        else h = w / MAspectRatio;
      }
    }
    // avoids a 'resize dance'..
    // jiggling down half a pixels a time in width/height,
    // until aspect ratio for both with and height agree..
    w = ceil(w);
    h = ceil(h);
    MIP_Print("%i, %i -> %i, %i\n",*width,*height,(int)w,(int)h);
    *width = w;
    *height = h;
    */
    return true;
  }

  //----------

  /*
    plugin windows doesn't receive the on_window_resize event,
    so we 'simulate' the resize, by setting the window size
    and scale here..
    and also, resize the root widget
  */

  virtual bool setSize(uint32_t width, uint32_t height) {
    //MIP_Print("%i,%i\n",width,height);
    //MEditorWidth = width;
    //MEditorHeight = height;
    if (MWindow) {
      if (MIsEditorOpen) {
        MWindow->setSize(width,height);
      }
      MIP_Widget* root_widget = MWindow->getRootWidget();
      if (root_widget) {
        //root_widget->setPos(0,0);
        root_widget->setSize(width,height);
        //MWindow->invalidate(0,0,width,height);
      }
      if (MInitialWidth > 0) {
        double s = (double)width / (double)MInitialWidth;
        MWindow->setWindowScale(s);
      }
      MIP_Painter* painter = MWindow->getWindowPainter();
      painter->setClipRect(MIP_DRect(0,0,width,height));
      //MWindow->on_window_resize(width,height);
      //MIP_Painter* painter = MWindow->getWindowPainter();
      //painter->resetClip();
      MEditorWidth = width;
      MEditorHeight = height;

      // will not redraw when resizing without this
      #ifdef MIP_WIN32
      if (MIsEditorOpen) {
        MWindow->invalidate(0,0,width,height);
      }
      #endif

    }
    return true;
  }

  //----------

  virtual bool setParent(const clap_window_t *window) {
    //MIP_Print("\n");
    #ifdef MIP_LINUX
      MParentWindow = window->x11;
      if (MWindow) MWindow->reparent(MParentWindow);
      return true;
    #endif
    #ifdef MIP_WIN32
      MParentWindow = (intptr_t)window->win32;
      if (MWindow) MWindow->reparent(MParentWindow);
      return true;
    #endif
  }

  //----------

  virtual bool setTransient(const clap_window_t *window) {
    //MIP_Print("-> true\n");
    return true;
  }

  //----------

  virtual void suggestTitle(const char *title) {
    //MIP_Print("title '%s'\n",title);
    if (MWindow) MWindow->setTitle(title);
  }

  //----------

  virtual bool show() {
    if (MWindow) {
      MWindow->open();
      //MWindow->invalidate(0,0,MWidth,MHeight);
      MWindow->startEventThread();
      MIsEditorOpen = true;
      return true;
    }
    return false;
  }

  //----------

  virtual bool hide() {
    if (MWindow) {
      MWindow->stopEventThread();
      MWindow->close();
      MIsEditorOpen = false;
      return true;
    }
    return false;
  }

//------------------------------
public: // widget listener
//------------------------------

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
    //MIP_PRINT;
    MIP_Parameter* parameter = AWidget->getParameter();
    //MIP_Print("parameter %p\n",parameter);
    if (parameter) {
      //MIP_PRINT;
      uint32_t index = parameter->getIndex();
      //double value = parameter->getValue();
      double value = AWidget->getValue();
      if (MListener) {
        //MIP_Print("%i = %f\n",index,value);
        MListener->on_editor_parameter_change(index,value);
      }
    }
    if (MWindow) MWindow->do_widget_update(AWidget,AMode);
  }

  //----------

  void do_widget_redraw(MIP_Widget* AWidget, uint32_t AMode=0) override {
    //MIP_PRINT;
    //MIP_Print("mode: %i\n",AMode);
    if (MWindow) MWindow->do_widget_redraw(AWidget,AMode);
  }

  //----------

  void do_widget_set_cursor(MIP_Widget* AWidget, int32_t ACursor) override {
    //MIP_Print("cursor: %i\n",ACursor);
    if (MWindow) MWindow->do_widget_set_cursor(AWidget,ACursor);
  }

  //----------

  void do_widget_set_key_capture(MIP_Widget* AWidget, uint32_t AMode) override {
    //MIP_Print("mode: %i\n",AMode);
    if (MWindow) MWindow->do_widget_set_key_capture(AWidget,AMode);
  }

  //----------

  void do_widget_set_modal(MIP_Widget* AWidget, uint32_t AMode) override {
    //MIP_Print("mode: %i\n",AMode);
    if (MWindow) MWindow->do_widget_set_modal(AWidget,AMode);
  }

  //----------

  void do_widget_set_hint(MIP_Widget* AWidget, const char* AHint) override {
    //MIP_Print("hint: '%s'\n",AHint);
    if (MWindow) MWindow->do_widget_set_hint(AWidget,AHint);
  }

  //----------

//  void do_widget_set_timer(MIP_Widget* AWidget, uint32_t ATime) override {
//    //MIP_Print("time: %i\n",ATime);
//    if (MWindow) MWindow->do_widget_set_timer(AWidget,ATime);
//  }

  void do_widget_notify(MIP_Widget* AWidget, int32_t AValue) override {
    if (MWindow) MWindow->do_widget_notify(AWidget,AValue);
  }

};

//----------------------------------------------------------------------
#endif
