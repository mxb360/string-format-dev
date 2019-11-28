CDEV_NAME := string-format

PWD:=$(shell pwd)

KERNEL:=$(HOME)/workspace/sony/q-seine/out/target/product/pdx201/obj/kernel/msm-4.14
CROSS_COMPILE=$(HOME)/workspace/sony/q-seine/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-
ARCH:=arm64

LD:=$(CROSS_COMPILE)ld

#驱动模块的名称
obj-m:= $(CDEV_NAME).o

#编译此驱动模块涉及的源码文件
#$(CDEV_NAME)-objs := 

all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL) M=$(PWD) modules

clean:
	rm -rf *.o *.cmd *.ko *.mod.c modules.order Module.symvers .tmp_versions .$(CDEV_NAME).*
