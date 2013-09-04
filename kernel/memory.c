/*

  作者：UESTC:\夏天笑
  文件名：memory.c
  功能：内存操作
  更新日期：~2006-11-5
  
	简单的内存管理部分
	无页表操作（虽然使用了分页机制）
	所以也就少了线性地址到物理地址的转换过程
	由于没有文件系统，虚拟内存管理还待实现（没了调入和换出就简单了哈）
	当然，也可以使用可变大小的内存块,并用一个结构来跟踪记录内存块的大小、起始地址等，这也许是个好主意
	但，因打算在后续版本中实现虚拟内存，【所以用了这个过渡的方法:
	最大分配单元为4K，分配已采用存储桶，共计4M bytes】
	
	  
*/
#include <memory.h>

#define MEM_START (4*1024*1024)
#define MEM_END   (8*1024*1024)
#define MEM_TOTAL (MEM_END-MEM_START)
#define MEM_BLOCK_SIZE (4096)

#define MEM_BLOCK_NUM MEM_TOTAL/MEM_BLOCK_SIZE

#define MEM_REF_ZERO  0xf0000000

static unsigned char mem_map[MEM_BLOCK_NUM]={0};


struct bucket_desc{
	ulong page;
	ulong free;
	struct bucket_desc*next;
	   ulong  used;
};
struct s_dir{
	int size;
	struct bucket_desc*chain;       
}; 
static struct s_dir storage[8];
/*
16|32|64|128|256|512|1024|2048
*/
static struct bucket_desc* first_free_desc=NULL;


void*alloc_page(){

	ulong i=0;

	while(mem_map[i])i++;
	mem_map[i]=1;

	return (void*)(MEM_START+1024*4*i);
}
//
int  free_page(char *p_addr){

	mem_map[(ulong)(p_addr-MEM_START)>>12]=0;

	return 0;
}
static int get_chain_index(int size){
    int i;

    for(i=0;i<sizeof(storage)/sizeof(struct s_dir);i++)
		if(size<=storage[i].size)
		{
			//kpf("%x",i);
			return i;
		}
		
		
		return -1;
}
static void*init_bucket(void*p_addr,int size,struct bucket_desc *p_desc){

    struct bucket_desc *p=p_desc;
	int block_num,i;
	char*t;

	p->page=(ulong)p_addr;
	p->free=(ulong)p_addr;
	block_num=4096/size;
	
	for(i=1,t=(char*)p->free;i<block_num;i++){
		*(ulong*)t=size+(ulong)t;
		t+=size;
	}
	*(ulong*)t=NULL;
	
	return 0;
}

static struct bucket_desc* get_unused_desc(){
	
	struct bucket_desc *p_desc,*p=first_free_desc;
	void*p_addr;int desc_num=0;
	int i=0;

	if(p){
		if(p->used!=0)return NULL;//Error!!!;
		first_free_desc=first_free_desc->next;
		
		return p;
		
	}//no unused desc now;
	
	p_addr=alloc_page();
	if(p_addr==NULL)return NULL;
	p_desc=(struct bucket_desc*)p_addr;
	desc_num=MEM_BLOCK_SIZE/sizeof(struct bucket_desc);
	for(i=0;i<desc_num-1;i++){
		p_desc[i].used=0;
		p_desc[i].next=&p_desc[i+1];
	}
	p_desc[desc_num-1].next=(first_free_desc==NULL)?NULL:first_free_desc->next;
	p=first_free_desc=p_desc;
	
	//	p->used=1;
	first_free_desc=(first_free_desc==NULL)?NULL:first_free_desc->next;
	//	kpf("[p:%x] ",p);
	return p;
	
	
	return NULL; 
}


static int attach_chain(int index,struct bucket_desc*p_desc){
	struct bucket_desc*p=p_desc;

	p->next=storage[index].chain;
	storage[index].chain=p;
	
	
	return 0;   
}
static struct bucket_desc*get_bucket_desc(int index){
    struct bucket_desc *p=NULL;
	void*p_addr=NULL;
	int size;

