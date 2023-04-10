/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CVCR_Controller.c
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

#include "CVCR_Controller.h"
#include <math.h>
#include "rtwtypes.h"

/* Block states (default storage) */
DW_CVCR_Controller_T CVCR_Controller_DW;

/* External inputs (root inport signals with default storage) */
ExtU_CVCR_Controller_T CVCR_Controller_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_CVCR_Controller_T CVCR_Controller_Y;

/* Real-time model */
static RT_MODEL_CVCR_Controller_T CVCR_Controller_M_;
RT_MODEL_CVCR_Controller_T *const CVCR_Controller_M = &CVCR_Controller_M_;

/* Model step function */
void CVCR_Controller_step(void)
{
  real_T rtb_Delay2_bw;
  real_T rtb_Sum;
  real_T rtb_Sum3_i;
  real_T rtb_fd_tmp;
  real_T rtb_fd_tmp_0;
  real_T rtb_fq_tmp;
  real_T rtb_fq_tmp_0;
  real_T rtb_fq_tmp_1;

  /* Trigonometry: '<S14>/Trigonometric Function2' incorporates:
   *  Constant: '<S14>/Constant5'
   *  Inport generated from: '<Root>/Theta'
   *  Sum: '<S14>/Sum7'
   *  Trigonometry: '<S15>/Trigonometric Function2'
   *  Trigonometry: '<S17>/Trigonometric Function4'
   */
  rtb_fq_tmp = sin(CVCR_Controller_U.Theta - 2.0943951023931953);

  /* Trigonometry: '<S14>/Trigonometric Function6' incorporates:
   *  Inport generated from: '<Root>/Theta'
   *  Trigonometry: '<S15>/Trigonometric Function6'
   *  Trigonometry: '<S17>/Trigonometric Function2'
   */
  rtb_fq_tmp_0 = sin(CVCR_Controller_U.Theta);

  /* Trigonometry: '<S14>/Trigonometric Function3' incorporates:
   *  Constant: '<S14>/Constant6'
   *  Inport generated from: '<Root>/Theta'
   *  Sum: '<S14>/Sum10'
   *  Trigonometry: '<S15>/Trigonometric Function3'
   *  Trigonometry: '<S17>/Trigonometric Function6'
   */
  rtb_fq_tmp_1 = sin(CVCR_Controller_U.Theta - 4.1887902047863905);

  /* Gain: '<S14>/Gain5' incorporates:
   *  Inport generated from: '<Root>/Iu'
   *  Inport generated from: '<Root>/Iv'
   *  Inport generated from: '<Root>/Iw'
   *  Product: '<S14>/Product2'
   *  Product: '<S14>/Product3'
   *  Product: '<S14>/Product8'
   *  Sum: '<S14>/Sum8'
   *  Trigonometry: '<S14>/Trigonometric Function2'
   *  Trigonometry: '<S14>/Trigonometric Function3'
   *  Trigonometry: '<S14>/Trigonometric Function6'
   */
  CVCR_Controller_Y.LOG_CVCR_Iq = (((0.0 - CVCR_Controller_U.Iu * rtb_fq_tmp_0)
    - CVCR_Controller_U.Iv * rtb_fq_tmp) - CVCR_Controller_U.Iw * rtb_fq_tmp_1) *
    0.66666666666666663;

  /* Gain: '<S16>/Gain7' incorporates:
   *  Inport generated from: '<Root>/Iq_c'
   *  Sum: '<S16>/Sum4'
   */
  rtb_Delay2_bw = (CVCR_Controller_U.Iq_c - CVCR_Controller_Y.LOG_CVCR_Iq) *
    0.69115038378975457;

  /* Trigonometry: '<S14>/Trigonometric Function4' incorporates:
   *  Constant: '<S14>/Constant2'
   *  Inport generated from: '<Root>/Theta'
   *  Sum: '<S14>/Sum3'
   *  Trigonometry: '<S15>/Trigonometric Function4'
   *  Trigonometry: '<S17>/Trigonometric Function3'
   */
  rtb_Sum = cos(CVCR_Controller_U.Theta - 2.0943951023931953);

  /* Trigonometry: '<S14>/Trigonometric Function1' incorporates:
   *  Inport generated from: '<Root>/Theta'
   *  Trigonometry: '<S15>/Trigonometric Function1'
   *  Trigonometry: '<S17>/Trigonometric Function1'
   */
  rtb_fd_tmp = cos(CVCR_Controller_U.Theta);

  /* Trigonometry: '<S14>/Trigonometric Function5' incorporates:
   *  Constant: '<S14>/Constant3'
   *  Inport generated from: '<Root>/Theta'
   *  Sum: '<S14>/Sum4'
   *  Trigonometry: '<S15>/Trigonometric Function5'
   *  Trigonometry: '<S17>/Trigonometric Function5'
   */
  rtb_fd_tmp_0 = cos(CVCR_Controller_U.Theta - 4.1887902047863905);

  /* Gain: '<S14>/Gain3' incorporates:
   *  Inport generated from: '<Root>/Iu'
   *  Inport generated from: '<Root>/Iv'
   *  Inport generated from: '<Root>/Iw'
   *  Product: '<S14>/Product1'
   *  Product: '<S14>/Product4'
   *  Product: '<S14>/Product5'
   *  Sum: '<S14>/Sum6'
   *  Trigonometry: '<S14>/Trigonometric Function1'
   *  Trigonometry: '<S14>/Trigonometric Function4'
   *  Trigonometry: '<S14>/Trigonometric Function5'
   */
  CVCR_Controller_Y.LOG_CVCR_Id = ((CVCR_Controller_U.Iu * rtb_fd_tmp +
    CVCR_Controller_U.Iv * rtb_Sum) + CVCR_Controller_U.Iw * rtb_fd_tmp_0) *
    0.66666666666666663;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Delay: '<S5>/Delay2'
   */
  CVCR_Controller_Y.LOG_CVCR_Wefilt = CVCR_Controller_DW.Delay2_DSTATE;

  /* Sum: '<S16>/Sum6' incorporates:
   *  Delay: '<S16>/Delay1'
   *  Delay: '<S5>/Delay2'
   *  Gain: '<S16>/Gain1'
   *  Gain: '<S16>/Gain2'
   *  Gain: '<S16>/Gain6'
   *  Product: '<S16>/Product2'
   *  Sum: '<S16>/Sum1'
   */
  CVCR_Controller_DW.Delay1_DSTATE += (0.000315 * CVCR_Controller_Y.LOG_CVCR_Id *
    CVCR_Controller_DW.Delay2_DSTATE + 87.684545454545443 * rtb_Delay2_bw) *
    0.0001;

  /* Sum: '<S16>/Sum2' incorporates:
   *  Delay: '<S16>/Delay1'
   */
  rtb_Delay2_bw += CVCR_Controller_DW.Delay1_DSTATE;

  /* Gain: '<S16>/Gain9' incorporates:
   *  Inport generated from: '<Root>/Id_c'
   *  Sum: '<S16>/Sum5'
   */
  rtb_Sum3_i = (CVCR_Controller_U.Id_c - CVCR_Controller_Y.LOG_CVCR_Id) *
    0.39584067435231396;

  /* Sum: '<S16>/Sum7' incorporates:
   *  Delay: '<S16>/Delay2'
   *  Delay: '<S5>/Delay2'
   *  Gain: '<S16>/Gain3'
   *  Gain: '<S16>/Gain4'
   *  Gain: '<S16>/Gain8'
   *  Product: '<S16>/Product3'
   *  Sum: '<S16>/Sum8'
   */
  CVCR_Controller_DW.Delay2_DSTATE_d += (153.1 * rtb_Sum3_i - 0.00055 *
    CVCR_Controller_Y.LOG_CVCR_Iq * CVCR_Controller_DW.Delay2_DSTATE) * 0.0001;

  /* Sum: '<S16>/Sum3' incorporates:
   *  Delay: '<S16>/Delay2'
   */
  rtb_Sum3_i += CVCR_Controller_DW.Delay2_DSTATE_d;

  /* Sum: '<S17>/Sum7' incorporates:
   *  Product: '<S17>/Product1'
   *  Product: '<S17>/Product2'
   */
  CVCR_Controller_Y.LOG_CVCR_Vv_c = rtb_Sum3_i * rtb_Sum - rtb_Delay2_bw *
    rtb_fq_tmp;

  /* Sum: '<S17>/Sum6' incorporates:
   *  Product: '<S17>/Product4'
   *  Product: '<S17>/Product5'
   */
  CVCR_Controller_Y.LOG_CVCR_Vu_c = rtb_Sum3_i * rtb_fd_tmp - rtb_Delay2_bw *
    rtb_fq_tmp_0;

  /* Sum: '<S17>/Sum11' incorporates:
   *  Product: '<S17>/Product3'
   *  Product: '<S17>/Product6'
   */
  CVCR_Controller_Y.LOG_CVCR_Vw_c = rtb_Sum3_i * rtb_fd_tmp_0 - rtb_Delay2_bw *
    rtb_fq_tmp_1;

  /* Gain: '<S15>/Gain5' incorporates:
   *  Product: '<S15>/Product2'
   *  Product: '<S15>/Product3'
   *  Product: '<S15>/Product8'
   *  Sum: '<S15>/Sum8'
   */
  CVCR_Controller_Y.LOG_CVCR_Vq_c = (((0.0 - CVCR_Controller_Y.LOG_CVCR_Vu_c *
    rtb_fq_tmp_0) - CVCR_Controller_Y.LOG_CVCR_Vv_c * rtb_fq_tmp) -
    CVCR_Controller_Y.LOG_CVCR_Vw_c * rtb_fq_tmp_1) * 0.66666666666666663;

  /* Gain: '<S15>/Gain3' incorporates:
   *  Product: '<S15>/Product1'
   *  Product: '<S15>/Product4'
   *  Product: '<S15>/Product5'
   *  Sum: '<S15>/Sum6'
   */
  CVCR_Controller_Y.LOG_CVCR_Vd_c = ((CVCR_Controller_Y.LOG_CVCR_Vu_c *
    rtb_fd_tmp + CVCR_Controller_Y.LOG_CVCR_Vv_c * rtb_Sum) +
    CVCR_Controller_Y.LOG_CVCR_Vw_c * rtb_fd_tmp_0) * 0.66666666666666663;

  /* Delay: '<S10>/Delay2' */
  rtb_Delay2_bw = CVCR_Controller_DW.Delay2_DSTATE_dh;

  /* Delay: '<S12>/Delay2' */
  CVCR_Controller_Y.LOG_CVCR_ElecP = CVCR_Controller_DW.Delay2_DSTATE_e;

  /* Sum: '<S12>/Sum2' incorporates:
   *  Delay: '<S10>/Delay2'
   *  Delay: '<S11>/Delay2'
   *  Delay: '<S12>/Delay2'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<S12>/Gain1'
   *  Gain: '<S12>/Gain3'
   *  Product: '<Root>/Product2'
   *  Product: '<Root>/Product3'
   *  Sum: '<Root>/Sum2'
   */
  CVCR_Controller_DW.Delay2_DSTATE_e = (CVCR_Controller_Y.LOG_CVCR_Vq_c *
    CVCR_Controller_DW.Delay2_DSTATE_o + CVCR_Controller_Y.LOG_CVCR_Vd_c *
    CVCR_Controller_DW.Delay2_DSTATE_dh) * 1.5 * 0.0062634873752217546 +
    0.99373651262477825 * CVCR_Controller_DW.Delay2_DSTATE_e;

  /* Delay: '<S8>/Delay2' */
  CVCR_Controller_Y.LOG_CVCR_Vdcfilt = CVCR_Controller_DW.Delay2_DSTATE_b;

  /* Product: '<Root>/Divide1' incorporates:
   *  Delay: '<S8>/Delay2'
   */
  CVCR_Controller_Y.LOG_CVCR_Ibuckff = CVCR_Controller_Y.LOG_CVCR_ElecP /
    CVCR_Controller_DW.Delay2_DSTATE_b;

  /* Sum: '<Root>/Sum1' incorporates:
   *  Inport generated from: '<Root>/Vdc'
   *  Inport generated from: '<Root>/Vdcc'
   */
  rtb_Sum3_i = CVCR_Controller_U.Vdcc - CVCR_Controller_U.Vdc;

  /* Sum: '<Root>/Sum6' incorporates:
   *  Delay: '<Root>/Delay1'
   *  Delay: '<Root>/Delay2'
   *  Gain: '<Root>/Gain7'
   *  Sum: '<Root>/Sum11'
   */
  CVCR_Controller_DW.Delay1_DSTATE_b += (rtb_Sum3_i -
    CVCR_Controller_DW.Delay2_DSTATE_h) * 0.0001;

  /* Sum: '<Root>/Sum3' incorporates:
   *  Delay: '<Root>/Delay1'
   */
  rtb_Sum3_i += CVCR_Controller_DW.Delay1_DSTATE_b;

  /* Saturate: '<Root>/Saturation' */
  if (rtb_Sum3_i > 0.0) {
    rtb_Sum = 0.0;
  } else if (rtb_Sum3_i < -80.0) {
    rtb_Sum = -80.0;
  } else {
    rtb_Sum = rtb_Sum3_i;
  }

  /* End of Saturate: '<Root>/Saturation' */

  /* Gain: '<Root>/Gain8' incorporates:
   *  Gain: '<Root>/Gain16'
   *  Gain: '<Root>/Gain9'
   *  Sum: '<Root>/Sum7'
   */
  CVCR_Controller_Y.LOG_CVCR_Vbuck_c = (-CVCR_Controller_Y.LOG_CVCR_Ibuckff +
    -rtb_Sum) * 8.925;

  /* Product: '<Root>/Divide4' incorporates:
   *  Inport generated from: '<Root>/Vdc'
   */
  CVCR_Controller_Y.LOG_CVCR_H41 = 1.0 / CVCR_Controller_U.Vdc *
    CVCR_Controller_Y.LOG_CVCR_Vbuck_c;

  /* Switch: '<S6>/Switch2' incorporates:
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/Constant7'
   *  RelationalOperator: '<S6>/LowerRelop1'
   *  RelationalOperator: '<S6>/UpperRelop'
   *  Switch: '<S6>/Switch'
   */
  if (CVCR_Controller_Y.LOG_CVCR_H41 > 1.0) {
    /* Product: '<Root>/Divide4' */
    CVCR_Controller_Y.LOG_CVCR_H41 = 1.0;
  } else if (CVCR_Controller_Y.LOG_CVCR_H41 < 0.0) {
    /* Product: '<Root>/Divide4' incorporates:
     *  Constant: '<Root>/Constant6'
     *  Switch: '<S6>/Switch'
     */
    CVCR_Controller_Y.LOG_CVCR_H41 = 0.0;
  }

  /* End of Switch: '<S6>/Switch2' */

  /* Sum: '<Root>/Sum4' incorporates:
   *  Delay: '<Root>/Delay2'
   */
  CVCR_Controller_DW.Delay2_DSTATE_h = rtb_Sum3_i - rtb_Sum;

  /* Sum: '<S8>/Sum2' incorporates:
   *  Delay: '<S8>/Delay2'
   *  Gain: '<S8>/Gain1'
   *  Gain: '<S8>/Gain3'
   *  Inport generated from: '<Root>/Vdc'
   */
  CVCR_Controller_DW.Delay2_DSTATE_b = 0.0062634873752217546 *
    CVCR_Controller_U.Vdc + 0.99373651262477825 *
    CVCR_Controller_DW.Delay2_DSTATE_b;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Delay: '<S9>/Delay2'
   */
  CVCR_Controller_Y.LOG_CVCR_Iloadfilt = CVCR_Controller_DW.Delay2_DSTATE_n;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Delay: '<S9>/Delay2'
   *  Product: '<Root>/Divide3'
   */
  CVCR_Controller_Y.LOG_CVCR_PowerBuck = CVCR_Controller_DW.Delay2_DSTATE_n *
    CVCR_Controller_Y.LOG_CVCR_Vdcfilt;

  /* Sum: '<S9>/Sum2' incorporates:
   *  Delay: '<S9>/Delay2'
   *  Gain: '<S9>/Gain1'
   *  Gain: '<S9>/Gain3'
   *  Inport generated from: '<Root>/Iload'
   */
  CVCR_Controller_DW.Delay2_DSTATE_n = 0.0062634873752217546 *
    CVCR_Controller_U.Iload + 0.99373651262477825 *
    CVCR_Controller_DW.Delay2_DSTATE_n;

  /* Product: '<S20>/Divide5' incorporates:
   *  Constant: '<S20>/Constant2'
   */
  rtb_Sum = 0.5 * CVCR_Controller_Y.LOG_CVCR_Vdcfilt;

  /* Product: '<S20>/Divide4' incorporates:
   *  Sum: '<S20>/Sum5'
   *  Switch: '<S19>/Switch7'
   */
  CVCR_Controller_Y.H11 = 1.0 / CVCR_Controller_Y.LOG_CVCR_Vdcfilt * (rtb_Sum +
    CVCR_Controller_Y.LOG_CVCR_Vu_c);

  /* Switch: '<S21>/Switch2' incorporates:
   *  Constant: '<S20>/Constant6'
   *  Constant: '<S20>/Constant7'
   *  RelationalOperator: '<S21>/LowerRelop1'
   *  RelationalOperator: '<S21>/UpperRelop'
   *  Switch: '<S21>/Switch'
   */
  if (CVCR_Controller_Y.H11 > 1.0) {
    /* Product: '<S20>/Divide4' incorporates:
     *  Outport generated from: '<Root>/H11'
     */
    CVCR_Controller_Y.H11 = 1.0;
  } else if (CVCR_Controller_Y.H11 < 0.0) {
    /* Product: '<S20>/Divide4' incorporates:
     *  Constant: '<S20>/Constant6'
     *  Outport generated from: '<Root>/H11'
     *  Switch: '<S21>/Switch'
     */
    CVCR_Controller_Y.H11 = 0.0;
  }

  /* End of Switch: '<S21>/Switch2' */

  /* Product: '<S20>/Divide6' incorporates:
   *  Sum: '<S20>/Sum4'
   *  Switch: '<S19>/Switch8'
   */
  CVCR_Controller_Y.H21 = 1.0 / CVCR_Controller_Y.LOG_CVCR_Vdcfilt * (rtb_Sum +
    CVCR_Controller_Y.LOG_CVCR_Vv_c);

  /* Switch: '<S22>/Switch2' incorporates:
   *  Constant: '<S20>/Constant6'
   *  Constant: '<S20>/Constant7'
   *  RelationalOperator: '<S22>/LowerRelop1'
   *  RelationalOperator: '<S22>/UpperRelop'
   *  Switch: '<S22>/Switch'
   */
  if (CVCR_Controller_Y.H21 > 1.0) {
    /* Product: '<S20>/Divide6' incorporates:
     *  Outport generated from: '<Root>/H21'
     */
    CVCR_Controller_Y.H21 = 1.0;
  } else if (CVCR_Controller_Y.H21 < 0.0) {
    /* Product: '<S20>/Divide6' incorporates:
     *  Constant: '<S20>/Constant6'
     *  Outport generated from: '<Root>/H21'
     *  Switch: '<S22>/Switch'
     */
    CVCR_Controller_Y.H21 = 0.0;
  }

  /* End of Switch: '<S22>/Switch2' */

  /* Product: '<S20>/Divide7' incorporates:
   *  Sum: '<S20>/Sum1'
   *  Switch: '<S19>/Switch9'
   */
  CVCR_Controller_Y.H31 = 1.0 / CVCR_Controller_Y.LOG_CVCR_Vdcfilt * (rtb_Sum +
    CVCR_Controller_Y.LOG_CVCR_Vw_c);

  /* Switch: '<S23>/Switch2' incorporates:
   *  Constant: '<S20>/Constant6'
   *  Constant: '<S20>/Constant7'
   *  RelationalOperator: '<S23>/LowerRelop1'
   *  RelationalOperator: '<S23>/UpperRelop'
   *  Switch: '<S23>/Switch'
   */
  if (CVCR_Controller_Y.H31 > 1.0) {
    /* Product: '<S20>/Divide7' incorporates:
     *  Outport generated from: '<Root>/H31'
     */
    CVCR_Controller_Y.H31 = 1.0;
  } else if (CVCR_Controller_Y.H31 < 0.0) {
    /* Product: '<S20>/Divide7' incorporates:
     *  Constant: '<S20>/Constant6'
     *  Outport generated from: '<Root>/H31'
     *  Switch: '<S23>/Switch'
     */
    CVCR_Controller_Y.H31 = 0.0;
  }

  /* End of Switch: '<S23>/Switch2' */

  /* Sum: '<S10>/Sum2' incorporates:
   *  Delay: '<S10>/Delay2'
   *  Gain: '<S10>/Gain1'
   *  Gain: '<S10>/Gain3'
   */
  CVCR_Controller_DW.Delay2_DSTATE_dh = 0.0062634873752217546 *
    CVCR_Controller_Y.LOG_CVCR_Id + 0.99373651262477825 *
    CVCR_Controller_DW.Delay2_DSTATE_dh;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Delay: '<S13>/Delay2'
   */
  CVCR_Controller_Y.LOG_CVCR_ElecQ = CVCR_Controller_DW.Delay2_DSTATE_be;

  /* Sum: '<S13>/Sum2' incorporates:
   *  Delay: '<S11>/Delay2'
   *  Delay: '<S13>/Delay2'
   *  Gain: '<Root>/Gain3'
   *  Gain: '<S13>/Gain1'
   *  Gain: '<S13>/Gain3'
   *  Product: '<Root>/Product5'
   *  Product: '<Root>/Product6'
   *  Sum: '<Root>/Sum5'
   */
  CVCR_Controller_DW.Delay2_DSTATE_be = (CVCR_Controller_Y.LOG_CVCR_Vq_c *
    rtb_Delay2_bw - CVCR_Controller_Y.LOG_CVCR_Vd_c *
    CVCR_Controller_DW.Delay2_DSTATE_o) * 1.5 * 0.0062634873752217546 +
    0.99373651262477825 * CVCR_Controller_DW.Delay2_DSTATE_be;

  /* Sum: '<S11>/Sum2' incorporates:
   *  Delay: '<S11>/Delay2'
   *  Gain: '<S11>/Gain1'
   *  Gain: '<S11>/Gain3'
   */
  CVCR_Controller_DW.Delay2_DSTATE_o = 0.0062634873752217546 *
    CVCR_Controller_Y.LOG_CVCR_Iq + 0.99373651262477825 *
    CVCR_Controller_DW.Delay2_DSTATE_o;

  /* Gain: '<Root>/Gain2' incorporates:
   *  Delay: '<S5>/Delay2'
   */
  CVCR_Controller_Y.LOG_CVCR_Wmfilt = 0.25 * CVCR_Controller_DW.Delay2_DSTATE;

  /* Sum: '<S5>/Sum2' incorporates:
   *  Delay: '<S5>/Delay2'
   *  Gain: '<S5>/Gain1'
   *  Gain: '<S5>/Gain3'
   *  Inport generated from: '<Root>/WeInst'
   */
  CVCR_Controller_DW.Delay2_DSTATE = 0.0018831801790618607 *
    CVCR_Controller_U.WeInst + 0.99811681982093814 *
    CVCR_Controller_DW.Delay2_DSTATE;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Delay: '<S3>/Delay2'
   */
  CVCR_Controller_Y.LOG_CVCR_RPM_c = CVCR_Controller_DW.Delay2_DSTATE_i;

  /* Sum: '<S3>/Sum2' incorporates:
   *  Delay: '<S3>/Delay2'
   *  Gain: '<S3>/Gain1'
   *  Gain: '<S3>/Gain3'
   *  Inport generated from: '<Root>/RPM_C'
   */
  CVCR_Controller_DW.Delay2_DSTATE_i = 0.0018831801790618607 *
    CVCR_Controller_U.RPM_C + 0.99811681982093814 *
    CVCR_Controller_DW.Delay2_DSTATE_i;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Inport generated from: '<Root>/Iq_c'
   */
  CVCR_Controller_Y.LOG_CVCR_Iq_c = CVCR_Controller_U.Iq_c;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Inport generated from: '<Root>/Id_c'
   */
  CVCR_Controller_Y.LOG_CVCR_Id_c = CVCR_Controller_U.Id_c;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Inport generated from: '<Root>/Iu'
   */
  CVCR_Controller_Y.LOG_CVCR_Iu = CVCR_Controller_U.Iu;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Inport generated from: '<Root>/Iv'
   */
  CVCR_Controller_Y.LOG_CVCR_Iv = CVCR_Controller_U.Iv;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Inport generated from: '<Root>/Iw'
   */
  CVCR_Controller_Y.LOG_CVCR_Iw = CVCR_Controller_U.Iw;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Inport generated from: '<Root>/Vdc'
   */
  CVCR_Controller_Y.LOG_CVCR_Vdc = CVCR_Controller_U.Vdc;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Inport generated from: '<Root>/Theta'
   */
  CVCR_Controller_Y.LOG_CVCR_Theta = CVCR_Controller_U.Theta;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Product: '<Root>/Product'
   *  Product: '<Root>/Product1'
   *  Sqrt: '<Root>/Sqrt'
   *  Sum: '<Root>/Sum'
   */
  CVCR_Controller_Y.LOG_CVCR_Vphasor = sqrt(CVCR_Controller_Y.LOG_CVCR_Vd_c *
    CVCR_Controller_Y.LOG_CVCR_Vd_c + CVCR_Controller_Y.LOG_CVCR_Vq_c *
    CVCR_Controller_Y.LOG_CVCR_Vq_c);

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Gain: '<Root>/Gain11'
   */
  CVCR_Controller_Y.LOG_CVCR_RPM = 9.5492965855137211 *
    CVCR_Controller_Y.LOG_CVCR_Wmfilt;

  /* Outport generated from: '<Root>/LOG_Outport_4' incorporates:
   *  Inport generated from: '<Root>/Iload'
   */
  CVCR_Controller_Y.LOG_CVCR_Iload = CVCR_Controller_U.Iload;

  /* Outport generated from: '<Root>/H41' */
  CVCR_Controller_Y.H41 = CVCR_Controller_Y.LOG_CVCR_H41;
}

/* Model initialize function */
void CVCR_Controller_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void CVCR_Controller_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
