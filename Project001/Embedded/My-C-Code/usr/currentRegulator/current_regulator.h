/*
 * current_regulator.h
 *
 *  Created on: 24-Oct-2022
 *      Author: aniru
 */

#ifndef USR_CURRENTREGULATOR_CURRENT_REGULATOR_H_
#define USR_CURRENTREGULATOR_CURRENT_REGULATOR_H_

#include "sys/scheduler.h"

#define TASK_CURRENTREGULATOR_UPDATES_PER_SEC (10000)
#define TASK_CURRENTREGULATOR_INTERVAL_USEC   (USEC_IN_SEC / TASK_CURRENTREGULATOR_UPDATES_PER_SEC)

// calibrate AMDS sampling
// suspension
#define PHASE_U_SUS_CURRENT_GAIN_AMDS (0.862)
#define PHASE_V_SUS_CURRENT_GAIN_AMDS (0.840)
#define PHASE_W_SUS_CURRENT_GAIN_AMDS (0.849)

#define PHASE_U_SUS_CURRENT_OFFSET_AMDS (0.184)
#define PHASE_V_SUS_CURRENT_OFFSET_AMDS (0.263)
#define PHASE_W_SUS_CURRENT_OFFSET_AMDS (0.209)


// Modify gains -> to twice of what Bharat had.
#define PHASE_U_TOR_CURRENT_GAIN_AMDS (1.553)
#define PHASE_V_TOR_CURRENT_GAIN_AMDS (1.612)
#define PHASE_W_TOR_CURRENT_GAIN_AMDS (1.619)


// Modify offsets -> after running curr cal routine
#define PHASE_U_TOR_CURRENT_OFFSET_AMDS (4.738)
#define PHASE_V_TOR_CURRENT_OFFSET_AMDS (4.679)
#define PHASE_W_TOR_CURRENT_OFFSET_AMDS (4.619)

#define ENCODER_PULSES_PER_REV_BITS (10)
#define ENCODER_PULSES_PER_REV (1 << ENCODER_PULSES_PER_REV_BITS)
#define ENCODER_OFFSET (70)
#define POLE_PAIRS (1)

#define I_MAX 15
#define V_DC 60

typedef enum {
    CABINET_CS_SUS_U = 0,
    CABINET_CS_SUS_V,
    CABINET_CS_SUS_W,
    CABINET_CS_TOR_U,
    CABINET_CS_TOR_V,
    CABINET_CS_TOR_W,
} cabinet_cs_e;

int task_current_regulator_init(void);
int task_current_regulator_deinit(void);
int task_current_regulator_set_frequency(double);
int task_current_regulator_set_id(double);
int task_current_regulator_set_iq(double);
int task_current_regulator_set_offset(double);


void task_current_regulator_callback(void *arg);

// function to sample currents in AMDS
int read_cs(cabinet_cs_e cs, double *adc_volts_out);


#endif /* USR_CURRENTREGULATOR_CURRENT_REGULATOR_H_ */
