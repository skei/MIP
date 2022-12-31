#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

//----------

#define BACKGROUNDCOLOR 0.2

#define WAVEFORM_LINES  (360 * 3)
#define WAVEFORM_COLOR  MIP_Color( 1,1,1 )
#define WAVEFORM_WIDTH  1.0

//----------------------------------------------------------------------

const char* prob_slices_text[] = {
  "1", "2", "3", "4", "5", "6", "7", "8"
};

#define SA_BOTAGE_FX_TYPE_COUNT 6
const char* fx_type_text[SA_BOTAGE_FX_TYPE_COUNT] = {
  "Off",
  "Filter",
  "Delay",
  "Distortion",
  "Bitcrusher",
  "Comb Filter"
};

//----------------------------------------------------------------------

class sa_botage_MenuWidget
: public MIP_MenuWidget {
public:
  sa_botage_MenuWidget(uint32_t AWidth, uint32_t AHeight, const char** AText, uint32_t ACount)
  : MIP_MenuWidget(MIP_DRect(0,0,AWidth,ACount*AHeight),nullptr) {
    setFillBackground(true);
    for (uint32_t i=0; i<ACount; i++) {
      MIP_MenuItemWidget* menuitem = new MIP_MenuItemWidget( MIP_DRect(0, i*AHeight, AWidth, AHeight ), AText[i] );
      appendChildWidget(menuitem);
      menuitem->setDrawBorder(false);
      menuitem->setFillBackground(true);
      menuitem->setTextSize(8);
      menuitem->setTextSize(9);
      menuitem->setTextColor(0);
      menuitem->setBackgroundColor(0.5);
      //menuitem->Layout.scaleMode = MIP_WIDGET_SCALE_MODE_INITIAL_RATIO;
      //menuitem->setTextSize(-0.7);
    }
  }
};


//----------------------------------------------------------------------

