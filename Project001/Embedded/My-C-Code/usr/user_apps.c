#include "usr/user_apps.h"

// Adding applications:
//
// To add a new application, create a block
// like the other example(s) below which
// conditionally includes the app's header file.
//
// Also, conditionally call the app's init function.

#ifdef APP_BM_2DoF
#include "usr/BM_2DoF/app_BM_2DoF.h"
#endif

#ifdef APP_PCBTEST
#include "usr/pcbtest/app_pcbtest.h"
#endif

#ifdef APP_BLINK
#include "usr/blink/app_blink.h"
#endif

#ifdef APP_BETA_LABS
#include "usr/beta_labs/app_beta_labs.h"
#endif

#ifdef APP_DPNV_CC
#include "usr/dpnv_cc/app_dpnv_cc.h"
#endif

#ifdef APP_DEMO
#include "usr/demo/app_demo.h"
#endif

#ifdef APP_DAC
#include "usr/dac/app_dac.h"
#endif

#ifdef APP_CONTROLLER
#include "usr/controller/app_controller.h"
#endif

#ifdef CURRENT_REGULATOR
#include <usr/currentRegulator/app_current_regulator.h>
#endif

void user_apps_init(void)
{


#ifdef APP_PCBTEST
    app_pcbtest_init();
#endif

#ifdef APP_BLINK
    app_blink_init();
#endif

#ifdef APP_BETA_LABS
    app_beta_labs_init();
#endif



#ifdef APP_DEMO
    app_demo_init();
#endif

#ifdef APP_DAC
    app_dac_init();
#endif

#ifdef APP_CONTROLLER
    app_controller_init();
#endif

#ifdef CURRENT_REGULATOR
    app_current_regulator_init();
#endif
}
