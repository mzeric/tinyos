/*
作者：UESTC:\夏天笑
文件名：string.c
功能：串相关操作：含kpf(),i2a()等
日期：~2006-5-31
*/
#include<asm.h>
#include<type.h>
#include<string.h>

void memcopy(char* des, char* sou, ulong len)
{
	char* t = des;char* p = sou;ulong i;
	/*	_asm__ __volatile__(
	"std\n\t"
	"rep\n\t"
	"movsb\n\t"
	"cld"
	: "=&c" (d0), "=&S" (d1), "=&D" (d2)
	:"0" (len),
	"1" (len-1+src),
	"2" (len-1+dst)
	:"memory");
	*/
	for (i = 0; i < len; i++)
		*t++ = *p++;
}

void memset(char* des, int data, ulong len)
{
	int i;
	for (i = 0; i < len; i++)
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
		return -1;//目前只支持2,4,8,10,16进制
	
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
void kpf(char* fmt, ...)
{
	char buf[32] = {0};   
	char* f = fmt, * t = buf;unsigned long * b = &fmt;
	int i = 1;
	
	for (; *f; f++) {
		if (*f == '%') {
			t = buf;//险啊
			memset(buf, 0, 32);
			switch (*(++f)) {
			case 'd':
				//不能输出0！（已解决）
				i2a(b[i++], buf, 10);
				puts(t); break;
			case 'x':
				i2a(b[i++], buf, 16);
				while(*t)putc(*t++); break;
			case 'c':
				putc(b[i++]);break;
			case 's':
				puts(b[i++]);break;//！
			case 'o':
				i2a(b[i++], buf, 8);
				puts(t); break;
			case 'b'://二进制输出
				i2a(b[i++], buf, 2);
				puts(t); break;
			default:
				putc(*f);
			}
		} else if (*f == '\n') {
			//不支持\t
			newline();
		} else if(*f=='\t'){
			puts("     ");
		} else {
			putc(*f);
		}
	}
}


