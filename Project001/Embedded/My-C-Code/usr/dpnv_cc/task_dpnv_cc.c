#ifdef APP_DPNV_CC

#include "usr/dpnv_cc/task_dpnv_cc.h"
#include "usr/user_config.h"
#include <math.h>
#include <stdint.h>
#include "drv/pwm.h"

#include "sys/scheduler.h"
#include "drv/analog.h"
#include "drv/motherboard.h"
#include "AMDC_DPNV_CC.h"

// Scheduler TCB which holds task "context"
static task_control_block_t tcb;

// Example logging variables for testing
//float LOG_Ia_ss = 0;
//float LOG_Ib_ss = 0;
//float LOG_Ic_ss = 0;
float LOG_Ia_t = 0;
float LOG_Ib_t = 0;
float LOG_Ic_t = 0;

float LOG_Id = 0;
float LOG_Iq = 0;
//float LOG_Ix = 0;
//float LOG_Iy = 0;

float LOG_Vd = 0;
float LOG_Vq = 0;
float LOG_theta = 0;


static double Ts = 1.0 / 10000.0; // [sec]
static double omega = 0.0;       // [rad/s]


int task_dpnv_cc_init(void)
{
    if (scheduler_tcb_is_registered(&tcb)) {
        return FAILURE;
    }

    // Fill TCB with parameters
    scheduler_tcb_init(&tcb, task_dpnv_cc_callback, NULL, "dpnv_cc", TASK_DPNV_CC_INTERVAL_USEC);

    pwm_set_switching_freq(1/Ts);
	pwm_set_all_duty_midscale();

    // initialize autogen control signals
	AMDC_DPNV_CC_U.select_cc = 0;
	AMDC_DPNV_CC_U.drv_en = 0;

	// initialize autogen input commands
	AMDC_DPNV_CC_U.omega_e = omega;
	AMDC_DPNV_CC_U.Vdq_cmd[0] = 0;
	AMDC_DPNV_CC_U.Vdq_cmd[1] = 0;
	AMDC_DPNV_CC_U.Idq_cmd[0] = 0;
	AMDC_DPNV_CC_U.Idq_cmd[1] = 0;

	AMDC_DPNV_CC_U.Vxy_cmd[0] = 0;
	AMDC_DPNV_CC_U.Vxy_cmd[1] = 0;
	AMDC_DPNV_CC_U.Ixy_cmd[0] = 0;
	AMDC_DPNV_CC_U.Ixy_cmd[1] = 0;

	encoder_set_pulses_per_rev_bits(ENCODER_PULSES_PER_REV_BITS);
	AMDC_DPNV_CC_initialize();

    // Register task with scheduler
    return scheduler_tcb_register(&tcb);
}

