/*
作者：UESTC:\夏天笑
文件名：kernel.c
功能：进程管理
日期：~2006-5-31

  by skysmiler
  
	proc_init()要……
	内存管理较简单
	哎，还是功能验证，累呀！
	######################
	multi-thread done now at 2006-10-31!!!
*/
#include<asm.h>
#include<i386.h>
#include<sched.h>
#include<type.h>
#include<video.h>

#define TIME_ALIVE 30
void switch_to(proc_t*t);
 proc_t* pro_table[32]={NULL};
 ulong pro_index=NULL;

proc_t proc[16];  
proc_t* next_proc;
proc_t* prev_proc; 
proc_t* curr_proc; // 当前运行的进程
proc_t* proc_head; // 进程链表的第一个进程
proc_t* proc_tail; // 进程链表中的最后一个进程
int switching;
int clock_tick;

void test1(void){
	putc('A');
//	curr_proc=&proc[0];
//	prev_proc=curr_proc;
	switch_to(&proc[1]);
	putc('a');
	switch_to(&proc[1]);
	putc('1');
	switch_to(&proc[1]);
	kpf("\nSwitch process OK\n");
//	while(1);
}
void test2(void){
//	BEGIN();

	putc('B');
	switch_to(&proc[0]); 
	putc('b');
	kpf(" ha ha ,we make it ! ");
	switch_to(&proc[0]);
	putc('2');
    switch_to(&proc[0]);
	while(1);
	
//	EXIT();
}
void switch_mm(void){//恢复页目录	
}
void save_stack(){	
}

void switch_to(proc_t*next_p)
{
//	static regs_t regs_temp;	
//	static regs_t regs_current;
	prev_proc=curr_proc;
	curr_proc=next_p;
	
	//save registers
/* ulong tp;
if(a++==1){
__asm(
	"mov %%esp,%0\n\t"
:"=m"(tp));
kpf("test2:esp:%x",tp);while(1);
}
*/
	//保存现场!!!
__asm volatile(
		"movl %%ebp,%0\n\t"
		"movl %%eax,%1\n\t"
		"movl %%ebx,%2\n\t"
		"movl %%ecx,%3\n\t"
		"movl %%edx,%4\n\t"
		"movl %%esp,%5\n\t"
		"movl %%edi,%6\n\t"
		"movl %%esi,%7\n\t"
		"pushfl\n\t"
		"popl %8\n\t"
		:"=m"(prev_proc->regs.ebp),
		"=m"(prev_proc->regs.eax),
		"=m"(prev_proc->regs.ebx),
		"=m"(prev_proc->regs.ecx),
		"=m"(prev_proc->regs.edx),
		"=m"(prev_proc->regs.esp),
		"=m"(prev_proc->regs.edi),
		"=m"(prev_proc->regs.esi),
		"=m"(prev_proc->regs.eflags)
		);

	//恢复现场（堆栈）
	//当开优化3时，eax会用作指针!!!，跳转前eax要保护!!!

//if(a++==1){	kpf("esp:%x;ebp:%x\n",p->regs.eip,prev_proc->regs.ebp);	while(1);}

	__asm volatile(
	"movl %2,%%ebx\n\t"
	"movl %3,%%ecx\n\t"
	"movl %4,%%edx\n\t"
	"movl %5,%%esp\n\t"
	"movl %6,%%edi\n\t"
	"movl %7,%%esi\n\t"
	"pushl %8     \n\t"
	"popfl        \n\t"
	"movl $1f,%10 \n\t"         //store   eip
	"sti          \n\t"
	"movl $0,%11  \n\t"        //switching = 0
	"movl %0,%%ebp\n\t"
	"movl %9,%%eax\n\t"
	"push %%eax   \n\t"
	"movl %1,%%eax\n\t"       //gcc affairs !!! use eax for addressing when -O3 ?
 //   "jmpl _switch_to\n\t"
	"ret\n\t"                 //restore eip
//  "jmp  *%9     \n\t"     
	"1:           \t"
	::
	"m"(curr_proc->regs.ebp),
	"m"(curr_proc->regs.eax),
	"m"(curr_proc->regs.ebx),
	"m"(curr_proc->regs.ecx),
	"m"(curr_proc->regs.edx),
	"m"(curr_proc->regs.esp),//%5
	"m"(curr_proc->regs.edi),
	"m"(curr_proc->regs.esi),
	"m"(curr_proc->regs.eflags),
	"m"(curr_proc->regs.eip),//%9
	"m"(prev_proc->regs.eip),
	"m"(switching));


}


void proc_init(void){
	proc[0].pid=0;
	proc[0].next=&proc[1];
	proc[0].regs.eip=(unsigned long)test1;
	
	proc[1].pid=1;
	proc[1].next=&proc[0];
	proc[1].regs.eip=(unsigned long)test2;
	curr_proc=&proc[0];
	proc[1].regs.ebp=0x300000;
	proc[1].regs.esp=0x300000;
	
	
	
}
int get_current(){
	return curr_proc->pid;
}
void proc_clock(void)
{
	return;
}

