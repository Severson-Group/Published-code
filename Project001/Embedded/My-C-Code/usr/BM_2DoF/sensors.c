/*
 * sensors.c
 *
 *  Created on: 28 Jun 2022
 *      Author: Ramadas
 */
#include <math.h>
#include <stdint.h>
#include "sys/defines.h"
#include "drv/analog.h"
#include "drv/motherboard.h"
#include "drv/encoder.h"
#include "drv/eddy_current_sensor.h"

#include "usr/BM_2DoF/sensors.h"


// CURRENT SENSORS
//////////////////////////////////////////////////////////////////
int read_cs(cabinet_cs_e cs, double *adc_volts_out)
{
    int err = FAILURE;
    int32_t out = 0;

    uint32_t MB_BASE_ADDR = MOTHERBOARD_1_BASE_ADDR;
    switch (cs) {
    case CABINET_CS_SUS_U:
        err = motherboard_get_data(MB_BASE_ADDR, MB_IN6, &out);	// MB_IN6 from current card interface to AMDS
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

void get_cabinet_currents(double *Iss_u, double *Iss_v, double *Iss_w, double *It_u, double *It_v, double *It_w)
{
	double Ius, Ivs, Iws, Iut, Ivt, Iwt;

	read_cs(CABINET_CS_SUS_U, &Ius);
	read_cs(CABINET_CS_SUS_V, &Ivs);
	read_cs(CABINET_CS_SUS_W, &Iws);
	read_cs(CABINET_CS_TOR_U, &Iut);
	read_cs(CABINET_CS_TOR_V, &Ivt);
	read_cs(CABINET_CS_TOR_W, &Iwt);

	*Iss_u = (double)PHASE_U_SUS_CURRENT_GAIN_AMDS*(Ius - PHASE_U_SUS_CURRENT_OFFSET_AMDS);
	*Iss_v = (double)PHASE_V_SUS_CURRENT_GAIN_AMDS*(Ivs - PHASE_V_SUS_CURRENT_OFFSET_AMDS);
	*Iss_w = (double)PHASE_W_SUS_CURRENT_GAIN_AMDS*(Iws - PHASE_W_SUS_CURRENT_OFFSET_AMDS);
	*It_u = (double)PHASE_U_TOR_CURRENT_GAIN_AMDS*(Iut - PHASE_U_TOR_CURRENT_OFFSET_AMDS);
	*It_v = (double)PHASE_V_TOR_CURRENT_GAIN_AMDS*(Ivt - PHASE_V_TOR_CURRENT_OFFSET_AMDS);
	*It_w = (double)PHASE_W_TOR_CURRENT_GAIN_AMDS*(Iwt - PHASE_W_TOR_CURRENT_OFFSET_AMDS);

}

//////////////////////////////////////////////////////////////////

// ANGULAR POSITION SENSOR(S)
//////////////////////////////////////////////////////////////////
double task_get_theta_e_enc(void)
{
    // Get raw encoder position
    uint32_t position;
    encoder_get_position(&position);


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

// RADIAL POSITION SENSOR(S)
void eddy_current_sensor_xy_mm(double *x, double *y)
{
	// GET EDDY CURRENT POSITION SENSOR DATA
	// get position data
	double v_out1 = 0;
	double v_out2 = 0;
	v_out1 = eddy_current_sensor_read_x_voltage(EDDY_CURRENT_SENSOR_3_BASE_ADDR);
	v_out2 = eddy_current_sensor_read_y_voltage(EDDY_CURRENT_SENSOR_3_BASE_ADDR);

	static const double GAIN_X = 1;
	static const double GAIN_Y = 1;

	*x = (CAL_GAIN_X * CAL_RATIO * (MM_PER_VOLT * v_out1) - OFFSET_X)*GAIN_X;
	*y = (CAL_GAIN_Y * CAL_RATIO * (MM_PER_VOLT * v_out2) - OFFSET_Y)*GAIN_Y;
}

