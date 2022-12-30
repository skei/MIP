#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

//----------

//#include "sa_botage_widgets.h"
//#include "../../../data/img/botage_img.h"

#define BACKGROUNDCOLOR 0.2

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
  //MIP_WaveformWidget*   MWaveformWidget   = nullptr;
  MIP_CircularWaveformWidget*   MWaveformWidget   = nullptr;
  MIP_DragValueWidget*  MNumBeatsWidget   = nullptr;
  MIP_DragValueWidget*  MNumSlicesWidget  = nullptr;
  MIP_TextWidget*       MHintWidget       = nullptr;

  MIP_PagesWidget*      MPages            = nullptr;

  MIP_ButtonWidget*     prob_page_button  = nullptr;
  MIP_ButtonWidget*     fx_page_button    = nullptr;
  MIP_ButtonWidget*     seq_page_button   = nullptr;
  MIP_ButtonWidget*     perf_page_button  = nullptr;

  uint32_t MCurrentPage = 0;

  //

  MIP_Color MReadPosColor   = MIP_Color( 0.0, 1.0, 0.0, 0.8 );
  MIP_Color MWritePosColor  = MIP_Color( 1.0, 0.0, 0.0, 1.0 );

  double    MReadPosWidth   = 2.0;
  double    MWritePosWidth  = 2.5;

  MIP_Color MRangeColor     = MIP_Color( 0.0, 0.4, 0.0, 0.5 );
  MIP_Color MLoopColor      = MIP_Color( 0.2, 0.6, 0.2, 0.7 );
  MIP_Color MSliceColor     = MIP_Color( 0.4, 0.4, 0.4, 0.5 );

  //

  MIP_KnobWidget*       MMainProbWidget           = nullptr;

  MIP_ButtonRowWidget*  MMainSlicesWidget         = nullptr;
  MIP_ButtonRowWidget*  MMainLoopsWidget          = nullptr;

  MIP_KnobWidget*       MRangeSizeWidget          = nullptr;
  MIP_KnobWidget*       MRangeSpeedWidget         = nullptr;
  MIP_KnobWidget*       MRangeOffsetWidget        = nullptr;
  MIP_KnobWidget*       MRangeReverseWidget       = nullptr;

  MIP_KnobWidget*       MLoopSizeWidget           = nullptr;
  MIP_KnobWidget*       MLoopSpeedWidget          = nullptr;
  MIP_KnobWidget*       MLoopOffsetWidget         = nullptr;
  MIP_KnobWidget*       MLoopReverseWidget        = nullptr;

  MIP_DualSliderWidget* MRangeSizeValueWidget     = nullptr;
  MIP_DualSliderWidget* MRangeSpeedValueWidget    = nullptr;
  MIP_DualSliderWidget* MRangeOffsetValueWidget   = nullptr;
  //MIP_DualSliderWidget* MRangeReverseValueWidget  = nullptr;

  MIP_DualSliderWidget* MLoopSizeValueWidget      = nullptr;
  MIP_DualSliderWidget* MLoopSpeedValueWidget     = nullptr;
  MIP_DualSliderWidget* MLoopOffsetValueWidget    = nullptr;
  //MIP_DualSliderWidget* MLoopReverseValueWidget   = nullptr;

