[bits 16]
[org 0x7c00]
; this will find the BSLDR in only one cluster dir struct 
; then load it to 0x7e00 untill the end of the file , so thie file shoule not be too large
; only support FAT32
jmp start
times 0x03-($-$$) db 0 ;bpb & FAT32 or FAT16 struct data :first offset-> 0x03H

times 0x60-($-$$) db 0 ;end here
;wel db 'Arouser Booting...'
;    db 13,10,0
;m_ok db '[ ok ]'
;    db 13,10,0
m_bsldrmiss db 'BSLDR missing.'
    db 13,10,0
bsldr db 'BSLDR      '
     
m_load_ok db 'Load OK'
    db 13,10,0
;
BOOTSEG   equ 0x0000
BOOTOFF   equ 0x7c00
TMPSEG  equ 0x1000  ;avalid from 0x00500->0x07bff sub the [bp-4]*0x200 :about 29k
                    ;the bsldr will load to from 0x7e00 untill 0x9ffff :about 608k
		    ;so the bsldr should never exceed the 608k !
TMPOFF  equ 0x0000
BOOTDRIVER equ 0x80  ; first hard disk No.1
 
lba;
size db 16
reserved db 0
count dw 0x0001
buffer dd TMPSEG
sectornuml dd 0
sectornumh dd 0

;__________________
print:			;this deal ax specially !
p_start:
lodsb
or al,al
jz return
call dis_char
jmp p_start
return:
ret

dis_char:
mov ah,0eh
int 10h
ret

;dis_num:  ; for debug to display hex num lsb first
;push eax
;push ebx
;push cx
;mov cx,8
;a:
;mov eax,ebx
;and eax,0xf

;cmp al,9
;ja c
;add al,'0'
;jmp b
;c:
;add al,'a'-10
;b:
;call dis_char
;shr ebx,4;
;loop a
;pop cx
;pop ebx
;pop eax
;ret


load:
load_start:
mov [sectornuml],eax
mov [buffer],ecx
mov ah,42h
mov dl,80h
mov si,lba
int 13h
jc load_start
ret



;++++++++++++++++++++++++++++++
start:
mov ax,BOOTSEG
mov ds,ax
mov es,ax
mov ss,ax
mov bp,0x7c00;BOOTSEG  ;bp and sp both 0x0 that is interesting this will roll back
mov sp,0x7c00

;----------------------------------------load bootsector of C:   ;扩展13中断
mov dword [sectornuml],63 ;C:盘分区引导扇区
mov ah,42h        ; 
mov dl,80h
mov si,lba
int 13h
xor eax,eax		;should be here!

mov ax,[TMPSEG+510]
cmp ax,0xaa55
jz find_bts
;mov si,m_not_find_bootsector
;call print
jmp $                ;Can't get the boot sector,system will now die !
find_bts:
;if fat32
			;not fat32 file-system system now halt !
find_fat32:
;get the fdt
mov ax,[TMPSEG+0x0b]  ;bp-2   bytes per sector 2
push ax
mov al,byte [TMPSEG+0x0d]  ;bp-2-2  sectors per clustet 1
xor ah,ah
push ax

mov ax,[TMPSEG+0x0e]
add ax,63
push eax               ;bp-2-2-4   fatstart 4
mov ecx,eax
mov eax,[TMPSEG+0x24];
shl eax,1
add eax,ecx
push eax              ;bp-2-2-4-4  fatdata 4
mov eax,[TMPSEG+0x2c] ;rootclusternum
sub eax,2
mov bx,[bp-4]
mul bx
add eax,[bp-12]   ; here we are fdt in eax
;search for bsboot in fat32

mov [sectornuml],eax
xor eax,eax
mov ax,[bp-4]
mov [count],ax
mov dword[buffer],TMPSEG+512
mov ah,42h        ; 
mov dl,80h
mov si,lba
int 13h

mov ax,TMPSEG+512
mov dx,[bp-4]
shl dx,9 ;*512
add dx,ax ;*512+0x9000+512 upper limit

find_bsldr: ;search in one cluster !
mov si,bsldr
mov di,ax;0x9000+512+0x80;
mov cx,11
repe cmpsb
jz get_bsldr
add ax,0x20
cmp ax,dx
jbe find_bsldr
mov si,m_bsldrmiss
call print
jmp $		;can't find the bsldr in one cluster

get_bsldr:  ;ax contain the address of dir struct in memory
;push eax
;mov si,m_bsldr  
;call print
;pop eax
mov bx,ax
mov ax,[bx+0x14]
push ax ;高16位
mov ax,[bx+0x1a]
push ax ;低16位
pop eax ;首簇号
xor ebx,ebx
mov bx,[bp-4]
mov [count],bx
mov ecx,0x7e00

load_bsldr:

push ecx
push eax

sub eax,2

xor ebx,ebx
mov bx,[bp-4]
mul ebx
add eax,[bp-12]    ;first cluster in data: eax=(eax-2)*[bp-4]+[bp-12]
;++++++++++++++

call load 
pop eax

shl eax,2


xor edx,edx
mov ebx,512;;;;! fix number
div ebx
push edx
add eax,[bp-8]     ;eax=eax*4/512+[bp-8] ; edx(ebx)=eax*4%512 

mov ecx,TMPSEG
call load

pop ebx
pop ecx

mov eax,[TMPSEG+bx]

cmp eax,0x0fffffff
jz load_ok

xor edx,edx
mov dx,[bp-4]
shl edx,9

add ecx,edx
jmp load_bsldr

load_ok:
;here we go hoho
jmp 0:0x7e00


times 510-($-$$) db 0
dw 0xaa55