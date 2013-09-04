#makefile for
#        kernel
#by skysmiler
# ~2006-5-31
OBJ=head.o kernel.o i386.o  string.o  message.o console.o video.o kbd.o mouse.o   \
	memory.o
#    fat32.o   hd_part.o   msdos_parti.o 

kernel.bin: $(OBJ)
	$(LD) $(LDFLAG) $(OBJ) -o kernel.bin
%.o:%.asm
	$(NASM) $(ASFLAG) -o $@ $<

%.o:%.c
	$(GCC) $(CFLAG)   -o $@ $<
