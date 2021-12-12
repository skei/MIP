#ifndef mip_clap_utils_included
#define mip_clap_utils_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* MIP_ClapEventNames[] = {
  "CLAP_EVENT_NOTE_ON",
  "CLAP_EVENT_NOTE_OFF",
  "CLAP_EVENT_NOTE_END",
  "CLAP_EVENT_NOTE_CHOKE",
  "CLAP_EVENT_NOTE_EXPRESSION",
  "CLAP_EVENT_NOTE_MASK",
  "CLAP_EVENT_PARAM_VALUE",
  "CLAP_EVENT_PARAM_MOD",
  "CLAP_EVENT_TRANSPORT",
  "CLAP_EVENT_MIDI",
  "CLAP_EVENT_MIDI_SYSEX"
};

const char* MIP_ClapNoteExpressionNames[] = {
  "CLAP_NOTE_EXPRESSION_VOLUME",
  "CLAP_NOTE_EXPRESSION_PAN",
  "CLAP_NOTE_EXPRESSION_TUNING",
  "CLAP_NOTE_EXPRESSION_VIBRATO",
  "CLAP_NOTE_EXPRESSION_BRIGHTNESS",
  "CLAP_NOTE_EXPRESSION_BREATH",
  "CLAP_NOTE_EXPRESSION_PRESSURE",
  "CLAP_NOTE_EXPRESSION_TIMBRE"
};

const char* MIP_ClapParamValueFlagNames[] = {
  "CLAP_EVENT_PARAM_BEGIN_ADJUST",
  "CLAP_EVENT_PARAM_END_ADJUST",
  "CLAP_EVENT_PARAM_SHOULD_RECORD"
};

