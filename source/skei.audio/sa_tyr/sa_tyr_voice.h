#ifndef sa_tyr_voice_incuded
#define sa_tyr_voice_incuded
//----------------------------------------------------------------------

#include "base/mip.h"
#include "audio/mip_audio_math.h"

#include "audio/old/filters/mip_allpass_filter.h"
#include "audio/old/filters/mip_rc_filter.h"
#include "audio/old/filters/mip_svf_filter.h"
#include "audio/old/processing/mip_delay.h"
#include "audio/old/synthesis/mip_oscillator2.h"
#include "audio/old/modulation/mip_envelope.h"
#include "audio/old/waveforms/mip_polyblep_waveform.h"

//#include "audio/old/mip_voice.h"
#include "plugin/mip_voice_manager.h"

//----------------------------------------------------------------------

#include "sa_tyr_osc.h"
#include "sa_tyr_res.h"

//----------

#define PARAM_SMOOTH_FACTOR   (1.0 / 200.0)
#define MOD_SMOOTH_FACTOR     (1.0 / 200.0)
#define EXPR_SMOOTH_FACTOR    (1.0 / 500.0)
#define PARMOD_SMOOTH_FACTOR  (1.0 / 200.0)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_voice {

//------------------------------
private:
//------------------------------

  //__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)
  //float MSliceBuffer[MIP_AUDIO_SLICE_SIZE] = {0};
  float MSliceBuffer[MIP_AUDIO_MAX_BLOCK_SIZE] = {0};

  MIP_VoiceContext* MContext      = nullptr;
  uint32_t          MVoiceIndex   = 0;

//------------------------------
private:
//------------------------------

//  float*            MVoiceBuffer    = nullptr;
//  uint32_t MIndex = 0;

  double*                MParameters     = nullptr;
  double*                MModulations    = nullptr;
  double*                MParMod         = nullptr;
  double*                MParModTargets  = nullptr;
  double*                MParModFactors  = nullptr;

  sa_tyr_osc<float>     MOscillator1    = {};
  sa_tyr_osc<float>     MOscillator2    = {};
  sa_tyr_res<float>     MResonator1     = {};
  sa_tyr_res<float>     MResonator2     = {};

  MIP_Envelope<float>   MAmpEnvelope    = {};
  MIP_SvfFilter         MFilter         = {};
  MIP_RcFilter<float>   MO1InputFilter  = {};
  MIP_RcFilter<float>   MO2InputFilter  = {};
  MIP_RcFilter<float>   MR1InputFilter  = {};
  MIP_RcFilter<float>   MR2InputFilter  = {};

  int32_t           note_key        = -1;
  double            note_onvel      = 0.0;
  double            note_offvel     = 0.0;

  // note expressions

  double                 note_vol        = 0.0;
  double                 note_pan        = 0.0;
  double                 note_tuning     = 0.0;
  double                 note_vibr       = 0.0;
  double                 note_expr       = 0.0;
  double                 note_bright     = 0.0;
  double                 note_press      = 0.0;

  double                 note_vol_tgt    = 0.0;
  double                 note_pan_tgt    = 0.0;
  double                 note_tuning_tgt = 0.0;
  double                 note_vibr_tgt   = 0.0;
  double                 note_expr_tgt   = 0.0;
  double                 note_bright_tgt = 0.0;
  double                 note_press_tgt  = 0.0;

  // processing

  float             O1              = 0.0;
  float             O2              = 0.0;
  float             R1              = 0.0;
  float             R2              = 0.0;

//------------------------------
public:
//------------------------------

  sa_tyr_voice() {
  }

  //----------

  ~sa_tyr_voice() {
    if (MParameters)        free(MParameters);
    if (MModulations)       free(MModulations);
    if (MParMod)            free(MParMod);
    if (MParModTargets)     free(MParModTargets);
    if (MParModFactors)     free(MParModFactors);
  }

//------------------------------
public:
//------------------------------

