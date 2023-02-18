#include "keymapper.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>

static void help_message(void)
{
  printf("\nUsage:\n"
    "keymapper [OPTION] <input-device>\n"
    "Options:\n"
    "  -l, --listen               Listen for key inputs\n"
    "  -s, --start                Start Keymapper point to /dev/null and continue &\n"
    "Input Device:\n"
    "  Device Input - input event  e.g. /dev/input/event3...\n"
    "\n");
}

int check_device(char *input_device, char *run_option)
{
  const int len = strlen(input_device);
  if (strstr(input_device, "/dev/input/") != NULL && input_device[len - 1] != '/')
  {
    if (access(input_device, F_OK) != -1)
    {
      run(run_option, input_device);
      return 1;
    }
    else
    {
      printf("%s Does not exsit\n", input_device);
      return 0;
    }
  }
  else
  {
    printf("%s is not a valid path.\n", input_device);
    return 0;
  }
}

int main(int argc, char **argv)
{
  int switch_c;
  static struct option op[] = {
    { "help", no_argument, 0, 'h' },
    { "listen", required_argument, 0, 'l' },
    { "start", required_argument, 0, 's' },
    { 0 }
  };

  if (argc < 2)
  {
    help_message();
    return 1;
  }
  while (1)
  {

    switch_c = getopt_long_only(argc, argv, "hl:s:", op, NULL);

    if (switch_c == -1)
    {
      break;
    }
    switch (switch_c)
    {
      case 'h':
        help_message();
        break;
      case 'l':
        check_device(optarg, "listen");
        break;
      case 's':
        check_device(optarg, "start");
        break;
      case '?':
        help_message();
        break;
      default:
        printf("%s\n", "Usage:\nkeymapper [OPTION] <input-device>");
        break;
    }
  }
  if (optind < argc)
  {
    printf("Not a valid option ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    printf("\n");
  }
  return 0;
}