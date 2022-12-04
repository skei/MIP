#ifndef mip_xcb_painter_included
#define mip_xcb_painter_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "gui/base/mip_base_painter.h"
#include "gui/xcb/mip_xcb.h"
#include "gui/xcb/mip_xcb_utils.h"
#include "gui/mip_paint_target.h"
#include "gui/mip_bitmap.h"

//#ifdef MIP_XCB_USE_STB_TRUETYPE
//  #define STB_TRUETYPE_IMPLEMENTATION
//  #include "extern/stb/stb_truetype.h"
//  #include "../data/fonts/liberation_ttf.h"
//#endif

//----------

#define MIP_XCB_PAINTER_MAX_IMAGES  64
#define MIP_XCB_PAINTER_MAX_FONTS   16

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_XcbPainter
: public MIP_BasePainter {

//------------------------------
private:
//------------------------------

  MIP_PaintTarget*  MPaintTarget    = nullptr;

  xcb_connection_t* MConnection     = nullptr;
  xcb_visualid_t    MVisual         = XCB_NONE;
  xcb_drawable_t    MDrawable       = XCB_NONE;
  xcb_gcontext_t    MGC             = XCB_NONE;
  xcb_font_t        MFont           = XCB_NONE;

//  MIP_Bitmap*       MImages[MIP_XCB_PAINTER_MAX_IMAGES] = {0};
//
//  #ifdef MIP_XCB_USE_STB_TRUETYPE
//  stbtt_fontinfo*   MFonts[MIP_XCB_PAINTER_MAX_FONTS]   = {0};
//  #endif

  uint32_t          MWidth          = 0;
  uint32_t          MHeight         = 0;

  MIP_Color         MDrawColor      = 1.0;
  MIP_Color         MFillColor      = 0.5;
  MIP_Color         MTextColor      = 0.0;

  double            MLineWidth      = 1.0;
  double            MTextSize       = 12.0;
  const char*       MTextFontName   = "Arial";

  int32_t           MFontAscent     = 0;
  int32_t           MFontDescent    = 0;
  int32_t           MFontWidth      = 0;
  int32_t           MFontHeight     = 0;
  int32_t           MFontOrigin     = 0;
  int32_t           MFontLeft       = 0;
  int32_t           MFontRight      = 0;

//  int               MShapeAntiAlias;
//  float             MMiterLimit;
//  int               MLineCap;
//  int               MLineJoin;
//  float             MGlobalAlpha;

//  MIP_Color         MStrokeColor                = MIP_Color(0);//MIP_COLOR_BLACK;
//  MIP_PaintSource   MStrokePaint                = {};
//  float             MStrokeWidth                = 1.0;

//  MIP_Color         MFillColor                  = MIP_Color(1);//MIP_COLOR_WHITE;
//  MIP_PaintSource   MFillPaint                  = {};

//  int               MFont                       = -1;
//  MIP_Color         MFontColor                  = MIP_Color(0);//MIP_COLOR_BLACK;
//  float             MFontSize                   = 13.0;

//------------------------------
public:
//------------------------------

  // ASurface = surface to paint to
  // ATarget  = target window/visual (window)

  MIP_XcbPainter(MIP_PaintTarget* ATarget)
  : MIP_BasePainter(ATarget) {
    MPaintTarget = ATarget;
    if (ATarget->tgtIsDrawable()) {
    //if (ATarget->tgtIsWindow()) {
      MConnection = ATarget->tgtGetConnection();
      MVisual     = ATarget->tgtGetVisual();
      MDrawable   = ATarget->tgtGetDrawable();
      MWidth      = ATarget->tgtGetWidth();
      MHeight     = ATarget->tgtGetHeight();
      MGC         = xcb_generate_id(MConnection);
      uint32_t mask =
        //XCB_GC_FUNCTION
        //XCB_GC_PLANE_MASK
        //XCB_GC_FOREGROUND
        //XCB_GC_BACKGROUND
        //XCB_GC_LINE_WIDTH
        //XCB_GC_LINE_STYLE
        //XCB_GC_CAP_STYLE
        //XCB_GC_JOIN_STYLE
        //XCB_GC_FILL_STYLE
        //XCB_GC_FILL_RULE
        //XCB_GC_TILE
        //XCB_GC_STIPPLE
        //XCB_GC_TILE_STIPPLE_ORIGIN_X
        //XCB_GC_TILE_STIPPLE_ORIGIN_Y
        //XCB_GC_FONT |
        //XCB_GC_SUBWINDOW_MODE
        XCB_GC_GRAPHICS_EXPOSURES;
        //XCB_GC_CLIP_ORIGIN_X
        //XCB_GC_CLIP_ORIGIN_Y
        //XCB_GC_CLIP_MASK
        //XCB_GC_DASH_OFFSET
        //XCB_GC_DASH_LIST
        //XCB_GC_ARC_MODE
      uint32_t values[] = {
        0
      };
      xcb_create_gc(MConnection,MGC,MDrawable,mask,values);
      xcb_flush(MConnection);
    }

    #ifdef MIP_XCB_USE_STB_TRUETYPE
//      int font = createFontMem("font1",(unsigned char*)liberation_ttf,liberation_ttf_size,0);
//      fontFaceId(font);
    #endif

  }

  //----------

  virtual ~MIP_XcbPainter() {
    xcb_free_gc(MConnection,MGC);

//    for (uint32_t i=0; i<MIP_XCB_PAINTER_MAX_IMAGES; i++) {
//      if (MImages[i]) delete MImages[i];
//    }

    #ifdef MIP_XCB_USE_STB_TRUETYPE
//      for (uint32_t i=0; i<MIP_XCB_PAINTER_MAX_FONTS; i++) {
//        if (MFonts[i]) free(MFonts[i]);
//      }
    #endif

  }

//------------------------------
public:
//------------------------------

  void flush(void) override {
  }

//------------------------------
public: // clipping
//------------------------------


  /*
    The XSetClipRectangles() function changes the clip-mask in the specified GC
    to the specified list of rectangles and sets the clip origin. The output is
    clipped to remain contained within the rectangles. The clip-origin is
    interpreted relative to the origin of whatever destination drawable is
    specified in a graphics request. The rectangle coordinates are interpreted
    relative to the clip-origin. The rectangles should be nonintersecting, or
    the graphics results will be undefined. Note that the list of rectangles
    can be empty, which effectively disables output. This is the opposite of
    passing None as the clip-mask in XCreateGC(), XChangeGC(), and
    XSetClipMask().
  */

  void setClip(MIP_DRect ARect) override {
    //MIP_Print("ARect: %.0f,%.0f,%.0f,%.0f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    //resetClip();
    xcb_rectangle_t rectangles[] = {{
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)(ARect.w + 1),
      (uint16_t)(ARect.h + 1),
    }};
    xcb_set_clip_rectangles(
      MConnection,
      XCB_CLIP_ORDERING_UNSORTED, // ordering,
      MGC,
      0, // clip_x_origin
      0, // clip_y_origin
      1, // rectangles_len
      rectangles
    );
    xcb_flush(MConnection);
  }

  //----------

  void resetClip() override {
    uint32_t mask = XCB_GC_CLIP_MASK;
    uint32_t values[1];
    values[0] = XCB_NONE;
    xcb_change_gc(MConnection, MGC, mask, values);
    xcb_flush(MConnection);
  }

