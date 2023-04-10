#ifdef APP_CVCR

#include <math.h>
#include <string.h>
#include "sys/scheduler.h"
#include "sys/task_stats.h"
#include "sys/injection.h"
#include "sys/util.h"
#include "drv/pwm.h"
#include "drv/motherboard.h"
#include "drv/analog.h"
#include "drv/encoder.h"
#include "usr/cvcr/task_cvcr.h"
#include "usr/power_stack.h"
#include "usr/measurement_io.h"
#include "usr/machine.h"
#include "usr/buck.h"
#include "CVCR_Controller.h"

// Scheduler TCB which holds task "context"
static task_control_block_t tcb;

// Array of 4 contexts (i.e., 4 injection points)
static inj_ctx_t inj_ctx_ctrl[4] = { 0 };

// User vars
uint32_t encoderPos = 0;
static double Iqc = 0.0;
static double Idc = 0.0;
static double Vq_inj = 0.0;
static double Vd_inj = 0.0;
static double Vdc_spoof = 0.0;
static double RPMc = 0.0;
static double Ts = (double)(1/(double)TASK_CVCR_UPDATES_PER_SEC);
double Vdc = 0.0;
double Vdc_star = 0.0;
double theta = 0.0;
double thetaPrev = 0.0;
double filtWm = 0.0;
double filtWe = 0.0;
double WeInst = 0.0;

// AMDS
int32_t out = 0;
uint32_t MB_BASE_ADDR = MOTHERBOARD_2_BASE_ADDR;

// Flags & Counters
int32_t i = 0;
int32_t error = SUCCESS;
int32_t injection = 0;
int32_t theta_lock = 0;
int32_t PWM_update_enable = 0;

// Log Vars
// Error Vars
int32_t LOG_error;
//
double LOG_encoder;
double LOG_encoder_raw;
double LOG_Vdc_ADC;
double LOG_WeInst = 0.0;
// CVCR Log Vars
double LOG_CVCR_Iq;                  /* '<Root>/LOG_CVCR_Iq' */
double LOG_CVCR_Id;                  /* '<Root>/LOG_CVCR_Id' */
double LOG_CVCR_Iq_c;                /* '<Root>/LOG_CVCR_Iq_c' */
double LOG_CVCR_Id_c;                /* '<Root>/LOG_CVCR_Id_c' */
double LOG_CVCR_Iu;                  /* '<Root>/LOG_CVCR_Ia' */
double LOG_CVCR_Iv;                  /* '<Root>/LOG_CVCR_Iv' */
double LOG_CVCR_Iw;                  /* '<Root>/LOG_CVCR_Iw' */
double LOG_CVCR_Vdc;                 /* '<Root>/LOG_CVCR_Vdc' */
double LOG_CVCR_Vu_c;                /* '<Root>/LOG_CVCR_Vu_c' */
double LOG_CVCR_Vv_c;                /* '<Root>/LOG_CVCR_Vv_c' */
double LOG_CVCR_Vw_c;                /* '<Root>/LOG_CVCR_Vw_c' */
double LOG_CVCR_Theta;            /* '<Root>/LOG_CVCR_Theta_un' */
double LOG_CVCR_Wmfilt;              /* '<Root>/LOG_CVCR_Wmfilt' */
double LOG_CVCR_Wefilt;              /* '<Root>/LOG_CVCR_Wefilt' */
double LOG_CVCR_Vd_c;                /* '<Root>/LOG_CVCR_Vd_c' */
double LOG_CVCR_Vq_c;                /* '<Root>/LOG_CVCR_Vq_c' */
double LOG_CVCR_Vdcfilt;             /* '<Root>/LOG_CVCR_Vdcfilt' */
double LOG_CVCR_Vphasor;             /* '<Root>/LOG_CVCR_Vphasor' */
double LOG_CVCR_RPM_c;               /* '<Root>/LOG_CVCR_RPM_c' */
double LOG_CVCR_RPM;               /* '<Root>/LOG_CVCR_RPM' */
double LOG_CVCR_ElecP;               /* '<Root>/LOG_CVCR_ElecP' */
double LOG_CVCR_ElecQ;               /* '<Root>/LOG_CVCR_ElecQ' */
double LOG_CVCR_Vq_inj;              /* '<Root>/LOG_CVCR_Vq_inj' */
double LOG_CVCR_Vd_inj;              /* '<Root>/LOG_CVCR_Vd_inj' */
double LOG_CVCR_Ibuckff;             /* '<Root>/LOG_CVCR_Ibuckff' */
double LOG_CVCR_Vbuck_c;             /* '<Root>/LOG_CVCR_Vbuck_c' */
double LOG_CVCR_H41;                  /* '<Root>/LOG_CVCR_H41' */
double LOG_CVCR_PowerBuck;            /* '<Root>/LOG_CVCR_PowerBuck' */
double LOG_CVCR_Iloadfilt;           /* '<Root>/LOG_CVCR_Iloadfilt' */

