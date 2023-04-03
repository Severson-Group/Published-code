#ifndef TASK_DPNV_CC_H
#define TASK_DPNV_CC_H

#include "sys/scheduler.h"
#include "drv/analog.h"
#include "drv/encoder.h"

#define TASK_DPNV_CC_UPDATES_PER_SEC (10000)
#define TASK_DPNV_CC_INTERVAL_USEC   (USEC_IN_SEC / TASK_DPNV_CC_UPDATES_PER_SEC)


// calibrate AMDS sampling
// suspension
#define PHASE_A_SUS_CURRENT_GAIN_AMDS (0.862)
#define PHASE_B_SUS_CURRENT_GAIN_AMDS (0.840)
#define PHASE_C_SUS_CURRENT_GAIN_AMDS (0.849)

#define PHASE_A_SUS_CURRENT_OFFSET_AMDS (0.184)
#define PHASE_B_SUS_CURRENT_OFFSET_AMDS (0.263)
#define PHASE_C_SUS_CURRENT_OFFSET_AMDS (0.209)

#define PHASE_A_TOR_CURRENT_GAIN_AMDS (1.628)
#define PHASE_B_TOR_CURRENT_GAIN_AMDS (1.638)
#define PHASE_C_TOR_CURRENT_GAIN_AMDS (1.583)

#define PHASE_A_TOR_CURRENT_OFFSET_AMDS (4.843)
#define PHASE_B_TOR_CURRENT_OFFSET_AMDS (4.879)
#define PHASE_C_TOR_CURRENT_OFFSET_AMDS (4.714)
#define Vdc (25)  // [V]

#define ENCODER_PULSES_PER_REV_BITS (13)
#define ENCODER_PULSES_PER_REV (1 << ENCODER_PULSES_PER_REV_BITS)
#define Offset (215)
#define POLE_PAIRS (1)

typedef enum {
    CABINET_CS_SUS_A = 0,
    CABINET_CS_SUS_B,
    CABINET_CS_SUS_C,
    CABINET_CS_TOR_A,
    CABINET_CS_TOR_B,
    CABINET_CS_TOR_C,
} cabinet_cs_e;
// Called in app init function to set up task (or via command)
int task_dpnv_cc_init(void);

// Callback function which scheduler calls periodically
void task_dpnv_cc_callback(void *arg);

// task stuff
void task_dpnv_cc_stats_print(void);

void task_dpnv_cc_stats_reset(void);

// function to sample currents in AMDS
int read_cs(cabinet_cs_e cs, double *adc_volts_out);

#endif // TASK_RLCC_H
