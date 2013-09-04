/*
作者：UESTC:\夏天笑
文件名：kbd.c
功能：键盘驱动
日期：~2006-5-31
*/
#include<video.h>
#include<asm.h>
#include<type.h>
#include<kbd.h>
#include<signal.h>
#include<i386.h>
#include<kernel.h>
#include<message.h>

void kbd(void);

char code_buf[16]={0};


MSG kbd_msg;

int  get_kbd_buf(){
	if(kbd_buf.head==kbd_buf.tail)
		return -1;
	kbd_buf.head=++kbd_buf.head%16;
	return kbd_buf.buf[kbd_buf.head];
}
int put_kbd_buf(char ch){
	int t;
	t=(kbd_buf.tail+1)%16;
	if(kbd_buf.head==t)return -1;
	kbd_buf.tail=t;
	kbd_buf.buf[kbd_buf.tail]=ch;
	return 0;
	
}

void  do_kbd(){
	
	char data,ch;
	
	
    data=inb(0x60);
	//if(!(data&0x80)){//make code
	//ch=keymap[data];
	ch=data;	
	_putc(ch,0,40);
	kbd_msg.pram=ch;
	put_kbd_buf(ch);
	
	kbd_msg.id=MID_KBD;
    put_message_kqueue(&kqueue,&kbd_msg);//
	
	
	
	
	
	outb(0x20,0x20);//send EOI to reset irr in 8259a
	
	
}

void init_kbd(void){
	
	uint oldcolor;
	
	puts("   init keyboard");
	
	outb(0x60,0xed);//set led
	outb(0x60,0x04);//caps|num|soroll ！点亮CAPS Lock
	
	outb(0x60,0xf0);//chose keymap set 1
	outb(0x60,0x01);
	
	kbd_buf.head=0;
	kbd_buf.tail=0;
	
	set_idt_gate(32+1,(uint)kbd,0x8e00);
	en_irq(1);
	
	
	oldcolor=getcolor();
	setcolor(4);
	puts("[OK]");
	newline();
	setcolor(oldcolor);
}
/*晴空											*/
void empty_kbd_mouse(){
	int i;
	for(i=0;i<3;i++)
		inb(0x60);
}
/*I/O                                           */  
uchar readch()
{
	
	uchar ch;
wait:
	ch =inb(0x64); 
	if(!(ch&1)) 
		goto wait; //until output buf full
	return inb(0x60);
}

uint translate_char(char ch){
	
}
