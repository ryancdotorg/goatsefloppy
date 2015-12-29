;
; floppy.asm - builds the floppy image, includes the required bootsector code
;

bits 16


org 0

KERNEL_START equ 1		

KERNEL_SEGMENT equ 0x1000	
KERNEL_OFFSET equ 0x100

	cli             
        mov ax,0x9000   
        mov ss,ax
        mov sp,0xffff   
        sti             
	push word KERNEL_SEGMENT
	pop es
	
	mov bx, KERNEL_OFFSET
	mov cx, KERNEL_SIZE
 	mov ax, 1
	mov dl, 0

 more:
	push ax
	push cx
	mov dx,0
	mov cx, 18
	div cx

	mov ch,al
	shr ch,1
	
	mov dh,al
	and dh,1

	mov cl,dl
	inc cl

	mov ax,2
	
	mov dl,0

again:	
	mov ax,0x201
	int 13h
	jnc ok
	jmp again
ok:	
		
	pop cx
	pop ax

	add bx,512
 	inc ax
 	loop more

	jmp KERNEL_SEGMENT:KERNEL_OFFSET

times 510 - ($ - $$) db 0
db 55h
db 0aah

IMAGE_START equ ($-$$)
incbin "hello.com"
IMAGE_SIZE equ ($-$$)-IMAGE_START
KERNEL_SIZE equ ((IMAGE_SIZE/512))		; Kernel size in disk blocks
times (2880*512 - ($ - $$)) db 0
