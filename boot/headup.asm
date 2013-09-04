[bits 16]
[org 0x7e00]

_setup:
jmp start_setup
lod db 'Setup runing OK !'
    db 13,10,0
m_a20 db 'A20 Gate Opend !'
    db 13,10,0
m_ker db 'Loading Kernel OK !'
    db 13,10,0
m_pm  db 'Here Entering Protect Model !'
    db 13,10,0
m_8259A db 'Reprogram The 8529A !'
      db 13,10,0
SETUPSEG  equ 0x0000   ; �ᱻgdt���� 
SETUPOFF  equ 0x7e00
SETUPSEC  equ  2
SYSSEC    equ  64

SYSSEG    equ 0x0000;real model address
SYSOFF    equ 0x8000
BOOTDRIVER equ 0;floppy a
;----------------------------�������� 
print:
lodsb
or al,al
jz return
call dis_char
jmp print
return:
ret

dis_char:
mov ah,0eh
int 10h
ret
;----------------------------GDT��
gdt_addr:
            dw      256*8-1        
            dw      _gdt           
            dw      0x0000    ;_gdt��ƫ����,�˴�Ϊ16λ�Ķ�ַ
_gdt:

gdt_null:
            dw      0x0000
            dw      0x0000
            dw      0x0000
            dw      0x0000
gdt_system_code:
            dw      0xffff;����(4G���ֶ�)
            dw      0x0000;0-15b
            dw      0x9a00;��8λΪ16-23
            dw      0x00cf;��8λΪ24-31
gdt_system_data:
            dw      0xffff
            dw      0x0000
            dw      0x9200
            dw      0x00cf   
;---------------------------

;�ȴ����̿��������е��ӳ���

Empty_8042:  

  in                al , 0x64

  test              al , 0x2

  jnz               Empty_8042

  ret

;+++++++++++++++++++++++++++
start_setup:

mov ax,SETUPSEG
mov ds,ax
mov es,ax
mov ss,ax
mov sp,0x07c00

mov si,lod
call print
;jmp $
;��A20��ַ��

call              Empty_8042
mov               al , 0xd1 
out               0x64 , al
call              Empty_8042
mov               al , 0xdf
out               0x60 , al
call              Empty_8042
mov si,m_a20
call print
;װ���ں�
;load_system:
;mov               ax , SYSSEG  ;setupseg        
;mov               es , ax
;mov               bx , SYSOFF  ;SETUPOFF + SETUPSEC*512
;mov               ah , 2
;mov               dh , 0
;mov               dl , BOOTDRIVER
;mov               ch , 0
;mov               cl , 1 + 1 + SETUPSEC    
;mov               al , SYSSEC           
;int               0x13
;jc                load_system

mov si,m_ker
call print

;�ƶ�system��0x5000

cli;���ж�ֱ��main
jmp movend
;cld
;mov si,SETUPOFF+SETUPSEC*512
;mov ax,SYSSEG
;MOV es,ax
;mov di,SYSOFF
;MOV cx,SYSSEC*128
;rep movsd
 
movend:
;��д8259A�ж�����
mov al,0x11;  
out 0x20,al
dw 0x00eb ,0x00eb
out 0xa0,al; set ����
dw 0x00eb ,0x00eb

mov al,0x20; first from 20h
out 0x21,al
dw 0x00eb ,0x00eb
mov al,0x28
out 0xa1,al; second from 28h
dw 0x00eb ,0x00eb

mov al,0x4;  
out 0x21,al; set 8259a 1 master
dw 0x00eb ,0x00eb
mov al,0x2
out 0xa1,al; set 8259a 2 slave
dw 0x00eb ,0x00eb

mov al,0x1;  
out 0x21,al
dw 0x00eb ,0x00eb
out 0xa1,al; 8086 mode for two chips
dw 0x00eb ,0x00eb

mov al,0xff; ���������ж�
out 0x21,al
dw 0x00eb ,0x00eb
out 0xa1,al

mov si,m_8259A
call print

lgdt [gdt_addr]

mov si,m_pm
call print
;����32λ����ģʽ
jmp enter_pm
enter_pm:
mov               eax , cr0
or                eax , 1
mov               cr0 , eax 
jmp pm
pm:
 
jmp dword         0x8:SYSOFF  ;

;after finishing the booting of comupter,setup of the hardware
;as the prepare for work of Kernel,now it's the time to

jmp $
times 510-($-$$)db 0
 dw 0xaa55