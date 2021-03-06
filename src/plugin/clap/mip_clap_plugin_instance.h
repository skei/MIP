#ifndef mip_clap_instance_included
#define mip_clap_instance_included
//----------------------------------------------------------------------

/*
  a (your) plugin instance
*/

#include "mip.h"
#include "audio/mip_voice_manager.h"
#include "base/types/mip_queue.h"
#include "plugin/mip_plugin_descriptor.h"
#include "plugin/mip_plugin_instance.h"
#include "plugin/mip_plugin_editor.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_utils.h"
#include "plugin/clap/mip_clap_plugin_host.h"

// max 1024 events per audioblock
typedef MIP_Queue<uint32_t,1024> MIP_HostParameterQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_ClapPluginInstance
: public MIP_EditorListener
/*, public MIP_VoiceListener*/ {

//------------------------------
private:
//------------------------------

  const clap_plugin*      MClapPlugin           = nullptr;
  MIP_PluginDescriptor*   MDescriptor           = nullptr;
  MIP_PluginInstance*     MInstance             = nullptr;
  MIP_ProcessContext      MProcessContext       = {};
  MIP_ClapPluginHost*     MHost                 = nullptr;
  MIP_PluginEditor*       MEditor               = nullptr;
  bool                    MEditorIsOpen         = false;
  MIP_HostParameterQueue  MHostParameterQueue   = {};
  float                   MSampleRate           = 0.0;
  uint32_t                MMinFrames            = 0;
  uint32_t                MMaxFrames            = 0; //INT_MAX;
  bool                    MIsProcessing         = false;
  uint32_t                MSelectedPortConfig   = 0;
  clap_plugin_render_mode MRenderMode           = CLAP_RENDER_REALTIME;
  uint32_t                MPluginIndex          = 0; // uses same index for plugin and editor..



//------------------------------
public:
//------------------------------

  MIP_ClapPluginInstance(uint32_t AIndex, MIP_PluginInstance* AInstance, const clap_host* host ) {
    MIP_ClapPrint("index %i instance %p\n",AIndex,AInstance);
    MPluginIndex = AIndex;
    MInstance = AInstance;
    //MInstance->setFormatSpecificInstance(this);
    //MHost = AHost;
    MHost = new MIP_ClapPluginHost(host);
    MDescriptor = MInstance->getDescriptor();

    // TODO, for now, assume stereo
    MProcessContext.inputs  = (float**)malloc(sizeof(float*) * 2);
    MProcessContext.outputs = (float**)malloc(sizeof(float*) * 2);

  }

  //----------

  virtual ~MIP_ClapPluginInstance() {
    MIP_ClapPrint("\n");
    free(MProcessContext.inputs);
    free(MProcessContext.outputs);
    if (MInstance) delete MInstance;
    delete MHost;
  }

//------------------------------
public: // editor listener
//------------------------------

  const clap_plugin* getClapPlugin() {
    return MClapPlugin;
  }

  const clap_host* getClapHost() {
    return MHost->getClapHost();
  }

//------------------------------
public: // voice listener
//------------------------------

//  void on_voice_end(MIP_ProcessContext* AContext, uint32_t time, uint32_t port, uint32_t key, uint32_t chan) final {
//    MIP_PRINT;
//    //MIP_Print("Sending event\n");
//    //const clap_process* process = (const clap_process*)userptr;
//    //clap_event event;
//    //event.type            = CLAP_EVENT_NOTE_END;
//    //event.time            = 0;
//    //event.note.port_index = 0;
//    //event.note.key        = MNote;
//    //event.note.channel    = MChan;
//    //event.note.velocity   = vel;
//    //process->out_events->push_back(process->out_events,&event);
//  }

//------------------------------
public: // editor listener
//------------------------------

  /*
    When the plugin changes a parameter value, it must inform the host.
    It will send @ref CLAP_EVENT_PARAM_VALUE event during process() or flush().
    - set the flag CLAP_EVENT_PARAM_BEGIN_ADJUST to mark the begining of automation recording
    - set the flag CLAP_EVENT_PARAM_END_ADJUST to mark the end of automation recording
    - set the flag CLAP_EVENT_PARAM_SHOULD_RECORD if the event should be recorded

    III. Turning a knob on the Plugin interface
    - if the plugin is not processing, call clap_host_params->request_flush() or
      clap_host->request_process().
    - send an automation event and don't forget to set begin_adjust, end_adjust and should_record
      attributes
    - the plugin is responsible to send the parameter value to its audio processor
  */

  void on_editor_parameter(uint32_t AIndex, float AValue) override {
    MIP_PluginParameter* parameter = MDescriptor->getParameter(AIndex);
    float value = parameter->from01(AValue);
    MInstance->setParameterValue(AIndex,value);
    MHostParameterQueue.write(AIndex);
    //if (MIsProcessing) MHost->request_process();
    //else MHost->params_request_flush();
    //if (!MIsProcessing) MHost->params_request_flush();

  }

  //----------

  void on_editor_resize(uint32_t AWidth, uint32_t AHeight) override {
  }


//------------------------------
private:
//------------------------------

  void handleNoteOn(const clap_event* event) {
    uint8_t msg1 = MIP_MIDI_NOTE_ON + event->note.channel;
    uint8_t msg2 = event->note.key;
    uint8_t msg3 = event->note.velocity * 127.0f;
    MInstance->on_plugin_midi(/*event->time,*/msg1,msg2,msg3);
  }

  //----------

  void handleNoteOff(const clap_event* event) {
    uint8_t msg1 = MIP_MIDI_NOTE_OFF + event->note.channel;
    uint8_t msg2 = event->note.key;
    uint8_t msg3 = event->note.velocity * 127.0f;
    MInstance->on_plugin_midi(/*event->time,*/msg1,msg2,msg3);
  }

  //----------

  void handleNoteEnd(const clap_event* event) {
    //uint8_t msg1 = MIP_MIDI_NOTE_OFF + event->note.channel;
    //uint8_t msg2 = event->note.key;
    //uint8_t msg3 = event->note.velocity * 127.0f;
    //MInstance->on_plugin_midi(/*event->time,*/msg1,msg2,msg3);
  }

  //----------

  void handleNoteChoke(const clap_event* event) {
    //uint8_t msg1 = MIP_MIDI_NOTE_OFF + event->note.channel;
    //uint8_t msg2 = event->note.key;
    //uint8_t msg3 = event->note.velocity * 127.0f;
    //MInstance->on_plugin_midi(/*event->time,*/msg1,msg2,msg3);
  }

  //----------

  void handleNoteExpression(const clap_event* event) {
    uint8_t   msg1 = 0;
    uint8_t   msg2 = 0;
    uint8_t   msg3 = 0;
    int32_t   bend = 0;
    float     f = 0.0;
    //uint32_t  time = event->time;
    switch (event->note_expression.expression_id) {

      case CLAP_NOTE_EXPRESSION_VOLUME:
        // x >= 0, use 20 * log(4 * x)
        break;
      case CLAP_NOTE_EXPRESSION_PAN:
        // pan, 0 left, 0.5 center, 1 right
        break;
      case CLAP_NOTE_EXPRESSION_TUNING:
        // relative tuning in semitone, from -120 to +120
        f = (event->note_expression.value / 48.0);
        bend = 8192 + (8192.0 * f);
        msg1 = MIP_MIDI_PITCHBEND + event->note_expression.channel;
        msg2 = bend & 0x7f;
        msg3 = (bend >> 7) & 0x7f;
        MInstance->on_plugin_midi(/*time,*/msg1,msg2,msg3);
        break;
      case CLAP_NOTE_EXPRESSION_VIBRATO:
        // 0..1
        break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS:
        // 0..1
        msg1 = MIP_MIDI_CONTROL_CHANGE + event->note_expression.channel;
        msg2 = 74;
        msg3 = event->note_expression.value * 127;
        MInstance->on_plugin_midi(/*time,*/msg1,msg2,msg3);
        break;
      case CLAP_NOTE_EXPRESSION_BREATH:
        // 0..1
        break;
      case CLAP_NOTE_EXPRESSION_PRESSURE:
        // 0..1
        //MIP_MIDI_POLY_AFTERTOUCH
        break;
      case CLAP_NOTE_EXPRESSION_TIMBRE:
        // 0..1
        break;
      default:
        break;

    } // switch, note expression id

    MInstance->on_plugin_midi(/*time,*/msg1,msg2,msg3);
  }

  //----------

  void handleNoteMask(const clap_event* event) {
  }

  //----------

  void handleParamValue(const clap_event* event) {
    uint32_t index = event->param_value.param_id;
    float value = event->param_value.value;
    //float v = MDescriptor->parameters[index]->from01(value);
    MInstance->setParameterValue(index,value);
    MInstance->on_plugin_parameter(index,value);
    if (MEditor && MEditorIsOpen) {
      value = MDescriptor->getParameter(index)->to01(value);
      //TODO queue, and do all in refresh/timer
      MEditor->updateParameter(index,value);
    }
  }

  //----------

  void handleParamMod(const clap_event* event) {
  }

  //----------

  void handleTransport(const clap_event* event) {
  }

  //----------

  void handleMidi(const clap_event* event) {
    uint8_t msg1 = event->midi.data[0];
    uint8_t msg2 = event->midi.data[1];
    uint8_t msg3 = event->midi.data[2];
    MInstance->on_plugin_midi(/*event->time,*/msg1,msg2,msg3);
  }

  //----------

  void handleMidiSysex(const clap_event* event) {
  }

//------------------------------
private:
//------------------------------

  void handleInputEvents(const clap_event_list* in_events) {
    if (in_events) {
      uint32_t num = in_events->size(in_events);
      for (uint32_t i=0; i<num; i++) {
        const clap_event* event = in_events->get(in_events,i);
        if (event) {
          //#ifdef MIP_DEBUG_CLAP
          //  MIP_PrintClapEvent(event);
          //#endif
          switch (event->type) {
            case CLAP_EVENT_NOTE_ON:          handleNoteOn(event);          break;
            case CLAP_EVENT_NOTE_OFF:         handleNoteOff(event);         break;
            case CLAP_EVENT_NOTE_END:         handleNoteEnd(event);         break;
            case CLAP_EVENT_NOTE_CHOKE:       handleNoteChoke(event);       break;
            case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpression(event);  break;
            case CLAP_EVENT_NOTE_MASK:        handleNoteMask(event);        break;
            case CLAP_EVENT_PARAM_VALUE:      handleParamValue(event);      break;
            case CLAP_EVENT_PARAM_MOD:        handleParamMod(event);        break;
            case CLAP_EVENT_TRANSPORT:        handleTransport(event);       break;
            case CLAP_EVENT_MIDI:             handleMidi(event);            break;
            case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysex(event);       break;
          }
        }
      }
    }
  }

  //----------

  /*
    It will send @ref CLAP_EVENT_PARAM_VALUE event during process() or flush().
    - set the flag CLAP_EVENT_PARAM_BEGIN_ADJUST to mark the begining of automation recording
    - set the flag CLAP_EVENT_PARAM_END_ADJUST to mark the end of automation recording
    - set the flag CLAP_EVENT_PARAM_SHOULD_RECORD if the event should be recorded
  */

  /*
    clap_event ev;
    ev.time = 0;
    ev.type = CLAP_EVENT_PARAM_VALUE;
    ev.param_value.param_id = value.paramId;
    ev.param_value.value = value.value;
    ev.param_value.channel = -1;
    ev.param_value.key = -1;
    ev.param_value.flags = value.flags;
    ev.param_value.cookie = p;
    process->out_events->push_back(process->out_events, &ev);
  */

  void handleOutputEvents(const clap_event_list* out_events) {
    if (out_events) {
      uint32_t index;
      while (MHostParameterQueue.read(&index)) {
        if (index < MDescriptor->getNumParameters()) {
          float value = MInstance->getParameterValue(index);
          clap_event event;
          event.type                    = CLAP_EVENT_PARAM_VALUE;
          event.time                    = 0;
          event.param_value.cookie      = nullptr;
          event.param_value.param_id    = index;
          event.param_value.port_index  = -1;
          event.param_value.key         = -1;
          event.param_value.channel     = -1;
          event.param_value.flags       = CLAP_EVENT_PARAM_BEGIN_ADJUST | CLAP_EVENT_PARAM_END_ADJUST | CLAP_EVENT_PARAM_SHOULD_RECORD;
          event.param_value.value       = value;
          out_events->push_back(out_events,&event);
          // ???
          //event.param_value.flags = CLAP_EVENT_PARAM_END_ADJUST;
          //out_events->push_back(out_events,&event);
        }
      }
    }
  }

//------------------------------
public:
//------------------------------

  /*
    Must be called after creating the plugin.
    If init returns false, the host must destroy the plugin instance.
  */

  bool clap_instance_init(const clap_plugin *plugin) {
    MClapPlugin = plugin;
    bool result = MInstance->on_plugin_init();
    if (result) {
      MInstance->setDefaultParameterValues();
      MInstance->updateAllParameters();
      MIP_ClapPrint("-> true\n");
      return true;
    }
    MIP_ClapPrint("-> false\n");
    return false;
  }

  //----------

  /*
    Free the plugin and its resources.
    It is not required to deactivate the plugin prior to this call.
  */

  void clap_instance_destroy() {
    MIP_ClapPrint("\n");
//
//    MInstance->on_plugin_destroy();
//
  }

  //----------

  /*
    Activate and deactivate the plugin.
    In this call the plugin may allocate memory and prepare everything needed
    for the process call. The process's sample rate will be constant and
    process's frame count will included in the [min, max] range, which is
    bounded by [1, INT32_MAX]. Once activated the latency and port
    configuration must remain constant, until deactivation.
    [main-thread
  */

  bool clap_instance_activate(double sample_rate, uint32_t minframes, uint32_t maxframes) {
    MSampleRate = sample_rate;
    MMinFrames  = minframes;
    MMaxFrames  = maxframes;
    bool result = MInstance->on_plugin_activate(MSampleRate,MMinFrames,MMaxFrames);
    MIP_ClapPrint("sample_rate %.2f min_frames %i max_frames %i -> %s\n",sample_rate,minframes,maxframes, result?"true":"false");
    return result;
  }

  //----------

  /* */

  void clap_instance_deactivate() {
    MIP_ClapPrint("\n");
    MInstance->on_plugin_deactivate();
  }

  //----------

  /*
    Set to true before processing,
    and to false before sending the plugin to sleep.
    [audio-thread]
  */

  bool clap_instance_start_processing() {
    MIP_ClapPrint("\n");
    MIsProcessing = MInstance->on_plugin_startProcessing();
    MIP_ClapPrint("returning %s\n",MIsProcessing?"true":"false");
    return MIsProcessing;
  }

  //----------

  /* */

  void clap_instance_stop_processing() {
    MIP_ClapPrint("\n");
    MInstance->on_plugin_stopProcessing();
    MIsProcessing = false;
  }

  //----------

  /*
    process audio, events, ...
    [audio-thread]
    TODO: sample accurate events
    TODO: handle ports/channels (see
  */

  clap_process_status clap_instance_process(const clap_process *process) {
    //MIP_ClapPrint("\n");
    MProcessContext.formatptr = (void*)process;
    handleInputEvents(process->in_events);
    MProcessContext.mode          = 0;
    //MProcessContext.offset        = 0;
    MProcessContext.playstate     = MIP_PLUGIN_PLAYSTATE_NONE;
    MProcessContext.samplerate    = MSampleRate;
    MProcessContext.samplepos     = process->steady_time;
    MProcessContext.blocksize     = process->frames_count;
    MProcessContext.numinputs     = process->audio_inputs[0].channel_count;
    MProcessContext.numoutputs    = process->audio_outputs[0].channel_count;
    uint32_t i;
    for (i=0; i<MProcessContext.numinputs; i++) {
      MProcessContext.inputs[i] = process->audio_inputs[0].data32[i];
    }
    for (i=0; i<MProcessContext.numoutputs; i++) {
      MProcessContext.outputs[i] = process->audio_outputs[0].data32[i];
    }
    MProcessContext.tempo         = process->transport->tempo;
    MProcessContext.timesignum    = process->transport->tsig_num;
    MProcessContext.timesigdenom  = process->transport->tsig_denom;
    MProcessContext.beatpos       = process->transport->song_pos_beats;
    if (process->transport->flags & CLAP_TRANSPORT_IS_PLAYING)      MProcessContext.playstate |= MIP_PLUGIN_PLAYSTATE_PLAYING;
    if (process->transport->flags & CLAP_TRANSPORT_IS_RECORDING)    MProcessContext.playstate |= MIP_PLUGIN_PLAYSTATE_RECORDING;
    if (process->transport->flags & CLAP_TRANSPORT_IS_LOOP_ACTIVE)  MProcessContext.playstate |= MIP_PLUGIN_PLAYSTATE_LOOPING;
    MInstance->on_plugin_process(&MProcessContext);
    handleOutputEvents(process->out_events);
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  /*
    Query an extension.
    The returned pointer is owned by the plugin.
    [thread-safe]
  */

  const void *clap_instance_get_extension(const char *id) {
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG) == 0)  { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapAudioPortsConfig);  return &MClapAudioPortsConfig;  }
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)         { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapAudioPorts);        return &MClapAudioPorts;        }
  //if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE) == 0)    { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapCheckForUpdate);    return &MClapCheckForUpdate;    }
    if (strcmp(id,CLAP_EXT_EVENT_FILTER) == 0)        { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapEventFilter);       return &MClapEventFilter;       }
    if (strcmp(id,CLAP_EXT_FD_SUPPORT) == 0)          { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapFdSupport);         return &MClapFdSupport;         }
    if (strcmp(id,CLAP_EXT_FILE_REFERENCE) == 0)      { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapFileReference);     return &MClapFileReference;     }

    if (MDescriptor->hasEditor()) {
      if (strcmp(id,CLAP_EXT_GUI) == 0)               { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapGui);               return &MClapGui;               }
      if (strcmp(id,CLAP_EXT_GUI_X11) == 0)           { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapGuiX11);            return &MClapGuiX11;            }
    }

    if (strcmp(id,CLAP_EXT_LATENCY) == 0)             { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapLatency);           return &MClapLatency;           }
    if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS) == 0)       { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapMidiMappings);      return &MClapMidiMappings;      }
    if (strcmp(id,CLAP_EXT_NOTE_NAME) == 0)           { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapNoteName);          return &MClapNoteName;          }
    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)          { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapNotePorts);         return &MClapNotePorts;         }
    if (strcmp(id,CLAP_EXT_PARAMS) == 0)              { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapParams);            return &MClapParams;            }
    if (strcmp(id,CLAP_EXT_PRESET_LOAD) == 0)         { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapPresetLoad);        return &MClapPresetLoad;        }
    if (strcmp(id,CLAP_EXT_QUICK_CONTROLS) == 0)      { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapQuickControls);     return &MClapQuickControls;     }
    if (strcmp(id,CLAP_EXT_RENDER) == 0)              { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapRender);            return &MClapRender;            }
    if (strcmp(id,CLAP_EXT_STATE) == 0)               { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapState);             return &MClapState;             }
    if (strcmp(id,CLAP_EXT_TIMER_SUPPORT) == 0)       { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapTimerSupport);      return &MClapTimerSupport;      }
    if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)         { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapThreadPool);        return &MClapThreadPool;        }
    if (strcmp(id,CLAP_EXT_TRACK_INFO) == 0)          { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapTrackInfo);         return &MClapTrackInfo;         }
    if (strcmp(id,CLAP_EXT_VST2_CONVERT) == 0)        { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapVst2Convert);       return &MClapVst2Convert;       }
    if (strcmp(id,CLAP_EXT_VST3_CONVERT) == 0)        { MIP_ClapPrint("id '%s' -> %p\n",id,&MClapVst3Convert);       return &MClapVst3Convert;       }
    MIP_ClapPrint("id '%s' -> NULL\n",id);
    return nullptr;
  }

  //----------

  /*
    Called by the host on the main thread in response to a previous call to:
    host->request_callback(host);
    [main-thread]
  */

  void clap_instance_on_main_thread() {
    MIP_ClapPrint("\n");
  }

