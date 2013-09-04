#makefile for 
#              boot
#
#

bootos.bin : boot.bin setup.bin
	copy /b boot.bin+setup.bin bootos.bin
boot.bin :  boot.asm
	$(NASM) boot.asm -f bin -o boot.bin
setup.bin:  setup.asm
	$(NASM) setup.asm -f bin -o setup.bin