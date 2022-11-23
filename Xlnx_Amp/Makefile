# Makefile

# Author: Yang Yujun
# E-mail: <yujiantianhu@163.com>
# Created on: 2022.11.19

Target	  	:= cpu0_amp
Arch_type 	?= arm
Compiler	?= arm-linux-gnueabihf-

Dev_target	:= $(Target)_device
Drv_target	:= $(Target)_driver
Dev_name	:= $(Dev_target).o
Drv_name	:= $(Drv_target).o

Current_dir	:= $(shell pwd)
kernel_dir	:= /home/ubuntu/ProgramWorks/Linux/Kernel/ZYNQ/SourceCode/linux-xlnx-xilinx-v2018.3

obj-m 		:= $(Dev_name) $(Drv_name)

.PHONY: all clean

build: all
all:
	make ARCH=$(Arch_type) CROSS_COMPILE=$(Compiler) -C $(kernel_dir) M=$(Current_dir) modules
	cp *.ko /home/ubuntu/Transport/nfs/xilinx/

clean:
	make ARCH=$(Arch_type) CROSS_COMPILE=$(Compiler) -C $(kernel_dir) M=$(Current_dir) clean

# end of file
