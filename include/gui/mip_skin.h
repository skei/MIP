#ifndef mip_skin_included
#define mip_skin_included
//----------------------------------------------------------------------

/*
  widget state:
  - normal
  - disabled
  - highlighted
  - focused
  - interactive
*/

class MIP_Skin {
public:
  MIP_Color getBackgroundColor(uint32_t AState, uint32_t AMode)
  MIP_Color getBorderColor(uint32_t AState, uint32_t AMode)
  MIP_Color getTextColor(uint32_t AState, uint32_t AMode)
  MIP_Color getValueColor(uint32_t AState, uint32_t AMode)
};

//----------------------------------------------------------------------
#endif
