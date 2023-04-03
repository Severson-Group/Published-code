#ifdef APP_DPNV_CC

#include "usr/dpnv_cc/cmd/cmd_dpnv_cc.h"
#include "sys/commands.h"
#include "sys/debug.h"
#include "sys/defines.h"
#include "sys/util.h"
#include "usr/dpnv_cc/task_dpnv_cc.h"
#include <stdlib.h>
#include <string.h>
#include "AMDC_DPNV_CC.h"

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

void cmd_dpnv_cc_register(void)
{
    // Populate the command entry block
    //
    // Here is where you define the base command string: "rlcc"
    // and what function is called to handle command
    commands_cmd_init(&cmd_entry, "dpnv", "DPNV CC application commands", cmd_help, ARRAY_SIZE(cmd_help), cmd_dpnv_cc);

    // Register the command with the system
    commands_cmd_register(&cmd_entry);
}

int cmd_dpnv_cc(int argc, char **argv)
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

    // command to set Vd voltage. Does not work if current control is enabled.
    if (argc == 3 && strcmp("vd", argv[1]) == 0){
    	double Vd = strtod(argv[2], NULL);

    	if (Vd > Vdc/2 || Vd < -Vdc/2){
    		cmd_resp_printf("Value too high\n");
			return CMD_INVALID_ARGUMENTS;
    	}
    	AMDC_DPNV_CC_U.Vdq_cmd[0] = Vd;
    	return CMD_SUCCESS;
    }

    // command to set Vq voltage. Does not work if current control is enabled.
    if (argc == 3 && strcmp("vq", argv[1]) == 0){
		double Vq = strtod(argv[2], NULL);

		if (Vq > Vdc/2 || Vq < -Vdc/2){
			cmd_resp_printf("Value too high\n");
			return CMD_INVALID_ARGUMENTS;
		}
		AMDC_DPNV_CC_U.Vdq_cmd[1] = Vq;
		return CMD_SUCCESS;
    }

    // command to set Vx voltage. Does not work if current control is enabled.
    if (argc == 3 && strcmp("vx", argv[1]) == 0){
		double Vx = strtod(argv[2], NULL);

		if (Vx > Vdc/2 || Vx < -Vdc/2){
			cmd_resp_printf("Value too high \n");
			return CMD_INVALID_ARGUMENTS;
		}
		AMDC_DPNV_CC_U.Vxy_cmd[0] = Vx;
		return CMD_SUCCESS;
	}

    // command to set Vy voltage. Does not work if current control is enabled.
	if (argc == 3 && strcmp("vy", argv[1]) == 0){
		double Vy = strtod(argv[2], NULL);

		if (Vy > Vdc/2 || Vy < -Vdc/2){
			cmd_resp_printf("Value too high \n");
			return CMD_INVALID_ARGUMENTS;
		}
		AMDC_DPNV_CC_U.Vxy_cmd[1] = Vy;
		return CMD_SUCCESS;
	}

	// command to set frequency of excited voltage / current. Required for RL loads
    if (argc == 3 && strcmp("freq", argv[1]) == 0){
		double freq = strtod(argv[2], NULL);
		AMDC_DPNV_CC_U.omega_e = freq * 2*PI;

		return CMD_SUCCESS;
	}

    // set frequency based on voltage command for spinning the motor in open loop
    if (argc == 3 && strcmp("vf", argv[1]) == 0){
		double Vd = strtod(argv[2], NULL);
		if (Vd > Vdc/2 || Vd < -Vdc/2){
				cmd_resp_printf("Value too high\n");
				return CMD_INVALID_ARGUMENTS;
			}
		AMDC_DPNV_CC_U.Vdq_cmd[0] = Vd;
		AMDC_DPNV_CC_U.omega_e = Vd / Kphi;

		return CMD_SUCCESS;
	}


    // command to enable (1) or disable (0) suspension current control
    if (argc == 4 && strcmp("cc", argv[1]) == 0 && strcmp("sus", argv[2]) == 0){
		double cc = strtod(argv[3], NULL);
		AMDC_DPNV_CC_U.select_cc = cc;

		return CMD_SUCCESS;
	}

    // command to enable (1) or disable (0) torque current control
        if (argc == 4 && strcmp("cc", argv[1]) == 0 && strcmp("tor", argv[2]) == 0){
    		double cc = strtod(argv[3], NULL);
    		AMDC_DPNV_CC_U.select_cc_g = cc;

    		return CMD_SUCCESS;
    	}

    // command to set Id current. Does not work if current control is disabled
    if (argc == 3 && strcmp("id", argv[1]) == 0){
		double id = strtod(argv[2], NULL);

		if (id > 20 || id < -20){
			cmd_resp_printf("Value too high \n");
			return CMD_INVALID_ARGUMENTS;
		}

		AMDC_DPNV_CC_U.Idq_cmd[0] = id;
		return CMD_SUCCESS;
	}

    // command to set Iq current. Does not work if current control is disabled
	if (argc == 3 && strcmp("iq", argv[1]) == 0){
		double iq = strtod(argv[2], NULL);

		if (iq > 34 || iq < -34){
			cmd_resp_printf("Value too high \n");
			return CMD_INVALID_ARGUMENTS;
		}

		AMDC_DPNV_CC_U.Idq_cmd[1] = iq;
		return CMD_SUCCESS;
	}

	// command to set Ix current. Does not work if current control is disabled
	if (argc == 3 && strcmp("ix", argv[1]) == 0){
		double ix = strtod(argv[2], NULL);

		if (ix > 15 || ix < -15){
			cmd_resp_printf("Value too high \n");
			return CMD_INVALID_ARGUMENTS;
		}

		AMDC_DPNV_CC_U.Ixy_cmd[0] = ix;
		return CMD_SUCCESS;
	}

	// command to set Iy current. Does not work if current control is disabled
	if (argc == 3 && strcmp("iy", argv[1]) == 0){
		double iy = strtod(argv[2], NULL);

		if (iy > 15 || iy < -15){
			cmd_resp_printf("Value too high \n");
			return CMD_INVALID_ARGUMENTS;
		}

		AMDC_DPNV_CC_U.Ixy_cmd[1] = iy;
		return CMD_SUCCESS;
	}

	if (argc >= 2 && strcmp("stats", argv[1]) == 0) {
		if (argc == 3 && strcmp("print", argv[2]) == 0) {
			task_dpnv_cc_stats_print();
			return CMD_SUCCESS;
		}

		if (argc == 3 && strcmp("reset", argv[2]) == 0) {
			task_dpnv_cc_stats_reset();
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
			Iss_a = AMDC_DPNV_CC_U.Iss_abc[0];
			Iss_b = AMDC_DPNV_CC_U.Iss_abc[1];
			Iss_c = AMDC_DPNV_CC_U.Iss_abc[2];
			It_a = AMDC_DPNV_CC_U.It_abc[0];
			It_b = AMDC_DPNV_CC_U.It_abc[1];
			It_c = AMDC_DPNV_CC_U.It_abc[2];

			off1 = PHASE_A_SUS_CURRENT_OFFSET_AMDS + Iss_a/PHASE_A_SUS_CURRENT_GAIN_AMDS;
			cmd_resp_printf("\n Current sensor SUS A new offset is %f \n", off1);

			off2 = PHASE_B_SUS_CURRENT_OFFSET_AMDS + Iss_b/PHASE_B_SUS_CURRENT_GAIN_AMDS;
			cmd_resp_printf("Current sensor SUS B new offset is %f \n", off2);

			off3 = PHASE_C_SUS_CURRENT_OFFSET_AMDS + Iss_c/PHASE_C_SUS_CURRENT_GAIN_AMDS;
			cmd_resp_printf("Current sensor SUS C new offset is %f \n", off3);

			off4 = PHASE_A_TOR_CURRENT_OFFSET_AMDS + It_a/PHASE_A_TOR_CURRENT_GAIN_AMDS;
			cmd_resp_printf("Current sensor TOR A new offset is %f \n", off4);

			off5 = PHASE_B_TOR_CURRENT_OFFSET_AMDS + It_b/PHASE_B_TOR_CURRENT_GAIN_AMDS;
			cmd_resp_printf("Current sensor TOR B new offset is %f \n", off5);

			off6 = PHASE_C_TOR_CURRENT_OFFSET_AMDS + It_c/PHASE_C_TOR_CURRENT_GAIN_AMDS;
			cmd_resp_printf("Current sensor TOR C new offset is %f \n", off6);

			return CMD_SUCCESS;
		}

	// get PWM_en status
	if (argc == 2 && strcmp("pwm", argv[1])==0) {
		double PWM_status = AMDC_DPNV_CC_Y.PWM_en;
		double drv_en = AMDC_DPNV_CC_U.drv_en;
//		 double PWM_status = 1;
		cmd_resp_printf("PWM status is %f \n", PWM_status);
		cmd_resp_printf("Drive status is %f \n", drv_en);
		return CMD_SUCCESS;
	}

	// enable / disable PWM
	if (argc == 3 && strcmp("drv", argv[1])==0) {
		double drv_en = strtod(argv[2], NULL);
		AMDC_DPNV_CC_U.drv_en = drv_en;

		return CMD_SUCCESS;
	}

	// set current offsets
		if (argc == 3 && strcmp("theta", argv[1])==0) {
			double theta = strtod(argv[2], NULL);
			AMDC_DPNV_CC_U.theta_off = theta;

			return CMD_SUCCESS;
		}

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



#endif // APP_DPNV_CC
