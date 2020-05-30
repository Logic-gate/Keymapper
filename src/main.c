#include "keymapper.h"
#include <stdio.h>
#include <string.h>


static void help_message(void)
{
	 printf("Usage:\n"
           "keymapper [OPTION]\n"
           "Options:\n"
           "  listen               Listen for key inputs\n"
           "  start                Start Keymapper\n"
           "\n");
}


int main(int argc, char** argv)
{       
    
    if(argc < 2)
    {
    	help_message();
    	return 1;
    }
    
    if(0 == strcmp(argv[1], "start"))
    {
        run("start");
    }
    else if (0 == strcmp(argv[1], "listen"))
    {
        run("listen");
    }
    
}