/*
作者：UESTC:\夏天笑
文件名：kernel.c
功能：进程管理
日期：~2006-5-31

  by skysmiler
  
	proc_init()要……
	内存管理较简单
	哎，还是功能验证，累呀！
*/
#include<asm.h>
#include<i386.h>
#include<sched.h>
#include<type.h>
#include<memory.h>

	static regs_t regs_current;
	static regs_t regs_temp;	

#define TIME_ALIVE 30
proc_t proc[16];  
proc_t* next_proc;
proc_t* prev_proc; 
proc_t* curr_proc; // 当前运行的进程
proc_t* proc_head; // 进程链表的第一个进程
proc_t* proc_tail; // 进程链表中的最后一个进程
int switching;
int clock_tick;

int sssss;
void test1(void){
	putc('A');

    curr_proc=&proc[0];

	prepare_to_switch();
	switch_to(&proc[1]);
	putc('C');
	while(1);
}
void test2(void){
	sssss=123;
	putc('B');
    
//	kpf("proc[0]:%x\n",proc[0].regs.eip);
	curr_proc=&proc[1];

	prepare_to_switch();
	switch_to(&proc[0]);

putc('D');

	while(1);
	
}
void switch_mm(void){//恢复页目录
	
}
void save_stack(){
	
	
	
	
}
void prepare_to_switch(void){
	
	
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
		:"=m"(regs_current.ebp),
		"=m"(regs_current.eax),
		"=m"(regs_current.ebx),
		"=m"(regs_current.ecx),
		"=m"(regs_current.edx),
		"=m"(regs_current.esp),
		"=m"(regs_current.edi),
		"=m"(regs_current.esi),
		"=m"(regs_current.eflags)
		);
	curr_proc->regs.ebp = regs_current.ebp;
	curr_proc->regs.eax = regs_current.eax;
	curr_proc->regs.ebx = regs_current.ebx;
	curr_proc->regs.ecx = regs_current.ecx;
	curr_proc->regs.edx = regs_current.edx;
	curr_proc->regs.esp = regs_current.esp;
	curr_proc->regs.edi = regs_current.edi;
	curr_proc->regs.esi = regs_current.esi;
	curr_proc->regs.eflags = regs_current.eflags;

//kpf(",curr_stack[%x:%x]\n",curr_proc->regs.ebp,curr_proc->regs.esp);

	
}
void _switch_to(proc_t* prev_p , proc_t* next_p){




	}
void switch_to(proc_t* p)
{
	
	//恢复现场（堆栈）
	// 跳到指定的进程中去运行，eip排到最后
	// 注意!!!!!!!!!!!!!!!!
	// gcc 在此将要使用eax做间接寻址，所以在第二行用eax的时候不行!!!
	regs_temp.ebp = p->regs.ebp;
	regs_temp.eip = p->regs.eip;
	regs_temp.eax = p->regs.eax;
	regs_temp.ebx = p->regs.ebx;
	regs_temp.ecx = p->regs.ecx;
	regs_temp.edx = p->regs.edx;
	regs_temp.esp = p->regs.esp;
	regs_temp.edi = p->regs.edi;
	regs_temp.esi = p->regs.esi;
	regs_temp.eflags = p->regs.eflags;


//kpf(" prev_proc->eip:%x curr_proc->eip:%x \n",prev_proc->regs.eip,curr_proc->regs.eip);
//kpf("switch:%x\n",switching);

//kpf("proc[0].eip:%x proc[1].eip:%x\n",proc[0].regs.eip,proc[1].regs.eip);
	


	__asm volatile(
	"movl %0,%%ecx\n\t"
	"movl %1,%%edx\n\t"
	"movl %2,%%edi\n\t"
	"movl %3,%%esi\n\t"
	"pushl %4     \n\t"
	"popfl        \n\t"
::
	"m"(regs_temp.ecx),
	"m"(regs_temp.edx),
	"m"(regs_temp.edi),
	"m"(regs_temp.esi),
	"m"(regs_temp.eflags)
	 );
//if(sssss==123){
		//while(1);
//	}
//kpf("jmpto:[%x] saveas:[%x] ,next_stack:[%x:%x],curr_stack[%x:%x]\n",regs_temp.eip,curr_proc->regs.eip,
//	                           regs_temp.ebp,regs_temp.esp,
//	                           curr_proc[0].regs.ebp,curr_proc->regs.esp);

	__asm volatile(
	"movl $1f,%1 \n\t"         //store   eip
	"sti          \n\t"
	"movl $100,%2  \n\t"        //switching = 0

"movl %2,%%ebx\n\t"
"movl %3,%%eax\n\t"
"movl %4,%%ebp\n\t"
"movl %5,%%esp\n\t"


	"jmp  *%0    \n\t"     //restore eip
//	"ret          \n\t    "
	"1:           \t"
	::"m"(regs_temp.eip),//%9
	"m"(curr_proc->regs.eip),
	"m"(regs_temp.ebx),
	"m"(regs_temp.eax),//%3
	"m"(regs_temp.ebp),
	"m"(regs_temp.esp),
	"m"(switching));
	
}
int proc_schedule(void)
{
	// 进行一些进进程管理
	// printk("当前正在运行的进程是%d\n",next_proc->pid);
	// 进入此函数时有堆栈变化，最好能把它变回，因为它并不返回!!!!	
	// 错!!!不必管堆栈，因为在clock_interrupt里面已经保存了堆栈的值了，哈哈
	
	if (next_proc->next == NULL) {
		// 只有这一个进程运行，跳回去继续运行
		switch_mm();
		switch_to(next_proc);
	}
	
	next_proc = next_proc->next; // 转到下一个进程
	switch_to(next_proc); // 切换过去
	
	return 1;
}
void _proc_init(proc_t*p){
	//	p->
	
	
	
}

