PWD := $(shell pwd)
TOP := $(PWD)
include $(TOP)/private/tegra.mk

MODULENAME    ?= kprobe
ARCH          ?=

CROSS_COMPILE ?=

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld

obj-m += $(MODULENAME).o
$(MODULENAME)-y += main.o

OUT     ?=
HEADER  ?=
KDIR    ?=


all:
	$(MAKE) TOP=$(TOP) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) TOP=$(TOP) M=$(PWD) clean
