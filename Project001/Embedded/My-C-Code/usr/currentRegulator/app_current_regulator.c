/*
 * app_current_regulator.c
 *
 *  Created on: 25-Oct-2022
 *      Author: aniru
 */


#ifdef CURRENT_REGULATOR

#include "usr/currentRegulator/app_current_regulator.h"
#include "usr/currentRegulator/cmd/cmd_current_regulator.h"
#include "drv/gp3io_mux.h"

void app_current_regulator_init(void)
{
	cmd_current_regulator_register();
    // Set up GPIO mux for the AMDS board
    // 0 means top AMDC port
    // GP3IO_MUX_DEVICE1 means which mux I/O in the fpga that the AMDS driver is hooked up to.
    gp3io_mux_set_device(GP3IO_MUX_1_BASE_ADDR, GP3IO_MUX_DEVICE1);
}

#endif // CURRENT_REGULATOR
