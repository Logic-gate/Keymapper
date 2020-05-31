MKDIR = mkdir -p

all: dirs
	$(MAKE) -C src

debug: dirs
	$(MAKE) -C src debug

clean: dirs
	$(MAKE) -C src clean

install:
	$(MAKE) -C src install

uninstall:
	$(MAKE) -C src uninstall

update:
	$(MAKE) -C src update
dirs:
	@$(MKDIR) bin
	@$(MKDIR) build


