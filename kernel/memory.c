/*

  作者：UESTC:\夏天笑
  文件名：memory.c
  功能：内存操作
  日期：~2006-5-31
  
	简单的内存管理部分
	无页表操作（虽然使用了分页机制）
	所以也就少了线性地址到物理地址的转换过程
	由于没有文件系统，虚拟内存管理还待实现（没了调入和换出就简单了哈）
	当然，也可以使用可变大小的内存块,并用一个结构来跟踪记录内存块的大小、起始地址等，这也许是个好主意
	但，因打算在后续版本中实现虚拟内存，所以用了这个过渡的方法:
	最小分配单元为4K，最大分配块数为1K，共计4M bytes
*/
#include <memory.h>


#define MEM_BLOCK_NUM 1024
#define MEM_BLOCK_SIZE (4*1024)
#define MEM_SIZE ((MEM_BLOCK_SIZE)*(MEM_BLOCK_NUM))
#define MEM_ADDRESS_START (4*1024*1024)

static mem_struct mem_block[MEM_BLOCK_NUM]={0};


int get_free_struct(){
	int i;
	for(i=0;i<MEM_BLOCK_NUM;i++)
		if(mem_block[i].count==-1){
			mem_block[i].count=0;
			return i;
		}
		return -1;
}

void mem_merge(void){
	mem_struct*m,*p;
	for(p=mem_block ; p ; p=p->next){
		if(p->next==NULL)return;
		m=p->next;
		if(p->count==0&&m->count==0){
			p->size+=m->size;
			m->start=0;m->count=-1;
			p->next=m->next;
		}
	}
	
	
}

char*kmalloc(long size){
	long t;
	mem_struct*p;
	if(size<=0||size>MEM_SIZE)
		return NULL; //
	if(size%MEM_BLOCK_SIZE)
		size=(size/MEM_BLOCK_SIZE+1)*MEM_BLOCK_SIZE;
	for(p=mem_block ; p ; p=p->next){
		if(p->count==0&&p->size>=size){
			//get one
			t=get_free_struct();
			if(t<0)	return NULL;//无可分配内存！
			mem_block[t].next=p->next;//开始解链
			p->next=&mem_block[t];
			
			p->next->size=p->size-size;p->size=size; 	
			p->count=1;	p->next->count=0;
			p->next->start=p->start+p->size;
			return (char*)p->start;
			
		}
	}
	return NULL; //无可分配内存！
}

int kfree(char*a){
	mem_struct*p;
	for(p=mem_block ; p ; p=p->next){
		if(p->start==(unsigned long)a){
			p->count-=1;
			if(p->count!=0)return -2;//本不该有共享内存！
			mem_merge();
			return 0;
		}
	}
	return -1; //试图释放不存在的内存空间！
}

void mem_init(){
	int i;
	for(i=0;i<MEM_BLOCK_NUM;i++){
		
		mem_block[i].count=-1;
		mem_block[i].next=NULL;
		mem_block[i].size=0;
		mem_block[i].start=NULL;
	}
	
	mem_block[0].count=0;
	mem_block[0].size=MEM_SIZE;
	mem_block[0].start=MEM_ADDRESS_START;
	
	
}



