/*
作者：UESTC:\夏天笑
文件名：video.c
功能：显示器的驱动
日期：~2006-5-31
*/

#include<video.h>
#include<asm.h>
#include<type.h>



//VGA初始化
void vga_init()
{
	_view_init(0, 0, 2);
	text_mode=1;
}
void _view_init(int row, int col, int color)
{
	c_cols = col;c_rows = row,v_color = color;
}
void set_rl(int rows,int cols){
	c_rows=rows;c_cols=cols;
}
//串输出，文本模式
void _puts(char* str, int x, int y)
{
	char* p = VRAM_T + (x << 1) * COLS + (y << 1);
	while (*str) {
		*p++ = *str++;
		*p++ = v_color;
	}
}
void puts(char* str)
{
	char* p = VRAM_T + (c_rows << 1) * COLS + (c_cols << 1);
	while (*str) {
		*p++ = *str++;
		*p++ = v_color;
		if(++c_cols>COLS){
			c_cols=0;
			c_rows++;
		}
	}
}

void putc(char ch)
{
	_putc(ch, c_rows, c_cols);
	if(++c_cols>COLS){
			c_cols=0;
			c_rows++;
		}
}
//字符输出
void _putc(char ch, int x, int y)
{
	char* p = VRAM_T + (x << 1) * COLS + (y << 1);
	*p++ = ch;*p = v_color;
}





//设置前景色
void setcolor(unsigned int color)
{
	v_color = color;
}
uint getcolor(void)
{
	return v_color;
}
uint getrows(void){
	return c_rows;
}
uint getcols(void){
	return c_cols;
}
//清屏
void clear()
{
	int x = 0, y = 0, c = v_color;
	setcolor(0);
	for (; x < ROWS; x++) {
		for (y = 0; y < COLS; y++)
			_putc(0, x, y);
	}
	setcolor(c);
}
//另起一行
void newline()
{
	if (++c_rows > 24){
		scrup();
		c_rows = 24;
	}
	c_cols = 0;
} 
void linehome()
{
	c_cols = 0;
}

//移动光标
void setcursor(int row, int col)
{
	unsigned short offset = col + row * 80;
	
	outb(VGA_CTL + 0, 14);  //crtc reg 14<-msb
	outb(VGA_CTL + 1, offset >> 8);
	
	outb(VGA_CTL + 0, 15);  //crtc reg 15<-lsb
	outb(VGA_CTL + 1, offset);
	
	
}
void set_cursor()
{
	cli();
	setcursor(c_rows,c_cols-1);
	sti();
}
unsigned char vga12h[VGA_NUM_REGS] = {
	0xE3, 0x03, 0x01, 0x08, 0x00, 0x06, 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80,
		0x0B, 0x3E, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0x0C,
		0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x03,
		0x00, 0x05, 0x0F, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x01, 0x00, 0x0F, 0x00,
		0x00
};
void switch_to_text(){
	return;
}
void switch_vga_mode()
{
	unsigned char*regs=vga12h;
	unsigned i;
	
	
	outb(VGA_MISC_WRITE, *regs);
	regs++;
	for (i = 0; i < VGA_NUM_SEQ_REGS; i++) {
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *regs);
		regs++;
	}
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
	for (i = 0; i < VGA_NUM_CRTC_REGS; i++) {
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *regs);
		regs++;
	}
	for (i = 0; i < VGA_NUM_GC_REGS; i++) {
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *regs);
		regs++;
	}
	for (i = 0; i < VGA_NUM_AC_REGS; i++) {
		(void) inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *regs);
		regs++;
	}
	(void) inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
}
void pixel(unsigned int x, unsigned int y, unsigned int c)
{
	unsigned char * off;
	unsigned int mask, i, pmask;
	
	x %= WIDTH;
	y %= HEIGHT;
	
	off = VRAM_G + COLS * y + (x >> 3);
	x &= 7;
	mask = 0x80 >> x;
	pmask = 1;
	for (i = 0; i < 4; i++) {
		outb(VGA_GC_INDEX, 4);
		outb(VGA_GC_DATA, i);
		outb(VGA_SEQ_INDEX, 2);
		outb(VGA_SEQ_DATA, (1 << i));
		
		if (pmask & c)
			*off |= mask;
		else
			*off &= ~mask;
		pmask <<= 1;
	}
}

void clear_vram(void)
{
	
	unsigned int i, j;
	unsigned char * off;
	
	for (i = 0; i < 4; i++) {
		outb(VGA_SEQ_INDEX, 2);
		outb(VGA_SEQ_DATA, (1 << i));
		
		off = VRAM_G;
		if ((1 << i) & 0)
			for (j = 0; j <= COLS*V_POS; j++)
				*off++ = 0x0;
			else
				for (j = 0; j <= COLS*V_POS; j++)
					*off++ = 0x0;
				
				if ((1 << i) & 0)
					for (; j < COLS * (V_POS + 1); j++)
						*off++ = 0x0;
					else
						for (; j < COLS * (V_POS + 1); j++)
							*off++ = 0x0;
						
						if ((1 << i) & 0)
							for (; j < COLS*HEIGHT; j++)
								*off++ = 0x0;
							else
								for (; j < COLS*HEIGHT; j++)
									*off++ = 0x0;
	} 
	
	return;
}

void line(int x,int y,int m,int n,int c){
	int q=1,i,j,k;
	if(m<x)q=-1;
	k=(n-y)/(m-x);
	
	for(i=0;i<(m-x)*q;i++){
		j=i*k*q+y;
		pixel(x+i*q,j,c);
	}
}

