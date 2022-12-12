#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

//----------

#include "sa_botage_widgets.h"

//----------------------------------------------------------------------

const char* prob_slices_txt[] = {
  "1", "2", "3", "4", "5", "6", "7", "8"
};

//----------------------------------------------------------------------

class sa_botage_editor
: public MIP_Editor {

  friend class sa_botage_plugin;

//------------------------------
private:
//------------------------------

  MIP_PanelWidget*      MRootWidget       = nullptr;
  MIP_WaveformWidget*   MWaveformWidget   = nullptr;
  MIP_DragValueWidget*  MNumBeatsWidget   = nullptr;
  MIP_DragValueWidget*  MNumSlicesWidget  = nullptr;

//------------------------------
public:
//------------------------------

  sa_botage_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray* AParameters)
  : MIP_Editor(AListener,AWidth,AHeight,AParameters) {

    double __MIP_UNUSED x,y,w,h = 0;
    double width  = AWidth;

    MRootWidget = new MIP_PanelWidget(MIP_DRect(0,0,SA_BOTAGE_EDITOR_WIDTH,SA_BOTAGE_EDITOR_HEIGHT));
    setRootWidget(MRootWidget);

    //--------------------
    // sa_header
    //

    sa_botage_header* header  = new sa_botage_header(MIP_DRect(0,0,width,SA_BOTAGE_HEADER_HEIGHT));
    MRootWidget->appendChildWidget(header);

    //--------------------
    // waveform
    //

    MWaveformWidget = new MIP_WaveformWidget( MIP_DRect(10,90,(SA_BOTAGE_EDITOR_WIDTH - 20),60) );
    MRootWidget->appendChildWidget(MWaveformWidget);
    MWaveformWidget->setNumGrid(4);
    MWaveformWidget->setNumSubGrid(2);

    //--------------------
    // buffer
    //

    //w = ((SA_BOTAGE_EDITOR_WIDTH - 30) / 2);
    //x = 10 + w + 10;

    MNumBeatsWidget = new MIP_DragValueWidget( MIP_DRect(10,160,120,20));
    MRootWidget->appendChildWidget(MNumBeatsWidget);
    MNumBeatsWidget->setFillBackground(true);
    MNumBeatsWidget->setBackgroundColor(MIP_COLOR_LIGHT_GRAY);
    MNumBeatsWidget->setTextColor(MIP_COLOR_BLACK);
    MNumBeatsWidget->setTextSize(14);
    MNumBeatsWidget->setValueColor(MIP_COLOR_BLACK);
    MNumBeatsWidget->setValueSize(14);

    MNumSlicesWidget = new MIP_DragValueWidget( MIP_DRect(140,160,120,20));
    MRootWidget->appendChildWidget(MNumSlicesWidget);
    MNumSlicesWidget->setFillBackground(true);
    MNumSlicesWidget->setBackgroundColor(MIP_COLOR_LIGHT_GRAY);
    MNumSlicesWidget->setTextColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setTextSize(14);
    MNumSlicesWidget->setValueColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setValueSize(14);

    connect( MNumBeatsWidget,  AParameters->getItem(0) );
    connect( MNumSlicesWidget, AParameters->getItem(1) );

    //--------------------
    // 1 : prob
    //

    x = 10;
    y = 200;

    // prob

    MIP_TextWidget* prob_header = new MIP_TextWidget(MIP_DRect(x,y,250,20),"Probability");
    MRootWidget->appendChildWidget(prob_header);
    prob_header->setFillBackground(true);
    prob_header->setBackgroundColor(MIP_COLOR_DARK_GRAY);
    prob_header->setTextColor(MIP_COLOR_LIGHT_GRAY);

    MIP_KnobWidget* prob_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,90,90), "", 0.0);
    MRootWidget->appendChildWidget(prob_knob);
    prob_knob->setArcThickness(15);
    prob_knob->setDrawValue(true);
    prob_knob->setValueSize(22);

    // slices

    MIP_TextWidget* prob_slices_label = new MIP_TextWidget( MIP_DRect(x+100,y+30,150,15), "Slices" );
    MRootWidget->appendChildWidget(prob_slices_label);
    prob_slices_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_slices_label->setDrawBorder(false);

    MIP_ButtonRowWidget* prob_slices = new MIP_ButtonRowWidget( MIP_DRect(x+100,y+45,150,20), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    MRootWidget->appendChildWidget(prob_slices);
    prob_slices->selectButton(0);
    prob_slices->setAllowZeroBits(false);
    //prob_slices->setValueIsBits(true,8);

    // loops

    MIP_TextWidget* prob_loops_label = new MIP_TextWidget( MIP_DRect(x+100,y+75,150,15), "Loops" );
    MRootWidget->appendChildWidget(prob_loops_label);
    prob_loops_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_loops_label->setDrawBorder(false);

    MIP_ButtonRowWidget* prob_loops = new MIP_ButtonRowWidget( MIP_DRect(x+100,y+90,150,20), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    MRootWidget->appendChildWidget(prob_loops);
    prob_loops->selectButton(0);
    prob_loops->setAllowZeroBits(false);
    //prob_loops->setValueIsBits(true,8);

    //MIP_Knob2Widget* knob2 = new MIP_Knob2Widget( MIP_DRect(10,200,60,100), "Knob2", 0.25);
    //ARootWidget->appendChildWidget(knob2);

    //--------------------
    // 2
    //

    x = 10 + 250 + 10 /*+ 175 + 10 + 10 + 175 + 10 */;
    y = 200;

    MIP_TextWidget* c1_header = new MIP_TextWidget(MIP_DRect(x,y,175,20),"Range");
    MRootWidget->appendChildWidget(c1_header);
    c1_header->setFillBackground(true);
    c1_header->setBackgroundColor(MIP_COLOR_DARK_GRAY);
    c1_header->setTextColor(MIP_COLOR_LIGHT_GRAY);

    //

    MIP_DualSliderWidget* ds1 = new MIP_DualSliderWidget( MIP_DRect(x,y+30,170,20), "Dual", 0.2, 0.7 );
    MRootWidget->appendChildWidget(ds1);
    ds1->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    ds1->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    //ds1->setDrawBorder(true);

    //--------------------
    // 3
    //

    x = 10 + 250 + 10 + 175 + 10 /* + 10 + 175 + 10 */;
    y = 200;

    MIP_TextWidget* c2_header = new MIP_TextWidget(MIP_DRect(x,y,175,20),"Loop");
    MRootWidget->appendChildWidget(c2_header);
    c2_header->setFillBackground(true);
    c2_header->setBackgroundColor(MIP_COLOR_DARK_GRAY);
    c2_header->setTextColor(MIP_COLOR_LIGHT_GRAY);

    //--------------------
    // footer
    //

    //double height = ARootWidget->getHeight();
    //sa_botage_footer* footer  = new sa_botage_footer(MIP_DRect(0,(height - SA_BOTAGE_FOOTER_HEIGHT),width,SA_BOTAGE_FOOTER_HEIGHT));
    //ARootWidget->appendChildWidget(footer);

  }

  //----------

  virtual ~sa_botage_editor() {
  }

//------------------------------
public:
//------------------------------

  bool show() override {
    uint32_t beats = MParameters->getItem(0)->getValue();
    uint32_t slices = MParameters->getItem(1)->getValue();
    MWaveformWidget->setNumGrid(beats);
    MWaveformWidget->setNumGrid(slices);
    return MIP_Editor::show();
  }

  //----------

  void do_widget_update(MIP_Widget* AWidget, uint32_t AMode=0) override {
    MIP_Editor::do_widget_update(AWidget,AMode);
    if (AWidget == MNumBeatsWidget) {
      double value = AWidget->getValue();
      MWaveformWidget->setNumGrid(value);
      MWaveformWidget->redraw();
    }
    if (AWidget == MNumSlicesWidget) {
      double value = AWidget->getValue();
      MWaveformWidget->setNumSubGrid(value);
      MWaveformWidget->redraw();
    }
  }

  //----------

  void do_widget_set_hint(MIP_Widget* AWidget, const char* AHint) override {
    MIP_Editor::do_widget_set_hint(AWidget,AHint);
  }

};

//----------------------------------------------------------------------
#endif
