#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/mip_rect.h"
#include "plugin/mip_descriptor.h"
#include "plugin/mip_editor.h"
#include "gui/mip_widget.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
// editor listener
//
//----------------------------------------------------------------------

class MIP_EditorListener {
public:
  virtual void on_editor_parameter(uint32_t AIndex, float AValue) {}
  virtual void on_editor_resize(uint32_t AWidth, uint32_t AHeight) {}
};

//----------------------------------------------------------------------
//
// editor window
//
//----------------------------------------------------------------------

class MIP_EditorWindow
: public MIP_Window {

//------------------------------
private:
//------------------------------

  MIP_Descriptor*     MDescriptor = nullptr;
  MIP_EditorListener* MListener   = nullptr;  // editor

//------------------------------
public:
//------------------------------

  MIP_EditorWindow(MIP_Descriptor* ADescriptor, MIP_EditorListener* AListener, int32_t AWidth, int32_t AHeight, void* AParent)
  : MIP_Window(AWidth,AHeight,AParent) {
    MIP_PRINT;
    MDescriptor = ADescriptor;
    MListener = AListener;
  }

//------------------------------
public: // MIP_Window
//------------------------------

  void do_widget_update(MIP_Widget* AWidget) override {
    MIP_PRINT;
    if (MListener) {
      int32_t index = AWidget->getParameterIndex();
      if ((index >= 0) && (index < (int32_t)MDescriptor->getNumParameters())) {
        float value = AWidget->getValue();
        MListener->on_editor_parameter(index,value);
      }
    }
    MIP_Window::do_widget_update(AWidget);
  }

};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class MIP_Editor
: public MIP_EditorListener {

//------------------------------
protected:
//------------------------------

  MIP_EditorListener* MListener           = nullptr; // instance
  MIP_Descriptor*     MDescriptor         = nullptr;
  MIP_Widget**        MParameterToWidget  = nullptr;
  const char*         MAttachedName       = nullptr;
  void*               MAttachedParent     = nullptr;
  MIP_EditorWindow*   MWindow             = nullptr;
  bool                MIsOpen             = false;
  float               MScale              = 1.0;

//------------------------------
public:
//------------------------------

  MIP_Editor(MIP_EditorListener* AListener, MIP_Descriptor* ADescriptor) {
    MIP_PRINT;
    MListener   = AListener;
    MDescriptor = ADescriptor;
    uint32_t num = MDescriptor->getNumParameters();
    uint32_t size = sizeof(MIP_Widget*) * num;
    MParameterToWidget = (MIP_Widget**)malloc(size);
    memset(MParameterToWidget,0,size);
  }

  //----------

  virtual ~MIP_Editor() {
    MIP_PRINT;
    if (MParameterToWidget) free(MParameterToWidget);
    if (MWindow) delete MWindow;

  }

//------------------------------
public:
//------------------------------

  bool        isOpen()    { return MIsOpen; }
  MIP_Window* getWindow() { return MWindow; }

  void setScale(float AScale) { MScale = AScale; }
  void resize(uint32_t Awidth, uint32_t AHeight) {}

//------------------------------
public:
//------------------------------

  bool attach(const char* AName, void* AParent) {
    MIP_PRINT;
    MAttachedName = AName;
    MAttachedParent = AParent;
    MIP_FRect rect = MDescriptor->getEditorRect();
    MWindow = new MIP_EditorWindow(MDescriptor,this,rect.w,rect.h,MAttachedParent);
    return true;
  }

  //----------

  //void detach() {
  //}

  //----------

  bool open() {
    MIP_PRINT;
    MWindow->open();
    MIsOpen = true;
    return true;
  }

  //----------

  void close() {
    MIP_PRINT;
    MWindow->close();
    MIsOpen = false;
  }

//------------------------------
public:
//------------------------------

  void updateParameter(uint32_t AIndex, float AValue, bool ARedraw=false) {
    MIP_PRINT;
    MIP_Widget* widget = MParameterToWidget[AIndex];
    if (widget) {
      widget->updateValue(AValue);
      if (ARedraw) widget->redraw();
    }
  }

  //----------

  void redrawParameter(uint32_t AIndex) {
    MIP_PRINT;
    MIP_Widget* widget = MParameterToWidget[AIndex];
    if (widget) widget->redraw();
  }

  //----------

  void connectParameter(MIP_Widget* AWidget, int32_t AParameterIndex) {
    MIP_PRINT;
    MParameterToWidget[AParameterIndex] = AWidget;
    AWidget->setParameterIndex(AParameterIndex);
    AWidget->on_connect(AParameterIndex);
  }

//------------------------------
public: // MIP_EditorListener
//------------------------------

  // updated from gui
  // pass on to instance

  void on_editor_parameter(uint32_t AIndex, float AValue) override {
    MIP_PRINT;
    if (MListener) MListener->on_editor_parameter(AIndex,AValue);
  }

  //----------

  void on_editor_resize(uint32_t AWidth, uint32_t AHeight) override {
    MIP_PRINT;
    if (MListener) MListener->on_editor_resize(AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
#endif
