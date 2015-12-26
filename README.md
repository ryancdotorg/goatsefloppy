**NOTE: I ( @ryancdotorg ) did not make this.** I happened to go looking for the
source only to find it was no longer on the internet, but I did find a copy on
on of my hard drives. I'm planning to add a script that will generate a .pal
and .raw file from any image, but aside from that it seemed like a shame to
keep the code to myself.

Goatse Rescue Floppy - When Everything Else Fails
-------------------------------------------------

Written by 2004 kometbomb - Feel free to treat like your own kids. Sicko.

* Usage: Write floppy.img on a 1.44 MB floppy, reboot with the mentioned
  floppy, gaze in awe.

* Requirements: x86 (probably needs a 286), MCGA (mode 0x13)

Files included in the archive
-----------------------------

* floppy.asm - used to build the raw floppy image, includes the bootsector code
* hello.asm - displays the receiver
* hello.pal - the palette for hello.raw
* hello.raw - raw 320x200x256 version of hello.jpg
* Makefile - fluffy bunnies
* readme.txt - cure for cancer
