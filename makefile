#makefile for
#         brightsky
#”√Nasm ∫Õ GCC±‡“Î
export

NASM=nasm 
LD  =ld
GCC =gcc
INCLUDE= -I..\include
CF = -c -Wall -O4    -fwritable-strings -fno-builtin -nostdlib -nostdinc 
CFLAG= $(CF) $(INCLUDE)
ASFLAG= -f aout
LDFLAG = -T sky.ld -nostdlib -s -x
bsky.flp : boo   kerne
	copy  /b boot\bootflp.bin+kernel\kernel.bin+add.bin bsky.flp
	copy  /b boot\headup.bin+kernel\kernel.bin    bsldr
	copy  /b boot\bsboot bsboot
	$(MAKE) clean
#	del boot\*.bin
#	del kernel\*.o
#	del kernel\*.bin
	@echo --------------------------
	@echo BrightSky °æ«Áø’°ø build successed!
	@echo --------------------------

boo  :
	make -C boot
kerne:  
	make -C kernel
clean:
	del boot\*.bin
	del kernel\*.o
	del kernel\*.bin