//------------------------------
public:
//------------------------------

  sa_botage_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray* AParameters, const clap_plugin_descriptor_t* descriptor)
  : MIP_Editor(AListener,AWidth,AHeight,AParameters) {

    //MIP_Print("%i,%i\n",AWidth,AHeight);

    double __MIP_UNUSED x,y,w,h = 0;
    double width  = AWidth;

    MRootWidget = new MIP_PanelWidget(MIP_DRect(0,0,SA_BOTAGE_EDITOR_WIDTH,SA_BOTAGE_EDITOR_HEIGHT));
    setRootWidget(MRootWidget);
    MRootWidget->setFillBackground(true);
    MRootWidget->setBackgroundColor(BACKGROUNDCOLOR);
    MRootWidget->setDrawBorder(true);
    MRootWidget->setBorderColor(0.5);

//------------------------------
// header
//------------------------------

    MIP_SAHeaderWidget* header  = new MIP_SAHeaderWidget(MIP_DRect(0,0,width,80),descriptor);
    MRootWidget->appendChildWidget(header);

    // icons

    MIP_TextWidget* c_icon = new MIP_TextWidget(MIP_DRect(475,65,10,10),"c");
    MRootWidget->appendChildWidget(c_icon);
    c_icon->setCursor(MIP_CURSOR_FINGER);
    c_icon->setTextSize(9);
    c_icon->setTextColor(0.8);
    c_icon->setDrawBorder(false);
    c_icon->setBorderColor(0.5);
    c_icon->setFillBackground(true);
    c_icon->setBackgroundColor(0.2);
    c_icon->setHint("Config");

    MIP_TextWidget* i_icon = new MIP_TextWidget(MIP_DRect(460,65,10,10),"i");
    MRootWidget->appendChildWidget(i_icon);
    i_icon->setCursor(MIP_CURSOR_FINGER);
    i_icon->setTextSize(9);
    i_icon->setTextColor(0.8);
    i_icon->setDrawBorder(false);
    i_icon->setBorderColor(0.5);
    i_icon->setFillBackground(true);
    i_icon->setBackgroundColor(0.2);
    i_icon->setHint("Info");

    // hint

    MHintWidget = new MIP_TextWidget(MIP_DRect(210,65,245,10),"Hello world!");
    MRootWidget->appendChildWidget(MHintWidget);
    MHintWidget->setFillBackground(true);
    MHintWidget->setBackgroundColor(0.23);
    MHintWidget->setDrawBorder(false);
    MHintWidget->setBorderColor(MIP_COLOR_GRAY);
    MHintWidget->setTextSize(9);
    MHintWidget->setTextColor(0.8);
    MHintWidget->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MHintWidget->setTextOffset(MIP_DRect(5,0,0,0));

//------------------------------
// waveform / buffer
//------------------------------

    MWaveformWidget = new MIP_CircularWaveformWidget( MIP_DRect(10,90,230,230), 256 );
    MRootWidget->appendChildWidget(MWaveformWidget);
    MWaveformWidget->setGrid(4,2);
    MWaveformWidget->setFillBackground(false);;
    MWaveformWidget->setWaveformWidth(1.5);
    MWaveformWidget->setGridColor(0.55, 0.4);
    MWaveformWidget->setZeroLineColor(0.6);

    // beats

    MNumBeatsWidget = new MIP_DragValueWidget( MIP_DRect(10,330,110,16),"Beats");
    MRootWidget->appendChildWidget(MNumBeatsWidget);
    MNumBeatsWidget->setFillBackground(true);
    MNumBeatsWidget->setBackgroundColor(0.4);
    MNumBeatsWidget->setTextColor(MIP_COLOR_BLACK);
    MNumBeatsWidget->setTextSize(11);
    MNumBeatsWidget->setValueColor(MIP_COLOR_BLACK);
    MNumBeatsWidget->setValueSize(11);
    MNumBeatsWidget->setHint("Number of beats in buffer");
    MNumBeatsWidget->setIValueColor(MIP_COLOR_WHITE);

  connect( MNumBeatsWidget,  AParameters->getItem(PAR_NUM_BEATS) );

    // slices

    MNumSlicesWidget = new MIP_DragValueWidget( MIP_DRect(130,330,110,16),"Slices");
    MRootWidget->appendChildWidget(MNumSlicesWidget);
    MNumSlicesWidget->setFillBackground(true);
    MNumSlicesWidget->setBackgroundColor(0.4);
    MNumSlicesWidget->setTextColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setTextSize(11);
    MNumSlicesWidget->setValueColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setValueSize(11);
    MNumSlicesWidget->setHint("Number of slices per beat");
    MNumSlicesWidget->setIValueColor(MIP_COLOR_WHITE);

  connect( MNumSlicesWidget, AParameters->getItem(PAR_NUM_SLICES) );

    // envelopes

    MIP_EnvSliderWidget* env1_slider = new MIP_EnvSliderWidget( MIP_DRect(10,356,110,16), "Loop Env", 0.1, 0.9 );
    MRootWidget->appendChildWidget(env1_slider);
    env1_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    env1_slider->setHint("Loop Envenvelope");
    env1_slider->setBackgroundColor(0.4);
    env1_slider->setSliderColor(0.6);
    env1_slider->setTextColor(0.2);
    env1_slider->setTextSize(9);
    env1_slider->setValueColor(0.2);
    //env1_slider->setTextSize(11);

  connect( env1_slider, 0, AParameters->getItem(PAR_LOOP_ENV_ATTACK) );
  connect( env1_slider, 1, AParameters->getItem(PAR_LOOP_ENV_DECAY) );

    MIP_EnvSliderWidget* env2_slider = new MIP_EnvSliderWidget( MIP_DRect(130,356,110,16), "Slice Env", 0.1, 0.9 );
    MRootWidget->appendChildWidget(env2_slider);
    env2_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    env2_slider->setHint("Slice envelope");
    env2_slider->setBackgroundColor(0.4);
    env2_slider->setSliderColor(0.6);
    env2_slider->setTextColor(0.2);
    env2_slider->setTextSize(9);
    env2_slider->setValueColor(0.2);

  connect( env2_slider, 0, AParameters->getItem(PAR_SLICE_ENV_ATTACK) );
  connect( env2_slider, 1, AParameters->getItem(PAR_SLICE_ENV_DECAY) );












    // pages

    prob_page_button = new MIP_ButtonWidget(MIP_DRect(240,5,30,15), "Prob",0);
    MRootWidget->appendChildWidget(prob_page_button);
    prob_page_button->setHint("Probabilities");
    prob_page_button->setFillBackground(true);
    prob_page_button->setBackgroundColor(0.3);
    prob_page_button->setDrawBorder(false);
    prob_page_button->setTextSize(10);
    prob_page_button->setTextColor(0.6);

    fx_page_button = new MIP_ButtonWidget(MIP_DRect(275,5,30,15), "FX",0);
    MRootWidget->appendChildWidget(fx_page_button);
    fx_page_button->setHint("Effects");
    fx_page_button->setFillBackground(true);
    fx_page_button->setBackgroundColor(0.25);
    fx_page_button->setDrawBorder(false);
    fx_page_button->setTextSize(10);
    fx_page_button->setTextColor(0.6);

    seq_page_button = new MIP_ButtonWidget(MIP_DRect(310,5,30,15), "Seq",0);
    MRootWidget->appendChildWidget(seq_page_button);
    seq_page_button->setHint("Sequence");
    seq_page_button->setFillBackground(true);
    seq_page_button->setBackgroundColor(0.25);
    seq_page_button->setDrawBorder(false);
    seq_page_button->setTextSize(10);
    seq_page_button->setTextColor(0.6);

    perf_page_button = new MIP_ButtonWidget(MIP_DRect(345,5,30,15), "Perf",0);
    MRootWidget->appendChildWidget(perf_page_button);
    perf_page_button->setHint("Perform");
    perf_page_button->setFillBackground(true);
    perf_page_button->setBackgroundColor(0.25);
    perf_page_button->setDrawBorder(false);
    perf_page_button->setTextSize(10);
    perf_page_button->setTextColor(0.6);

    //prob_page_button->setBackgroundColor(0.8);
    //fx_page_button->setBackgroundColor(0.6);
    //seq_page_button->setBackgroundColor(0.6);
    //perf_page_button->setBackgroundColor(0.6);

    //

    MPages = new MIP_PagesWidget( MIP_DRect(255,90,225,292));
    MRootWidget->appendChildWidget(MPages);

      MIP_Widget* page1 = new MIP_PagesWidget( MIP_DRect(255,90,225,292));
      MIP_Widget* page2 = new MIP_PagesWidget( MIP_DRect(255,90,225,292));
      MIP_Widget* page3 = new MIP_PagesWidget( MIP_DRect(255,90,225,292));
      MIP_Widget* page4 = new MIP_PagesWidget( MIP_DRect(255,90,225,292));

      MPages->appendPage(page1);
      MPages->appendPage(page2);
      MPages->appendPage(page3);
      MPages->appendPage(page4);

    MPages->setPage(0);





//--------------------------------------------------
//
// page 1 - prob
//
//--------------------------------------------------





//------------------------------
// 1 prob
//------------------------------

    // prob

    MMainProbWidget = new MIP_KnobWidget(MIP_DRect(255,90,80,80), "%", 0.0);
    page1->appendChildWidget(MMainProbWidget);
    MMainProbWidget->setArcThickness(15);
    MMainProbWidget->setDrawText(true);
    MMainProbWidget->setTextSize(20);
    MMainProbWidget->setTextColor(0.6);
    MMainProbWidget->setDrawValue(true);
    MMainProbWidget->setValueSize(25);
    MMainProbWidget->setValueColor(0.7);
    MMainProbWidget->setHint("Main probability");

  connect( MMainProbWidget, AParameters->getItem(PAR_RANGE_PROB) );

    // slices

    MIP_TextWidget* prob_slices_label = new MIP_TextWidget( MIP_DRect(345,95,135,12), "Slices" );
    page1->appendChildWidget(prob_slices_label);
    prob_slices_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_slices_label->setTextColor(0.8);
    prob_slices_label->setTextSize(12);
    prob_slices_label->setDrawBorder(false);
    prob_slices_label->setFillBackground(false);

    MMainSlicesWidget = new MIP_ButtonRowWidget( MIP_DRect(345,109,135,16), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    page1->appendChildWidget(MMainSlicesWidget);
    MMainSlicesWidget->selectButton(0);
    MMainSlicesWidget->setAllowZeroBits(false);
    MMainSlicesWidget->setValueIsBits(true,8);
    MMainSlicesWidget->setHint("Number of slices in range");
    MMainSlicesWidget->setDrawBorder(false);

  connect( MMainSlicesWidget,  AParameters->getItem(PAR_RANGE_SLICE_COUNT) );

    // loops

    MIP_TextWidget* prob_loops_label = new MIP_TextWidget( MIP_DRect(345,134,135,12), "Loops" );
    page1->appendChildWidget(prob_loops_label);
    prob_loops_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_loops_label->setTextColor(0.8);
    prob_loops_label->setTextSize(12);
    prob_loops_label->setDrawBorder(false);
    prob_loops_label->setFillBackground(false);

    MMainLoopsWidget = new MIP_ButtonRowWidget( MIP_DRect(345,148,135,16), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    page1->appendChildWidget(MMainLoopsWidget);
    MMainLoopsWidget->selectButton(0);
    MMainLoopsWidget->setAllowZeroBits(false);
    MMainLoopsWidget->setValueIsBits(true,8);
    MMainLoopsWidget->setHint("Range subdivision");
    MMainLoopsWidget->setDrawBorder(false);

  connect( MMainLoopsWidget,  AParameters->getItem(PAR_RANGE_LOOP_COUNT) );

//------------------------------
// 2 range
//------------------------------

    MIP_TextWidget* c1_header = new MIP_TextWidget(MIP_DRect(255,180,103,14),"Range");
    page1->appendChildWidget(c1_header);
    c1_header->setDrawBorder(true);
    c1_header->setBorderColor(0.6);
    c1_header->setBorderEdges(MIP_EDGE_BOTTOM);
    c1_header->setFillBackground(false);
    c1_header->setBackgroundColor(0.35);
    c1_header->setTextColor(0.8);
    c1_header->setTextSize(11);

    // range length

    MRangeSizeWidget = new MIP_KnobWidget(MIP_DRect(255,205,33,33),"%", 0.0);
    page1->appendChildWidget(MRangeSizeWidget);
    MRangeSizeWidget->setArcThickness(7);
    MRangeSizeWidget->setHint("Loop length");
    MRangeSizeWidget->setTextColor(0.6);
    MRangeSizeWidget->setValueColor(0.8);

  connect( MRangeSizeWidget, AParameters->getItem(PAR_PROB_SIZE_PROB_RANGE) );

    MIP_TextWidget* range_length_text = new MIP_TextWidget(MIP_DRect(292,205,66,9),"Length");
    page1->appendChildWidget(range_length_text);
    range_length_text->setDrawBorder(false);
    range_length_text->setFillBackground(false);
    range_length_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_length_text->setTextColor(0.6);
    range_length_text->setTextSize(10);

    MRangeSizeValueWidget = new MIP_DualSliderWidget( MIP_DRect(292,216,66,16), "%");
    page1->appendChildWidget(MRangeSizeValueWidget);
    MRangeSizeValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MRangeSizeValueWidget->setHint("Loop min/max");

  connect( MRangeSizeValueWidget, 0, AParameters->getItem(PAR_PROB_SIZE_MIN_RANGE) );
  connect( MRangeSizeValueWidget, 1, AParameters->getItem(PAR_PROB_SIZE_MAX_RANGE) );

    // range speed

    MRangeSpeedWidget = new MIP_KnobWidget(MIP_DRect(255,240,33,33),"%", 0.0);
    page1->appendChildWidget(MRangeSpeedWidget);
    MRangeSpeedWidget->setArcThickness(7);
    MRangeSpeedWidget->setValueColor(0.8);
    MRangeSpeedWidget->setTextColor(0.6);

  connect( MRangeSpeedWidget, AParameters->getItem(PAR_PROB_SPEED_PROB_RANGE) );

    MIP_TextWidget* range_speed_text = new MIP_TextWidget(MIP_DRect(292,240,66,9),"Speed");
    page1->appendChildWidget(range_speed_text);
    range_speed_text->setDrawBorder(false);
    range_speed_text->setFillBackground(false);
    range_speed_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_speed_text->setTextColor(0.6);
    range_speed_text->setTextSize(10);

    MRangeSpeedValueWidget= new MIP_DualSliderWidget( MIP_DRect(292,251,66,16), "%", 0.5, 0.5 );
    page1->appendChildWidget(MRangeSpeedValueWidget);
    MRangeSpeedValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MRangeSpeedValueWidget, 0, AParameters->getItem(PAR_PROB_SPEED_MIN_RANGE) );
  connect( MRangeSpeedValueWidget, 1, AParameters->getItem(PAR_PROB_SPEED_MAX_RANGE) );

    // range offset

    MRangeOffsetWidget = new MIP_KnobWidget(MIP_DRect(255,275,33,33),"%", 0.0);
    page1->appendChildWidget(MRangeOffsetWidget);
    MRangeOffsetWidget->setArcThickness(7);
    MRangeOffsetWidget->setValueColor(0.8);
    MRangeOffsetWidget->setTextColor(0.6);

  connect( MRangeOffsetWidget, AParameters->getItem(PAR_PROB_OFFSET_PROB_RANGE) );

    MIP_TextWidget* range_offset_text = new MIP_TextWidget(MIP_DRect(292,275,66,9),"Offset");
    page1->appendChildWidget(range_offset_text);
    range_offset_text->setFillBackground(false);
    range_offset_text->setDrawBorder(false);
    range_offset_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_offset_text->setTextColor(0.6);
    range_offset_text->setTextSize(10);

    MRangeOffsetValueWidget = new MIP_DualSliderWidget( MIP_DRect(292,286,66,16), "sl", 0.5, 0.5 );
    page1->appendChildWidget(MRangeOffsetValueWidget);
    MRangeOffsetValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MRangeOffsetValueWidget, 0, AParameters->getItem(PAR_PROB_OFFSET_MIN_RANGE) );
  connect( MRangeOffsetValueWidget, 1, AParameters->getItem(PAR_PROB_OFFSET_MAX_RANGE) );

    // loop reverse

    MRangeReverseWidget = new MIP_KnobWidget(MIP_DRect(255,310,33,33),"%", 0.0);
    page1->appendChildWidget(MRangeReverseWidget);
    MRangeReverseWidget->setArcThickness(7);
    MRangeReverseWidget->setValueColor(0.8);
    MRangeReverseWidget->setTextColor(0.6);

  connect( MRangeReverseWidget, AParameters->getItem(PAR_PROB_REVERSE_PROB_RANGE) );

    MIP_TextWidget* range_reverse_text = new MIP_TextWidget(MIP_DRect(292,310,66,9),"Reverse");
    page1->appendChildWidget(range_reverse_text);
    range_reverse_text->setFillBackground(false);
    range_reverse_text->setDrawBorder(false);
    range_reverse_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_reverse_text->setTextColor(0.6);
    range_reverse_text->setTextSize(10);

    // loop fx

    MIP_KnobWidget* range_fx_knob = new MIP_KnobWidget(MIP_DRect(255,345,33,33),"%", 0.0);
    page1->appendChildWidget(range_fx_knob);
    range_fx_knob->setArcThickness(7);
    range_fx_knob->setDisabled(true);
    range_fx_knob->setDisabledColor(BACKGROUNDCOLOR);
    range_fx_knob->setActive(false);
    range_fx_knob->setValueColor(0.8);
    range_fx_knob->setTextColor(0.6);

  connect( range_fx_knob, AParameters->getItem(PAR_PROB_FX_PROB_RANGE) );

    MIP_TextWidget* range_fx_text = new MIP_TextWidget(MIP_DRect(292,345,66,9),"FX");
    page1->appendChildWidget(range_fx_text);
    range_fx_text->setFillBackground(false);
    range_fx_text->setDrawBorder(false);
    range_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_fx_text->setTextColor(0.6);
    range_fx_text->setTextSize(10);
    range_fx_text->setDisabled(true);
    range_fx_text->setDisabledColor(BACKGROUNDCOLOR);
    range_fx_text->setActive(false);

    MIP_DualSliderWidget* range_fx_range_slider = new MIP_DualSliderWidget( MIP_DRect(292,356,66,16), "%", 0.5, 0.5 );
    page1->appendChildWidget(range_fx_range_slider);
    range_fx_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    range_fx_range_slider->setDisabled(true);
    range_fx_range_slider->setDisabledColor(BACKGROUNDCOLOR);
    range_fx_range_slider->setActive(false);

  connect( range_fx_range_slider, 0, AParameters->getItem(PAR_PROB_FX_MIN_RANGE) );
  connect( range_fx_range_slider, 1, AParameters->getItem(PAR_PROB_FX_MAX_RANGE) );

//------------------------------
// 3 loop
//------------------------------

    MIP_TextWidget* c2_header = new MIP_TextWidget(MIP_DRect(377,180,103,14),"Loop");
    page1->appendChildWidget(c2_header);
    c2_header->setDrawBorder(true);
    c2_header->setBorderColor(0.6);
    c2_header->setBorderEdges(MIP_EDGE_BOTTOM);
    c2_header->setFillBackground(false);
    c2_header->setBackgroundColor(0.35);
    c2_header->setTextColor(0.8);
    c2_header->setTextSize(11);

    // loop length

    MLoopSizeWidget = new MIP_KnobWidget(MIP_DRect(377,205,33,33),"%", 0.0);
    page1->appendChildWidget(MLoopSizeWidget);
    MLoopSizeWidget->setArcThickness(7);
    MLoopSizeWidget->setValueColor(0.8);
    MLoopSizeWidget->setTextColor(0.6);

  connect( MLoopSizeWidget, AParameters->getItem(PAR_PROB_SIZE_PROB_LOOP) );

    MIP_TextWidget* loop_length_text = new MIP_TextWidget(MIP_DRect(414,205,66,9),"Length");
    page1->appendChildWidget(loop_length_text);
    loop_length_text->setDrawBorder(false);
    loop_length_text->setFillBackground(false);
    loop_length_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_length_text->setTextColor(0.6);
    loop_length_text->setTextSize(10);

    MLoopSizeValueWidget = new MIP_DualSliderWidget( MIP_DRect(414,216,66,16), "%", 0.5, 0.5 );
    page1->appendChildWidget(MLoopSizeValueWidget);
    MLoopSizeValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MLoopSizeValueWidget, 0, AParameters->getItem(PAR_PROB_SIZE_MIN_LOOP) );
  connect( MLoopSizeValueWidget, 1, AParameters->getItem(PAR_PROB_SIZE_MAX_LOOP) );

    // loop speed

    MLoopSpeedWidget = new MIP_KnobWidget(MIP_DRect(377,240,33,33),"%", 0.0);
    page1->appendChildWidget(MLoopSpeedWidget);
    MLoopSpeedWidget->setArcThickness(7);
    MLoopSpeedWidget->setValueColor(0.8);
    MLoopSpeedWidget->setTextColor(0.6);

  connect( MLoopSpeedWidget, AParameters->getItem(PAR_PROB_SPEED_PROB_LOOP) );

    MIP_TextWidget* loop_speed_text = new MIP_TextWidget(MIP_DRect(414,240,66,9),"Speed");
    page1->appendChildWidget(loop_speed_text);
    loop_speed_text->setDrawBorder(false);
    loop_speed_text->setFillBackground(false);
    loop_speed_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_speed_text->setTextColor(0.6);
    loop_speed_text->setTextSize(10);

    MLoopSpeedValueWidget = new MIP_DualSliderWidget( MIP_DRect(414,251,66,16), "%", 0.5, 0.5 );
    page1->appendChildWidget(MLoopSpeedValueWidget);
    MLoopSpeedValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MLoopSpeedValueWidget, 0, AParameters->getItem(PAR_PROB_SPEED_MIN_LOOP) );
  connect( MLoopSpeedValueWidget, 1, AParameters->getItem(PAR_PROB_SPEED_MAX_LOOP) );

    // loop offset

    MLoopOffsetWidget = new MIP_KnobWidget(MIP_DRect(377,275,33,33),"%", 0.0);
    page1->appendChildWidget(MLoopOffsetWidget);
    MLoopOffsetWidget->setArcThickness(7);
    MLoopOffsetWidget->setValueColor(0.8);
    MLoopOffsetWidget->setTextColor(0.6);

  connect( MLoopOffsetWidget, AParameters->getItem(PAR_PROB_OFFSET_PROB_LOOP) );

    MIP_TextWidget* loop_offset_text = new MIP_TextWidget(MIP_DRect(414,275,66,9),"Offset");
    page1->appendChildWidget(loop_offset_text);
    loop_offset_text->setDrawBorder(false);
    loop_offset_text->setFillBackground(false);
    loop_offset_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_offset_text->setTextColor(0.6);
    loop_offset_text->setTextSize(10);

    MLoopOffsetValueWidget = new MIP_DualSliderWidget( MIP_DRect(414,286,66,16), "sl", 0.5, 0.5 );
    page1->appendChildWidget(MLoopOffsetValueWidget);
    MLoopOffsetValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MLoopOffsetValueWidget, 0, AParameters->getItem(PAR_PROB_OFFSET_MIN_LOOP) );
  connect( MLoopOffsetValueWidget, 1, AParameters->getItem(PAR_PROB_OFFSET_MAX_LOOP) );

    // loop reverse

    MLoopReverseWidget = new MIP_KnobWidget(MIP_DRect(377,310,33,33),"%", 0.0);
    page1->appendChildWidget(MLoopReverseWidget);
    MLoopReverseWidget->setArcThickness(7);
    MLoopReverseWidget->setValueColor(0.8);
    MLoopReverseWidget->setTextColor(0.6);

  connect( MLoopReverseWidget, AParameters->getItem(PAR_PROB_REVERSE_PROB_LOOP) );

    MIP_TextWidget* loop_reverse_text = new MIP_TextWidget(MIP_DRect(414,310,66,9),"Reverse");
    page1->appendChildWidget(loop_reverse_text);
    loop_reverse_text->setDrawBorder(false);
    loop_reverse_text->setFillBackground(false);
    loop_reverse_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_reverse_text->setTextColor(0.6);
    loop_reverse_text->setTextSize(10);

    // loop fx

    MIP_KnobWidget* loop_fx_knob = new MIP_KnobWidget(MIP_DRect(377,345,33,33),"%", 0.0);
    page1->appendChildWidget(loop_fx_knob);
    loop_fx_knob->setArcThickness(7);
    loop_fx_knob->setDisabled(true);
    loop_fx_knob->setDisabledColor(BACKGROUNDCOLOR);
    loop_fx_knob->setActive(false);
    loop_fx_knob->setValueColor(0.8);
    loop_fx_knob->setTextColor(0.6);

  connect( loop_fx_knob, AParameters->getItem(PAR_PROB_FX_PROB_LOOP) );

    MIP_TextWidget* loop_fx_text = new MIP_TextWidget(MIP_DRect(414,345,66,9),"FX");
    page1->appendChildWidget(loop_fx_text);
    loop_fx_text->setDrawBorder(false);
    loop_fx_text->setFillBackground(false);
    loop_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_fx_text->setTextColor(0.6);
    loop_fx_text->setTextSize(10);
    loop_fx_text->setDisabled(true);
    loop_fx_text->setDisabledColor(BACKGROUNDCOLOR);
    loop_fx_text->setActive(false);

    MIP_DualSliderWidget* loop_fx_range_slider = new MIP_DualSliderWidget( MIP_DRect(414,356,66,16), "%", 0.5, 0.5 );
    page1->appendChildWidget(loop_fx_range_slider);
    loop_fx_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    loop_fx_range_slider->setDisabled(true);
    loop_fx_range_slider->setDisabledColor(BACKGROUNDCOLOR);
    loop_fx_range_slider->setActive(false);

  connect( loop_fx_range_slider, 0, AParameters->getItem(PAR_PROB_FX_MIN_LOOP) );
  connect( loop_fx_range_slider, 1, AParameters->getItem(PAR_PROB_FX_MAX_LOOP) );

