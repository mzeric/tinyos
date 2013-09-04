#makefile for
#         brightsky
#
export

NASM=nasm 
LD  =ld
GCC =gcc
INCLUDE= -I..\include
CF = -c -Wall  -fwritable-strings -fno-builtin -nostdlib -nostdinc 
CFLAG= $(CF) $(INCLUDE)
ASFLAG= -f aout
LDFLAG = -T sky.ld -nostdlib -s -x
bsky.flp : boo   kerne
	copy  /b boot\bootos.bin+kernel\kernel.bin+add.bin bsky.flp
	copy  /b bsky.flp c:\bochs\os\kernel.flp
	$(MAKE) clean
	del boot\*.bin
#	del kernel\*.o
#	del kernel\*.bin
	@echo --------------------------
	@echo Arouser build successed!
	@echo --------------------------

boo  :
	make -C boot
kerne:  
	make -C kernel
clean:
	del boot\*.bin
	del kernel\*.o
	del kernel\*.bin