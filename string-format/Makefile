#EXEC_NAME = hello-world-test
OBJ = string-format.o

CC = $(HOME)/workspace/sony/q-seine/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-
SYSROOT = --sysroot=$(HOME)/workspace/sony/q-seine/prebuilts/ndk/r18/platforms/android-28/arch-arm64
CC_INCLUDE = -I/usr/include/ -I..
CFLAG = -g -Wall -Werror $(SYSROOT) $(CC_INCLUDE)

all: string-format.c
	$(CC)gcc string-format.c -c -o string-format.o $(CFLAG)

clean:
	rm -rf *.o
