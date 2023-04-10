#ifdef APP_VDC_CTRL

#include <stdlib.h>
#include <string.h>
#include "sys/commands.h"
#include "sys/defines.h"
#include "sys/util.h"
#include "drv/pwm.h"
#include "usr/vdc_ctrl/cmd/cmd_vdc_ctrl.h"
#include "usr/vdc_ctrl/task_vdc_ctrl.h"

// Stores command entry for command system module
static command_entry_t cmd_entry;

// Defines help content displayed for this command
// when user types "help" at command prompt
static command_help_t cmd_help[] = {
    { "init", "Start task" },
    { "deinit", "Stop task" },
    { "stats print", "Print stats to screen" },
    { "stats reset", "Reset the task timing stats" },
    { "vdc <vdc>", "Set value of DC Link voltage (0-1,000)" },
};

void cmd_vdc_ctrl_register(void)
{
    commands_cmd_init(&cmd_entry, "vdc_ctrl", "Vdc Controller Commands",
                        cmd_help, ARRAY_SIZE(cmd_help), cmd_vdc_ctrl);
    commands_cmd_register(&cmd_entry);
}

int cmd_vdc_ctrl(int argc, char **argv)
{
    if (argc == 2 && STREQ("init", argv[1])) {
        if (task_vdc_ctrl_init() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_enable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 2 && STREQ("deinit", argv[1])) {
        if (task_vdc_ctrl_deinit() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_disable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("vdc", argv[1])) {
        double new_vdc = strtod(argv[2], NULL);

        if (task_vdc_ctrl_set_vdc(new_vdc) != SUCCESS) {
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
