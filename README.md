# Keymapper
Keyboard shortcuts for the fxtec pro1 - SailfishOS


# Getting Started
This works kind of like the sym button on the n900, where you have an instigator and then a trigger. The holder is the instigator. Once you click it(pressing the physical button) it registers that key to a file called `/home/nemo/.config/keymapper/key.log`, the next button you use is the command trigger, as in, what actually runs whatever you give it.


# Update
If you've already installed keymapper and want to update. Download the latest version and overwrite the install dir. Don't delete anything. 

Then,

```bash
make update

```
You'll need to specify the input device by name.

```bash
keymapper -s /dev/input/event3 #FXTEC Keyboard

```

# Installation

```bash
pkcon install gcc make libconfig
```

After downloading keymapper

```bash
#as nemo, not root
$ cd keymapper
$ ls
makefile src
$ make
$ make install
```
It will ask for your root password, this is needed to soft link the executable to `/usr/local/bin/`

# How to

Start by running the listener

```bash
$ keymapper -l /dev/input/event3
#OR
$ keymapper -l /dev/input/by-path/platform-c17a000.i2c-event-kbd 
```

Start typing using your fxtec keyboard, the output should be something like this

```bash
CODE --> 20
TYPE --> 1
VALUE --> 1
---------
```

Note, it only supports key event types `EV_KEY`.

Now that you know what the key codes are, navigate to `/home/nemo/.config/keymapper/keymap.config`

The config format is as follows:

```cfg

// Not recommended to run keymapper as root,
name = "Fxtec Pro 1 Keymap shortcuts";
keymap = {
        something_to_remeber = {
                proc = notification header,
                can be blank using ""
                holder = the first button you press;
                trigger = the second button you press;
                cmd = a command;
        };
        another_thing = {
                proc = notification header,
                can be blank using ""
                holder = the first button you press;
                trigger = the second button you press;
                cmd = a command;
        };
};

```
You can add as many as you like...just keep in mind the following
* This is not the best approach to shortcuts
* when you run `./keymapper -s devname`, it needs to run indefinitely
* The shortcuts will be active even if the device is locked.

The default config has `echo this works` as the command for `FX + backspace`

```cfg

// Not recommended to run keymapper as root,

name = "Fxtec Pro 1 Keymap shortcuts";
keymap = {
        testing = {
                proc = "I am a notification"
                holder = 125;
                trigger = 14;
                cmd = "echo this works";
        };
};

```
Where 125 is the `FX` button and 14 is `backspace`



A demo can be found here: [https://twitter.com/m4d_d3v/status/1266329270227333120?s=20](https://twitter.com/m4d_d3v/status/1266329270227333120?s=20)

# Final Thoughts

I do acknowledge that this is not an ideal way to achieve keyboard shortcuts, it has it's flaws and could damage your phone. Especially if you mess up the command; it takes regular system commands and could be used as root...red flag. 

Another issue is the logic of the process. `listen for input --> take first accepted input --> log it to a file --> wait for the second accepted input --> execute command`. The logic is flawed. A better way to do it is by simply listening for the first two consecutive and accepted inputs or by using a key combination method, as in listening for the sum of two codes...moved to todo.

# Todo

* change makefile
* Refactoring
* Use key combination methods rather than instigator and trigger
* Implement notifications in Glib or pure C
* Add Type EV_ABS, codes X, Y, and Pressure for touchscreen - could be an interesting avenue to explore. 

 


