#makefile for 
#              boot
#
#

bootflp.bin : boot.bin setup.bin    headup.bin  bsboot
	$(COPY) /b boot.bin+setup.bin bootflp.bin

setup.bin: setup.asm
	$(NASM) setup.asm -f bin -o setup.bin
boot.bin :  boot.asm
	$(NASM) boot.asm -f bin -o boot.bin
headup.bin : headup.asm
	$(NASM) headup.asm -f bin -o headup.bin
bsboot  :bsboot.asm
	$(NASM) bsboot.asm -f bin -o bsboot
#	copy /b setup.obj  ..\kernel
#	nasm setup.asm -f bin -o setup.bin