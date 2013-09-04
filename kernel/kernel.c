/*
作者：UESTC:\夏天笑(skysmiler)
文件名：kernel.c
功能：系统初始化，启动内核
日期：~2006-5-31
*/
#include<asm.h>
#include<i386.h>
#include<kbd.h>
#include<mouse.h>
#include<video.h>
#include<type.h>
#include<signal.h>
#include<message.h>
#include<kernel.h>
#include<string.h>
#include<sched.h>


MSG_QUEUE kqueue;
extern int text_mode;
typedef struct sysinfo
{
	long memory;
	int  monitor;
	struct {
		u32 eax;
		u32 ebx;
		u32 edx;
		u32 ecx;
	}cpu;
}sysinfo;
sysinfo brightskyinfo;

void system_call_asm(void);


void test_system_call()//进程切换测试
{
	_putc('m', 4, 50);
}
void test_task_switch()
{
	_putc('T', 5, 50);
	
	while (1);
}
/*						*/
void detect_system(sysinfo*info){
	int i;
	char c=*(unsigned short*)0x410&0x30;
	kpf("system information:\n\t");
	info->monitor=c;
	c!=0x30?kpf("Monitor: Color\n"):kpf("Monitor: Monochrome\n");
	
	struct 	{char*str;}cpu_str[]={
		"DualProcessors",
			"!386",
			"!486",
			"Pentium",
			"PentiumPro"};
		
		__asm (
			"mov $0,%%eax \n\t"
			"cpuid":
		"=a"(info->cpu.eax),
			"=b"(info->cpu.ebx),
			"=c"(info->cpu.ecx),
			"=d"(info->cpu.edx));
		kpf("CPU -> ID: ");
		for(i=0;i<12;i++)
			putc(((char*)&(info->cpu.ebx))[i]);
		
		__asm(
			"mov $1,%%eax \n\t"
			"cpuid":
		"=a"(i));
		i=(i>>8)&0xf;
		if(i>6)kpf("Style UnKnown!\n");
		else kpf("\n    -> Style: %s\n",cpu_str[i-2]);
		
}

int main()
{
	MSG kmsg;
	int i;
	
	cli();
	
	vga_init(0, 0, 2);//before the kpf()！;
	clear();
	setcolor(2);
	
    detect_system(&brightskyinfo);
	
	kpf("init system ...\n");
	
	//__asm__("lgdt %0\n\t"::"m"(&gdt_addr));
	
	init_8259a();
	init_gdt();
	init_idt();
	
	init_kbd();
	init_mouse();
	
	//switch_vga_mode(vga12h);
	//clear_vram();
	
	//hd_write(1,buf);
	//hd_read(1,b);
	
	kpf("init message queue\n");
	init_message_kqueue(&kqueue);
	
	kpf("init system_call\n");
	
	set_invoke_gate(3, (ulong) system_call_asm, 0);//调用门：gdt_table[3]
	//system_call();//系统调用测试
	
	
	
	empty_kbd_mouse();
	en_irq(0);//enable timer interrupt
	sti();
	
	kpf("Hello,Brightsky!\n");
	
	/*	atomic_t ato;
	ato.counter=4;
	atomic_inc(&ato);
	kpf("i:%d\n",ato.counter);
	atomic_dec(&ato);
	kpf("i:%d\n",ato.counter);
	
	*/
	proc_init();
	test1();
	
	init_con();
	
	
	while (1) {
		
		i = get_message_kqueue(&kqueue, &kmsg);
		if (i != 1)
			continue;
		
		switch (kmsg.id) {
		case MID_MOUSE:
			//here is:鼠标处理
			break;
		case MID_KBD:
			if(text_mode)
				con_write();
			else
				switch_to_text();
			break;
		default:
			kpf("U:%x ", kmsg.id);//未知类型
		}
	}//end while
}
