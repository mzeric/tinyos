/*

  ���ߣ�UESTC:\����Ц
  �ļ�����memory.c
  ���ܣ��ڴ����
  ���ڣ�~2006-5-31
  
	�򵥵��ڴ������
	��ҳ���������Ȼʹ���˷�ҳ���ƣ�
	����Ҳ���������Ե�ַ�������ַ��ת������
	����û���ļ�ϵͳ�������ڴ������ʵ�֣�û�˵���ͻ����ͼ��˹���
	��Ȼ��Ҳ����ʹ�ÿɱ��С���ڴ��,����һ���ṹ�����ټ�¼�ڴ��Ĵ�С����ʼ��ַ�ȣ���Ҳ���Ǹ�������
	����������ں����汾��ʵ�������ڴ棬��������������ɵķ���:
	��С���䵥ԪΪ4K�����������Ϊ1K������4M bytes
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
			if(t<0)	return NULL;//�޿ɷ����ڴ棡
			mem_block[t].next=p->next;//��ʼ����
			p->next=&mem_block[t];
			
			p->next->size=p->size-size;p->size=size; 	
			p->count=1;	p->next->count=0;
			p->next->start=p->start+p->size;
			return (char*)p->start;
			
		}
	}
	return NULL; //�޿ɷ����ڴ棡
}

int kfree(char*a){
	mem_struct*p;
	for(p=mem_block ; p ; p=p->next){
		if(p->start==(unsigned long)a){
			p->count-=1;
			if(p->count!=0)return -2;//�������й����ڴ棡
			mem_merge();
			return 0;
		}
	}
	return -1; //��ͼ�ͷŲ����ڵ��ڴ�ռ䣡
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



