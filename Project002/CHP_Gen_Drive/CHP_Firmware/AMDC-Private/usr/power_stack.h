#ifndef POWER_STACK_H
#define POWER_STACK_H

// Power stack stated dead time = 2uS - THIS CAUSES HW INTERVENTION
#define POWERSTACK_DEADTIME (2100.0)
#define POWERSTACK_PWM_FREQ (20000)

#define OP_LIMIT_VDC (100)
#define OP_LIMIT_PHASE_CURRENT (50)

// VSI Setup
#define PWM_U	0
#define PWM_V	1
#define PWM_W	2


#endif // POWER_STACK_H
