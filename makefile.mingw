#makefile for
#         brightsky
#”√Nasm ∫Õ GCC±‡“Î
#for 0.04 
export

NASM=nasm 
LD  =ld
GCC =gcc
INCLUDE= -I..\include
CF = -c -Wall -O2    -fno-builtin -nostdlib -nostdinc 
CFLAG= $(CF) $(INCLUDE)
ASFLAG= -f win32
LDFLAG = -T sky.ld -nostdlib -s -x
COPY=copy
DEL=del
bsky.flp : boo   kerne
	$(COPY)  /b boot\bootflp.bin+kernel\kernel.bin+add.bin bsky.flp
	$(COPY)  /b boot\headup.bin+kernel\kernel.bin    bsldr
	$(COPY)  /b boot\bsboot bsboot
#	$(MAKE) clean
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
	$(DEL) boot\*.bin
	$(DEL) kernel\*.o
	$(DEL) kernel\*.bin