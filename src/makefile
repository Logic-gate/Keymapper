TARGET = keymapper
OBJ = main.o keymapper.o
OBJECTSDIR = ../build
TARGETDIR = ../bin
CONFIGPATH = $(HOME)/.config/keymapper
CONFIGFILE = keymap.config
LOGFILE = key.log
KEYMAPPERHOME = $(HOME)/keymapper


DEL_FILE = rm -f
DEL_DIR = rm -rf
INSTALL_PROGRAM = install -m 755 -p
CC = gcc -Wall
CFLAGS = `pkg-config --cflags libconfig`
LIBS = `pkg-config --libs libconfig`


ifeq ($(BUILD),debug)
CFLAGS += -O0 -g
else
CFLAGS += -O2 -s -DNDEBUG
endif

OBJECTS = $(patsubst %,$(OBJECTSDIR)/%,$(OBJ))

$(OBJECTSDIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	mkdir $(CONFIGPATH); touch $(CONFIGPATH)/$(LOGFILE); cp $(CONFIGFILE) $(CONFIGPATH)/$(CONFIGFILE); mkdir $(KEYMAPPERHOME)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(CFLAGS) $(LIBS)

debug:
	make "BUILD=debug"

clean:
	rm -f $(OBJECTSDIR)/*.o *~ core

install:
	$(INSTALL_PROGRAM) $(TARGETDIR)/$(TARGET) $(KEYMAPPERHOME)/$(TARGET)
	@echo "We need root access to soft link keymapper to /usr/local/bin"
	devel-su ln -s $(KEYMAPPERHOME)/$(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	@echo "We need root access to remove the soft link to /usr/local/bin"
	devel-su $(DEL_FILE) /usr/local/bin/$(TARGET)

	$(DEL_FILE) $(KEYMAPPERHOME)/$(TARGET); $(DEL_FILE) $(CONFIGPATH)/$(CONFIGFILE); $(DEL_FILE) $(CONFIGPATH)/$(LOGFILE)
	$(DEL_DIR) $(CONFIGPATH)
	$(DEL_DIR) $(KEYMAPPERHOME)

update:
	rm -f $(OBJECTSDIR)/*.o *~ core
	@echo "We need root access to remove the soft link to /usr/local/bin"
	devel-su $(DEL_FILE) /usr/local/bin/$(TARGET)
	$(DEL_FILE) $(KEYMAPPERHOME)/$(TARGET); $(DEL_FILE) $(CONFIGPATH)/$(LOGFILE)
	cp $(CONFIGPATH)/$(CONFIGFILE) /tmp/$(CONFIGFILE)
	$(DEL_FILE) $(CONFIGPATH)/$(CONFIGFILE)
	$(DEL_DIR) $(CONFIGPATH)
	$(DEL_DIR) $(KEYMAPPERHOME)
	make
	make install
	cp /tmp/$(CONFIGFILE) $(CONFIGPATH)/$(CONFIGFILE)
	$(DEL_FILE) /tmp/$(CONFIGFILE)
	@echo DONE!

