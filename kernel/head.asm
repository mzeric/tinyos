[bits 32]
;����ʼ������
;����ҳ���ں�ջ��GDT,IDT���ж����
;ϵͳ���õĸ�����ಿ��
;

_kpg_dir equ 0
_pgt0 equ 0x1000
_pgt1 equ 0x2000
extern _kernel_main,_do_kbd,_do_nop
extern _do_mouse,_do_timer,_test_system_call
global _start ,_idt ,_kbd ,_hd_asm,_nop ;�����Ա�ldʶ��
global _mouse_asm,_timer_asm
global _gdt_addr,_gdt
global _system_call_asm

SECTION .text

_start:			;�ں˿�ʼ�ĵط�,�����
mov ax,0x10
mov ds,ax		;�ظ����üĴ���->��setup.asm��һ��
mov ss,ax

;jmp $

        
;����ҳĿ¼,8M,2 entry
mov dword[_kpg_dir],_pgt0+7   ;ҳ����þ��Զ�λ
mov dword[_kpg_dir+4],_pgt1+7 ;�ֶ��Ѿ�����(setup.asm)


        ;��������ҳ��, ƽ̹ӳ��
	mov edi,_pgt0     ;
	mov eax,0x007     ;
   fill_pte:	
        mov [edi],eax     
	add eax,0x1000    ;4K
	add edi,4         ;next
        cmp eax,0x800007  ;0x7ff007(7Ϊ����)
        jl  fill_pte

;mov dword[_pgt0+4*4],0xb8007 ;�����÷�ҳ����
;mov dword[_pgt1+4],0xb8007   ;mov [0x401000],cl

        ;׼��������ҳ,����cr3ΪҳĿ¼��ַ
	mov eax,_kpg_dir	; pg_dir  
	mov cr3,eax		; load pg_dir 

        ;������ҳ
	mov eax,cr0
	or  eax,0x80000000
	mov cr0,eax		;ShowTime

	
        jmp page_flush          ;ˢ�¿��
        page_flush:

lgdt   [_gdt_addr]
push dword 0
        popf
;jmp $
	jmp 8:sky
	sky:
 
mov ax,0x10
mov ds,ax
mov es,ax
mov ss,ax
mov fs,ax
mov gs,ax
mov esp,_k_top	;�����ں�ջ
;�����ж���

     mov edx,_nop
     mov eax,0x00080000;��ѡ���(data��)
     mov ax,dx;
     mov dx,0x8e00;
     mov edi,_idt
     mov ecx,256

fill_idt:
     mov [edi],eax
     mov [edi+4],edx
     add edi,8
     dec ecx
     jne fill_idt

loadidt:


;��ʱCPU��δ���ж�,8259AҲ��ֹ�ж�

lidt	[idt_descr]

mov eax,0
call _kernel_main

jmp $

_mouse_asm:      ;���
pushad
call _do_mouse
popad
iret;

_kbd:            ;����
pushad
call _do_kbd
popad
iret;

_timer_asm:	 ;ʱ��
pushad
call _do_timer
popad
iret;


_system_call_asm:		;ϵͳ����
pushad
call _test_system_call
popad
retf



_nop:              ;������error code�Ĵ���
;xchg eax,[esp+4]
;push eax
pushad
call _do_nop
popad
iret;


;�жϷ���
;���ݶ�
section .data

align 2
dw 0

_k_stack:	;4k�ں�ջ
times 4096 db 0 
_k_top equ $	;ջ��
_idt: times 256*8 db 0  ; 
idt_descr:              ;װ��idt ��α������
	dw  256*8-1	;idt contains 256 entries
	dd  _idt
_gdt_addr:
            dw 256*8-1
            dd _gdt
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

times 250*8 db 0
