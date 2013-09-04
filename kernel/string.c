/*
���ߣ�UESTC:\����Ц
�ļ�����string.c
���ܣ�����ز�������kpf(),i2a()��
���ڣ�~2007-3-31
*/
#include<asm.h>
#include<type.h>
#include<string.h>

void memcopy(char* des, char* sou, ulong len)
{
	char* t = des;char* p = sou;ulong i;
	
	for (i = 0; i < len; i++)
		*t++ = *p++;
}

void memset(char* des, int data, ulong len)
{
	int i;
	for (i = len; i!=0; i--)
		*(des++) = data;

}

int strlen(char* str)
{
	char* p = str;
	int i = 0;
	while (*p++)
		i++;
	return i;
}

int strcmp(char*a,char*b){
	while(*a&&*b){
		if(*a++!=*b++)
			return -1;
	}
	if(*a||*b)return 1;
	return 0;
}


/**/
int i2a(long i, char* a, int rax)
{
	unsigned long k;
	int m = 0, n, j;
	if (rax == 10)
		j = 9;
	else if (rax == 16)
		j = 7;
	else if (rax == 2)
		j = 31;
	else if (rax == 8)
		j = 10;
	else
		return -1;//Ŀǰֻ֧��2,4,8,10,16����
	
	for (n = 0,k = 1; n < j; n++,k *= rax)
		;
	if (i == 0) {
		a[0] = '0';return 0;
	}
	while (!(i / k))
		k /= rax;
	while (k) {
		if (i / k % rax > 9)
			a[m++] = i / k % rax + 'a' - 10;
		else
			a[m++] = i / k % rax + '0';
		k /= rax;
	}
	return 0;
}



char out_char(int d){
	if(d<10)
		return d+'0';
	else
		return d+'A'-10;
	
}


/*
int i2a(long i, char* a, int rax)//�Ľ���Ĵ���䳤�����ٶȼӿ죨�����������ˣ���Ȼ���ܳ󡭡���
{
	unsigned long k;
	int m = 0, n, j;
	if(rax==16)
	{
		int tmp,pos;
		k=i;m=0;pos=0;
		
		for(n=0;n<8;n++)
		{
			k=(k<<4)|(k>>28);
			tmp=k&0x0f;
			if(tmp==0&&m==0)continue;
			else
			{
				m=1;
				a[pos++]=out_char(tmp);
				
			}
			
		}
		return 0;
		
	}
	else if(rax==2)
	{	
		int tmp,pos;
		k=i;m=0;pos=0;
		
		for(n=0;n<32;n++)
		{
			k=(k<<1)|(k>>31);
			tmp=k&0x1;
			if(tmp==0&&m==0)continue;
			else
			{
				m=1;
				a[pos++]=out_char(tmp);
				
			}
			
		}
		return 0;
	}
	else if (rax == 10)
		
		j = 9;
	else if (rax == 2)
		j = 31;
	else if (rax == 8)
		j = 10;
	else 
		return -1;
	
	
	for (n = 0,k = 1; n < j; n++,k *= rax)
		;
	if (i == 0) {
		a[0] = '0';return 0;
	}
	while (!(i / k))
		k /= rax;
	while (k) {
		if (i / k % rax > 9)
			a[m++] = i / k % rax + 'a' - 10;
		else
			a[m++] = i / k % rax + '0';
		k /= rax;
	}
	
	return 0;
	
	
	
}
*/
void kpf(char* fmt, ...)
{
	char buf[32] = {0};   
	char* f = fmt, * t = buf;unsigned long * b = &fmt;
	int i = 1;
	
	for (; *f; f++) {
		if (*f == '%') {
			t = buf;//�հ�
			memset(buf, 0, 32);
			switch (*(++f)) {
			case 'd':
				//�������0�����ѽ����
				i2a(b[i++], buf, 10);
				puts(t); break;
			case 'x':
				i2a(b[i++], buf, 16);
				while(*t)putc(*t++); break;
			case 'c':
				putc(b[i++]);break;
			case 's':
				puts(b[i++]);break;//��
			case 'o':
				i2a(b[i++], buf, 8);
				puts(t); break;
			case 'b'://���������
				i2a(b[i++], buf, 2);
				puts(t); break;
			default:
				putc(*f);
			}
		} else if (*f == '\n') {
			//��֧��\t
			newline();
		} else if(*f=='\t'){
			puts("     ");
		} else {
			putc(*f);
		}
	}
}


