PATH := ${PATH}:/home/ktims/openwrt/trunk/staging_dir/toolchain-avr32_gcc-4.4.7_uClibc-0.9.33.2/bin
STAGING_DIR=/home/ktims/openwrt/trunk/staging_dir/toolchain-avr32_gcc-4.4.7_uClibc-0.9.33.2

CC=avr32-openwrt-linux-uclibc-gcc
LD=avr32-openwrt-linux-uclibc-ld
CFLAGS=-Wall -Os

all: oncore

oncore_shmem.o: oncore_shmem.c oncore_shmem.h
oncore: oncore.c oncore_shmem.o
clean: 
	rm -f oncore oncore_shmem.o
