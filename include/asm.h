#ifndef _HASM_
#define _HASM_
/*
大部分的宏汇编码都在这
包含I/O,开/关中断，原子操作


*/
#define outb(port,value) \
__asm__ volatile ("outb %%al,%%dx"::"a" (value),"d" (port))
#define inb(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
_v; \
})

#define out(port,value) __asm__( "out %%al , %%dx" : : "a"(value),"d"(port));
//unsigned char in( unsigned short port )
//{
//  unsigned char data ;
//  __asm__( "in %%dx , %%al" : "=a"( data ) : "d"( port ) ) ;
//  return data ;
//}

#define outw(port,value) \
__asm__ volatile ("outw %%ax,%%dx"::"a" (value),"d" (port))
#define inw(port) ({ \
ushort _v; \
__asm__ volatile ("inb %%dx,%%ax":"=a" (_v):"d" (port)); \
_v; \
})
#define outl(port,value) \
__asm__ volatile ("outl %%eax,%%dx"::"a" (value),"d" (port))


#define outb_p(port,value) \
__asm__ volatile ("outb %%al,%%dx\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:"::"a" (value),"d" (port))
#define inb_p(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al\n" \
	"\tjmp 1f\n" \
	"1:\tjmp 1f\n" \
	"1:":"=a" (_v):"d" (port)); \
_v; \
})

#define outsw(port,buf,nr) \
__asm__("cld;rep;outsw"::"d" (port),"S" (buf),"c" (nr))
#define insw(port,buf,nr) \
__asm__("cld;rep;insw"::"d" (port),"D" (buf),"c" (nr))


#define cli() \
__asm__ volatile ("cli")

#define sti() \
__asm__ volatile ("sti")


/*	atomic operator		*/
typedef struct { volatile int counter; } atomic_t;

static __inline__ void atomic_inc(atomic_t *v)
{
	__asm__ volatile (
		 "incl %0"
		:"=m" (v->counter)
		:"m"  (v->counter));

}
static __inline__ void atomic_dec(atomic_t *v)
{
	__asm__ __volatile__(
		 "decl %0"
		:"=m" (v->counter)
		:"m"  (v->counter));
}


#endif
