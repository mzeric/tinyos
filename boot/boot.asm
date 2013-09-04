[bits 16]
[org 0x7c00]
jmp start
times 60-($-$$) db 0
wel db 'Arouser Booting...'
    db 13,10,0
boot db 'Boot runing ok !'
    db 13,10,0
lodsetup db 'Loading Setup......'
    db 0
ok db '[ ok ]'
    db 13,10,0
    

;
BOOTSEG   equ 0x0000
SETUPSEG  equ 0x9000
SETUPOFF  equ 0x0000
SETUPSEC  equ  2
BOOTDRIVER equ 0;floppy a
SCREENSIZE equ 0x0fa0
;__________________
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

dis_ok:
mov si,ok
call print
ret


;++++++++++++++++++++++++++++++
start:
mov ax,BOOTSEG
mov ds,ax
sti

mov si,wel
mov bl,4
call print

mov si,boot
mov bl,4
call print


mov si,lodsetup
call print
mov dh,2
mov dl,70
call dis_ok
;___________________________
;¶Ásetup.asm->0x90000
read:

mov ax,SETUPSEG
mov es,ax
mov bx,SETUPOFF
mov ah,2
mov dl,BOOTDRIVER
mov dh,0
mov ch,0
mov cl,2
mov al,SETUPSEC
int 13h
jc read



jmp SETUPSEG:SETUPOFF







times 510-($-$$) db 0
dw 0aa55h