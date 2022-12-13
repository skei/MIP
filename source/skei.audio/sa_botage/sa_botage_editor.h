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
  MIP_TextWidget*       MHintWidget       = nullptr;

//------------------------------
public:
//------------------------------

  sa_botage_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray* AParameters)
  : MIP_Editor(AListener,AWidth,AHeight,AParameters) {

    //MIP_Print("%i,%i\n",AWidth,AHeight);

    double __MIP_UNUSED x,y,w,h = 0;
    double width  = AWidth;

    MRootWidget = new MIP_PanelWidget(MIP_DRect(0,0,SA_BOTAGE_EDITOR_WIDTH,SA_BOTAGE_EDITOR_HEIGHT));
    setRootWidget(MRootWidget);

//------------------------------
// header
//------------------------------

    sa_botage_header* header  = new sa_botage_header(MIP_DRect(0,0,width,SA_BOTAGE_HEADER_HEIGHT));
    MRootWidget->appendChildWidget(header);

    // hint

    MHintWidget = new MIP_TextWidget(MIP_DRect(250,50,260,25),"Hello world!");
    MRootWidget->appendChildWidget(MHintWidget);
    MHintWidget->setFillBackground(false);
    //MHintWidget->setBackgroundColor(0.3);
    MHintWidget->setDrawBorder(true);
    MHintWidget->setBorderColor(MIP_COLOR_GRAY);
    MHintWidget->setTextSize(14);
    MHintWidget->setTextColor(MIP_COLOR_LIGHT_GRAY);
    MHintWidget->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MHintWidget->setTextOffset(MIP_DRect(5,0,0,0));

//------------------------------
// waveform / buffer
//------------------------------

    MWaveformWidget = new MIP_WaveformWidget( MIP_DRect(10,90,(SA_BOTAGE_EDITOR_WIDTH - 20),60) );
    MRootWidget->appendChildWidget(MWaveformWidget);
    MWaveformWidget->setNumGrid(4);
    MWaveformWidget->setNumSubGrid(2);

    // beats

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
    MNumBeatsWidget->setHint("Number of beats in buffer");

    connect( MNumBeatsWidget,  AParameters->getItem(0) );

    // slices

    MNumSlicesWidget = new MIP_DragValueWidget( MIP_DRect(140,160,120,20));
    MRootWidget->appendChildWidget(MNumSlicesWidget);
    MNumSlicesWidget->setFillBackground(true);
    MNumSlicesWidget->setBackgroundColor(MIP_COLOR_LIGHT_GRAY);
    MNumSlicesWidget->setTextColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setTextSize(14);
    MNumSlicesWidget->setValueColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setValueSize(14);
    MNumSlicesWidget->setHint("Number of slices per beat");

    connect( MNumSlicesWidget, AParameters->getItem(1) );

    // pages

    MIP_ButtonWidget* prob_page_button = new MIP_ButtonWidget(MIP_DRect(400,160,50,20), "Prob",0);
    MRootWidget->appendChildWidget(prob_page_button);
    prob_page_button->setBackgroundColor(0.8);
    prob_page_button->setHint("Probabilities");

    MIP_ButtonWidget* seq_page_button = new MIP_ButtonWidget(MIP_DRect(460,160,50,20), "Seq",0);
    MRootWidget->appendChildWidget(seq_page_button);
    seq_page_button->setBackgroundColor(0.6);
    seq_page_button->setHint("Sequence");

    MIP_ButtonWidget* perf_page_button = new MIP_ButtonWidget(MIP_DRect(520,160,50,20), "Perf",0);
    MRootWidget->appendChildWidget(perf_page_button);
    perf_page_button->setBackgroundColor(0.6);
    perf_page_button->setHint("Perform");

    MIP_ButtonWidget* fx_page_button = new MIP_ButtonWidget(MIP_DRect(580,160,50,20), "FX",0);
    MRootWidget->appendChildWidget(fx_page_button);
    fx_page_button->setBackgroundColor(0.6);
    fx_page_button->setHint("Effects");

//------------------------------
// 1 prob
//------------------------------

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
    prob_knob->setValueAlignment(MIP_TEXT_ALIGN_CENTER);
    prob_knob->setValueOffset(0);
    prob_knob->setValueSize(22);
    prob_knob->setHint("Main probability");

    // slices

    x = 10 + 100;
    y = 200;

    MIP_TextWidget* prob_slices_label = new MIP_TextWidget( MIP_DRect(x,y+30,150,15), "Slices" );
    MRootWidget->appendChildWidget(prob_slices_label);
    prob_slices_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_slices_label->setDrawBorder(false);

    MIP_ButtonRowWidget* prob_slices = new MIP_ButtonRowWidget( MIP_DRect(x,y+45,150,20), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    MRootWidget->appendChildWidget(prob_slices);
    prob_slices->selectButton(0);
    prob_slices->setAllowZeroBits(false);
    //prob_slices->setValueIsBits(true,8);
    prob_slices->setHint("Number of slices in range");

    // loops

    MIP_TextWidget* prob_loops_label = new MIP_TextWidget( MIP_DRect(x,y+75,150,15), "Loops" );
    MRootWidget->appendChildWidget(prob_loops_label);
    prob_loops_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_loops_label->setDrawBorder(false);

    MIP_ButtonRowWidget* prob_loops = new MIP_ButtonRowWidget( MIP_DRect(x,y+90,150,20), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    MRootWidget->appendChildWidget(prob_loops);
    prob_loops->selectButton(0);
    prob_loops->setAllowZeroBits(false);
    //prob_loops->setValueIsBits(true,8);
    prob_loops->setHint("Range subdivision");

    // envelopes

    x = 10;
    y = 350;

    MIP_TextWidget* loop_env_label = new MIP_TextWidget(MIP_DRect(x,y,250,20), "Loop Env");
    MRootWidget->appendChildWidget(loop_env_label);
    loop_env_label->setFillBackground(true);
    loop_env_label->setBackgroundColor(MIP_COLOR_DARK_GRAY);
    loop_env_label->setTextColor(MIP_COLOR_LIGHT_GRAY);

    MIP_SliderWidget* loop_env_att_slider = new MIP_SliderWidget(MIP_DRect(x,y+30,120,20),"Att");
    MRootWidget->appendChildWidget(loop_env_att_slider);
    loop_env_att_slider->setHint("Loop Envelope Attack");

    MIP_SliderWidget* loop_env_rel_slider = new MIP_SliderWidget(MIP_DRect(x+130,y+30,120,20),"Rel");
    MRootWidget->appendChildWidget(loop_env_rel_slider);
    loop_env_rel_slider->setHint("Loop Envelope Release");

    y = 350 + 60;

    MIP_TextWidget* slice_env_label = new MIP_TextWidget(MIP_DRect(x,y,250,20), "Slice Env");
    MRootWidget->appendChildWidget(slice_env_label);
    slice_env_label->setFillBackground(true);
    slice_env_label->setBackgroundColor(MIP_COLOR_DARK_GRAY);
    slice_env_label->setTextColor(MIP_COLOR_LIGHT_GRAY);

    MIP_SliderWidget* slice_env_att_slider = new MIP_SliderWidget(MIP_DRect(x,y+30,120,20),"Att");
    MRootWidget->appendChildWidget(slice_env_att_slider);
    slice_env_att_slider->setHint("Slice Envelope Attack");

    MIP_SliderWidget* slice_env_rel_slider = new MIP_SliderWidget(MIP_DRect(x+130,y+30,120,20),"Rel");
    MRootWidget->appendChildWidget(slice_env_rel_slider);
    slice_env_rel_slider->setHint("Slice Envelope Release");

//------------------------------
// 2 range
//------------------------------

    x = 10 + 250 + 10 /*+ 175 + 10 + 10 + 175 + 10 */;
    y = 200;

    MIP_TextWidget* c1_header = new MIP_TextWidget(MIP_DRect(x,y,175,20),"Range");
    MRootWidget->appendChildWidget(c1_header);
    c1_header->setFillBackground(true);
    c1_header->setBackgroundColor(0.35);
    c1_header->setTextColor(MIP_COLOR_LIGHT_GRAY);

    // loop length

    MIP_KnobWidget* range_length_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(range_length_knob);
    range_length_knob->setArcThickness(7);
    range_length_knob->setHint("Loop length");

    MIP_TextWidget* range_length_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Length");
    MRootWidget->appendChildWidget(range_length_text);
    range_length_text->setDrawBorder(false);
    range_length_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    MIP_DualSliderWidget* range_length_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "range", 0.2, 0.7 );
    MRootWidget->appendChildWidget(range_length_range_slider);
    //range_length_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_length_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    range_length_range_slider->setHint("Loop min/max");

    // loop speed

    y = 245;

    MIP_KnobWidget* range_speed_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(range_speed_knob);
    range_speed_knob->setArcThickness(7);

    MIP_TextWidget* range_speed_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Speed");
    MRootWidget->appendChildWidget(range_speed_text);
    range_speed_text->setDrawBorder(false);
    range_speed_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    MIP_DualSliderWidget* range_speed_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    MRootWidget->appendChildWidget(range_speed_range_slider);
    range_speed_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_speed_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop offset

    y = 290;

    MIP_KnobWidget* range_offset_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(range_offset_knob);
    range_offset_knob->setArcThickness(7);

    MIP_TextWidget* range_offset_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Offset");
    MRootWidget->appendChildWidget(range_offset_text);
    range_offset_text->setDrawBorder(false);
    range_offset_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    MIP_DualSliderWidget* range_offset_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    MRootWidget->appendChildWidget(range_offset_range_slider);
    range_offset_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_offset_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop reverse

    y = 335;

    MIP_KnobWidget* range_reverse_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(range_reverse_knob);
    range_reverse_knob->setArcThickness(7);

    MIP_TextWidget* range_reverse_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Reverse");
    MRootWidget->appendChildWidget(range_reverse_text);
    range_reverse_text->setDrawBorder(false);
    range_reverse_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* range_reverse_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    //MRootWidget->appendChildWidget(range_reverse_range_slider);
    //range_reverse_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    //range_reverse_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop fx

    y = 380;

    MIP_KnobWidget* range_fx_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(range_fx_knob);
    range_fx_knob->setArcThickness(7);

    MIP_TextWidget* range_fx_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"FX");
    MRootWidget->appendChildWidget(range_fx_text);
    range_fx_text->setDrawBorder(false);
    range_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    MIP_DualSliderWidget* range_fx_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    MRootWidget->appendChildWidget(range_fx_range_slider);
    range_fx_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_fx_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);