//  void setup(uint32_t AIndex, MIP_VoiceContext* AContext) {
//    MVoiceIndex = AIndex;
//    MContext = AContext;

  void init(uint32_t AIndex, MIP_VoiceContext* AContext) {
    MVoiceIndex = AIndex;
    MContext = AContext;

//    // define in voice_manager
//    #ifdef MIP_VOICE_ADD_TO_BUFFER
//      MVoiceBuffer = AContext->voicebuffer;
//    #else
//      MVoiceBuffer = AContext->voicebuffer + (AIndex * MIP_VOICE_MAX_FRAMESIZE);
//    #endif

    MOscillator1.setSampleRate(MContext->samplerate);
    MOscillator2.setSampleRate(MContext->samplerate);
    MResonator1.setSampleRate(MContext->samplerate);
    MResonator2.setSampleRate(MContext->samplerate);
    MAmpEnvelope.setSampleRate(MContext->samplerate);

    MO1InputFilter.setSampleRate(MContext->samplerate);
    MO2InputFilter.setSampleRate(MContext->samplerate);
    MR1InputFilter.setSampleRate(MContext->samplerate);
    MR2InputFilter.setSampleRate(MContext->samplerate);

    uint32_t num = MContext->parameters->size();

    MParameters         = (double*)malloc(num * sizeof(double));
    MModulations        = (double*)malloc(num * sizeof(double));
    MParMod             = (double*)malloc(num * sizeof(double));
    MParModTargets      = (double*)malloc(num * sizeof(double));
    MParModFactors      = (double*)malloc(num * sizeof(double));

    for (uint32_t i=0; i<num; i++) {
      MParameters[i]        = 0.0;
      MModulations[i]       = 0.0;
      MParMod[i]            = 0.0;
      MParModTargets[i]     = 0.0;
      MParModFactors[i]     = PARMOD_SMOOTH_FACTOR;
    }

  }

  //----------

  double getEnvLevel() {
    return MAmpEnvelope.getValue();
  }

//------------------------------
public:
//------------------------------

  //todo: reset modulations

  uint32_t noteOn(int32_t key, double velocity) {
    //MIP_Print("%i velocity %.3f\n",MVoiceIndex,velocity);

    note_key          = key;
    note_onvel        = velocity;

    note_press        = 0.0;
    note_bright       = 0.0;
    note_tuning       = 0.0;
    note_vol          = 0.0;
    note_pan          = 0.0;
    note_vibr         = 0.0;
    note_expr         = 0.0;

    note_press_tgt    = 0.0;
    note_bright_tgt   = 0.0;
    note_tuning_tgt   = 0.0;
    note_vol_tgt      = 0.0;
    note_pan_tgt      = 0.0;
    note_vibr_tgt     = 0.0;
    note_expr_tgt     = 0.0;

    MOscillator1.restart(true,true);//false); // reset phase, random phase
    MOscillator2.restart(true,true);//false);
    MResonator1.restart();
    MResonator2.restart();
    MAmpEnvelope.noteOn();
    MFilter.reset();

    O1 = 0.0;
    O2 = 0.0;
    R1 = 0.0;
    R2 = 0.0;

    for (uint32_t i=0; i<PARAM_COUNT; i++) {
      MModulations[i] = 0.0;
      MParMod[i] = MParameters[i];
      MParModTargets[i] = MParameters[i];
    }

    return MIP_VOICE_PLAYING;

  }

  //----------

  uint32_t noteOff(int32_t key, double velocity) {
    //MIP_Print("%i velocity %.3f\n",MIndex,velocity);
    note_offvel = velocity;
    MAmpEnvelope.noteOff();
    return MIP_VOICE_RELEASED;
  }

  //----------

  void noteChoke(int32_t expr, double amount) {
    //MIP_Print("%i\n",MIndex);
  }

  //----------

  void noteExpression(int32_t expr, double value) {
  }

  //----------

  void volume(double value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_vol = value;
    note_vol_tgt = value;
  }

  //----------

  void pan(double value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_pan = value;
    note_pan_tgt = value;
  }

  //----------

  void tuning(double value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_tuning = value;
    note_tuning_tgt = value;
  }

  //----------

  void vibrato(double value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_vibr = value;
    note_vibr_tgt = value;
  }

  //----------

  void expression(double value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_expr = value;
    note_expr_tgt = value;
  }

  //----------

  void brightness(double value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_bright = (value * 2.0) - 1.0;
    note_bright_tgt = (value * 2.0) - 1.0;
  }

  //----------

  void pressure(double value) {
    //MIP_Print("%i value %.3f\n",MIndex,value);
    //note_press = value;
    note_press_tgt = value;
  }

  //----------

  void parameter(uint32_t index, double value) {
    //MIP_Print("index %i value %f\n",index,value);

    MParameters[index] = value;
    //MParameterTargets[index] = value;

    //MIP_Print("---\n");
    switch(index) {
      case PAR_ENV1_ATT:      MAmpEnvelope.setAttack(value*5);      break;
      case PAR_ENV1_DEC:      MAmpEnvelope.setDecay(value*5);       break;
      case PAR_ENV1_SUS:      MAmpEnvelope.setSustain(value*value); break;
      case PAR_ENV1_REL:      MAmpEnvelope.setRelease(value*5);     break;
      case PAR_OSC1_TYPE:     MOscillator1.setType(value);          break;
      case PAR_OSC1_PM_TYPE:  MOscillator1.setPhaseModType(value);  break;
      case PAR_OSC1_WM_TYPE:  MOscillator1.setWaveModType(value);   break;
      case PAR_OSC2_TYPE:     MOscillator2.setType(value);          break;
      case PAR_OSC2_PM_TYPE:  MOscillator2.setPhaseModType(value);  break;
      case PAR_OSC2_WM_TYPE:  MOscillator2.setWaveModType(value);   break;
      case PAR_FLT1_TYPE:     MFilter.setMode(value);               break;
    }
  }

  //----------

  void modulation(uint32_t index, double value) {
    //MIP_Print("%i index %i value %.3f\n",MIndex,index,value);

    MModulations[index] = value;
    //MModulationTargets[index] = value;

  }

