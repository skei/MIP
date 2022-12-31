#ifndef mip_nanovg_painter_included
#define mip_nanovg_painter_included
//----------------------------------------------------------------------

#include "base/mip.h"

#ifdef MIP_USE_GLX
  #include "gui/glx/mip_glx_painter.h"
#endif

#ifdef MIP_USE_WGL
  #include "gui/wgl/mip_wgl_painter.h"
#endif

#include "gui/nvg/mip_nvg.h"
#include "gui/nvg/mip_nvg_utils.h"

#include "../data/fonts/Roboto/Roboto-Regular.h"
#include "../data/fonts/Manjari/Manjari-Thin.h"


//#include "../data/fonts/Quicksand_ttf.h"

//#include "extern/blendish/blendish.h"
//#include "extern/blendish/blendish.c"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_NvgPainter
#ifdef MIP_USE_GLX
: public MIP_GlxPainter {
#endif
//#ifdef MIP_WIN32
#ifdef MIP_USE_WGL
: public MIP_WglPainter {
#endif

//------------------------------
private:
//------------------------------

  NVGcontext* MContext        = nullptr;
  int         MDefaultFont    = -1;
  int         MHeaderFont     = -1;

//  NVGpaint    MImagePaint     = {0};
//  NVGpaint    MGradientPaint  = {0};

  //int   MFont_Header  = 0;
  //int   MIconImage    = 0;

  float MTextBounds[4] = {0};

protected:

  MIP_Color   MDrawColor  = 1.0;
  MIP_Color   MFillColor  = 0.5;
  MIP_Color   MTextColor  = 0.0;
  double      MTextSize   = 10.0;
  double      MLineWidth  = 1.0;

//------------------------------
public:
//------------------------------

  MIP_NvgPainter(MIP_PaintTarget* ATarget)

  //#ifdef MIP_LINUX
  #ifdef MIP_USE_GLX
  : MIP_GlxPainter(ATarget) {
    MIP_GlxPainter::makeCurrent();
  #endif

  //#ifdef MIP_WIN32
  #ifdef MIP_USE_WGL
  : MIP_WglPainter(ATarget) {
    MIP_WglPainter::makeCurrent();
  #endif

    // create context

    #if MIP_OPENGL_VERSION_MAJOR >= 3
      MContext = nvgCreateGL3(NVG_ANTIALIAS);// | NVG_STENCIL_STROKES); // NVG_DEBUG
    #else
      MContext = nvgCreateGL2(NVG_ANTIALIAS);// | NVG_STENCIL_STROKES); // NVG_DEBUG
    #endif

    // load fint

    MDefaultFont = nvgCreateFontMem(MContext,"Roboto-Regular",(unsigned char*)Roboto_Regular,Roboto_Regular_size,0);
    MHeaderFont = nvgCreateFontMem(MContext,"Manjari-Thin",(unsigned char*)Manjari_Thin,Manjari_Thin_size,0);

    nvgFontFaceId(MContext,MDefaultFont);
    nvgFontSize(MContext,MTextSize);

    //#ifdef BLENDISH_H
    //  bndSetFont(MFont);
    //  MIconImage  = nvgCreateImage(MContext,"/DISKS/sda2/code/git/MIP2/include/extern/oui-blendish/blender_icons16.png",0); // NVG_IMAGE_PREMULTIPLIED
    //  bndSetIconImage(MIconImage);
    //#endif

    #ifdef MIP_USE_GLX
      MIP_GlxPainter::resetCurrent();
    #endif

    #ifdef MIP_USE_WGL
      MIP_WglPainter::resetCurrent();
    #endif

  }

  //----------

  /*
    will nvgDelete also delete images and fonts?
  */

  virtual ~MIP_NvgPainter() {
    #if MIP_OPENGL_VERSION_MAJOR >= 3
      nvgDeleteGL3(MContext);
    #else
      nvgDeleteGL2(MContext);
    #endif
    //nvgDeleteImage(MIconImage);
  }

//------------------------------
public:
//------------------------------

  NVGcontext* getNvgContext() {
    //MIP_PRINT;
    return MContext;
  }

  //----------

  int getDefaultFont() {
    return MDefaultFont;
  }

  int getHeaderFont() {
    return MHeaderFont;
  }

  //----------

  //int getFont_Header(int index=0) {
  //  return MFont_Header;
  //}

  //----------

  //int getNvgIconImage(int index=0) {
  //  return MIconImage;
  //}

//------------------------------
public:
//------------------------------

  void flush(void) override {
    //nvgFlush(MContext);
  }

  //----------

  // test/hack..
  // extend clipping rectanlge, so we 'catch' antialiased edges..
  //
  // is pixel coordinate 0,0 center, or upper left of pixel?

  void setClip(MIP_DRect ARect) override {
    //MIP_Print("%.2f,%.2f - %.2f,%.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    //nvgScissor(MContext,ARect.x - 0.5,ARect.y - 0.5,ARect.w + 1,ARect.h + 1);
    nvgScissor(MContext,ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  void resetClip() override {
    //MIP_Print("\n");
    nvgResetScissor(MContext);
  }

//------------------------------
public:
//------------------------------

  void setDrawColor(MIP_Color AColor) override {
    MDrawColor = AColor;
    nvgStrokeColor(MContext,nvg_color(AColor));
  }

  //----------

  void setFillColor(MIP_Color AColor) override {
    MFillColor = AColor;
    nvgFillColor(MContext,nvg_color(AColor));
  }

  //----------

  void setTextColor(MIP_Color AColor) override {
    MTextColor = AColor;
    //nvgStrokeColor(MContext,nvg_color(AColor));
    nvgFillColor(MContext,nvg_color(AColor));
  }

  //----------

  bool getTextBounds(const char* AText, double* ABounds) override {
    nvgTextBounds(MContext,0,0,AText,nullptr,MTextBounds);
    ABounds[0] = MTextBounds[0];
    ABounds[1] = MTextBounds[1];
    ABounds[2] = MTextBounds[2];
    ABounds[3] = MTextBounds[3];
    return true;
  }

  //----------

  void setTextSize(double ASize) override {
    MTextSize = ASize;
    nvgFontSize(MContext,ASize);
  }

  //----------

  void setLineWidth(double AWidth) override {
    MLineWidth = AWidth;
    nvgStrokeWidth(MContext,AWidth);
  }

  //----------

  void setGlobalAlpha(double AAlpha) override {
    nvgGlobalAlpha(MContext,AAlpha);
  }

//------------------------------
public:
//------------------------------

  void drawLine(double x1, double y1, double x2, double y2) override {
    nvgBeginPath(MContext);
    nvgMoveTo(MContext,x1,y1);
    nvgLineTo(MContext,x2,y2);
    nvgStroke(MContext);
  }

  //----------

  void drawLines(uint32_t num, double* coords) override {
    if (num >= 2) {
      nvgBeginPath(MContext);
      for (uint32_t i=0; i<num; i++) {
        double x = *coords++;
        double y = *coords++;
        nvgMoveTo(MContext,x,y);
        x = *coords++;
        y = *coords++;
        nvgLineTo(MContext,x,y);
      }
      nvgStroke(MContext);
    }
  }

  //----------

  void drawLineStrip(uint32_t num, double* coords) override {
    if (num >= 2) {
      nvgBeginPath(MContext);
      double x = *coords++;
      double y = *coords++;
      nvgMoveTo(MContext,x,y);
      for (uint32_t i=1; i<num; i++) {
        x = *coords++;
        y = *coords++;
        nvgLineTo(MContext,x,y);
      }
      nvgStroke(MContext);
    }
  }

  //----------

  void drawArc(double cx, double cy, double r, double a1, double a2) override {
    nvgBeginPath(MContext);
    //a2 += a1;
    a2 = a1 - a2;
    nvgArc(MContext,cx,cy,r,a1,a2,1); // 0 = dir
    nvgStroke(MContext);
  }

  //----------

  void drawRect(double x, double y, double w, double h) override {
    nvgBeginPath(MContext);
    nvgRect(MContext,x,y,w,h);
    nvgStroke(MContext);
  }

  //----------

  void drawRoundedRect(double x, double y, double w, double h, double r) override {
    nvgBeginPath(MContext);
    nvgRoundedRect(MContext,x,y,w,h,r);
    nvgStroke(MContext);
  }

  //----------

  void drawRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) override {
    nvgBeginPath(MContext);
    nvgRoundedRectVarying(MContext,x,y,w,h,rtl,rtr,rbr,rbl);
    nvgStroke(MContext);
  }

  //----------

  void drawCurveQuad(double x1, double y1, double x2, double y2, double c1x, double c1y) override {
    nvgBeginPath(MContext);
    nvgMoveTo(MContext,x1,y1);
    nvgQuadTo(MContext,c1x,c1y,x2,y2);
    nvgStroke(MContext);
  }

  //drawCurveQuads

  //----------

  void drawCurveBezier(double x1, double y1, double x2, double y2, double c1x, double c1y, double c2x, double c2y) override {
    nvgBeginPath(MContext);
    nvgMoveTo(MContext,x1,y1);
    nvgBezierTo(MContext,c1x,c1y,c2x,c2y,x2,y2);
    nvgStroke(MContext);
  }

  //drawCurveBeziers

//------------------------------
public:
//------------------------------

  void fillRect(double x, double y, double w, double h) override {
    nvgBeginPath(MContext);
    nvgRect(MContext,x,y,w,h);
    nvgFill(MContext);
  }

  //----------

  void fillLines(uint32_t num, double* coords) override {
    if (num >= 2) {
      nvgBeginPath(MContext);
      for (uint32_t i=1; i<num; i++) {
        double x = *coords++;
        double y = *coords++;
        nvgMoveTo(MContext,x,y);
        x = *coords++;
        y = *coords++;
        nvgLineTo(MContext,x,y);
      }
      nvgStroke(MContext);
      nvgFill(MContext);
    }
  }

  //----------

  void fillLineStrip(uint32_t num, double* coords) override {
    if (num >= 2) {
      nvgBeginPath(MContext);
      double x = *coords++;
      double y = *coords++;
      nvgMoveTo(MContext,x,y);
      for (uint32_t i=1; i<num; i++) {
        x = *coords++;
        y = *coords++;
        nvgLineTo(MContext,x,y);
      }
      nvgFill(MContext);
    }
  }

//------------------------------
public:
//------------------------------

  void drawText(double x, double y, const char* text) override {
    nvgText(MContext,x,y,text,nullptr);
  }

//------------------------------
public:
//------------------------------

  void blit(double ADstX, double ADstY, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
    if (ASource->srcIsNanovg()) {
      int image = ASource->srcGetNvgImage();
      setFillImage(image,ASrcX,ASrcY,1,1,1,0);
      fillRect(ADstX,ADstY,ASrcW,ASrcH);
    }
  }

  //----------

  void stretch(double ADstX, double ADstY, double ADstW, double ADstH, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
  }

  //----------

  void blend(double ADstX, double ADstY, double ADstW, double ADstH, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
  }

//------------------------------
public:
//------------------------------

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {

    #ifdef MIP_USE_GLX
      MIP_GlxPainter::beginPaint(AXpos,AYpos,AWidth,AHeight);
    #endif
    #ifdef MIP_USE_WGL
      MIP_WglPainter::beginPaint(AXpos,AYpos,AWidth,AHeight);
    #endif
    nvgBeginFrame(MContext,AWidth,AHeight,1.0);
  }

  //----------

  void endPaint() override {
    nvgEndFrame(MContext);
    #ifdef MIP_USE_GLX
      MIP_GlxPainter::endPaint();
    #endif
    #ifdef MIP_USE_WGL
      MIP_WglPainter::endPaint();
    #endif
  }

//----------
//----------
//----------

  void setFillGradient(double AX1, double AY1, double AX2, double AY2, MIP_Color AColor1, MIP_Color AColor2) override {
    NVGcolor icol = nvg_color(AColor1);
    NVGcolor ocol = nvg_color(AColor2);
    NVGpaint paint = nvgLinearGradient(MContext,AX1,AY1,AX2,AY2,icol,ocol);
    nvgFillPaint(MContext,paint);
  }

//----------
//----------
//----------

  int32_t loadFont(const char* AName, const char* AFilename) override {
    int font = nvgCreateFont(MContext,AName,AFilename);
    return font;
  }

  //----------

  int32_t loadFont(const char* AName, void* ABuffer, uint32_t ASize) override {
    int font = nvgCreateFontMem(MContext,AName,(unsigned char*)ABuffer,ASize,0);
    return font;
  }

  //----------

  void deleteFont(int32_t AFont) override {
  }

  //----------

  void selectFont(int32_t AFont) override {
    nvgFontFaceId(MContext,AFont);
  }

//----------
//----------
//----------

  int32_t loadImage(const char* AFilename) override {
    int flags = 0;
    //flags |= NVG_IMAGE_GENERATE_MIPMAPS;  // Generate mipmaps during creation of the image.
    //flags |= NVG_IMAGE_REPEATX;           // Repeat image in X direction.
    //flags |= NVG_IMAGE_REPEATY;           // Repeat image in Y direction.
    //flags |= NVG_IMAGE_FLIPY;             // Flips (inverses) image in Y direction when rendered.
    //flags |= NVG_IMAGE_PREMULTIPLIED;     // Image data has premultiplied alpha.
    //flags |= NVG_IMAGE_NEAREST;           // Image interpolation is Nearest instead Linear
    int image = nvgCreateImage(MContext,AFilename,flags);
    MIP_Assert(image);
//    int w,h;
//    nvgImageSize(MContext,image,&w,&h);
//    MIP_Print("image %i size %i,%i\n",image,w,h);
    return image;
  }

  //----------

  int32_t loadImage(void* ABuffer, uint32_t ASize) override {
    int flags = 0;
    int image = nvgCreateImageMem(MContext,flags,(unsigned char*)ABuffer,ASize);
    return image;
  }

  //----------

  int32_t loadImage(int32_t AWidth, int32_t AHeight, void* ABuffer) override {
    int flags = 0;
    //flags |= NVG_IMAGE_GENERATE_MIPMAPS;  // Generate mipmaps during creation of the image.
    flags |= NVG_IMAGE_REPEATX;           // Repeat image in X direction.
    flags |= NVG_IMAGE_REPEATY;           // Repeat image in Y direction.
    //flags |= NVG_IMAGE_FLIPY;             // Flips (inverses) image in Y direction when rendered.
    flags |= NVG_IMAGE_PREMULTIPLIED;     // Image data has premultiplied alpha.
    //flags |= NVG_IMAGE_NEAREST;           // Image interpolation is Nearest instead Linear
    int image = nvgCreateImageRGBA(MContext,AWidth,AHeight,flags,(unsigned char*)ABuffer);
    return image;
  }

  //----------

  void deleteImage(int32_t AImage) override {
    nvgDeleteImage(MContext,AImage);
  }

  //----------

  /*
    nvgImagePattern
    Creates and returns an image pattern. Parameters (ox,oy) specify the
    left-top location of the image pattern, (ex,ey) the size of one image,
    angle rotation around the top-left corner, image is handle to the image to
    render.

    ----------

    https://github.com/memononen/nanovg/issues/348

    The image pattern is described as "rectangle" in the same world space as
    the rectangle you're rendering. Imagine this:

    Image
    o.......................
    :                      :
    :      Rectangle       :
    :      x------+        :
    :      |      |        :
    :      |      |        :
    :      +------+        :
    :                      :
    .......................:

    So the drawn rectangle is a "view" to the texture.
    This should work pretty close to what html canvas drawImage does (untested)
    http://www.w3schools.com/TAgs/canvas_drawimage.asp

    float drawImage(NVGcontext* vg, int image, float alpha,
                    float sx, float sy, float sw, float sh, // sprite location on texture
                    float x, float y, float w, float h) {   // position and size of the sprite rectangle on screen
      float ax, ay;
      int iw,ih;
      NVGpaint img;
      nvgImageSize(vg, image, &iw, &ih);
      // Aspect ration of pixel in x an y dimensions. This allows us to scale
      // the sprite to fill the whole rectangle.
      ax = w / sw;
      ay = h / sh;
      img = nvgImagePattern(vg, x - sx*ax, y - sy*ay, (float)iw*ax, (float)ih*ay, 0, image, alpha);
      nvgBeginPath(vg);
      nvgRect(vg, x,y, w,h);
      nvgFillPaint(vg, img);
      nvgFill(vg);
    }


  */

  void setFillImage(int32_t AImage, double xofs, double yofs, double xscale, double yscale, double AAlpha=1.0, double AAngle=0.0) override {
    int iw,ih;
    nvgImageSize(MContext,AImage,&iw,&ih);
    // sprite location on texture
    float sx = 0;     // 0;
    float sy = 0;     // 0;
//    float sw = iw;    // 256;
//    float sh = ih;    // 256;
    // position and size of the sprite rectangle on screen
    float dx = xofs;  // 0;
    float dy = yofs;  // 0;
//    float dw = iw;    // 256;
//    float dh = ih;    // 256;
    // Aspect ration of pixel in x an y dimensions. This allows us to scale
    // the sprite to fill the whole rectangle.
    float ax = xscale;//dw / sw;
    float ay = yscale;//dh / sh;

    NVGpaint paint = nvgImagePattern(
      MContext,
//      xofs,//dx - (sx * ax),
//      yofs,//dy - (sy * ay),
//      (float)iw * xscale,//(float)iw * ax,
//      (float)ih * yscale,//(float)ih * ay,
      dx - (sx * ax),
      dy - (sy * ay),
      (float)iw * ax,
      (float)ih * ay,
      AAngle,
      AImage,
      AAlpha
    );

    //nvgBeginPath(MContext);
    //nvgRect(MContext, dx,dy, dw,dh);
    nvgFillPaint(MContext,paint);
    //nvgFill(MContext);
  }

  //----------

  // Updates image data specified by image handle.
  //void nvgUpdateImage(NVGcontext* ctx, int image, const unsigned char* data);

//----------
//----------

  void* createRenderBuffer(uint32_t AWidth, uint32_t AHeight) override {
    int flags = 0;
    NVGLUframebuffer* fb = nvgluCreateFramebuffer(MContext,AWidth,AHeight,flags);
    return fb;
  }

  //----------

  void deleteRenderBuffer(void* buffer) override {
    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    nvgluDeleteFramebuffer(fb);
  }

  //----------

  void selectRenderBuffer(void* buffer) override {
    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    nvgluBindFramebuffer(fb);
  }

  //----------

  int32_t getImageFromRenderBuffer(void* buffer) override {
    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    return fb->image;
  }

};

//----------------------------------------------------------------------
#endif
















#if 0

  //--------------------
  // color
  //--------------------


//  NVGcolor RGB(unsigned char r, unsigned char g, unsigned char b) {
//    return nvgRGB(r, g, b);
//  }
//
//  NVGcolor RGBf(float r, float g, float b) {
//    return nvgRGBf(r, g, b);
//  }
//
//  NVGcolor RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
//    return nvgRGBA(r, g, b, a);
//  }
//
//  NVGcolor RGBAf(float r, float g, float b, float a) {
//    return nvgRGBAf(r, g, b, a);
//  }
//
//  NVGcolor LerpRGBA(NVGcolor c0, NVGcolor c1, float u) {
//    return nvgLerpRGBA(c0,c1,u);
//  }
//
//  NVGcolor TransRGBA(NVGcolor c0, unsigned char a) {
//    return nvgTransRGBA(c0,a);
//  }
//
//  NVGcolor TransRGBAf(NVGcolor c0, float a) {
//    return nvgTransRGBAf(c0,a);
//  }
//
//  NVGcolor HSL(float h, float s, float l) {
//    return nvgHSL(h, s, l);
//  }
//
//  NVGcolor HSLA(float h, float s, float l, unsigned char a) {
//    return nvgHSLA(h, s, l, a);
//  }

  //--------------------
  // State Handling
  //--------------------

  void save() override {
    nvgSave(MContext);
  }

  void restore() override {
    nvgRestore(MContext);
  }

  void reset() override {
    nvgReset(MContext);
  }

  //--------------------
  // Render styles
  //--------------------

  void shapeAntiAlias(int enabled) override {
    nvgShapeAntiAlias(MContext,enabled);
  }

  //void strokePaint(NVGpaint paint) {
  void strokePaint(MIP_PaintSource paint) override {
    //nvgStrokePaint(MContext,paint.nvg);
    nvgStrokePaint(MContext,paint.nvg);
  }

  void fillPaint(MIP_PaintSource paint) override {
    //nvgFillPaint(MContext,paint.nvg);
    nvgFillPaint(MContext,paint.nvg);
  }

  void miterLimit(float limit) override {
    nvgMiterLimit(MContext,limit);
  }

  void lineCap(int cap) override {
    nvgLineCap(MContext,cap);
  }

  void lineJoin(int join) override {
    nvgLineJoin(MContext,join);
  }

  void globalAlpha(float alpha) override {
    nvgGlobalAlpha(MContext,alpha);
  }

  //--------------------
  // Transforms
  //--------------------

  void resetTransform() override {
    nvgResetTransform(MContext);
  }

  void transform(float a, float b, float c, float d, float e, float f) override {
    nvgTransform(MContext,a,b,c,d,e,f);
  }

  void translate(float x, float y) override {
    nvgTranslate(MContext,x,y);
  }

  void rotate(float angle) override {
    nvgRotate(MContext,angle);
  }

  void skewX(float angle) override {
    nvgSkewX(MContext,angle);
  }

  void skewY(float angle) override {
    nvgSkewY(MContext,angle);
  }

  void scale(float x, float y) override {
    nvgScale(MContext,x,y);
  }

  void currentTransform(float* xform) override {
    nvgCurrentTransform(MContext,xform);
  }

  void transformIdentity(float* dst) override {
    nvgTransformIdentity(dst);
  }

  void transformTranslate(float* dst, float tx, float ty) override {
    nvgTransformTranslate(dst,tx,ty);
  }

  void transformScale(float* dst, float sx, float sy) override {
    nvgTransformScale(dst,sx,sy);
  }

  void transformRotate(float* dst, float a) override {
    nvgTransformRotate(dst,a);
  }

  void transformSkewX(float* dst, float a) override {
    nvgTransformSkewX(dst,a);
  }

  void transformSkewY(float* dst, float a) override {
    nvgTransformSkewY(dst,a);
  }

  void transformMultiply(float* dst, const float* src) override {
    nvgTransformMultiply(dst,src);
  }

  void transformPremultiply(float* dst, const float* src) override {
    nvgTransformPremultiply(dst,src);
  }

  int transformInverse(float* dst, const float* src) override {
    return nvgTransformInverse(dst,src);
  }

  void transformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy) override {
    nvgTransformPoint(dstx,dsty,xform,srcx,srcy);
  }

  float degToRad(float deg) override {
    return nvgDegToRad(deg);
  }

  float radToDeg(float rad) override {
    return nvgRadToDeg(rad);
  }

  //--------------------
  // Images
  //--------------------

  int createImage(const char* filename, int imageFlags) override {
    return nvgCreateImage(MContext,filename,imageFlags);
  }

  int createImageMem(int imageFlags, unsigned char* data, int ndata) override {
    return nvgCreateImageMem(MContext,imageFlags,data,ndata);
  }

  int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) override {
    return nvgCreateImageRGBA(MContext,w,h,imageFlags,data);
  }

  void updateImage(int image, const unsigned char* data) override {
    nvgUpdateImage(MContext,image,data);
  }

  void imageSize(int image, int* w, int* h) override {
    nvgImageSize(MContext,image,w,h);
  }

  void deleteImage(int image) override {
    nvgDeleteImage(MContext,image);
  }

  //--------------------
  // Paints
  //--------------------

  //NVGpaint linearGradient(float sx, float sy, float ex, float ey, NVGcolor icol, NVGcolor ocol) override {
  MIP_PaintSource linearGradient(float sx, float sy, float ex, float ey, MIP_Color icol, MIP_Color ocol) override {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    MIP_PaintSource paint;
    //paint.nvg = nvgLinearGradient(MContext,sx,sy,ex,ey,ic,oc);
    paint.nvg = nvgLinearGradient(MContext,sx,sy,ex,ey,ic,oc);
    return paint;
  }

  //NVGpaint boxGradient(float x, float y, float w, float h, float r, float f, NVGcolor icol, NVGcolor ocol) override {
  MIP_PaintSource boxGradient(float x, float y, float w, float h, float r, float f, MIP_Color icol, MIP_Color ocol) override {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    MIP_PaintSource paint;
    //paint.nvg = nvgBoxGradient(MContext,x,y,w,h,r,f,ic,oc);
    paint.nvg = nvgBoxGradient(MContext,x,y,w,h,r,f,ic,oc);
    return paint;
  }

  //NVGpaint radialGradient(float cx, float cy, float inr, float outr, NVGcolor icol, NVGcolor ocol) override {
  MIP_PaintSource radialGradient(float cx, float cy, float inr, float outr, MIP_Color icol, MIP_Color ocol) override {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    MIP_PaintSource paint;
    //paint.nvg = nvgRadialGradient(MContext,cx,cy,inr,outr,ic,oc);
    paint.nvg = nvgRadialGradient(MContext,cx,cy,inr,outr,ic,oc);
    return paint;
  }

  MIP_PaintSource imagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha) override {
    MIP_PaintSource paint;
    //paint.nvg = nvgImagePattern(MContext,ox,oy,ex,ey,angle,image,alpha);
    paint.nvg = nvgImagePattern(MContext,ox,oy,ex,ey,angle,image,alpha);
    return paint;
  }

  //--------------------
  // Scissoring
  //--------------------

  void scissor(float x, float y, float w, float h) override {
    nvgScissor(MContext,x,y,w,h);
  }

  void intersectScissor(float x, float y, float w, float h) override {
    nvgIntersectScissor(MContext,x,y,w,h);
  }

  void resetScissor() override {
    nvgResetScissor(MContext);
  }

  //--------------------
  // Paths
  //--------------------

  void beginPath() override {
    nvgBeginPath(MContext);
  }

  void moveTo(float x, float y) override {
    nvgMoveTo(MContext,x,y);
  }

  void lineTo(float x, float y) override {
    nvgLineTo(MContext,x,y);
  }

  void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override {
    nvgBezierTo(MContext,c1x,c1y,c2x,c2y,x,y);
  }

  void quadTo(float cx, float cy, float x, float y) override {
    nvgQuadTo(MContext,cx,cy,x,y);
  }

  void arcTo(float x1, float y1, float x2, float y2, float radius) override {
    nvgArcTo(MContext,x1,y1,x2,y2,radius);
  }

  void closePath() override {
    nvgClosePath(MContext);
  }

  void pathWinding(int dir) override {
    nvgPathWinding(MContext,dir);
  }

  void arc(float cx, float cy, float r, float a0, float a1, int dir) override {
    a1 += a0;
    nvgArc(MContext,cx,cy,r,a1,a0,dir);
  }

  void rect(float x, float y, float w, float h) override {
    nvgRect(MContext,x,y,w,h);
  }

  void roundedRect(float x, float y, float w, float h, float r) override {
    nvgRoundedRect(MContext,x,y,w,h,r);
  }

  void roundedRectVarying(float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft) override {
    nvgRoundedRectVarying(MContext,x,y,w,h,radTopLeft,radTopRight,radBottomRight,radBottomLeft);
  }

  void ellipse(float cx, float cy, float rx, float ry) override {
    nvgEllipse(MContext,cx,cy,rx,ry);
  }

  void circle(float cx, float cy, float r) override {
    nvgCircle(MContext,cx,cy,r);
  }

  void fill() override {
    nvgFill(MContext);
  }

  void stroke() override {
    nvgStroke(MContext);
  }

  //--------------------
  // Text
  //--------------------

  int createFont(const char* name, const char* filename) override {
    return nvgCreateFont(MContext,name,filename);
  }

  int createFontAtIndex(const char* name, const char* filename, const int fontIndex) override {
    return nvgCreateFontAtIndex(MContext,name,filename,fontIndex);
  }

  int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) override {
    return nvgCreateFontMem(MContext,name,data,ndata,freeData);
  }

  int createFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) override {
    return nvgCreateFontMemAtIndex(MContext,name,data,ndata,freeData,fontIndex);
  }

  int findFont(const char* name) override {
    return nvgFindFont(MContext,name);
  }

  int addFallbackFontId(int baseFont, int fallbackFont) override {
    return nvgAddFallbackFontId(MContext,baseFont,fallbackFont);
  }

  int addFallbackFont(const char* baseFont, const char* fallbackFont) override {
    return nvgAddFallbackFont(MContext,baseFont,fallbackFont);
  }

  void resetFallbackFontsId(int baseFont) override {
    nvgResetFallbackFontsId(MContext,baseFont);
  }

  void resetFallbackFonts(const char* baseFont) override {
    nvgResetFallbackFonts(MContext,baseFont);
  }

  void fontBlur(float blur) override {
    nvgFontBlur(MContext,blur);
  }

  void textLetterSpacing(float spacing) override {
    nvgTextLetterSpacing(MContext,spacing);
  }

  void textLineHeight(float lineHeight) override {
    nvgTextLineHeight(MContext,lineHeight);
  }

  void textAlign(int align) override {
    nvgTextAlign(MContext,align);
  }

  void fontFaceId(int font) override {
    nvgFontFaceId(MContext,font);
  }

  void fontFace(const char* font) override {
    nvgFontFace(MContext,font);
  }

  float text(float x, float y, const char* string, const char* end) override {
    return nvgText(MContext,x,y,string,end);
  }

  void textBox(float x, float y, float breakRowWidth, const char* string, const char* end) override {
    nvgTextBox(MContext,x,y,breakRowWidth,string,end);
  }

  float textBounds(float x, float y, const char* string, const char* end, float* bounds) override {
    return nvgTextBounds(MContext,x,y,string,end,bounds);
  }

  void textBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) override {
    nvgTextBoxBounds(MContext,x,y,breakRowWidth,string,end,bounds);
  }

  int textGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) override {
    return nvgTextGlyphPositions(MContext,x,y,string,end,(NVGglyphPosition*)positions,maxPositions);
  }

  void textMetrics(float* ascender, float* descender, float* lineh) override {
    nvgTextMetrics(MContext,ascender,descender,lineh);
  }

  int textBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) override {
    return nvgTextBreakLines(MContext,string,end,breakRowWidth,(NVGtextRow*)rows,maxRows);
  }

};

#endif // 0

