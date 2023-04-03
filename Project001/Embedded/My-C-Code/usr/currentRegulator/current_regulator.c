/*
 * current_regulator.c
 *
 *  Created on: 24-Oct-2022
 *      Author: aniru
 */
#ifdef CURRENT_REGULATOR

#include "usr/currentRegulator/current_regulator.h"
#include "sys/scheduler.h"
#include "drv/pwm.h"
#include <math.h>
#include "drv/motherboard.h"
#include "Controller_syncPI.h"
#include "sys/injection.h"
#include "sys/cmd/cmd_inj.h"
#include "sys/util.h"
#include "sys/defines.h"

// Scheduler TCB which holds task "context"
static task_control_block_t tcb;
// Initialize signal injection points
static inj_ctx_t inj_ctx_ctrl[7] = { 0 };

static double enc_theta_m_offset;
static double omega_m; // Rotor speed to calculate speed dependent offset


int read_cs(cabinet_cs_e cs, double *adc_volts_out)
{
    int err = FAILURE;
    int32_t out = 0;

    uint32_t MB_BASE_ADDR = MOTHERBOARD_1_BASE_ADDR;
    switch (cs) {
    case CABINET_CS_SUS_U:
        err = motherboard_get_data(MB_BASE_ADDR, MB_IN6, &out);
        *adc_volts_out = 0.00125 * (double) out;
        break;

    case CABINET_CS_SUS_V:
        err = motherboard_get_data(MB_BASE_ADDR, MB_IN4, &out);
        *adc_volts_out = 0.00125 * (double) out;
        break;

    case CABINET_CS_SUS_W:
        err = motherboard_get_data(MB_BASE_ADDR, MB_IN2, &out);
        *adc_volts_out = 0.00125 * (double) out;
        break;

    case CABINET_CS_TOR_U:
		err = motherboard_get_data(MB_BASE_ADDR, MB_IN5, &out);
		*adc_volts_out = 0.00125 * (double) out;
		break;

	case CABINET_CS_TOR_V:
		err = motherboard_get_data(MB_BASE_ADDR, MB_IN3, &out);
		*adc_volts_out = 0.00125 * (double) out;
		break;

	case CABINET_CS_TOR_W:
		err = motherboard_get_data(MB_BASE_ADDR, MB_IN1, &out);
		*adc_volts_out = 0.00125 * (double) out;
		break;

    default:
        err = FAILURE;
    }

    // Magic delta value to deal with odd analog design... :(
    // Specific to CurrentCard design, perhaps VoltageCard as well.
    //
    // ==> at 0A input, the CurrentCard ADC voltage input is mid-rail,
    //     about 2.4V, and -FS current is about 0.2V, +FS is ~4.8V.
    //
    //     The binary ADC codes for these are odd, they go from
    //     negative to positive weirdly. I haven't debugged if
    //     this is actually correct of not... So, this DELTA value
    //     shifts them around to get a nice linear output
    //     relative to the current input value.
    double DELTA = (double) 0xFFFF * 0.00125 / 2.0;
    if (*adc_volts_out > 0) {
        *adc_volts_out -= DELTA;
    } else {
        *adc_volts_out += DELTA;
    }

    return err;
}

double task_get_theta_e_enc(void)
{
    // Get raw encoder position
    uint32_t position;
    encoder_get_position(&position);

    double theta_e_enc;

    enc_theta_m_offset = 35.5 + 0.0382*omega_m;

    // Convert to radians
    theta_e_enc = (double) PI2 * ( ( (double)ENCODER_PULSES_PER_REV - (double)1 -(double)position + enc_theta_m_offset )/ (double) ENCODER_PULSES_PER_REV);

    // Multiple by pole pairs to convert mechanical to electrical degrees
    theta_e_enc *= POLE_PAIRS;

    // Mod by 2 pi
    while (theta_e_enc > PI2) {
        theta_e_enc -= PI2;
    }

    return theta_e_enc;
}



