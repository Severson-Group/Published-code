/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Controller_syncPI.c
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

#include "Controller_syncPI.h"
#include "rtwtypes.h"
#include "Controller_syncPI_private.h"
#include <math.h>
#include <float.h>

/* Named constants for Chart: '<Root>/StateTransition' */
#define Controller_syncPI_IN_DRIVE     ((uint8_T)1U)
#define Controller_syncPI_IN_FAIL      ((uint8_T)2U)
#define Controller_syncPI_IN_IDLE      ((uint8_T)3U)

/* Block states (default storage) */
DW_Controller_syncPI_T Controller_syncPI_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Controller_syncPI_T Controller_syncPI_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Controller_syncPI_T Controller_syncPI_Y;

/* Real-time model */
static RT_MODEL_Controller_syncPI_T Controller_syncPI_M_;
RT_MODEL_Controller_syncPI_T *const Controller_syncPI_M = &Controller_syncPI_M_;
real_T rt_modd(real_T u0, real_T u1)
{
  real_T y;
  y = u0;
  if (u1 == 0.0) {
    if (u0 == 0.0) {
      y = u1;
    }
  } else if (u0 == 0.0) {
    y = 0.0 / u1;
  } else {
    boolean_T yEq;
    y = fmod(u0, u1);
    yEq = (y == 0.0);
    if ((!yEq) && (u1 > floor(u1))) {
      real_T q;
      q = fabs(u0 / u1);
      yEq = (fabs(q - floor(q + 0.5)) <= DBL_EPSILON * q);
    }

    if (yEq) {
      y = 0.0;
    } else if ((u0 < 0.0) != (u1 < 0.0)) {
      y += u1;
    }
  }

  return y;
}