//------------------------------
public: // set, get..
//------------------------------

  void setDrawColor(MIP_Color color) override {
    MDrawColor = color;
    _set_color(color);
  }

  //----------

  void setFillColor(MIP_Color color) override {
    MFillColor = color;
    _set_color(color);
  }

  //----------

  void setTextColor(MIP_Color color) override {
    MTextColor = color;
    _set_color(color);
  }

  //----------

  void setTextSize(double ASize) override {
    MTextSize = ASize;
    //loadFont(MTextFontName,ASize);
  }

  //----------

  void setLineWidth(double AWidth) override {
    MLineWidth = AWidth;
    //_set_line_width(AWidth);
    uint32_t mask = XCB_GC_LINE_WIDTH;
    uint32_t values[1];
    values[0] = AWidth;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

//------------------------------
public: // draw
//------------------------------

  void drawLine(double x1, double y1, double x2, double y2) override {
    xcb_point_t polyline[] =  {
      (int16_t)x1,
      (int16_t)y1,
      (int16_t)x2,
      (int16_t)y2
    };
    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,2,polyline);
  }

  //----------

  void drawLines(uint32_t num, double* coords) override {
  }

  //----------

  void drawLineStrip(uint32_t num, double* coords) override {
  }

  //----------

  // a1,a2 : start/end angle..
  // 0..PI2, starts art 3 o'clock, counterclockwise

  void drawArc(double cx, double cy, double r, double a1, double a2) override {
    //a1 = MIP_PI2 - a1;
    //a2 = - a2;
    float x  = cx - r;  // MPath[i].data[0] - MPath[i].data[2];
    float y  = cy - r;  // MPath[i].data[1] - MPath[i].data[2];
    float w  = r * 2.0; // MPath[i].data[2] * 2.0;
    float h  = r * 2.0; // MPath[i].data[2] * 2.0;
    xcb_arc_t arcs[] = {
      (int16_t)x,
      (int16_t)y,
      (uint16_t)w,
      (uint16_t)h,
      (int16_t)(a1 * 360.0f * 64.0f * MIP_INVPI2),
      (int16_t)(a2 * 360.0f * 64.0f * MIP_INVPI2)
    };
    xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  //----------

  void drawRect(double x, double y, double w, double h) override {
    xcb_rectangle_t rectangles[] = {{
      (int16_t)x,
      (int16_t)y,
      (uint16_t)w,
      (uint16_t)h,
    }};
    xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }

  //----------

//  void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor, float AWidth=1) override {
//    xcb_point_t polyline[] =  {
//      (int16_t)AX1, (int16_t)AY1, (int16_t)AX2, (int16_t)AY2,
//      (int16_t)AX2, (int16_t)AY2, (int16_t)AX3, (int16_t)AY3,
//      (int16_t)AX3, (int16_t)AY3, (int16_t)AX1, (int16_t)AY1,
//    };
//    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,6,polyline);
//  }

//------------------------------
public: // fill
//------------------------------

  void fillRect(double x, double y, double w, double h) override {
    xcb_rectangle_t rectangles[] = {{
      (int16_t)x,
      (int16_t)y,
      (uint16_t)w,
      (uint16_t)h,
    }};
    xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }

  //----------

//  void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor) override {
//    set_color(AColor);
//    xcb_point_t polyline[] =  {
//      (int16_t)AX1, (int16_t)AY1, (int16_t)AX2, (int16_t)AY2,
//      (int16_t)AX2, (int16_t)AY2, (int16_t)AX3, (int16_t)AY3,
//      (int16_t)AX3, (int16_t)AY3, (int16_t)AX1, (int16_t)AY1,
//    };
//    xcb_fill_poly(MConnection,MDrawable,MGC,XCB_POLY_SHAPE_CONVEX,XCB_COORD_MODE_ORIGIN,6,polyline);
//  }

//------------------------------
public: // text
//------------------------------

  void drawText(double x, double y, const char* text) override {
    //MIP_PRINT;
//    _measure_string(string);
    //
    #ifdef MIP_XCB_USE_STB_TRUETYPE
      //unsigned char* textbuffer = _render_string(string);
      //if (textbuffer) {
      //  _draw_buffer((uint32_t*)textbuffer,x,y,MFontWidth,MFontHeight);
      //  free (textbuffer);
      //}
    #else
      uint8_t buffer[512];
      MIP_XcbPolyText8 pt;
      pt.data = buffer;
      pt.used = 0;
      mip_xcb_add_string_text8(&pt,text);
      xcb_poly_text_8(MConnection,MDrawable,MGC,x,y + MFontAscent,pt.used,pt.data);
    #endif
  }

//------------------------------
public: // bitmaps
//------------------------------

//  void drawBitmap(uint32_t* buffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
//    uint32_t buffersize = width * height * sizeof(uint32_t);
//    xcb_image_t* image = xcb_image_create(
//      width,                          // width      width in pixels.
//      height,                         // height     height in pixels.
//      XCB_IMAGE_FORMAT_Z_PIXMAP,      // format
//      32,                             // xpad       scanline pad (8,16,32)
//      24,                             // depth      (1,4,8,16,24 zpixmap),    (1 xybitmap), (anything xypixmap)
//      32,                             // bpp        (1,4,8,16,24,32 zpixmap,  (1 xybitmap), (anything xypixmap)
//      32,                             // unit       unit of image representation, in bits (8,16,32)
//      XCB_IMAGE_ORDER_LSB_FIRST,      // byte_order
//      XCB_IMAGE_ORDER_LSB_FIRST,      // bit_order
//      buffer,                         // base       The base address of malloced image data
//      buffersize,                     // bytes      The size in bytes of the storage pointed to by base.
//                                      //            If base == 0 and bytes == ~0 and data == 0, no storage will be auto-allocated.
//      (uint8_t*)buffer                // data       The image data. If data is null and bytes != ~0, then an attempt will be made
//                                      //            to fill in data; from base if it is non-null (and bytes is large enough), else
//                                      //            by mallocing sufficient storage and filling in base.
//    );
//    xcb_image_put(
//      MConnection,            // xcb_connection_t*  conn,
//      MDrawable,              // xcb_drawable_t     draw,
//      MGC,                    // xcb_gcontext_t     gc,
//      image,                  // xcb_image_t*       image,
//      x,                      // int16_t            x,
//      y,                      // int16_t            y,
//      0                       // uint8_t            left_pad
//    );
//    image->base = nullptr;
//    xcb_image_destroy(image);
//    xcb_flush(MConnection);
//  }

  //----------

//  void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource) override {
//    if (ASource->isImage()) {
//      xcb_image_put(
//        MConnection,            // xcb_connection_t*  conn,
//        MDrawable,              // xcb_drawable_t     draw,
//        MGC,                    // xcb_gcontext_t     gc,
//        ASource->getXcbImage(), // xcb_image_t*       image,
//        AXpos,                  // int16_t            x,
//        AYpos,                  // int16_t            y,
//        0                       // uint8_t            left_pad
//      );
//      xcb_flush(MConnection);
//    }
//    else if (ASource->isSurface()) {
//      //#ifdef MIP_USE_CAIRO
//      //cairo_surface_flush(MCairoSurface);
//      //#endif
//      xcb_copy_area(
//        MConnection,                // Pointer to the xcb_connection_t structure
//        ASource->getXcbDrawable(),  // The Drawable we want to paste
//        MDrawable,                  // The Drawable on which we copy the previous Drawable
//        MGC,                        // A Graphic Context
//        0,                          // Top left x coordinate of the region we want to copy
//        0,                          // Top left y coordinate of the region we want to copy
//        AXpos,                      // Top left x coordinate of the region where we want to copy
//        AYpos,                      // Top left y coordinate of the region where we want to copy
//        ASource->getWidth(),        // Width                 of the region we want to copy
//        ASource->getHeight()        // Height of the region we want to copy
//      );
//      xcb_flush(MConnection);
//      //#ifdef MIP_USE_CAIRO
//      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
//      //#endif
//    }
//    //else {
//    //  MIP_Print("unknown ADrawable for blit()\n");
//    //}
//  }

  //----------

  void blit(double ADstX, double ADstY, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
    if (ASource->srcIsBitmap()) {
      uint32_t* pixelptr  = ASource->srcGetBuffer() + (ASource->srcGetStride() * (int)ASrcY) + (int)ASrcX;
      mip_xcb_put_image(
        MConnection,
        MDrawable,
        MGC,
        ASrcW,
        ASrcH,
        ADstX,
        ADstY,
        MPaintTarget->tgtGetDepth(), //ASource->srcGetDepth(),
        ASource->srcGetStride(),
        pixelptr

      );
      xcb_flush(MConnection);
      //#ifdef MIP_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    else if (ASource->srcIsSurface()) {
      //#ifdef MIP_USE_CAIRO
      //cairo_surface_flush(MCairoSurface);
      //#endif
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->srcGetDrawable(),  // The Drawable we want to paste
        MDrawable,                    // The Drawable on which we copy the previous Drawable
        MGC,                  // A Graphic Context
        ASrcX,                      // Top left x coordinate of the region we want to copy
        ASrcY,                      // Top left y coordinate of the region we want to copy
        ADstX,                      // Top left x coordinate of the region where we want to copy
        ADstY,                      // Top left y coordinate of the region where we want to copy
        ASrcW,                      // Width                 of the region we want to copy
        ASrcW                       // Height of the region we want to copy
      );
      xcb_flush(MConnection);
      //#ifdef MIP_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    //else {
    //  MIP_Print("unknown ADrawable for blit()\n");
    //}
  }

  //----------

  void stretch(double ADstX, double ADstY, double ADstW, double ADstH, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
  }

  //----------

  void blend(double ADstX, double ADstY, double ADstW, double ADstH, MIP_PaintSource* ASource, double ASrcX, double ASrcY, double ASrcW, double ASrcH) override {
  }

//------------------------------
private:
//------------------------------

  float _get_text_width(const char* AText) {
    _measure_string(AText);
    return MFontWidth;
  }

  //----------

  float _get_text_height(const char* AText) {
    _measure_string(AText);
    return MFontHeight;
  }

  //----------

  //  #ifdef MIP_USE_CAIRO
  //
  //  cairo_surface_t* createCairoSurface() {
  //    cairo_surface_t* surface = cairo_xcb_surface_create(
  //      MConnection,
  //      MDrawable,
  //      mip_xcb_find_visual(MConnection,MVisual),
  //      MWidth,
  //      MHeight
  //    );
  //    return surface;
  //  }
  //
  //  #endif

  //----------

  void _set_color(MIP_Color AColor) {
    uint8_t r = AColor.r * 255.0f;
    uint8_t g = AColor.g * 255.0f;
    uint8_t b = AColor.b * 255.0f;
    uint8_t a = AColor.a * 255.0f;
    uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = color;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void _set_background_color(MIP_Color AColor) {
    uint8_t r = AColor.r * 255.0f;
    uint8_t g = AColor.g * 255.0f;
    uint8_t b = AColor.b * 255.0f;
    uint8_t a = AColor.a * 255.0f;
    uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
    uint32_t mask = XCB_GC_BACKGROUND;
    uint32_t values[1];
    values[0] = color;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  const char* _find_font(const char* AName, uint32_t ASize) {
    return "";
  }

  //----------

  //  char* font_name = "-misc-dejavu sans-medium-o-normal--0-0-0-0-p-0-ascii-0";

  void _open_font(const char* AName) {
    _close_font();
    MFont = xcb_generate_id(MConnection);
    xcb_open_font(
      MConnection,
      MFont, //font,
      strlen(AName),
      AName
    );
  }

  //----------

  void _close_font(void) {
    if (MFont) {
      xcb_close_font(MConnection,MFont);
    }
  }

  //----------

  void _select_font(const char* AName) {
    _open_font(AName);
    uint32_t mask = XCB_GC_FONT;
    uint32_t values[1];
    values[0] = MFont;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  //cscale is from stbtt_ScaleForPixelHeight.

  #ifdef MIP_XCB_USE_STB_TRUETYPE

  float _calc_text_width(const char* text) {
    stbtt_fontinfo* font = MFonts[MFont];
    float length = 0;
    for (uint32_t i=0; i<strlen(text); i++) {
      int advancewidth;
      int leftsidebearing;
      stbtt_GetCodepointHMetrics(font,text[i], &advancewidth, &leftsidebearing);
      length += advancewidth;
    }
    return length * MFontSize;//cscale;
  }

  #endif

  //----------

  void _measure_string(const char *string) {
    #ifdef MIP_XCB_USE_STB_TRUETYPE
      stbtt_fontinfo* font = MFonts[MFont];
      float scale = stbtt_ScaleForPixelHeight(font,MFontSize);
      int ascent, descent, lineGap;
      stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
      //ascent = roundf(ascent * scale);
      //descent = roundf(descent * scale);
      MFontAscent   = ascent * scale;
      MFontDescent  = descent * scale;
      MFontWidth    = _calc_text_width(string) * scale;
      MFontHeight   = MFontSize;//(y1 - y0 + 1);
      MFontOrigin   = 0;
      MFontLeft     = 0;
      MFontRight    = 0;
      MIP_Print("ascent %i descent %i\n",MFontAscent,MFontDescent);
      MIP_Print("width %i height %i\n",MFontWidth,MFontHeight);
    #else
      xcb_char2b_t xcb_str[256];
      for (uint32_t i = 0; i < strlen(string); i++) {
        xcb_str[i].byte1 = 0;
        xcb_str[i].byte2 = string[i];
      }
      xcb_query_text_extents_cookie_t cookie = xcb_query_text_extents(MConnection, MGC, strlen(string), xcb_str);
      xcb_query_text_extents_reply_t* reply = xcb_query_text_extents_reply(MConnection, cookie, NULL);
      MFontAscent   = reply->font_ascent;
      MFontDescent  = reply->font_descent;
      MFontWidth    = reply->overall_width;
      MFontHeight   = reply->font_ascent + reply->font_descent;
      MFontOrigin   = reply->font_ascent;
      MFontLeft     = reply->overall_left;
      MFontRight    = reply->overall_right;
      //MFontOverallAscent = reply->overall_ascent;
      //MFontOverallDescent = reply->overall_descent;
      //free(xcb_str); // ???
      free(reply);
    #endif
  }

  // called must free returned memory block

  #ifdef MIP_XCB_USE_STB_TRUETYPE

  unsigned char* _render_string(const char* string, unsigned char* bitmap=nullptr) {
    uint32_t string_length = strlen(string);
    if (string_length = 0) return nullptr;
    _measure_string(string);
    stbtt_fontinfo* font = MFonts[MFont];
    if (!bitmap) bitmap = (unsigned char*)calloc(MFontWidth * MFontHeight, sizeof(unsigned char));
    float scale = stbtt_ScaleForPixelHeight(font,MFontSize);
    int x = 0;
    for (uint32_t i = 0; i < string_length; ++i) {
      // how wide is this character
      int ax, lsb;
      stbtt_GetCodepointHMetrics(font, string[i], &ax, &lsb);
      // (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].)
      // get bounding box for character (may be offset to account for chars that dip above or below the line)
      int c_x1, c_y1, c_x2, c_y2;
      stbtt_GetCodepointBitmapBox(font, string[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
      //* compute y (different characters have different heights)
      int y = MFontAscent + c_y1;
      //* render character (stride and offset is important here)
      int byteOffset = x + roundf(lsb * scale) + (y * MFontWidth);
      stbtt_MakeCodepointBitmap(font, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, MFontWidth, scale, scale, string[i]);
      //* advance x
      x += roundf(ax * scale);
      //* add kerning
      int kern;
      kern = stbtt_GetCodepointKernAdvance(font, string[i], string[i + 1]);
      x += roundf(kern * scale);
    }
    //* save out a 1 channel image
    //stbi_write_png("out.png", b_w, b_h, 1, bitmap, b_w);
    //free(fontBuffer);
    //free(bitmap);
    return bitmap;
  }

  #endif

  //----------


};

//----------------------------------------------------------------------
#endif

















#if 0

//--------------------
  // Text
  //--------------------

  int createFont(const char* name, const char* filename) override {
    MIP_PRINT;
    // load font
    long size;
    unsigned char* fontBuffer;
    FILE* fontFile = fopen(filename, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */
    fontBuffer = (unsigned char*)malloc(size);
    long num_read = fread(fontBuffer, size, 1, fontFile);
    if (num_read != size) {
      MIP_Print("error reading font\n");
    }
    fclose(fontFile);
    int font = createFontMem("font1",fontBuffer,size,0);
//    free(fontBuffer);
    return font;
  }

  //----------

  int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) override {
    MIP_PRINT;
    #ifdef MIP_XCB_USE_STB_TRUETYPE
    for (int i=0; i<MIP_XCB_PAINTER_MAX_FONTS; i++) {
      if (MFonts[i] == nullptr) {
        MFonts[i] = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo));
        if (!stbtt_InitFont(MFonts[i],data,0)) {
          printf("Error loading font\n");
        }
        return i;
      }
    }
    #endif
    return -1;
  }

};

#endif // 0
