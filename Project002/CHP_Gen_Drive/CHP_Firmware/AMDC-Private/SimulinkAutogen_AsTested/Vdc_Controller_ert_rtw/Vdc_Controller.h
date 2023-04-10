/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Vdc_Controller.h
 *
 * Code generated for Simulink model 'Vdc_Controller'.
 *
 * Model version                  : 2.343
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Thu Aug 11 08:26:46 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Vdc_Controller_h_
#define RTW_HEADER_Vdc_Controller_h_
#ifndef Vdc_Controller_COMMON_INCLUDES_
#define Vdc_Controller_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Vdc_Controller_COMMON_INCLUDES_ */

#include "Vdc_Controller_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Delay1_DSTATE;                /* '<Root>/Delay1' */
  real_T Delay2_DSTATE;                /* '<Root>/Delay2' */
  real_T Delay2_DSTATE_l;              /* '<S3>/Delay2' */
  real_T Delay2_DSTATE_m;              /* '<S2>/Delay2' */
} DW_Vdc_Controller_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Vdcc;                         /* '<Root>/Vdcc' */
  real_T Vdc;                          /* '<Root>/B_-1_-1' */
  real_T Iload;                        /* '<Root>/B_-1_-1' */
  real_T Ibuckff;                      /* '<Root>/B_-1_-1' */
} ExtU_Vdc_Controller_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T H41;                          /* '<Root>/H41' */
  real_T LOG_buck_Vdcfilt;             /* '<Root>/LOG_buck_Vdcfilt' */
  real_T LOG_buck_Iload;               /* '<Root>/LOG_buck_Iload' */
  real_T LOG_buck_Ibuckff;             /* '<Root>/LOG_buck_Ibuckff' */
  real_T LOG_buck_Vbuck_c;             /* '<Root>/LOG_buck_Vbuck_c' */
  real_T LOG_buck_Vdc;                 /* '<Root>/LOG_buck_Vdc' */
  real_T LOG_buck_H41;                 /* '<Root>/LOG_buck_H41' */
  real_T LOG_buck_PowerBuck;           /* '<Root>/LOG_buck_PowerBuck' */
  real_T LOG_buck_Iloadfilt;           /* '<Root>/LOG_buck_Iloadfilt' */
  real_T LOG_buck_signal9;             /* '<Root>/LOG_buck_signal9' */
  real_T LOG_buck_signal10;            /* '<Root>/LOG_buck_signal10' */
  real_T LOG_buck_signal11;            /* '<Root>/LOG_buck_signal11' */
  real_T LOG_buck_signal12;            /* '<Root>/LOG_buck_signal12' */
  real_T LOG_buck_signal13;            /* '<Root>/LOG_buck_signal13' */
  real_T LOG_buck_signal14;            /* '<Root>/LOG_buck_signal14' */
  real_T LOG_buck_signal15;            /* '<Root>/LOG_buck_signal15' */
  real_T LOG_buck_signal16;            /* '<Root>/LOG_buck_signal16' */
  real_T LOG_buck_signal17;            /* '<Root>/LOG_buck_signal17' */
  real_T LOG_buck_signal18;            /* '<Root>/LOG_buck_signal18' */
  real_T LOG_buck_signal19;            /* '<Root>/LOG_buck_signal19' */
  real_T LOG_buck_signal20;            /* '<Root>/LOG_buck_signal20' */
  real_T LOG_buck_signal21;            /* '<Root>/LOG_buck_signal21' */
  real_T LOG_buck_signal22;            /* '<Root>/LOG_buck_signal22' */
  real_T LOG_buck_signal23;            /* '<Root>/LOG_buck_signal23' */
  real_T LOG_buck_signal24;            /* '<Root>/LOG_buck_signal24' */
  real_T LOG_buck_signal25;            /* '<Root>/LOG_buck_signal25' */
} ExtY_Vdc_Controller_T;

/* Real-time Model Data Structure */
struct tag_RTM_Vdc_Controller_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_Vdc_Controller_T Vdc_Controller_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Vdc_Controller_T Vdc_Controller_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Vdc_Controller_T Vdc_Controller_Y;

/* Model entry point functions */
extern void Vdc_Controller_initialize(void);
extern void Vdc_Controller_step(void);
extern void Vdc_Controller_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Vdc_Controller_T *const Vdc_Controller_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Data Type Duplicate' : Unused code path elimination
 * Block '<S1>/Data Type Propagation' : Unused code path elimination
 * Block '<Root>/Gain4' : Eliminated nontunable gain of 1
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
 * '<Root>' : 'Vdc_Controller'
 * '<S1>'   : 'Vdc_Controller/Saturation Dynamic'
 * '<S2>'   : 'Vdc_Controller/Vdc LPF1'
 * '<S3>'   : 'Vdc_Controller/Vdc LPF3'
 */
#endif                                 /* RTW_HEADER_Vdc_Controller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
