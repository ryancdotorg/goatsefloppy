;
; hello.asm - the infamous hello.jpg on your MCGA 320x200x256 screen
;

bits 16
org 0x100
start:
	push cs
	pop ds
	push 0a000h
	pop es
		
	mov ax,13h
	int 10h
	
	mov dx,3c8h
	mov al,0
	out dx,al
	inc dx
	mov cx,256*3
	mov si,imagedata
setpal:	 
	lodsb
	shr al,2	; vga palette is 6-bit
	out dx,al
	loop setpal
	
	xor di,di
	mov cx,(320*200)/2
	rep movsw
	
	jmp short $
	
	
imagedata: ; 3*256 bytes palettte (8 bits) + 320*200 bytes pixel data
	%include "hello.pal"
	incbin "hello.raw",0,320*200 
	
the_end equ ($-start)
size_in_sectors equ ((the_end - the_end % 512)/512+1)
times (size_in_sectors*512)-($-$$) db 0 ; pad the file so it fills the last sector