void proc_init(void){

	proc[0].pid=0;
	proc[0].next=&proc[1];
	proc[0].regs.eip=test1;
	
	proc[1].pid=1;
	proc[1].next=&proc[0];
	proc[1].regs.eip=test2;
	proc[1].regs.ebp=0x0;
	proc[1].regs.esp=0x50000;
	proc[1].regs.eax=0;
	proc[1].regs.ebx=0;
	proc[1].regs.ecx=0;

	curr_proc=&proc[0];
	
	
	
}
int get_current(){
	return curr_proc->pid;
}
void proc_clock(void)
{
	static regs_t regs_current;
	
	if (switching == 1) {
		
		return;
	}
	
	clock_tick++;
	if (clock_tick == TIME_ALIVE) {
		switching = 1; // 正在切换
		// 保存现场
		// 首先是ebp和eip，要把这两个都弹出来
		__asm__("popl %0\n\t" // 弹出ebp
			"popl %1\n\t" // 弹出eip，现在的esp就是原来未进中断时的esp
			"movl %%eax,%2\n\t"
			"movl %%ebx,%3\n\t"
			"movl %%ecx,%4\n\t"
			"movl %%edx,%5\n\t"
			"movl %%esp,%6\n\t"
			"movl %%edi,%7\n\t"
			"movl %%esi,%8\n\t"
			"pushfl\n\t"
			"popl %9\n\t"
			:"=m"(regs_current.ebp),
			"=m"(regs_current.eip),
			"=m"(regs_current.eax),
			"=m"(regs_current.ebx),
			"=m"(regs_current.ecx),
			"=m"(regs_current.edx),
			"=m"(regs_current.esp),
			"=m"(regs_current.edi),
			"=m"(regs_current.esi),
			"=m"(regs_current.eflags)
			);
		
		clock_tick = 0;
		
		next_proc->regs.eip = regs_current.eip;
		next_proc->regs.ebp = regs_current.ebp;
		next_proc->regs.eax = regs_current.eax;
		next_proc->regs.ebx = regs_current.ebx;
		next_proc->regs.ecx = regs_current.ecx;
		next_proc->regs.edx = regs_current.edx;
		next_proc->regs.esp = regs_current.esp;
		next_proc->regs.edi = regs_current.edi;
		next_proc->regs.esi = regs_current.esi;
		next_proc->regs.eflags = regs_current.eflags;
		
		proc_schedule();		// 这时又压进去一个eip，比原来的堆栈多了四个字节
	}
	return;
}