const char* MIP_ClapTransportFlagNames[] = {
  "CLAP_TRANSPORT_HAS_TEMPO",
  "CLAP_TRANSPORT_HAS_BEATS_TIMELINE",
  "CLAP_TRANSPORT_HAS_SECONDS_TIMELINE",
  "CLAP_TRANSPORT_HAS_TIME_SIGNATURE",
  "CLAP_TRANSPORT_IS_PLAYING",
  "CLAP_TRANSPORT_IS_RECORDING",
  "CLAP_TRANSPORT_IS_LOOP_ACTIVE",
  "CLAP_TRANSPORT_IS_WITHIN_PRE_ROLL"
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void MIP_PrintClapEvent(const clap_event* event) {
  //#ifdef MIP_DEBUG_CLAP

  switch (event->type) {
    case CLAP_EVENT_NOTE_ON:
      MIP_Print("CLAP_EVENT_NOTE_ON\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- port_index %i\n",event->note.port_index);
      MIP_Print("- key %i\n",event->note.key);
      MIP_Print("- channel %i\n",event->note.channel);
      MIP_Print("- velocity %.3f\n",event->note.velocity);
      break;
    case CLAP_EVENT_NOTE_OFF:
      MIP_Print("CLAP_EVENT_NOTE_OFF\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- port_index %i\n",event->note.port_index);
      MIP_Print("- key %i\n",event->note.key);
      MIP_Print("- channel %i\n",event->note.channel);
      MIP_Print("- velocity %.3f\n",event->note.velocity);
      break;
    case CLAP_EVENT_NOTE_END:
      MIP_Print("CLAP_EVENT_NOTE_END\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- port_index %i\n",event->note.port_index);
      MIP_Print("- key %i\n",event->note.key);
      MIP_Print("- channel %i\n",event->note.channel);
      MIP_Print("- velocity %.3f\n",event->note.velocity);
      break;
    case CLAP_EVENT_NOTE_CHOKE:
      MIP_Print("CLAP_EVENT_NOTE_CHOKE\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- port_index %i\n",event->note.port_index);
      MIP_Print("- key %i\n",event->note.key);
      MIP_Print("- channel %i\n",event->note.channel);
      MIP_Print("- velocity %.3f\n",event->note.velocity);
      break;
    case CLAP_EVENT_NOTE_EXPRESSION:
      MIP_Print("CLAP_EVENT_NOTE_EXPRESSION\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- expression_id %i (%s)\n",event->note_expression.expression_id,MIP_ClapNoteExpressionNames[event->note_expression.expression_id]);
      MIP_Print("- port_index %i\n",event->note_expression.port_index);
      MIP_Print("- key %i\n",event->note_expression.key);
      MIP_Print("- channel %i\n",event->note_expression.channel);
      MIP_Print("- value %.3f\n",event->note_expression.value);
      break;

    case CLAP_EVENT_NOTE_MASK:
      MIP_Print("CLAP_EVENT_NOTE_MASK\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- port_index %i\n",event->note_mask.port_index);
      MIP_Print("- note_mask %i (%11b)\n",event->note_mask.note_mask,event->note_mask.note_mask);
      MIP_Print("- root_note %i\n",event->note_mask.root_note);
      break;
    case CLAP_EVENT_PARAM_VALUE:
      MIP_Print("CLAP_EVENT_PARAM_VALUE\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- cookie %p\n",event->param_value.cookie);
      MIP_Print("- param_id %i\n",event->param_value.param_id);
      MIP_Print("- port_index %i\n",event->param_value.port_index);
      MIP_Print("- key %i\n",event->param_value.key);
      MIP_Print("- channel %i\n",event->param_value.channel);
      MIP_Print("- flags %i (%08b)\n",event->param_value.flags,event->param_value.flags);
      MIP_Print("- value %.3f\n",event->param_value.value);
      break;
    case CLAP_EVENT_PARAM_MOD:
      MIP_Print("CLAP_EVENT_PARAM_MOD\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- cookie %p\n",event->param_mod.cookie);
      MIP_Print("- param_id %i\n",event->param_mod.param_id);
      MIP_Print("- port_index %i\n",event->param_mod.port_index);
      MIP_Print("- key %i\n",event->param_mod.key);
      MIP_Print("- channel %i\n",event->param_mod.channel);
      MIP_Print("- amount %.3f\n",event->param_mod.amount);
      break;
    case CLAP_EVENT_TRANSPORT:
      MIP_Print("CLAP_EVENT_TRANSPORT\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- flags %i (%08b)\n",event->time_info.flags,event->time_info.flags,event->time_info.flags);
      MIP_Print("- song_pos_beats %i\n",event->time_info.song_pos_beats);
      MIP_Print("- song_pos_seconds %f\n",event->time_info.song_pos_seconds);
      MIP_Print("- tempo %.2f\n",event->time_info.tempo);
      MIP_Print("- tempo_inc %.2f\n",event->time_info.tempo_inc);
      MIP_Print("- bar_start %i\n",event->time_info.bar_start);
      MIP_Print("- bar_number %i\n",event->time_info.bar_number);
      MIP_Print("- loop_start_beats %i\n",event->time_info.loop_start_beats);
      MIP_Print("- loop_end_beats %i\n",event->time_info.loop_end_beats);
      MIP_Print("- loop_start_seconds %f\n",event->time_info.loop_start_seconds);
      MIP_Print("- loop_end_seconds %f\n",event->time_info.loop_end_seconds);
      MIP_Print("- tsig_num %i\n",event->time_info.tsig_num);
      MIP_Print("- tsig_denom %i\n",event->time_info.tsig_denom);
      break;
    case CLAP_EVENT_MIDI:
      MIP_Print("CLAP_EVENT_MIDI\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- port_index %i\n",event->midi.port_index);
      MIP_Print("- data %02x %02x %02x\n",event->midi.data[0],event->midi.data[1],event->midi.data[2]);
      break;
    case CLAP_EVENT_MIDI_SYSEX:
      MIP_Print("CLAP_EVENT_MIDI_SYSEX\n");
      MIP_Print("- time %i\n",event->time);
      MIP_Print("- port_index %i\n",event->midi_sysex.port_index);
      MIP_Print("- size %i\n",event->midi_sysex.size);
      MIP_Print("- buffer %p\n",event->midi_sysex.buffer);
      break;
    default:
      MIP_Print("unknown clap_event.type: %i\n",event->type);
      break;

  } // switch event type

  //#endif // MIP_DEBUG_CLAP
}

//----------------------------------------------------------------------
#endif