//------------------------------
public: // extensions
//------------------------------

  //------------------------------
  // audio ports
  //------------------------------

  /*
    This extension provides a way for the plugin to describe its current audio
    ports.
    If the plugin does not implement this extension, it will have a default
    stereo input and output.
    The plugin is only allowed to change its ports configuration while it is
    deactivated.
  */

  //----------

  /*
    number of ports, for either input or output
    [main-thread]
  */

  //TODO: for now, assume stereo in/out

  uint32_t clap_audio_ports_count(bool is_input) {
    if (is_input) {
      MIP_ClapPrint("is_input true -> %i\n",MDescriptor->getNumInputPorts());
      return MDescriptor->getNumInputPorts();
    }
    else {
      MIP_ClapPrint("is_input false -> %i\n",MDescriptor->getNumOutputPorts());
      return MDescriptor->getNumOutputPorts();
    }
  }

  //----------

  /*
    get info about about an audio port.
    [main-thread]

    see clap_audio_ports_config_get (above) for CLAP_CHMAP_* flags
  */

  bool clap_audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info *info) {
    if (is_input) {
      //switch(index) {
      //  case 0:
          MIP_PluginPort* port = MDescriptor->getInputPort(index);
          info->id            = 0;

          // warning: ???char* __builtin_strncpy(char*, const char*, long unsigned int)??? specified bound 64 equals destination size [-Wstringop-truncation]
          strncpy(info->name,port->name,CLAP_NAME_SIZE);

          info->channel_count = port->channels;
          //info->channel_map   = CLAP_CHMAP_STEREO;
          if (port->channels==1) info->channel_map = CLAP_CHMAP_MONO;
          else if (port->channels==2) info->channel_map = CLAP_CHMAP_STEREO;
          else info->channel_map = CLAP_CHMAP_UNSPECIFIED;
          info->sample_size   = 32;     // 32 for float and 64 for double
          info->is_main       = true;   // there can only be 1 main input and output
          info->is_cv         = false;  // control voltage
          info->in_place      = true;   // if true the daw can use the same buffer for input and output, only for main input to main output
          MIP_ClapPrint("index %i is_input %s -> true\n",index,is_input?"true":"false");
          return true;
      //}
    }
    else { // output
      //switch(index) {
      //  case 0:
          MIP_PluginPort* port = MDescriptor->getInputPort(index);
          info->id            = 0;

          // warning: ???char* __builtin_strncpy(char*, const char*, long unsigned int)??? specified bound 64 equals destination size [-Wstringop-truncation]
          strncpy(info->name,port->name,CLAP_NAME_SIZE);

          info->channel_count = port->channels;
          //info->channel_map   = CLAP_CHMAP_STEREO;
          if (port->channels==1) info->channel_map = CLAP_CHMAP_MONO;
          else if (port->channels==2) info->channel_map = CLAP_CHMAP_STEREO;
          else info->channel_map = CLAP_CHMAP_UNSPECIFIED;
          info->sample_size   = 32;     // 32 for float and 64 for double
          info->is_main       = true;   // there can only be 1 main input and output
          info->is_cv         = false;  // control voltage
          info->in_place      = true;   // if true the daw can use the same buffer for input and output, only for main input to main output
          MIP_ClapPrint("index %i is_input %s -> true\n",index,is_input?"true":"false");
          return true;
      //}
    }
    MIP_ClapPrint("index %i is_input %s -> false\n",index,is_input?"true":"false");
    return false;
  }

  //------------------------------
  // audio ports config
  //------------------------------

  /*
    This extension provides a way for the plugin to describe possible ports
    configurations, for example mono, stereo, surround, ... and a way for the
    host to select a configuration.

    After the plugin initialization, the host may scan the list of
    configurations and eventually select one that fits the plugin context.
    The host can only select a configuration if the plugin is deactivated.

    A configuration is a very simple description of the audio ports:
    - it describes the main input and output ports
    - it has a name that can be displayed to the user

    The idea behind the configurations, is to let the user choose one via a
    menu.

    Plugin with very complex configuration possibilities should let the user
    configure the ports from the plugin GUI, and call
    clap_host_audio_ports.rescan(CLAP_AUDIO_PORTS_RESCAN_ALL).
  */

  //----------

  /*
    gets the number of available configurations
    [main-thread]
  */

  uint32_t clap_audio_ports_config_count() {
    MIP_ClapPrint("-> 1\n");
    return 1;
  }

  //----------

  /*
    gets information about a configuration
    [main-thread]

    CLAP_CHMAP_UNSPECIFIED
    CLAP_CHMAP_MONO
    CLAP_CHMAP_STEREO
    CLAP_CHMAP_SURROUND
  */

  // todo: channel count/chmap from descriptor (port #0?)

  bool clap_audio_ports_config_get(uint32_t index, clap_audio_ports_config *config) {
    switch(index) {
      case 0:
        config->id                    = 0;
        strncpy(config->name,"port config 1",CLAP_NAME_SIZE);
        config->input_channel_count   = 2;
        config->input_channel_map     = CLAP_CHMAP_STEREO;
        config->output_channel_count  = 2;
        config->output_channel_map    = CLAP_CHMAP_STEREO;
        MIP_ClapPrint("index %i -> true\n");
        return true;
    }
    MIP_ClapPrint("index %i -> false\n");
    return false;
  }

  //----------

  /*
   selects the configuration designated by id
   returns true if the configuration could be applied
   [main-thread,plugin-deactivated]
  */

  bool clap_audio_ports_config_select(clap_id config_id) {
    MIP_ClapPrint("config_id %i -> true\n",config_id);
    MSelectedPortConfig = config_id;
    return true;
  }

  //------------------------------
  // check for update
  //------------------------------

  // [main-thread]

  //void clap_check_for_update_check(const clap_host *host, bool include_beta) {
  //}

  void clap_check_for_update_check(const clap_plugin *plugin, bool include_beta) {
  }

  //------------------------------
  // event filter
  //------------------------------

  /*
    This extension lets the host know which event types the plugin is
    interested in.
  */

  //----------

  /*
    Returns true if the plugin is interested in the given event type.
    [main-thread]
  */

  bool clap_event_filter_accepts(clap_event_type event_type) {
    //MIP_ClapPrint("\n");
    switch (event_type) {
      case CLAP_EVENT_NOTE_ON:          MIP_ClapPrint("event_type CLAP_EVENT_NOTE_ON -> true\n");          return true;
      case CLAP_EVENT_NOTE_OFF:         MIP_ClapPrint("event_type CLAP_EVENT_NOTE_OFF -> true\n");         return true;
      case CLAP_EVENT_NOTE_END:         MIP_ClapPrint("event_type CLAP_EVENT_NOTE_END -> true\n");         return true;
      case CLAP_EVENT_NOTE_CHOKE:       MIP_ClapPrint("event_type CLAP_EVENT_NOTE_CHOKE -> true\n");       return true;
      case CLAP_EVENT_NOTE_EXPRESSION:  MIP_ClapPrint("event_type CLAP_EVENT_NOTE_EXPRESSION -> true\n");  return true;
      case CLAP_EVENT_NOTE_MASK:        MIP_ClapPrint("event_type CLAP_EVENT_NOTE_MASK -> true\n");        return true;
      case CLAP_EVENT_PARAM_VALUE:      MIP_ClapPrint("event_type CLAP_EVENT_PARAM_VALUE -> true\n");      return true;
      case CLAP_EVENT_PARAM_MOD:        MIP_ClapPrint("event_type CLAP_EVENT_PARAM_MOD -> true\n");        return true;
      case CLAP_EVENT_TRANSPORT:        MIP_ClapPrint("event_type CLAP_EVENT_TRANSPORT -> true\n");        return true;
      case CLAP_EVENT_MIDI:             MIP_ClapPrint("event_type CLAP_EVENT_MIDI -> true\n");             return true;
      case CLAP_EVENT_MIDI_SYSEX:       MIP_ClapPrint("event_type CLAP_EVENT_MIDI_SYSEX -> true\n");       return true;
    }
    MIP_ClapPrint("event_type %i unknown -> false\n",event_type);
    return false;
  }

  //------------------------------
  // fd support
  //------------------------------

  /*
  */

  //----------

  /*
    This callback is "level-triggered".
    It means that a writable fd will continuously produce "on_fd()" events;
    don't forget using modify_fd() to remove the write notification once you're
    done writting.
    [main-thread]

    CLAP_FD_READ
    CLAP_FD_WRITE
    CLAP_FD_ERROR
  */

  void clap_fd_support_on_fd(clap_fd fd, clap_fd_flags flags) {
    MIP_ClapPrint("fd %i flags %i\n",fd,flags);
  }

  //------------------------------
  // file reference
  //------------------------------

  // returns the number of file reference this plugin has
  // [main-thread]

  uint32_t clap_file_reference_count() {
    MIP_ClapPrint("\n");
    return 0;
  }

  // gets the file reference at index
  // returns true on success
  // [main-thread]

  bool clap_file_reference_get(uint32_t index, clap_file_reference *file_reference) {
    MIP_ClapPrint("index %i\n",index);
    return false;
  }

  // This method does not compute the hash.
  // It is only used in case of missing resource. The host may have additionnal known resource
  // location and may be able to locate the file by using its known hash.
  // [main-thread]

  bool clap_file_reference_get_hash(clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) {
    MIP_ClapPrint("resource_id %i\n",resource_id);
    return false;
  }

  // updates the path to a file reference
  // [main-thread]

  bool clap_file_reference_update_path(clap_id resource_id, const char *path) {
    MIP_ClapPrint("resource_id %i path %s\n",resource_id,path);
    return false;
  }

  // [main-thread]

  bool clap_file_reference_save_resources() {
    MIP_ClapPrint("\n");
    return false;
  }

  //------------------------------
  // gui
  //------------------------------

  /*
    This extension is split in two interfaces:
    - `gui` which is the generic part
    - `gui_XXX` which is the platform specific interfaces; @see clap_gui_win32.

    Showing the GUI works as follow:
    1. clap_plugin_gui->create(), allocates gui resources
    2. clap_plugin_gui->set_scale()
    3. clap_plugin_gui->get_size(), gets initial size
    4. clap_plugin_gui_win32->embed(), or any other platform specific interface
    5. clap_plugin_gui->show()
    6. clap_plugin_gui->hide()/show() ...
    7. clap_plugin_gui->close() when done with the gui

    Resizing the window:
    1. Only possible if clap_plugin_gui->can_resize() returns true
    2. Mouse drag -> new_size
    3. clap_plugin_gui->round_size(new_size) -> working_size
    4. clap_plugin_gui->set_size(working_
  */

  //----------

  /*
    Create and allocate all resources necessary for the gui.
    After this call, the GUI is ready to be shown but it is not yet visible.
    [main-thread]
  */

  // note: the editor doesn't have a window yet!

  bool clap_gui_create() {
    #ifndef MIP_NO_GUI
      MEditor = MIP_CreateEditor(MPluginIndex,this,MDescriptor);
      //MEditor = _mip_create_editor(this,MDescriptor);
      //bool result = MInstance->on_plugin_createEditor(MEditor);
      //return result;
      MIP_ClapPrint("-> true\n");
      return true;
    #else
      return false;
    #endif
  }

  //----------

  /*
    Free all resources associated with the gui.
    [main-thread]
  */

  void clap_gui_destroy() {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("\n");
    if (MEditor) {
      //MInstance->on_plugin_destroyEditor(MEditor);
      delete MEditor;
      MEditor = nullptr;
    }
    #endif
  }

  //----------

  /*
    Set the absolute GUI scaling factor.
    [main-thread]
  */

  void clap_gui_set_scale(double scale) {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("scale %f\n",scale);
    if (MEditor) {
      MEditor->setScale(scale);
    }
    #endif
  }

  //----------

  /*
    Get the current size of the plugin UI, with the scaling applied.
    clap_plugin_gui->create() must have been called prior to asking the size.
    [main-thread]
  */

  bool clap_gui_get_size(uint32_t *width, uint32_t *height) {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("-> true\n");
    // todo: if editor open, read from editor, else:
    *width  = MDescriptor->getEditorRect().w;
    *height = MDescriptor->getEditorRect().h;
    return true;
    #else
      return false;
    #endif
  }

  //----------

  /*
    [main-thread]
  */

  bool clap_gui_can_resize() {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("-> true\n");
    if (MDescriptor->canResizeEditor()) return true;
    return false;
    #else
      return false;
    #endif
  }

  //----------

  /*
    If the plugin gui is resizable, then the plugin will calculate the closest
    usable size to the given arguments.
    The scaling is applied.
    This method does not change the size.
    [main-thread]
  */

  void clap_gui_round_size(uint32_t *width, uint32_t *height) {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("\n");
    //MIP_ClapPrint("width %i height %i\n",*width,*height);
    // todo: if editor open, read from editor, else:
    *width  = MDescriptor->getEditorRect().w;
    *height = MDescriptor->getEditorRect().h;
    #endif
  }

  //----------

  /*
    Sets the window size
    Returns true if the size is supported.
    [main-thread]
  */

  bool clap_gui_set_size(uint32_t width, uint32_t height) {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("width %i height %i -> true\n",width,height);
    if (MEditor) {
      MEditor->resize(width,height);
    }
    return true;
    #else
      return false;
    #endif
  }

  //----------

  /*
    Show the window.
    [main-thread]
  */

  void clap_gui_show() {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("\n");
    if (MEditor && !MEditorIsOpen) {
      /*MEditorIsOpen =*/ MInstance->on_plugin_openEditor(MEditor->getWindow());
      MInstance->updateAllEditorParameters(MEditor,false);
      MEditorIsOpen = true;
      MEditor->open();
    }
    #endif
  }

  //----------

  /*
    Hide the window, this method do not free the resources, it just hides
    the window content. Yet it maybe a good idea to stop painting timers.
    [main-thread]
  */

  void clap_gui_hide() {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("\n");
    if (MEditor && MEditorIsOpen) {
      MEditor->close();
      MEditorIsOpen = false;
    }
    #endif
  }

  //------------------------------
  // gui x11
  //------------------------------

  /*
  */

  //----------

  /*
    Use the protocol XEmbed
    [main-thread]
  */

  bool clap_gui_x11_attach(const char* display_name, unsigned long window) {
    #ifndef MIP_NO_GUI
    MIP_ClapPrint("display_name '%s' window %i -> true\n",display_name,window);
    MEditor->attach(display_name,(void*)window);
    MEditorIsOpen = false;
    return true;
    #else
      return false;
    #endif
  }

  //------------------------------
  // latency
  //------------------------------

  /*
    Returns the plugin latency.
    [main-thread]
  */

  uint32_t clap_latency_get() {
    MIP_ClapPrint("-> 0\n");
    return 0;
  }

  //------------------------------
  // midi mappings
  //------------------------------

  // [main-thread]

  uint32_t clap_midi_mappings_count() {
    return 0;
  }

  // [main-thread]

  bool clap_midi_mappings_get(uint32_t index, clap_midi_mapping *mapping) {
    return false;
  }

  //------------------------------
  // note name
  //------------------------------

  /*
  */

  //----------

  /*
    Return the number of note names
    [main-thread]
  */

  uint32_t clap_note_name_count() {
    MIP_ClapPrint("-> 0\n");
    return 0;
  }

  //----------

  /*
    Returns true on success and stores the result into note_name
    [main-thread]
  */

  bool clap_note_name_get(uint32_t index, clap_note_name *note_name) {
    MIP_ClapPrint("index %i -> false\n",index);
    return false;
  }

  //------------------------------
  // note-ports
  //------------------------------

  // number of ports, for either input or output
  // [main-thread]

  uint32_t clap_note_ports_count(bool is_input) {
    MIP_ClapPrint("is_input %s\n", is_input?"true":"false" );
    return 0;
  }

  // get info about about a note port.
  // [main-thread]

  bool clap_note_ports_get(uint32_t index, bool is_input, clap_note_port_info* info) {
    MIP_ClapPrint("index %i is_input %s\n",index,is_input?"true":"false");
    return false;
  }


  //------------------------------
  // params
  //------------------------------

  /*
    The host sees the plugin as an atomic entity; and acts as a controler on
    top of its parameters. The plugin is responsible to keep in sync its audio
    processor and its GUI.
    The host can read at any time parameters value on the [main-thread] using
    @ref clap_plugin_params.value().
    There is two options to communicate parameter value change, and they are
    not concurrent.
    - send automation points during clap_plugin.process()
    - send automation points during clap_plugin_params.flush(),
      this one is used when the plugin is not processing

    When the plugin changes a parameter value, it must inform the host.
    It will send @ref CLAP_EVENT_PARAM_VALUE event during process() or flush().
    - set the flag CLAP_EVENT_PARAM_BEGIN_ADJUST to mark the begining of
      automation recording
    - set the flag CLAP_EVENT_PARAM_END_ADJUST to mark the end of automation
      recording
    - set the flag CLAP_EVENT_PARAM_SHOULD_RECORD if the event should be
      recorded

    MIDI CCs are a tricky because you may not know when the parameter
    adjustment ends. Also if the hosts records incoming MIDI CC and parameter
    change automation at the same time, there will be a conflict at playback:
    MIDI CC vs Automation. The parameter automation will always target the same
    parameter because the param_id is stable. The MIDI CC may have a different
    mapping in the future and may result in a different playback.

    When a MIDI CC changes a parameter's value, set
    clap_event_param.should_record to false.
    That way the host may record the MIDI CC automation, but not the parameter
    change and there won't be conflict at playback.

    Scenarios:

    I. Loading a preset
    - load the preset in a temporary state
    - call @ref clap_host_params.changed() if anything changed
    - call @ref clap_host_latency.changed() if latency changed
    - invalidate any other info that may be cached by the host
    - if the plugin is activated and the preset will introduce breaking change
      (latency, audio ports, new parameters, ...) be sure to wait for the host
      to deactivate the plugin to apply those changes.
      If there are no breaking changes, the plugin can apply them them right away.
      The plugin is resonsible to update both its audio processor and its gui.

    II. Turning a knob on the DAW interface
    - the host will send an automation event to the plugin via a process() or flush()

    III. Turning a knob on the Plugin interface
    - if the plugin is not processing, call clap_host_params->request_flush() or
      clap_host->request_process().
    - send an automation event and don't forget to set begin_adjust, end_adjust and should_record
      attributes
    - the plugin is responsible to send the parameter value to its audio processor

    IV. Turning a knob via automation
    - host sends an automation point during clap_plugin->process() or clap_plugin_params->flush().
    - the plugin is responsible to update its GUI

    V. Turning a knob via plugin's internal MIDI mapping
    - the plugin sends a CLAP_EVENT_PARAM_SET output event, set should_record to false
    - the plugin is responsible to update its GUI (since bitwig returns null

    VI. Adding or removing parameters
    - if the plugin is activated call clap_host->restart()
    - once the plugin isn't active:
      - apply the new state
      - call clap_host_params->rescan(CLAP_PARAM_RESCAN_ALL)
      - if a parameter is created with an id that may have been used before,
        call clap_host_params.clear(host, param_id, CLAP_PARAM_CLEAR_ALL)
  */

  //----------

  /*
    Returns the number of parameters.
    [main-thread]
  */

  uint32_t clap_params_count() {
    uint32_t num = MDescriptor->getNumParameters();
    MIP_ClapPrint("-> %i\n",num);
    return num;
  }

  //----------

  /*
    Copies the parameter's info to param_info and returns true on success.
    [main-thread]

    param_info->flags
      CLAP_PARAM_IS_STEPPED         // Is this param stepped? (integer values only). if so the double value is converted to integer using a cast (equivalent to trunc).
      CLAP_PARAM_IS_PER_NOTE        // Does this param supports per note automations?
      CLAP_PARAM_IS_PER_CHANNEL     // Does this param supports per channel automations?
      CLAP_PARAM_IS_PER_PORT        // Does this param supports per port automations?
      CLAP_PARAM_IS_PERIODIC        // Useful for for periodic parameters like a phase
      CLAP_PARAM_IS_HIDDEN          // The parameter should not be shown to the user, because it is currently not used. It is not necessary to process automation for this parameter.
      CLAP_PARAM_IS_BYPASS          // This parameter is used to merge the plugin and host bypass button. It implies that the parameter is stepped. min: 0 -> bypass off. max: 1 -> bypass on
      CLAP_PARAM_IS_READONLY        // The parameter can't be changed by the host.
      CLAP_PARAM_IS_MODULATABLE     // Does the parameter support the modulation signal?
      CLAP_PARAM_REQUIRES_PROCESS   // Any change to this parameter will affect the plugin output and requires to be done via process() if the plugin is active. A simple example would be a DC Offset, changing it will change the output signal and must be processed.
  */

  bool clap_params_get_info(int32_t param_index, clap_param_info *param_info) {
    MIP_ClapPrint("param_index %i -> true\n",param_index);
    MIP_PluginParameter* parameter = MDescriptor->getParameter(param_index);
    uint32_t flags = 0;
    if (parameter->isHidden())        param_info->flags |= CLAP_PARAM_IS_HIDDEN;
    if (parameter->isReadOnly())      param_info->flags |= CLAP_PARAM_IS_READONLY;
    if (parameter->canModulate())     param_info->flags |= CLAP_PARAM_IS_MODULATABLE;
    if (parameter->getNumSteps() > 1) param_info->flags |= CLAP_PARAM_IS_STEPPED;

    // warning: ???char* __builtin_strncpy(char*, const char*, long unsigned int)??? specified bound 64 equals destination size [-Wstringop-truncation]
    strncpy(param_info->name,parameter->getName(),CLAP_NAME_SIZE);

    strncpy(param_info->module,"",CLAP_MODULE_SIZE);
    param_info->flags         = flags;
    param_info->id            = param_index;
    param_info->cookie        = parameter;
    param_info->min_value     = parameter->getMinValue();
    param_info->max_value     = parameter->getMaxValue();
    param_info->default_value = parameter->getDefaultValue();
    return true;
  }



  //----------

  /*
    Gets the parameter plain value.
    [main-thread]
  */

  bool clap_params_get_value(clap_id param_id, double *value) {
    MIP_ClapPrint("param_id %i -> true\n",param_id);
    float v = MInstance->getParameterValue(param_id); // !!!
    //v = MDescriptor->parameters[param_id]->from01(v);
    *value = v;
    return true;
  }

  //----------

  /*
    Formats the display text for the given parameter value.
    [main-thread]
  */

  bool clap_params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) {
    MIP_ClapPrint("param_id %i, value %f -> true\n",param_id,value);
    char buffer[256];
    MIP_PluginParameter* param = MDescriptor->getParameter(param_id);
    param->getDisplayText(value,buffer);
    strncpy(display,buffer,size);
    //display[size-1] = 0;
    //MIP_ClapPrint("param_id %i value %.3f -> true (*display '%s'\n",param_id,value,display);
    return true;
  }

  //----------

  /*
    Converts the display text to a parameter value.
    [main-thread]
  */

  bool clap_params_text_to_value(clap_id param_id, const char* display, double* value) {
    MIP_ClapPrint("param_id %i display '%s' -> true\n",param_id,display);
    //int i = atoi(display);
    float f = atof(display);
    *value = f;
    return true;
  }

  //----------

  /*
    Flushes a set of parameter changes.
    This method must not be called concurrently to clap_plugin->process().
    This method must not be used if the plugin is processing.
    [active && !processing : audio-thread]
    [!active : main-thread]
  */

  void clap_params_flush(const clap_event_list *input_parameter_changes, const clap_event_list *output_parameter_changes) {
    MIP_ClapPrint("input_param_changes %p output_param_changes %p\n",input_parameter_changes,output_parameter_changes);
    handleInputEvents(input_parameter_changes);
    handleOutputEvents(output_parameter_changes);
  }

  //------------------------------
  // preset load
  //------------------------------

  // Loads a preset in the plugin native preset file format from a path.
  // [main-thread]

  bool clap_preset_load_from_file(const char *path) {
    MIP_ClapPrint("path %s\n",path);
    return false;
  }

  //------------------------------
  // quick controls
  //------------------------------

  // [main-thread]

  uint32_t clap_quick_controls_count() {
    return 0;
  }

  // [main-thread]

  bool clap_quick_controls_get(uint32_t page_index, clap_quick_controls_page *page) {
    return false;
  }

  // [main-thread]

  void clap_quick_controls_select(clap_id page_id) {
  }

  // [main-thread]

  clap_id clap_quick_controls_get_selected() {
    return 0;
  }

  //------------------------------
  // render
  //------------------------------

  /*
    The render extension is used to let the plugin know if it has "realtime"
    pressure to process.
    If this information does not influence your rendering code, then don't
    implement this extension
  */

  //----------

  /*
    [main-thread]

    CLAP_RENDER_REALTIME
    CLAP_RENDER_OFFLINE
  */

  void clap_render_set(clap_plugin_render_mode mode) {
    MIP_ClapPrint("mode %i\n",mode);
    MRenderMode = mode;
  }

  //------------------------------
  // state
  //------------------------------

  /*
  */

  //----------

  /*
    Saves the plugin state into stream.
    Returns true if the state was correctly saved.
    [main-thread]

    clap_ostream:
      void     *ctx;                                                                      // reserved pointer for the stream
      int64_t (*write)(struct clap_ostream *stream, const void *buffer, uint64_t size);   // returns the number of bytes written. -1 on error.
  */

  bool clap_state_save(clap_ostream *stream) {
    MIP_ClapPrint("stream %p -> true\n",stream);
    MIP_Assert(stream);
      uint32_t version = MDescriptor->getVersion();
      stream->write(stream,&version,sizeof(uint32_t));
      uint32_t num_params = MDescriptor->getNumParameters();
      stream->write(stream,&num_params,sizeof(uint32_t));
      for (uint32_t i=0; i<num_params; i++) {
        float value = MInstance->getParameterValue(i);
        stream->write(stream,&value,sizeof(float));
      }
    return true;
  }

  //----------

  /*
    Tell the host that the plugin state has changed and should be saved again.
    If a parameter value changes, then it is implicit that the state is dirty.
    [main-thread]

    clap_istream:
      void    *ctx;                                                             // reserved pointer for the stream
      nt64_t (*read)(struct clap_istream *stream, void *buffer, uint64_t size); // returns the number of bytes read. 0 for end of file. -1 on error.
  */

  bool clap_state_load(clap_istream *stream) {
    MIP_ClapPrint("stream %p -> true\n",stream);
    MIP_Assert(stream);
    uint32_t version = 0;
    stream->read(stream,&version,sizeof(uint32_t));
    uint32_t num_params = 0;
    stream->read(stream,&num_params,sizeof(uint32_t));
    for (uint32_t i=0; i<num_params; i++) {
      float value = 0.0;
      stream->read(stream,&value,sizeof(float));
      MInstance->setParameterValue(i,value);
    }
    return true;
  }

  //------------------------------
  // thread pool
  //------------------------------

  // Called by the thread pool

  void clap_thread_pool_exec(uint32_t task_index) {
    MIP_ClapPrint("task_index %i\n",task_index);
  }

  //------------------------------
  // timer support
  //------------------------------

  /*
  */

  //----------

  /*
    [main-thread]
  */

  void clap_timer_support_on_timer(clap_id timer_id) {
    MIP_ClapPrint("timer_id %i\n",timer_id);
//    if (MEditor && MEditorIsOpen) {
//      MInstance->on_plugin_updateEditor(MEditor);
//    }
  }

  //------------------------------
  // track info
  //------------------------------

  // [main-thread]

  void clap_track_info_changed() {
    MIP_ClapPrint("\n");
  }

  //------------------------------
  // vst2 convert
  //------------------------------

  // Copies the name and VST2 plugin id that we can convert from.
  // Returns the lenght of the name.
  // [thread-safe]

  int32_t clap_vst2_convert_get_vst2_plugin_id(uint32_t* vst2_plugin_id, char* name, uint32_t name_size) {
    MIP_ClapPrint("\n");
    return 0;
  }

  // Loads the plugin state from stream using the VST2 chunk.
  // [main-thread]

  bool clap_vst2_convert_restore_vst2_state(clap_istream *stream) {
    MIP_ClapPrint("\n");
    return false;
  }

  // converts the vst2 param id and normalized value to clap param id and
  // plain value.
  // [thread-safe]

  bool clap_vst2_convert_convert_normalized_value(uint32_t vst2_param_id, double vst2_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_ClapPrint("\n");
    return false;
  }

  // converts the vst2 param id and plain value to clap param id and
  // plain value.
  // [thread-safe]

  bool clap_vst2_convert_convert_plain_value(uint32_t vst2_param_id, double vst2_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_ClapPrint("\n");
    return false;
  }


  //------------------------------
  // vst3 convert
  //------------------------------

  // Copies the name and VST3 plugin id that we can convert from.
  // [thread-safe]

  void clap_vst3_convert_get_vst3_plugin_id(const clap_plugin *plugin, uint8_t *vst3_plugin_uuid) {
    MIP_ClapPrint("\n");
  }

  // Loads the plugin state from stream using the vst3 chunk.
  // [main-thread]

  bool clap_vst3_convert_restore_vst3_state(const clap_plugin *plugin, clap_istream *stream) {
    MIP_ClapPrint("\n");
    return false;
  }

  // converts the VST3 param id and normalized value to clap param id and
  // normalized value.
  // [thread-safe]

  bool clap_vst3_convert_convert_normalized_value(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_ClapPrint("\n");
    return false;
  }

  // converts the vst3 param id and plain value to clap param id and
  // plain value.
  // [thread-safe]

  bool clap_vst3_convert_convert_plain_value(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_ClapPrint("\n");
    return false;
  }

