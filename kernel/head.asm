[bits 32]
;汇编初始化部分
;设置页表，内核栈，GDT,IDT，中断入口
;系统调用的辅助汇编部分
;

_kpg_dir equ 0
_pgt0 equ 0x1000
_pgt1 equ 0x2000
extern _kernel_main,_do_kbd,_do_nop
extern _do_mouse,_do_timer,_test_system_call
global _start ,_idt ,_kbd ,_hd_asm,_nop ;引出以便ld识别
global _mouse_asm,_timer_asm
global _gdt_addr,_gdt
global _system_call_asm

SECTION .text

_start:			;内核开始的地方,代码段
mov ax,0x10
mov ds,ax		;重复设置寄存器->和setup.asm中一样
mov ss,ax

;jmp $

        
;设置页目录,8M,2 entry
mov dword[_kpg_dir],_pgt0+7   ;页表采用绝对定位
mov dword[_kpg_dir+4],_pgt1+7 ;分段已经屏蔽(setup.asm)


        ;设置两个页表, 平坦映射
	mov edi,_pgt0     ;
	mov eax,0x007     ;
   fill_pte:	
        mov [edi],eax     
	add eax,0x1000    ;4K
	add edi,4         ;next
        cmp eax,0x800007  ;0x7ff007(7为属性)
        jl  fill_pte

;mov dword[_pgt0+4*4],0xb8007 ;测试用分页表项
;mov dword[_pgt1+4],0xb8007   ;mov [0x401000],cl

        ;准备开启分页,设置cr3为页目录地址
	mov eax,_kpg_dir	; pg_dir  
	mov cr3,eax		; load pg_dir 

        ;开启分页
	mov eax,cr0
	or  eax,0x80000000
	mov cr0,eax		;ShowTime

	
        jmp page_flush          ;刷新快表
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
mov esp,_k_top	;设置内核栈
;设置中断门

     mov edx,_nop
     mov eax,0x00080000;段选择符(data段)
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


;此时CPU尚未开中断,8259A也禁止中断

lidt	[idt_descr]

mov eax,0
call _kernel_main

jmp $

_mouse_asm:      ;鼠标
pushad
call _do_mouse
popad
iret;

_kbd:            ;键盘
pushad
call _do_kbd
popad
iret;

_timer_asm:	 ;时钟
pushad
call _do_timer
popad
iret;


_system_call_asm:		;系统调用
pushad
call _test_system_call
popad
retf



_nop:              ;不进行error code的处理
;xchg eax,[esp+4]
;push eax
pushad
call _do_nop
popad
iret;


;中断返回
;数据段
section .data

align 2
dw 0

_k_stack:	;4k内核栈
times 4096 db 0 
_k_top equ $	;栈顶
_idt: times 256*8 db 0  ; 
idt_descr:              ;装载idt 的伪描述符
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
            dw      0xffff;段限(4G不分段)
            dw      0x0000;0-15b
            dw      0x9a00;低8位为16-23
            dw      0x00cf;高8位为24-31
gdt_system_data:
            dw      0xffff
            dw      0x0000
            dw      0x9200
            dw      0x00cf 

times 250*8 db 0
