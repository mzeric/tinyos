/*
作者：UESTC:\夏天笑
文件名：!386.c
功能：!386cpu 相关设置，中断门，调用门，各种描述符，任务状态段
时钟中断服务例程，
日期：~2006-5-31
*/
#include<i386.h>
#include<type.h>
#include<video.h>
#include<asm.h>


static ushort  _irr=0xffff;

extern void system_call_asm(void);
extern void proc_clock(void);

void set_idt_gate(ushort index,ulong offset,ushort type_res){
	idt_gate*p=(idt_gate*)&idt+index;
	
	p->offsetl=(ushort)offset;
	p->codesel=0x8;
	p->type_res=type_res;
	p->offseth=(ushort)(offset>>16);
	
}


void set_invoke_gate(ushort index,ulong offset,uchar stack){
	invoke_gate test_invoke;
	
	test_invoke.selector=0x8;
	test_invoke.saved_stack=stack;  // 栈copy数，单位是双字 
	test_invoke.type=0x8c ;      // 调用门为0xC-4|系统描述符为0-1 |特权级-2| 存在位-1 >0xC1;
	test_invoke.offseth=(ulong)offset >>16 ;
	test_invoke.offsetl=(ulong)offset;//见鬼 ！，gcc会生成错误的代码？！
	
	gdt_table[index]=*((gdt_des*)&test_invoke);
}


void set_gdt_des(ushort index,ushort limit,ushort basel,ushort type_basem,ushort baseh_t_l){
	gdt_table[index].limit=limit;
	gdt_table[index].basel=basel;
	gdt_table[index].type_basem=type_basem;
	gdt_table[index].baseh_t_l=baseh_t_l;
}
void system_call(){
	__asm__ ("lcall $0x18,$0");
}

void init_gdt(){
	gdt_table=&gdt;
}

void nop(void);
void do_nop(ulong code){
	kpf("nop!");
	outb(0x20,0x20);
}
void timer_asm(void);
void do_timer(void){
    static char ch='a';
	_putc(ch++,0,COLS-1);
	//proc_clock();
	outb(0x20,0x20);
	
}

void init_8259a(void){
	uint oldcolor;
	puts("   init 8259a   ");
	outb(0x20, 0x11); /* ICW1 */
	outb(0xA0, 0x11);
	
	outb(0x21, 0x20); /* ICW2 */
	outb(0xA1, 0x28); /* 重置向量号IRQ：0->INT 0x20 */
	
	outb(0x21, 0x04); /* ICW3 */
	outb(0xA1, 0x02);
	
	outb(0x21, 0x01); /* ICW4 */
	outb(0xA1, 0x01);
	
	/*Disable all 8259 irq*/
	outb_p(0x21, 0xff);
	outb_p(0xA1, 0xff);
	oldcolor=getcolor();
	setcolor(4);
	puts("[OK]");
	newline();
	setcolor(oldcolor);
	
}

void en_irq(uchar irq){
	_irr&=(~(1<<irq));
	if(irq&0x8)
		outb(0xa1,_irr>>8 );
	else outb(0x21,_irr );
}





/*	中断调用点												*/



#if  0
/*未显著调用这些函数，而只是定义符号供ld链接
保留异常/故障/错误
*/
void divide_error(void); 
void debug(void);
void nmi(void);
void int3(void);
void overflow(void);
void bounds(void);
void invalid_op(void);
void device_not_available(void);
void double_fault(void);
void coprocessor_segment_overrun(void);
void invalid_tss(void);
void segment_not_present(void);
void stack_segment(void);
void general_protection(void);
void page_fault(void);
void coprocessor_error(void);
void alignment_check(void);
void simd_float_point(void);
void machine_check(void);
void reserved(void);


#endif



void init_idt(){
	int i;
	for(i=0;i<255;i++) //默认中断调用
		set_idt_gate(i,(uint)nop,0x8e00);
	set_idt_gate(32+0,(uint)timer_asm,0x8e00);//timer_asm 在 head.asm
											  /*	
											  set_idt_gate(0,(uint)divide_error,0x8e00);
											  set_idt_gate(1,(uint)debug,0x8e00);
											  set_idt_gate(2,(uint)nmi,0x8e00);
											  set_idt_gate(3,(uint)int3,0x8e00);
											  set_idt_gate(4,(uint)overflow,0x8e00);
											  set_idt_gate(5,(uint)bounds,0x8e00);
											  set_idt_gate(6,(uint)invalid_op,0x8e00);
											  set_idt_gate(7,(uint)device_not_available,0x8e00);
											  set_idt_gate(8,(uint)double_fault,0x8e00);
											  set_idt_gate(9,(uint)coprocessor_segment_overrun,0x8e00);
											  set_idt_gate(10,(uint)invalid_tss,0x8e00);
											  set_idt_gate(11,(uint)segment_not_present,0x8e00);
											  set_idt_gate(12,(uint)stack_segment,0x8e00);
											  set_idt_gate(13,(uint)general_protection,0x8e00);
											  set_idt_gate(14,(uint)page_fault,0x8e00);
											  set_idt_gate(15,(uint)reserved,0x8e00);
											  set_idt_gate(16,(uint)coprocessor_error,0x8e00);
											  set_idt_gate(17,(uint)alignment_check,0x8e00);
											  set_idt_gate(18,(uint)machine_check,0x8e00);
											  set_idt_gate(19,(uint)simd_float_point,0x8e00);
											  for(i=20;i<32;i++)
											  set_idt_gate(i,(uint)reserved,0x8e00);
											  
	*/
	
	
	
	
}


//标准0-31号intel中断
void cdivide_error(ulong esp,ulong code){
}
void cdebug(ulong esp,ulong code){
}
void cnmi(ulong esp,ulong code){
}
void cint3(ulong esp,ulong code){
}
void coverflow(ulong esp, ulong code){	
}
void cbounds(ulong esp, ulong code){	
}
void cinvalid_op(ulong esp, ulong code){
}
void cdevice_not_available(ulong esp, ulong code){	
}
void cdouble_fault(ulong esp,ulong code){
}
void ccoprocessor_segment_overrun(ulong esp, ulong code){
}
void cinvalid_tss(ulong esp,ulong code){	
}
void csegment_not_present(ulong esp,ulong code){
}
void cstack_segment(ulong esp,ulong code){
}
void cgeneral_protection(ulong esp,ulong code){
}
void cpage_fault(ulong esp,ulong code){
}
void ccoprocessor_error(ulong esp, ulong code){	
}
void calignment_check(ulong esp,ulong code){
}
void cmachine_check(ulong esp,ulong code){
}
void csimd_float_point(ulong esp,ulong code){
}
void creserved(ulong esp, ulong code){	
}
