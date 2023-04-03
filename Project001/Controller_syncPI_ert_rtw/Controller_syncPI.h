/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Controller_syncPI.h
 *
 * Code generated for Simulink model 'Controller_syncPI'.
 *
 * Model version                  : 5.91
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Tue Mar 28 16:57:20 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Controller_syncPI_h_
#define RTW_HEADER_Controller_syncPI_h_
#ifndef Controller_syncPI_COMMON_INCLUDES_
#define Controller_syncPI_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Controller_syncPI_COMMON_INCLUDES_ */

#include "Controller_syncPI_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  creal_T Delay_DSTATE;                /* '<S8>/Delay' */
  creal_T Delay1_DSTATE;               /* '<S8>/Delay1' */
  creal_T Delay1_DSTATE_j;             /* '<S17>/Delay1' */
  creal_T Delay_DSTATE_k;              /* '<S17>/Delay' */
  creal_T Delay_DSTATE_b;              /* '<S18>/Delay' */
  creal_T Delay1_DSTATE_g;             /* '<S18>/Delay1' */
  creal_T Delay4_DSTATE;               /* '<S13>/Delay4' */
  real_T Delay1_DSTATE_f;              /* '<S20>/Delay1' */
  real_T Delay_DSTATE_m;               /* '<S20>/Delay' */
  real_T Delay_DSTATE_m2;              /* '<S14>/Delay' */
  real_T Delay_DSTATE_c;               /* '<S21>/Delay' */
  real_T Delay1_DSTATE_c;              /* '<S21>/Delay1' */
  real_T Delay1_DSTATE_cl;             /* '<Root>/Delay1' */
  real_T Delay_DSTATE_d;               /* '<Root>/Delay' */
  real_T Delay5_DSTATE;                /* '<Root>/Delay5' */
  real_T DiscreteTransferFcn_states;   /* '<S5>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_states_o; /* '<S14>/Discrete Transfer Fcn' */
  real_T count;                        /* '<Root>/StateTransition' */
  uint8_T is_active_c3_Controller_syncPI;/* '<Root>/StateTransition' */
  uint8_T is_c3_Controller_syncPI;     /* '<Root>/StateTransition' */
  boolean_T icLoad;                    /* '<S20>/Delay' */
  boolean_T icLoad_h;                  /* '<S21>/Delay1' */
  boolean_T icLoad_e;                  /* '<Root>/Delay1' */
  boolean_T icLoad_e0;                 /* '<Root>/Delay' */
} DW_Controller_syncPI_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T I_uvw_fb[3];                  /* '<Root>/V_dc' */
  real_T V_dc;                         /* '<Root>/CMD_I_d' */
  real_T CMD_I_d;                      /* '<Root>/CMD_I_d' */
  real_T CMD_I_q;                      /* '<Root>/CMD_I_q' */
  real_T CMD_theta_m;                  /* '<Root>/CMD_theta_m' */
  real_T CMD_V_d;                      /* '<Root>/CMD_V_d' */
  real_T CMD_V_q;                      /* '<Root>/CMD_V_q' */
  real_T CMD_speed;                    /* '<Root>/CMD_speed' */
  real_T PARAMS_cc_enable;             /* '<Root>/PARAMS_cc_enable' */
  real_T PARAMS_drv_enable;            /* '<Root>/PARAMS_drv_enable' */
  real_T PARAMS_I_max;                 /* '<Root>/PARAMS_I_max' */
  real_T PARAMS_speed_control;         /* '<Root>/PARAMS_speed_control' */
  real_T PARAMS_theta_m_init;          /* '<Root>/PARAMS_theta_m_init' */
  real_T PARAMS_omega_m_init;          /* '<Root>/PARAMS_omega_m_init' */
  real_T PARAMS_RPM_limit;             /* '<Root>/PARAMS_RPM_limit' */
  real_T PARAMS_selfsensing_enable;    /* '<Root>/B_-1_-1' */
} ExtU_Controller_syncPI_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T dutyU;                        /* '<Root>/dutyU' */
  real_T dutyV;                        /* '<Root>/dutyV' */
  real_T dutyW;                        /* '<Root>/dutyW' */
  real_T PWM_enable;                   /* '<Root>/PWM_enable' */
  real_T LOG_signals_i_dq_cmd[2];      /* '<Root>/LOG_signals_i_dq_cmd' */
  real_T LOG_signals_i_dq_fb[2];       /* '<Root>/LOG_signals_i_dq_fb' */
  real_T LOG_signals_v_uvw[3];         /* '<Root>/LOG_signals_v_uvw' */
  real_T LOG_signals_i_uvw[3];         /* '<Root>/LOG_signals_i_uvw' */
  real_T LOG_signals_v_dq[2];          /* '<Root>/LOG_signals_v_dq' */
  real_T LOG_signals_omega;            /* '<Root>/LOG_signals_omega' */
  real_T LOG_signals_omega_filt;       /* '<Root>/LOG_signals_omega_filt' */
  real_T LOG_signals_theta_m_hat;      /* '<Root>/LOG_signals_theta_m_hat' */
  real_T LOG_signals_omega_m_hat;      /* '<Root>/LOG_signals_omega_m_hat' */
  real_T LOG_bemfo_i_dq[2];            /* '<Root>/LOG_bemfo_i_dq' */
  real_T LOG_bemfo_v_dq[2];            /* '<Root>/LOG_bemfo_v_dq' */
  real_T LOG_bemfo_emf[2];             /* '<Root>/LOG_bemfo_emf' */
} ExtY_Controller_syncPI_T;

