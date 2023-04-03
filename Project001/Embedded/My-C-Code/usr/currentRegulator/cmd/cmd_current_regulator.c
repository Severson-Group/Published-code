/*
 * cmd_currentRegulator.c
 *
 *  Created on: 24-Oct-2022
 *      Author: aniru
 */


#ifdef CURRENT_REGULATOR

#include "usr/currentRegulator/cmd/cmd_current_regulator.h"
#include "sys/commands.h"
#include "sys/defines.h"
#include "sys/util.h"
#include "usr/currentRegulator/current_regulator.h"
#include "drv/pwm.h"
#include <stdlib.h>
#include <string.h>
#include "Controller_syncPI.h"

// Stores command entry for command system module
static command_entry_t cmd_entry;

// Defines help content displayed for this command
// when user types "help" at command prompt
static command_help_t cmd_help[] = {
    { "init", "Start task" },
    { "deinit", "Stop task" },
	{ "frequency <Hz>", "Set the frequency of current in Hz" },
    { "id <A>", "Set Id current in A (0 to 2)" },
    { "iq <A>", "Set Iq current in A (0 to 2)" },
    { "curr cal", "Calibrate current measurement" },
	{ "control", "0 voltage control, 1 current control, 2 speed control [default]"},
	{ "speed <RPM>", "Set speed in RPM "},
	{ "enc init", "Initialize the encoder"},
	{ "offset <pos>", "Set offset position"}
};

void cmd_current_regulator_register(void)
{
    commands_cmd_init(&cmd_entry, "currentreg", "current regulator commands",
                        cmd_help, ARRAY_SIZE(cmd_help), cmd_current_regulator);
    commands_cmd_register(&cmd_entry);
}

int cmd_current_regulator(int argc, char **argv)
{
    if (argc == 2 && STREQ("init", argv[1])) {
        if (task_current_regulator_init() != SUCCESS) {
            return CMD_FAILURE;
        }

		#if USER_CONFIG_HARDWARE_TARGET == AMDC_REV_E
			pwm_enable_hw(true);
		#endif

        if (pwm_enable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 2 && STREQ("deinit", argv[1])) {
        if (task_current_regulator_deinit() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_disable() != SUCCESS) {
            return CMD_FAILURE;
        }

		#if USER_CONFIG_HARDWARE_TARGET == AMDC_REV_E
			pwm_enable_hw(false);
		#endif

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("frequency", argv[1])) {
        double new_freq = strtod(argv[2], NULL);

        if (task_current_regulator_set_frequency(new_freq) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }
    if (argc == 3 && STREQ("id", argv[1])) {
        double new_id = strtod(argv[2], NULL);

		if (new_id > 2 || new_id < -2){
			cmd_resp_printf("Value too high \n");
			return CMD_INVALID_ARGUMENTS;
		}

        if (task_current_regulator_set_id(new_id) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }
    if (argc == 3 && STREQ("iq", argv[1])) {
        double new_iq = strtod(argv[2], NULL);

		if (new_iq > 2 || new_iq < -2){
			cmd_resp_printf("Value too high \n");
			return CMD_INVALID_ARGUMENTS;
		}

        if (task_current_regulator_set_iq(new_iq) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("control", argv[1])) {
        double cc = strtod(argv[2], NULL);

        if ( cc == 2){

        	Controller_syncPI_U.CMD_speed = 0;
        	Controller_syncPI_U.PARAMS_speed_control = 1;
        	Controller_syncPI_U.PARAMS_cc_enable = 1;


        }
        else{

        	Controller_syncPI_U.CMD_I_d = 0;
			Controller_syncPI_U.CMD_I_q = 0;
        	Controller_syncPI_U.CMD_V_d = 0;
			Controller_syncPI_U.CMD_V_q = 0;

        	Controller_syncPI_U.PARAMS_speed_control = 0;
            Controller_syncPI_U.PARAMS_cc_enable = cc;



        }



        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("speed", argv[1])) {
        double speed = strtod(argv[2], NULL);
        Controller_syncPI_U.CMD_speed = speed;

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("enc", argv[1]) && strcmp("init", argv[2])==0) {
    	encoder_set_pulses_per_rev_bits(ENCODER_PULSES_PER_REV_BITS);
    	return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("offset", argv[1])) {
            double new_offset = strtod(argv[2], NULL);



            if (task_current_regulator_set_offset(new_offset) != SUCCESS) {
                return CMD_FAILURE;
            }

            return CMD_SUCCESS;
        }






	if (argc == 3 && strcmp("curr", argv[1])==0 && strcmp("cal", argv[2])==0) {
		double Iss_a, Iss_b, Iss_c, It_a, It_b, It_c;
		double off1, off2, off3, off4, off5, off6;

		It_a = Controller_syncPI_U.I_uvw_fb[0];
		It_b = Controller_syncPI_U.I_uvw_fb[1];
		It_c = Controller_syncPI_U.I_uvw_fb[2];



		off4 = PHASE_U_TOR_CURRENT_OFFSET_AMDS + It_a/PHASE_U_TOR_CURRENT_GAIN_AMDS;
		cmd_resp_printf("Current sensor TOR A new offset is %f \n", off4);

		off5 = PHASE_V_TOR_CURRENT_OFFSET_AMDS + It_b/PHASE_V_TOR_CURRENT_GAIN_AMDS;
		cmd_resp_printf("Current sensor TOR B new offset is %f \n", off5);

		off6 = PHASE_W_TOR_CURRENT_OFFSET_AMDS + It_c/PHASE_W_TOR_CURRENT_GAIN_AMDS;
		cmd_resp_printf("Current sensor TOR C new offset is %f \n", off6);

		return CMD_SUCCESS;
	}

	// Calculate current sensor gain. Inputs are sensor number, actual current and sensor offset.
	if (argc == 5 && strcmp("cal", argv[1])==0) {
		double cabinet = strtod(argv[2], NULL);
		double I_act = strtod(argv[3], NULL);
		double offset= strtod(argv[4], NULL);
		double I, gain;
		read_cs(cabinet, &I);
		gain = I_act/(I-offset);
		cmd_resp_printf("Current sensor gain is %f \n", gain);
		return CMD_SUCCESS;
	}

    return CMD_INVALID_ARGUMENTS;

}

#endif // CURRENT_REGULATOR
