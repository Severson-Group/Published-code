/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CVCR_Controller.h
 *
 * Code generated for Simulink model 'CVCR_Controller'.
 *
 * Model version                  : 2.430
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Thu Aug 11 17:54:33 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CVCR_Controller_h_
#define RTW_HEADER_CVCR_Controller_h_
#ifndef CVCR_Controller_COMMON_INCLUDES_
#define CVCR_Controller_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* CVCR_Controller_COMMON_INCLUDES_ */

#include "CVCR_Controller_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Delay1_DSTATE;                /* '<S16>/Delay1' */
  real_T Delay2_DSTATE;                /* '<S5>/Delay2' */
  real_T Delay2_DSTATE_d;              /* '<S16>/Delay2' */
  real_T Delay2_DSTATE_o;              /* '<S11>/Delay2' */
  real_T Delay2_DSTATE_dh;             /* '<S10>/Delay2' */
  real_T Delay2_DSTATE_e;              /* '<S12>/Delay2' */
  real_T Delay2_DSTATE_b;              /* '<S8>/Delay2' */
  real_T Delay1_DSTATE_b;              /* '<Root>/Delay1' */
  real_T Delay2_DSTATE_h;              /* '<Root>/Delay2' */
  real_T Delay2_DSTATE_n;              /* '<S9>/Delay2' */
  real_T Delay2_DSTATE_be;             /* '<S13>/Delay2' */
  real_T Delay2_DSTATE_i;              /* '<S3>/Delay2' */
} DW_CVCR_Controller_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Iq_c;                         /* '<Root>/RPM_C' */
  real_T Theta;                        /* '<Root>/WeInst' */
  real_T Id_c;                         /* '<Root>/Vdcc' */
  real_T Iu;                           /* '<Root>/B_-1_-1' */
  real_T Iv;                           /* '<Root>/B_-1_-1' */
  real_T Iw;                           /* '<Root>/B_-1_-1' */
  real_T Vdc;                          /* '<Root>/B_-1_-1' */
  real_T RPM_C;                        /* '<Root>/B_-1_-1' */
  real_T WeInst;                       /* '<Root>/B_-1_-1' */
  real_T Vdcc;                         /* '<Root>/B_-1_-1' */
  real_T Iload;                        /* '<Root>/B_-1_-1' */
} ExtU_CVCR_Controller_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T H11;                          /* '<Root>/H11' */
  real_T H21;                          /* '<Root>/H21' */
  real_T H31;                          /* '<Root>/H31' */
  real_T LOG_CVCR_Iq;                  /* '<Root>/LOG_CVCR_Iq' */
  real_T LOG_CVCR_Id;                  /* '<Root>/LOG_CVCR_Id' */
  real_T LOG_CVCR_Iq_c;                /* '<Root>/LOG_CVCR_Iq_c' */
  real_T LOG_CVCR_Id_c;                /* '<Root>/LOG_CVCR_Id_c' */
  real_T LOG_CVCR_Iu;                  /* '<Root>/LOG_CVCR_Iu' */
  real_T LOG_CVCR_Iv;                  /* '<Root>/LOG_CVCR_Iv' */
  real_T LOG_CVCR_Iw;                  /* '<Root>/LOG_CVCR_Iw' */
  real_T LOG_CVCR_Vdc;                 /* '<Root>/LOG_CVCR_Vdc' */
  real_T LOG_CVCR_Vu_c;                /* '<Root>/LOG_CVCR_Vu_c' */
  real_T LOG_CVCR_Vv_c;                /* '<Root>/LOG_CVCR_Vv_c' */
  real_T LOG_CVCR_Vw_c;                /* '<Root>/LOG_CVCR_Vw_c' */
  real_T LOG_CVCR_Theta;               /* '<Root>/LOG_CVCR_Theta' */
  real_T LOG_CVCR_Wefilt;              /* '<Root>/LOG_CVCR_Wefilt' */
  real_T LOG_CVCR_Wmfilt;              /* '<Root>/LOG_CVCR_Wmfilt' */
  real_T LOG_CVCR_Vd_c;                /* '<Root>/LOG_CVCR_Vd_c' */
  real_T LOG_CVCR_Vq_c;                /* '<Root>/LOG_CVCR_Vq_c' */
  real_T LOG_CVCR_Vdcfilt;             /* '<Root>/LOG_CVCR_Vdcfilt' */
  real_T LOG_CVCR_Vphasor;             /* '<Root>/LOG_CVCR_Vphasor' */
  real_T LOG_CVCR_RPM_c;               /* '<Root>/LOG_CVCR_RPM_c' */
  real_T LOG_CVCR_RPM;                 /* '<Root>/LOG_CVCR_RPM' */
  real_T LOG_CVCR_ElecP;               /* '<Root>/LOG_CVCR_ElecP' */
  real_T LOG_CVCR_ElecQ;               /* '<Root>/LOG_CVCR_ElecQ' */
  real_T LOG_CVCR_Iload;               /* '<Root>/LOG_CVCR_Iload' */
  real_T LOG_CVCR_Ibuckff;             /* '<Root>/LOG_CVCR_Ibuckff' */
  real_T LOG_CVCR_Vbuck_c;             /* '<Root>/LOG_CVCR_Vbuck_c' */
  real_T LOG_CVCR_H41;                 /* '<Root>/LOG_CVCR_H41' */
  real_T LOG_CVCR_PowerBuck;           /* '<Root>/LOG_CVCR_PowerBuck' */
  real_T LOG_CVCR_Iloadfilt;           /* '<Root>/LOG_CVCR_Iloadfilt' */
  real_T LOG_CVCR_signal29;            /* '<Root>/LOG_CVCR_signal29' */
  real_T LOG_CVCR_signal30;            /* '<Root>/LOG_CVCR_signal30' */
  real_T LOG_CVCR_signal31;            /* '<Root>/LOG_CVCR_signal31' */
  real_T LOG_CVCR_signal32;            /* '<Root>/LOG_CVCR_signal32' */
  real_T LOG_CVCR_signal33;            /* '<Root>/LOG_CVCR_signal33' */
  real_T LOG_CVCR_signal34;            /* '<Root>/LOG_CVCR_signal34' */
  real_T LOG_CVCR_signal35;            /* '<Root>/LOG_CVCR_signal35' */
  real_T H41;                          /* '<Root>/H41' */
} ExtY_CVCR_Controller_T;

