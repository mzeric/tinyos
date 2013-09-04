#ifndef _HSCHED_
#define STACK_NUM	512*1024

#include<type.h>

typedef struct regs_t {
unsigned	long	ebx;
unsigned	long	ecx;
unsigned	long	edx;
unsigned	long	esi;
unsigned	long	edi;
unsigned	long	ebp;
unsigned	long	eax;
unsigned	long	ds;
unsigned	long	es;	/* index of execption */
unsigned	long	index;	/* index of interrupt, error of execption */
unsigned	long	eip;
unsigned	long	cs;
unsigned	long	eflags;
unsigned	long	esp;
unsigned	long	ss;
} regs_t;

typedef struct proc_t
{
	int		pid; 
	int		parent;
    int     state;

	long    time;    //×ÜÔËÐÐµÎ´ð
	struct proc_t	*next; 
	struct proc_t	*prev; 
	
	regs_t regs;   
	int    total_tick;
} proc_t;
#endif
