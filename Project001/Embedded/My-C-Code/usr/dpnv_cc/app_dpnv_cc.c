#ifdef APP_DPNV_CC

#include "usr/dpnv_cc/app_dpnv_cc.h"
#include "usr/dpnv_cc/cmd/cmd_dpnv_cc.h"
#include "usr/dpnv_cc/task_dpnv_cc.h"
#include "usr/blink/app_blink.h"
#include "usr/blink/cmd/cmd_blink.h"
#include "usr/blink/task_blink.h"
#include "drv/gpio_mux.h"

void app_dpnv_cc_init(void)
{
    // Register "rl_cc" command with system
    cmd_dpnv_cc_register();

    // Initialize rl_cc task with system
    task_dpnv_cc_init();

    // Set up GPIO mux for the AMDS board
    // 0 means top AMDC port
    // GPIO_MUX_DEVICE2 means which mux I/O in the fpga that the AMDS driver is hooked up to.
    gpio_mux_set_device(0, GPIO_MUX_DEVICE2);
}

#endif // APP_DPNV_CC
