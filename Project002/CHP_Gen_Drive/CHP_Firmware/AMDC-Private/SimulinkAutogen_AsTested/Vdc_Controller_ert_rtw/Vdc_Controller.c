/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Vdc_Controller.c
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

#include "Vdc_Controller.h"
#include "rtwtypes.h"

/* Block states (default storage) */
DW_Vdc_Controller_T Vdc_Controller_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Vdc_Controller_T Vdc_Controller_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Vdc_Controller_T Vdc_Controller_Y;

/* Real-time model */
static RT_MODEL_Vdc_Controller_T Vdc_Controller_M_;
RT_MODEL_Vdc_Controller_T *const Vdc_Controller_M = &Vdc_Controller_M_;

/* Model step function */
void Vdc_Controller_step(void)
{
  real_T rtb_I;
  real_T rtb_Sum1;

  /* Sum: '<Root>/Sum' incorporates:
   *  Inport generated from: '<Root>/Vdc'
   *  Inport generated from: '<Root>/Vdcc'
   */
  rtb_Sum1 = Vdc_Controller_U.Vdcc - Vdc_Controller_U.Vdc;

  /* Sum: '<Root>/Sum3' incorporates:
   *  Delay: '<Root>/Delay1'
   *  Delay: '<Root>/Delay2'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<Root>/Gain2'
   *  Gain: '<Root>/Gain5'
   *  Sum: '<Root>/Sum11'
   */
  Vdc_Controller_DW.Delay1_DSTATE += (1.5 * rtb_Sum1 - 100.0 *
    Vdc_Controller_DW.Delay2_DSTATE) * 0.0001;

  /* Sum: '<Root>/Sum1' incorporates:
   *  Delay: '<Root>/Delay1'
   */
  rtb_Sum1 += Vdc_Controller_DW.Delay1_DSTATE;

  /* Saturate: '<Root>/Saturation' */
  if (rtb_Sum1 > 0.0) {
    rtb_I = 0.0;
  } else if (rtb_Sum1 < -80.0) {
    rtb_I = -80.0;
  } else {
    rtb_I = rtb_Sum1;
  }

  /* End of Saturate: '<Root>/Saturation' */

  /* Gain: '<Root>/Gain7' incorporates:
   *  Gain: '<Root>/Gain16'
   *  Inport generated from: '<Root>/Ibuckff'
   *  Sum: '<Root>/Sum7'
   */
  Vdc_Controller_Y.LOG_buck_Vbuck_c = (Vdc_Controller_U.Ibuckff + -rtb_I) *
    8.925;

  /* Product: '<Root>/Divide4' incorporates:
   *  Inport generated from: '<Root>/Vdc'
   */
  Vdc_Controller_Y.LOG_buck_H41 = 1.0 / Vdc_Controller_U.Vdc *
    Vdc_Controller_Y.LOG_buck_Vbuck_c;

  /* Switch: '<S1>/Switch2' incorporates:
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/Constant7'
   *  RelationalOperator: '<S1>/LowerRelop1'
   *  RelationalOperator: '<S1>/UpperRelop'
   *  Switch: '<S1>/Switch'
   */
  if (Vdc_Controller_Y.LOG_buck_H41 > 1.0) {
    /* Product: '<Root>/Divide4' */
    Vdc_Controller_Y.LOG_buck_H41 = 1.0;
  } else if (Vdc_Controller_Y.LOG_buck_H41 < 0.0) {
    /* Product: '<Root>/Divide4' incorporates:
     *  Constant: '<Root>/Constant6'
     *  Switch: '<S1>/Switch'
     */
    Vdc_Controller_Y.LOG_buck_H41 = 0.0;
  }

  /* End of Switch: '<S1>/Switch2' */

  /* Sum: '<Root>/Sum2' incorporates:
   *  Delay: '<Root>/Delay2'
   */
  Vdc_Controller_DW.Delay2_DSTATE = rtb_Sum1 - rtb_I;

  /* Delay: '<S3>/Delay2' */
  Vdc_Controller_Y.LOG_buck_Vdcfilt = Vdc_Controller_DW.Delay2_DSTATE_l;

  /* Sum: '<S3>/Sum2' incorporates:
   *  Delay: '<S3>/Delay2'
   *  Gain: '<S3>/Gain1'
   *  Gain: '<S3>/Gain3'
   *  Inport generated from: '<Root>/Vdc'
   */
  Vdc_Controller_DW.Delay2_DSTATE_l = 0.0062634873752217546 *
    Vdc_Controller_U.Vdc + 0.99373651262477825 *
    Vdc_Controller_DW.Delay2_DSTATE_l;

  /* Outport generated from: '<Root>/LOG_Outport_2' incorporates:
   *  Delay: '<S2>/Delay2'
   */
  Vdc_Controller_Y.LOG_buck_Iloadfilt = Vdc_Controller_DW.Delay2_DSTATE_m;

  /* Outport generated from: '<Root>/LOG_Outport_2' incorporates:
   *  Delay: '<S2>/Delay2'
   *  Product: '<Root>/Divide3'
   */
  Vdc_Controller_Y.LOG_buck_PowerBuck = Vdc_Controller_DW.Delay2_DSTATE_m *
    Vdc_Controller_Y.LOG_buck_Vdcfilt;

  /* Sum: '<S2>/Sum2' incorporates:
   *  Delay: '<S2>/Delay2'
   *  Gain: '<S2>/Gain1'
   *  Gain: '<S2>/Gain3'
   *  Inport generated from: '<Root>/Iload'
   */
  Vdc_Controller_DW.Delay2_DSTATE_m = 0.0062634873752217546 *
    Vdc_Controller_U.Iload + 0.99373651262477825 *
    Vdc_Controller_DW.Delay2_DSTATE_m;

  /* Outport generated from: '<Root>/LOG_Outport_2' incorporates:
   *  Inport generated from: '<Root>/Iload'
   */
  Vdc_Controller_Y.LOG_buck_Iload = Vdc_Controller_U.Iload;

  /* Outport generated from: '<Root>/LOG_Outport_2' incorporates:
   *  Inport generated from: '<Root>/Ibuckff'
   */
  Vdc_Controller_Y.LOG_buck_Ibuckff = Vdc_Controller_U.Ibuckff;

  /* Outport generated from: '<Root>/LOG_Outport_2' incorporates:
   *  Inport generated from: '<Root>/Vdc'
   */
  Vdc_Controller_Y.LOG_buck_Vdc = Vdc_Controller_U.Vdc;

  /* Outport generated from: '<Root>/H41' */
  Vdc_Controller_Y.H41 = Vdc_Controller_Y.LOG_buck_H41;
}

/* Model initialize function */
void Vdc_Controller_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void Vdc_Controller_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
