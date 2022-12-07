#ifndef mip_exe_window_included
#define mip_exe_window_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/exe/mip_exe.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------

class MIP_ExeWindow
: public MIP_ImplementedWindow {
//: public MIP_Window {

//------------------------------
private:
//------------------------------

  const clap_plugin_t*      plugin  = nullptr;
  const clap_plugin_gui_t*  gui     = nullptr;

//------------------------------
public:
//------------------------------

  MIP_ExeWindow(uint32_t AWidth, uint32_t AHeight, const clap_plugin_t* APlugin, const clap_plugin_gui_t* AGui)
  : MIP_ImplementedWindow(AWidth,AHeight) {
  //: MIP_Window(AWidth,AHeight,false) {
    //MWindowType = "MIP_ExeWindow";
    plugin = APlugin;
    gui = AGui;
    setTitle("MIP_ExeWindow");
    //setWindowBackgroundColor(0.5);
    //setWindowFillBackground(true);
  }

  //----------

  virtual ~MIP_ExeWindow() {
  }

//------------------------------
public:
//------------------------------

  void on_window_resize(int32_t AWidth, int32_t AHeight) override {

    // scaling

    //double xscale = (double)AWidth / MInitialWidth;
    //double yscale = (double)AHeight / MInitialHeight;
    //MIP_Print("xscale %f yscale %f\n",xscale,yscale);
    //MIP_Window::on_window_resize(AWidth,AHeight);

    // resize child windows

    #ifdef MIP_USE_XCB
      //xcb_connection_t* connection = drawable_getXcbConnection();
      //xcb_window_t window = drawable_getXcbWindow();
      //
      //xcb_query_tree_cookie_t cookie;
      //xcb_query_tree_reply_t *reply;
      //cookie = xcb_query_tree(connection,window);
      //if ((reply = xcb_query_tree_reply(connection, cookie, NULL))) {
      //  //printf("root = 0x%08x\n", reply->root);
      //  //printf("parent = 0x%08x\n", reply->parent);
      //  xcb_window_t *children = xcb_query_tree_children(reply);
      //  for (int i = 0; i < xcb_query_tree_children_length(reply); i++) {
      //    //printf("child window = 0x%08x\n", children[i]);
      //    xcb_window_t child = children[i];
      //    int32_t values[] = { AWidth, AHeight };
      //    xcb_configure_window(connection,child,XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,values);
      //  }
      //  //xcb_aux_sync(connection);
      //  xcb_flush(connection);
      //  free(reply);
      //}
    #endif

    /*
      the stuff below should be improved!!!
      what if we are buffering in win32?
    */

    //MIP_Plugin* plug = nullptr;

    //#ifdef MIP_WIN32
    //#ifdef MIP_GUI_NANOVG
    #ifdef MIP_WINDOW_BUFFERED
      MIP_Plugin* plug = (MIP_Plugin*)plugin->plugin_data;
      if (plug) {
        if (plug->MEditor) {
          MIP_Window* window = plug->MEditor->getWindow();
          if (window) {
            window->on_window_resize(AWidth,AHeight);
          }
        }
      }
    #endif

    // let our plugin know..
    gui->set_size(plugin,AWidth,AHeight);

    #ifdef MIP_USE_WIN32
      MIP_Plugin* plug = (MIP_Plugin*)plugin->plugin_data;
      if (plug) {
       if (plug->MEditor) {
          MIP_Window* window = plug->MEditor->getWindow();
          if (window) {
            window->invalidateRegion(0,0,AWidth,AHeight);
            //HWND hwnd = window->getWinHandle();
            //UpdateWindow(hwnd);
            //RedrawWindow
          }
        }
      }
    #endif

  }

};

//----------------------------------------------------------------------
#endif
