#ifndef _HI386_
#define _HI386_
#include<type.h>
extern unsigned long idt ;

extern unsigned long gdt_addr;


void set_gdt_des(ushort index,ushort limit,ushort basel,ushort type_basem,ushort baseh_t_l);
void set_idt_gate(ushort index ,ulong offset,ushort type_res);
void set_invoke_gate(ushort index,ulong offset,uchar stack);
void system_call();
void init_idt(void);
void init_gdt(void);
void init_8259a(void);
void en_irq(uchar irq);

/*	段描述符									*/
typedef struct gdt_des{
	unsigned short limit ;//段限;
	unsigned short basel ;//0-15;
	unsigned short type_basem ;//16-23   //可以是调用门0x8c，任务门0x85，TSS描述符0x89
	unsigned short baseh_t_l  ;//|段属性4|段限19-16|;
	
}gdt_des;
/*	全局描述表								*/
extern gdt_des gdt;
gdt_des*gdt_table;


/*	中断门									*/
typedef struct idt_gate{
  unsigned short offsetl;   
  unsigned short codesel;
  unsigned short type_res;
  unsigned short offseth;
}idt_gate;

/*	调用门									*/
typedef struct invoke_gate{
  unsigned short offsetl ;	 // 偏移量低16位 
  unsigned short selector ;	 // 段选择符 
  unsigned char  saved_stack ; //  保留为0-3|栈copy数，单位是双字-5; 
  unsigned char  type ;      //  存在位-1|特权级-2|系统描述符为0-1|调用门为0xC-4  >0x8C;
  unsigned short offseth ;
}invoke_gate;

/*	任务门									*/
typedef struct task_gate{		//实际上结构和中断门一样，可以‘当作’段描述符放入gdt中
  unsigned short offset1;    //not used
  unsigned short tss_sel;
  unsigned short type_saved  ;    // p|dpl|0|type-0101|saved:0;//0x8500;
  unsigned short offseth;   //not used
}task_gate;
/*	TSS门？描述符								*/
typedef struct tss_des{//TSS当不属门描述符，这里只是借用一下gate名^_^
  unsigned short limitl;   //size of tss，0-15位
  unsigned short basel ;   //低16位
  unsigned char  basem ;   //16-23位
  unsigned char  type  ;   //p|dpl|
  unsigned char  limith_t; //G-1|0|0|AVL|limit 16-19
  unsigned char  baseh ;   //基址24-31位
}tss_des;					//des can't be fixed in idt
/*	任务状态段								*/
typedef struct tss_struct{
unsigned long back_link;	// 16 high bits zero 
unsigned long esp0;
unsigned long ss0;		    // 16 high bits zero 
unsigned long esp1;
unsigned long ss1;		    // 16 high bits zero 
unsigned long esp2;
unsigned long ss2;		    // 16 high bits zero 
unsigned long cr3;
unsigned long eip;
unsigned long eflags;
unsigned long eax,ecx,edx,ebx;
unsigned long esp;
unsigned long ebp;
unsigned long esi;
unsigned long edi;
unsigned long es;			// 16 high bits zero 
unsigned long cs;			// 16 high bits zero 
unsigned long ss;			// 16 high bits zero 
unsigned long ds;			// 16 high bits zero 
unsigned long fs;			// 16 high bits zero 
unsigned long gs;			// 16 high bits zero 
unsigned long ldt;			// 16 high bits zero 
unsigned long bitmap_trace;	// bits:  bitmap 16-31,trace 0
}tss_struct;            //104 bytes total

#endif
