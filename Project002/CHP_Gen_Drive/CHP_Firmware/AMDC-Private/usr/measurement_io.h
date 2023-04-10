/*
 * measurement_io.h
 *
 *  Created on: Jul 16, 2022
 *      Author: MasterGnome
 */

#ifndef USR_GEN_CONTROL_MEASUREMENT_IO_H_
#define USR_GEN_CONTROL_MEASUREMENT_IO_H_

#include "drv/motherboard.h"

#define ADC_I_U    3
#define ADC_I_V	   2
#define ADC_I_W    0

#define ADC_I_DC    6
#define ADC_V_DC    1

// Current Calibration SJ-7/17/22
#define PHASE_U_CURRENT_OFFSET_COUNT    (1992)
#define PHASE_V_CURRENT_OFFSET_COUNT    (1991)
#define PHASE_W_CURRENT_OFFSET_COUNT    (2006)
#define IDC_CURRENT_OFFSET_COUNT      	(16)
//Current card Idc measure//(-32589)

#define PHASE_U_CURRENT_GAIN (-0.2)
#define PHASE_V_CURRENT_GAIN (-0.2)
#define PHASE_W_CURRENT_GAIN (-0.2)

#define Idc_CURRENT_GAIN (0.0155823918)
// Current Card Idc measure//(0.00214)

#define Vdc_VOLTAGE_OFFSET_COUNT (32745)
#define Vdc_VOLTAGE_GAIN (0.01918717)
// LV ADC w/ Custom Scalar + Filter //(0.14)
// LV ADC w/ Semikron Vdc scalar //(0.41586234)


#endif /* USR_GEN_CONTROL_MEASUREMENT_IO_H_ */
