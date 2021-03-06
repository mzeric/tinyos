#makefile for
#         brightsky
#��Nasm �� GCC����
#for 0.04 
export

NASM=nasm 
LD  =ld
GCC =gcc
INCLUDE= -I../include
CF = -c -Wall -O2 -fleading-underscore -fno-builtin -nostdlib -nostdinc -m32
CFLAG= $(CF) $(INCLUDE)
ASFLAG= -f win32
LDFLAG = -T sky.ld -nostdlib -s -x
COPY=cp
DEL=rm
bsky.flp : boo   kerne
	cat   boot/bootflp.bin kernel/kernel.bin  > bsky.flp
	cat   boot/headup.bin kernel/kernel.bin >   bsldr
	$(COPY)   boot/bsboot bsboot
#	$(MAKE) clean
#	del boot\*.bin
#	del kernel\*.o
#	del kernel\*.bin
	@echo --------------------------
	@echo BrightSky ����ա� build successed!
	@echo --------------------------

boo  :
	make -C boot
kerne:  
	make -C kernel
clean:
	$(DEL) boot/*.bin
	$(DEL) kernel/*.o
	$(DEL) kernel/*.bin
