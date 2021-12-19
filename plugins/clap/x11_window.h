#ifndef x11_window_incuded
#define x11_window_incuded
//----------------------------------------------------------------------

class X11_Window {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  X11_Window() {
  }

  virtual ~X11_Window() {
  }

//------------------------------
public:
//------------------------------

  bool attach(const char* display_name, unsigned long window) {
    return true;
  }

  void show() {
  }

  void hide() {
  }

   void set_width(uint32_t w) {
  }

  void set_height(uint32_t h) {
  }

  void set_scale(double scale) {
  }

//------------------------------
public:
//------------------------------

  uint32_t get_width() {
    return 0;
  }

  uint32_t get_height() {
    return 0;
  }

};

//----------------------------------------------------------------------
#endif
