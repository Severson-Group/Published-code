#ifdef APP_VDC_CTRL

#include <math.h>
#include <string.h>
#include "sys/scheduler.h"
#include "sys/task_stats.h"

#include "drv/pwm.h"
#include "drv/motherboard.h"
#include "drv/analog.h"
#include "drv/encoder.h"

#include "usr/vdc_ctrl/task_vdc_ctrl.h"
#include "usr/power_stack.h"
#include "usr/measurement_io.h"
#include "usr/machine.h"
#include "usr/buck.h"

#include "Vdc_Controller.h"

// Scheduler TCB which holds task "context"
static task_control_block_t tcb;

double Vdc_c = 0.0;
double Iload = 0.0;
double Vdc_buck = 0.0;
double Ibuckff = 0.0;

// Measurement Pool
int err_vdc = FAILURE;
int32_t adc_sample = 0;

// Log Vars
double LOG_buck_Vdcfilt;               /* '<Root>/LOG_buck_ElecP' */
double LOG_buck_Iload;             /* '<Root>/LOG_buck_signal2' */
double LOG_buck_Ibuckff;             /* '<Root>/LOG_buck_signal3' */
double LOG_buck_Vbuck_c;                /* '<Root>/LOG_buck_Vbuck_c' */
double LOG_buck_Vdc;                 /* '<Root>/LOG_buck_Vdc' */
double LOG_duty;
double LOG_buck_PowerBuck;           /* '<Root>/LOG_buck_PowerBuck' */
double LOG_buck_Iloadfilt;           /* '<Root>/LOG_buck_Iloadfilt' */

int task_vdc_ctrl_init(void)
{
	if (scheduler_tcb_is_registered(&tcb)) {
		return FAILURE;
	}

	// Fill TCB with parameters
	scheduler_tcb_init(&tcb, task_vdc_ctrl_callback,
						NULL, "CVCR", TASK_VDC_CTRL_INTERVAL_USEC);

	// Enable Task Profiling
	task_stats_enable(&tcb.stats);

	/*
	// Initialize signal injection points
	injection_ctx_init(&inj_ctx_ctrl[0], "Id_inj");
	injection_ctx_init(&inj_ctx_ctrl[1], "Iq_inj");
	injection_ctx_init(&inj_ctx_ctrl[2], "Vd_inj");
	injection_ctx_init(&inj_ctx_ctrl[3], "Vq_inj");

	// Register all signal injection points
	for (int i = 0; i < ARRAY_SIZE(inj_ctx_ctrl); i++) {
		injection_ctx_register(&inj_ctx_ctrl[i]);
	}
	*/

 // THESE ARE ALL ENABLED BY MOTOR CONTROLLER ***************************
	// Set HW PWM frequency, deadtime, and initialize duty ratios
	pwm_set_deadtime_ns((double)BUCK_DEADTIME);
	pwm_set_switching_freq((double)BUCK_PWM_FREQ);

	// Update PWM
	pwm_set_duty(PWM_BUCK, 0.0);
    pwm_enable_hw(true);

	// Initialize CVCR Controller
	Vdc_Controller_initialize();

	// Register task with scheduler
	return scheduler_tcb_register(&tcb);
}

int task_vdc_ctrl_deinit(void)
{
	/*
	 // DONT TOUCH PWMS IN HERE. MOTOR CONTROL HAS IT.
	// Turn off PWMs
	pwm_enable_hw(false);
	pwm_set_duty(PWM_BUCK, 0.0);
*/
	// set all commands to zero
	Vdc_Controller_U.Vdcc = 0.0;

	// Deinit CVCR controller
	Vdc_Controller_terminate();

	// Clear CVCR control state variables
	memset(((void *) &Vdc_Controller_DW), 0, sizeof(DW_Vdc_Controller_T));

    return scheduler_tcb_unregister(&tcb);
}

void task_vdc_ctrl_callback(void *arg)
{
	double Vbuck;

	// Get Samples
	err_vdc = motherboard_get_data(MOTHERBOARD_2_BASE_ADDR, ADC_I_DC, &adc_sample);
	Iload = (double) Idc_CURRENT_GAIN*(adc_sample - IDC_CURRENT_OFFSET_COUNT);

	err_vdc = motherboard_get_data(MOTHERBOARD_2_BASE_ADDR, ADC_V_DC, &adc_sample);
	Vdc_buck = (double) Vdc_VOLTAGE_GAIN*(adc_sample + Vdc_VOLTAGE_OFFSET_COUNT);

	Vdc_Controller_U.Vdcc = Vdc_c;                         /* '<Root>/Vdcc' */
	Vdc_Controller_U.Vdc = Vdc_buck;                          /* '<Root>/Vdc' */
	Vdc_Controller_U.Iload = Iload;                        /* '<Root>/B_-1_-1' */
	Vdc_Controller_U.Ibuckff = Ibuckff;                      /* '<Root>/B_-1_-1' */

	// Perform signal injections
    //injection_inj(&Idc, &inj_ctx_ctrl[0], Ts);
    //injection_inj(&Iqc, &inj_ctx_ctrl[1], Ts);

	// Do the thing!
	Vdc_Controller_step();

	// Update Outputs
	Vbuck = Vdc_Controller_Y.H41;                          /* '<Root>/H41' */

	pwm_set_duty(PWM_BUCK, Vbuck);

	// Update Log Vars
	LOG_buck_Vdcfilt = Vdc_Controller_Y.LOG_buck_Vdcfilt;               /* '<Root>/LOG_buck_ElecP' */
	LOG_buck_Iload = Vdc_Controller_Y.LOG_buck_Iload;             /* '<Root>/LOG_buck_signal2' */
	LOG_buck_Ibuckff = Vdc_Controller_Y.LOG_buck_Ibuckff;             /* '<Root>/LOG_buck_signal3' */
	LOG_buck_Vbuck_c = Vdc_Controller_Y.LOG_buck_Vbuck_c;                /* '<Root>/LOG_buck_Vp_c' */
	LOG_buck_Vdc =  Vdc_Controller_Y.LOG_buck_Vdc;                /* '<Root>/LOG_buck_Vdc' */
	LOG_duty = Vdc_Controller_Y.LOG_buck_H41;
	LOG_buck_PowerBuck = Vdc_Controller_Y.LOG_buck_PowerBuck;           /* '<Root>/LOG_buck_PowerBuck' */
	LOG_buck_Iloadfilt = Vdc_Controller_Y.LOG_buck_Iloadfilt;           /* '<Root>/LOG_buck_Iloadfilt' */
}


int task_vdc_ctrl_set_vdc(double new_vdc_c)
{
	Vdc_c = new_vdc_c;

    return SUCCESS;
}

void task_vdc_ctrl_stats_print(void)
{
    task_stats_print(&tcb.stats);
}

void task_vdc_ctrl_stats_reset(void)
{
    task_stats_reset(&tcb.stats);
}


#endif // APP_VDC_CTRL
