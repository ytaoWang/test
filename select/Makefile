#Open/Close Debug
#DEBUG = y

ifeq ($(DEBUG),y)
  DEBFLAGS = -O -o -DSCULL_DEBUG
else
  DEBFLAGS = -O2

endif

EXTRA_CFLAGS += $(DEBFLAGS)
#CFLAGS += -I$(LDDINC)

ifneq ($(KERNELRELEASE),)
# call from kernel build system

select-objs := scull.o

obj-m := select.o

else 


KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

#modules:
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

#depend .depend dep:
#	$(CC) $(CFLAGS) -M *.c > .depend

#ifeq (.depend,$(wildcard .depend))
#include .depend
#endif