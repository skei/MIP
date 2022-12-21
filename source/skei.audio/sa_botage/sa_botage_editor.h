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

  //

  MIP_Color MReadPosColor   = MIP_COLOR_BRIGHT_GREEN;
  MIP_Color MWritePosColor  = MIP_COLOR_BRIGHT_RED;

  double    MReadPosWidth   = 1.5;
  double    MWritePosWidth  = 1.5;

  MIP_Color MRangeColor     = MIP_Color( 0, 0.5, 0, 0.2);
  MIP_Color MLoopColor      = MIP_Color( 0, 1.0, 0, 0.2);
  MIP_Color MSliceColor     = MIP_Color( 0, 0,   0, 0.2);

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

    sa_botage_header* header  = new sa_botage_header(MIP_DRect(0,0,width,SA_BOTAGE_HEADER_HEIGHT),SA_BOTAGE_VERSION);
    MRootWidget->appendChildWidget(header);

    // icons

    MIP_TextWidget* c_icon = new MIP_TextWidget(MIP_DRect(475,30,15,15),"c");
    MRootWidget->appendChildWidget(c_icon);
    c_icon->setCursor(MIP_CURSOR_FINGER);
    c_icon->setTextSize(12);
    c_icon->setTextColor(0.5);
    c_icon->setDrawBorder(true);
    c_icon->setBorderColor(0.5);
    c_icon->setFillBackground(false);
    c_icon->setHint("Config");

    MIP_TextWidget* i_icon = new MIP_TextWidget(MIP_DRect(495,30,15,15),"i");
    MRootWidget->appendChildWidget(i_icon);
    i_icon->setCursor(MIP_CURSOR_FINGER);
    i_icon->setTextSize(12);
    i_icon->setTextColor(0.5);
    i_icon->setDrawBorder(true);
    i_icon->setBorderColor(0.5);
    i_icon->setFillBackground(false);
    i_icon->setHint("Info");

    // hint

    MHintWidget = new MIP_TextWidget(MIP_DRect(250,50,260,20),"Hello world!");
    MRootWidget->appendChildWidget(MHintWidget);
    MHintWidget->setFillBackground(false);
    //MHintWidget->setBackgroundColor(0.3);
    MHintWidget->setDrawBorder(true);
    MHintWidget->setBorderColor(MIP_COLOR_GRAY);
    MHintWidget->setTextSize(10);
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

    MNumBeatsWidget = new MIP_DragValueWidget( MIP_DRect(10,160,120,20),"Beats");
    MRootWidget->appendChildWidget(MNumBeatsWidget);
    MNumBeatsWidget->setFillBackground(true);
    MNumBeatsWidget->setBackgroundColor(MIP_COLOR_LIGHT_GRAY);
    MNumBeatsWidget->setTextColor(MIP_COLOR_BLACK);
    MNumBeatsWidget->setTextSize(14);
    MNumBeatsWidget->setValueColor(MIP_COLOR_BLACK);
    MNumBeatsWidget->setValueSize(14);
    MNumBeatsWidget->setHint("Number of beats in buffer");
    MNumBeatsWidget->setIValueColor(MIP_COLOR_LIGHT_RED);

  connect( MNumBeatsWidget,  AParameters->getItem(PAR_NUM_BEATS) );

    // slices

    MNumSlicesWidget = new MIP_DragValueWidget( MIP_DRect(140,160,120,20),"Slices");
    MRootWidget->appendChildWidget(MNumSlicesWidget);
    MNumSlicesWidget->setFillBackground(true);
    MNumSlicesWidget->setBackgroundColor(MIP_COLOR_LIGHT_GRAY);
    MNumSlicesWidget->setTextColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setTextSize(14);
    MNumSlicesWidget->setValueColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setValueSize(14);
    MNumSlicesWidget->setHint("Number of slices per beat");
    MNumSlicesWidget->setIValueColor(MIP_COLOR_LIGHT_RED);

  connect( MNumSlicesWidget, AParameters->getItem(PAR_NUM_SLICES) );

    // pages

    MIP_ButtonWidget* prob_page_button = new MIP_ButtonWidget(MIP_DRect(400,160,50,20), "Prob",0);
    MRootWidget->appendChildWidget(prob_page_button);
    prob_page_button->setBackgroundColor(0.8);
    prob_page_button->setHint("Probabilities");

    MIP_ButtonWidget* fx_page_button = new MIP_ButtonWidget(MIP_DRect(460,160,50,20), "FX",0);
    MRootWidget->appendChildWidget(fx_page_button);
    fx_page_button->setBackgroundColor(0.6);
    fx_page_button->setHint("Effects");

    MIP_ButtonWidget* seq_page_button = new MIP_ButtonWidget(MIP_DRect(520,160,50,20), "Seq",0);
    MRootWidget->appendChildWidget(seq_page_button);
    seq_page_button->setBackgroundColor(0.6);
    seq_page_button->setHint("Sequence");

    MIP_ButtonWidget* perf_page_button = new MIP_ButtonWidget(MIP_DRect(580,160,50,20), "Perf",0);
    MRootWidget->appendChildWidget(perf_page_button);
    perf_page_button->setBackgroundColor(0.6);
    perf_page_button->setHint("Perform");


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

    //MIP_KnobWidget* prob_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,90,90), "%", 0.0);
    MMainProbWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,90,90), "%", 0.0);
    MRootWidget->appendChildWidget(MMainProbWidget);
    MMainProbWidget->setArcThickness(15);
    MMainProbWidget->setDrawText(true);
    MMainProbWidget->setTextSize(20);
    MMainProbWidget->setDrawValue(true);
    MMainProbWidget->setValueSize(25);
    MMainProbWidget->setHint("Main probability");
    //MMainProbWidget->setSnap(true);
    //MMainProbWidget->setSnapPos(0.5);
    //MMainProbWidget->setSnapDist(0.05);

  connect( MMainProbWidget, AParameters->getItem(PAR_RANGE_PROB) );

    // slices

    x = 10 + 100;
    y = 200;

    MIP_TextWidget* prob_slices_label = new MIP_TextWidget( MIP_DRect(x,y+30,150,15), "Slices" );
    MRootWidget->appendChildWidget(prob_slices_label);
    prob_slices_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_slices_label->setDrawBorder(false);

    //MIP_ButtonRowWidget* prob_slices = new MIP_ButtonRowWidget( MIP_DRect(x,y+45,150,20), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    MMainSlicesWidget = new MIP_ButtonRowWidget( MIP_DRect(x,y+45,150,20), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    MRootWidget->appendChildWidget(MMainSlicesWidget);
    MMainSlicesWidget->selectButton(0);
    MMainSlicesWidget->setAllowZeroBits(false);
    MMainSlicesWidget->setValueIsBits(true,8);
    MMainSlicesWidget->setHint("Number of slices in range");

  connect( MMainSlicesWidget,  AParameters->getItem(PAR_RANGE_SLICE_COUNT) );

    // loops

    MIP_TextWidget* prob_loops_label = new MIP_TextWidget( MIP_DRect(x,y+75,150,15), "Loops" );
    MRootWidget->appendChildWidget(prob_loops_label);
    prob_loops_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_loops_label->setDrawBorder(false);

    //MIP_ButtonRowWidget* prob_loops = new MIP_ButtonRowWidget( MIP_DRect(x,y+90,150,20), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    MMainLoopsWidget = new MIP_ButtonRowWidget( MIP_DRect(x,y+90,150,20), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    MRootWidget->appendChildWidget(MMainLoopsWidget);
    MMainLoopsWidget->selectButton(0);
    MMainLoopsWidget->setAllowZeroBits(false);
    MMainLoopsWidget->setValueIsBits(true,8);
    MMainLoopsWidget->setHint("Range subdivision");

  connect( MMainLoopsWidget,  AParameters->getItem(PAR_RANGE_LOOP_COUNT) );

    // envelopes

    x = 10;
    y = 350;

    MIP_TextWidget* loop_env_label = new MIP_TextWidget(MIP_DRect(x,y,120,20), "Loop Env");
    MRootWidget->appendChildWidget(loop_env_label);
    loop_env_label->setFillBackground(true);
    loop_env_label->setBackgroundColor(MIP_COLOR_DARK_GRAY);
    loop_env_label->setTextColor(MIP_COLOR_LIGHT_GRAY);

    MIP_EnvSliderWidget* env1_slider = new MIP_EnvSliderWidget( MIP_DRect(x,y+30,120,25), "", 0.1, 0.9 );
    MRootWidget->appendChildWidget(env1_slider);
    env1_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    env1_slider->setHint("Loop envelope");

  connect( env1_slider, 0, AParameters->getItem(PAR_LOOP_ENV_ATTACK) );
  connect( env1_slider, 1, AParameters->getItem(PAR_LOOP_ENV_DECAY) );

    MIP_TextWidget* slice_env_label = new MIP_TextWidget(MIP_DRect(x+130,y,120,20), "Slice Env");
    MRootWidget->appendChildWidget(slice_env_label);
    slice_env_label->setFillBackground(true);
    slice_env_label->setBackgroundColor(MIP_COLOR_DARK_GRAY);
    slice_env_label->setTextColor(MIP_COLOR_LIGHT_GRAY);

    MIP_EnvSliderWidget* env2_slider = new MIP_EnvSliderWidget( MIP_DRect(x+130,y+30,120,25), "", 0.1, 0.9 );
    MRootWidget->appendChildWidget(env2_slider);
    env2_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    env2_slider->setHint("Slice envelope");

  connect( env2_slider, 0, AParameters->getItem(PAR_SLICE_ENV_ATTACK) );
  connect( env2_slider, 1, AParameters->getItem(PAR_SLICE_ENV_DECAY) );

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

    // range length

    //MIP_KnobWidget* range_length_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRangeSizeWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(MRangeSizeWidget);
    MRangeSizeWidget->setArcThickness(7);
    MRangeSizeWidget->setHint("Loop length");
    //MRangeSizeWidget->setSnap(true);
    //MRangeSizeWidget->setSnapPos(0.5);
    //MRangeSizeWidget->setSnapDist(0.05);

  connect( MRangeSizeWidget, AParameters->getItem(PAR_PROB_SIZE_PROB_RANGE) );

    MIP_TextWidget* range_length_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Size");
    MRootWidget->appendChildWidget(range_length_text);
    range_length_text->setDrawBorder(false);
    range_length_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* range_length_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%");//, 0.5, 0.5 );
    MRangeSizeValueWidget = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%");//, 0.5, 0.5 );
    MRootWidget->appendChildWidget(MRangeSizeValueWidget);
    MRangeSizeValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MRangeSizeValueWidget->setHint("Loop min/max");