    p = storage[index].chain;
    size = storage[index].size;
	
	do{
		
		if(p==NULL){//first use this chain or all the chain is full;
			p=(struct bucket_desc *)get_unused_desc();
			if(p==NULL){return NULL;}
			p_addr=alloc_page();//kpf("[chain:%x] ",p_addr);
			if(p_addr==NULL)return NULL;
			init_bucket(p_addr,size,p);
			attach_chain(index,p);		
			
		}
		if((p->free)!=NULL)
			return p;
		p=p->next;
		
    }while(1);
	
	
}





char *kmalloc(int size){
	int index;
	
//	kpf("size:%d ",size);
	index=get_chain_index(size);
	if(index<0)return NULL;//参数无效
	struct bucket_desc*p=get_bucket_desc(index);
//	kpf("des:%x ",p);//if(uon==1)while(1);
//	kpf("desc.free:%x ",p->free);
	char*t=(char*)p->free;
	p->free=*(ulong*)t;
	*(ulong*)t=0;
	p->used++;
	
	return t;
	
}

static int free_desc(int index,struct bucket_desc*p_desc){
	
	struct bucket_desc *t,*p_next,*p=p_desc;
	
	if(p->used!=0)return -2;
	t=storage[index].chain;
	
	p->page=0;
	p_next=p->next;
	
	if(t==p){
		storage[index].chain=NULL;
		p->next=first_free_desc;
		first_free_desc=p;//kpf("[del.chain] ");
		return 0;
	}
	while(t){
		if(t->next==p){
			t->next=NULL;
			p->next=first_free_desc->next;
			first_free_desc=p;
			return 0;
		}
		t=t->next;
	}
	return -1;
}

int kfree(char*p_addr){
	int tidy,num,i;
	ulong t;
	struct bucket_desc*p=NULL;
	
//	kpf("%x ",p_addr);
	tidy=(ulong)p_addr&(0xffffffff<<12);
	num=sizeof(storage)/sizeof(struct s_dir);
	for(i=0;i<num-1;i++)
	{
		p=storage[i].chain;
		
		while(p){
			
			if(p->page==tidy){//find it;
				
				t=p->free;
				while(*(ulong*)t){
					if(t==(ulong)p_addr)
						return -2;
					t+=storage[i].size;//try to free free block!!!;
				}
				t=p->free;
				p->free=(ulong)p_addr;
				*(ulong*)p_addr=t;//unmask the block;

				if((--p->used)==0){
					free_page((char*)p->page);
					free_desc(i,p);
				}
//				kpf("sfree:%x,used:%d ",p->page,p->used);
				return 0;
			}
			p=p->next;
			
		}
	}
	
	return -1;
	
}


void mem_stat(){

	int i,j;
	int free_num,used_num;
	struct bucket_desc*p=first_free_desc;

	free_num=0;

	while(p){

		free_num++;
		p=p->next;

	}

	kpf("MemStatics;\n   FreeDescs: 0x%x\n   ",free_num);

	j=sizeof(storage)/sizeof(struct s_dir);
	used_num=0;

	for(i=0;i<j;i++)
	{
		p=storage[i].chain;
		
		while(p){			
			used_num++;
			p=p->next;			
		}
	}

	kpf("Used_Descs: 0x%x\n",used_num);
}

void mem_init(){
	int i;
	for(i=0;i<MEM_BLOCK_NUM;i++)
		mem_map[i]=0;
	for(i=0;i<sizeof(storage)/sizeof(struct s_dir);i++){
		storage[i].size=1<<(4+i);
		storage[i].chain=NULL;
	}

//	for(i=0;i<sizeof(storage)/sizeof(struct s_dir);i++)
//		kpf("[%d]",storage[i].size);
		mem_stat();
	 

}

