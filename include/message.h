#ifndef _HMESSAGE_
#define _HMESSAGE_

#define MAX_KENEL_MSG_QUEUE  8  //内核消息队列最大长度


#define MID_TIMER    0x1
#define MID_KBD      0x2
#define MID_MOUSE    0x3

typedef  unsigned int MSGTYPE ;
typedef  struct _position{
	unsigned int x;
	unsigned int y;
}POINT;
typedef struct _message{
	unsigned int id;
	MSGTYPE  msg_type;
	POINT    pos;
	int	pram;
}MSG;

typedef struct _msg_queue{
	int head;
	int tail;
	MSG msg[MAX_KENEL_MSG_QUEUE];//buf for test;
}MSG_QUEUE;

void init_message_kqueue(MSG_QUEUE*msg);
int get_message_kqueue(MSG_QUEUE*msgq,MSG*msg);
int put_message_kqueue(MSG_QUEUE*msgq,MSG*msg);
int test(MSG*msgq,int i);



#endif


