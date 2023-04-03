/*
 * sensors.h
 *
 *  Created on: 28 Jun 2022
 *      Author: Ramadas
 */

#ifndef USR_BM_2DOF_SENSORS_H_
#define USR_BM_2DOF_SENSORS_H_

#include "drv/analog.h"
#include "drv/encoder.h"

// calibrate AMDS sampling
// suspension
#define PHASE_U_SUS_CURRENT_GAIN_AMDS (0.862)
#define PHASE_V_SUS_CURRENT_GAIN_AMDS (0.840)
#define PHASE_W_SUS_CURRENT_GAIN_AMDS (0.849)

#define PHASE_U_SUS_CURRENT_OFFSET_AMDS (0.178)
#define PHASE_V_SUS_CURRENT_OFFSET_AMDS (0.238)
#define PHASE_W_SUS_CURRENT_OFFSET_AMDS (0.198)

// torque
#define PHASE_U_TOR_CURRENT_GAIN_AMDS (1.612)
#define PHASE_V_TOR_CURRENT_GAIN_AMDS (1.610)
#define PHASE_W_TOR_CURRENT_GAIN_AMDS (1.616)

#define PHASE_U_TOR_CURRENT_OFFSET_AMDS (4.855)
#define PHASE_V_TOR_CURRENT_OFFSET_AMDS (4.795)
#define PHASE_W_TOR_CURRENT_OFFSET_AMDS (4.718)
//


typedef enum {
    CABINET_CS_SUS_U = 0,
    CABINET_CS_SUS_V,
    CABINET_CS_SUS_W,
    CABINET_CS_TOR_U,
    CABINET_CS_TOR_V,
    CABINET_CS_TOR_W,
} cabinet_cs_e;

//
//#define Vdc_tor (30)	  // [V]
//#define Vdc_sus (30)	  // [V]
//#define Imax (10)  		  // [A]

// ENCODER defs
#define ENCODER_PULSES_PER_REV_BITS (13)
#define ENCODER_PULSES_PER_REV (1 << ENCODER_PULSES_PER_REV_BITS)
#define ENCODER_OFFSET (125 * PI/180)
#define POLE_PAIRS (1)

// RADIAL POSITION SENSOR defs
// WARNING: Below gains are for BP1
// Per Kaman calibration sheet for the eddy current sensor devices:
#define VOLT_PER_MM (1.374) // [V / mm]
#define MM_PER_VOLT (1.0 / VOLT_PER_MM)


#define CAL_RATIO (0.5)

#define CAL_GAIN_X (1.0)
#define CAL_GAIN_Y (1.0)

// BP2 sensor compensation over BP1 gains

// air bearing at center
#define OFFSET_X (0.004)
#define OFFSET_Y (0.020)

// functions
int read_cs(cabinet_cs_e cs, double *adc_volts_out);
double task_get_theta_e_enc(void);
void eddy_current_sensor_xy_mm(double *x, double *y);
void get_cabinet_currents(double *Iss_u, double *Iss_v, double *Iss_w, double *It_u, double *It_v, double *It_w);
#endif /* USR_BM_2DOF_SENSORS_H_ */
