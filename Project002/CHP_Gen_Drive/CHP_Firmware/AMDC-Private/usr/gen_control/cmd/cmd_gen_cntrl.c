#ifdef APP_GEN_CONTROLLER

#include "usr/gen_control/cmd/cmd_gen_cntrl.h"
#include "sys/commands.h"
#include "sys/defines.h"
#include "sys/util.h"
#include "usr/gen_control/task_gen_control.h"
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
    { "freq <freq>", "Set frequency of voltage output (rad/s)" },
    { "amplitude <amp>", "Set amplitude of voltage output (0 to 1)" },
    { "stats print", "Print stats to screen" },
    { "stats reset", "Reset the task timing stats" },
	{ "chn <channel>", "Sets current cal channel, 3 = half rail, 4 = all off, >4 = rotating current"},
};

void cmd_gen_ctrl_register(void)
{
    commands_cmd_init(&cmd_entry, "gen_ctrl", "Generator Controller commands",
                        cmd_help, ARRAY_SIZE(cmd_help), cmd_gen_ctrl);
    commands_cmd_register(&cmd_entry);
}

int cmd_gen_ctrl(int argc, char **argv)
{
    if (argc == 2 && STREQ("init", argv[1])) {
        if (task_gen_controller_init() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_enable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 2 && STREQ("deinit", argv[1])) {
        if (task_gen_controller_deinit() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_disable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("chn", argv[1])) {
        int new_channel = strtod(argv[2], NULL);

        if (task_gen_controller_set_curr_channel(new_channel) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("freq", argv[1])) {
        double new_freq = strtod(argv[2], NULL);

        if (task_gen_controller_set_frequency(new_freq) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("amplitude", argv[1])) {
        double new_amplitude = strtod(argv[2], NULL);

        if (task_gen_controller_set_amplitude(new_amplitude) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("stats", argv[1])) {
            if (STREQ("print", argv[2])) {
                task_gen_controller_stats_print();
                return CMD_SUCCESS;
            }

            if (STREQ("reset", argv[2])) {
                task_gen_controller_stats_reset();
                return CMD_SUCCESS;
            }
        }

    return CMD_INVALID_ARGUMENTS;
}

#endif // APP_GEN_CONTROLLER