int task_cvcr_init(void)
{
    if (scheduler_tcb_is_registered(&tcb)) {
        return FAILURE;
    }

    // Fill TCB with parameters
    scheduler_tcb_init(&tcb, task_cvcr_callback,
                        NULL, "CVCR", TASK_CVCR_INTERVAL_USEC);

    // Enable Task Profiling
    task_stats_enable(&tcb.stats);

	// Init Encoder
	encoder_init();

    // Set HW PWM frequency, deadtime, and initialize duty ratios
    pwm_set_deadtime_ns((double)POWERSTACK_DEADTIME);
    pwm_set_switching_freq((double)POWERSTACK_PWM_FREQ);
	// Update PWM
//    pwm_set_duty(PWM_U, 0.5);
//	pwm_set_duty(PWM_V, 0.5);
//	pwm_set_duty(PWM_W, 0.5);
	pwm_set_duty(PWM_BUCK, 0.0);

    pwm_enable_hw(true);

    // Initialize signal injection points
	injection_ctx_init(&inj_ctx_ctrl[0], "Id_inj");
	injection_ctx_init(&inj_ctx_ctrl[1], "Iq_inj");
	injection_ctx_init(&inj_ctx_ctrl[2], "Vd_inj");
	injection_ctx_init(&inj_ctx_ctrl[3], "Vq_inj");
	injection = 0;

	// Register all signal injection points
	for (int i = 0; i < ARRAY_SIZE(inj_ctx_ctrl); i++) {
		injection_ctx_register(&inj_ctx_ctrl[i]);
	}

	// Clear Command Inputs
	Iqc = 0.0;
	Idc = 0.0;
	Vdc = 0.0;
	RPMc = 0.0;
	Vd_inj = 0.0;
	Vq_inj = 0.0;
	CVCR_Controller_U.Iq_c = Iqc;                         /* '<Root>/Theta' */
	CVCR_Controller_U.Id_c = Idc;                         /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.RPM_C = RPMc;

	// Get rotor position
	encoder_set_pulses_per_rev_bits(MY_ENCODER_PULSES_PER_REV_BITS); 	// ensure encoder is updated
	encoder_get_position(&encoderPos);
	theta = encoderPos*PULSE_TO_RAD*POLEPAIRS + ENCODER_OFFSET_RAD; 	// Electrical angle [erad]
	CVCR_Controller_U.Theta = theta;
	thetaPrev = theta;
	CVCR_Controller_U.WeInst = 0.0;

	// Clear all inputs
	CVCR_Controller_U.Iu = 0.0;                           /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.Iv = 0.0;                           /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.Iw = 0.0;                           /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.WeInst = 0.0;

	CVCR_Controller_U.Vdcc = Vdc_star;
	// Get DC Link Voltage
	error = motherboard_get_data(MB_BASE_ADDR, ADC_V_DC, &out);
	CVCR_Controller_U.Vdc = (double) Vdc_VOLTAGE_GAIN*(out + Vdc_VOLTAGE_OFFSET_COUNT);//Vdc_spoof;                    /* '<Root>/B_-1_-1' */

	// Clear CVCR control state variables
	memset(((void *) &CVCR_Controller_DW), 0, sizeof(DW_CVCR_Controller_T));

	// Load delay states
//	CVCR_Controller_DW.delay_Theta_DSTATE = CVCR_Controller_U.Theta;	// prevent first sample from comparing to zero

    // Initialize CVCR Controller
    CVCR_Controller_initialize();

	// Update Log Vars
    LOG_encoder 	= 0.0;
	LOG_CVCR_Iq 	= 0.0;                  /* '<Root>/LOG_CVCR_Iq' */
	LOG_CVCR_Id 	= 0.0;                  /* '<Root>/LOG_CVCR_Id' */
	LOG_CVCR_Iq_c 	= 0.0;                /* '<Root>/LOG_CVCR_Iq_c' */
	LOG_CVCR_Id_c 	= 0.0;                /* '<Root>/LOG_CVCR_Id_c' */
	LOG_CVCR_Iu 	= 0.0;                  /* '<Root>/LOG_CVCR_Ia' */
	LOG_CVCR_Iv 	= 0.0;                  /* '<Root>/LOG_CVCR_Iv' */
	LOG_CVCR_Iw 	= 0.0;                  /* '<Root>/LOG_CVCR_Iw' */
	LOG_CVCR_Vdc 	= 0.0;                 /* '<Root>/LOG_CVCR_Vdc' */
	LOG_CVCR_Vu_c 	= 0.0;                /* '<Root>/LOG_CVCR_Vu_c' */
	LOG_CVCR_Vv_c 	= 0.0;                /* '<Root>/LOG_CVCR_Vv_c' */
	LOG_CVCR_Vw_c 	= 0.0;                /* '<Root>/LOG_CVCR_Vw_c' */
	LOG_CVCR_Theta 	= 0.0;          /* '<Root>/LOG_CVCR_Theta_un' *///elecPos; //
	LOG_CVCR_Wmfilt	= 0.0;              /* '<Root>/LOG_CVCR_WmFilt' */
	LOG_CVCR_Wefilt = 0.0;              /* '<Root>/LOG_CVCR_Wefilt' */
	LOG_CVCR_Vd_c 	= 0.0;                /* '<Root>/LOG_CVCR_Vd_c' */
	LOG_CVCR_Vq_c 	= 0.0;                /* '<Root>/LOG_CVCR_Vq_c' */
	LOG_CVCR_Vdcfilt	= 0.0;             /* '<Root>/LOG_CVCR_Vdcfilt' */
	LOG_CVCR_Vphasor	= 0.0;             /* '<Root>/LOG_CVCR_Vphasor' */
	LOG_CVCR_RPM_c 	= 0.0;
	LOG_CVCR_ElecP 	= 0.0;               /* '<Root>/LOG_CVCR_ElecP' */
	LOG_CVCR_ElecQ 	= 0.0;;               /* '<Root>/LOG_CVCR_ElecQ' */
	LOG_CVCR_Vq_inj	= 0.0;;              /* '<Root>/LOG_CVCR_Vq_inj' */
	LOG_CVCR_Vd_inj = 0.0;;              /* '<Root>/LOG_CVCR_Vd_inj' */
	LOG_WeInst = 0.0;

    // Register task with scheduler
    return scheduler_tcb_register(&tcb);
}

