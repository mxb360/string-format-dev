OBJ = string-format.o

CC = $(HOME)/workspace/sony/q-seine/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-
SYSROOT = --sysroot=$(HOME)/workspace/sony/q-seine/prebuilts/ndk/r18/platforms/android-27/arch-arm64
CC_INCLUDE = -I/usr/include -I..
CFLAG = -g -Wall -Werror -std=c99 $(SYSROOT) $(CC_INCLUDE) 

all: string-format.c
	$(CC)gcc string-format.c ../string-format/string-format.o -o string-format $(CFLAG)
	#$(CC)gcc string-format.c  $(CFLAG)

clean:
	rm string-format *.o -rf