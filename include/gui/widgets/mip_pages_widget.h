#ifndef mip_pages_widget_included
#define mip_pages_widget_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/mip_widget.h"
#include "gui/mip_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_PagesWidget
: public MIP_Widget {

//------------------------------
protected:
//------------------------------

  uint32_t MActivePage = 0;

//------------------------------
public:
//------------------------------

  MIP_PagesWidget(MIP_DRect ARect)
  : MIP_Widget(ARect) {
  }

  //----------

  virtual ~MIP_PagesWidget() {
  }

//------------------------------
public:
//------------------------------

  int32_t appendPage(MIP_Widget* APage) {
    //APage->State.active = false;
    APage->setActive(false);
    APage->setVisible(false);
    return appendChildWidget(APage)->getIndex();
  }

  //----------

  void setPage(int32_t APage) {
    int num = getNumChildWidgets();//MChildren.size();
    if (APage >= num) APage = num - 1;
    for (int32_t i=0; i<num; i++) {
      MIP_Widget* wdg = getChildWidget(i);//MChildren[i];
      if (i == APage) {
        wdg->setActive(true);
        wdg->setVisible(true);
      }
      else {
        wdg->setActive(false);
        wdg->setVisible(false);
      }
    }
    MActivePage = APage;
  }

};

//----------------------------------------------------------------------
#endif