/* Model step function */
void Controller_syncPI_step(void)
{
  real_T tmp[9];
  real_T rtb_d_uvw[3];
  real_T rtb_x_dq0[3];
  real_T rtb_Add2;
  real_T rtb_Add3;
  real_T rtb_Delay_im;
  real_T rtb_Delay_re;
  real_T rtb_Gain_im;
  real_T rtb_Gain_k;
  real_T rtb_Gain_mb_idx_1;
  real_T rtb_Gain_mb_idx_2;
  real_T rtb_Switch3;
  real_T rtb_Switch4;
  real_T rtb_del_theta;
  real_T rtb_omega_ref;
  int32_T i;

  /* Gain: '<S1>/Gain' incorporates:
   *  Inport generated from: '<Root>/I_uvw_fb'
   *  SignalConversion generated from: '<Root>/I_uvw_fb'
   */
  rtb_Gain_mb_idx_1 = 0.66666666666666663 * Controller_syncPI_U.I_uvw_fb[1];
  rtb_Gain_mb_idx_2 = 0.66666666666666663 * Controller_syncPI_U.I_uvw_fb[2];

  /* Gain: '<S20>/Gain' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  rtb_Gain_k = 0.0001 * Controller_syncPI_DW.Delay1_DSTATE_f;

  /* Delay: '<S20>/Delay' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  if (Controller_syncPI_DW.icLoad) {
    /* Sum: '<S20>/Sum' incorporates:
     *  Inport generated from: '<Root>/In Bus Element11'
     */
    Controller_syncPI_DW.Delay_DSTATE_m =
      Controller_syncPI_U.PARAMS_omega_m_init;
  }

  Controller_syncPI_DW.Delay1_DSTATE_f = Controller_syncPI_DW.Delay_DSTATE_m;

  /* End of Delay: '<S20>/Delay' */

  /* Sum: '<S20>/Sum' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  Controller_syncPI_DW.Delay_DSTATE_m = rtb_Gain_k +
    Controller_syncPI_DW.Delay1_DSTATE_f;

  /* Delay: '<S14>/Delay' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  Controller_syncPI_DW.Delay1_DSTATE_f = Controller_syncPI_DW.Delay_DSTATE_m2;

  /* Sum: '<S14>/Sum2' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  Controller_syncPI_Y.LOG_signals_omega_m_hat =
    Controller_syncPI_DW.Delay_DSTATE_m + Controller_syncPI_DW.Delay1_DSTATE_f;

  /* Delay: '<S21>/Delay' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  Controller_syncPI_DW.Delay1_DSTATE_f = Controller_syncPI_DW.Delay_DSTATE_c;

  /* Sum: '<S21>/Sum' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  rtb_Gain_k = Controller_syncPI_Y.LOG_signals_omega_m_hat +
    Controller_syncPI_DW.Delay1_DSTATE_f;

  /* Delay: '<S21>/Delay1' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  if (Controller_syncPI_DW.icLoad_h) {
    /* Sum: '<S21>/Sum1' incorporates:
     *  Inport generated from: '<Root>/In Bus Element10'
     */
    Controller_syncPI_DW.Delay1_DSTATE_c =
      Controller_syncPI_U.PARAMS_theta_m_init;
  }

  Controller_syncPI_DW.Delay1_DSTATE_f = Controller_syncPI_DW.Delay1_DSTATE_c;

  /* End of Delay: '<S21>/Delay1' */

  /* Sum: '<S21>/Sum1' incorporates:
   *  Delay: '<S20>/Delay1'
   *  Gain: '<S21>/Gain'
   */
  Controller_syncPI_DW.Delay1_DSTATE_c = 5.0E-5 * rtb_Gain_k +
    Controller_syncPI_DW.Delay1_DSTATE_f;

  /* Math: '<S14>/Mod1' incorporates:
   *  Constant: '<S14>/Constant1'
   */
  Controller_syncPI_Y.LOG_signals_theta_m_hat = rt_modd
    (Controller_syncPI_DW.Delay1_DSTATE_c, 6.2831853071795862);

  /* Switch: '<Root>/Switch3' incorporates:
   *  Inport generated from: '<Root>/In Bus Element15'
   *  Inport generated from: '<Root>/In Bus Element13'
   */
  if (Controller_syncPI_U.PARAMS_selfsensing_enable > 0.0) {
    /* Switch: '<Root>/Switch2' incorporates:
     *  Delay: '<Root>/Delay4'
     *  Inport generated from: '<Root>/In Bus Element15'
     */
    if (Controller_syncPI_Y.LOG_signals_omega_filt > 1308.9969389957471) {
      rtb_Switch3 = Controller_syncPI_Y.LOG_signals_theta_m_hat;
    } else {
      rtb_Switch3 = Controller_syncPI_U.CMD_theta_m;
    }

    /* End of Switch: '<Root>/Switch2' */
  } else {
    rtb_Switch3 = Controller_syncPI_U.CMD_theta_m;
  }

  /* End of Switch: '<Root>/Switch3' */

  /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
   *  Inport generated from: '<Root>/I_uvw_fb'
   *  SignalConversion generated from: '<Root>/I_uvw_fb'
   *  Trigonometry: '<S2>/Trigonometric Function'
   *  Trigonometry: '<S2>/Trigonometric Function1'
   */
  rtb_Add2 = cos(rtb_Switch3);
  rtb_Add3 = sin(rtb_Switch3);
  tmp[0] = 0.66666666666666663 * rtb_Add2;
  tmp[3] = cos(rtb_Switch3 - 2.0943951023931953) * 0.66666666666666663;
  tmp[6] = cos(rtb_Switch3 - 4.1887902047863905) * 0.66666666666666663;
  tmp[1] = 0.66666666666666663 * -rtb_Add3;
  tmp[4] = -sin(rtb_Switch3 - 2.0943951023931953) * 0.66666666666666663;
  tmp[7] = -sin(rtb_Switch3 - 4.1887902047863905) * 0.66666666666666663;
  tmp[2] = 0.33333333333333331;
  tmp[5] = 0.33333333333333331;
  tmp[8] = 0.33333333333333331;
  for (i = 0; i < 3; i++) {
    rtb_x_dq0[i] = (tmp[i + 3] * Controller_syncPI_U.I_uvw_fb[1] + tmp[i] *
                    Controller_syncPI_U.I_uvw_fb[0]) + tmp[i + 6] *
      Controller_syncPI_U.I_uvw_fb[2];
  }

  /* End of MATLAB Function: '<Root>/MATLAB Function' */

  /* Delay: '<Root>/Delay1' incorporates:
   *  Delay: '<S20>/Delay1'
   *  Inport generated from: '<Root>/In Bus Element11'
   */
  if (Controller_syncPI_DW.icLoad_e) {
    Controller_syncPI_DW.Delay1_DSTATE_cl =
      Controller_syncPI_U.PARAMS_omega_m_init;
  }

  Controller_syncPI_DW.Delay1_DSTATE_f = Controller_syncPI_DW.Delay1_DSTATE_cl;

  /* End of Delay: '<Root>/Delay1' */

  /* Gain: '<Root>/Gain5' */
  rtb_Gain_k = 0.011790000000000002 * rtb_x_dq0[1];

  /* Delay: '<Root>/Delay' incorporates:
   *  Inport generated from: '<Root>/In Bus Element10'
   */
  if (Controller_syncPI_DW.icLoad_e0) {
    Controller_syncPI_DW.Delay_DSTATE_d =
      Controller_syncPI_U.PARAMS_theta_m_init;
  }

  /* MATLAB Function: '<Root>/speed_calc' incorporates:
   *  Delay: '<Root>/Delay'
   *  Inport generated from: '<Root>/In Bus Element15'
   */
  for (rtb_del_theta = Controller_syncPI_U.CMD_theta_m -
       Controller_syncPI_DW.Delay_DSTATE_d; rtb_del_theta > 3.1415926535897931;
       rtb_del_theta -= 6.2831853071795862) {
  }

  while (rtb_del_theta < -3.1415926535897931) {
    rtb_del_theta += 6.2831853071795862;
  }

  /* End of MATLAB Function: '<Root>/speed_calc' */

  /* Gain: '<Root>/Gain' */
  Controller_syncPI_Y.LOG_signals_omega = 10000.0 * rtb_del_theta;

  /* Sum: '<Root>/Sum2' incorporates:
   *  Delay: '<Root>/Delay2'
   *  Delay: '<Root>/Delay3'
   *  Delay: '<S20>/Delay1'
   *  Gain: '<Root>/Gain1'
   *  Gain: '<Root>/Gain2'
   *  Gain: '<Root>/Gain3'
   *  Gain: '<Root>/Gain4'
   *  Sum: '<Root>/Sum1'
   *  Sum: '<Root>/Sum3'
   */
  Controller_syncPI_Y.LOG_signals_omega_filt =
    (((Controller_syncPI_Y.LOG_signals_omega -
       Controller_syncPI_Y.LOG_signals_omega_filt) * 0.0006521946348852411 +
      rtb_Gain_k) - 7.21E-6 * Controller_syncPI_Y.LOG_signals_omega_filt) *
    96339.113680154143 * 0.0001 + Controller_syncPI_DW.Delay1_DSTATE_f;

  /* Switch: '<Root>/Switch4' incorporates:
   *  Delay: '<Root>/Delay3'
   *  Inport generated from: '<Root>/In Bus Element13'
   */
  if (Controller_syncPI_U.PARAMS_selfsensing_enable > 0.0) {
    /* Switch: '<Root>/Switch' incorporates:
     *  Delay: '<Root>/Delay3'
     *  Delay: '<Root>/Delay5'
     */
    if (Controller_syncPI_DW.Delay5_DSTATE > 1308.9969389957471) {
      rtb_Switch4 = Controller_syncPI_Y.LOG_signals_omega_m_hat;
    } else {
      rtb_Switch4 = Controller_syncPI_Y.LOG_signals_omega_filt;
    }

    /* End of Switch: '<Root>/Switch' */
  } else {
    rtb_Switch4 = Controller_syncPI_Y.LOG_signals_omega_filt;
  }

  /* End of Switch: '<Root>/Switch4' */

  /* Gain: '<S8>/Gain2' incorporates:
   *  Delay: '<S18>/Delay'
   *  Gain: '<S8>/Gain1'
   */
  rtb_Delay_im = 0.0001 * rtb_Switch4;

  /* Math: '<S8>/Exp' incorporates:
   *  Delay: '<S13>/Delay4'
   *  Delay: '<S18>/Delay'
   *
   * About '<S8>/Exp':
   *  Operator: exp
   */
  if (rtb_Delay_im == 0.0) {
    rtb_Delay_re = 1.0;
    rtb_Delay_im = 0.0;
  } else {
    rtb_Delay_re = cos(rtb_Delay_im);
    rtb_Delay_im = sin(rtb_Delay_im);
  }

  /* End of Math: '<S8>/Exp' */

  /* Gain: '<Root>/Gain11' incorporates:
   *  Inport generated from: '<Root>/In Bus Element12'
   */
  rtb_del_theta = 0.10471975511965977 * Controller_syncPI_U.PARAMS_RPM_limit;

  /* Gain: '<Root>/Gain6' incorporates:
   *  Inport generated from: '<Root>/In Bus Element9'
   */
  rtb_omega_ref = 0.10471975511965977 * Controller_syncPI_U.CMD_speed;

  /* Switch: '<S6>/Switch2' incorporates:
   *  Gain: '<Root>/Gain13'
   *  RelationalOperator: '<S6>/LowerRelop1'
   *  RelationalOperator: '<S6>/UpperRelop'
   *  Switch: '<S6>/Switch'
   */
  if (rtb_omega_ref > rtb_del_theta) {
    rtb_omega_ref = rtb_del_theta;
  } else if (rtb_omega_ref < -rtb_del_theta) {
    /* Switch: '<S6>/Switch' incorporates:
     *  Gain: '<Root>/Gain13'
     */
    rtb_omega_ref = -rtb_del_theta;
  }

  /* End of Switch: '<S6>/Switch2' */

  /* Sum: '<S5>/Sum' */
  rtb_del_theta = rtb_omega_ref - rtb_Switch4;

  /* DiscreteTransferFcn: '<S5>/Discrete Transfer Fcn' incorporates:
   *  Gain: '<S5>/Gain'
   */
  Controller_syncPI_DW.DiscreteTransferFcn_states = 4.5301766064764811E-5 *
    rtb_del_theta - (-Controller_syncPI_DW.DiscreteTransferFcn_states);

  /* Switch: '<Root>/Switch1' incorporates:
   *  Constant: '<Root>/Constant1'
   *  Inport generated from: '<Root>/In Bus Element'
   *  Inport generated from: '<Root>/In Bus Element1'
   *  Inport generated from: '<Root>/In Bus Element8'
   */
  if (Controller_syncPI_U.PARAMS_speed_control > 0.0) {
    Controller_syncPI_Y.LOG_signals_i_dq_cmd[0] = 0.0;

    /* Gain: '<Root>/Gain7' incorporates:
     *  Constant: '<Root>/Constant1'
     *  DiscreteTransferFcn: '<S5>/Discrete Transfer Fcn'
     *  Gain: '<S5>/Gain1'
     *  Sum: '<S5>/Sum1'
     */
    rtb_del_theta = (6.52194634885241E-5 * rtb_del_theta + 0.0001 *
                     Controller_syncPI_DW.DiscreteTransferFcn_states) *
      84.817642069550459;

    /* Saturate: '<Root>/Saturation - Current' */
    if (rtb_del_theta > 10.0) {
      Controller_syncPI_Y.LOG_signals_i_dq_cmd[1] = 10.0;
    } else if (rtb_del_theta < -10.0) {
      Controller_syncPI_Y.LOG_signals_i_dq_cmd[1] = -10.0;
    } else {
      Controller_syncPI_Y.LOG_signals_i_dq_cmd[1] = rtb_del_theta;
    }

    /* End of Saturate: '<Root>/Saturation - Current' */
  } else {
    Controller_syncPI_Y.LOG_signals_i_dq_cmd[0] = Controller_syncPI_U.CMD_I_d;
    Controller_syncPI_Y.LOG_signals_i_dq_cmd[1] = Controller_syncPI_U.CMD_I_q;
  }

  /* End of Switch: '<Root>/Switch1' */

  /* Gain: '<S8>/Gain' incorporates:
   *  RealImagToComplex: '<S8>/Real-Imag to Complex'
   *  RealImagToComplex: '<S8>/Real-Imag to Complex1'
   *  Sum: '<S8>/Sum'
   */
  rtb_del_theta = (Controller_syncPI_Y.LOG_signals_i_dq_cmd[0] - rtb_x_dq0[0]) *
    0.35435267650528984;
  rtb_Gain_im = (Controller_syncPI_Y.LOG_signals_i_dq_cmd[1] - rtb_x_dq0[1]) *
    0.35435267650528984;

  /* Sum: '<S8>/Sum2' incorporates:
   *  Delay: '<S13>/Delay4'
   *  Delay: '<S8>/Delay'
   *  Delay: '<S8>/Delay1'
   *  Gain: '<S8>/Gain'
   *  Product: '<S8>/Product'
   *  Sum: '<S8>/Sum1'
   */
  Controller_syncPI_DW.Delay1_DSTATE.re += (rtb_Delay_re * rtb_del_theta -
    rtb_Delay_im * rtb_Gain_im) - Controller_syncPI_DW.Delay_DSTATE.re;
  Controller_syncPI_DW.Delay1_DSTATE.im += (rtb_Delay_re * rtb_Gain_im +
    rtb_Delay_im * rtb_del_theta) - Controller_syncPI_DW.Delay_DSTATE.im;

  /* Switch: '<S8>/Switch' incorporates:
   *  Delay: '<S8>/Delay1'
   *  Gain: '<S8>/Gain10'
   *  Inport generated from: '<Root>/In Bus Element3'
   *  Inport generated from: '<Root>/In Bus Element4'
   *  Inport generated from: '<Root>/In Bus Element5'
   *  Sum: '<S8>/Sum2'
   *  Sum: '<S8>/Sum8'
   */
  if (Controller_syncPI_U.PARAMS_cc_enable > 0.0) {
    Controller_syncPI_Y.LOG_signals_v_dq[0] =
      Controller_syncPI_DW.Delay1_DSTATE.re;
    Controller_syncPI_Y.LOG_signals_v_dq[1] = 0.00786 * rtb_Switch4 +
      Controller_syncPI_DW.Delay1_DSTATE.im;
  } else {
    Controller_syncPI_Y.LOG_signals_v_dq[0] = Controller_syncPI_U.CMD_V_d;
    Controller_syncPI_Y.LOG_signals_v_dq[1] = Controller_syncPI_U.CMD_V_q;
  }

  /* End of Switch: '<S8>/Switch' */

  /* Sum: '<Root>/Sum4' incorporates:
   *  Gain: '<Root>/Gain9'
   */
  rtb_omega_ref = 0.00015000000000000001 * rtb_Switch4 + rtb_Switch3;

  /* MATLAB Function: '<Root>/MATLAB Function1' */
  tmp[0] = cos(rtb_omega_ref);
  tmp[3] = -sin(rtb_omega_ref);
  tmp[6] = 1.0;
  tmp[1] = cos(rtb_omega_ref + 4.1887902047863905);
  tmp[4] = -sin(rtb_omega_ref + 4.1887902047863905);
  tmp[7] = 1.0;
  tmp[2] = cos(rtb_omega_ref + 2.0943951023931953);
  tmp[5] = -sin(rtb_omega_ref + 2.0943951023931953);
  tmp[8] = 1.0;

  /* SignalConversion generated from: '<S4>/ SFunction ' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  rtb_Switch3 = Controller_syncPI_Y.LOG_signals_v_dq[0];
  rtb_Delay_im = Controller_syncPI_Y.LOG_signals_v_dq[1];

  /* Gain: '<S9>/Gain1' incorporates:
   *  Inport generated from: '<Root>/V_dc'
   */
  rtb_omega_ref = 0.5 * Controller_syncPI_U.V_dc;

  /* Gain: '<S9>/Gain2' incorporates:
   *  Delay: '<S20>/Delay1'
   *  Inport generated from: '<Root>/V_dc'
   */
  Controller_syncPI_DW.Delay1_DSTATE_f = -0.5 * Controller_syncPI_U.V_dc;
  for (i = 0; i < 3; i++) {
    /* MATLAB Function: '<Root>/MATLAB Function1' */
    Controller_syncPI_Y.LOG_signals_v_uvw[i] = tmp[i + 3] * rtb_Delay_im + tmp[i]
      * rtb_Switch3;

    /* Switch: '<S12>/Switch2' incorporates:
     *  Delay: '<S20>/Delay1'
     *  RelationalOperator: '<S12>/LowerRelop1'
     *  RelationalOperator: '<S12>/UpperRelop'
     *  Switch: '<S12>/Switch'
     */
    if (Controller_syncPI_Y.LOG_signals_v_uvw[i] > rtb_omega_ref) {
      rtb_Delay_re = rtb_omega_ref;
    } else if (Controller_syncPI_Y.LOG_signals_v_uvw[i] <
               Controller_syncPI_DW.Delay1_DSTATE_f) {
      /* Switch: '<S12>/Switch' incorporates:
       *  Delay: '<S20>/Delay1'
       */
      rtb_Delay_re = Controller_syncPI_DW.Delay1_DSTATE_f;
    } else {
      rtb_Delay_re = Controller_syncPI_Y.LOG_signals_v_uvw[i];
    }

    /* End of Switch: '<S12>/Switch2' */

    /* Sum: '<S9>/Sum1' incorporates:
     *  Constant: '<S9>/Constant7'
     *  Inport generated from: '<Root>/V_dc'
     *  Product: '<S9>/Divide'
     */
    rtb_d_uvw[i] = rtb_Delay_re / Controller_syncPI_U.V_dc + 0.5;
  }

  /* Product: '<S2>/Product' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  Controller_syncPI_DW.Delay1_DSTATE_f = Controller_syncPI_Y.LOG_signals_v_dq[0]
    * rtb_Add2;

  /* Sum: '<S2>/Add' incorporates:
   *  Delay: '<S20>/Delay1'
   *  Gain: '<S2>/Gain4'
   *  Product: '<S2>/Product1'
   */
  Controller_syncPI_DW.Delay1_DSTATE_f += Controller_syncPI_Y.LOG_signals_v_dq[1]
    * -rtb_Add3;

  /* Math: '<S10>/Mod' incorporates:
   *  Constant: '<S10>/Constant'
   */
  rtb_omega_ref = rt_modd(Controller_syncPI_Y.LOG_signals_theta_m_hat,
    6.2831853071795862);

  /* Trigonometry: '<S16>/Trigonometric Function' incorporates:
   *  Trigonometry: '<S15>/Trigonometric Function'
   */
  rtb_Switch3 = cos(rtb_omega_ref);

  /* Sum: '<S2>/Add1' incorporates:
   *  Product: '<S2>/Product2'
   *  Product: '<S2>/Product3'
   */
  rtb_Delay_im = Controller_syncPI_Y.LOG_signals_v_dq[0] * rtb_Add3 +
    Controller_syncPI_Y.LOG_signals_v_dq[1] * rtb_Add2;

  /* Trigonometry: '<S16>/Trigonometric Function1' incorporates:
   *  Trigonometry: '<S15>/Trigonometric Function1'
   */
  rtb_Delay_re = sin(rtb_omega_ref);

  /* Sum: '<S16>/Add2' incorporates:
   *  Delay: '<S20>/Delay1'
   *  Product: '<S16>/Product'
   *  Product: '<S16>/Product1'
   *  Trigonometry: '<S16>/Trigonometric Function'
   *  Trigonometry: '<S16>/Trigonometric Function1'
   */
  rtb_Add2 = Controller_syncPI_DW.Delay1_DSTATE_f * rtb_Switch3 + rtb_Delay_im *
    rtb_Delay_re;

  /* Sum: '<S16>/Add3' incorporates:
   *  Delay: '<S20>/Delay1'
   *  Gain: '<S16>/Gain5'
   *  Product: '<S16>/Product2'
   *  Product: '<S16>/Product3'
   *  Trigonometry: '<S16>/Trigonometric Function'
   *  Trigonometry: '<S16>/Trigonometric Function1'
   */
  rtb_Add3 = Controller_syncPI_DW.Delay1_DSTATE_f * -rtb_Delay_re + rtb_Delay_im
    * rtb_Switch3;

  /* Sum: '<S1>/Add' incorporates:
   *  Gain: '<S1>/Gain'
   *  Gain: '<S1>/Gain1'
   *  Gain: '<S1>/Gain2'
   *  Inport generated from: '<Root>/I_uvw_fb'
   *  SignalConversion generated from: '<Root>/I_uvw_fb'
   */
  rtb_Delay_im = (0.66666666666666663 * Controller_syncPI_U.I_uvw_fb[0] + -0.5 *
                  rtb_Gain_mb_idx_1) + -0.5 * rtb_Gain_mb_idx_2;

  /* Gain: '<S1>/Gain4' incorporates:
   *  Delay: '<S20>/Delay1'
   */
  Controller_syncPI_DW.Delay1_DSTATE_f = -0.8660254037844386 * rtb_Gain_mb_idx_2;

  /* Sum: '<S1>/Add1' incorporates:
   *  Delay: '<S20>/Delay1'
   *  Gain: '<S1>/Gain3'
   */
  rtb_omega_ref = 0.8660254037844386 * rtb_Gain_mb_idx_1 +
    Controller_syncPI_DW.Delay1_DSTATE_f;

  /* Sum: '<S15>/Add2' incorporates:
   *  Product: '<S15>/Product'
   *  Product: '<S15>/Product1'
   */
  rtb_Gain_mb_idx_1 = rtb_Delay_im * rtb_Switch3 + rtb_omega_ref * rtb_Delay_re;

  /* Sum: '<S15>/Add3' incorporates:
   *  Gain: '<S15>/Gain5'
   *  Product: '<S15>/Product2'
   *  Product: '<S15>/Product3'
   */
  rtb_Gain_mb_idx_2 = rtb_Delay_im * -rtb_Delay_re + rtb_omega_ref * rtb_Switch3;

  /* Gain: '<S17>/Gain4' incorporates:
   *  Delay: '<S18>/Delay'
   *  Gain: '<S17>/Gain3'
   */
  rtb_Delay_im = -(0.0001 * Controller_syncPI_Y.LOG_signals_omega_m_hat);

  /* Math: '<S17>/Exp' incorporates:
   *  Delay: '<S18>/Delay'
   *  Gain: '<S18>/Gain3'
   *
   * About '<S17>/Exp':
   *  Operator: exp
   */
  if (rtb_Delay_im == 0.0) {
    rtb_omega_ref = 1.0;
    rtb_Switch3 = 0.0;
  } else {
    rtb_omega_ref = cos(rtb_Delay_im);
    rtb_Switch3 = sin(rtb_Delay_im);
  }

  /* End of Math: '<S17>/Exp' */

  /* Sum: '<S17>/Sum1' incorporates:
   *  Delay: '<S17>/Delay'
   *  Delay: '<S17>/Delay1'
   *  Gain: '<S17>/Gain1'
   *  Gain: '<S17>/Gain2'
   *  Sum: '<S17>/Sum'
   */
  rtb_Delay_im = (Controller_syncPI_DW.Delay1_DSTATE_j.re - 0.898159681534261 *
                  Controller_syncPI_DW.Delay1_DSTATE_j.re) + 0.898159681534261 *
    Controller_syncPI_DW.Delay_DSTATE_k.re;
  rtb_Delay_re = (Controller_syncPI_DW.Delay1_DSTATE_j.im - 0.898159681534261 *
                  Controller_syncPI_DW.Delay1_DSTATE_j.im) + 0.898159681534261 *
    Controller_syncPI_DW.Delay_DSTATE_k.im;

  /* Product: '<S17>/Product' incorporates:
   *  Delay: '<S17>/Delay'
   *  Gain: '<S18>/Gain3'
   */
  Controller_syncPI_DW.Delay_DSTATE_k.re = rtb_omega_ref * rtb_Delay_im -
    rtb_Switch3 * rtb_Delay_re;
  Controller_syncPI_DW.Delay_DSTATE_k.im = rtb_omega_ref * rtb_Delay_re +
    rtb_Switch3 * rtb_Delay_im;

  /* Gain: '<S18>/Gain3' incorporates:
   *  Delay: '<S17>/Delay'
   *  Product: '<S17>/Product'
   *  RealImagToComplex: '<S13>/Real-Imag to Complex'
   *  Sum: '<S18>/Sum'
   */
  rtb_omega_ref = (rtb_Gain_mb_idx_1 - Controller_syncPI_DW.Delay_DSTATE_k.re) *
    0.35435267650528984;
  rtb_Switch3 = (rtb_Gain_mb_idx_2 - Controller_syncPI_DW.Delay_DSTATE_k.im) *
    0.35435267650528984;

  /* Gain: '<S18>/Gain2' incorporates:
   *  Delay: '<S13>/Delay4'
   *  Gain: '<S18>/Gain1'
   */
  rtb_Delay_im = 0.0001 * Controller_syncPI_Y.LOG_signals_omega_m_hat;

  /* Math: '<S18>/Exp' incorporates:
   *  Delay: '<S13>/Delay4'
   *  Delay: '<S18>/Delay'
   *
   * About '<S18>/Exp':
   *  Operator: exp
   */
  if (rtb_Delay_im == 0.0) {
    rtb_Delay_re = 1.0;
    rtb_Delay_im = 0.0;
  } else {
    rtb_Delay_re = cos(rtb_Delay_im);
    rtb_Delay_im = sin(rtb_Delay_im);
  }

  /* End of Math: '<S18>/Exp' */

  /* Sum: '<S18>/Sum7' incorporates:
   *  Delay: '<S18>/Delay'
   *  Delay: '<S18>/Delay1'
   *  Gain: '<S18>/Gain3'
   *  Product: '<S18>/Product3'
   *  Sum: '<S18>/Sum6'
   */
  Controller_syncPI_DW.Delay1_DSTATE_g.re += (rtb_Delay_re * rtb_omega_ref -
    rtb_Delay_im * rtb_Switch3) - Controller_syncPI_DW.Delay_DSTATE_b.re;
  Controller_syncPI_DW.Delay1_DSTATE_g.im += (rtb_Delay_re * rtb_Switch3 +
    rtb_Delay_im * rtb_omega_ref) - Controller_syncPI_DW.Delay_DSTATE_b.im;

  /* Product: '<S19>/Divide' incorporates:
   *  Constant: '<S19>/Constant'
   *  Delay: '<S18>/Delay1'
   *  Gain: '<S13>/Gain12'
   *  Product: '<S19>/Product'
   *  Saturate: '<S19>/Saturation'
   *  Sum: '<S18>/Sum7'
   *  Sum: '<S19>/Sum'
   */
  rtb_Delay_im = Controller_syncPI_DW.Delay1_DSTATE_g.re / (fmax
    (Controller_syncPI_Y.LOG_signals_omega_m_hat, 5.0) * 0.00786);

  /* DiscreteTransferFcn: '<S14>/Discrete Transfer Fcn' incorporates:
   *  Gain: '<S14>/Gain'
   */
  Controller_syncPI_DW.DiscreteTransferFcn_states_o = 16.092257597075609 *
    rtb_Delay_im - (-Controller_syncPI_DW.DiscreteTransferFcn_states_o);

  /* Chart: '<Root>/StateTransition' incorporates:
   *  Inport generated from: '<Root>/In Bus Element7'
   *  Inport generated from: '<Root>/In Bus Element6'
   *  Math: '<Root>/Square'
   *  Math: '<Root>/Square1'
   *  Sqrt: '<Root>/Sqrt'
   *  Sum: '<Root>/Sum'
   */
  if (Controller_syncPI_DW.is_active_c3_Controller_syncPI == 0U) {
    Controller_syncPI_DW.is_active_c3_Controller_syncPI = 1U;
    Controller_syncPI_DW.is_c3_Controller_syncPI = Controller_syncPI_IN_IDLE;

    /* Outport generated from: '<Root>/PWM_enable' */
    Controller_syncPI_Y.PWM_enable = 0.0;
  } else {
    switch (Controller_syncPI_DW.is_c3_Controller_syncPI) {
     case Controller_syncPI_IN_DRIVE:
      /* Outport generated from: '<Root>/PWM_enable' */
      Controller_syncPI_Y.PWM_enable = 1.0;
      if (Controller_syncPI_U.PARAMS_drv_enable == 0.0) {
        Controller_syncPI_DW.is_c3_Controller_syncPI = Controller_syncPI_IN_IDLE;

        /* Outport generated from: '<Root>/PWM_enable' */
        Controller_syncPI_Y.PWM_enable = 0.0;
      } else if (Controller_syncPI_DW.count > 100.0) {
        Controller_syncPI_DW.is_c3_Controller_syncPI = Controller_syncPI_IN_FAIL;

        /* Outport generated from: '<Root>/PWM_enable' */
        Controller_syncPI_Y.PWM_enable = 0.0;
      } else if (sqrt(rtb_x_dq0[0] * rtb_x_dq0[0] + rtb_x_dq0[1] * rtb_x_dq0[1])
                 > Controller_syncPI_U.PARAMS_I_max) {
        Controller_syncPI_DW.count++;
      } else {
        Controller_syncPI_DW.count = 0.0;
      }
      break;

     case Controller_syncPI_IN_FAIL:
      /* Outport generated from: '<Root>/PWM_enable' */
      Controller_syncPI_Y.PWM_enable = 0.0;
      if (Controller_syncPI_U.PARAMS_drv_enable == 0.0) {
        Controller_syncPI_DW.is_c3_Controller_syncPI = Controller_syncPI_IN_IDLE;
      }
      break;

     default:
      /* Outport generated from: '<Root>/PWM_enable' */
      /* case IN_IDLE: */
      Controller_syncPI_Y.PWM_enable = 0.0;
      if (Controller_syncPI_U.PARAMS_drv_enable != 0.0) {
        Controller_syncPI_DW.is_c3_Controller_syncPI =
          Controller_syncPI_IN_DRIVE;

        /* Outport generated from: '<Root>/PWM_enable' */
        Controller_syncPI_Y.PWM_enable = 1.0;
        Controller_syncPI_DW.count = 0.0;
      }
      break;
    }
  }

  /* End of Chart: '<Root>/StateTransition' */

  /* Update for Delay: '<S20>/Delay1' incorporates:
   *  DiscreteTransferFcn: '<S14>/Discrete Transfer Fcn'
   *  Gain: '<S14>/Gain1'
   *  Gain: '<S14>/Gain3'
   *  Gain: '<S14>/Gain4'
   *  Sum: '<S14>/Sum'
   *  Sum: '<S14>/Sum1'
   */
  Controller_syncPI_DW.Delay1_DSTATE_f = (((0.0001 *
    Controller_syncPI_DW.DiscreteTransferFcn_states_o + 0.6659022089414991 *
    rtb_Delay_im) + rtb_Gain_k) - 7.21E-6 *
    Controller_syncPI_Y.LOG_signals_omega_m_hat) * 96339.113680154143;

  /* Update for Delay: '<S20>/Delay' */
  Controller_syncPI_DW.icLoad = false;

  /* Update for Delay: '<S14>/Delay' incorporates:
   *  Gain: '<S14>/Gain2'
   *  Gain: '<S14>/Gain7'
   */
  Controller_syncPI_DW.Delay_DSTATE_m2 = 0.0052175570790819288 * rtb_Delay_im *
    96339.113680154143;

  /* Update for Delay: '<S21>/Delay' */
  Controller_syncPI_DW.Delay_DSTATE_c =
    Controller_syncPI_Y.LOG_signals_omega_m_hat;

  /* Update for Delay: '<S21>/Delay1' */
  Controller_syncPI_DW.icLoad_h = false;

  /* Update for Delay: '<Root>/Delay1' incorporates:
   *  Delay: '<Root>/Delay3'
   */
  Controller_syncPI_DW.icLoad_e = false;
  Controller_syncPI_DW.Delay1_DSTATE_cl =
    Controller_syncPI_Y.LOG_signals_omega_filt;

  /* Update for Delay: '<Root>/Delay' incorporates:
   *  Inport generated from: '<Root>/In Bus Element15'
   */
  Controller_syncPI_DW.icLoad_e0 = false;
  Controller_syncPI_DW.Delay_DSTATE_d = Controller_syncPI_U.CMD_theta_m;

  /* Update for Delay: '<Root>/Delay5' incorporates:
   *  Delay: '<Root>/Delay3'
   */
  Controller_syncPI_DW.Delay5_DSTATE =
    Controller_syncPI_Y.LOG_signals_omega_filt;

  /* Update for Delay: '<S8>/Delay' incorporates:
   *  Gain: '<S8>/Gain'
   *  Gain: '<S8>/Gain3'
   */
  Controller_syncPI_DW.Delay_DSTATE.re = 0.898159681534261 * rtb_del_theta;
  Controller_syncPI_DW.Delay_DSTATE.im = 0.898159681534261 * rtb_Gain_im;

  /* Update for Delay: '<S17>/Delay1' incorporates:
   *  Delay: '<S13>/Delay4'
   *  Delay: '<S18>/Delay1'
   *  Gain: '<S17>/Gain'
   *  Sum: '<S13>/Sum6'
   *  Sum: '<S18>/Sum7'
   */
  Controller_syncPI_DW.Delay1_DSTATE_j.re =
    (Controller_syncPI_DW.Delay1_DSTATE_g.re +
     Controller_syncPI_DW.Delay4_DSTATE.re) * 6.8965517241379315;
  Controller_syncPI_DW.Delay1_DSTATE_j.im =
    (Controller_syncPI_DW.Delay1_DSTATE_g.im +
     Controller_syncPI_DW.Delay4_DSTATE.im) * 6.8965517241379315;

  /* Update for Delay: '<S18>/Delay' incorporates:
   *  Gain: '<S18>/Gain3'
   *  Gain: '<S18>/Gain5'
   */
  Controller_syncPI_DW.Delay_DSTATE_b.re = 0.898159681534261 * rtb_omega_ref;
  Controller_syncPI_DW.Delay_DSTATE_b.im = 0.898159681534261 * rtb_Switch3;

  /* Update for Delay: '<S13>/Delay4' incorporates:
   *  RealImagToComplex: '<S13>/Real-Imag to Complex1'
   */
  Controller_syncPI_DW.Delay4_DSTATE.re = rtb_Add2;
  Controller_syncPI_DW.Delay4_DSTATE.im = rtb_Add3;

  /* Outport generated from: '<Root>/LOG_Outport_5' */
  Controller_syncPI_Y.LOG_signals_i_dq_fb[0] = rtb_x_dq0[0];
  Controller_syncPI_Y.LOG_signals_i_dq_fb[1] = rtb_x_dq0[1];

  /* Outport generated from: '<Root>/LOG_Outport_5' incorporates:
   *  Inport generated from: '<Root>/I_uvw_fb'
   *  SignalConversion generated from: '<Root>/I_uvw_fb'
   */
  Controller_syncPI_Y.LOG_signals_i_uvw[0] = Controller_syncPI_U.I_uvw_fb[0];
  Controller_syncPI_Y.LOG_signals_i_uvw[1] = Controller_syncPI_U.I_uvw_fb[1];
  Controller_syncPI_Y.LOG_signals_i_uvw[2] = Controller_syncPI_U.I_uvw_fb[2];

  /* Outport generated from: '<Root>/LOG_bemfo_Outport_6' */
  Controller_syncPI_Y.LOG_bemfo_i_dq[0] = rtb_Gain_mb_idx_1;
  Controller_syncPI_Y.LOG_bemfo_i_dq[1] = rtb_Gain_mb_idx_2;

  /* Outport generated from: '<Root>/LOG_bemfo_Outport_6' */
  Controller_syncPI_Y.LOG_bemfo_v_dq[0] = rtb_Add2;
  Controller_syncPI_Y.LOG_bemfo_v_dq[1] = rtb_Add3;

  /* Outport generated from: '<Root>/LOG_bemfo_Outport_6' incorporates:
   *  Delay: '<S18>/Delay1'
   *  Gain: '<S13>/Gain12'
   *  Sum: '<S18>/Sum7'
   */
  Controller_syncPI_Y.LOG_bemfo_emf[0] =
    -Controller_syncPI_DW.Delay1_DSTATE_g.re;
  Controller_syncPI_Y.LOG_bemfo_emf[1] =
    -Controller_syncPI_DW.Delay1_DSTATE_g.im;

  /* Outport generated from: '<Root>/dutyU' */
  Controller_syncPI_Y.dutyU = rtb_d_uvw[0];

  /* Outport generated from: '<Root>/dutyV' */
  Controller_syncPI_Y.dutyV = rtb_d_uvw[1];

  /* Outport generated from: '<Root>/dutyW' */
  Controller_syncPI_Y.dutyW = rtb_d_uvw[2];
}

/* Model initialize function */
void Controller_syncPI_initialize(void)
{
  /* InitializeConditions for Delay: '<S20>/Delay' */
  Controller_syncPI_DW.icLoad = true;

  /* InitializeConditions for Delay: '<S21>/Delay1' */
  Controller_syncPI_DW.icLoad_h = true;

  /* InitializeConditions for Delay: '<Root>/Delay1' */
  Controller_syncPI_DW.icLoad_e = true;

  /* InitializeConditions for Delay: '<Root>/Delay' */
  Controller_syncPI_DW.icLoad_e0 = true;
}

/* Model terminate function */
void Controller_syncPI_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
