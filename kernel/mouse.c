/*
作者：UESTC:\夏天笑(skysmiler)
文件名：mouse.c
功能：鼠标驱动
日期：~2006-5-31
*/
#include<mouse.h>
#include<video.h>
#include<asm.h>
#include<type.h>
#include<i386.h>
#include<message.h>
#include<kernel.h>
#include<string.h>
void mouse_asm(void);

#define MLEFTDOWN 1
#define MRIGHTDOWN 2

unsigned long mouse_x,x_sign;
unsigned long mouse_y,y_sign;
unsigned long mouse_z,z_sign;

MSG mouse_msg;

void do_mouse(){
	char ch;
	
	
	mouse_msg.id=MID_MOUSE;//MID_MOUSE==4;
	
	
	put_message_kqueue(&kqueue,&mouse_msg);
	
	
	
	
	ch=inb(0x60);
//	if(ch&0x01)_puts("left down",5,40);else _puts("left up  ",5,40);
//	if(ch&0x02)_puts("right down",6,40);else _puts("right up  ",6,40);
//	x_sign=ch&0x10?0xffffff00:0;
//	y_sign=ch&0x20?0xffffff00:0; 
	ch=inb(0x60);
//	mouse_x+=x_sign|ch;
	ch=inb(0x60);
//	mouse_y+=y_sign+=ch;
	ch=inb(0x60);
//	mouse_z+=z_sign+=ch;
	//_kpf(7,30," X:%d ",mouse_x);
	outb(0x20,0x20);
	outb(0xa0,0x20);
	
}
void en_mouse(){
	out(0x64,0xa8);
	out(0x64,0xd4);
	out(0x60,0xf4);
	out(0x64,0x60);
	out(0x60,0x47);
	
}
void init_mouse(){
	uint oldcolor;
	puts("   init mouse   ");
	en_mouse();
	set_idt_gate(32+12,(uint)mouse_asm,0x8e00);
	
	en_irq(2);
	en_irq(12);
	oldcolor=getcolor();
	setcolor(4);
	puts("[OK]");
	newline();
	setcolor(oldcolor);
	mouse_z=0;
	mouse_x=0;
	mouse_y=0;
	
}




