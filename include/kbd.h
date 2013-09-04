#ifndef _HKBD_
#define _HKBD_

#define KP_EN  0x1C 
#define R_CTRL 0x1D  

#define KP_/ 0x35  
#define R_ALT 0x38 

#define K_HOME 0x47 
#define K_UP   0x48 
#define PG_UP 0x49

#define K_LEFT 0x4B 
#define K_RIGHT 0x4D 

#define K_END 0x4F 
#define K_DOWN 0x50 
#define PG_DOWN 0x51 
#define K_INSERT 0x52 
#define K_DELETE 0x53

#define L_GUI 0x5B
#define R_GUI 0x5C  
#define APPS 0x5D 

void do_kbd();
void init_kbd();
void kbd_wait();
uchar readch();

uint translate_char(char ch);

 struct{
	char buf[16];
	int head;
	int tail;

}kbd_buf;

static const  char keymap[256]={
	0,0x01,'1','2','3','4','5','6','7','8','9','0',
	'-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0x1d,
	'a','s','d','f','g','h','j','k','l',';','\'','`',0x2a,'\\',
	'z','x','c','v','b','n','m',',','.','/',0x36,0x37,0x38,' ',0x3a};
	



#endif