int task_current_regulator_init(void)
{
    if (scheduler_tcb_is_registered(&tcb)) {
        return FAILURE;
    }



    // Fill TCB with parameters
    scheduler_tcb_init(&tcb, task_current_regulator_callback,
                        NULL, "current regulator", TASK_CURRENTREGULATOR_INTERVAL_USEC );




    // Initialize the current sense variables
    Controller_syncPI_U.I_uvw_fb[0] = 0;
    Controller_syncPI_U.I_uvw_fb[1] = 0;
    Controller_syncPI_U.I_uvw_fb[2] = 0;

    Controller_syncPI_U.CMD_theta_m = 0;
    Controller_syncPI_U.CMD_I_d = 0;
    Controller_syncPI_U.CMD_I_q = 0;
    Controller_syncPI_U.CMD_speed = 0;

    Controller_syncPI_U.PARAMS_I_max = I_MAX;
    Controller_syncPI_U.PARAMS_drv_enable = 1;
    Controller_syncPI_U.PARAMS_cc_enable = 1;
    Controller_syncPI_U.PARAMS_speed_control = 1;
	Controller_syncPI_U.PARAMS_theta_m_init = task_get_theta_e_enc();
	Controller_syncPI_U.PARAMS_omega_m_init = 0;
	Controller_syncPI_U.PARAMS_selfsensing_enable = 0; // To use self sensing for current control
	Controller_syncPI_U.PARAMS_RPM_limit = 36000;

	// Fixed at 12 for now!! Add AMDS voltage sense later
    Controller_syncPI_U.V_dc = V_DC;



    injection_ctx_init(&inj_ctx_ctrl[0], "Id*");
    injection_ctx_init(&inj_ctx_ctrl[1], "Iq*");
    injection_ctx_init(&inj_ctx_ctrl[2], "Vd*");
    injection_ctx_init(&inj_ctx_ctrl[3], "Vq*");
    injection_ctx_init(&inj_ctx_ctrl[4], "theta*");
    injection_ctx_init(&inj_ctx_ctrl[5], "Vq**");
    injection_ctx_init(&inj_ctx_ctrl[6], "speed*");


    // Register all signal injection points
    for (int i = 0; i < ARRAY_SIZE(inj_ctx_ctrl); i++) {
        injection_ctx_register(&inj_ctx_ctrl[i]);
    }

    //pwm_set_all_duty_midscale();
    pwm_set_all_duty_midscale();

    //pwm_set_switching_freq(freq);

    // Register task with scheduler
    return scheduler_tcb_register(&tcb);
}

int task_current_regulator_deinit(void)
{
	for (int i = 0; i < ARRAY_SIZE(inj_ctx_ctrl); i++) {
	        injection_ctx_clear(&inj_ctx_ctrl[i]);
	    }
    // Clear state struct for Simulink controller
    memset(((void *)&Controller_syncPI_DW), 0, sizeof(DW_Controller_syncPI_T));

    return scheduler_tcb_unregister(&tcb);
}








double LOG_Id_ref = 0;
double LOG_Iq_ref = 0;
double LOG_Id = 0;
double LOG_Iq = 0;
double LOG_Iu = 0;
double LOG_Iv = 0;
double LOG_Iw = 0;
double LOG_Vu = 0;
double LOG_Vv = 0;
double LOG_Vw = 0;
double LOG_Vd = 0;
double LOG_Vq = 0;
double LOG_dutyU = 0;
double LOG_dutyV = 0;
double LOG_dutyW = 0;
double LOG_theta = 0;
double LOG_speed = 0;
double LOG_PWM_en = 0;
double LOG_encoder_raw = 0;
double LOG_speed_filt = 0;
double LOG_theta_hat = 0;
double LOG_speed_hat = 0;


double LOG_i_gamma = 0;
double LOG_i_delta = 0;
double LOG_v_gamma = 0;
double LOG_v_delta = 0;
double LOG_emf_gamma = 0;
double LOG_emf_delta = 0;


float LOG_isenseu = 0;
float LOG_isensev = 0;
float LOG_isensew = 0;



static double Vd_star = 0;
static double Vq_star = 0;


static double PWM_dutyU = 0.5;
static double PWM_dutyV = 0.5;
static double PWM_dutyW = 0.5;






double Ts_local = 1.0/(double)TASK_CURRENTREGULATOR_UPDATES_PER_SEC;


int counter = 0;

