/*
���ߣ�UESTC:\����Ц
�ļ�����message.c
���ܣ���Ϣ���Ƶ�ʵ��
���ڣ�~2006-5-31
*/
#include<asm.h>
#include<type.h>
#include<video.h>
#include<message.h>
#include<string.h>


void init_message_kqueue(MSG_QUEUE* msgq)
{
	msgq->head = 0;
	msgq->tail = 0;
}
int get_message_kqueue(MSG_QUEUE* msgq, MSG* msg)
{
	cli();//�����жϡ�������ֵ��������ֵ
	if (msgq->head == msgq->tail) {
		sti();
		
		return 0;
	}
	msgq->head = (++msgq->head) % MAX_KENEL_MSG_QUEUE; 
	*msg = msgq->msg[msgq->head];
	
	sti();
	return 1;
}
int put_message_kqueue(MSG_QUEUE* msgq, MSG* msg)
{
	int t;
	cli();
	t = (msgq->tail + 1) % MAX_KENEL_MSG_QUEUE;
	if (msgq->head == t) {
		sti();
		return -1;
	}
	msgq->tail = t;
	
	msgq->msg[msgq->tail] = *msg; 
	sti();
	return 1;
}