/* Real-time Model Data Structure */
struct tag_RTM_Controller_syncPI_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_Controller_syncPI_T Controller_syncPI_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Controller_syncPI_T Controller_syncPI_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Controller_syncPI_T Controller_syncPI_Y;

/* Model entry point functions */
extern void Controller_syncPI_initialize(void);
extern void Controller_syncPI_step(void);
extern void Controller_syncPI_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Controller_syncPI_T *const Controller_syncPI_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S6>/Data Type Duplicate' : Unused code path elimination
 * Block '<S6>/Data Type Propagation' : Unused code path elimination
 * Block '<S12>/Data Type Duplicate' : Unused code path elimination
 * Block '<S12>/Data Type Propagation' : Unused code path elimination
 * Block '<S14>/Constant' : Unused code path elimination
 * Block '<S14>/Gain5' : Unused code path elimination
 * Block '<S14>/Mod' : Unused code path elimination
 * Block '<S14>/Scope' : Unused code path elimination
 * Block '<Root>/Gain10' : Eliminated nontunable gain of 1
 * Block '<Root>/Gain8' : Eliminated nontunable gain of 1
 * Block '<S10>/Gain' : Eliminated nontunable gain of 1
 * Block '<S10>/Gain1' : Eliminated nontunable gain of 1
 * Block '<S14>/Gain8' : Eliminated nontunable gain of 1
 * Block '<S8>/1: CVCR 0: SFPI' : Unused code path elimination
 * Block '<S8>/Constant' : Unused code path elimination
 * Block '<S8>/Delay2' : Unused code path elimination
 * Block '<S8>/Gain4' : Unused code path elimination
 * Block '<S8>/Gain5' : Unused code path elimination
 * Block '<S8>/Gain6' : Unused code path elimination
 * Block '<S8>/Gain7' : Unused code path elimination
 * Block '<S8>/Gain8' : Unused code path elimination
 * Block '<S8>/Gain9' : Unused code path elimination
 * Block '<S8>/Product1' : Unused code path elimination
 * Block '<S8>/Real-Imag to Complex2' : Unused code path elimination
 * Block '<S8>/Sum3' : Unused code path elimination
 * Block '<S8>/Sum4' : Unused code path elimination
 * Block '<S8>/Sum5' : Unused code path elimination
 * Block '<S8>/Sum6' : Unused code path elimination
 * Block '<S8>/Sum7' : Unused code path elimination
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
 * '<Root>' : 'Controller_syncPI'
 * '<S1>'   : 'Controller_syncPI/F3ph_to_2ph5'
 * '<S2>'   : 'Controller_syncPI/Fdq_to_2ph'
 * '<S3>'   : 'Controller_syncPI/MATLAB Function'
 * '<S4>'   : 'Controller_syncPI/MATLAB Function1'
 * '<S5>'   : 'Controller_syncPI/Motion Controller'
 * '<S6>'   : 'Controller_syncPI/Saturation Dynamic'
 * '<S7>'   : 'Controller_syncPI/StateTransition'
 * '<S8>'   : 'Controller_syncPI/Subsystem'
 * '<S9>'   : 'Controller_syncPI/Subsystem1'
 * '<S10>'  : 'Controller_syncPI/Synchronous frame Estimator'
 * '<S11>'  : 'Controller_syncPI/speed_calc'
 * '<S12>'  : 'Controller_syncPI/Subsystem1/Saturation Dynamic'
 * '<S13>'  : 'Controller_syncPI/Synchronous frame Estimator/Current observer sync'
 * '<S14>'  : 'Controller_syncPI/Synchronous frame Estimator/Motion Observer '
 * '<S15>'  : 'Controller_syncPI/Synchronous frame Estimator/Current observer sync/F2ph_to_dq'
 * '<S16>'  : 'Controller_syncPI/Synchronous frame Estimator/Current observer sync/F2ph_to_dq1'
 * '<S17>'  : 'Controller_syncPI/Synchronous frame Estimator/Current observer sync/Plant'
 * '<S18>'  : 'Controller_syncPI/Synchronous frame Estimator/Current observer sync/complex compensator'
 * '<S19>'  : 'Controller_syncPI/Synchronous frame Estimator/Motion Observer /Compute error'
 * '<S20>'  : 'Controller_syncPI/Synchronous frame Estimator/Motion Observer /Integrator'
 * '<S21>'  : 'Controller_syncPI/Synchronous frame Estimator/Motion Observer /integrator_2'
 */
#endif                                 /* RTW_HEADER_Controller_syncPI_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