void task_current_regulator_callback(void *arg)
{

	// Write PWM duty cycles to FPGA peripheral at start of task
    pwm_set_duty(3, PWM_dutyU ); // Set HB1 duty ratio (INV1, PWM1 and PWM2)
    pwm_set_duty(4, PWM_dutyV); // Set HB2 duty ratio (INV1, PWM3 and PWM4)
    pwm_set_duty(5, PWM_dutyW); // Set HB3 duty ratio (INV1, PWM5 and PWM6)

	// Check pwm enable status
	if (Controller_syncPI_Y.PWM_enable) {
		pwm_enable_hw(true);
		pwm_enable();
	}
	else {
		pwm_disable();
		pwm_enable_hw(false);
	}

	LOG_PWM_en = Controller_syncPI_Y.PWM_enable;



	// Injection active
	 injection_inj(&Controller_syncPI_U.CMD_I_d, &inj_ctx_ctrl[0], Ts_local);
	 injection_inj(&Controller_syncPI_U.CMD_I_q, &inj_ctx_ctrl[1], Ts_local);
	 injection_inj(&Vd_star, &inj_ctx_ctrl[2], Ts_local);
	 injection_inj(&Vq_star, &inj_ctx_ctrl[3], Ts_local);
	// extra hook for injecting sine on a dc
	 injection_inj(&Vq_star, &inj_ctx_ctrl[5], Ts_local);
	 injection_inj(&Controller_syncPI_U.CMD_speed, &inj_ctx_ctrl[6], Ts_local);



	 Controller_syncPI_U.CMD_V_d = Vd_star;
	 Controller_syncPI_U.CMD_V_q = Vq_star;


    //Read voltage from analog channels from current sense
	//Scaling factor included in autogen code







	double Iut;
	double Ivt;
	double Iwt;
	double theta;





	uint32_t position;
	encoder_get_position(&position);
	LOG_encoder_raw = (double)position;


	theta = task_get_theta_e_enc();
	injection_inj(&theta, &inj_ctx_ctrl[4], Ts_local);
	//theta = 0;




	read_cs(CABINET_CS_TOR_U, &Iut);
	read_cs(CABINET_CS_TOR_V, &Ivt);
	read_cs(CABINET_CS_TOR_W, &Iwt);





    Controller_syncPI_U.I_uvw_fb[0] = (double)PHASE_U_TOR_CURRENT_GAIN_AMDS*(Iut - PHASE_U_TOR_CURRENT_OFFSET_AMDS);
    Controller_syncPI_U.I_uvw_fb[1] = (double)PHASE_V_TOR_CURRENT_GAIN_AMDS*(Ivt - PHASE_V_TOR_CURRENT_OFFSET_AMDS);
    Controller_syncPI_U.I_uvw_fb[2] = (double)PHASE_W_TOR_CURRENT_GAIN_AMDS*(Iwt - PHASE_W_TOR_CURRENT_OFFSET_AMDS);
    Controller_syncPI_U.CMD_theta_m = theta;


    LOG_isenseu = Iut;
    LOG_isensev = Ivt;
    LOG_isensew = Iwt;
    LOG_theta = theta;


    // Run control step
	Controller_syncPI_step();

    // Store PWM duty ratios
	PWM_dutyU = Controller_syncPI_Y.dutyU;
	PWM_dutyV = Controller_syncPI_Y.dutyV;
	PWM_dutyW = Controller_syncPI_Y.dutyW;





    omega_m = Controller_syncPI_Y.LOG_signals_omega_filt;

    // Log signals
     LOG_Id_ref = Controller_syncPI_Y.LOG_signals_i_dq_cmd[0];
     LOG_Iq_ref = Controller_syncPI_Y.LOG_signals_i_dq_cmd[1];
     LOG_Id = Controller_syncPI_Y.LOG_signals_i_dq_fb[0];
     LOG_Iq = Controller_syncPI_Y.LOG_signals_i_dq_fb[1];
     LOG_Iu = Controller_syncPI_Y.LOG_signals_i_uvw[0];
     LOG_Iv = Controller_syncPI_Y.LOG_signals_i_uvw[1];
     LOG_Iw = Controller_syncPI_Y.LOG_signals_i_uvw[2];
     LOG_Vu = Controller_syncPI_Y.LOG_signals_v_uvw[0];
     LOG_Vv = Controller_syncPI_Y.LOG_signals_v_uvw[1];
     LOG_Vw = Controller_syncPI_Y.LOG_signals_v_uvw[2];
     LOG_Vd = Controller_syncPI_Y.LOG_signals_v_dq[0];
     LOG_Vq = Controller_syncPI_Y.LOG_signals_v_dq[1];
     LOG_speed = Controller_syncPI_Y.LOG_signals_omega;
     LOG_speed_filt = Controller_syncPI_Y.LOG_signals_omega_filt;
     LOG_theta_hat = Controller_syncPI_Y.LOG_signals_theta_m_hat;
	 LOG_speed_hat = Controller_syncPI_Y.LOG_signals_omega_m_hat;
     LOG_dutyU = Controller_syncPI_Y.dutyU;
     LOG_dutyV = Controller_syncPI_Y.dutyV;
     LOG_dutyW = Controller_syncPI_Y.dutyW;



     // LOG BEMFO terms
     LOG_i_gamma = Controller_syncPI_Y.LOG_bemfo_i_dq[0];
     LOG_i_delta = Controller_syncPI_Y.LOG_bemfo_i_dq[1];
     LOG_v_gamma = Controller_syncPI_Y.LOG_bemfo_v_dq[0];
     LOG_v_delta = Controller_syncPI_Y.LOG_bemfo_v_dq[1];
     LOG_emf_gamma = Controller_syncPI_Y.LOG_bemfo_emf[0];
     LOG_emf_delta = Controller_syncPI_Y.LOG_bemfo_emf[1];

}

int task_current_regulator_set_frequency(double freq)
{
    return SUCCESS;
}

int task_current_regulator_set_id(double id)
{
	Controller_syncPI_U.CMD_I_d = id;
	return SUCCESS;
}

int task_current_regulator_set_iq(double iq)
{
	Controller_syncPI_U.CMD_I_q = iq;
	return SUCCESS;
}

int task_current_regulator_set_offset(double offset)
{
	enc_theta_m_offset = offset;
	return SUCCESS;
}


#endif // CURRENT_REGULATOR