class sa_botage_editor
: public MIP_Editor {

  friend class sa_botage_plugin;

//------------------------------
private:
//------------------------------

  MIP_PanelWidget*            MRootWidget       = nullptr;
  MIP_CircularWaveformWidget* MWaveformWidget   = nullptr;
  MIP_DragValueWidget*        MNumBeatsWidget   = nullptr;
  MIP_DragValueWidget*        MNumSlicesWidget  = nullptr;
  MIP_TextWidget*             MHintWidget       = nullptr;

  MIP_PagesWidget*            MPages            = nullptr;

  MIP_ButtonWidget*           prob_page_button  = nullptr;
  //MIP_ButtonWidget*           fx_page_button    = nullptr;
  MIP_ButtonWidget*           seq_page_button   = nullptr;
  MIP_ButtonWidget*           perf_page_button  = nullptr;

  uint32_t                    MCurrentPage = 0;

  //

  MIP_Color                   MReadPosColor   = MIP_Color( 0.0, 1.0, 0.0, 0.8 );
  MIP_Color                   MWritePosColor  = MIP_Color( 1.0, 0.0, 0.0, 1.0 );

  double                      MReadPosWidth   = 2.0;
  double                      MWritePosWidth  = 2.5;

  MIP_Color                   MRangeColor     = MIP_Color( 0.0, 0.4, 0.0, 0.5 );
  MIP_Color                   MLoopColor      = MIP_Color( 0.2, 0.6, 0.2, 0.7 );
  MIP_Color                   MSliceColor     = MIP_Color( 0.4, 0.4, 0.4, 0.5 );

  //

  MIP_KnobWidget*             MMainProbWidget           = nullptr;

  MIP_ButtonRowWidget*        MMainSlicesWidget         = nullptr;
  MIP_ButtonRowWidget*        MMainLoopsWidget          = nullptr;

  MIP_KnobWidget*             MRangeSizeWidget          = nullptr;
  MIP_KnobWidget*             MRangeSpeedWidget         = nullptr;
  MIP_KnobWidget*             MRangeOffsetWidget        = nullptr;
  MIP_KnobWidget*             MRangeReverseWidget       = nullptr;
  MIP_KnobWidget*             MRangeFXWidget            = nullptr;

  MIP_KnobWidget*             MLoopSizeWidget           = nullptr;
  MIP_KnobWidget*             MLoopSpeedWidget          = nullptr;
  MIP_KnobWidget*             MLoopOffsetWidget         = nullptr;
  MIP_KnobWidget*             MLoopReverseWidget        = nullptr;
  MIP_KnobWidget*             MLoopFXWidget             = nullptr;

  MIP_DualSliderWidget*       MRangeSizeValueWidget     = nullptr;
  MIP_DualSliderWidget*       MRangeSpeedValueWidget    = nullptr;
  MIP_DualSliderWidget*       MRangeOffsetValueWidget   = nullptr;
//MIP_DualSliderWidget*       MRangeReverseValueWidget  = nullptr;
  MIP_DualSliderWidget*       MRangeFXValueWidget       = nullptr;

  MIP_DualSliderWidget*       MLoopSizeValueWidget      = nullptr;
  MIP_DualSliderWidget*       MLoopSpeedValueWidget     = nullptr;
  MIP_DualSliderWidget*       MLoopOffsetValueWidget    = nullptr;
//MIP_DualSliderWidget*       MLoopReverseValueWidget   = nullptr;
  MIP_DualSliderWidget*       MLoopFXValueWidget        = nullptr;

//------------------------------
public:
//------------------------------

  sa_botage_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray* AParameters, const clap_plugin_descriptor_t* descriptor)
  : MIP_Editor(AListener,AWidth,AHeight,AParameters) {

    // root

    MRootWidget = new MIP_PanelWidget(MIP_DRect(0,0,SA_BOTAGE_EDITOR_WIDTH,SA_BOTAGE_EDITOR_HEIGHT));
    setRootWidget(MRootWidget);
    MRootWidget->setFillBackground(true);
    MRootWidget->setBackgroundColor(BACKGROUNDCOLOR);
    MRootWidget->setDrawBorder(true);
    MRootWidget->setBorderColor(0.5);

    // header

    MIP_SAHeaderWidget* header  = new MIP_SAHeaderWidget(MIP_DRect(0,0,AWidth,80),descriptor);
    MRootWidget->appendChildWidget(header);

    // c

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

    // i

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

    MWaveformWidget = new MIP_CircularWaveformWidget( MIP_DRect(10,90,230,230), WAVEFORM_LINES );
    MRootWidget->appendChildWidget(MWaveformWidget);
    MWaveformWidget->setGrid(4,2);
    MWaveformWidget->setFillBackground(false);;
    MWaveformWidget->setWaveformWidth(WAVEFORM_WIDTH);
    MWaveformWidget->setGridColor(0.55, 0.4);
    MWaveformWidget->setZeroLineColor(0.6);
    MWaveformWidget->setWaveformColor( WAVEFORM_COLOR );


    // beats

    MNumBeatsWidget = new MIP_DragValueWidget( MIP_DRect(10,330,110,16),"Beats");
    //MNumBeatsWidget = new MIP_DragValueWidget( MIP_DRect(255,90,103,16),"Beats");
    MRootWidget->appendChildWidget(MNumBeatsWidget);
    MNumBeatsWidget->setFillBackground(true);
    MNumBeatsWidget->setBackgroundColor( 0.5 );
    MNumBeatsWidget->setTextColor(MIP_COLOR_BLACK);
    MNumBeatsWidget->setTextSize(11);
    MNumBeatsWidget->setValueColor(MIP_COLOR_BLACK);
    MNumBeatsWidget->setValueSize(11);
    MNumBeatsWidget->setHint("Number of beats in buffer");
    MNumBeatsWidget->setInteractiveValueColor(MIP_COLOR_WHITE);

    connect( MNumBeatsWidget,  AParameters->getItem(PAR_NUM_BEATS) );

    // slices

    MNumSlicesWidget = new MIP_DragValueWidget( MIP_DRect(130,330,110,16),"Slices");
    //MNumSlicesWidget = new MIP_DragValueWidget( MIP_DRect(255,111,103,16),"Slices");
    MRootWidget->appendChildWidget(MNumSlicesWidget);
    MNumSlicesWidget->setFillBackground(true);
    MNumSlicesWidget->setBackgroundColor( 0.5 );
    MNumSlicesWidget->setTextColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setTextSize(11);
    MNumSlicesWidget->setValueColor(MIP_COLOR_BLACK);
    MNumSlicesWidget->setValueSize(11);
    MNumSlicesWidget->setHint("Number of slices per beat");
    MNumSlicesWidget->setInteractiveValueColor(MIP_COLOR_WHITE);

    connect( MNumSlicesWidget, AParameters->getItem(PAR_NUM_SLICES) );

    // loop env

    MIP_EnvSliderWidget* env1_slider = new MIP_EnvSliderWidget( MIP_DRect(10,356,110,16), "L ms", 0.1, 0.9 );
    //MIP_EnvSliderWidget* env1_slider = new MIP_EnvSliderWidget( MIP_DRect(255,132,103,16), "L ms", 0.1, 0.9 );
    MRootWidget->appendChildWidget(env1_slider);
    env1_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    env1_slider->setHint("Loop Envelope");
    env1_slider->setBackgroundColor(0.4);
    env1_slider->setSliderColor(0.6);
    env1_slider->setTextColor(MIP_Color(0,0,0,0.5));
    env1_slider->setTextSize(9);
    env1_slider->setValueColor(0.2);
    env1_slider->setBackgroundColor( MIP_Color(0.0, 0.5, 0.5) );
    env1_slider->setSliderColor( MIP_Color(0.0, 0.7, 0.7) );
    env1_slider->setInteractiveSliderColor( MIP_Color(0.0, 0.9, 0.9) );

    connect( env1_slider, 0, AParameters->getItem(PAR_LOOP_ENV_ATTACK) );
    connect( env1_slider, 1, AParameters->getItem(PAR_LOOP_ENV_DECAY) );

    // slice env

    MIP_EnvSliderWidget* env2_slider = new MIP_EnvSliderWidget( MIP_DRect(130,356,110,16), "S %", 0.1, 0.9 );
    //MIP_EnvSliderWidget* env2_slider = new MIP_EnvSliderWidget( MIP_DRect(255,153,103,16), "S %", 0.1, 0.9 );
    MRootWidget->appendChildWidget(env2_slider);
    env2_slider->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    env2_slider->setHint("Slice envelope");
    env2_slider->setBackgroundColor(0.4);
    env2_slider->setSliderColor(0.6);
    env2_slider->setTextColor(MIP_Color(0,0,0,0.5));
    env2_slider->setTextSize(9);
    env2_slider->setValueColor(0.2);
    env2_slider->setBackgroundColor( MIP_Color(0.0, 0.5, 0.5) );
    env2_slider->setSliderColor( MIP_Color(0.0, 0.7, 0.7) );
    env2_slider->setInteractiveSliderColor( MIP_Color(0.0, 0.9, 0.9) );

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

//    fx_page_button = new MIP_ButtonWidget(MIP_DRect(275,5,30,15), "FX",0);
//    MRootWidget->appendChildWidget(fx_page_button);
//    fx_page_button->setHint("Effects");
//    fx_page_button->setFillBackground(true);
//    fx_page_button->setBackgroundColor(0.25);
//    fx_page_button->setDrawBorder(false);
//    fx_page_button->setTextSize(10);
//    fx_page_button->setTextColor(0.6);

    seq_page_button = new MIP_ButtonWidget(MIP_DRect(275/*310*/,5,30,15), "Seq",0);
    MRootWidget->appendChildWidget(seq_page_button);
    seq_page_button->setHint("Sequence");
    seq_page_button->setFillBackground(true);
    seq_page_button->setBackgroundColor(0.25);
    seq_page_button->setDrawBorder(false);
    seq_page_button->setTextSize(10);
    seq_page_button->setTextColor(0.6);

    perf_page_button = new MIP_ButtonWidget(MIP_DRect(310/*345*/,5,30,15), "Perf",0);
    MRootWidget->appendChildWidget(perf_page_button);
    perf_page_button->setHint("Perform");
    perf_page_button->setFillBackground(true);
    perf_page_button->setBackgroundColor(0.25);
    perf_page_button->setDrawBorder(false);
    perf_page_button->setTextSize(10);
    perf_page_button->setTextColor(0.6);

    MPages = new MIP_PagesWidget( MIP_DRect(255,90,/*225*/357,292));
    MRootWidget->appendChildWidget(MPages);

    #include "sa_botage_editor_page_prob.h"
//    #include "sa_botage_editor_page_fx.h"
    #include "sa_botage_editor_page_seq.h"
    #include "sa_botage_editor_page_perf.h"

    MPages->appendPage(page_prob);
//    MPages->appendPage(page_fx);
    MPages->appendPage(page_seq);
    MPages->appendPage(page_perf);

    MPages->setPage(0);

    // footer

    //double height = ARootWidget->getHeight();
    //sa_botage_footer* footer  = new sa_botage_footer(MIP_DRect(0,(height - SA_BOTAGE_FOOTER_HEIGHT),width,SA_BOTAGE_FOOTER_HEIGHT));
    //ARootWidget->appendChildWidget(footer);

    MRootWidget->appendChildWidget(fx_type_menu);


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
    MWaveformWidget->setGrid(beats,slices);
    //double scale = 1.0;
    //double aspect = (double)MEditorWidth / (double)MEditorHeight;
    //if (aspect >= MAspectRatio) scale = (double)MEditorHeight / (double)MInitialHeight;
    //else scale = (double)MEditorWidth / (double)MInitialWidth;
    //MWindow->setWindowScale(scale);
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
//      fx_page_button->setBackgroundColor(0.25);
      seq_page_button->setBackgroundColor(0.25);
      perf_page_button->setBackgroundColor(0.25);
      prob_page_button->redraw();
  //    fx_page_button->redraw();
      seq_page_button->redraw();
      perf_page_button->redraw();
    }
