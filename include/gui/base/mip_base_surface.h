#ifndef mip_base_surface_included
#define mip_base_surface_included
//----------------------------------------------------------------------

class MIP_BaseSurface {

public:

  MIP_BaseSurface(MIP_PaintTarget* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0) {}
  virtual ~MIP_BaseSurface() {}

};

//----------------------------------------------------------------------
#endif
