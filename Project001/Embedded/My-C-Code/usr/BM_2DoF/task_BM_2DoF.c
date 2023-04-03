#ifdef APP_BM_2DoF

#include <math.h>
#include <stdint.h>
#include "drv/pwm.h"
#include "sys/injection.h"
#include "sys/task_stats.h"
#include "drv/eddy_current_sensor.h"

#include "BM_2DoF_Ctrl.h"
#include "usr/BM_2DoF/task_BM_2DoF.h"
#include "usr/BM_2DoF/sensors.h"
#include "usr/user_config.h"
// Scheduler TCB which holds task "context"
static task_control_block_t tcb;
static double Ts = 1.0 / TASK_BM_2DoF_UPDATES_PER_SEC; // [sec]

// logging variables
float LOG_Iu_ss = 0;
float LOG_Iv_ss = 0;
float LOG_Iw_ss = 0;
float LOG_Iu_t = 0;
float LOG_Iv_t = 0;
float LOG_Iw_t = 0;

//float LOG_Vu_t = 0;
//float LOG_Vv_t = 0;
//float LOG_Vw_t = 0;

float LOG_Id = 0;
float LOG_Iq = 0;
float LOG_Ix = 0;
float LOG_Iy = 0;
//float LOG_Id_cmd = 0;
//float LOG_Ix_cmd = 0;
//
//float LOG_Vd = 0;
//float LOG_Vq = 0;
//float LOG_Vx = 0;
//float LOG_Vy = 0;

float LOG_Fx = 0;
float LOG_Fy = 0;
//float LOG_Fx_dist = 0;
//float LOG_Fy_dist = 0;
//float LOG_x0 = 0;
//float LOG_y0 = 0;
//float LOG_x_pos = 0;
//float LOG_y_pos = 0;
//float LOG_theta = 0;
//float LOG_theta_hat = 0;
//float LOG_theta_star = 0;
//float LOG_omega_hat = 0;
//float LOG_torque = 0;

//float LOG_ctrl_dt = 0;
//float LOG_loop_dt = 0;

// injection variables
// 15 injection points
static inj_ctx_t inj_ctx_ctrl[15] = { 0 };


int task_BM_2DoF_init(void)
{
    if (scheduler_tcb_register_high_priority(&tcb)) {
        return FAILURE;
    }

    // Fill TCB with parameters
    scheduler_tcb_init(&tcb, task_BM_2DoF_callback, NULL, "BM_2DoF", TASK_BM_2DoF_INTERVAL_USEC);

    // set PWM frequency and initialize
    pwm_set_switching_freq(49000);
	pwm_set_all_duty_midscale();
	// set encoder resolution
	encoder_set_pulses_per_rev_bits(ENCODER_PULSES_PER_REV_BITS);

	// AMDS setup
	//	motherboard_set_adc_sampling(MOTHERBOARD_1_BASE_ADDR, true);

	// Initialize signal injection points
	injection_ctx_init(&inj_ctx_ctrl[0], "Id*");
	injection_ctx_init(&inj_ctx_ctrl[1], "Iq*");
	injection_ctx_init(&inj_ctx_ctrl[2], "Vd*");
	injection_ctx_init(&inj_ctx_ctrl[3], "Vq*");
	injection_ctx_init(&inj_ctx_ctrl[4], "W*");
	injection_ctx_init(&inj_ctx_ctrl[5], "theta*");
	injection_ctx_init(&inj_ctx_ctrl[6], "Ix*");
	injection_ctx_init(&inj_ctx_ctrl[7], "Iy*");
	injection_ctx_init(&inj_ctx_ctrl[8], "Vx*");
	injection_ctx_init(&inj_ctx_ctrl[9], "Vy*");
	injection_ctx_init(&inj_ctx_ctrl[10], "Sus_Off*");
//	injection_ctx_init(&inj_ctx_ctrl[10], "offset");
	injection_ctx_init(&inj_ctx_ctrl[11], "x*");
	injection_ctx_init(&inj_ctx_ctrl[12], "y*");
	injection_ctx_init(&inj_ctx_ctrl[13], "Fx*");
	injection_ctx_init(&inj_ctx_ctrl[14], "Fy*");

	// Register all signal injection points
	double ARRAY_SIZE;
	ARRAY_SIZE = sizeof(inj_ctx_ctrl)/sizeof(inj_ctx_ctrl[0]);
	for (int i = 0; i < ARRAY_SIZE; i++) {
		injection_ctx_register(&inj_ctx_ctrl[i]);
	}

    // initialize autogen control signals
	BM_2DoF_Ctrl_U.PARAMS_Sus_CC = 0;
	BM_2DoF_Ctrl_U.PARAMS_Tor_CC = 0;
	BM_2DoF_Ctrl_U.PARAMS_drv_EN = 0;
	BM_2DoF_Ctrl_U.PARAMS_w_Control = 0;
	BM_2DoF_Ctrl_U.PARAMS_ObsEn = 0;
	BM_2DoF_Ctrl_U.PARAMS_UseObs = 0;
	BM_2DoF_Ctrl_U.PARAMS_susp_Control = 0;
	// set DC link voltage and coil current limit
	BM_2DoF_Ctrl_U.PARAMS_I_max = Imax;
	BM_2DoF_Ctrl_U.FDB_Vdc_tor = Vdc_tor;
	BM_2DoF_Ctrl_U.FDB_Vdc_sus = Vdc_sus;

	// initialize autogen input commands
	BM_2DoF_Ctrl_U.CMD_V_dq[0] = 0;
	BM_2DoF_Ctrl_U.CMD_V_dq[1] = 0;
	BM_2DoF_Ctrl_U.CMD_I_dq[0] = 0;
	BM_2DoF_Ctrl_U.CMD_I_dq[1] = 0;
	BM_2DoF_Ctrl_U.CMD_V_xy[0] = 0;
	BM_2DoF_Ctrl_U.CMD_V_xy[1] = 0;
	BM_2DoF_Ctrl_U.CMD_I_xy[0] = 0;
	BM_2DoF_Ctrl_U.CMD_I_xy[1] = 0;
	BM_2DoF_Ctrl_U.CMD_Fx_dist = 0;
	BM_2DoF_Ctrl_U.CMD_Fy_dist = 0;
	BM_2DoF_Ctrl_U.CMD_x0 = 0;
	BM_2DoF_Ctrl_U.CMD_y0 = 0;

	// initialize Matlab Autogen
	BM_2DoF_Ctrl_initialize();

    // Register task with scheduler
    return scheduler_tcb_register(&tcb);
}

