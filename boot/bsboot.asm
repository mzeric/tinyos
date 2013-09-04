[bits 16]
[org 0x7c00]
;为了提供灵活的加载方式，采用间接读取FAT32参数
;本文件由BIOS加载至0x7C00处执行，
;从C盘的FAT32中读取BSLDR将其加载到0x7e00处，跳转执行...
; this will find the BSLDR in only one cluster dir struct 
; then load it to 0x7e00 untill the end of the file , so thie file shoule not be too large
; only support FAT32
jmp start
times 0x03-($-$$) db 0 ;bpb & FAT32 or FAT16 struct data :first offset-> 0x03H

times 0x60-($-$$) db 0 ;end here

m_bsldrmiss db 'BSLDR missing.'
    db 13,10,0
bsldr db 'BSLDR      '

BOOTSEG   EQU 0X0000
BOOTOFF   EQU 0X7C00
TMPSEG  EQU 0X1000  ;AVALID FROM 0X00500->0X07BFF SUB THE [BP-4]*0X200 :ABOUT 29K
                    ;THE BSLDR WILL LOAD TO FROM 0X7E00 UNTILL 0X9FFFF :ABOUT 608K
		    ;SO THE BSLDR SHOULD NEVER EXCEED THE 608K !
TMPOFF  EQU 0X0000
BOOTDRIVER EQU 0X80  ; FIRST HARD DISK NO.1
 
LBA;
SIZE DB 16
RESERVED DB 0
COUNT DW 0X0001
BUFFER DD TMPSEG
SECTORNUML DD 0
SECTORNUMH DD 0

;__________________
PRINT:			;THIS DEAL ax SPECIALLY !
P_START:
LODSB
OR AL,AL
JZ RETURN
CALL DIS_CHAR
JMP P_START
RETURN:
RET

DIS_CHAR:
MOV AH,0EH
INT 10H
RET

LOAD:
LOAD_START:
MOV [SECTORNUML],eax
MOV [BUFFER],Ecx
MOV AH,42H
MOV DL,80H
MOV SI,LBA
INT 13H
JC LOAD_START
RET



;++++++++++++++++++++++++++++++
start:
mov ax,BOOTSEG
mov ds,ax
mov es,ax
mov ss,ax
mov bp,0x7c00;bootseg  ;bp and sp both 0x0 that is interesting this will roll back
mov sp,0x7c00

;----------------------------------------load bootsector of c:   ;????13????
mov dword [SECTORNUML],63 ;c:??????????????
mov ah,42h        ; 
mov dl,80h
mov si,LBA
int 13h
xor eax,eax		;should be here!

mov ax,[TMPSEG+510]
cmp ax,0xaa55
jz find_bts
;mov si,m_not_find_bootsector
;call print
jmp $                ;can't get the boot sector,system will now die !
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

mov ax,[TMPSEG+0x0e]  ;
add ax,63
push eax               ;bp-2-2-4   fatstart 4
mov ecx,eax
mov eax,[TMPSEG+0x24];
shl eax,1
add eax,ecx
push eax              ;bp-2-2-4-4  fatdata 4
mov eax,[TMPSEG+0x2c] ;rootclusternum

search_rootdir:
push eax


call GET_SECT_FROM_CLUST
;search for bsldr in fat32

mov dword[SECTORNUML],eax
xor eax,eax
mov al,[bp-4]
mov [COUNT],ax
mov dword[BUFFER],TMPSEG+512
mov ah,42h        ; 
mov dl,80h
mov si,LBA
int 13h

mov ax,TMPSEG+512
mov dl,[bp-4]
shl dx,9 ;*512
add dx,ax ;*512+0x9000+512 upper limit

find_bsldr: ;在本簇中搜索
mov si,bsldr
mov di,ax;0x9000+512+0x80;
mov cx,11
repe cmpsb
jz get_bsldr
add ax,0x20
cmp ax,dx
jbe find_bsldr
;准备在下一簇中搜索
pop eax
call GET_NEXT_CLUST
cmp eax ,0xffffffff;
jnz search_rootdir;历遍完所有的根目录!
mov si,m_bsldrmiss
call PRINT
jmp $		;can't find the bsldr 

get_bsldr:  ;ax contain the address of dir struct in memory

mov bx,ax
mov ax,[bx+0x14]
push ax ;
mov ax,[bx+0x1a]
push ax ;
pop eax ;
xor ebx,ebx
mov bx,[bp-4]
mov [COUNT],bx
mov ecx,0x7e00

load_bsldr:
push ecx
push eax

call GET_SECT_FROM_CLUST

call LOAD 
pop eax  ;

call GET_NEXT_CLUST;
pop ecx  ;get_next_clust破坏了ecx

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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;子例程
GET_NEXT_CLUST:
push ecx

shl eax,2

xor edx,edx
xor ebx,ebx
mov bx,[bp-2];512;;;;! fix number
div ebx
push edx
add eax,[bp-8]     ;eax=eax*4/512+[bp-8] ; edx(ebx)=eax*4%512 

mov ecx,TMPSEG
call LOAD

pop ebx           ;ebx=edx=簇号偏移
mov eax,[TMPSEG+bx]
pop ecx
ret 
;
;give me the clustnum in eax ,i'll offer the datasec_begin back in eax
GET_SECT_FROM_CLUST:

sub eax,2
xor ebx,ebx
mov bl,[bp-4]
mul ebx
add eax,[bp-12]    ;first cluster in data: eax=(eax-2)*[bp-4]+[bp-12]
ret

times 510-($-$$) db 0
dw 0xaa55