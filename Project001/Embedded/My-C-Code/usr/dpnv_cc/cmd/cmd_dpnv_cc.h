#ifndef CMD_DPNV_H
#define CMD_DPNV_H

// Called in app init function to register command with system
void cmd_dpnv_cc_register(void);

// Function called when user types "dpnv" command into command prompt
// i.e., this is the blink command handler function
int cmd_dpnv_cc(int argc, char **argv);

#define Kphi (0.01)	// not actual flux constant. Just value used for open loop shaft rotation

#endif // CMD_DPNV_H