void task_setup(void) {
    // enable stats for task
    task_stats_enable(&tcb.stats);
}

void task_BM_2DoF_clear(void)
{
    // Clear all injection points
	double ARRAY_SIZE;
	ARRAY_SIZE = sizeof(inj_ctx_ctrl)/sizeof(inj_ctx_ctrl[0]);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        injection_ctx_clear(&inj_ctx_ctrl[i]);
    }
}

static double Id_star = 0;
static double Iq_star = 0;
static double Vd_star = 0;
static double Vq_star = 0;
static double W_star = 0;
static double theta_star = 0;
static double Ix_star = 0;
static double Iy_star = 0;
static double Vx_star = 0;
static double Vy_star = 0;
static double sus_offset_star = 0;
static double x0_star = 0;
static double y0_star = 0;
static double Fx_star = 0;
static double Fy_star = 0;

//static uint32_t last_begin = 0;

uint32_t V_cnt_prev = 0;
uint32_t V_cnt_now = 0;

//float LOG_my_counter = 0;
//float LOG_my_counter_max = 0;


void task_BM_2DoF_callback(void *arg)
{
	// get time at beginning of loop
//	uint32_t begin = cpu_timer_now();
//	uint32_t loop_dt = begin - last_begin;
//	LOG_loop_dt = cpu_timer_ticks_to_usec(loop_dt);
//	last_begin = begin;

//	// request data from AMDS
//	motherboard_request_new_data(MOTHERBOARD_1_BASE_ADDR);

	if (BM_2DoF_Ctrl_Y.PWM_en) {
		pwm_enable_hw(true);
		pwm_enable();
	}
	else {
		pwm_disable();
		pwm_enable_hw(false);
	}
	// get currents using analog inputs
	//analog_getf(CC_PHASE_A_ADC, &Iabc_f[0]);

//#if 1
//	// poll counters until change obtained
//	while (V_cnt_prev == V_cnt_now){
//		motherboard_get_counters(MOTHERBOARD_1_BASE_ADDR, &V_cnt_now, NULL, NULL);
//	}
//	// set previous counter to current value
//	V_cnt_prev = V_cnt_now;
//
//	// request for new data again to discard old data
//	motherboard_request_new_data(MOTHERBOARD_1_BASE_ADDR);
//	while (V_cnt_prev == V_cnt_now){
//		motherboard_get_counters(MOTHERBOARD_1_BASE_ADDR, &V_cnt_now, NULL, NULL);
//	}
//	// set previous counter to current value
//	V_cnt_prev = V_cnt_now;
//#endif

	//get currents from AMDS
	double Iss_u, Iss_v, Iss_w, It_u, It_v, It_w;
	get_cabinet_currents(&Iss_u, &Iss_v, &Iss_w, &It_u, &It_v, &It_w);

	// get position from encoder
	double theta;
	theta = task_get_theta_e_enc();

	// or create position artificially using ramp injection
	injection_inj(&theta_star, &inj_ctx_ctrl[5], Ts);
	injection_inj(&sus_offset_star, &inj_ctx_ctrl[10], Ts);
	BM_2DoF_Ctrl_U.FDB_sus_offset = sus_offset_star;

	double x,y;
	eddy_current_sensor_xy_mm(&x, &y);

	// sensor inputs
	BM_2DoF_Ctrl_U.FDB_theta_m = (theta_star) * PI/180;
//	BM_2DoF_Ctrl_U.FDB_theta_m = theta - ENCODER_OFFSET;
//	BM_2DoF_Ctrl_U.FDB_theta_m = 0;
	BM_2DoF_Ctrl_U.FDB_Iss_uvw[0] = Iss_u;
	BM_2DoF_Ctrl_U.FDB_Iss_uvw[1] = Iss_v;
	BM_2DoF_Ctrl_U.FDB_Iss_uvw[2] = Iss_w;
	BM_2DoF_Ctrl_U.FDB_It_uvw[0] = It_u;
	BM_2DoF_Ctrl_U.FDB_It_uvw[1] = It_v;
	BM_2DoF_Ctrl_U.FDB_It_uvw[2] = It_w;
	BM_2DoF_Ctrl_U.FDB_x0 = x/1000;
	BM_2DoF_Ctrl_U.FDB_y0 = y/1000;


	// Perform signal injections
	injection_inj(&Id_star, &inj_ctx_ctrl[0], Ts);
	injection_inj(&Iq_star, &inj_ctx_ctrl[1], Ts);
	injection_inj(&Vd_star, &inj_ctx_ctrl[2], Ts);
	injection_inj(&Vq_star, &inj_ctx_ctrl[3], Ts);
	injection_inj(&W_star, &inj_ctx_ctrl[4], Ts);
	injection_inj(&Ix_star, &inj_ctx_ctrl[6], Ts);
	injection_inj(&Iy_star, &inj_ctx_ctrl[7], Ts);
	injection_inj(&Vx_star, &inj_ctx_ctrl[8], Ts);
	injection_inj(&Vy_star, &inj_ctx_ctrl[9], Ts);
	injection_inj(&x0_star, &inj_ctx_ctrl[11], Ts);
	injection_inj(&y0_star, &inj_ctx_ctrl[12], Ts);
	injection_inj(&Fx_star, &inj_ctx_ctrl[13], Ts);
	injection_inj(&Fy_star, &inj_ctx_ctrl[14], Ts);

	// command inputs
	BM_2DoF_Ctrl_U.CMD_I_dq[0] = Id_star;
	BM_2DoF_Ctrl_U.CMD_I_dq[1] = Iq_star;
	BM_2DoF_Ctrl_U.CMD_V_dq[0] = Vd_star;
	BM_2DoF_Ctrl_U.CMD_V_dq[1] = Vq_star;
	BM_2DoF_Ctrl_U.CMD_w_m = W_star;
	BM_2DoF_Ctrl_U.CMD_I_xy[0] = Ix_star;
	BM_2DoF_Ctrl_U.CMD_I_xy[1] = Iy_star;
	BM_2DoF_Ctrl_U.CMD_V_xy[0] = Vx_star;
	BM_2DoF_Ctrl_U.CMD_V_xy[1] = Vy_star;
	BM_2DoF_Ctrl_U.CMD_x0 = x0_star;
	BM_2DoF_Ctrl_U.CMD_y0 = y0_star;
	BM_2DoF_Ctrl_U.CMD_Fx_dist = Fx_star;
	BM_2DoF_Ctrl_U.CMD_Fy_dist = Fy_star;

	//  LOG INPUT VARIABLES
	LOG_Iu_ss = (BM_2DoF_Ctrl_U.FDB_Iss_uvw[0]);
	LOG_Iv_ss = (BM_2DoF_Ctrl_U.FDB_Iss_uvw[1]);
	LOG_Iw_ss = (BM_2DoF_Ctrl_U.FDB_Iss_uvw[2]);
	LOG_Iu_t = (BM_2DoF_Ctrl_U.FDB_It_uvw[0]);
	LOG_Iv_t = (BM_2DoF_Ctrl_U.FDB_It_uvw[1]);
	LOG_Iw_t = (BM_2DoF_Ctrl_U.FDB_It_uvw[2]);

//	LOG_theta = (theta_star - offset) * PI/180;;
//	LOG_theta_star = theta_star;
//	LOG_x_pos = x;
//	LOG_y_pos = y;
//	LOG_x0 = (double) (x0_star);
//	LOG_y0 = (double) (y0_star);

	// execute code
	BM_2DoF_Ctrl_step();

	// LOG OUTPUT VARIABLES
//	LOG_Vd = (double) (BM_2DoF_Ctrl_Y.Vdq_app[0]);
//	LOG_Vq = (double) (BM_2DoF_Ctrl_Y.Vdq_app[1]);
//	LOG_Vx = (double) (BM_2DoF_Ctrl_Y.Vxy_app[0]);
//	LOG_Vy = (double) (BM_2DoF_Ctrl_Y.Vxy_app[1]);
//
	LOG_Id = (double) (BM_2DoF_Ctrl_Y.Idq_act[0]);
	LOG_Iq = (double) (BM_2DoF_Ctrl_Y.Idq_act[1]);
	LOG_Ix = (double) (BM_2DoF_Ctrl_Y.Ixy_act[0]);
	LOG_Iy = (double) (BM_2DoF_Ctrl_Y.Ixy_act[1]);
//	LOG_Id_cmd = (double) (Id_star);
//	LOG_Ix_cmd = (double) (Ix_star);

//	LOG_omega_hat = (double) (BM_2DoF_Ctrl_Y.w_hat);
//	LOG_theta_hat = (double) (BM_2DoF_Ctrl_Y.theta_hat);
//	LOG_torque = (double) (BM_2DoF_Ctrl_Y.Te_speed);
	LOG_Fx = (double) (BM_2DoF_Ctrl_Y.Fx);
	LOG_Fy = (double) (BM_2DoF_Ctrl_Y.Fy);

	double UsDuty, VsDuty, WsDuty, UtDuty, VtDuty, WtDuty;
	// assign torque inverter duty cycle (INV2 on AMDC)
	UtDuty = BM_2DoF_Ctrl_Y.Vt_uvw[0]/Vdc_tor + 0.5;
	VtDuty = BM_2DoF_Ctrl_Y.Vt_uvw[1]/Vdc_tor + 0.5;
	WtDuty = BM_2DoF_Ctrl_Y.Vt_uvw[2]/Vdc_tor + 0.5;

	// log each phase voltage for system ID
//	LOG_Vu_t = (BM_2DoF_Ctrl_Y.Vt_uvw[0]);
//	LOG_Vv_t = (BM_2DoF_Ctrl_Y.Vt_uvw[1]);
//	LOG_Vw_t = (BM_2DoF_Ctrl_Y.Vt_uvw[2]);


	pwm_set_duty(3, UtDuty);
	pwm_set_duty(4, VtDuty);
	pwm_set_duty(5, WtDuty);

	// assign suspension inverter duty cycle (INV1 on AMDC)
	UsDuty = BM_2DoF_Ctrl_Y.Vs_uvw[0]/Vdc_sus + 0.5;
	VsDuty = BM_2DoF_Ctrl_Y.Vs_uvw[1]/Vdc_sus + 0.5;
	WsDuty = BM_2DoF_Ctrl_Y.Vs_uvw[2]/Vdc_sus + 0.5;

	pwm_set_duty(0, UsDuty);
	pwm_set_duty(1, VsDuty);
	pwm_set_duty(2, WsDuty);

	// get time at end of loop
//	uint32_t end = cpu_timer_now();
//	uint32_t dt = end-begin;
//	// get time needed for code
//	LOG_ctrl_dt = cpu_timer_ticks_to_usec(dt);
}

// task statistics stuff
void task_BM_2DoF_stats_print(void)
{
    task_stats_print(&tcb.stats);
}

void task_BM_2DoF_stats_reset(void)
{
    task_stats_reset(&tcb.stats);
}

#endif // APP_DPNV_CC
