CC=nasmw

floppy.img: floppy.asm hello.com
	$(CC) floppy.asm -o floppy.img

hello.com: hello.asm hello.raw hello.pal
	$(CC) hello.asm -o hello.com
	