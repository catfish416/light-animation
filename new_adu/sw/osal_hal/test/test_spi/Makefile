#this is for arm cross compile
CURRENT_PATH := $(shell pwd)
BIN_PATH := /bin

CC := /home/wsj/share/code/gcc-linaro-aarch64-linux-gnu-4.9-2014.07_linux/bin/aarch64-linux-gnu-gcc
#export PATH=$PATH:/home/wsj/share/code/gcc-linaro-aarch64-linux-gnu-4.9-2014.07_linux/bin

INCLUDE = -I /home/wsj/share/code/nxp_git_code/kernel/linux/include/uapi \
-I /home/wsj/share/code/nxp_git_code/kernel/linux/include

SRC_C = spidev_test.c
TARGET := spidev_test.o

all:
	$(CC) $(INCLUDE) $(SRC_C) -o $(TARGET)
.PHONY: clean
clean:
	$(BIN_PATH)/rm *.o
