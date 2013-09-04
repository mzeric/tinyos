#makefile for
#        kernel
#by skysmiler
# ~2006-5-31
OBJ= head.o kernel.o sched.o i386.o  string.o  message.o console.o video.o kbd.o mouse.o   \
    memory.o 

kernel.bin: $(OBJ)
	$(LD) $(LDFLAG) $(OBJ) -o kernel.win32
	objcopy   kernel.win32 kernel.bin -O binary
%.o:%.asm
	$(NASM) $(ASFLAG) -o $@ $<

%.o:%.c
	$(GCC) $(CFLAG)   -o $@ $<
