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

/*	��������									*/
typedef struct gdt_des{
	unsigned short limit ;//����;
	unsigned short basel ;//0-15;
	unsigned short type_basem ;//16-23   //�����ǵ�����0x8c��������0x85��TSS������0x89
	unsigned short baseh_t_l  ;//|������4|����19-16|;
	
}gdt_des;
/*	ȫ��������								*/
extern gdt_des gdt;
gdt_des*gdt_table;


/*	�ж���									*/
typedef struct idt_gate{
  unsigned short offsetl;   
  unsigned short codesel;
  unsigned short type_res;
  unsigned short offseth;
}idt_gate;

/*	������									*/
typedef struct invoke_gate{
  unsigned short offsetl ;	 // ƫ������16λ 
  unsigned short selector ;	 // ��ѡ��� 
  unsigned char  saved_stack ; //  ����Ϊ0-3|ջcopy������λ��˫��-5; 
  unsigned char  type ;      //  ����λ-1|��Ȩ��-2|ϵͳ������Ϊ0-1|������Ϊ0xC-4  >0x8C;
  unsigned short offseth ;
}invoke_gate;

/*	������									*/
typedef struct task_gate{		//ʵ���Ͻṹ���ж���һ�������ԡ�������������������gdt��
  unsigned short offset1;    //not used
  unsigned short tss_sel;
  unsigned short type_saved  ;    // p|dpl|0|type-0101|saved:0;//0x8500;
  unsigned short offseth;   //not used
}task_gate;
/*	TSS�ţ�������								*/
typedef struct tss_des{//TSS��������������������ֻ�ǽ���һ��gate��^_^
  unsigned short limitl;   //size of tss��0-15λ
  unsigned short basel ;   //��16λ
  unsigned char  basem ;   //16-23λ
  unsigned char  type  ;   //p|dpl|
  unsigned char  limith_t; //G-1|0|0|AVL|limit 16-19
  unsigned char  baseh ;   //��ַ24-31λ
}tss_des;					//des can't be fixed in idt
/*	����״̬��								*/
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
