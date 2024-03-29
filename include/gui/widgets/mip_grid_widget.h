#ifndef mip_grid_widget_included
#define mip_grid_widget_included
//----------------------------------------------------------------------

/*
  todo:
  - selected x1,y1,x2,y2
    invert if necesary
    MIP_List<cells>
  - scroll?
  - cut, copy, paste
  - move/drag
*/

//----------------------------------------------------------------------

#include "base/mip.h"
//#include "base/utils/mip_math.h"
#include "gui/mip_widget.h"
#include "gui/mip_paint_context.h"
#include "gui/widgets/mip_panel_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_GridWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  bool      MIsDragging           = false;
  bool      MIsMoving             = false;
  int32_t   MClickedX             = -1;
  int32_t   MClickedY             = -1;
  int32_t   MSelectedX            = -1;
  int32_t   MSelectedY            = -1;
  int32_t   MSelectedXcount       = 0;
  int32_t   MSelectedYcount       = 0;

//------------------------------
protected:
//------------------------------

  int32_t   MNumColumns           = 0;
  int32_t   MNumRows              = 0;          // rows

  bool      MSelectCell           = true;
  bool      MSelectMultipleCells  = true;

  bool      MDrawCells            = true;
  bool      MDrawHorizontalLines  = true;
  bool      MDrawVerticalLines    = true;
  bool      MDrawSelectedCells    = true;

  MIP_Color MGridColor            = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
  double    MGridLineWidth        = 1.0;

  //bool      MFillBackground       = false;
  //MIP_Color MBackgroundColor      = MIP_COLOR_GRAY;

