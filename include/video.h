#ifndef _HVIDEO_
#define _HVIDEO_

#include<type.h>
#include<asm.h>

void _view_init(int rows,int cols,int color);
void vga_init();
void _puts(char*str,int x,int y);
void _putc(char ch,int x,int y);
void puts(char*str);
void putc(char ch);


void _int_w();    //测试函数
void _int_x();
void nweline(void);
void linehome(void);

void setcolor(unsigned int color);
uint getcolor(void);
void setcursor(int row,int col);
void set_cursor(void);
void clear(void);

void switch_vga_mode(void);
void vga_pixel(unsigned int x, unsigned int y, unsigned int c);
void clear_vram(void);


unsigned int v_color;//前景色
unsigned int c_rows;
unsigned int c_cols;
int text_mode;

//VGA设置
#define VRAM_T ((unsigned char*)0x000b8000)
#define COLS  80
#define ROWS  25
#define VRAM_G ((unsigned char*)0x000a0000)
#define WIDTH 640
#define HEIGHT 480

#define SPACING 2
#define CHAR_HEIGHT 16
#define LINE_HEIGHT (CHAR_HEIGHT + SPACING)
#define V_POS	(LINE_HEIGHT * ROWS)
#define V_HEIGHT	(HEIGHT - LINE_HEIGHT * ROWS)

#define ASC_WIDTH 8
#define CHS_WIDTH 16
#define ASC_NUM 256
#define CHS_QU 87
#define CHS_WEI 94
#define CHS_NUM (CHS_QU * CHS_WEI)//字库

//VGA寄存器

#define VGA_AC_INDEX 0x3C0
#define VGA_AC_WRITE 0x3C0
#define VGA_AC_READ 0x3C1
#define VGA_MISC_WRITE 0x3C2

#define VGA_SEQ_INDEX 0x3C4//时序发生器
#define VGA_SEQ_DATA 0x3C5 
/*
控制所有VGA功能的总定时信号,并完成某些存储地址译码工作
有5个寄存器,复用两个I/O地址
*/
#define VGA_DAC_READ_INDEX 0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA 0x3C9
#define VGA_MISC_READ 0x3CC

#define VGA_GC_INDEX 0x3CE//图形控制器
#define VGA_GC_DATA 0x3CF  
/*							
0 置位/复位寄存器
1 允许置位/复位寄存器
2 颜色比较寄存器
3 数据移位和函数选择寄存器
4 读平面选择寄存器
5 方式寄存器
6 混合寄存器
7 颜色忽略寄存器
8 位屏蔽寄存器
*/

#define VGA_INSTAT_READ 0x3DA
#define VGA_CTL   0x3D4
#define VGA_CRTC_INDEX 0x3D4//CRT控制器！慎用！易损坏CRT ？	      
#define VGA_CRTC_DATA 0x3D5		/* 0x3B5 */
#define VGA_IN     0x3D4
#define VGA_DATA   0x3D5


#define VGA_NUM_SEQ_REGS  5
#define VGA_NUM_CRTC_REGS 25
#define VGA_NUM_GC_REGS   9
#define VGA_NUM_AC_REGS   21
#define VGA_NUM_REGS (VGA_NUM_SEQ_REGS+VGA_NUM_CRTC_REGS+VGA_NUM_GC_REGS+VGA_NUM_AC_REGS+1)


#endif

