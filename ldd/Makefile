.phony: clean
obj-m := hello.o
KDIR = /lib/modules/`uname -r`/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	rm -f *.mod.c *.mod.o *.ko *.o modules.order Module.symvers
