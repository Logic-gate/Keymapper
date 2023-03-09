#ifndef KEYMAPPER_H_
#define KEYMAPPER_H_

struct Key_Combo
{
	int trigger;
};

struct Key_Combo para_keys;
const char *cmd, *proc, *process_name;
int trigger, seq;
void run(char *param, char *input_device);

#endif