//----------------------------------------------------------------------
//
// page 2 - fx
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
// page 3 - seq
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// page 4 - perf
//
//----------------------------------------------------------------------

//------------------------------
// footer
//------------------------------

    //double height = ARootWidget->getHeight();
    //sa_botage_footer* footer  = new sa_botage_footer(MIP_DRect(0,(height - SA_BOTAGE_FOOTER_HEIGHT),width,SA_BOTAGE_FOOTER_HEIGHT));
    //ARootWidget->appendChildWidget(footer);

  }


  //----------

  virtual ~sa_botage_editor() {
    if (MRootWidget) delete MRootWidget;
  }

//------------------------------
public:
//------------------------------

  bool show() override {
    uint32_t beats = MParameters->getItem(0)->getValue();
    uint32_t slices = MParameters->getItem(1)->getValue();

    //MWaveformWidget->setNumGrid(beats);
    //MWaveformWidget->setNumSubGrid(slices);
    MWaveformWidget->setGrid(beats,slices);

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
      MWaveformWidget->setGrid(value);
      MWaveformWidget->redraw();
    }
    else if (AWidget == MNumSlicesWidget) {
      double value = AWidget->getValue();
      MWaveformWidget->setSubGrid(value);
      MWaveformWidget->redraw();
    }
    else if (AWidget == prob_page_button) {
      MCurrentPage = 0;
      MPages->setPage(0);
      MPages->redraw();
      prob_page_button->setBackgroundColor(0.3);
      fx_page_button->setBackgroundColor(0.25);
      seq_page_button->setBackgroundColor(0.25);
      perf_page_button->setBackgroundColor(0.25);
      prob_page_button->redraw();
      fx_page_button->redraw();
      seq_page_button->redraw();
      perf_page_button->redraw();
    }
    else if (AWidget == fx_page_button)   {
      MCurrentPage = 1;
      MPages->setPage(1);
      MPages->redraw();
      prob_page_button->setBackgroundColor(0.25);
      fx_page_button->setBackgroundColor(0.3);
      seq_page_button->setBackgroundColor(0.25);
      perf_page_button->setBackgroundColor(0.25);
      prob_page_button->redraw();
      fx_page_button->redraw();
      seq_page_button->redraw();
      perf_page_button->redraw();
    }
    else if (AWidget == seq_page_button)  {
      MCurrentPage = 2;
      MPages->setPage(2); MPages->redraw();
      prob_page_button->setBackgroundColor(0.25);
      fx_page_button->setBackgroundColor(0.25);
      seq_page_button->setBackgroundColor(0.3);
      perf_page_button->setBackgroundColor(0.25);
      prob_page_button->redraw();
      fx_page_button->redraw();
      seq_page_button->redraw();
      perf_page_button->redraw();
    }
    else if (AWidget == perf_page_button) {
      MCurrentPage = 3;
      MPages->setPage(3); MPages->redraw();
      prob_page_button->setBackgroundColor(0.25);
      fx_page_button->setBackgroundColor(0.25);
      seq_page_button->setBackgroundColor(0.25);
      perf_page_button->setBackgroundColor(0.3);
      prob_page_button->redraw();
      fx_page_button->redraw();
      seq_page_button->redraw();
      perf_page_button->redraw();
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

//------------------------------
public:
//------------------------------

//  void timer_update(sa_botage_processor* processor) {
//    //MIP_Assert(MWaveformWidget);
//    if (MIsEditorOpen) {
//      updateWaveformWidget(processor);
//      MWaveformWidget->redraw(); // do_widget_redraw(MWaveformWidget);
//    }
//  }

  //----------

  void updateWaveformWidget(sa_botage_processor* processor) {

    uint32_t buflen = processor->MBufferLength;
    if (buflen > 0) {

      MWaveformWidget->setAudioBuffer(buflen, processor->MLeftBuffer, false);
      MWaveformWidget->setGrid(processor->par_num_beats, processor->par_num_slices);

      // marker 0 write pos
      MWaveformWidget->setMarkerActive(0, true);
      MWaveformWidget->setMarkerPos(   0, (double)processor->MWritePos/buflen, 0.001);
      MWaveformWidget->setMarkerColor( 0, MWritePosColor);
      //MWaveformWidget->setMarkerWidth( 0, MWritePosWidth);

      // marker 1 read pos
      MWaveformWidget->setMarkerActive(1, true);
      MWaveformWidget->setMarkerPos(   1, processor->MReadPos/buflen, 0.001);
      MWaveformWidget->setMarkerColor( 1, MReadPosColor);
      //MWaveformWidget->setMarkerWidth( 1,MReadPosWidth);

      if (processor->MRange) {
        MWaveformWidget->setAreaActive(0, false);
        // area 1 range
        MWaveformWidget->setAreaActive(1, true);
        MWaveformWidget->setAreaPos(   1, processor->MRangeStart/buflen, processor->MRangeLength/buflen);
        MWaveformWidget->setAreaColor( 1, MRangeColor);

        if (processor->MLoop) {
          // area 2 loop
          MWaveformWidget->setAreaActive(2, true);
          MWaveformWidget->setAreaPos(   2, processor->MLoopStart/buflen, processor->MLoopLength/buflen);
          MWaveformWidget->setAreaColor( 2, MLoopColor);
        }
        else {
          MWaveformWidget->setAreaActive(2, false);
        }
      }

      else { // no range
        MWaveformWidget->setAreaActive(1, false);
        MWaveformWidget->setAreaActive(2, false);

        // area 0 current slice
        if (processor->MIsPlaying) {
          MWaveformWidget->setAreaActive(0, true);
          MWaveformWidget->setAreaPos(   0, processor->MSliceStart/buflen, processor->MSliceLength/buflen);
          MWaveformWidget->setAreaColor( 0, MSliceColor);
        }
        else {
          MWaveformWidget->setAreaActive(0, false);
        }

      } // range

      MWaveformWidget->redraw();

    } // buflen

  }

  //----------

  void updateProbIndicators(sa_botage_processor* processor) {

    if (MCurrentPage == 0) {

      MMainProbWidget->setIndicatorValue(processor->rnd_main_prob);
      if (processor->MRange) {
        //MMainProbWidget->setIndicatorValue(processor->rnd_main_prob);
        MMainSlicesWidget->setIndicatorValue(processor->rnd_main_slices);
        MMainLoopsWidget->setIndicatorValue(processor->rnd_main_subdiv);
        //
        MRangeSizeWidget->setIndicatorValue(processor->rnd_range_size);
        MRangeSpeedWidget->setIndicatorValue(processor->rnd_range_speed);
        MRangeOffsetWidget->setIndicatorValue(processor->rnd_range_offset);
        MRangeReverseWidget->setIndicatorValue(processor->rnd_range_reverse);
        //
        if (processor->rnd_range_size_on) MRangeSizeValueWidget->setIndicatorValue(processor->rnd_range_size_value);
        else MRangeSizeValueWidget->setIndicatorValue(-1);
        if (processor->rnd_range_speed_on) MRangeSpeedValueWidget->setIndicatorValue(processor->rnd_range_speed_value);
        else MRangeSpeedValueWidget->setIndicatorValue(-1);
        if (processor->rnd_range_offset_on) MRangeOffsetValueWidget->setIndicatorValue(processor->rnd_range_offset_value);
        else MRangeOffsetValueWidget->setIndicatorValue(-1);
        //
        if (processor->MLoopWrapped) {
          MLoopSizeWidget->setIndicatorValue(processor->rnd_loop_size);
          MLoopSpeedWidget->setIndicatorValue(processor->rnd_loop_speed);
          MLoopOffsetWidget->setIndicatorValue(processor->rnd_loop_offset);
          MLoopReverseWidget->setIndicatorValue(processor->rnd_loop_reverse);
          //
          if (processor->rnd_loop_size_on) MLoopSizeValueWidget->setIndicatorValue(processor->rnd_loop_size_value);
          else MLoopSizeValueWidget->setIndicatorValue(-1);
          if (processor->rnd_loop_speed_on) MLoopSpeedValueWidget->setIndicatorValue(processor->rnd_loop_speed_value);
          else MLoopSpeedValueWidget->setIndicatorValue(-1);
          if (processor->rnd_loop_offset_on) MLoopOffsetValueWidget->setIndicatorValue(processor->rnd_loop_offset_value);
          else MLoopOffsetValueWidget->setIndicatorValue(-1);
        }
        else {
          MLoopSizeWidget->setIndicatorValue(-1);
          MLoopSpeedWidget->setIndicatorValue(-1);
          MLoopOffsetWidget->setIndicatorValue(-1);
          MLoopReverseWidget->setIndicatorValue(-1);
          //
          MLoopSizeValueWidget->setIndicatorValue(-1);
          MLoopSpeedValueWidget->setIndicatorValue(-1);
          MLoopOffsetValueWidget->setIndicatorValue(-1);
        }
      }
      else { // ! range
        //MMainProbWidget->setIndicatorValue(-1);
        MMainSlicesWidget->setIndicatorValue(-1);
        MMainLoopsWidget->setIndicatorValue(-1);
        //
        MRangeSizeWidget->setIndicatorValue(-1);
        MRangeSpeedWidget->setIndicatorValue(-1);
        MRangeOffsetWidget->setIndicatorValue(-1);
        MRangeReverseWidget->setIndicatorValue(-1);
        //
        MRangeSizeValueWidget->setIndicatorValue(-1);
        MRangeSpeedValueWidget->setIndicatorValue(-1);
        MRangeOffsetValueWidget->setIndicatorValue(-1);
        //
        MLoopSizeWidget->setIndicatorValue(-1);
        MLoopSpeedWidget->setIndicatorValue(-1);
        MLoopOffsetWidget->setIndicatorValue(-1);
        MLoopReverseWidget->setIndicatorValue(-1);
        //
        MLoopSizeValueWidget->setIndicatorValue(-1);
        MLoopSpeedValueWidget->setIndicatorValue(-1);
        MLoopOffsetValueWidget->setIndicatorValue(-1);
      }

    } // page == 0
  }

};

//----------------------------------------------------------------------
#endif