//------------------------------
public:
//------------------------------

  MIP_GridWidget(MIP_DRect ARect, uint32_t AColumns, uint32_t ARows)
  : MIP_PanelWidget(ARect) {
    setCursor(MIP_CURSOR_FINGER);
    setNumCells(AColumns,ARows);
    //setDrawBorder(true);
  }

  //----------

  virtual ~MIP_GridWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual int32_t getNumColumns()                           { return MNumColumns; }
  virtual int32_t getnumRows()                              { return MNumRows; }

  virtual void    setNumColumns(int32_t AWidth)             { MNumColumns = AWidth; }
  virtual void    setNumRows(int32_t AHeight)               { MNumRows = AHeight; }
  virtual void    setNumCells(int32_t AX, int32_t AY)       { MNumColumns = AX; MNumRows = AY; }

  virtual void    setSelectCell(bool ASelect=true)          { MSelectCell = ASelect; }
  virtual void    setSelectMultipleCells(bool ASelect=true) { MSelectMultipleCells = ASelect; }

  virtual void    setDrawCells(bool ADraw=true)             { MDrawCells = ADraw; }
  virtual void    setDrawLines(bool H=true, bool V=true)    { MDrawHorizontalLines = H; MDrawVerticalLines = V; }
  virtual void    setDrawHorizontalLines(bool ADraw=true)   { MDrawHorizontalLines = ADraw; }
  virtual void    setDrawVerticalLines(bool ADraw=true)     { MDrawVerticalLines = ADraw; }
  virtual void    setDrawSelectedCells(bool ADraw=true)     { MDrawSelectedCells = ADraw; }
  virtual void    setBackgroundColor(MIP_Color AColor)     { MBackgroundColor = AColor; }
  virtual void    setGridColor(MIP_Color AColor)           { MGridColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void selectCell(int32_t AX, int32_t AY, bool AState=true) {
    if (AState) {
      MSelectedX = AX;
      MSelectedY = AY;
      MSelectedXcount = 1;
      MSelectedYcount = 1;
    }
    else {
      MSelectedX = -1;
      MSelectedY = -1;
      MSelectedXcount = 0;
      MSelectedYcount = 0;
    }
  }

  //MSelectCell = AState;
  //MSelectMultipleCells = false;

//------------------------------
public:
//------------------------------

  virtual void on_initCell(MIP_DRect ARect, int32_t AX, int32_t AY) {}
  virtual void on_clickCell(int32_t AX, int32_t AY, int32_t AB) {}
  virtual void on_paintCell(MIP_PaintContext* AContext, MIP_DRect ARect, int32_t AX, int32_t AY) {}

//------------------------------
public:
//------------------------------

  virtual void initCells(void) {
    MIP_DRect mrect = getRect();
    if ((MNumColumns > 0) && (MNumRows > 0)) {
      float xcell = ( mrect.w / (float)MNumColumns );
      float ycell = ( mrect.h / (float)MNumRows );
      for (int32_t yy=0; yy<MNumRows; yy++) {
        for (int32_t xx=0; xx<MNumColumns; xx++) {
          MIP_DRect R = MIP_DRect(
            mrect.x + /*floorf*/(xx * xcell),
            mrect.y + /*floorf*/(yy * ycell),
                      /*floorf*/(     xcell),
                      /*floorf*/(     ycell)
          );
          on_initCell(R,xx,yy);
        }
      }
    }
  }

  //----------

  /*
  virtual void redrawCell(int32_t x, int32_t y) {
    float xcell = ( (float)MRect.w / (float)MNumColumns );
    float ycell = ( (float)MRect.h / (float)MNumRows );
    MSelectedX = x;
    MSelectedY = y;
    //MIP_Trace("%.3f %.3f %.3f %.3f\n",x*xcell,y*ycell,xcell,ycell);
    do_redraw(this,MIP_Rect(x*xcell,y*ycell,xcell,ycell));
  }
  */

//------------------------------
public: // parent to child
//------------------------------

//  void on_widget_move(double AXpos, double AYpos)  override {}
//  void on_widget_resize(double AWidth, double AHeight) override {}
//  void on_widget_align(bool ARecursive=true) override {}
//  void on_widget_paint(MIP_PaintContext* AContext) override { paintChildWidgets(AContext); }
//  void on_widget_key_press(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_key_release(uint32_t AKey, uint32_t AState, uint32_t ATime) override {}
//  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_enter(MIP_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {}
//  void on_widget_leave(MIP_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {}
//  //void on_widget_connect(MIP_Parameter* AParameter) override {}

//------------------------------
public:
//------------------------------

  /*
    TODO
    - only draw visible cells..
    - cells intersecting with update-rect
  */

  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_DRect mrect = getRect();
    MIP_Painter* painter = AContext->painter;
    MIP_Window* window = (MIP_Window*)getOwnerWindow();
    double S = window->getWindowScale();

    fillBackground(AContext);

    if ((MNumColumns > 0) && (MNumRows > 0)) {
      float xcell = ( mrect.w / (float)MNumColumns );
      float ycell = ( mrect.h / (float)MNumRows );

      // cells
      if (MDrawCells) {
        for (int32_t xx=0; xx<MNumColumns; xx++) {
          for (int32_t yy=0; yy<MNumRows; yy++) {
            MIP_DRect R = MIP_DRect(
              mrect.x + /*floorf*/(xx * xcell),
              mrect.y + /*floorf*/(yy * ycell),
              /*floorf*/(xcell),
              /*floorf*/(ycell)
            );
            on_paintCell(AContext,R,xx,yy);
          }
        }
      }

      // grid-lines
      float x = mrect.x + xcell - 1;
      float y = mrect.y + ycell - 1;

      if (MDrawVerticalLines) {
        if (MNumColumns > 1) {
          painter->setLineWidth(MGridLineWidth * S);
          painter->setDrawColor(MGridColor);
          for (int32_t i=0; i<MNumColumns-1; i++) {
            painter->drawLine( floorf(x), mrect.y, floorf(x), mrect.y2() );
            x += xcell;
          }
        }
      }

      if (MDrawHorizontalLines) {
        if (MNumRows > 1) {
          painter->setLineWidth(MGridLineWidth * S);
          painter->setDrawColor(MGridColor);
          for (int32_t i=0; i<MNumRows-1; i++) {
            painter->drawLine( mrect.x, floorf(y), mrect.x2(), floorf(y) );
            y += ycell;
          }
        }
      }

      // selected cell(s)
      if (MDrawSelectedCells) {
        if ((MSelectedX >= 0) && (MSelectedY >= 0)) {
          x = xcell * MSelectedX;
          y = ycell * MSelectedY;
          float xn = xcell * MSelectedXcount;
          float yn = ycell * MSelectedYcount;
          float ww = xn - 1;
          float hh = yn - 1;
          x  += mrect.x;
          y  += mrect.y;
          painter->setDrawColor(MIP_COLOR_WHITE);
          painter->setLineWidth(MGridLineWidth * S);
          painter->drawRect( x,y,ww,hh );
        }
      }

    }

    // border
    drawBorder(AContext);

  }

  //----------

  /*
    todo:
    - has selection -> move
  */

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATimeStamp=0) override {
    MIP_DRect mrect = getRect();
    float xcell = ( mrect.w / (float)MNumColumns );
    float ycell = ( mrect.h / (float)MNumRows );
    int32_t x = /*floorf*/( (float)(AXpos - mrect.x) / xcell );
    int32_t y = /*floorf*/( (float)(AYpos - mrect.y) / ycell );
    MClickedX = x;
    MClickedY = y;
    if (AButton == MIP_BUTTON_LEFT) {
      if (MSelectCell) {
        MSelectedX = x;
        MSelectedY = y;
        MSelectedXcount = 1;
        MSelectedYcount = 1;
        if (MSelectMultipleCells) {
          MIsDragging = true;
        }
        on_clickCell(x,y,AButton);
        do_widget_redraw(this);
      }
    }
  }

  //----------

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATimeStamp=0) override {
    if (AButton == MIP_BUTTON_LEFT) {
      MIsDragging = false;
    }
  }

  //----------

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATimeStamp=0) override {
    MIP_DRect mrect = getRect();
    float xcell = ( mrect.w / (float)MNumColumns );
    float ycell = ( mrect.h / (float)MNumRows );
    float x = /*floorf*/( (float)(AXpos - mrect.x) / xcell );
    float y = /*floorf*/( (float)(AYpos - mrect.y) / ycell );
    x = MIP_Clamp(x,0,MNumColumns-1);
    y = MIP_Clamp(y,0,MNumRows-1);
    if (MIsDragging) {
      //if (MIsMoving) {
      //}
      //else {
        if (x < MClickedX) {
          MSelectedX = x;
          MSelectedXcount = MClickedX - x + 1;
        }
        else {
          MSelectedX = MClickedX;
          MSelectedXcount = x - MClickedX + 1;
        }
        if (y < MClickedY) {
          MSelectedY = y;
          MSelectedYcount = MClickedY - y + 1;
        }
        else {
          MSelectedY = MClickedY;
          MSelectedYcount = y - MClickedY + 1;
        }
      //}
      do_widget_redraw(this);
    }
  }

};

//----------------------------------------------------------------------
#endif

