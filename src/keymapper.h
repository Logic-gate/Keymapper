
#ifndef KEYMAPPER_H_
#define KEYMAPPER_H_


struct Key_Combo
{
        int holder;
        int trigger;
};
 
struct Key_Combo para_keys;
const char *cmd;
int holder, trigger;
void run(char *param);

#endif
