#ifndef CMD_VDC_CTRL_H
#define CMD_VDC_CTRL_H

void cmd_vdc_ctrl_register(void);
int cmd_vdc_ctrl(int argc, char **argv);
void task_vdc_ctrl_stats_print(void);
void task_vdc_ctrl_stats_reset(void);

#endif // CMD_VDC_CTRL_H