//------------------------------
public:
//------------------------------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {

    //MIP_Assert(note_key >= 0);

//    float* input0 = MContext->process->audio_inputs->data32[0];
//    float* input1 = MContext->process->audio_inputs->data32[1];
//    float* output = MVoiceBuffer + AOffset;

    //MIP_Print("voice index %i buffer %p offset %i length %i\n",MVoiceIndex,MContext->buffer,AOffset,ALength);

    float* input0 = MContext->process_context->process->audio_inputs->data32[0];
    float* input1 = MContext->process_context->process->audio_inputs->data32[1];

    float* output = MContext->buffer;
    output += (MVoiceIndex * MIP_AUDIO_MAX_BLOCK_SIZE);
    output += AOffset;

    float* output_buffer = output;
    output = MSliceBuffer;

    //------------------------------
    // per sample
    //------------------------------

    for (uint32_t i=0; i<ALength; i++) {

      //------------------------------
      // prepare
      //------------------------------

      // smooth parameters / modulations
      // not all parameters are 0..1 !!!

      for (uint32_t j=0; j<PARAM_COUNT; j++) {
        //MParModTargets[j] = MIP_Clamp( (MParameters[j] + MModulations[j]), 0, 1);

        // MParameters = 0x80000000

        //double a = MParModTargets[j];
        //double b = MParameters[j];
        //double c = MModulations[j];

        MParModTargets[j] = MParameters[j] + MModulations[j];

        //T diff = abs( MParModTargets[j] - MParMod[j] );
        double diff = MParModTargets[j] - MParMod[j];
        if (abs(diff) < 0.001) MParMod[j] = MParModTargets[j];
        else MParMod[j] += (diff * MParModFactors[j]);
      }

      // smooth note expression values

      note_vol    += (note_vol_tgt    - note_vol)    * EXPR_SMOOTH_FACTOR;
      note_pan    += (note_pan_tgt    - note_pan)    * EXPR_SMOOTH_FACTOR;
      note_tuning += (note_tuning_tgt - note_tuning) * EXPR_SMOOTH_FACTOR;
      note_vibr   += (note_vibr_tgt   - note_vibr)   * EXPR_SMOOTH_FACTOR;
      note_expr   += (note_expr_tgt   - note_expr)   * EXPR_SMOOTH_FACTOR;
      note_bright += (note_bright_tgt - note_bright) * EXPR_SMOOTH_FACTOR;
      note_press  += (note_press_tgt  - note_press)  * EXPR_SMOOTH_FACTOR;

      // prepare osc 1

      float o1_pitch  = ((int)MParMod[PAR_OSC1_OCT]  * 12.0)
                  + ((int)MParMod[PAR_OSC1_SEMI] * 1.0 )
                  + (     MParMod[PAR_OSC1_CENT]       );

      float osc1_hz = MIP_NoteToHz(note_key + note_tuning + o1_pitch);
      osc1_hz = MIP_Clamp(osc1_hz,20,20000);
      MOscillator1.setFrequency(osc1_hz);

      float o1_shape      = MIP_Clamp( MParMod[PAR_OSC1_SHAPE], 0, 1 );
      float o1_width      = MIP_Clamp( MParMod[PAR_OSC1_WIDTH], 0, 1 );
      float o1_phase_mod  = MIP_Clamp( MParMod[PAR_OSC1_PM_AMOUNT], 0, 1 );
      float o1_wave_mod   = MIP_Clamp( MParMod[PAR_OSC1_WM_AMOUNT], 0, 1 );

      MOscillator1.setShape(o1_shape);
      MOscillator1.setWidth(o1_width);
      MOscillator1.setPhaseModAmount(o1_phase_mod);
      MOscillator1.setWaveModAmount(o1_wave_mod);

      uint32_t o1_mode = MParMod[PAR_OSC1_TYPE];
      o1_mode = MIP_Clamp( o1_mode, 0, SA_TYR_OSC_TYPE_COUNT - 1);
      MOscillator1.setType(o1_mode);

      float o1s = MIP_Clamp( MParMod[PAR_OSC1_IN_S], 0, 1 );
      o1s   = 1.0 - o1s;
      o1s   = (o1s * o1s * o1s * o1s);
      MO1InputFilter.setWeight(o1s);

      // prepare osc 2

      float o2_pitch  = ((int)MParMod[PAR_OSC2_OCT]  * 12.0)
                  + ((int)MParMod[PAR_OSC2_SEMI] * 1.0 )
                  + (     MParMod[PAR_OSC2_CENT]       );

      float osc2_hz = MIP_NoteToHz(note_key + note_tuning + o2_pitch);
      osc2_hz = MIP_Clamp(osc2_hz,20,20000);
      MOscillator2.setFrequency(osc2_hz);

      float o2_shape      = MIP_Clamp( MParMod[PAR_OSC2_SHAPE], 0, 1 );
      float o2_width      = MIP_Clamp( MParMod[PAR_OSC2_WIDTH], 0, 1 );
      float o2_wave_mod   = MIP_Clamp( MParMod[PAR_OSC2_WM_AMOUNT], 0, 1 );
      float o2_phase_mod  = MIP_Clamp( MParMod[PAR_OSC2_PM_AMOUNT], 0, 1 );

      MOscillator2.setShape(o2_shape);
      MOscillator2.setWidth(o2_width);
      MOscillator2.setWaveModAmount(o2_wave_mod);
      MOscillator2.setPhaseModAmount(o2_phase_mod);

      uint32_t o2_mode = MParMod[PAR_OSC2_TYPE];
      o2_mode = MIP_Clamp( o2_mode, 0, SA_TYR_OSC_TYPE_COUNT - 1);
      MOscillator2.setType(o2_mode);

      float o2s = MIP_Clamp( MParMod[PAR_OSC2_IN_S], 0, 1 );
      o2s = 1.0 - o2s;
      o2s = (o2s * o2s * o2s * o2s);
      MO2InputFilter.setWeight(o2s);

      // prepare res 1

      float r1_pitch  = ((int)MParMod[PAR_RES1_OCT]  * 12.0)
                  + ((int)MParMod[PAR_RES1_SEMI] * 1.0 )
                  + (     MParMod[PAR_RES1_CENT]       );

      float res1_hz = MIP_NoteToHz(note_key + note_tuning + r1_pitch);
      res1_hz = MIP_Clamp(res1_hz,20,20000);
      MResonator1.setHz(res1_hz);

      float r1_shape  = MIP_Clamp( MParMod[PAR_RES1_SHAPE], 0, 1 );
      float r1_fb     = MIP_Clamp( MParMod[PAR_RES1_FB], 0, 1 );
      float r1_damp   = MIP_Clamp( MParMod[PAR_RES1_DAMP], 0, 1 );
      float r1_rough  = MIP_Clamp( MParMod[PAR_RES1_ROUGH], 0, 1 );

      MResonator1.setShape(r1_shape);
      MResonator1.setFeedback(r1_fb);
      MResonator1.setDamp(r1_damp);
      MResonator1.setRough(r1_rough);

      float r1s = MIP_Clamp( MParMod[PAR_RES1_IN_S], 0, 1 );
      r1s = 1.0 - r1s;
      r1s = (r1s * r1s * r1s * r1s);
      MR1InputFilter.setWeight(r1s);

      uint32_t r1_mode = MParMod[PAR_RES1_TYPE];
      r1_mode = MIP_Clamp( r1_mode, 0, SA_TYR_RES_TYPE_COUNT - 1);
      MResonator1.setMode(r1_mode);

      float imp1 = MIP_Clamp( MParMod[PAR_RES1_IN_I], 0, 1 );
      //imp1 = (imp1 * imp1 * imp1 * imp1 * imp1);
      MResonator1.setImpulse(imp1);

      float r1_spd = 1.0 - MIP_Clamp(MParMod[PAR_RES1_SPEED],0,1);
      r1_spd = (r1_spd * r1_spd * r1_spd) * 10000;
      MResonator1.setSpeed( r1_spd );

      // prepare res 2

      float r2_pitch  = ((int)MParMod[PAR_RES2_OCT]  * 12.0)
                  + ((int)MParMod[PAR_RES2_SEMI] * 1.0 )
                  + (     MParMod[PAR_RES2_CENT]       );

      float res2_hz = MIP_NoteToHz(note_key + note_tuning + r2_pitch);
      res2_hz = MIP_Clamp(res2_hz,20,20000);
      MResonator2.setHz(res2_hz);

      float r2_shape  = MIP_Clamp( MParMod[PAR_RES2_SHAPE], 0, 1 );
      float r2_fb     = MIP_Clamp( MParMod[PAR_RES2_FB], 0, 1 );
      float r2_damp   = MIP_Clamp( MParMod[PAR_RES2_DAMP], 0, 1 );
      float r2_rough  = MIP_Clamp( MParMod[PAR_RES2_ROUGH], 0, 1 );

      MResonator2.setShape(r2_shape);
      MResonator2.setFeedback(r2_fb);
      MResonator2.setDamp(r2_damp);
      MResonator2.setRough(r2_rough);

      float r2s = MIP_Clamp( MParMod[PAR_RES2_IN_S], 0, 1 );
      r2s = 1.0 - r2s;
      r2s = (r2s * r2s * r2s * r2s);
      MR2InputFilter.setWeight(r2s);

      uint32_t r2_mode = MParMod[PAR_RES2_TYPE];
      r2_mode = MIP_Clamp( r2_mode, 0, SA_TYR_RES_TYPE_COUNT - 1);
      MResonator2.setMode( r2_mode );

      float imp2 = MIP_Clamp( MParMod[PAR_RES2_IN_I], 0,1 );;
      //imp2 = (imp2 * imp2 * imp2 * imp2 * imp2);
      MResonator2.setImpulse(imp2);

      float r2_spd = 1.0 - MIP_Clamp( MParMod[PAR_RES2_SPEED], 0,1 );
      r2_spd = (r2_spd * r2_spd * r2_spd) * 10000;
      MResonator2.setSpeed( r2_spd );

      //------------------------------
      // processing
      //------------------------------

      float in0 = *input0++;
      float in1 = *input1++;
      float input  = (in0 + in1) * 0.5;

      // sum inputs

      // osc 1

      float rnd = MIP_RandomSigned();

      float o1_in = O1    * MIP_Clamp( MParMod[PAR_OSC1_IN_O1], 0, 1 )
              + O2    * MIP_Clamp( MParMod[PAR_OSC1_IN_O2], 0, 1 )
              + R1    * MIP_Clamp( MParMod[PAR_OSC1_IN_R1], 0, 1 )
              + R2    * MIP_Clamp( MParMod[PAR_OSC1_IN_R2], 0, 1 )
              + rnd   * MIP_Clamp( MParMod[PAR_OSC1_IN_N],  0, 1 )
              + input * MIP_Clamp( MParMod[PAR_OSC1_IN_A],  0, 1 );

      o1_in = MO1InputFilter.process(o1_in);
      //O1 = MOscillator1.process(o1_in);

      // osc 2

      rnd = MIP_RandomSigned();

      float o2_in = O1    * MIP_Clamp( MParMod[PAR_OSC2_IN_O1], 0, 1 )
              + O2    * MIP_Clamp( MParMod[PAR_OSC2_IN_O2], 0, 1 )
              + R1    * MIP_Clamp( MParMod[PAR_OSC2_IN_R1], 0, 1 )
              + R2    * MIP_Clamp( MParMod[PAR_OSC2_IN_R2], 0, 1 )
              + rnd   * MIP_Clamp( MParMod[PAR_OSC2_IN_N],  0, 1 )
              + input * MIP_Clamp( MParMod[PAR_OSC2_IN_A],  0, 1 );

      o2_in = MO2InputFilter.process(o2_in);
      //O2 = MOscillator2.process(o2_in);

      // res 1

      rnd = MIP_RandomSigned();

      float r1_in = O1    * MIP_Clamp( MParMod[PAR_RES1_IN_O1], 0, 1 )
              + O2    * MIP_Clamp( MParMod[PAR_RES1_IN_O2], 0, 1 )
              + R1    * MIP_Clamp( MParMod[PAR_RES1_IN_R1], 0, 1 )
              + R2    * MIP_Clamp( MParMod[PAR_RES1_IN_R2], 0, 1 )
              + rnd   * MIP_Clamp( MParMod[PAR_RES1_IN_N],  0, 1 )
              + input * MIP_Clamp( MParMod[PAR_RES1_IN_A],  0, 1 );

      r1_in = MR1InputFilter.process(r1_in);
      //R1 = MResonator1.process(r1_in);

      // res 2

      rnd = MIP_RandomSigned();

      float r2_in = O1    * MIP_Clamp( MParMod[PAR_RES2_IN_O1], 0, 1 )
              + O2    * MIP_Clamp( MParMod[PAR_RES2_IN_O2], 0, 1 )
              + R1    * MIP_Clamp( MParMod[PAR_RES2_IN_R1], 0, 1 )
              + R2    * MIP_Clamp( MParMod[PAR_RES2_IN_R2], 0, 1 )
              + rnd   * MIP_Clamp( MParMod[PAR_RES2_IN_N],  0, 1 )
              + input * MIP_Clamp( MParMod[PAR_RES2_IN_A],  0, 1 );

      r2_in = MR2InputFilter.process(r2_in);
      //R2 = MResonator2.process(r2_in);

      // process

      O1 = MOscillator1.process(o1_in);
      O2 = MOscillator2.process(o2_in);
      R1 = MResonator1.process(r1_in);
      R2 = MResonator2.process(r2_in);

      float out = (O1 * MIP_Clamp( MParMod[PAR_MASTER_OSC1_OUT], 0, 1 ))
            + (O2 * MIP_Clamp( MParMod[PAR_MASTER_OSC2_OUT], 0, 1 ))
            + (R1 * MIP_Clamp( MParMod[PAR_MASTER_RES1_OUT], 0, 1 ))
            + (R2 * MIP_Clamp( MParMod[PAR_MASTER_RES2_OUT], 0, 1 ));

      //------------------------------
      // post
      //------------------------------

      // filter

      float fr = MIP_Clamp( MParMod[PAR_FLT1_FREQ], 0, 1 );
      float bw = MIP_Clamp( MParMod[PAR_FLT1_RES],  0, 1 );

      MFilter.setBW(1.0 - bw);
      MFilter.setFreq(fr * fr);
      out = MFilter.process(out);

      // env, velocity, press..

      float amp = note_onvel + note_press;
      amp = MIP_Clamp( amp, 0, 1 );

      amp *= MAmpEnvelope.process();
      out *= amp;

      // out

      //#ifdef MIP_VOICE_ADD_TO_BUFFER
      //  *output++ += out;
      //#else
        *output++ = out;
      //#endif

      // prepare for next sample
      //

    }

    //memcpy(output_buffer,MSliceBuffer,MIP_AUDIO_SLICE_SIZE*sizeof(float));
    memcpy(output_buffer,MSliceBuffer,ALength*sizeof(float));

    //------------------------------
    //
    //------------------------------

    uint32_t stage = MAmpEnvelope.getStage();

    if (stage == MIP_ENVELOPE_FINISHED) return MIP_VOICE_FINISHED;
    else return AState;

  }

  //----------

  /*
    will the optimizer pick up the const buffersize across the methods?
    we can also do a kind of inverse scheme, where we always calculate
    a certain size, but copy only the part we need, and keep track of how
    many are left.. next process call, if we need more than the currently
    available samples, we calculate another const size buffer (we need two
    scratch buffers), and copy the end of bufer 1 and the start of bufer 2..
    too much hassle?
    or two separate functions..
    tempated? macros? think.. think..
  */

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,MIP_AUDIO_SLICE_SIZE);
  }

};

//----------------------------------------------------------------------
#endif
