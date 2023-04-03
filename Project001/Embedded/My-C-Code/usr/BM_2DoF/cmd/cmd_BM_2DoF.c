#ifdef APP_BM_2DoF

#include "sys/commands.h"
#include "sys/debug.h"
#include "sys/defines.h"
#include "sys/util.h"
#include <stdlib.h>
#include <string.h>

#include "usr/BM_2DoF/task_BM_2DoF.h"
#include "usr/BM_2DoF/sensors.h"
#include "usr/BM_2DoF/cmd/cmd_BM_2DoF.h"
#include "BM_2DoF_Ctrl.h"


// Stores command entry for command system module
static command_entry_t cmd_entry;

// Defines help content displayed for this command
// when user types "help" at command prompt
static command_help_t cmd_help[] = {
    { "hello <name>", "Print hello to screen" },
    { "vd", "Set Vd voltage" },
    { "vq", "Set Vq voltage" },
	{ "vx", "Set Vx voltage" },
	{ "vy", "Set Vy voltage" },
	{ "cc", "0 voltage control, 1 current control" },
	{ "freq", "Set electrical frequency" },
	{ "id", "Set Id current" },
	{ "iq", "Set Iq current" },
	{ "ix", "Set Ix current" },
	{ "iy", "Set Iy current" },
	{ "stats print", "Get task execution time" },
	{ "stats reset", "Reset stats" },
};

void cmd_BM_2DoF_register(void)
{
    // Populate the command entry block
    //
    // Here is where you define the base command string: "rlcc"
    // and what function is called to handle command
    commands_cmd_init(&cmd_entry, "bm2", "BM 2DoF app commands", cmd_help, ARRAY_SIZE(cmd_help), cmd_BM_2DoF);

    // Register the command with the system
    commands_cmd_register(&cmd_entry);
}

