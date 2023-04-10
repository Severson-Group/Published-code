#ifndef TASK_VDC_CTRL_H
#define TASK_VDC_CTRL_H

#include "sys/scheduler.h"

#define TASK_VDC_CTRL_UPDATES_PER_SEC (5000)
#define TASK_VDC_CTRL_INTERVAL_USEC   (USEC_IN_SEC / TASK_VDC_CTRL_UPDATES_PER_SEC)

int task_vdc_ctrl_init(void);
int task_vdc_ctrl_deinit(void);

void task_vdc_ctrl_callback(void *arg);

int task_vdc_ctrl_set_vdc(double new_vdc_c);

void task_vdc_ctrl_stats_print(void);
void task_vdc_ctrl_stats_reset(void);

#endif // TASK_VDC_CTRL_H