int task_cvcr_deinit(void)
{
	// Update PWM
	pwm_set_duty(PWM_U, 0.0);
	pwm_set_duty(PWM_V, 0.0);
	pwm_set_duty(PWM_W, 0.0);

	// Clear Command Inputs
	Iqc = 0.0;
	Idc = 0.0;
	RPMc = 0.0;
	Vd_inj = 0.0;
	Vq_inj = 0.0;

	// Clear all inputs
	CVCR_Controller_U.Theta = 0.0; //elecPos;
	CVCR_Controller_U.Iq_c = Iqc;                         /* '<Root>/Theta' */
	CVCR_Controller_U.Id_c = Idc;                         /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.Iu = 0.0;                           /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.Iv = 0.0;                           /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.Iw = 0.0;                           /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.Vdc = 0.0;                          /* '<Root>/B_-1_-1' */
	CVCR_Controller_U.RPM_C = RPMc;
	CVCR_Controller_U.WeInst = 0.0;
	PWM_update_enable = 0;

	// Return CVCR back to normal
	task_cvcr_set_injection(0);

	// Deinit CVCR controller
	CVCR_Controller_terminate();

	// Clear CVCR control state variables
	memset(((void *) &CVCR_Controller_DW), 0, sizeof(DW_CVCR_Controller_T));

	// TODO: Single use to catch over limit failure
	// Update to expand to other error types
	if(error != SUCCESS){
		// Tag error case
		LOG_error = 66;
	}

	pwm_enable_hw(false);

    return scheduler_tcb_unregister(&tcb);
}

