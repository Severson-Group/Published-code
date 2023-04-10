/*
 * machine.h
 *
 *  Created on: Jul 22, 2022
 *      Author: MasterGnome
 */

#ifndef USR_MACHINE_H_
#define USR_MACHINE_H_

#include "drv/encoder.h"
#include <math.h>

#define CHEVY_BOLT
//#define THINGAP

#ifdef CHEVY_BOLT

#define POLEPAIRS			(4)
#define Z_PULSE_TO_REV		(2)
#define ENCODER_TO_PHASE_REVERSED (0)
#define MY_ENCODER_PULSES_PER_REV_BITS	(12)
#define PULSE_TO_RAD		(PI/(double)(ENCODER_PULSES_PER_REV))	// Encoder has 2 z pulse per rev, fixed at 4096 pulses per rev
#define ENCODER_OFFSET_RAD	(0.0)


// Machine Params
// Per 8/5/22 test
// Ld = 4.4464399616157393853787541069645e-4
// Lq = 0.00117569448266496266097067254846
// Rd = 0.114163
// Rq = 0.11778
// Re = (Rd + Rq)/2.0

#endif

#ifdef THINGAP

#define POLEPAIRS			(32)
#define ENCODER_TO_PHASE_REVERSED (1)
#define Z_PULSE_TO_REV		(1)
#define MY_ENCODER_PULSES_PER_REV_BITS	(12)
#define PULSE_TO_RAD		(PI/(double)(ENCODER_PULSES_PER_REV))	// Encoder has 2 z pulse per rev, fixed at 4096 pulses per rev
#define ENCODER_OFFSET_RAD	(0.0)

// Machine Params
// Per Nathan's system ID
//    Re  = 0.260;         % Stator Resistance [Ohm]
//    Lq  = 19.7e-6;       % q-axis Inductance [H}
//    Ld  = 18.7e-6;       % d-axis Inductance [H]

#endif

#endif /* USR_MACHINE_H_ */
