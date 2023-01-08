#ifndef mip_exe_entry
#define mip_exe_entry
//----------------------------------------------------------------------

#include "base/mip.h"
#include "base/utils/mip_arguments.h"
#include "plugin/exe/mip_exe_window.h"

//----------------------------------------------------------------------

MIP_Arguments                   arguments       = {};
const clap_host_t*              claphost        = nullptr;
const clap_plugin_descriptor_t* clapdescriptor  = nullptr;
const clap_plugin_t*            clapplugin      = nullptr;
MIP_Plugin*                     plugin          = nullptr;

//----------------------------------------------------------------------

void mip_exe_print_help() {
  printf("arguments:\n");
  printf("  -?,-h   this help\n");
  printf("  -i n    plugin index (default: 0)\n");
  printf("  -ned    no editor (default: show editor)\n");
  printf("  -lpl    list plugins\n");
  printf("  -lpa    list parameters\n");
  printf("  -lpo    list ports\n");
}

//----------------------------------------------------------------------

bool mip_exe_load_plugin(uint32_t AIndex) {
  uint32_t num = MIP_REGISTRY.getNumDescriptors();
  if (AIndex >= num) {
    printf("error: plugin index (%i) out of bounds (0..%i)\n",AIndex,num-1);
    return false;
  }
  claphost = nullptr; //TODO
  clapdescriptor = MIP_REGISTRY.getDescriptor(AIndex);
  plugin = MIP_CreatePlugin(AIndex,clapdescriptor,claphost);
  clapplugin = plugin->getClapPlugin();
  plugin->init();
  return true;
}

//----------

void mip_exe_unload_plugin() {
  plugin->stop_processing();
  plugin->deactivate();
  plugin->destroy();
  delete plugin;
}

//----------------------------------------------------------------------

void mip_exe_list_plugins() {
  uint32_t num = MIP_REGISTRY.getNumDescriptors();
  for (uint32_t i=0; i<num; i++) {
    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(i);
    printf("%i. %s (\"%s\")\n",i,descriptor->name,descriptor->id);
  }
}

//----------

void mip_exe_list_parameters(uint32_t index) {
  if (plugin) {
    clap_plugin_params_t* params = (clap_plugin_params_t*)plugin->get_extension(CLAP_EXT_PARAMS);
    if (params) {
      uint32_t count = params->count(clapplugin);
      printf("%i parameters:\n",count);
      for (uint32_t index=0; index<count; index++) {
        clap_param_info_t info;
        if (params->get_info(clapplugin,index,&info)) {
          printf("    %i. [%i] %s [%s] %.2f (%.2f - %.2f) 0x%04x\n", index, info.id, info.name, info.module, info.default_value, info.min_value, info.max_value, info.flags );
        }
      }
    }
  }
}

//----------

void mip_exe_list_ports(uint32_t index) {
  if (plugin) {
    // audio ports
    clap_plugin_audio_ports_t* audio_ports = (clap_plugin_audio_ports_t*)plugin->get_extension(CLAP_EXT_AUDIO_PORTS);
    if (audio_ports) {
      // audio inputs
      uint32_t count = audio_ports->count(clapplugin,true);
      printf("%i audio inputs:\n",count);
      for (uint32_t index=0; index<count; index++) {
        clap_audio_port_info_t info;
        if (audio_ports->get(clapplugin,index,true,&info)) {
          printf("    %i. [%i] %s %s %i %i 0x%02x\n", index, info.id, info.name, info.port_type, info.channel_count, info.in_place_pair, info.flags );
        }
      }
      // audio outputs
      count = audio_ports->count(clapplugin,false);
      printf("%i audio outputs:\n",count);
      for (uint32_t index=0; index<count; index++) {
        clap_audio_port_info_t info;
        if (audio_ports->get(clapplugin,index,false,&info)) {
          printf("    %i. [%i] %s %s %i %i 0x%02x\n", index, info.id, info.name, info.port_type, info.channel_count, info.in_place_pair, info.flags );
        }
      }
    }
    // note ports
    clap_plugin_note_ports_t* note_ports = (clap_plugin_note_ports_t*)plugin->get_extension(CLAP_EXT_NOTE_PORTS);
    if (note_ports) {
      // note inputs
      uint32_t count = note_ports->count(clapplugin,true);
      printf("%i note inputs:\n",count);
      for (uint32_t index=0; index<count; index++) {
        clap_note_port_info_t info;
        if (note_ports->get(clapplugin,index,true,&info)) {
          printf("    %i. [%i] %s 0x%02x 0x%02x\n", index, info.id, info.name, info.preferred_dialect, info.supported_dialects );
        }
      }
      // note outputs
      count = note_ports->count(clapplugin,false);
      printf("%i note outputs:\n",count);
      for (uint32_t index=0; index<count; index++) {
        clap_note_port_info_t info;
        if (note_ports->get(clapplugin,index,false,&info)) {
          printf("    %i. [%i] %s 0x%02x 0x%02x\n", index, info.id, info.name, info.preferred_dialect, info.supported_dialects );
        }
      }
    }
  }
}


//----------------------------------------------------------------------

void mip_exe_open_editor() {
  clap_plugin_gui_t* gui = (clap_plugin_gui_t*)plugin->get_extension(CLAP_EXT_GUI);
  if (gui) {
    #ifdef MIP_LINUX
    if (gui->is_api_supported(clapplugin,CLAP_WINDOW_API_X11,false)) {
      gui->create(clapplugin,CLAP_WINDOW_API_X11,false);
    #endif
    #ifdef MIP_WIN32
    if (gui->is_api_supported(clapplugin,CLAP_WINDOW_API_WIN32,false)) {
      gui->create(clapplugin,CLAP_WINDOW_API_WIN32,false);
    #endif
      gui->set_scale(clapplugin,1.0);
      uint32_t width, height;
      gui->get_size(clapplugin,&width,&height);
      MIP_ExeWindow* window = new MIP_ExeWindow(width,height,clapplugin,gui);
      if (window) {
        window->open();
        clap_window_t clap_window;
        #ifdef MIP_LINUX
          clap_window.api = CLAP_WINDOW_API_X11;
          clap_window.x11 = window->tgtGetWindow();
        #endif
        #ifdef MIP_WIN32
          clap_window.api = CLAP_WINDOW_API_WIN32;
          clap_window.win32 = window->tgtGetHWND();
        #endif
        gui->set_parent(clapplugin,&clap_window);
        gui->show(clapplugin);
        gui->set_size(clapplugin,width,height); // above show?
        window->eventLoop();
        gui->hide(clapplugin);
        gui->destroy(clapplugin);
        window->close();
        delete window;
      } // window
    } // api supported
  } // gui
}

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main(int argc, char** argv) {
  arguments.init(argc,argv);
  int index = arguments.getArgInt("-i");
  if      (arguments.hasOption("-?"  )) mip_exe_print_help();
  else if (arguments.hasOption("-h"  )) mip_exe_print_help();
  else if (arguments.hasOption("-lpl")) mip_exe_list_plugins();
  else {
    bool result = mip_exe_load_plugin(index);
    if (result) {
      if (arguments.hasOption("-lpa")) mip_exe_list_parameters(index);
      if (arguments.hasOption("-lpo")) mip_exe_list_ports(index);
      plugin->activate(44100,0,1024);
      plugin->start_processing();
      bool with_gui = !arguments.hasOption("-ned");
      if (with_gui) mip_exe_open_editor();
      //..
      mip_exe_unload_plugin();
    }
  }
  return 0;
}

//----------------------------------------------------------------------
#endif