void task_cvcr_callback(void *arg)
{
	double Iu, Iv, Iw, Iload;
	double Vuc, Vvc, Vwc, Vbuck;
	double temp;

	// Latch Encoder Pos
	encoder_get_position(&encoderPos);

	// Get Currents
	error = motherboard_get_data(MB_BASE_ADDR, ADC_I_U, &out);
	Iu = (double) PHASE_U_CURRENT_GAIN*(out - PHASE_U_CURRENT_OFFSET_COUNT);

	error = motherboard_get_data(MB_BASE_ADDR, ADC_I_V, &out);
	Iv = (double) PHASE_V_CURRENT_GAIN*(out - PHASE_V_CURRENT_OFFSET_COUNT);

	error = motherboard_get_data(MB_BASE_ADDR, ADC_I_W, &out);
	Iw = (double) PHASE_W_CURRENT_GAIN*(out - PHASE_W_CURRENT_OFFSET_COUNT);

	// Get DC Link Voltage
	error = motherboard_get_data(MB_BASE_ADDR, ADC_V_DC, &out);
	Vdc = (double) Vdc_VOLTAGE_GAIN*(out + Vdc_VOLTAGE_OFFSET_COUNT);

	error = motherboard_get_data(MB_BASE_ADDR, ADC_I_DC, &out);
	Iload = (double) Idc_CURRENT_GAIN*(out - IDC_CURRENT_OFFSET_COUNT);

	// Calc Rotor Position
	theta = encoderPos*PULSE_TO_RAD*POLEPAIRS + ENCODER_OFFSET_RAD; 	// Electrical angle [erad]

	//Unwrap for Omega
	WeInst = (double)((double)theta - (double)thetaPrev);

	if(WeInst < -PI){	// positive going wrap
		WeInst = (double)((theta + 4*PI) - thetaPrev);
	}
	else if (WeInst > PI){
		WeInst = (double)(theta - (thetaPrev + 4*PI));
	}
	temp = (double)WeInst;
	WeInst = (double)(temp/Ts);
	thetaPrev = (double)theta;

	// Limit Operating Frequency < 600Hz
	if (WeInst > FREQUENCY_LIMIT){
		WeInst = FREQUENCY_LIMIT;
	}

	CVCR_Controller_U.WeInst = WeInst;

	// Update input
	if(theta_lock){
		CVCR_Controller_U.Theta = 0.0; //--DEBUG
	}
	else{
		CVCR_Controller_U.Theta = theta;
	}

    // Perform signal injections
    injection_inj(&Idc, &inj_ctx_ctrl[0], Ts);
    injection_inj(&Iqc, &inj_ctx_ctrl[1], Ts);

    // Update command
	CVCR_Controller_U.Id_c = Idc;
	CVCR_Controller_U.Iq_c = Iqc;
	CVCR_Controller_U.Vdcc = Vdc_star;                         /* '<Root>/B_-1_-1' */

	// Update Current Input
	CVCR_Controller_U.Iu = Iu;
	CVCR_Controller_U.Iv = Iv;
	CVCR_Controller_U.Iw = Iw;
	CVCR_Controller_U.Iload = Iload;                        /* '<Root>/B_-1_-1' */

	// Update DC Link Voltage Input
	CVCR_Controller_U.Vdc = Vdc; //Vdc_spoof;//Vdc; //

	CVCR_Controller_U.RPM_C = RPMc;

	if(injection == 0)	// Run system as normal
	{
		// Do the thing!
		CVCR_Controller_step();

		// Phase Duty Ratio
		Vuc = CVCR_Controller_Y.H11;
		Vvc = CVCR_Controller_Y.H21;
		Vwc = CVCR_Controller_Y.H31;
		Vbuck = CVCR_Controller_Y.H41;

		// Update PWM
		if(PWM_update_enable){
			pwm_set_duty(PWM_U, Vuc);
			pwm_set_duty(PWM_V, Vvc);
			pwm_set_duty(PWM_W, Vwc);
			pwm_set_duty(PWM_BUCK, Vbuck);
		}

		// Update Log Vars
		LOG_encoder = theta;
		LOG_Vdc_ADC = Vdc;
		LOG_CVCR_Iq 	= CVCR_Controller_Y.LOG_CVCR_Iq;                  /* '<Root>/LOG_CVCR_Iq' */
		LOG_CVCR_Id 	= CVCR_Controller_Y.LOG_CVCR_Id;                  /* '<Root>/LOG_CVCR_Id' */
		LOG_CVCR_Iq_c 	= CVCR_Controller_Y.LOG_CVCR_Iq_c;                /* '<Root>/LOG_CVCR_Iq_c' */
		LOG_CVCR_Id_c 	= CVCR_Controller_Y.LOG_CVCR_Id_c;                /* '<Root>/LOG_CVCR_Id_c' */
		LOG_CVCR_Iu 	= CVCR_Controller_Y.LOG_CVCR_Iu;                  /* '<Root>/LOG_CVCR_Ia' */
		LOG_CVCR_Iv 	= CVCR_Controller_Y.LOG_CVCR_Iv;                  /* '<Root>/LOG_CVCR_Iv' */
		LOG_CVCR_Iw 	= CVCR_Controller_Y.LOG_CVCR_Iw;                  /* '<Root>/LOG_CVCR_Iw' */
		LOG_CVCR_Vdc 	= CVCR_Controller_Y.LOG_CVCR_Vdc;                 /* '<Root>/LOG_CVCR_Vdc' */
		LOG_CVCR_Vu_c 	= CVCR_Controller_Y.LOG_CVCR_Vu_c;                /* '<Root>/LOG_CVCR_Vu_c' */
		LOG_CVCR_Vv_c 	= CVCR_Controller_Y.LOG_CVCR_Vv_c;                /* '<Root>/LOG_CVCR_Vv_c' */
		LOG_CVCR_Vw_c 	= CVCR_Controller_Y.LOG_CVCR_Vw_c;                /* '<Root>/LOG_CVCR_Vw_c' */
		LOG_CVCR_Theta 	= CVCR_Controller_Y.LOG_CVCR_Theta;          	  /* '<Root>/LOG_CVCR_Theta_un' */
		LOG_CVCR_Wmfilt	= CVCR_Controller_Y.LOG_CVCR_Wmfilt;              /* '<Root>/LOG_CVCR_WmFilt' */
		LOG_CVCR_Wefilt = CVCR_Controller_Y.LOG_CVCR_Wefilt;              /* '<Root>/LOG_CVCR_Wefilt' */
		LOG_CVCR_Vd_c 	= CVCR_Controller_Y.LOG_CVCR_Vd_c;                /* '<Root>/LOG_CVCR_Vd_c' */
		LOG_CVCR_Vq_c 	= CVCR_Controller_Y.LOG_CVCR_Vq_c;                /* '<Root>/LOG_CVCR_Vq_c' */
		LOG_CVCR_Vdcfilt	= CVCR_Controller_Y.LOG_CVCR_Vdcfilt;         /* '<Root>/LOG_CVCR_Vdcfilt' */
		LOG_CVCR_Vphasor	= CVCR_Controller_Y.LOG_CVCR_Vphasor;         /* '<Root>/LOG_CVCR_Vphasor' */
		LOG_CVCR_RPM_c 	= CVCR_Controller_Y.LOG_CVCR_RPM_c;
		LOG_CVCR_RPM 	= CVCR_Controller_Y.LOG_CVCR_RPM;
		LOG_CVCR_ElecP 	= CVCR_Controller_Y.LOG_CVCR_ElecP;               /* '<Root>/LOG_CVCR_ElecP' */
		LOG_CVCR_ElecQ 	= CVCR_Controller_Y.LOG_CVCR_ElecQ;               /* '<Root>/LOG_CVCR_ElecQ' */
		LOG_CVCR_Ibuckff = CVCR_Controller_Y.LOG_CVCR_Ibuckff;             /* '<Root>/LOG_CVCR_Ibuckff' */
		LOG_CVCR_Vbuck_c = CVCR_Controller_Y.LOG_CVCR_Vbuck_c;             /* '<Root>/LOG_CVCR_Vbuck_c' */
		LOG_CVCR_H41 = CVCR_Controller_Y.LOG_CVCR_H41;                 /* '<Root>/LOG_CVCR_H41' */
		LOG_CVCR_PowerBuck = CVCR_Controller_Y.LOG_CVCR_PowerBuck;           /* '<Root>/LOG_CVCR_PowerBuck' */
		LOG_CVCR_Iloadfilt = CVCR_Controller_Y.LOG_CVCR_Iloadfilt;           /* '<Root>/LOG_CVCR_Iloadfilt' */
	}
	else	// Enable Vd Vq injection for System ID
	{
		// Perform signal injections
	    injection_inj(&Vd_inj, &inj_ctx_ctrl[2], Ts);
	    injection_inj(&Vq_inj, &inj_ctx_ctrl[3], Ts);

		// DQ to ABC for Injection
		Vuc = (double)(Vd_inj * cos(0.0 + theta) - Vq_inj*sin(0.0 + theta));
		Vvc = (double)(Vd_inj * cos(-2.0*PI/3.0 + theta) - Vq_inj*sin(-2.0*PI/3.0 + theta));
		Vwc = (double)(Vd_inj * cos(-4.0*PI/3.0 + theta) - Vq_inj*sin(-4.0*PI/3.0 + theta));

		// Prep duty ratio
		Vuc = Vuc/Vdc_spoof;
		Vvc = Vvc/Vdc_spoof;
		Vwc = Vwc/Vdc_spoof;

		// Update PWM
		pwm_set_duty(PWM_U, Vuc);
		pwm_set_duty(PWM_V, Vvc);
		pwm_set_duty(PWM_W, Vwc);

		// ABC to DQ for Currents
		LOG_CVCR_Id = (double)(2.0/3.0*(Iu*cos(0.0 + theta) + Iv*cos(theta - 2.0*PI/3.0) + Iw*cos(theta -4.0*PI/3.0)));
		LOG_CVCR_Iq = (double)(-2.0/3.0*(Iu*sin(0.0 + theta) + Iv*sin(theta - 2.0*PI/3.0) + Iw*sin(theta -4.0*PI/3.0)));

		LOG_CVCR_Vd_c = (double)(2.0/3.0*Vdc_spoof*(Vuc*cos(0.0 + theta) + Vvc*cos(theta - 2.0*PI/3.0) + Vwc*cos(theta -4.0*PI/3.0)));
		LOG_CVCR_Vq_c = (double)(-2.0/3.0*Vdc_spoof*(Vuc*sin(0.0 + theta) + Vvc*sin(theta - 2.0*PI/3.0) + Vwc*sin(theta -4.0*PI/3.0)));

		// Spoof Update Log Vars
		LOG_CVCR_Iu 	= Iu;
		LOG_CVCR_Iv 	= Iv;
		LOG_CVCR_Iw 	= Iw;
		LOG_CVCR_Vdc 	= Vdc_spoof;
		LOG_CVCR_Vu_c 	= Vuc*Vdc_spoof;
		LOG_CVCR_Vv_c 	= Vvc*Vdc_spoof;
		LOG_CVCR_Vw_c 	= Vwc*Vdc_spoof;
		LOG_CVCR_Theta 	= theta;
		LOG_CVCR_Vphasor	= (double)sqrt(LOG_CVCR_Vd_c*LOG_CVCR_Vd_c + LOG_CVCR_Vq_c*LOG_CVCR_Vq_c);
	}

	LOG_WeInst = (double)WeInst;
	LOG_encoder_raw = (double)encoderPos;
}

