#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <libconfig.h>
#include <string.h>
#include "keymapper.h"

void INThandler()
{
    exit(0);
}
void debug_(char note[], char details[])
{
    printf("%s --> %s\n", note, details);
}

int log_keys(int store, char *state)
{
    FILE * fptr;
    int first_key;
    if (0 == strcmp(state, "w"))
    {
        fptr = fopen("/home/defaultuser/.config/keymapper/key.log", "w+");
        if (fptr == NULL)
        {
            debug_("Error!", "File is NULL");
            exit(1);
        }
        fprintf(fptr, "%d\n", store);
        fclose(fptr);
    }
    if (0 == strcmp(state, "r"))
    {
        fptr = fopen("/home/defaultuser/.config/keymapper/key.log", "r");
        fscanf(fptr, "%d", &first_key);
        fclose(fptr);
        return first_key;
    }

    return 0;
}

char *join(char *start, const char *msg, char *close)
{
  int size = strlen(start) + strlen(msg) + strlen(close) + 1;
  char *str = malloc(size);
  strcpy (str, start);
  strcat (str, msg);
  strcat (str, close); 
  return str;
}

int release()
{
    para_keys.holder = 0;
    para_keys.trigger = 0;
    return 1;
}

void *dbus_notify(const char *msg)
{
    /* Lazy, but it works - TODO: Implement in Glib */
    /* Timeout 0 = 5000?? */
    char *gdbus;
    gdbus = join("gdbus call --session --dest org.freedesktop.Notifications --object-path /org/freedesktop/Notifications --method org.freedesktop.Notifications.Notify keymapper 0 \"icon-m-notifications\" \"\" \"\" \"['default', '']\" \"{'x-nemo-preview-summary': <'", msg, "'>}\" 1000");
    system(gdbus);
    return EXIT_SUCCESS;
}


int read_config(int key)
{
    config_t cfg;
    config_setting_t * keymapSetting;
    config_init(&cfg);
    if (!config_read_file(&cfg, "/home/defaultuser/.config/keymapper/keymap.config"))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
    }

    keymapSetting = config_lookup(&cfg, "keymap");
    if (keymapSetting != NULL)
    {
        int countKeymap = config_setting_length(keymapSetting);
        int i;
        for (i = 0; i < countKeymap; i++)
        {
            config_setting_t *keymap = config_setting_get_elem(keymapSetting, i);
            if (!(config_setting_lookup_int(keymap, "holder", &holder) &&
                    config_setting_lookup_int(keymap, "trigger", &trigger) &&
                    config_setting_lookup_string(keymap, "cmd", &cmd) &&
                    config_setting_lookup_string(keymap, "proc", &proc)))
                continue;
            if (key == holder)
            {
                para_keys.holder = key;
                log_keys(para_keys.holder, "w");

            }
            else if (key == trigger)
            {
                para_keys.trigger = key;
            }
            else
            {
                continue;
            }

            para_keys.holder = log_keys(0, "r");
            if (para_keys.trigger == trigger && para_keys.holder == holder)
            {   
                if (proc[0] == '\0')
                {
                    ;
                }
                else
                {
                    dbus_notify(proc);
                    fprintf(stderr, "%s - %s\n", "Keymapper Notification Sent", proc);
                }
                
                fprintf(stderr, "%s - %s\n", "Keymapper Excecuting Command", cmd);
                system(cmd);
                log_keys(0, "w");
                config_destroy(&cfg);
                return (EXIT_SUCCESS);
            }

        }
    }
    return (EXIT_FAILURE);

}

int check_key(int key)
{
    read_config(key);
    release();
    return 1;
}

void run(char *param, char *input_device)
{
    char *devname;
    devname = input_device;
    int device = open(devname, O_RDONLY);
    /* Double Check */
    if (device == -1) {
        fprintf(stderr, "%s is not a vaild device\n", devname);
        return;
    }
    struct input_event ev;
    signal(SIGINT, INThandler);
    dbus_notify("Keymapper Started");
    while (1)
    {
        read(device, &ev, sizeof(ev));
        /* TODO: Add Type EV_ABS, and codes X, Y, and pressure for touchscreen */
        if (ev.type == EV_KEY || ev.code != 0 || ev.value != 0)
        {
            if (ev.value == 1)
            {
                if (0 == strcmp(param, "start"))
                {
                    check_key(ev.code);
                }
                if (0 == strcmp(param, "listen"))
                {
                    printf("CODE --> %d\nTYPE --> %d\nVALUE --> %d\n---------\n", ev.code, ev.type, ev.value);
                }
            }
        }
    }
}