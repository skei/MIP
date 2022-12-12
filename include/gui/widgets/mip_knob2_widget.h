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
private:
//------------------------------

  MIP_TextWidget* MLabelWidget      = nullptr;
  MIP_KnobWidget* MKnobWidget       = nullptr;
  MIP_TextWidget* MValueWidget      = nullptr;
  char            MValueString[32]  = {0};

//------------------------------
protected:
//------------------------------




//------------------------------
public:
//------------------------------

  MIP_Knob2Widget(MIP_DRect ARect, const char* AText="", double AValue=0.0)
  : MIP_PanelWidget(ARect) {

    double x = ARect.x;
    double y = ARect.y;
    double w = ARect.w;
    double h = ARect.h;

    //if (h > w) { // 'vertical'
      double kw = w;          // knob height
      double kh = w;          // knob height
      double th = (h - kh) / 2.0;  // text(s) height
      MLabelWidget = new MIP_TextWidget(MIP_DRect( x,y,       kw,th ), AText      );
      MKnobWidget  = new MIP_KnobWidget(MIP_DRect( x,y+th,    kw,kh ), "", AValue );
      MValueWidget = new MIP_TextWidget(MIP_DRect( x,y+th+kh, kw,th ), "0.0"      );
    //}
    //else { // 'horizontal'
    //}

    appendChildWidget(MLabelWidget);
    appendChildWidget(MKnobWidget);
    appendChildWidget(MValueWidget);

    setDrawBorder(false);
    setFillBackground(false);

    MLabelWidget->setDrawBorder(false);
    MLabelWidget->setFillBackground(false);

    MKnobWidget->setArcThickness(12);

    sprintf(MValueString,"%.3f",AValue);

    MValueWidget->setDrawBorder(false);
    MValueWidget->setFillBackground(false);
    MValueWidget->setText(MValueString);

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

  void setValue(double AValue) override {
    MIP_PanelWidget::setValue(AValue);
    MKnobWidget->setValue(AValue);
    MKnobWidget->redraw();
  }

  //----------

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
    //if (MListener) MListener->do_widget_update(AWidget,AMode);
    MIP_PanelWidget::do_widget_update(AWidget,AMode);
    if (AWidget == MKnobWidget) {
      double value = MKnobWidget->getValue();
      sprintf(MValueString,"%.3f",value);
      MValueWidget->setText(MValueString);
      MValueWidget->redraw();
    }

  }

};

//----------------------------------------------------------------------
#endif
