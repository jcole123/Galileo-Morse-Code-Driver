obj-m := zlge6x5c-leds.o
SRC := $(shellpwd)
all:
	$(MAKE)-C $(KERNEL_SRC) M=$(SRC)
modules_install:
	$(MAKE)INSTALL_MOD_DIR=kernel/drivers/leds-C $(KERNEL_SRC) M=$(SRC)modules_install
clean:
	rm -f *.o *~ core .depend .*.cmd*.ko*.mod.c
	rm -f Module.markersModule.symversmodules.order
	rm -rf.tmp_versionsModules.symvers
