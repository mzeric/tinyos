/*
基本的硬盘驱动


*/
#include<asm.h>
#include<video.h>
#include<hd.h>
#include<i386.h>

#define STAT_OK		 (READY_STAT|SEEK_STAT)
#define OK_STATUS(s) (((s)&(STAT_OK|(BUSY_STAT|WRERR_STAT|ERR_STAT)))==STAT_OK)

void hd_asm(void);//for irq

void do_hd(void){
	
    inb(HD_STATUS);
	outb(0x20,0x20);
	outb(0xa0,0x20);
}
void init_hd(){
	set_idt_gate(46,(uint)hd_asm,0x8e00);
	en_irq(2);
	en_irq(14);
}

//for test below
hd_driveid hdinfo; 

void init_ide(void)
{
	int i;
	uint oldcolor;
	kpf("   init ide     \n");
	
    get_ide_info();
    kpf("%s\n",hdinfo.model);
	
	
	for(i=0;i<20;i++)putc(hdinfo.serial_no[i]);
	kpf("<-id\n");
	
	
    set_idt_gate(32+14,(uint)hd_asm,0x8e00);
	en_irq(2) ;
	en_irq(14); 
	
	oldcolor=getcolor();
	setcolor(4);
	puts("                [OK]");
	newline();
	setcolor(oldcolor);
	
}

int  get_ide_info(void)
{
	
	outb(HD_DRVHED, 0xA0); 
	outb(HD_COMMAND, WIN_IDENTIFY);
	
	while(inb(HD_STATUS)&0x80);
	insw(HD_DATA, &hdinfo, sizeof(hdinfo)/2);
	return 0;
}

static int controller_busy(void)
{
	int retries = 100000;
	unsigned char status;
	
	do {
		status = inb_p(HD_STATUS);
	} while ((status & BUSY_STAT) && --retries);
	return status;
}

static int status_ok(void)
{
	unsigned char status = inb_p(HD_STATUS);
	
	if (status & BUSY_STAT)
		return 1; 
	
	if (status & WRERR_STAT)
		return 0; 
	
	if (!(status & READY_STAT))
		return 0;
	
	if (!(status & SEEK_STAT))
		return 0; 
	
	return 1;
}

static int controller_ready(unsigned int drive, unsigned int head)
{
	int retry = 100000;
	
	do {
		if (controller_busy() & BUSY_STAT)
			return 0;
		outb_p(HD_DRVHED, 0xA0 | (drive<<4) | head);
		if (status_ok())
			return 1;
	} while (--retry);
	return 0;
}


static void hd_out(unsigned int drive,unsigned int head,
                   unsigned int cyl,unsigned int sect,unsigned int nsect)
{
	
	if (!controller_ready(drive, head)) {
		puts("\nhd_out hd busy!");
		while(1);
		return;
	}
	
	outb_p(HD_DC, 0x8); 
	
	outb_p(HD_NSECTOR,nsect);
	outb_p(HD_SECTOR,sect);
	outb_p(HD_LCYL,cyl);
	outb_p(HD_HCYL,cyl>>8);
	outb_p(HD_DRVHED,0xA0|(drive<<4)|head);
	
}

static void hd_lba_out(unsigned int drive,unsigned int lba,unsigned int sect)
{
	
	int retry = 100000;
	
	do {
		if (controller_busy() & BUSY_STAT)
			puts("hd_lba_out driver busy!");
		
		if (status_ok())
			break;
	} while (--retry);
	
	outb_p(HD_DC, 0x8); 
	
	outb_p(HD_NSECTOR,sect);
	outb_p(HD_SECTOR,lba);
	outb_p(HD_LCYL,lba>>=8);
	outb_p(HD_HCYL,lba>>=8);
	outb_p(HD_DRVHED,0xE0|((drive<<4)&0x10)|((lba>>8)&0x0f)); 
	
	
}



static  int wait_DRQ(void)
{
	int retries = 100000, stat;
	
	while (--retries > 0)
		if ((stat = inb_p(HD_STATUS)) & DRQ_STAT)
			return 0;
		
		return -1;
}


int hd_read(unsigned int sector_no, unsigned char *buff)
{
    unsigned int c, h, s,temp;
	int i, retries = 100000;
	
	c = sector_no/(hdinfo.heads*hdinfo.sectors);
	temp = sector_no%(hdinfo.heads*hdinfo.sectors);
	h = temp/hdinfo.sectors;
	s = temp%hdinfo.sectors+1;
	
    hd_lba_out(0,sector_no,1);
	
	outb(HD_COMMAND,WIN_READ);
	
	do {
		i = (unsigned) inb_p(HD_STATUS);
		if (i & BUSY_STAT)
			continue;
		if (!OK_STATUS(i))
			break;
		if (i & DRQ_STAT)
			goto ok;
	} while (--retries > 0);
	
	puts("hd_read logic error");
ok:
	insw(HD_DATA,buff,256);
	
	return 1;
}



int hd_write(int sector_no, unsigned char *buff)
{
	int c, h, s,temp;
    int repeats = 1000;
	
	c = sector_no/(hdinfo.heads*hdinfo.sectors);
	temp = sector_no%(hdinfo.heads*hdinfo.sectors);
	h = temp/hdinfo.sectors;
	s = temp%hdinfo.sectors+1;
	
repeat:
    if(!repeats)
    {
		inb_p(HD_STATUS);
		return 1;
    }
    hd_out(0,h,c,s,1);
	outb(HD_COMMAND,WIN_WRITE);
	if (wait_DRQ()) {
		goto repeat;
	}
	outsw(HD_DATA,buff,256);
	return 1;
}






