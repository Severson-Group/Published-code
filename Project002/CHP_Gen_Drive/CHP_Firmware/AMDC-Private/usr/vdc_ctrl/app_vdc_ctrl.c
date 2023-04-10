#ifdef APP_VDC_CTRL

#include "usr/vdc_ctrl/app_vdc_ctrl.h"
#include "usr/vdc_ctrl/cmd/cmd_vdc_ctrl.h"
#include "drv/gp3io_mux.h"

void app_vdc_ctrl_init(void)
{
	// Register App Commands
	cmd_vdc_ctrl_register();

    // Set up GPIO mux for the AMDS board
	// GP3IO_MUX_1_BASE_ADDR means top AMDC port
	// GP3IO_MUX_DEVICE1 is AMDS IP block
	// GP3IO_MUX_DEVICE2 is Eddy Current Sensor IP block
	gp3io_mux_set_device(GP3IO_MUX_2_BASE_ADDR, GP3IO_MUX_DEVICE1);
}

#endif // APP_VDC_CTRL