//------------------------------
// 3 loop
//------------------------------

    x = 10 + 250 + 10 + 175 + 10 /* + 10 + 175 + 10 */;
    y = 200;

    MIP_TextWidget* c2_header = new MIP_TextWidget(MIP_DRect(x,y,175,20),"Loop");
    MRootWidget->appendChildWidget(c2_header);
    c2_header->setFillBackground(true);
    c2_header->setBackgroundColor(0.35);
    c2_header->setTextColor(MIP_COLOR_LIGHT_GRAY);

    // loop length

    MIP_KnobWidget* loop_length_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(loop_length_knob);
    loop_length_knob->setArcThickness(7);

    MIP_TextWidget* loop_length_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Length");
    MRootWidget->appendChildWidget(loop_length_text);
    loop_length_text->setDrawBorder(false);
    loop_length_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    MIP_DualSliderWidget* loop_length_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    MRootWidget->appendChildWidget(loop_length_range_slider);
    loop_length_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_length_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop speed

    y = 245;

    MIP_KnobWidget* loop_speed_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(loop_speed_knob);
    loop_speed_knob->setArcThickness(7);

    MIP_TextWidget* loop_speed_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Speed");
    MRootWidget->appendChildWidget(loop_speed_text);
    loop_speed_text->setDrawBorder(false);
    loop_speed_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    MIP_DualSliderWidget* loop_speed_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    MRootWidget->appendChildWidget(loop_speed_range_slider);
    loop_speed_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_speed_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop offset

    y = 290;

    MIP_KnobWidget* loop_offset_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(loop_offset_knob);
    loop_offset_knob->setArcThickness(7);

    MIP_TextWidget* loop_offset_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Offset");
    MRootWidget->appendChildWidget(loop_offset_text);
    loop_offset_text->setDrawBorder(false);
    loop_offset_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    MIP_DualSliderWidget* loop_offset_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    MRootWidget->appendChildWidget(loop_offset_range_slider);
    loop_offset_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_offset_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop reverse

    y = 335;

    MIP_KnobWidget* loop_reverse_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(loop_reverse_knob);
    loop_reverse_knob->setArcThickness(7);

    MIP_TextWidget* loop_reverse_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Reverse");
    MRootWidget->appendChildWidget(loop_reverse_text);
    loop_reverse_text->setDrawBorder(false);
    loop_reverse_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* loop_reverse_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    //MRootWidget->appendChildWidget(loop_reverse_range_slider);
    //loop_reverse_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    //loop_reverse_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop fx

    y = 380;

    MIP_KnobWidget* loop_fx_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"knob", 0.0);
    MRootWidget->appendChildWidget(loop_fx_knob);
    loop_fx_knob->setArcThickness(7);

    MIP_TextWidget* loop_fx_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"FX");
    MRootWidget->appendChildWidget(loop_fx_text);
    loop_fx_text->setDrawBorder(false);
    loop_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    MIP_DualSliderWidget* loop_fx_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.2, 0.7 );
    MRootWidget->appendChildWidget(loop_fx_range_slider);
    loop_fx_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_fx_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

//------------------------------
// footer
//------------------------------

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
    MWaveformWidget->setNumSubGrid(slices);
//    double scale = 1.0;
//    double aspect = (double)MEditorWidth / (double)MEditorHeight;
//    if (aspect >= MAspectRatio) scale = (double)MEditorHeight / (double)MInitialHeight;
//    else scale = (double)MEditorWidth / (double)MInitialWidth;
//    MWindow->setWindowScale(scale);
    //MIP_Print("%i,%i\n",MEditorWidth,MEditorHeight);
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
    if (strcmp(AHint,MHintWidget->getText()) != 0) {
      MHintWidget->setText(AHint);
      MHintWidget->redraw();
    }
    MIP_Editor::do_widget_set_hint(AWidget,AHint);
  }

};

//----------------------------------------------------------------------
#endif
