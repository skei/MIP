
// inline

//----------------------------------------------------------------------

    // fx type

    sa_botage_MenuWidget* fx_type_menu = new sa_botage_MenuWidget(70,13,fx_type_text,SA_BOTAGE_FX_TYPE_COUNT);
    fx_type_menu->setDrawBorder(true);
    fx_type_menu->setBorderColor(0);
    fx_type_menu->setFillBackground(true);
    fx_type_menu->setBackgroundColor(0.5);

//----------------------------------------------------------------------

    MIP_Widget* page_prob = new MIP_Widget( MIP_DRect(255,90,357,292));

    //------------------------------
    //
    //------------------------------

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

    MMainSlicesWidget = new MIP_ButtonRowWidget( MIP_DRect(345,109,135,16), 8, prob_slices_text, MIP_BUTTON_ROW_MULTI );
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

    MMainLoopsWidget = new MIP_ButtonRowWidget( MIP_DRect(345,148,135,16), 8, prob_slices_text, MIP_BUTTON_ROW_MULTI );
    page_prob->appendChildWidget(MMainLoopsWidget);
    MMainLoopsWidget->selectButton(0);
    MMainLoopsWidget->setAllowZeroBits(false);
    MMainLoopsWidget->setValueIsBits(true,8);
    MMainLoopsWidget->setHint("Range subdivision");
    MMainLoopsWidget->setDrawBorder(false);
    MMainLoopsWidget->setBackgroundCellColor( MIP_Color(0.0, 0.3, 0.6) );
    MMainLoopsWidget->setActiveCellColor(     MIP_Color(0.0, 0.5, 1.0) );

    connect( MMainLoopsWidget,  AParameters->getItem(PAR_RANGE_LOOP_COUNT) );

    //------------------------------
    //
    //------------------------------

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

    // txt effects

    MIP_TextWidget* c3_header = new MIP_TextWidget(MIP_DRect(499,180,103,14),"Effects");
    page_prob->appendChildWidget(c3_header);
    c3_header->setDrawBorder(true);
    c3_header->setBorderColor(0.6);
    c3_header->setBorderEdges(MIP_EDGE_BOTTOM);
    c3_header->setFillBackground(false);
    c3_header->setBackgroundColor(0.35);
    c3_header->setTextColor(0.8);
    c3_header->setTextSize(11);

    //------------------------------
    //
    //------------------------------

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

    //------------------------------

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

    //------------------------------

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

    //------------------------------

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

    //------------------------------

    // range fx

    MIP_TextWidget* range_fx_text = new MIP_TextWidget(MIP_DRect(292,345,66,9),"FX");
    page_prob->appendChildWidget(range_fx_text);
    range_fx_text->setFillBackground(false);
    range_fx_text->setDrawBorder(false);
    range_fx_text->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    range_fx_text->setTextColor(0.6);
    range_fx_text->setTextSize(10);

    MRangeFXWidget = new MIP_KnobWidget(MIP_DRect(255,345,33,33),"%", 0.0);
    page_prob->appendChildWidget(MRangeFXWidget);
    MRangeFXWidget->setArcThickness(7);
    MRangeFXWidget->setValueColor(0.8);
    MRangeFXWidget->setTextColor(0.6);
    MRangeFXWidget->setArcBackColor(             MIP_Color(0.0, 0.5, 0.0) );
    MRangeFXWidget->setArcValueColor(            MIP_Color(0.0, 0.7, 0.0) );
    MRangeFXWidget->setInteractiveArcValueColor( MIP_Color(0.0, 0.9, 0.0) );

    connect( MRangeFXWidget, AParameters->getItem(PAR_PROB_FX_PROB_RANGE) );

    MRangeFXValueWidget = new MIP_DualSliderWidget( MIP_DRect(292,356,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MRangeFXValueWidget);
    MRangeFXValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MRangeFXValueWidget->setBackgroundColor(        MIP_Color(0.0, 0.5, 0.0) );
    MRangeFXValueWidget->setSliderColor(            MIP_Color(0.0, 0.7, 0.0) );
    MRangeFXValueWidget->setInteractiveSliderColor( MIP_Color(0.0, 0.9, 0.0) );
    MRangeFXValueWidget->setTextColor(              MIP_Color(0, 0, 0, 0.5));

    //MRangeFXValueWidget->setSnap(true);

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

    MLoopFXWidget = new MIP_KnobWidget(MIP_DRect(377,345,33,33),"%", 0.0);
    page_prob->appendChildWidget(MLoopFXWidget);
    MLoopFXWidget->setArcThickness(7);
    MLoopFXWidget->setValueColor(0.8);
    MLoopFXWidget->setTextColor(0.6);
    MLoopFXWidget->setArcBackColor(            MIP_Color(0.0, 0.5, 0.0) );
    MLoopFXWidget->setArcValueColor(           MIP_Color(0.0, 0.7, 0.0) );
    MLoopFXWidget->setInteractiveArcValueColor(MIP_Color(0.0, 0.9, 0.0) );

    connect( MLoopFXWidget, AParameters->getItem(PAR_PROB_FX_PROB_LOOP) );

    MLoopFXValueWidget = new MIP_DualSliderWidget( MIP_DRect(414,356,66,16), "%", 0.5, 0.5 );
    page_prob->appendChildWidget(MLoopFXValueWidget);
    MLoopFXValueWidget->setCursor(MIP_CURSOR_ARROW_LEFT_RIGHT);
    MLoopFXValueWidget->setBackgroundColor(       MIP_Color(0.0, 0.5, 0.0) );
    MLoopFXValueWidget->setSliderColor(           MIP_Color(0.0, 0.7, 0.0) );
    MLoopFXValueWidget->setInteractiveSliderColor(MIP_Color(0.0, 0.9, 0.0) );
    MLoopFXValueWidget->setTextColor(             MIP_Color(0, 0, 0, 0.5));

    //MLoopFXValueWidget->setSnap(true);

    connect( MLoopFXValueWidget, 0, AParameters->getItem(PAR_PROB_FX_MIN_LOOP) );
    connect( MLoopFXValueWidget, 1, AParameters->getItem(PAR_PROB_FX_MAX_LOOP) );

    //------------------------------
    //
    //------------------------------

    // FX 1

    MIP_KnobWidget* fx1_knob = new MIP_KnobWidget(MIP_DRect(499,205,33,33),"%", 0.0);
    page_prob->appendChildWidget(fx1_knob);
    fx1_knob->setArcThickness(7);
    fx1_knob->setValueColor(0.8);
    fx1_knob->setTextColor(0.6);
    fx1_knob->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx1_knob->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx1_knob->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx1_knob, AParameters->getItem(PAR_FX1_PROB) );

    MIP_SelectorWidget* fx1_type = new MIP_SelectorWidget(MIP_DRect(537,205,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(fx1_type);
    fx1_type->setDrawBorder(true);
    fx1_type->setBorderColor(0.4);
    fx1_type->setBorderEdges(MIP_EDGE_BOTTOM);
    fx1_type->setFillBackground(false);
    fx1_type->setBackgroundColor(0.5);
    fx1_type->setTextColor(0.8);
    fx1_type->setTextSize(9);
    fx1_type->setTextOffset(MIP_DRect(2,0,0,0));
    fx1_type->setDrawArrow(true);
    fx1_type->setArrowColor(0.5);

    connect( fx1_type, AParameters->getItem(PAR_FX1_TYPE) );

    // fx1 par 1

    MIP_KnobWidget* fx1_par1 = new MIP_KnobWidget(MIP_DRect(537,220,18,18),"", 0.0);
    page_prob->appendChildWidget(fx1_par1);
    fx1_par1->setDrawText(false);
    fx1_par1->setDrawValue(false);
    fx1_par1->setArcThickness(4);
    fx1_par1->setValueColor(0.8);
    fx1_par1->setTextColor(0.6);
    fx1_par1->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx1_par1->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx1_par1->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx1_par1, AParameters->getItem(PAR_FX1_PAR1) );

    // fx1 par 2

    MIP_KnobWidget* fx1_par2 = new MIP_KnobWidget(MIP_DRect(557,220,18,18),"", 0.0);
    page_prob->appendChildWidget(fx1_par2);
    fx1_par2->setDrawText(false);
    fx1_par2->setDrawValue(false);
    fx1_par2->setArcThickness(4);
    fx1_par2->setValueColor(0.8);
    fx1_par2->setTextColor(0.6);
    fx1_par2->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx1_par2->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx1_par2->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx1_par2, AParameters->getItem(PAR_FX1_PAR2) );

    // fx1 par 3

    MIP_KnobWidget* fx1_par3 = new MIP_KnobWidget(MIP_DRect(577,220,18,18),"", 0.0);
    page_prob->appendChildWidget(fx1_par3);
    fx1_par3->setDrawText(false);
    fx1_par3->setDrawValue(false);
    fx1_par3->setArcThickness(4);
    fx1_par3->setValueColor(0.8);
    fx1_par3->setTextColor(0.6);
    fx1_par3->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx1_par3->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx1_par3->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx1_par3, AParameters->getItem(PAR_FX1_PAR3) );

    // FX 2

    MIP_KnobWidget* fx2_knob = new MIP_KnobWidget(MIP_DRect(499,205+35,33,33),"%", 0.0);
    page_prob->appendChildWidget(fx2_knob);
    fx2_knob->setArcThickness(7);
    fx2_knob->setValueColor(0.8);
    fx2_knob->setTextColor(0.6);
    fx2_knob->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx2_knob->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx2_knob->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx2_knob, AParameters->getItem(PAR_FX2_PROB) );

    MIP_SelectorWidget* fx2_type = new MIP_SelectorWidget(MIP_DRect(537,205+35,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(fx2_type);
    fx2_type->setDrawBorder(true);
    fx2_type->setBorderColor(0.4);
    fx2_type->setBorderEdges(MIP_EDGE_BOTTOM);
    fx2_type->setFillBackground(false);
    fx2_type->setBackgroundColor(0.5);
    fx2_type->setTextColor(0.8);
    fx2_type->setTextSize(9);
    fx2_type->setTextOffset(MIP_DRect(2,0,0,0));
    fx2_type->setDrawArrow(true);
    fx2_type->setArrowColor(0.5);

    connect( fx2_type, AParameters->getItem(PAR_FX2_TYPE) );

    // fx2 par 1

    MIP_KnobWidget* fx2_par1 = new MIP_KnobWidget(MIP_DRect(537,220+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx2_par1);
    fx2_par1->setDrawText(false);
    fx2_par1->setDrawValue(false);
    fx2_par1->setArcThickness(4);
    fx2_par1->setValueColor(0.8);
    fx2_par1->setTextColor(0.6);
    fx2_par1->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx2_par1->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx2_par1->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx2_par1, AParameters->getItem(PAR_FX2_PAR1) );

    // fx2 par 2

    MIP_KnobWidget* fx2_par2 = new MIP_KnobWidget(MIP_DRect(557,220+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx2_par2);
    fx2_par2->setDrawText(false);
    fx2_par2->setDrawValue(false);
    fx2_par2->setArcThickness(4);
    fx2_par2->setValueColor(0.8);
    fx2_par2->setTextColor(0.6);
    fx2_par2->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx2_par2->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx2_par2->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx2_par2, AParameters->getItem(PAR_FX2_PAR2) );

    // fx2 par 3

    MIP_KnobWidget* fx2_par3 = new MIP_KnobWidget(MIP_DRect(577,220+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx2_par3);
    fx2_par3->setDrawText(false);
    fx2_par3->setDrawValue(false);
    fx2_par3->setArcThickness(4);
    fx2_par3->setValueColor(0.8);
    fx2_par3->setTextColor(0.6);
    fx2_par3->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx2_par3->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx2_par3->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx2_par3, AParameters->getItem(PAR_FX2_PAR3) );

    // FX 3

    MIP_KnobWidget* fx3_knob = new MIP_KnobWidget(MIP_DRect(499,205+35+35,33,33),"%", 0.0);
    page_prob->appendChildWidget(fx3_knob);
    fx3_knob->setArcThickness(7);
    fx3_knob->setValueColor(0.8);
    fx3_knob->setTextColor(0.6);
    fx3_knob->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx3_knob->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx3_knob->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx3_knob, AParameters->getItem(PAR_FX3_PROB) );

    MIP_SelectorWidget* fx3_type = new MIP_SelectorWidget(MIP_DRect(537,205+35+35,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(fx3_type);
    fx3_type->setDrawBorder(true);
    fx3_type->setBorderColor(0.4);
    fx3_type->setBorderEdges(MIP_EDGE_BOTTOM);
    fx3_type->setFillBackground(false);
    fx3_type->setBackgroundColor(0.5);
    fx3_type->setTextColor(0.8);
    fx3_type->setTextSize(9);
    fx3_type->setTextOffset(MIP_DRect(2,0,0,0));
    fx3_type->setDrawArrow(true);
    fx3_type->setArrowColor(0.5);

    connect( fx3_type, AParameters->getItem(PAR_FX3_TYPE) );

    // fx3 par 1

    MIP_KnobWidget* fx3_par1 = new MIP_KnobWidget(MIP_DRect(537,220+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx3_par1);
    fx3_par1->setDrawText(false);
    fx3_par1->setDrawValue(false);
    fx3_par1->setArcThickness(4);
    fx3_par1->setValueColor(0.8);
    fx3_par1->setTextColor(0.6);
    fx3_par1->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx3_par1->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx3_par1->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx3_par1, AParameters->getItem(PAR_FX3_PAR1) );

    // fx3 par 2

    MIP_KnobWidget* fx3_par2 = new MIP_KnobWidget(MIP_DRect(557,220+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx3_par2);
    fx3_par2->setDrawText(false);
    fx3_par2->setDrawValue(false);
    fx3_par2->setArcThickness(4);
    fx3_par2->setValueColor(0.8);
    fx3_par2->setTextColor(0.6);
    fx3_par2->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx3_par2->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx3_par2->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx3_par2, AParameters->getItem(PAR_FX3_PAR2) );

    // fx3 par 3

    MIP_KnobWidget* fx3_par3 = new MIP_KnobWidget(MIP_DRect(577,220+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx3_par3);
    fx3_par3->setDrawText(false);
    fx3_par3->setDrawValue(false);
    fx3_par3->setArcThickness(4);
    fx3_par3->setValueColor(0.8);
    fx3_par3->setTextColor(0.6);
    fx3_par3->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx3_par3->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx3_par3->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx3_par3, AParameters->getItem(PAR_FX3_PAR3) );

    // FX 4

    MIP_KnobWidget* fx4_knob = new MIP_KnobWidget(MIP_DRect(499,205+35+35+35,33,33),"%", 0.0);
    page_prob->appendChildWidget(fx4_knob);
    fx4_knob->setArcThickness(7);
    fx4_knob->setValueColor(0.8);
    fx4_knob->setTextColor(0.6);
    fx4_knob->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx4_knob->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx4_knob->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx4_knob, AParameters->getItem(PAR_FX4_PROB) );

    MIP_SelectorWidget* fx4_type = new MIP_SelectorWidget(MIP_DRect(537,205+35+35+35,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(fx4_type);
    fx4_type->setDrawBorder(true);
    fx4_type->setBorderColor(0.4);
    fx4_type->setBorderEdges(MIP_EDGE_BOTTOM);
    fx4_type->setFillBackground(false);
    fx4_type->setBackgroundColor(0.5);
    fx4_type->setTextColor(0.8);
    fx4_type->setTextSize(9);
    fx4_type->setTextOffset(MIP_DRect(2,0,0,0));
    fx4_type->setDrawArrow(true);
    fx4_type->setArrowColor(0.5);

    connect( fx4_type, AParameters->getItem(PAR_FX4_TYPE) );

    // fx4 par 1

    MIP_KnobWidget* fx4_par1 = new MIP_KnobWidget(MIP_DRect(537,220+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx4_par1);
    fx4_par1->setDrawText(false);
    fx4_par1->setDrawValue(false);
    fx4_par1->setArcThickness(4);
    fx4_par1->setValueColor(0.8);
    fx4_par1->setTextColor(0.6);
    fx4_par1->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx4_par1->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx4_par1->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx4_par1, AParameters->getItem(PAR_FX4_PAR1) );

    // fx4 par 2

    MIP_KnobWidget* fx4_par2 = new MIP_KnobWidget(MIP_DRect(557,220+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx4_par2);
    fx4_par2->setDrawText(false);
    fx4_par2->setDrawValue(false);
    fx4_par2->setArcThickness(4);
    fx4_par2->setValueColor(0.8);
    fx4_par2->setTextColor(0.6);
    fx4_par2->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx4_par2->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx4_par2->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx4_par2, AParameters->getItem(PAR_FX4_PAR2) );

    // fx4 par 3

    MIP_KnobWidget* fx4_par3 = new MIP_KnobWidget(MIP_DRect(577,220+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx4_par3);
    fx4_par3->setDrawText(false);
    fx4_par3->setDrawValue(false);
    fx4_par3->setArcThickness(4);
    fx4_par3->setValueColor(0.8);
    fx4_par3->setTextColor(0.6);
    fx4_par3->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx4_par3->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx4_par3->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx4_par3, AParameters->getItem(PAR_FX4_PAR3) );

    // FX 5

    MIP_KnobWidget* fx5_knob = new MIP_KnobWidget(MIP_DRect(499,205+35+35+35+35,33,33),"%", 0.0);
    page_prob->appendChildWidget(fx5_knob);
    fx5_knob->setArcThickness(7);
    fx5_knob->setValueColor(0.8);
    fx5_knob->setTextColor(0.6);
    fx5_knob->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx5_knob->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx5_knob->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx5_knob, AParameters->getItem(PAR_FX5_PROB) );

    MIP_SelectorWidget* fx5_type = new MIP_SelectorWidget(MIP_DRect(537,205+35+35+35+35,66,11), "Off", fx_type_menu);
    page_prob->appendChildWidget(fx5_type);
    fx5_type->setDrawBorder(true);
    fx5_type->setBorderColor(0.4);
    fx5_type->setBorderEdges(MIP_EDGE_BOTTOM);
    fx5_type->setFillBackground(false);
    fx5_type->setBackgroundColor(0.5);
    fx5_type->setTextColor(0.8);
    fx5_type->setTextSize(9);
    fx5_type->setTextOffset(MIP_DRect(2,0,0,0));
    fx5_type->setDrawArrow(true);
    fx5_type->setArrowColor(0.5);

    connect( fx5_type, AParameters->getItem(PAR_FX5_TYPE) );

    // fx4 par 1

    MIP_KnobWidget* fx5_par1 = new MIP_KnobWidget(MIP_DRect(537,220+35+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx5_par1);
    fx5_par1->setDrawText(false);
    fx5_par1->setDrawValue(false);
    fx5_par1->setArcThickness(4);
    fx5_par1->setValueColor(0.8);
    fx5_par1->setTextColor(0.6);
    fx5_par1->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx5_par1->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx5_par1->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx5_par1, AParameters->getItem(PAR_FX5_PAR1) );

    // fx4 par 2

    MIP_KnobWidget* fx5_par2 = new MIP_KnobWidget(MIP_DRect(557,220+35+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx5_par2);
    fx5_par2->setDrawText(false);
    fx5_par2->setDrawValue(false);
    fx5_par2->setArcThickness(4);
    fx5_par2->setValueColor(0.8);
    fx5_par2->setTextColor(0.6);
    fx5_par2->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx5_par2->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx5_par2->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx5_par2, AParameters->getItem(PAR_FX5_PAR2) );

    // fx4 par 3

    MIP_KnobWidget* fx5_par3 = new MIP_KnobWidget(MIP_DRect(577,220+35+35+35+35,18,18),"", 0.0);
    page_prob->appendChildWidget(fx5_par3);
    fx5_par3->setDrawText(false);
    fx5_par3->setDrawValue(false);
    fx5_par3->setArcThickness(4);
    fx5_par3->setValueColor(0.8);
    fx5_par3->setTextColor(0.6);
    fx5_par3->setArcBackColor(             MIP_Color(0.5, 0.5, 0.5) );
    fx5_par3->setArcValueColor(            MIP_Color(0.7, 0.7, 0.7) );
    fx5_par3->setInteractiveArcValueColor( MIP_Color(0.9, 0.9, 0.9) );

    connect( fx5_par3, AParameters->getItem(PAR_FX5_PAR3) );

//

