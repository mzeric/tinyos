/*
作者：UESTC:\夏天笑
文件名：kernel.c
功能：控制台驱动
内部命令：help，credit，vga-test，sysinfo，whoami
日期：~2006-5-31
修改：~2010-10-20
*/

#include<type.h>
#include<asm.h>
#include<video.h>
#include<string.h> 
#include<kbd.h>
#include<console.h>
/*define in video.h   

  unsigned int v_color;//前景色
  unsigned int c_rows; //行号
  unsigned int c_cols; //列号
  
*/
unsigned int colums;

int top;
int bottom;
int scr_start;
int pos;
#define SCRSTART 0xb8000
#define POS ( SCRSTART+((c_rows*80) + c_cols)*2)

#define UPSTEP   160
#define PROM_LOGO "cmd#"

void del_char()
{
	if (c_cols>strlen(PROM_LOGO)) {
		c_cols--;
		*(unsigned short *) POS = 0x0700;
		set_cursor();
	}
}

/*
硬件滚屏，速度快，但软件更灵活
*/
/*
void scrup(int c){
//软件滚屏
cli();
outb(0x3d4,16);
outb(0x3d5,0xff&((c)>>9));
outb(0x3d4,17);
outb(0x3d5,0xff&((c)>>1));
sti();

  }
*/

void scrup(void)
{
/*
可嵌入汇编，那样效率会更高
	*/
	int i, c;
	unsigned long des,sou;
	
	des = VRAM_T;
	sou = des + UPSTEP;
	
	for (i = 0; i < c_rows; i++) {
		memcopy(des, sou, COLS << 1);
		des += UPSTEP;
		sou += UPSTEP;
	}
	
	memset(des, 0, COLS << 1);
	c_rows--;
	setcursor(c_rows, c_cols);
}

char* get_line(int num)
{
	static char buf[COLS];
	char* p = (char*) (SCRSTART+num*UPSTEP),* t = buf;
	memset(buf, 0, COLS);
	while (*p) {
		*t = *p;
		t++;p += 2;
	}
	return (buf+strlen(PROM_LOGO));
}
void prom_logo(){
	newline();
	kpf(PROM_LOGO);
}
char scan_decode1(char ch)
{
	char cook;
	
	cook = keymap[ch];
	
	return cook;
}
char scan_decode2(char ch)
{
	return 0xff & ch;
}
void con_write()
{
	int d, ch;
	static int count = 1;
	d = get_kbd_buf();//原始扫描码
	
	if ((0xff&d)==0xe0){
		count=2;
		return;
	}
	
	if (!(d & 0x80)){
		//make code
		if (count==2){
			count=1;
			ch=scan_decode2(d);
			
		} else {
			ch=scan_decode1(d);//ASSII码
		}
		sti();
		switch (ch) {
		case '\n':
			exec_inter(get_line(getrows()));
			prom_logo();
			set_cursor();
			break;
		case '\b':
			del_char();
			break;
		case K_HOME:
			set_rl(c_rows,1+strlen(PROM_LOGO));
			set_cursor();
			set_rl(c_rows,0+strlen(PROM_LOGO));
			break;
			
			
		default:
			putc(ch);
			set_cursor();
		}
	}
	count=1;//reset the break code 
}
#define  x_vga 640
#define  y_vga 480

void vga_test(void)
{
	
	kpf("VGA_now");
	switch_vga_mode();
	clear_vram();
	text_mode=0;
	
	
	line(100,300,240,10,1);
	line(380,300,240,10,2);
	line(100,300,380,300,4);
	
	
}
void credit(void)
{
	kpf("\tWelcome to this OS [Brightsky]\n"
		"this a simple edition,but it's very interesting\n\tjust enjoy! ** ^_^ **"
		"\n\t\t\t\t\t\t---- Skysmiler	\n\t\t\t\t\t\t(With me:xiatianxiao@gmail.com )");
}

void exec_inter(char* str)
{
	newline();
	if (!strcmp(str, "vga-test"))
		vga_test();
	else if (!strcmp(str, "credit"))
		credit();
	else if(!strcmp(str,"help"))
		kpf("internal command:\thelp,credit,sysinfo,who[whoami],vga-test");
	else if(!strcmp(str,"sysinfo"))
		detect_system();
	else if(!strcmp(str,"whoami")||!strcmp(str,"who"))
		kpf("\tBrightsky 0.04 by Skysmiler build at 2010-10-20 with mingw");
	else
		kpf("     Unknown command!");
	return ;
}

void init_con()
{
	kpf(PROM_LOGO);
}
