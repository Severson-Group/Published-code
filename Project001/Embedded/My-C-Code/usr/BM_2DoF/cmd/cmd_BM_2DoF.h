#ifndef CMD_DPNV_H
#define CMD_DPNV_H

// Called in app init function to register command with system
void cmd_BM_2DoF_register(void);

// Function called when user types "dpnv" command into command prompt
// i.e., this is the blink command handler function
int cmd_BM_2DoF(int argc, char **argv);


#endif // CMD_DPNV_H
