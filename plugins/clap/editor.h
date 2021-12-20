#ifndef editor_incuded
#define editor_incuded
//----------------------------------------------------------------------

class Editor {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  Editor() {
  }

  virtual ~Editor() {
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

  uint32_t get_width() {
    return 0;
  }

  uint32_t get_height() {
    return 0;
  }



};

//----------------------------------------------------------------------
#endif
