#Serial bootloader for PIC18F46K22

It is the AN1310 tuned for 64MHz (16MIPS), and set for 19kbs serial coom. It uses the first 
serial port of the microcontroller to communicate with the PC. The required software can be
found here: http://www.microchip.com/wwwAppNotes/AppNotes.aspx?appnote=en546974

It needs a switch on RA4 pin to enter the boot mode, and a LED on RC2 pin to signal
the fact that the bootloader took control. I'll add a schematic soon.