//------------------------------
public: // extension callbacks
//------------------------------

  // clap.audio-ports-config

  static uint32_t clap_audio_ports_config_count_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_audio_ports_config_count();
  }

  static bool clap_audio_ports_config_get_callback(const clap_plugin *plugin, uint32_t index, clap_audio_ports_config *config) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_audio_ports_config_get(index,config);
  }

  static bool clap_audio_ports_config_select_callback(const clap_plugin *plugin, clap_id config_id) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_audio_ports_config_select(config_id);
  }

  // clap.audio-ports

  static uint32_t clap_audio_ports_count_callback(const clap_plugin *plugin, bool is_input) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_audio_ports_count(is_input);

  }

  static bool clap_audio_ports_get_callback(const clap_plugin* plugin, uint32_t index, bool is_input, clap_audio_port_info *info) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_audio_ports_get(index,is_input,info);
  };

  // clap.check-for-updates

  //TODO: wait for 'official' fix..

  //static void clap_check_for_update_check_callback(const clap_plugin *plugin, bool include_beta) {
  //  MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
  //  instance->clap_check_for_update(include_beta);
  //}

  static void clap_check_for_update_check_callback(const clap_host *host, bool include_beta) {
  }

  // clap.event-filter

  static bool clap_event_filter_accepts_callback(const clap_plugin *plugin, clap_event_type event_type) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_event_filter_accepts(event_type);
  }

  // clap.fd-support

  static void clap_fd_support_on_fd_callback(const clap_plugin *plugin, clap_fd fd, clap_fd_flags flags) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_fd_support_on_fd(fd,flags);
  }

  // clap.file-reference

  static uint32_t clap_file_reference_count_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_file_reference_count();
  }

  static bool clap_file_reference_get_callback(const clap_plugin *plugin, uint32_t index, clap_file_reference *file_reference) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_file_reference_get(index,file_reference);
  }

  static bool clap_file_reference_get_hash_callback(const clap_plugin *plugin, clap_id resource_id, clap_hash hash, uint8_t* digest, uint32_t digest_size) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_file_reference_get_hash(resource_id,hash,digest,digest_size);
  }

  static bool clap_file_reference_update_path_callback(const clap_plugin *plugin,  clap_id resource_id, const char *path) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_file_reference_update_path(resource_id,path);
  }

  static bool clap_file_reference_save_resources_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_file_reference_save_resources();
  }

  // clap.gui

  static bool clap_gui_create_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_gui_create();
  }

  static void clap_gui_destroy_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_gui_destroy();
  }

  static void clap_gui_set_scale_callback(const clap_plugin *plugin, double scale) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_gui_set_scale(scale);
  }

  static bool clap_gui_get_size_callback(const clap_plugin *plugin, uint32_t *width, uint32_t *height) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_gui_get_size(width,height);
  }

  static bool clap_gui_can_resize_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_gui_can_resize();
  }

  static void clap_gui_round_size_callback(const clap_plugin *plugin, uint32_t *width, uint32_t *height) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_gui_round_size(width,height);
  }

  static bool clap_gui_set_size_callback(const clap_plugin *plugin, uint32_t width, uint32_t height) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_gui_set_size(width,height);
  }

  static void clap_gui_show_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_gui_show();
  }

  static void clap_gui_hide_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_gui_hide();
  }

  // clap.gui-x11

  static bool clap_gui_x11_attach_callback(const clap_plugin * plugin, const char* display_name, unsigned long window) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_gui_x11_attach(display_name,window);
  }

  // clap.latency

  static uint32_t clap_latency_get_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_latency_get();
  }

  // clap.midi-mappings

  static uint32_t clap_midi_mappings_count_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_midi_mappings_count();
  }

  static bool clap_midi_mappings_get_callback(const clap_plugin *plugin, uint32_t index, clap_midi_mapping *mapping) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_midi_mappings_get(index,mapping);
  }

  // clap.note-name

  static uint32_t clap_note_name_count_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_note_name_count();
  }

  static bool clap_note_name_get_callback(const clap_plugin *plugin, uint32_t index, clap_note_name *note_name) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_note_name_get(index,note_name);
  }

  // clap.note-ports


  static uint32_t clap_note_ports_count_callback(const clap_plugin *plugin, bool is_input) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_note_ports_count(is_input);
  }

  static bool clap_note_ports_get_callback(const clap_plugin* plugin, uint32_t index, bool is_input, clap_note_port_info *info) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_note_ports_get(index,is_input,info);
  }

  // clap.params

  static uint32_t clap_params_count_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_params_count();
  }

  static bool clap_params_get_info_callback(const clap_plugin *plugin, int32_t param_index, clap_param_info *param_info) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_params_get_info(param_index,param_info);
  }

  static bool clap_params_get_value_callback(const clap_plugin *plugin, clap_id param_id, double *value) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_params_get_value(param_id,value);
  }

  static bool clap_params_value_to_text_callback(const clap_plugin *plugin, clap_id param_id, double value, char *display, uint32_t size) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_params_value_to_text(param_id,value,display,size);
  }

  static bool clap_params_text_to_value_callback(const clap_plugin *plugin, clap_id param_id, const char* display, double* value) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_params_text_to_value(param_id,display,value);
  }

  static void clap_params_flush_callback(const clap_plugin* plugin, const clap_event_list *input_parameter_changes, const clap_event_list *output_parameter_changes) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_params_flush(input_parameter_changes,output_parameter_changes);
  }

  // clap.preset-load

  static bool clap_preset_load_from_file_callback(const clap_plugin *plugin, const char *path) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_preset_load_from_file(path);
  }

  // clap.quick-controls

  static uint32_t clap_quick_controls_count_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_quick_controls_count();
  }

  static bool clap_quick_controls_get_callback(const clap_plugin *plugin, uint32_t page_index, clap_quick_controls_page *page) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_quick_controls_get(page_index,page);
  }

  static void clap_quick_controls_select_callback(const clap_plugin *plugin, clap_id page_id) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_quick_controls_select(page_id);
  }

  static clap_id clap_quick_controls_get_selected_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_quick_controls_get_selected();
  }

  // clap.render

  static void clap_render_set_callback(const clap_plugin *plugin, clap_plugin_render_mode mode) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_render_set(mode);
  }

  // clap.state

  static bool clap_state_save_callback(const clap_plugin *plugin, clap_ostream *stream) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_state_save(stream);
  }

  static bool clap_state_load_callback(const clap_plugin *plugin, clap_istream *stream) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_state_load(stream);
  }

  // clap.thread-pool

  static void clap_thread_pool_exec_callback(const clap_plugin *plugin, uint32_t task_index) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_thread_pool_exec(task_index);
  }

  // clap.timer-support

  static void clap_timer_support_on_timer_callback(const clap_plugin *plugin, clap_id timer_id) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_timer_support_on_timer(timer_id);
  }

  // clap.track-info

  static void clap_track_info_changed_callback(const clap_plugin *plugin) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    instance->clap_track_info_changed();
  }

  // clap.vst2-convert

  static int32_t clap_vst2_convert_get_vst2_plugin_id_callback(const clap_plugin *plugin, uint32_t* vst2_plugin_id, char* name, uint32_t name_size) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_vst2_convert_get_vst2_plugin_id(vst2_plugin_id,name,name_size);
  }

  static bool clap_vst2_convert_restore_vst2_state_callback(const clap_plugin *plugin, clap_istream *stream) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_vst2_convert_restore_vst2_state(stream);
  }

  static bool clap_vst2_convert_convert_normalized_value_callback(const clap_plugin *plugin, uint32_t vst2_param_id, double vst2_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_vst2_convert_convert_normalized_value(vst2_param_id,vst2_normalized_value,clap_param_id,clap_plain_value);
  }

  static bool clap_vst2_convert_convert_plain_value_callback(const clap_plugin *plugin, uint32_t vst2_param_id, double vst2_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    MIP_ClapPluginInstance* instance = (MIP_ClapPluginInstance*)plugin->plugin_data;
    return instance->clap_vst2_convert_convert_plain_value(vst2_param_id,vst2_plain_value,clap_param_id,clap_plain_value);
  }

  // clap.vst3-convert

  static void clap_vst3_convert_get_vst3_plugin_id_callback(const clap_plugin *plugin, uint8_t *vst3_plugin_uuid) {
  }

  static bool clap_vst3_convert_restore_vst3_state_callback(const clap_plugin *plugin, clap_istream *stream) {
    return false;
  }

  static bool clap_vst3_convert_convert_normalized_value_callback(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_normalized_value, clap_id* clap_param_id, double* clap_plain_value) {
    return false;
  }

  static bool clap_vst3_convert_convert_plain_value_callback(const clap_plugin *plugin, uint32_t vst3_param_id, double vst3_plain_value, clap_id* clap_param_id, double* clap_plain_value) {
    return false;
  }

