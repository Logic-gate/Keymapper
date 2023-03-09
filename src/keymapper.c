#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <libconfig.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "keymapper.h"
#include <time.h>


void INThandler() {
  exit(0);
}
void debug_(char note[], char details[]) {
  printf("%s --> %s\n", note, details);
}

int log_keys(int store, char * state) {
  FILE * fptr;
  int first_key;
  if (0 == strcmp(state, "w")) {
    fptr = fopen("/home/defaultuser/.config/keymapper/key.log", "w+");
    if (fptr == NULL) {
      debug_("Error!", "File is NULL");
      exit(1);
    }
    fprintf(fptr, "%d\n", store);
    fclose(fptr);
  }
  if (0 == strcmp(state, "r")) {
    fptr = fopen("/home/defaultuser/.config/keymapper/key.log", "r");
    fscanf(fptr, "%d", & first_key);
    fclose(fptr);
    return first_key;
  }

  return 0;
}

char * join(char * start,
  const char * msg, char * close) {
  int size = strlen(start) + strlen(msg) + strlen(close) + 1;
  char * str = malloc(size);
  strcpy(str, start);
  strcat(str, msg);
  strcat(str, close);
  return str;
}

int release() {
  para_keys.trigger = 0;
  log_keys(0, "w");
  return 1;
}

void * dbus_notify(const char * msg) {
  /* Lazy, but it works - TODO: Implement in Glib */
  /* Timeout 0 = 5000?? */
  char * gdbus;
  gdbus = join("gdbus call --session --dest org.freedesktop.Notifications --object-path /org/freedesktop/Notifications --method org.freedesktop.Notifications.Notify keymapper 0 \"icon-m-notifications\" \"\" \"\" \"['default', '']\" \"{'x-nemo-preview-summary': <'", msg, "'>}\" 1000");
  system(gdbus);
  return EXIT_SUCCESS;
}

int read_config(int key, int seq_state) {

  config_t cfg;
  config_setting_t * keymapSetting;
  config_init( & cfg);
  if (!config_read_file( & cfg, "/home/defaultuser/.config/keymapper/keymap.config")) {
    fprintf(stderr, "%s:%d - %s\n", config_error_file( & cfg),
      config_error_line( & cfg), config_error_text( & cfg));
    config_destroy( & cfg);
  }
  keymapSetting = config_lookup( & cfg, "keymap");
  if (keymapSetting != NULL) {
    int countKeymap = config_setting_length(keymapSetting);
    int i;
    for (i = 0; i < countKeymap; i++) {
      config_setting_t * keymap = config_setting_get_elem(keymapSetting, i);
      if (!(config_setting_lookup_int(keymap, "trigger", & trigger) &&
          config_setting_lookup_string(keymap, "cmd", & cmd) &&
          config_setting_lookup_string(keymap, "process_name", & process_name) &&
          config_setting_lookup_int(keymap, "seq", & seq) &&
          config_setting_lookup_string(keymap, "proc", & proc)))
        continue;

      if (seq == seq_state) {

        pid_t pid_start = proc_find(process_name);

        if (key == trigger) {
          para_keys.trigger = key;
        } else {
          continue;
        }
        /* only run if cmd is not running */
        if (pid_start == -1) {
          if (para_keys.trigger == trigger) {
            if (proc[0] == '\0') {
              ;
            } else {
              dbus_notify(proc);
              fprintf(stderr, "%s - %s\n", "Keymapper:Notification:Sent", proc);
            }
            fprintf(stderr, "%s - %s, %d\n", "Keymapper:Excecuting:Command", cmd, pid_start);
            popen(cmd, "r");
            pid_t pid = proc_find(process_name);
            fprintf(stderr, "%s - %s, %d\n", "Keymapper:Command:Done", cmd, pid);
            config_destroy( & cfg);
            return (EXIT_SUCCESS);
          }
        }
      }
    }
  }
  return (EXIT_FAILURE);

}

int check_key(int key, int seq_state) {

  read_config(key, seq_state);
  release();
  return 1;
}

pid_t proc_find(const char * name) {
  DIR * dir;
  struct dirent * ent;
  char * endptr;
  char buf[512];

  if (!(dir = opendir("/proc"))) {
    perror("can't open /proc");
    return -1;
  }

  while ((ent = readdir(dir)) != NULL) {
    /* if endptr is not a null character, the directory is not
     * entirely numeric, so ignore it */
    long lpid = strtol(ent -> d_name, & endptr, 10);
    if ( * endptr != '\0') {
      continue;
    }

    /* try to open the cmdline file */
    snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
    FILE * fp = fopen(buf, "r");

    if (fp) {
      if (fgets(buf, sizeof(buf), fp) != NULL) {
        /* check the first token in the file, the program name */
        char * first = strtok(buf, " ");
        if (!strcmp(first, name)) {
          fprintf(stderr, "%s - %s:%s\n", "Cannot Run:", first, name);
          fclose(fp);
          closedir(dir);
          return (pid_t) lpid;
        }
      }
      fclose(fp);
    }
  }

  closedir(dir);
  return -1;
}

void run(char * param, char * input_device) {
  int seq_state;
  struct timeval ts;
  char * devname;
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

  while (1) {
    gettimeofday( & ts, NULL);
    time_t t;
    t = clock();
    int dev = read(device, & ev, sizeof(ev));
    t = clock()-t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    /* TODO: Add Type EV_ABS, and codes X, Y, and pressure for touchscreen */
    if (ev.type == EV_KEY || ev.code != 0 || ev.value != 0) {
      if (ev.value == 1) {
        time_t time = (time_t) ev.time.tv_sec;
        if (0 == strcmp(param, "start")) {
          fprintf(stderr, "%ld--%ld--%f\n", time, ts.tv_sec, time_taken);
          if (time == ts.tv_sec) {
            seq_state = 2;
          } else if (time > ts.tv_sec){
            seq_state = 1;
          }
            else{
             seq_state = 0;
            }
          check_key(ev.code, seq_state);
        }
        if (0 == strcmp(param, "listen")) {
          printf("CODE --> %d\nTYPE --> %d\nVALUE --> %d\n---------\n", ev.code, ev.type, ev.value);
        }
      }
    }
  }
}