# Keymapper - testing Branch
Keyboard shortcuts for the fxtec pro1 - SailfishOS


# Getting Started
This works kind of like the sym button on the n900, where you have an instigator and then a trigger. The holder is the instigator. Once you click it(pressing the physical button) it registers that key to a file called `/home/defaultuser/.config/keymapper/key.log`, the next button you use is the command trigger, as in, what actually runs whatever you give it.


# Installation

```bash
pkcon install gcc make libconfig
```

You may want to build libconfig.  

After downloading keymapper

```bash
#as nemo, not root
$ cd keymapper
$ ls
makefile src
$ make
$ make install
```

# How to

Start by running keymapper

```
$ keymapper -l /dev/input/event3
#OR
$ keymapper -l /dev/input/by-path/platform-c17a000.i2c-event-kbd 
```

Start typing using your fxtec keyboard, the output should be something like this

```
CODE --> 125
CODE --> 100
..etc
```

Now that you know what the key codes are, navigate to `/home/nemo/.config/keymapper/keymap.config`

The config format is as follows:

```cfg

// Not recommended to run keymapper as root,
// Provide full correct-cmdline e.g /usr/bin/invoker -s -n -d 5 --type=silica-media,silica-qt5 -A -- /usr/bin/jolla-camera
// Otherwise the process will never die
name = "Fxtec Pro 1 Keymap shortcuts";
keymap = {
        something_to_remeber = {
                proc = notification header can be blank using ";
                trigger = the button you press;
                process_name = cmdline-- this is needed to know if the pid exists
                cmd = a command -- full cmdline.;
        };
        another_think = {
                proc = notification header can be blank using ";
                trigger = the button you press;
                process_name = cmdline-- this is needed to know if the pid exists
                cmd = a command -- full cmdline.;
        };
        };
};

```
You can add as many as you like...just keep in mind the following
* This is not the best approach to shortcuts
* when you run ./keymapper start, it need to run indefinitely
* The shortcuts will be active even if the device is locked.

The default config has `will lanuch the camera` as the command for the `assist button`

```cfg

// Not recommended to run keymapper as root,
// Provide full correct-cmdline e.g /usr/bin/invoker -s -n -d 5 --type=silica-media,silica-qt5 -A -- /usr/bin/jolla-camera
// Otherwise the process will never die

name = "TEST CONFING";
keymap = {
        assist_button = {
                proc = "Camera Starting...";
                trigger = 457;
                process_name = "/usr/bin/jolla-camera"
                cmd = "/usr/bin/invoker -s -n -d 5 --type=silica-media,silica-qt5 -A -- /usr/bin/jolla-camera";
        };
};
```
Where 125 is the `FX` button and 14 is `backspace`



A demo can be found here: [https://twitter.com/m4d_d3v/status/1266329270227333120?s=20](https://twitter.com/m4d_d3v/status/1266329270227333120?s=20)

# Final Thoughts

I do acknowledge that this is not an ideal way to achieve keyboard shortcuts, it has it's flaws and could damage your phone. Especially if you mess up the command; it takes regular system commands and could be used as root...red flag. 

Another issue is the logic of the process. `listen for input --> take first accepted input --> log it to a file --> wait for the second accepted input --> execute command`. The logic is flawed. A better way to do it is by simply listening for the first two consecutive and accepted inputs or by using a key combination method, as in listening for two codes...moved to todo.

# Todo

* change makefile
* Refactoring
* Use key combination methods rather than instigator and trigger

 


