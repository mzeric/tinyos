#ifndef _HSCHED_
#define STACK_NUM	512*1024

#include<type.h>

typedef struct regs_t {
	long	ebx;
	long	ecx;
	long	edx;
	long	esi;
	long	edi;
	long	ebp;
	long	eax;
	long	ds;
	long	es;	/* index of execption */
	long	index;	/* index of interrupt, error of execption */
	long	eip;
	long	cs;
	long	eflags;
	long	esp;
	long	ss;
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