//------------------------------
private: // extension structs
//------------------------------

  struct clap_plugin_audio_ports_config MClapAudioPortsConfig = {
    clap_audio_ports_config_count_callback,
    clap_audio_ports_config_get_callback,
    clap_audio_ports_config_select_callback
  };

  struct clap_plugin_audio_ports MClapAudioPorts =  {
    clap_audio_ports_count_callback,
    clap_audio_ports_get_callback
  };

  struct clap_plugin_check_for_update MClapCheckForUpdate = {
    clap_check_for_update_check_callback
  };

  struct clap_plugin_event_filter MClapEventFilter = {
    clap_event_filter_accepts_callback
  };

  struct clap_plugin_fd_support MClapFdSupport = {
    clap_fd_support_on_fd_callback
  };

  struct clap_plugin_file_reference MClapFileReference = {
    clap_file_reference_count_callback,
    clap_file_reference_get_callback,
    clap_file_reference_get_hash_callback,
    clap_file_reference_update_path_callback,
    clap_file_reference_save_resources_callback
  };

  struct clap_plugin_gui MClapGui = {
    clap_gui_create_callback,
    clap_gui_destroy_callback,
    clap_gui_set_scale_callback,
    clap_gui_get_size_callback,
    clap_gui_can_resize_callback,
    clap_gui_round_size_callback,
    clap_gui_set_size_callback,
    clap_gui_show_callback,
    clap_gui_hide_callback
  };

  struct clap_plugin_gui_x11 MClapGuiX11 = {
    clap_gui_x11_attach_callback
  };

  struct clap_plugin_latency MClapLatency = {
    clap_latency_get_callback
  };

  struct clap_plugin_midi_mappings MClapMidiMappings = {
    clap_midi_mappings_count_callback,
    clap_midi_mappings_get_callback
  };

  struct clap_plugin_note_name MClapNoteName = {
    clap_note_name_count_callback,
    clap_note_name_get_callback
  };

  struct clap_plugin_note_ports MClapNotePorts = {
    clap_note_ports_count_callback,
    clap_note_ports_get_callback
  };

  struct clap_plugin_params MClapParams = {
    clap_params_count_callback,
    clap_params_get_info_callback,
    clap_params_get_value_callback,
    clap_params_value_to_text_callback,
    clap_params_text_to_value_callback,
    clap_params_flush_callback
  };

  struct clap_plugin_preset_load MClapPresetLoad = {
    clap_preset_load_from_file_callback
  };

  struct clap_plugin_quick_controls MClapQuickControls = {
    clap_quick_controls_count_callback,
    clap_quick_controls_get_callback,
    clap_quick_controls_select_callback,
    clap_quick_controls_get_selected_callback
  };

  struct clap_plugin_render MClapRender = {
    clap_render_set_callback
  };

  struct clap_plugin_state MClapState = {
    clap_state_save_callback,
    clap_state_load_callback
  };

  struct clap_plugin_timer_support MClapTimerSupport = {
    clap_timer_support_on_timer_callback
  };

  struct clap_plugin_thread_pool MClapThreadPool = {
    clap_thread_pool_exec_callback
  };

  struct clap_plugin_track_info MClapTrackInfo = {
    clap_track_info_changed_callback
  };

  struct clap_plugin_vst2_convert MClapVst2Convert = {
    clap_vst2_convert_get_vst2_plugin_id_callback,
    clap_vst2_convert_restore_vst2_state_callback,
    clap_vst2_convert_convert_normalized_value_callback,
    clap_vst2_convert_convert_plain_value_callback
  };

  struct clap_plugin_vst3_convert MClapVst3Convert = {
    clap_vst3_convert_get_vst3_plugin_id_callback,
    clap_vst3_convert_restore_vst3_state_callback,
    clap_vst3_convert_convert_normalized_value_callback,
    clap_vst3_convert_convert_plain_value_callback
  };

};

//----------------------------------------------------------------------
#endif



