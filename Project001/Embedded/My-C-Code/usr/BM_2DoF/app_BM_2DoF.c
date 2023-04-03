#ifdef APP_BM_2DoF

#include "usr/BM_2DoF/app_BM_2DoF.h"
#include "usr/BM_2DoF/cmd/cmd_BM_2DoF.h"
#include "usr/BM_2DoF/task_BM_2DoF.h"
#include "drv/gp3io_mux.h"
#include "drv/eddy_current_sensor.h"

void app_BM_2DoF_init(void)
{
    // Register "bm2" command with system
    cmd_BM_2DoF_register();

    // Initialize BM_2DoF task with system
    task_BM_2DoF_init();

    // Set up GPIO mux for the AMDS board
    // GP3IO_MUX_1_BASE_ADDR means top AMDC port
    // GP3IO_MUX_DEVICE1 means which mux I/O in the fpga that the AMDS driver is hooked up to.
    gp3io_mux_set_device(GP3IO_MUX_1_BASE_ADDR, GP3IO_MUX_DEVICE1);

    // Set up GPIO mux for the AMDS board
	// GP3IO_MUX_3_BASE_ADDR --> top AMDC port 2nd row
	// GP3IO_MUX_DEVICE2 --> eddy current sensor IP block
	gp3io_mux_set_device(GP3IO_MUX_3_BASE_ADDR, GP3IO_MUX_DEVICE2);

	// Turn on position sensor sampling
	eddy_current_sensor_enable(EDDY_CURRENT_SENSOR_3_BASE_ADDR);

}

#endif // APP_BM_2DoF