int cmd_BM_2DoF(int argc, char **argv)
{
    // This function is called every time the user types "dpnv"
    // followed by a space and any number of characters.
    //
    // Example user input: dpnv foo 123
    //
    // It is up to the application developer to handle this
    // incoming command in a manner that reflects this command's
    // help message, as to not confuse the user.
    //
    // The arguments passed into this function (argc & argv)
    // follow standard C convention for main() programs called
    // via command line interface (CLI).
    //
    // 'argc' is the number of CLI arguments, including the base command
    // For above example: argc = 3
    //
    // 'argv' is an array of char strings which contain the CLI arguments
    // For above example:
    // - argv[0] => "blink"
    // - argv[1] => "foo"
    // - argv[2] => "123"
    //
    // NOTE: The system constrains user CLI input to ensure responsive
    // behavior for arbitrary commands. This involves limiting individual
    // command argument length as well as the total number of arguments
    // (this is defined in sys/commands.c)

    // Handle 'hello' sub-command
    //
    // First, verify correct number of arguments (2)
    // Second, verify second argument is "hello"
    if (argc == 3 && strcmp("hello", argv[1]) == 0) {
        // Perform desired action for command...

        // Fun example: hello message based on name
        if (strcmp("bharat", argv[2]) == 0) {
            // Wow, happy to talk to Bharat!
        	cmd_resp_printf("Welcome macha!!\r\n");
            cmd_resp_printf("\r\n");

            // Indicate success, but hide SUCCESS message
            return CMD_SUCCESS_QUIET;

        } else {
            // Normal hello for anyone else
        	cmd_resp_printf("Hello, %s\r\n", argv[2]);
        	cmd_resp_printf("\r\n");

            // Indicate success, but hide SUCCESS message
            return CMD_SUCCESS_QUIET;
        }

        // If user typed a valid command, return SUCCESS
        return CMD_SUCCESS;
    }


    // get PWM_en status
	if (argc == 2 && strcmp("pwm", argv[1])==0) {
		double PWM_status = BM_2DoF_Ctrl_Y.PWM_en;
		double drv_en = BM_2DoF_Ctrl_U.PARAMS_drv_EN;
		cmd_resp_printf("PWM status is %f \n", PWM_status);
		cmd_resp_printf("Drive status is %f \n", drv_en);

		return CMD_SUCCESS;
	}

	// enable / disable PWM
	if (argc == 3 && strcmp("drv", argv[1])==0) {
		double drv_en = strtod(argv[2], NULL);
		BM_2DoF_Ctrl_U.PARAMS_drv_EN = drv_en;

		return CMD_SUCCESS;
	}

	// enable / disable PWM
	if (argc == 3 && strcmp("obsen", argv[1])==0) {
		double obs_en = strtod(argv[2], NULL);
		BM_2DoF_Ctrl_U.PARAMS_ObsEn = obs_en;

		return CMD_SUCCESS;
	}

	// enable / disable PWM
	if (argc == 3 && strcmp("useobs", argv[1])==0) {
		double use_obs = strtod(argv[2], NULL);
		BM_2DoF_Ctrl_U.PARAMS_UseObs = use_obs;

		return CMD_SUCCESS;
	}

    // command to enable (1) or disable (0) suspension current control
    if (argc == 4 && strcmp("cc", argv[1]) == 0 && strcmp("sus", argv[2]) == 0){
		double cc = strtod(argv[3], NULL);
		BM_2DoF_Ctrl_U.PARAMS_Sus_CC = cc;

		return CMD_SUCCESS;
	}

    // command to enable (1) or disable (0) torque current control
	if (argc == 4 && strcmp("cc", argv[1]) == 0 && strcmp("tor", argv[2]) == 0){
		double cc = strtod(argv[3], NULL);
		BM_2DoF_Ctrl_U.PARAMS_Tor_CC = cc;

		return CMD_SUCCESS;
	}

	// command to enable (1) or disable (0) speed control
	if (argc == 3 && strcmp("w_reg", argv[1]) == 0){
		double w_reg = strtod(argv[2], NULL);
		BM_2DoF_Ctrl_U.PARAMS_w_Control = w_reg;

		return CMD_SUCCESS;
	}

	// command to enable (1) or disable (0) suspension control
	if (argc == 3 && strcmp("lev", argv[1]) == 0){
		double lev = strtod(argv[2], NULL);
		BM_2DoF_Ctrl_U.PARAMS_susp_Control = lev;

		return CMD_SUCCESS;
	}


	if (argc >= 2 && strcmp("stats", argv[1]) == 0) {
		if (argc == 3 && strcmp("print", argv[2]) == 0) {
			task_BM_2DoF_stats_print();
			return CMD_SUCCESS;
		}

		if (argc == 3 && strcmp("reset", argv[2]) == 0) {
			task_BM_2DoF_stats_reset();
			return CMD_SUCCESS;
		}
	}

	// command to get current reading
	if (argc == 3 && strcmp("readc", argv[1])==0) {
		double sensor = strtod(argv[2], NULL);
		double I;
		read_cs(sensor, &I);
		cmd_resp_printf("Current reading %f \n", I);
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

	// Extract offset of all 6 sensors
	if (argc == 3 && strcmp("curr", argv[1])==0 && strcmp("cal", argv[2])==0) {
		double Iss_a, Iss_b, Iss_c, It_a, It_b, It_c;
		double off1, off2, off3, off4, off5, off6;
		Iss_a = BM_2DoF_Ctrl_U.FDB_Iss_uvw[0];
		Iss_b = BM_2DoF_Ctrl_U.FDB_Iss_uvw[1];
		Iss_c = BM_2DoF_Ctrl_U.FDB_Iss_uvw[2];
		It_a = BM_2DoF_Ctrl_U.FDB_It_uvw[0];
		It_b = BM_2DoF_Ctrl_U.FDB_It_uvw[1];
		It_c = BM_2DoF_Ctrl_U.FDB_It_uvw[2];

		off1 = PHASE_U_SUS_CURRENT_OFFSET_AMDS + Iss_a/PHASE_U_SUS_CURRENT_GAIN_AMDS;
		cmd_resp_printf("\n Current sensor SUS A new offset is %f \n", off1);

		off2 = PHASE_V_SUS_CURRENT_OFFSET_AMDS + Iss_b/PHASE_V_SUS_CURRENT_GAIN_AMDS;
		cmd_resp_printf("Current sensor SUS B new offset is %f \n", off2);

		off3 = PHASE_W_SUS_CURRENT_OFFSET_AMDS + Iss_c/PHASE_W_SUS_CURRENT_GAIN_AMDS;
		cmd_resp_printf("Current sensor SUS C new offset is %f \n", off3);

		off4 = PHASE_U_TOR_CURRENT_OFFSET_AMDS + It_a/PHASE_U_TOR_CURRENT_GAIN_AMDS;
		cmd_resp_printf("Current sensor TOR A new offset is %f \n", off4);

		off5 = PHASE_V_TOR_CURRENT_OFFSET_AMDS + It_b/PHASE_V_TOR_CURRENT_GAIN_AMDS;
		cmd_resp_printf("Current sensor TOR B new offset is %f \n", off5);

		off6 = PHASE_W_TOR_CURRENT_OFFSET_AMDS + It_c/PHASE_W_TOR_CURRENT_GAIN_AMDS;
		cmd_resp_printf("Current sensor TOR C new offset is %f \n", off6);

		return CMD_SUCCESS;
	}


//	// set current offsets
//		if (argc == 3 && strcmp("theta", argv[1])==0) {
//			double theta = strtod(argv[2], NULL);
//			BM_2DoF_Ctrl_U.theta_off = theta;
//
//			return CMD_SUCCESS;
//		}

    // At any point, if an error is detected in given input command,
    // simply return an error code (defined in sys/defines.h)
    //
    // The return statement below is used to catch all user input which
    // didn't match the if statements above. In general, to handle commands,
    // assume they are invalid. Only after checking if each argument is
    // valid should you trust them.
    //
    // Common error return values are: FAILURE, INVALID_ARGUMENTS
    return CMD_INVALID_ARGUMENTS;
}



#endif // APP_BM_2DoF
