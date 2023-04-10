#ifdef APP_CVCR

#include "usr/cvcr/cmd/cmd_cvcr.h"
#include "sys/commands.h"
#include "sys/defines.h"
#include "sys/util.h"
#include "usr/cvcr/task_cvcr.h"
#include "drv/pwm.h"
#include <stdlib.h>
#include <string.h>

// Stores command entry for command system module
static command_entry_t cmd_entry;

// Defines help content displayed for this command
// when user types "help" at command prompt
static command_help_t cmd_help[] = {
    { "init", "Start task" },
    { "deinit", "Stop task" },
    { "stats print", "Print stats to screen" },
    { "stats reset", "Reset the task timing stats" },
	{ "enc", "Enables encoder and logs position to LOG_encPos"},
	{ "Iqc", "Set commanded Iq current" },
	{ "Idc", "Set commanded Id current" },
	{ "Vdc", "Set commanded Vdc voltage" },
	{ "RPMc", "Set commanded RPM" },
	{ "inj", "0/1, Turn Vd Vq Injection capability off/on"},
	{ "theta_lock", "0/1, 0 updates theta as normal. 1 locks theta = 0 radians"},
	{ "PWM", "Set PWM Update Enable, 0 duty ratio locked, 1 controller updates duty ratio" },
};

void cmd_cvcr_register(void)
{
    commands_cmd_init(&cmd_entry, "cvcr", "CVCR commands",
                        cmd_help, ARRAY_SIZE(cmd_help), cmd_cvcr);
    commands_cmd_register(&cmd_entry);
}

int cmd_cvcr(int argc, char **argv)
{
    if (argc == 2 && STREQ("init", argv[1])) {
        if (task_cvcr_init() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_enable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 2 && STREQ("deinit", argv[1])) {
        if (task_cvcr_deinit() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_disable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 2 && STREQ("enc", argv[1])) {

        if (task_cvcr_enc_init() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("Iqc", argv[1])) {

        double new_iqc = strtod(argv[2], NULL);

        if (task_cvcr_set_iqc(new_iqc) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("Idc", argv[1])) {

    	double new_idc = strtod(argv[2], NULL);

        if (task_cvcr_set_idc(new_idc) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("Vdc", argv[1])) {

    	double new_Vdc = strtod(argv[2], NULL);

        if (task_cvcr_set_Vdc(new_Vdc) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("RPMc", argv[1])) {
        double new_rpm = strtod(argv[2], NULL);

        if (task_cvcr_set_RPM(new_rpm) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }


    if (argc == 3 && STREQ("PWM", argv[1])) {
    	int32_t new_pwm_update_enable = strtod(argv[2], NULL);

        if (task_cvcr_set_update_enable(new_pwm_update_enable) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("inj", argv[1])) {
    	int32_t new_injection = strtod(argv[2], NULL);

        if (task_cvcr_set_injection(new_injection) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("theta_lock", argv[1])) {
    	int32_t new_theta_lock = strtod(argv[2], NULL);

        if (task_cvcr_set_theta_lock(new_theta_lock) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }


    if (argc == 3 && STREQ("stats", argv[1])) {
            if (STREQ("print", argv[2])) {
                task_cvcr_stats_print();
                return CMD_SUCCESS;
            }

            if (STREQ("reset", argv[2])) {
                task_cvcr_stats_reset();
                return CMD_SUCCESS;
            }
        }

    return CMD_INVALID_ARGUMENTS;
}

#endif // APP_CVCR