/* Real-time Model Data Structure */
struct tag_RTM_CVCR_Controller_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_CVCR_Controller_T CVCR_Controller_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_CVCR_Controller_T CVCR_Controller_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_CVCR_Controller_T CVCR_Controller_Y;

/* Model entry point functions */
extern void CVCR_Controller_initialize(void);
extern void CVCR_Controller_step(void);
extern void CVCR_Controller_terminate(void);

/* Real-time Model object */
extern RT_MODEL_CVCR_Controller_T *const CVCR_Controller_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S21>/Data Type Duplicate' : Unused code path elimination
 * Block '<S21>/Data Type Propagation' : Unused code path elimination
 * Block '<S22>/Data Type Duplicate' : Unused code path elimination
 * Block '<S22>/Data Type Propagation' : Unused code path elimination
 * Block '<S23>/Data Type Duplicate' : Unused code path elimination
 * Block '<S23>/Data Type Propagation' : Unused code path elimination
 * Block '<S4>/Delay2' : Unused code path elimination
 * Block '<S4>/Gain1' : Unused code path elimination
 * Block '<S4>/Gain3' : Unused code path elimination
 * Block '<S4>/Sum2' : Unused code path elimination
 * Block '<S6>/Data Type Duplicate' : Unused code path elimination
 * Block '<S6>/Data Type Propagation' : Unused code path elimination
 * Block '<S7>/Delay1' : Unused code path elimination
 * Block '<S7>/Delay3' : Unused code path elimination
 * Block '<S7>/Gain11' : Unused code path elimination
 * Block '<S7>/Gain4' : Unused code path elimination
 * Block '<S7>/Gain5' : Unused code path elimination
 * Block '<S7>/Gain6' : Unused code path elimination
 * Block '<S7>/Kt1' : Unused code path elimination
 * Block '<S7>/Saturation2' : Unused code path elimination
 * Block '<S7>/Sum11' : Unused code path elimination
 * Block '<S7>/Sum12' : Unused code path elimination
 * Block '<S7>/Sum6' : Unused code path elimination
 * Block '<S7>/Sum7' : Unused code path elimination
 * Block '<S7>/Sum9' : Unused code path elimination
 * Block '<Root>/Gain4' : Eliminated nontunable gain of 1
 * Block '<Root>/Gain5' : Eliminated nontunable gain of 1
 * Block '<Root>/Gain6' : Eliminated nontunable gain of 1
 * Block '<S19>/1-Sine, 0-THPWM' : Unused code path elimination
 * Block '<S25>/Constant1' : Unused code path elimination
 * Block '<S25>/Constant3' : Unused code path elimination
 * Block '<S25>/Divide' : Unused code path elimination
 * Block '<S25>/Divide1' : Unused code path elimination
 * Block '<S25>/Gain1' : Unused code path elimination
 * Block '<S25>/Sum1' : Unused code path elimination
 * Block '<S25>/Sum2' : Unused code path elimination
 * Block '<S25>/Sum3' : Unused code path elimination
 * Block '<S25>/Sum4' : Unused code path elimination
 * Block '<S25>/Trigonometric Function6' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'CVCR_Controller'
 * '<S1>'   : 'CVCR_Controller/CVCR'
 * '<S2>'   : 'CVCR_Controller/PWM Generation'
 * '<S3>'   : 'CVCR_Controller/RPM_c LPF'
 * '<S4>'   : 'CVCR_Controller/RPM_c LPF1'
 * '<S5>'   : 'CVCR_Controller/RPM_c LPF4'
 * '<S6>'   : 'CVCR_Controller/Saturation Dynamic'
 * '<S7>'   : 'CVCR_Controller/Speed Control'
 * '<S8>'   : 'CVCR_Controller/Vdc LPF'
 * '<S9>'   : 'CVCR_Controller/Vdc LPF1'
 * '<S10>'  : 'CVCR_Controller/Vdc LPF3'
 * '<S11>'  : 'CVCR_Controller/Vdc LPF4'
 * '<S12>'  : 'CVCR_Controller/Vdc LPF5'
 * '<S13>'  : 'CVCR_Controller/Vdc LPF6'
 * '<S14>'  : 'CVCR_Controller/a-d Scalar Invariant'
 * '<S15>'  : 'CVCR_Controller/a-d Scalar Invariant1'
 * '<S16>'  : 'CVCR_Controller/CVCR/PI Zoh Series Xcouple'
 * '<S17>'  : 'CVCR_Controller/CVCR/d-a Scalar Invariant'
 * '<S18>'  : 'CVCR_Controller/PWM Generation/Duty Cycle Calc'
 * '<S19>'  : 'CVCR_Controller/PWM Generation/Modulation'
 * '<S20>'  : 'CVCR_Controller/PWM Generation/Duty Cycle Calc/Avg Switch Model'
 * '<S21>'  : 'CVCR_Controller/PWM Generation/Duty Cycle Calc/Avg Switch Model/Saturation Dynamic'
 * '<S22>'  : 'CVCR_Controller/PWM Generation/Duty Cycle Calc/Avg Switch Model/Saturation Dynamic1'
 * '<S23>'  : 'CVCR_Controller/PWM Generation/Duty Cycle Calc/Avg Switch Model/Saturation Dynamic2'
 * '<S24>'  : 'CVCR_Controller/PWM Generation/Modulation/SinePWM'
 * '<S25>'  : 'CVCR_Controller/PWM Generation/Modulation/THPWM'
 */
#endif                                 /* RTW_HEADER_CVCR_Controller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