int read_cs(cabinet_cs_e cs, double *adc_volts_out)
{
    int err = FAILURE;
    int32_t out = 0;

    switch (cs) {
    case CABINET_CS_SUS_A:
        err = motherboard_get_data(MB_IN6, &out);
        *adc_volts_out = 0.00125 * (double) out;
        break;

    case CABINET_CS_SUS_B:
        err = motherboard_get_data(MB_IN4, &out);
        *adc_volts_out = 0.00125 * (double) out;
        break;

    case CABINET_CS_SUS_C:
        err = motherboard_get_data(MB_IN2, &out);
        *adc_volts_out = 0.00125 * (double) out;
        break;

    case CABINET_CS_TOR_A:
		err = motherboard_get_data(MB_IN5, &out);
		*adc_volts_out = 0.00125 * (double) out;
		break;

	case CABINET_CS_TOR_B:
		err = motherboard_get_data(MB_IN3, &out);
		*adc_volts_out = 0.00125 * (double) out;
		break;

	case CABINET_CS_TOR_C:
		err = motherboard_get_data(MB_IN1, &out);
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

double task_cc_get_theta_e_enc(void)
{
    // Get raw encoder position
    uint32_t position;
    encoder_get_position(&position);

    // Add offset (align to DQ frame)
    position += 0;

    while (position >= ENCODER_PULSES_PER_REV) {
        position -= ENCODER_PULSES_PER_REV;
    }

    double theta_e_enc = 0.0;

    // Convert to radians
    theta_e_enc = (double) PI2 * ((double) position / (double) ENCODER_PULSES_PER_REV);

    // Multiple by pole pairs to convert mechanical to electrical degrees
    theta_e_enc *= POLE_PAIRS;

    // Mod by 2 pi
    while (theta_e_enc > PI2) {
        theta_e_enc -= PI2;
    }

    return theta_e_enc;
}

void task_dpnv_cc_callback(void *arg)
{
	// initial duty for each pole
	double VasDuty, VbsDuty, VcsDuty, VatDuty, VbtDuty, VctDuty;

	if (AMDC_DPNV_CC_Y.PWM_en) {
		pwm_enable();
	}
	else {
		pwm_disable();
	}
	// get currents using analog inputs
	//analog_getf(CC_PHASE_A_ADC, &Iabc_f[0]);

	//get currents using AMDS
	double Ias;
	double Ibs;
	double Ics;
	double Iat;
	double Ibt;
	double Ict;
	double theta;

	read_cs(CABINET_CS_SUS_A, &Ias);
	read_cs(CABINET_CS_SUS_B, &Ibs);
	read_cs(CABINET_CS_SUS_C, &Ics);
	read_cs(CABINET_CS_TOR_A, &Iat);
	read_cs(CABINET_CS_TOR_B, &Ibt);
	read_cs(CABINET_CS_TOR_C, &Ict);

	theta = task_cc_get_theta_e_enc();


	// scale currents
	AMDC_DPNV_CC_U.Iss_abc[0] = (double)PHASE_A_SUS_CURRENT_GAIN_AMDS*(Ias - PHASE_A_SUS_CURRENT_OFFSET_AMDS);
	AMDC_DPNV_CC_U.Iss_abc[1] = (double)PHASE_B_SUS_CURRENT_GAIN_AMDS*(Ibs - PHASE_B_SUS_CURRENT_OFFSET_AMDS);
	AMDC_DPNV_CC_U.Iss_abc[2] = (double)PHASE_C_SUS_CURRENT_GAIN_AMDS*(Ics - PHASE_C_SUS_CURRENT_OFFSET_AMDS);
	AMDC_DPNV_CC_U.It_abc[0] = (double)PHASE_A_TOR_CURRENT_GAIN_AMDS*(Iat - PHASE_A_TOR_CURRENT_OFFSET_AMDS);
	AMDC_DPNV_CC_U.It_abc[1] = (double)PHASE_B_TOR_CURRENT_GAIN_AMDS*(Ibt - PHASE_B_TOR_CURRENT_OFFSET_AMDS);
	AMDC_DPNV_CC_U.It_abc[2] = (double)PHASE_C_TOR_CURRENT_GAIN_AMDS*(Ict - PHASE_C_TOR_CURRENT_OFFSET_AMDS);


	// Update logging variables
//	LOG_Ia_ss = (AMDC_DPNV_CC_U.Iss_abc[0]);
//	LOG_Ib_ss = (AMDC_DPNV_CC_U.Iss_abc[1]);
//	LOG_Ic_ss = (AMDC_DPNV_CC_U.Iss_abc[2]);
	LOG_Ia_t = (AMDC_DPNV_CC_U.It_abc[0]);
	LOG_Ib_t = (AMDC_DPNV_CC_U.It_abc[1]);
	LOG_Ic_t = (AMDC_DPNV_CC_U.It_abc[2]);

	LOG_Id = (double) (AMDC_DPNV_CC_Y.Idq_act[0]);
	LOG_Iq = (double) (AMDC_DPNV_CC_Y.Idq_act[1]);
	LOG_Vd = (double) (AMDC_DPNV_CC_Y.Vdq_app[0]);
	LOG_Vq = (double) (AMDC_DPNV_CC_Y.Vdq_app[1]);
//	LOG_Ix = (double) (AMDC_DPNV_CC_Y.Ixy_act[0]);
//	LOG_Iy = (double) (AMDC_DPNV_CC_Y.Ixy_act[1]);

	LOG_theta = theta;

	// execute code
	AMDC_DPNV_CC_step();

	// assign torque inverter duty cycle (INV2 on AMDC)
	VatDuty = AMDC_DPNV_CC_Y.Vt_abc[0]/Vdc + 0.5;
	VbtDuty = AMDC_DPNV_CC_Y.Vt_abc[1]/Vdc + 0.5;
	VctDuty = AMDC_DPNV_CC_Y.Vt_abc[2]/Vdc + 0.5;

	pwm_set_duty(3, VatDuty);
	pwm_set_duty(4, VbtDuty);
	pwm_set_duty(5, VctDuty);

	// assign suspension inverter duty cycle (INV1 on AMDC)
	VasDuty = AMDC_DPNV_CC_Y.Vs_abc[0]/Vdc + 0.5;
	VbsDuty = AMDC_DPNV_CC_Y.Vs_abc[1]/Vdc + 0.5;
	VcsDuty = AMDC_DPNV_CC_Y.Vs_abc[2]/Vdc + 0.5;

	pwm_set_duty(0, VasDuty);
	pwm_set_duty(1, VbsDuty);
	pwm_set_duty(2, VcsDuty);
}

// task statistics stuff
void task_dpnv_cc_stats_print(void)
{
    task_stats_print(&tcb.stats);
}

void task_dpnv_cc_stats_reset(void)
{
    task_stats_reset(&tcb.stats);
}

#endif // APP_DPNV_CC
