#ifndef BUCK_H
#define BUCK_H

// Power stack stated dead time = 2uS - THIS CAUSES HW INTERVENTION
#define BUCK_DEADTIME (2100.0)
#define BUCK_PWM_FREQ (20000)

// #define OP_LIMIT_VDC (100)
// #define OP_LIMIT_PHASE_CURRENT (50)

// VSI Setup
#define PWM_BUCK	3
#endif // BUCK_H
