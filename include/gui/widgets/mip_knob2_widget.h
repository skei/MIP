#ifndef mip_knob2_widget_included
#define mip_knob2_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_knob_widget.h"
#include "gui/widgets/mip_text_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Knob2Widget
: public MIP_PanelWidget {

//------------------------------
protected:
//------------------------------

  MIP_TextWidget* MLabelWidget      = nullptr;
  MIP_KnobWidget* MKnobWidget       = nullptr;
  MIP_TextWidget* MValueWidget      = nullptr;
  char            MValueString[32]  = {0};

  bool MDrawLabel = true;
  bool MDrawValue = true;

//------------------------------
protected:
//------------------------------




//------------------------------
public:
//------------------------------

  MIP_Knob2Widget(MIP_DRect ARect, const char* AText="", double AValue=0.0, bool ADrawLabel=true, bool ADrawValue=true)
  : MIP_PanelWidget(ARect) {

    setDrawBorder(false);
    setFillBackground(false);

    MDrawLabel = ADrawLabel;
    MDrawValue = ADrawValue;

    double x = ARect.x;
    double y = ARect.y;
    double w = ARect.w;
    double h = ARect.h;

    double kw = w;          // knob height
    double kh = w;          // knob height
    double th = (h - kh);   // text(s) height
    if (ADrawLabel && ADrawValue) th *= 0.5;

    if (ADrawLabel) {
      MLabelWidget = new MIP_TextWidget(MIP_DRect( x,y, kw,th ), AText );
      appendChildWidget(MLabelWidget);
      MLabelWidget->setDrawBorder(false);
      MLabelWidget->setFillBackground(false);
    }

    y += th;

    MKnobWidget  = new MIP_KnobWidget(MIP_DRect( x,y,    kw,kh ), "", AValue );
    appendChildWidget(MKnobWidget);
    MKnobWidget->setArcThickness(12);

    y += kh;

    if (ADrawValue) {
      MValueWidget = new MIP_TextWidget(MIP_DRect( x,y, kw,th ), "0.0"      );
      appendChildWidget(MValueWidget);
      sprintf(MValueString,"%.3f",AValue);
      MValueWidget->setDrawBorder(false);
      MValueWidget->setFillBackground(false);
      MValueWidget->setText(MValueString);
    }

  }

  //----------

  virtual ~MIP_Knob2Widget() {
  }

//------------------------------
public:
//------------------------------

  MIP_TextWidget* getLabelWidget() { return MLabelWidget; }
  MIP_KnobWidget* getKnobWidget() { return MKnobWidget; }
  MIP_TextWidget* getValueWidget() { return MValueWidget; }

//------------------------------
public:
//------------------------------

  MIP_Widget* getConnection(uint32_t AIndex) override {
    return MKnobWidget;
  }

  //----------

  void setValue(double AValue) override {
    MIP_PanelWidget::setValue(AValue);
    MKnobWidget->setValue(AValue);
    MKnobWidget->redraw();
  }

  //----------

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
    //if (MWidgetListener) MWidgetListener->do_widget_update(AWidget,AMode);
    MIP_PanelWidget::do_widget_update(AWidget,AMode);
    if (MDrawValue) {
      if (AWidget == MKnobWidget) {
        double value = MKnobWidget->getValue();
        sprintf(MValueString,"%.3f",value);
        MValueWidget->setText(MValueString);
        MValueWidget->redraw();
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
