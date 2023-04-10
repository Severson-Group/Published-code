#ifndef TASK_CVCR_H
#define TASK_CVCR_H

#include "sys/scheduler.h"
#include <math.h>

#define TASK_CVCR_UPDATES_PER_SEC (10000)
#define TASK_CVCR_INTERVAL_USEC   (USEC_IN_SEC / TASK_CVCR_UPDATES_PER_SEC)

#define FREQUENCY_LIMIT (2*PI*600)

int task_cvcr_init(void);
int task_cvcr_deinit(void);

void task_cvcr_callback(void *arg);

int task_cvcr_enc_init(void);
int task_cvcr_set_injection(int32_t injection);
int task_cvcr_set_update_enable(int32_t new_pwm_update_enable);
int task_cvcr_set_theta_lock(int32_t new_theta_lock);
int task_cvcr_set_RPM(double new_RPMc);
int task_cvcr_set_iqc(double iqc);
int task_cvcr_set_idc(double idc);
int task_cvcr_set_Vdc(double new_Vdc);

#endif // TASK_CVCR_H
