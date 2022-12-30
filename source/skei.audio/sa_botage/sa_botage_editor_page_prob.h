
// inline

//----------------------------------------------------------------------

    MIP_Widget* page_prob = new MIP_Widget( MIP_DRect(255,90,225,292));

    //----------

    // main prob

    MMainProbWidget = new MIP_KnobWidget(MIP_DRect(255,90,80,80), "%", 0.0);
    page_prob->appendChildWidget(MMainProbWidget);
    MMainProbWidget->setHint("Main probability");
    MMainProbWidget->setDrawText(true);
    MMainProbWidget->setDrawValue(true);
    MMainProbWidget->setArcThickness(15);
    MMainProbWidget->setTextSize(20);
    MMainProbWidget->setValueSize(25);
    MMainProbWidget->setTextColor(0.6);
    MMainProbWidget->setValueColor(0.7);
    MMainProbWidget->setArcBackColor(             MIP_Color(0.0, 0.3, 0.6) );
    MMainProbWidget->setArcValueColor(            MIP_Color(0.0, 0.5, 1.0) );
    MMainProbWidget->setInteractiveArcValueColor( MIP_Color(0.5, 0.75, 1.0) );

    connect( MMainProbWidget, AParameters->getItem(PAR_RANGE_PROB) );

    // slices

    MIP_TextWidget* prob_slices_label = new MIP_TextWidget( MIP_DRect(345,95,135,12), "Slices" );
    page_prob->appendChildWidget(prob_slices_label);
    prob_slices_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_slices_label->setTextColor(0.8);
    prob_slices_label->setTextSize(12);
    prob_slices_label->setDrawBorder(false);
    prob_slices_label->setFillBackground(false);

    MMainSlicesWidget = new MIP_ButtonRowWidget( MIP_DRect(345,109,135,16), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    page_prob->appendChildWidget(MMainSlicesWidget);
    MMainSlicesWidget->setHint("Number of slices in range");
    MMainSlicesWidget->selectButton(0);
    MMainSlicesWidget->setAllowZeroBits(false);
    MMainSlicesWidget->setValueIsBits(true,8);
    MMainSlicesWidget->setDrawBorder(false);
    MMainSlicesWidget->setBackgroundCellColor(MIP_Color(0.0, 0.3, 0.6) );
    MMainSlicesWidget->setActiveCellColor(    MIP_Color(0.0, 0.5, 1.0) );

    connect( MMainSlicesWidget,  AParameters->getItem(PAR_RANGE_SLICE_COUNT) );

    // loops

    MIP_TextWidget* prob_loops_label = new MIP_TextWidget( MIP_DRect(345,134,135,12), "Loops" );
    page_prob->appendChildWidget(prob_loops_label);
    prob_loops_label->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    prob_loops_label->setTextColor(0.8);
    prob_loops_label->setTextSize(12);
    prob_loops_label->setDrawBorder(false);
    prob_loops_label->setFillBackground(false);

    MMainLoopsWidget = new MIP_ButtonRowWidget( MIP_DRect(345,148,135,16), 8, prob_slices_txt, MIP_BUTTON_ROW_MULTI );
    page_prob->appendChildWidget(MMainLoopsWidget);
    MMainLoopsWidget->selectButton(0);
    MMainLoopsWidget->setAllowZeroBits(false);
    MMainLoopsWidget->setValueIsBits(true,8);
    MMainLoopsWidget->setHint("Range subdivision");
    MMainLoopsWidget->setDrawBorder(false);
    MMainLoopsWidget->setBackgroundCellColor( MIP_Color(0.0, 0.3, 0.6) );
    MMainLoopsWidget->setActiveCellColor(     MIP_Color(0.0, 0.5, 1.0) );

    connect( MMainLoopsWidget,  AParameters->getItem(PAR_RANGE_LOOP_COUNT) );

    // txt range

    MIP_TextWidget* c1_header = new MIP_TextWidget(MIP_DRect(255,180,103,14),"Range");
    page_prob->appendChildWidget(c1_header);
    c1_header->setDrawBorder(true);
    c1_header->setBorderColor(0.6);
    c1_header->setBorderEdges(MIP_EDGE_BOTTOM);
    c1_header->setFillBackground(false);
    c1_header->setBackgroundColor(0.35);
    c1_header->setTextColor(0.8);
    c1_header->setTextSize(11);

    // txt loop

    MIP_TextWidget* c2_header = new MIP_TextWidget(MIP_DRect(377,180,103,14),"Loop");
    page_prob->appendChildWidget(c2_header);
    c2_header->setDrawBorder(true);
    c2_header->setBorderColor(0.6);
    c2_header->setBorderEdges(MIP_EDGE_BOTTOM);
    c2_header->setFillBackground(false);
    c2_header->setBackgroundColor(0.35);
    c2_header->setTextColor(0.8);
    c2_header->setTextSize(11);

    // range length

    MIP_TextWidget* range_length_text = new MIP_TextWidget(MIP_DRect(292,205,66,9),"Length");
    page_prob->appendChildWidget(range_length_text);
    range_length_text->setDrawBorder(false);
    range_length_text->setFillBackground(false);
    range_length_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_length_text->setTextColor(0.6);
    range_length_text->setTextSize(10);

    MRangeSizeWidget = new MIP_KnobWidget(MIP_DRect(255,205,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeSizeWidget);
    MRangeSizeWidget->setArcThickness(7);
    MRangeSizeWidget->setHint("Loop length");
    MRangeSizeWidget->setTextColor(0.6);
    MRangeSizeWidget->setValueColor(0.8);
    MRangeSizeWidget->setArcBackColor(            MIP_Color(0.5, 0.0, 0.0) );
    MRangeSizeWidget->setArcValueColor(           MIP_Color(0.7, 0.0, 0.0) );
    MRangeSizeWidget->setInteractiveArcValueColor(MIP_Color(0.9, 0.0, 0.0) );

    connect( MRangeSizeWidget, AParameters->getItem(PAR_PROB_SIZE_PROB_RANGE) );

    MRangeSizeValueWidget = new MIP_DualSliderWidget( MIP_DRect(292,216,66,16), "%");
    page_prob->appendChildWidget(MRangeSizeValueWidget);
    MRangeSizeValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MRangeSizeValueWidget->setHint("Loop min/max");
    MRangeSizeValueWidget->setBackgroundColor( MIP_Color(0.5, 0.0, 0.0) );
    MRangeSizeValueWidget->setTextColor(              MIP_Color(0,0,0,0.5));
    MRangeSizeValueWidget->setSliderColor(            MIP_Color(0.7, 0.0, 0.0) );
    MRangeSizeValueWidget->setInteractiveSliderColor( MIP_Color(0.9, 0.0, 0.0) );

    connect( MRangeSizeValueWidget, 0, AParameters->getItem(PAR_PROB_SIZE_MIN_RANGE) );
    connect( MRangeSizeValueWidget, 1, AParameters->getItem(PAR_PROB_SIZE_MAX_RANGE) );

    // loop length

    MIP_TextWidget* loop_length_text = new MIP_TextWidget(MIP_DRect(414,205,66,9),"Length");
    page_prob->appendChildWidget(loop_length_text);
    loop_length_text->setDrawBorder(false);
    loop_length_text->setFillBackground(false);
    loop_length_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_length_text->setTextColor(0.6);
    loop_length_text->setTextSize(10);

    MLoopSizeWidget = new MIP_KnobWidget(MIP_DRect(377,205,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopSizeWidget);
    MLoopSizeWidget->setArcThickness(7);
    MLoopSizeWidget->setValueColor(0.8);
    MLoopSizeWidget->setTextColor(0.6);
    MLoopSizeWidget->setArcBackColor(             MIP_Color(0.5, 0.0, 0.0) );
    MLoopSizeWidget->setArcValueColor(            MIP_Color(0.7, 0.0, 0.0) );
    MLoopSizeWidget->setInteractiveArcValueColor( MIP_Color(0.9, 0.0, 0.0) );

    connect( MLoopSizeWidget, AParameters->getItem(PAR_PROB_SIZE_PROB_LOOP) );

    MLoopSizeValueWidget = new MIP_DualSliderWidget( MIP_DRect(414,216,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopSizeValueWidget);
    MLoopSizeValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MLoopSizeValueWidget->setBackgroundColor(       MIP_Color(0.5, 0.0, 0.0) );
    MLoopSizeValueWidget->setSliderColor(           MIP_Color(0.7, 0.0, 0.0) );
    MLoopSizeValueWidget->setInteractiveSliderColor(MIP_Color(0.9, 0.0, 0.0) );
    MLoopSizeValueWidget->setTextColor(             MIP_Color(0, 0, 0, 0.5));

    connect( MLoopSizeValueWidget, 0, AParameters->getItem(PAR_PROB_SIZE_MIN_LOOP) );
    connect( MLoopSizeValueWidget, 1, AParameters->getItem(PAR_PROB_SIZE_MAX_LOOP) );

    // range speed

    MIP_TextWidget* range_speed_text = new MIP_TextWidget(MIP_DRect(292,240,66,9),"Speed");
    page_prob->appendChildWidget(range_speed_text);
    range_speed_text->setDrawBorder(false);
    range_speed_text->setFillBackground(false);
    range_speed_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_speed_text->setTextColor(0.6);
    range_speed_text->setTextSize(10);

    MRangeSpeedWidget = new MIP_KnobWidget(MIP_DRect(255,240,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeSpeedWidget);
    MRangeSpeedWidget->setArcThickness(7);
    MRangeSpeedWidget->setValueColor(0.8);
    MRangeSpeedWidget->setTextColor(0.6);
    MRangeSpeedWidget->setArcBackColor(             MIP_Color(0.5, 0.25, 0.0) );
    MRangeSpeedWidget->setArcValueColor(            MIP_Color(0.7, 0.35, 0.0) );
    MRangeSpeedWidget->setInteractiveArcValueColor( MIP_Color(0.9, 0.45, 0.0) );

    connect( MRangeSpeedWidget, AParameters->getItem(PAR_PROB_SPEED_PROB_RANGE) );

    MRangeSpeedValueWidget= new MIP_DualSliderWidget( MIP_DRect(292,251,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MRangeSpeedValueWidget);
    MRangeSpeedValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MRangeSpeedValueWidget->setBackgroundColor(       MIP_Color(0.5, 0.25, 0.0) );
    MRangeSpeedValueWidget->setSliderColor(           MIP_Color(0.7, 0.35, 0.0) );
    MRangeSpeedValueWidget->setInteractiveSliderColor(MIP_Color(0.9, 0.45, 0.0) );
    MRangeSpeedValueWidget->setTextColor(             MIP_Color(0, 0, 0, 0.5));

    connect( MRangeSpeedValueWidget, 0, AParameters->getItem(PAR_PROB_SPEED_MIN_RANGE) );
    connect( MRangeSpeedValueWidget, 1, AParameters->getItem(PAR_PROB_SPEED_MAX_RANGE) );

    // loop speed

    MIP_TextWidget* loop_speed_text = new MIP_TextWidget(MIP_DRect(414,240,66,9),"Speed");
    page_prob->appendChildWidget(loop_speed_text);
    loop_speed_text->setDrawBorder(false);
    loop_speed_text->setFillBackground(false);
    loop_speed_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_speed_text->setTextColor(0.6);
    loop_speed_text->setTextSize(10);

    MLoopSpeedWidget = new MIP_KnobWidget(MIP_DRect(377,240,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopSpeedWidget);
    MLoopSpeedWidget->setArcThickness(7);
    MLoopSpeedWidget->setValueColor(0.8);
    MLoopSpeedWidget->setTextColor(0.6);
    MLoopSpeedWidget->setArcBackColor(            MIP_Color(0.5, 0.25, 0.0) );
    MLoopSpeedWidget->setArcValueColor(           MIP_Color(0.7, 0.35, 0.0) );
    MLoopSpeedWidget->setInteractiveArcValueColor(MIP_Color(0.9, 0.45, 0.0) );

    connect( MLoopSpeedWidget, AParameters->getItem(PAR_PROB_SPEED_PROB_LOOP) );

    MLoopSpeedValueWidget = new MIP_DualSliderWidget( MIP_DRect(414,251,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopSpeedValueWidget);
    MLoopSpeedValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MLoopSpeedValueWidget->setBackgroundColor(        MIP_Color(0.5, 0.25, 0.0) );
    MLoopSpeedValueWidget->setSliderColor(            MIP_Color(0.7, 0.35, 0.0) );
    MLoopSpeedValueWidget->setInteractiveSliderColor( MIP_Color(0.9, 0.45, 0.0) );
    MLoopSpeedValueWidget->setTextColor(              MIP_Color(0, 0, 0, 0.5));

    connect( MLoopSpeedValueWidget, 0, AParameters->getItem(PAR_PROB_SPEED_MIN_LOOP) );
    connect( MLoopSpeedValueWidget, 1, AParameters->getItem(PAR_PROB_SPEED_MAX_LOOP) );

    // range offset

    MIP_TextWidget* range_offset_text = new MIP_TextWidget(MIP_DRect(292,275,66,9),"Offset");
    page_prob->appendChildWidget(range_offset_text);
    range_offset_text->setFillBackground(false);
    range_offset_text->setDrawBorder(false);
    range_offset_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_offset_text->setTextColor(0.6);
    range_offset_text->setTextSize(10);

    MRangeOffsetWidget = new MIP_KnobWidget(MIP_DRect(255,275,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeOffsetWidget);
    MRangeOffsetWidget->setArcThickness(7);
    MRangeOffsetWidget->setValueColor(0.8);
    MRangeOffsetWidget->setTextColor(0.6);
    MRangeOffsetWidget->setArcBackColor(            MIP_Color(0.5, 0.5, 0.0) );
    MRangeOffsetWidget->setArcValueColor(           MIP_Color(0.7, 0.7, 0.0) );
    MRangeOffsetWidget->setInteractiveArcValueColor(MIP_Color(0.9, 0.9, 0.0) );

    connect( MRangeOffsetWidget, AParameters->getItem(PAR_PROB_OFFSET_PROB_RANGE) );

    MRangeOffsetValueWidget = new MIP_DualSliderWidget( MIP_DRect(292,286,66,16), "sl", 0.5, 0.5 );
    page_prob->appendChildWidget(MRangeOffsetValueWidget);
    MRangeOffsetValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MRangeOffsetValueWidget->setBackgroundColor(        MIP_Color(0.5, 0.5, 0.0) );
    MRangeOffsetValueWidget->setSliderColor(            MIP_Color(0.7, 0.7, 0.0) );
    MRangeOffsetValueWidget->setInteractiveSliderColor( MIP_Color(0.9, 0.9, 0.0) );
    MRangeOffsetValueWidget->setTextColor(              MIP_Color(0, 0, 0, 0.5));

    connect( MRangeOffsetValueWidget, 0, AParameters->getItem(PAR_PROB_OFFSET_MIN_RANGE) );
    connect( MRangeOffsetValueWidget, 1, AParameters->getItem(PAR_PROB_OFFSET_MAX_RANGE) );

    // loop offset

    MIP_TextWidget* loop_offset_text = new MIP_TextWidget(MIP_DRect(414,275,66,9),"Offset");
    page_prob->appendChildWidget(loop_offset_text);
    loop_offset_text->setDrawBorder(false);
    loop_offset_text->setFillBackground(false);
    loop_offset_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_offset_text->setTextColor(0.6);
    loop_offset_text->setTextSize(10);

    MLoopOffsetWidget = new MIP_KnobWidget(MIP_DRect(377,275,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopOffsetWidget);
    MLoopOffsetWidget->setArcThickness(7);
    MLoopOffsetWidget->setValueColor(0.8);
    MLoopOffsetWidget->setTextColor(0.6);
    MLoopOffsetWidget->setArcBackColor(             MIP_Color(0.5, 0.5, 0.0) );
    MLoopOffsetWidget->setArcValueColor(            MIP_Color(0.7, 0.7, 0.0) );
    MLoopOffsetWidget->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.0) );

    connect( MLoopOffsetWidget, AParameters->getItem(PAR_PROB_OFFSET_PROB_LOOP) );

    MLoopOffsetValueWidget = new MIP_DualSliderWidget( MIP_DRect(414,286,66,16), "sl", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopOffsetValueWidget);
    MLoopOffsetValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MLoopOffsetValueWidget->setBackgroundColor(       MIP_Color(0.5, 0.5, 0.0) );
    MLoopOffsetValueWidget->setSliderColor(           MIP_Color(0.7, 0.7, 0.0) );
    MLoopOffsetValueWidget->setInteractiveSliderColor(MIP_Color(0.9, 0.9, 0.0) );
    MLoopOffsetValueWidget->setTextColor(             MIP_Color(0, 0, 0, 0.5));

    connect( MLoopOffsetValueWidget, 0, AParameters->getItem(PAR_PROB_OFFSET_MIN_LOOP) );
    connect( MLoopOffsetValueWidget, 1, AParameters->getItem(PAR_PROB_OFFSET_MAX_LOOP) );

    // range reverse

    MIP_TextWidget* range_reverse_text = new MIP_TextWidget(MIP_DRect(292,310,66,9),"Reverse");
    page_prob->appendChildWidget(range_reverse_text);
    range_reverse_text->setFillBackground(false);
    range_reverse_text->setDrawBorder(false);
    range_reverse_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_reverse_text->setTextColor(0.6);
    range_reverse_text->setTextSize(10);

    MRangeReverseWidget = new MIP_KnobWidget(MIP_DRect(255,310,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeReverseWidget);
    MRangeReverseWidget->setArcThickness(7);
    MRangeReverseWidget->setValueColor(0.8);
    MRangeReverseWidget->setTextColor(0.6);
    MRangeReverseWidget->setArcBackColor(             MIP_Color(0.25, 0.5, 0.0) );
    MRangeReverseWidget->setArcValueColor(            MIP_Color(0.35, 0.7, 0.0) );
    MRangeReverseWidget->setInteractiveArcValueColor( MIP_Color(0.45, 0.9, 0.0) );

    connect( MRangeReverseWidget, AParameters->getItem(PAR_PROB_REVERSE_PROB_RANGE) );

    // loop reverse

    MIP_TextWidget* loop_reverse_text = new MIP_TextWidget(MIP_DRect(414,310,66,9),"Reverse");
    page_prob->appendChildWidget(loop_reverse_text);
    loop_reverse_text->setDrawBorder(false);
    loop_reverse_text->setFillBackground(false);
    loop_reverse_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_reverse_text->setTextColor(0.6);
    loop_reverse_text->setTextSize(10);

    MLoopReverseWidget = new MIP_KnobWidget(MIP_DRect(377,310,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopReverseWidget);
    MLoopReverseWidget->setArcThickness(7);
    MLoopReverseWidget->setValueColor(0.8);
    MLoopReverseWidget->setTextColor(0.6);
    MLoopReverseWidget->setArcBackColor(            MIP_Color(0.25, 0.5, 0.0) );
    MLoopReverseWidget->setArcValueColor(           MIP_Color(0.35, 0.7, 0.0) );
    MLoopReverseWidget->setInteractiveArcValueColor(MIP_Color(0.45, 0.9, 0.0) );

    connect( MLoopReverseWidget, AParameters->getItem(PAR_PROB_REVERSE_PROB_LOOP) );

    // range fx

    MIP_TextWidget* range_fx_text = new MIP_TextWidget(MIP_DRect(292,345,66,9),"FX");
    page_prob->appendChildWidget(range_fx_text);
    range_fx_text->setFillBackground(false);
    range_fx_text->setDrawBorder(false);
    range_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_fx_text->setTextColor(0.6);
    range_fx_text->setTextSize(10);
    //range_fx_text->setDisabled(true);
    //range_fx_text->setDisabledColor(BACKGROUNDCOLOR);
    //range_fx_text->setActive(false);

    MRangeFXWidget = new MIP_KnobWidget(MIP_DRect(255,345,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeFXWidget);
    MRangeFXWidget->setArcThickness(7);
    MRangeFXWidget->setValueColor(0.8);
    MRangeFXWidget->setTextColor(0.6);
    MRangeFXWidget->setArcBackColor(             MIP_Color(0.0, 0.5, 0.0) );
    MRangeFXWidget->setArcValueColor(            MIP_Color(0.0, 0.7, 0.0) );
    MRangeFXWidget->setInteractiveArcValueColor( MIP_Color(0.0, 0.9, 0.0) );
    //MRangeFXWidget->setDisabled(true);
    //MRangeFXWidget->setDisabledColor(BACKGROUNDCOLOR);
    //MRangeFXWidget->setActive(false);

    connect( MRangeFXWidget, AParameters->getItem(PAR_PROB_FX_PROB_RANGE) );

    MRangeFXValueWidget = new MIP_DualSliderWidget( MIP_DRect(292,356,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MRangeFXValueWidget);
    MRangeFXValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MRangeFXValueWidget->setBackgroundColor(        MIP_Color(0.0, 0.5, 0.0) );
    MRangeFXValueWidget->setSliderColor(            MIP_Color(0.0, 0.7, 0.0) );
    MRangeFXValueWidget->setInteractiveSliderColor( MIP_Color(0.0, 0.9, 0.0) );
    MRangeFXValueWidget->setTextColor(              MIP_Color(0, 0, 0, 0.5));
    //MRangeFXValueWidget->setDisabled(true);
    //MRangeFXValueWidget->setDisabledColor(BACKGROUNDCOLOR);
    //MRangeFXValueWidget->setActive(false);

    connect( MRangeFXValueWidget, 0, AParameters->getItem(PAR_PROB_FX_MIN_RANGE) );
    connect( MRangeFXValueWidget, 1, AParameters->getItem(PAR_PROB_FX_MAX_RANGE) );

    // loop fx

    MIP_TextWidget* loop_fx_text = new MIP_TextWidget(MIP_DRect(414,345,66,9),"FX");
    page_prob->appendChildWidget(loop_fx_text);
    loop_fx_text->setDrawBorder(false);
    loop_fx_text->setFillBackground(false);
    loop_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    loop_fx_text->setTextColor(0.6);
    loop_fx_text->setTextSize(10);
    //loop_fx_text->setDisabled(true);
    //loop_fx_text->setDisabledColor(BACKGROUNDCOLOR);
    //loop_fx_text->setActive(false);

    MLoopFXWidget = new MIP_KnobWidget(MIP_DRect(377,345,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopFXWidget);
    MLoopFXWidget->setArcThickness(7);
    MLoopFXWidget->setValueColor(0.8);
    MLoopFXWidget->setTextColor(0.6);
    MLoopFXWidget->setArcBackColor(            MIP_Color(0.0, 0.5, 0.0) );
    MLoopFXWidget->setArcValueColor(           MIP_Color(0.0, 0.7, 0.0) );
    MLoopFXWidget->setInteractiveArcValueColor(MIP_Color(0.0, 0.9, 0.0) );
    //MLoopFXWidget->setDisabled(true);
    //MLoopFXWidget->setDisabledColor(BACKGROUNDCOLOR);
    //MLoopFXWidget->setActive(false);

    connect( MLoopFXWidget, AParameters->getItem(PAR_PROB_FX_PROB_LOOP) );

    MLoopFXValueWidget = new MIP_DualSliderWidget( MIP_DRect(414,356,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopFXValueWidget);
    MLoopFXValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MLoopFXValueWidget->setBackgroundColor(       MIP_Color(0.0, 0.5, 0.0) );
    MLoopFXValueWidget->setSliderColor(           MIP_Color(0.0, 0.7, 0.0) );
    MLoopFXValueWidget->setInteractiveSliderColor(MIP_Color(0.0, 0.9, 0.0) );
    MLoopFXValueWidget->setTextColor(             MIP_Color(0, 0, 0, 0.5));
    //MLoopFXValueWidget->setDisabled(true);
    //MLoopFXValueWidget->setDisabledColor(BACKGROUNDCOLOR);
    //MLoopFXValueWidget->setActive(false);

    connect( MLoopFXValueWidget, 0, AParameters->getItem(PAR_PROB_FX_MIN_LOOP) );
    connect( MLoopFXValueWidget, 1, AParameters->getItem(PAR_PROB_FX_MAX_LOOP) );