int task_cvcr_enc_init(void)
{
	// Init Encoder
	encoder_init();

    return SUCCESS;
}

int task_cvcr_set_RPM(double new_RPMc)
{
	RPMc = new_RPMc;
    return SUCCESS;
}

int task_cvcr_set_injection(int32_t new_injection)
{
	injection = new_injection;
	return SUCCESS;
}

int task_cvcr_set_update_enable(int32_t new_pwm_update_enable)
{
	PWM_update_enable = new_pwm_update_enable;
	return SUCCESS;
}

int task_cvcr_set_theta_lock(int32_t new_theta_lock)
{
	theta_lock = new_theta_lock;
	return SUCCESS;
}

int task_cvcr_set_iqc(double new_iqc)
{
	Iqc = new_iqc;
	return SUCCESS;
}

int task_cvcr_set_idc(double new_idc)
{
	Idc = new_idc;
	return SUCCESS;
}

int task_cvcr_set_Vdc(double new_Vdc)
{
	Vdc_spoof = new_Vdc;
	Vdc_star = new_Vdc;
	return SUCCESS;
}

void task_cvcr_stats_print(void)
{
    task_stats_print(&tcb.stats);
}

void task_cvcr_stats_reset(void)
{
    task_stats_reset(&tcb.stats);
}


#endif // TASK_CVCR
