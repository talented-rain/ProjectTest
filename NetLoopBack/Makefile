# Makefile
# 
# Author: Yang Yujun
# E-mail: <yujiantianhu@163.com>
# Created on:
#		  2022.12.12

# 配置宏
CONFIG_LINUX_NET_LOOPBACK := m

target			:= loopback
objfiles		:= $(target).o
local			:= $(shell pwd)
kernel			:= /home/ubuntu/ProgramWorks/Linux/Kernel/Linux/TI/SourceCode/myir-kernel4.18

obj-$(CONFIG_LINUX_NET_LOOPBACK) = $(objfiles)

build: all
all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(kernel) M=$(local) modules

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(kernel) M=$(local) clean
	rm -rf *.ko

# end of file