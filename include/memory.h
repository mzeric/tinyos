#ifndef _HMEMORY_
#define _HMEMORY_

#include <type.h>

typedef struct mem_struct{
char count;     //  ������
long size;   //  �ڴ���С
long start;  //  ��ʼ��ַ
struct mem_struct*next;
}mem_struct;



#endif
