;
; hello.asm - the infamous hello.jpg on your MCGA 320x200x256 screen
;

bits 16
org 0x100
start:
	; copy code segment address to data segment address
	push cs
	pop ds

	; load start of video memory into es
	push 0a000h
	pop es
		
	; set video mode to 13h
	mov ax,13h
	int 10h

	; tell video card we're going to write to the palette at color 0
	mov dx,3c8h
	mov al,0
	out dx,al

	; update port address to palette data
	inc dx

	; write pallet data to port
	mov cx,256*3
	mov si,imagedata
	rep outsb

	; write image data to video memory
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
