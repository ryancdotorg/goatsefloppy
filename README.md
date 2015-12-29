**NOTE: I ( @ryancdotorg ) did not make this.** I happened to go looking for the
source only to find it was no longer on the internet, but I did find a copy on
on of my hard drives. I'm planning to add a script that will generate a .pal
and .raw file from any image, but aside from that it seemed like a shame to
keep the code to myself.

Goatse Rescue Floppy - When Everything Else Fails
-------------------------------------------------

Written by 2004-2005 kometbomb (and some other people, thanks to them) 
Feel free to treat like your own kids. Sicko.

* Usage: Write goatsefloppy.img on a 1.44 MB floppy, reboot with the mentioned 
  floppy, gaze in awe. Or, use the supplied other image files.

* Requirements: x86 (probably needs a 286), MCGA (mode 0x13)

Files included in the archive
-----------------------------

* tools/convpic.c - converts hello.jpg to hello.raw and hello.pal
* floppy.asm - used to build the raw floppy image, includes the bootsector code
* hello.asm - displays the receiver
* ~~hello.jpg - eh~~
* Makefile - fluffy bunnies
* readme.txt - cure for cancer

Rolling your own
----------------

You need at least gcc, nasm, mkisofs, mknbi, libjpeg, make and maybe some makefile 
tweaking. Compiles on Win32 (you don't need cygwin or other crap), a makefile
for Linux is included also. I don't think there's anything that would make 
compiling different between distributions.
    
Win32:  `make -f Makefile.w32`
            
Or, a specific image:
    
`make -f Makefile.w32 cdrom`  
`make -f Makefile.w32 floppy`  
`make -f Makefile.w32 netboot`  
            
Delete all files but the original source code files:
            
`make -f Makefile.w32 clean`
            
Linux:  `make -f Makefile.linux`
    
The meat is in the hello.com file, which is a standard 16-bit DOS executable.
Basically you could use something like a little game in place of it (as long as
it doesn't use anything provided by DOS etc.). The simplest way to customize the
experience is obviously to change hello.jpg.
