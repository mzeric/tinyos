#ifndef _HMEMORY_
#define _HMEMORY_

#include <type.h>

typedef struct mem_struct{
char count;     //  引用数
long size;   //  内存块大小
long start;  //  起始地址
struct mem_struct*next;
}mem_struct;



#endif
