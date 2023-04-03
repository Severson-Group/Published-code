/*
 * app_controller.c
 *
 *  Created on: 21-Sep-2022
 *      Author: aniru
 */
#ifdef APP_CONTROLLER

#include "usr/controller/app_controller.h"
#include "usr/controller/cmd/cmd_ctrl.h"

void app_controller_init(void)
{
    cmd_ctrl_register();
}

#endif // APP_CONTROLLER