//    else if (AWidget == fx_page_button)   {
//      MCurrentPage = 1;
//      MPages->setPage(1);
//      MPages->redraw();
//      prob_page_button->setBackgroundColor(0.25);
//      fx_page_button->setBackgroundColor(0.3);
//      seq_page_button->setBackgroundColor(0.25);
//      perf_page_button->setBackgroundColor(0.25);
//      prob_page_button->redraw();
//      fx_page_button->redraw();
//      seq_page_button->redraw();
//      perf_page_button->redraw();
//    }
    else if (AWidget == seq_page_button)  {
      MCurrentPage = 1;//2;
      MPages->setPage(2); MPages->redraw();
      prob_page_button->setBackgroundColor(0.25);
//      fx_page_button->setBackgroundColor(0.25);
      seq_page_button->setBackgroundColor(0.3);
      perf_page_button->setBackgroundColor(0.25);
      prob_page_button->redraw();
//      fx_page_button->redraw();
      seq_page_button->redraw();
      perf_page_button->redraw();
    }
    else if (AWidget == perf_page_button) {
      MCurrentPage = 2;//3;
      MPages->setPage(3); MPages->redraw();
      prob_page_button->setBackgroundColor(0.25);
//      fx_page_button->setBackgroundColor(0.25);
      seq_page_button->setBackgroundColor(0.25);
      perf_page_button->setBackgroundColor(0.3);
      prob_page_button->redraw();
//      fx_page_button->redraw();
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

  void updateWaveformWidget(sa_botage_processor* processor) {

    uint32_t buflen = processor->MBufferLength;
    if (buflen > 0) {

      MWaveformWidget->setAudioBuffer(buflen, processor->MLeftBuffer, false);
      MWaveformWidget->setGrid(processor->par_num_beats, processor->par_num_slices);

      // marker 0 write pos

      MWaveformWidget->setMarkerActive(0, true);
      MWaveformWidget->setMarkerPos(   0, (double)processor->MWritePos/buflen, 0.0025);
      MWaveformWidget->setMarkerColor( 0, MWritePosColor);
      //MWaveformWidget->setMarkerWidth( 0, MWritePosWidth);

      // marker 1 read pos

      MWaveformWidget->setMarkerActive(1, true);
      MWaveformWidget->setMarkerPos(   1, processor->MReadPos/buflen, 0.0025);
      MWaveformWidget->setMarkerColor( 1, MReadPosColor);
      //MWaveformWidget->setMarkerWidth( 1,MReadPosWidth);

      if (processor->MRange) {

        // area 1 range

        MWaveformWidget->setAreaActive(0, false);
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

    // only if page is visible
    if (MCurrentPage == 0) {

      MMainProbWidget->setIndicatorValue(processor->rnd_main_prob);
      if (processor->MRange) {

        //MMainProbWidget->setIndicatorValue(processor->rnd_main_prob);
        MMainSlicesWidget->setIndicatorValue(processor->rnd_main_slices);
        MMainLoopsWidget->setIndicatorValue(processor->rnd_main_subdiv);

        MRangeSizeWidget->setIndicatorValue(processor->rnd_range_size);
        MRangeSpeedWidget->setIndicatorValue(processor->rnd_range_speed);
        MRangeOffsetWidget->setIndicatorValue(processor->rnd_range_offset);
        MRangeReverseWidget->setIndicatorValue(processor->rnd_range_reverse);
        MRangeFXWidget->setIndicatorValue(processor->rnd_range_fx);

        if (processor->rnd_range_size_on) MRangeSizeValueWidget->setIndicatorValue(processor->rnd_range_size_value);
        else MRangeSizeValueWidget->setIndicatorValue(-1);
        if (processor->rnd_range_speed_on) MRangeSpeedValueWidget->setIndicatorValue(processor->rnd_range_speed_value);
        else MRangeSpeedValueWidget->setIndicatorValue(-1);
        if (processor->rnd_range_offset_on) MRangeOffsetValueWidget->setIndicatorValue(processor->rnd_range_offset_value);
        else MRangeOffsetValueWidget->setIndicatorValue(-1);
        if (processor->rnd_range_fx_on) MRangeFXValueWidget->setIndicatorValue(processor->rnd_range_fx_value);
        else MRangeFXValueWidget->setIndicatorValue(-1);

        if (processor->MLoopWrapped) {
          MLoopSizeWidget->setIndicatorValue(processor->rnd_loop_size);
          MLoopSpeedWidget->setIndicatorValue(processor->rnd_loop_speed);
          MLoopOffsetWidget->setIndicatorValue(processor->rnd_loop_offset);
          MLoopReverseWidget->setIndicatorValue(processor->rnd_loop_reverse);
          MLoopFXWidget->setIndicatorValue(processor->rnd_loop_fx);

          if (processor->rnd_loop_size_on) MLoopSizeValueWidget->setIndicatorValue(processor->rnd_loop_size_value);
          else MLoopSizeValueWidget->setIndicatorValue(-1);
          if (processor->rnd_loop_speed_on) MLoopSpeedValueWidget->setIndicatorValue(processor->rnd_loop_speed_value);
          else MLoopSpeedValueWidget->setIndicatorValue(-1);
          if (processor->rnd_loop_offset_on) MLoopOffsetValueWidget->setIndicatorValue(processor->rnd_loop_offset_value);
          else MLoopOffsetValueWidget->setIndicatorValue(-1);
          if (processor->rnd_loop_fx_on) MLoopFXValueWidget->setIndicatorValue(processor->rnd_loop_fx_value);
          else MLoopFXValueWidget->setIndicatorValue(-1);
        }
        else {
          MLoopSizeWidget->setIndicatorValue(-1);
          MLoopSpeedWidget->setIndicatorValue(-1);
          MLoopOffsetWidget->setIndicatorValue(-1);
          MLoopReverseWidget->setIndicatorValue(-1);
          MLoopFXWidget->setIndicatorValue(-1);

          MLoopSizeValueWidget->setIndicatorValue(-1);
          MLoopSpeedValueWidget->setIndicatorValue(-1);
          MLoopOffsetValueWidget->setIndicatorValue(-1);
          MLoopFXValueWidget->setIndicatorValue(-1);
        }
      }
      else { // ! range

        //MMainProbWidget->setIndicatorValue(-1);
        MMainSlicesWidget->setIndicatorValue(-1);
        MMainLoopsWidget->setIndicatorValue(-1);

        MRangeSizeWidget->setIndicatorValue(-1);
        MRangeSpeedWidget->setIndicatorValue(-1);
        MRangeOffsetWidget->setIndicatorValue(-1);
        MRangeReverseWidget->setIndicatorValue(-1);
        MRangeFXWidget->setIndicatorValue(-1);

        MRangeSizeValueWidget->setIndicatorValue(-1);
        MRangeSpeedValueWidget->setIndicatorValue(-1);
        MRangeOffsetValueWidget->setIndicatorValue(-1);
        MRangeFXValueWidget->setIndicatorValue(-1);

        MLoopSizeWidget->setIndicatorValue(-1);
        MLoopSpeedWidget->setIndicatorValue(-1);
        MLoopOffsetWidget->setIndicatorValue(-1);
        MLoopReverseWidget->setIndicatorValue(-1);
        MLoopFXWidget->setIndicatorValue(-1);

        MLoopSizeValueWidget->setIndicatorValue(-1);
        MLoopSpeedValueWidget->setIndicatorValue(-1);
        MLoopOffsetValueWidget->setIndicatorValue(-1);
        MLoopFXValueWidget->setIndicatorValue(-1);
      }

    } // page == 0
  }

};

//----------------------------------------------------------------------
#endif