//    MRangeSizeValueWidget->setSnap(true);
//    MRangeSizeValueWidget->setSnapPos(0.5);
//    MRangeSizeValueWidget->setSnapDist(0.05);

  connect( MRangeSizeValueWidget, 0, AParameters->getItem(PAR_PROB_SIZE_MIN_RANGE) );
  connect( MRangeSizeValueWidget, 1, AParameters->getItem(PAR_PROB_SIZE_MAX_RANGE) );

    // range speed

    y = 245;

    //MIP_KnobWidget* range_speed_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRangeSpeedWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(MRangeSpeedWidget);
    MRangeSpeedWidget->setArcThickness(7);
    //MRangeSpeedWidget->setSnap(true);
    //MRangeSpeedWidget->setSnapPos(0.5);
    //MRangeSpeedWidget->setSnapDist(0.05);

  connect( MRangeSpeedWidget, AParameters->getItem(PAR_PROB_SPEED_PROB_RANGE) );

    MIP_TextWidget* range_speed_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Speed");
    MRootWidget->appendChildWidget(range_speed_text);
    range_speed_text->setDrawBorder(false);
    range_speed_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* range_speed_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MRangeSpeedValueWidget= new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MRootWidget->appendChildWidget(MRangeSpeedValueWidget);
    MRangeSpeedValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MRangeSpeedValueWidget, 0, AParameters->getItem(PAR_PROB_SPEED_MIN_RANGE) );
  connect( MRangeSpeedValueWidget, 1, AParameters->getItem(PAR_PROB_SPEED_MAX_RANGE) );

    // range offset

    y = 290;

    //MIP_KnobWidget* range_offset_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRangeOffsetWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(MRangeOffsetWidget);
    MRangeOffsetWidget->setArcThickness(7);
    //MRangeOffsetWidget->setSnap(true);
    //MRangeOffsetWidget->setSnapPos(0.5);
    //MRangeOffsetWidget->setSnapDist(0.05);

  connect( MRangeOffsetWidget, AParameters->getItem(PAR_PROB_OFFSET_PROB_RANGE) );

    MIP_TextWidget* range_offset_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Offset");
    MRootWidget->appendChildWidget(range_offset_text);
    range_offset_text->setDrawBorder(false);
    range_offset_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* range_offset_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MRangeOffsetValueWidget = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "sl", 0.5, 0.5 );
    MRootWidget->appendChildWidget(MRangeOffsetValueWidget);
    MRangeOffsetValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MRangeOffsetValueWidget, 0, AParameters->getItem(PAR_PROB_OFFSET_MIN_RANGE) );
  connect( MRangeOffsetValueWidget, 1, AParameters->getItem(PAR_PROB_OFFSET_MAX_RANGE) );

    // loop reverse

    y = 335;

    //MIP_KnobWidget* range_reverse_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRangeReverseWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(MRangeReverseWidget);
    MRangeReverseWidget->setArcThickness(7);
    //MRangeReverseWidget->setSnap(true);
    //MRangeReverseWidget->setSnapPos(0.5);
    //MRangeReverseWidget->setSnapDist(0.05);

  connect( MRangeReverseWidget, AParameters->getItem(PAR_PROB_REVERSE_PROB_RANGE) );

    MIP_TextWidget* range_reverse_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Reverse");
    MRootWidget->appendChildWidget(range_reverse_text);
    range_reverse_text->setDrawBorder(false);
    range_reverse_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* range_reverse_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.5, 0.5 );
    //MRootWidget->appendChildWidget(range_reverse_range_slider);
    //range_reverse_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    //range_reverse_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop fx

    y = 380;

    MIP_KnobWidget* range_fx_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(range_fx_knob);
    range_fx_knob->setArcThickness(7);
    range_fx_knob->setDisabled(true);
    range_fx_knob->setActive(false);
    //range_fx_knob->setSnap(true);
    //range_fx_knob->setSnapPos(0.5);
    //range_fx_knob->setSnapDist(0.05);

  connect( range_fx_knob, AParameters->getItem(PAR_PROB_FX_PROB_RANGE) );

    MIP_TextWidget* range_fx_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"FX");
    MRootWidget->appendChildWidget(range_fx_text);
    range_fx_text->setDrawBorder(false);
    range_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_fx_text->setDisabled(true);
    range_fx_text->setActive(false);

    MIP_DualSliderWidget* range_fx_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MRootWidget->appendChildWidget(range_fx_range_slider);
    range_fx_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    range_fx_range_slider->setDisabled(true);
    range_fx_range_slider->setActive(false);

  connect( range_fx_range_slider, 0, AParameters->getItem(PAR_PROB_FX_MIN_RANGE) );
  connect( range_fx_range_slider, 1, AParameters->getItem(PAR_PROB_FX_MAX_RANGE) );

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

    //MIP_KnobWidget* loop_length_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MLoopSizeWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(MLoopSizeWidget);
    MLoopSizeWidget->setArcThickness(7);
    //MLoopSizeWidget->setAutoHideCursor(false);
    //MLoopSizeWidget->setAutoLockCursor(false);

  connect( MLoopSizeWidget, AParameters->getItem(PAR_PROB_SIZE_PROB_LOOP) );

    MIP_TextWidget* loop_length_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Size");
    MRootWidget->appendChildWidget(loop_length_text);
    loop_length_text->setDrawBorder(false);
    loop_length_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* loop_length_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MLoopSizeValueWidget = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MRootWidget->appendChildWidget(MLoopSizeValueWidget);
    MLoopSizeValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MLoopSizeValueWidget, 0, AParameters->getItem(PAR_PROB_SIZE_MIN_LOOP) );
  connect( MLoopSizeValueWidget, 1, AParameters->getItem(PAR_PROB_SIZE_MAX_LOOP) );

    // loop speed

    y = 245;

    //MIP_KnobWidget* loop_speed_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MLoopSpeedWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(MLoopSpeedWidget);
    MLoopSpeedWidget->setArcThickness(7);
    //MLoopSpeedWidget->setSnap(true);
    //MLoopSpeedWidget->setSnapPos(0.5);
    //MLoopSpeedWidget->setSnapDist(0.05);

  connect( MLoopSpeedWidget, AParameters->getItem(PAR_PROB_SPEED_PROB_LOOP) );

    MIP_TextWidget* loop_speed_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Speed");
    MRootWidget->appendChildWidget(loop_speed_text);
    loop_speed_text->setDrawBorder(false);
    loop_speed_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* loop_speed_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MLoopSpeedValueWidget = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MRootWidget->appendChildWidget(MLoopSpeedValueWidget);
    MLoopSpeedValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MLoopSpeedValueWidget, 0, AParameters->getItem(PAR_PROB_SPEED_MIN_LOOP) );
  connect( MLoopSpeedValueWidget, 1, AParameters->getItem(PAR_PROB_SPEED_MAX_LOOP) );

    // loop offset

    y = 290;

    //MIP_KnobWidget* loop_offset_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MLoopOffsetWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(MLoopOffsetWidget);
    MLoopOffsetWidget->setArcThickness(7);
    //MLoopOffsetWidget->setSnap(true);
    //MLoopOffsetWidget->setSnapPos(0.5);
    //MLoopOffsetWidget->setSnapDist(0.05);

  connect( MLoopOffsetWidget, AParameters->getItem(PAR_PROB_OFFSET_PROB_LOOP) );

    MIP_TextWidget* loop_offset_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Offset");
    MRootWidget->appendChildWidget(loop_offset_text);
    loop_offset_text->setDrawBorder(false);
    loop_offset_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* loop_offset_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MLoopOffsetValueWidget = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MRootWidget->appendChildWidget(MLoopOffsetValueWidget);
    MLoopOffsetValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

  connect( MLoopOffsetValueWidget, 0, AParameters->getItem(PAR_PROB_OFFSET_MIN_LOOP) );
  connect( MLoopOffsetValueWidget, 1, AParameters->getItem(PAR_PROB_OFFSET_MAX_LOOP) );

    // loop reverse

    y = 335;

    //MIP_KnobWidget* loop_reverse_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MLoopReverseWidget = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"sl%", 0.0);
    MRootWidget->appendChildWidget(MLoopReverseWidget);
    MLoopReverseWidget->setArcThickness(7);
    //MLoopReverseWidget->setSnap(true);
    //MLoopReverseWidget->setSnapPos(0.5);
    //MLoopReverseWidget->setSnapDist(0.05);

  connect( MLoopReverseWidget, AParameters->getItem(PAR_PROB_REVERSE_PROB_LOOP) );

    MIP_TextWidget* loop_reverse_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"Reverse");
    MRootWidget->appendChildWidget(loop_reverse_text);
    loop_reverse_text->setDrawBorder(false);
    loop_reverse_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);

    //MIP_DualSliderWidget* loop_reverse_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "", 0.5, 0.5 );
    //MRootWidget->appendChildWidget(loop_reverse_range_slider);
    //loop_reverse_range_slider->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    //loop_reverse_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);

    // loop fx

    y = 380;

    MIP_KnobWidget* loop_fx_knob = new MIP_KnobWidget(MIP_DRect(x,y+30,35,35),"%", 0.0);
    MRootWidget->appendChildWidget(loop_fx_knob);
    loop_fx_knob->setArcThickness(7);
    loop_fx_knob->setDisabled(true);
    loop_fx_knob->setActive(false);
    //loop_fx_knob->setSnap(true);
    //loop_fx_knob->setSnapPos(0.5);
    //loop_fx_knob->setSnapDist(0.05);

  connect( loop_fx_knob, AParameters->getItem(PAR_PROB_FX_PROB_LOOP) );

    MIP_TextWidget* loop_fx_text = new MIP_TextWidget(MIP_DRect(x+45,y+30,130,15),"FX");
    MRootWidget->appendChildWidget(loop_fx_text);
    loop_fx_text->setDrawBorder(false);
    loop_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_fx_text->setDisabled(true);
    loop_fx_text->setActive(false);

    MIP_DualSliderWidget* loop_fx_range_slider = new MIP_DualSliderWidget( MIP_DRect(x+45,y+45,130,20), "%", 0.5, 0.5 );
    MRootWidget->appendChildWidget(loop_fx_range_slider);
    loop_fx_range_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    loop_fx_range_slider->setDisabled(true);
    loop_fx_range_slider->setActive(false);

  connect( loop_fx_range_slider, 0, AParameters->getItem(PAR_PROB_FX_MIN_LOOP) );
  connect( loop_fx_range_slider, 1, AParameters->getItem(PAR_PROB_FX_MAX_LOOP) );

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

    MWaveformWidget->setBuffer(processor->MLeftBuffer,processor->MBufferLength);
    MWaveformWidget->setNumGrid(processor->par_num_beats);
    MWaveformWidget->setNumSubGrid(processor->par_num_slices);

    // marker 0 write pos
    MWaveformWidget->setMarkerActive(0,true);
    MWaveformWidget->setMarkerPos(   0,processor->MWritePos);
    MWaveformWidget->setMarkerColor( 0,MWritePosColor);
    MWaveformWidget->setMarkerWidth( 0,MWritePosWidth);

    // marker 1 read pos
    MWaveformWidget->setMarkerActive(1,true);
    MWaveformWidget->setMarkerPos(   1,processor->MReadPos);
    MWaveformWidget->setMarkerColor( 1,MReadPosColor);
    MWaveformWidget->setMarkerWidth( 1,MReadPosWidth);

    if (processor->MRange) {
      MWaveformWidget->setAreaActive(0,false);
      // area 1 range
      MWaveformWidget->setAreaActive(1,true);
      MWaveformWidget->setAreaStart( 1,processor->MRangeStart);
      MWaveformWidget->setAreaLength(1,processor->MRangeLength);
      MWaveformWidget->setAreaColor( 1,MRangeColor);

      if (processor->MLoop) {
        // area 2 loop
        MWaveformWidget->setAreaActive(2,true);
        MWaveformWidget->setAreaStart( 2,processor->MLoopStart);
        MWaveformWidget->setAreaLength(2,processor->MLoopLength);
        MWaveformWidget->setAreaColor( 2,MLoopColor);
      }
      else {
        MWaveformWidget->setAreaActive(2,false);
      }
    }

    else { // no range
      MWaveformWidget->setAreaActive(1,false);
      MWaveformWidget->setAreaActive(2,false);

      // area 0 current slice
      if (processor->MIsPlaying) {
        MWaveformWidget->setAreaActive(0,true);
        MWaveformWidget->setAreaStart( 0,processor->MSliceStart);
        MWaveformWidget->setAreaLength(0,processor->MSliceLength);
        MWaveformWidget->setAreaColor( 0,MSliceColor);
      }
      else {
        MWaveformWidget->setAreaActive(0,false);
      }

    }

    MWaveformWidget->redraw();

  }

  //----------

  void updateProbIndicators(sa_botage_processor* processor) {

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
      MLoopSizeWidget->setIndicatorValue(processor->rnd_loop_size);
      MLoopSpeedWidget->setIndicatorValue(processor->rnd_loop_speed);
      MLoopOffsetWidget->setIndicatorValue(processor->rnd_loop_offset);
      MLoopReverseWidget->setIndicatorValue(processor->rnd_loop_reverse);
      //
      MRangeSizeValueWidget->setIndicatorValue(processor->rnd_range_size_value);
      MRangeSpeedValueWidget->setIndicatorValue(processor->rnd_range_speed_value);
      MRangeOffsetValueWidget->setIndicatorValue(processor->rnd_range_offset_value);
      //
      MLoopSizeValueWidget->setIndicatorValue(processor->rnd_loop_size_value);
      MLoopSpeedValueWidget->setIndicatorValue(processor->rnd_loop_speed_value);
      MLoopOffsetValueWidget->setIndicatorValue(processor->rnd_loop_offset_value);
    }

    else {
      //MMainProbWidget->setIndicatorValue(-1);
      MMainSlicesWidget->setIndicatorValue(-1);
      MMainLoopsWidget->setIndicatorValue(-1);
      //
      MRangeSizeWidget->setIndicatorValue(-1);
      MRangeSpeedWidget->setIndicatorValue(-1);
      MRangeOffsetWidget->setIndicatorValue(-1);
      MRangeReverseWidget->setIndicatorValue(-1);
      //
      MLoopSizeWidget->setIndicatorValue(-1);
      MLoopSpeedWidget->setIndicatorValue(-1);
      MLoopOffsetWidget->setIndicatorValue(-1);
      MLoopReverseWidget->setIndicatorValue(-1);
      //
      MRangeSizeValueWidget->setIndicatorValue(-1);
      MRangeSpeedValueWidget->setIndicatorValue(-1);
      MRangeOffsetValueWidget->setIndicatorValue(-1);
      //
      MLoopSizeValueWidget->setIndicatorValue(-1);
      MLoopSpeedValueWidget->setIndicatorValue(-1);
      MLoopOffsetValueWidget->setIndicatorValue(-1);
    }

  }

};

//----------------------------------------------------------------------
#endif
