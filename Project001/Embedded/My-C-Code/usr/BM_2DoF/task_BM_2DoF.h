#ifndef TASK_BM_2DoF_H
#define TASK_BM_2DoF_H

#include "sys/scheduler.h"

#define TASK_BM_2DoF_UPDATES_PER_SEC (25000)
#define TASK_BM_2DoF_INTERVAL_USEC   (USEC_IN_SEC / TASK_BM_2DoF_UPDATES_PER_SEC)


// calibrate AMDS sampling
// suspension
//#define PHASE_U_SUS_CURRENT_GAIN_AMDS (0.862)
//#define PHASE_V_SUS_CURRENT_GAIN_AMDS (0.840)
//#define PHASE_W_SUS_CURRENT_GAIN_AMDS (0.849)
//
//#define PHASE_U_SUS_CURRENT_OFFSET_AMDS (0.178)
//#define PHASE_V_SUS_CURRENT_OFFSET_AMDS (0.254)
//#define PHASE_W_SUS_CURRENT_OFFSET_AMDS (0.218)
//
//#define PHASE_U_TOR_CURRENT_GAIN_AMDS (1.612)
//#define PHASE_V_TOR_CURRENT_GAIN_AMDS (1.610)
//#define PHASE_W_TOR_CURRENT_GAIN_AMDS (1.616)
//
//#define PHASE_U_TOR_CURRENT_OFFSET_AMDS (4.844)
//#define PHASE_V_TOR_CURRENT_OFFSET_AMDS (4.785)
//#define PHASE_W_TOR_CURRENT_OFFSET_AMDS (4.749)


#define Vdc_tor (40)	  // [V]
#define Vdc_sus (40)	  // [V]
#define Imax (15)  		  // [A]

// Called in app init function to set up task (or via command)
int task_BM_2DoF_init(void);

// Callback function which scheduler calls periodically
void task_BM_2DoF_callback(void *arg);

// task stuff
void task_BM_2DoF_stats_print(void);

void task_BM_2DoF_stats_reset(void);

// function to sample currents in AMDS
//int read_cs(cabinet_cs_e cs, double *adc_volts_out);

#endif // TASK_RLCC_H
