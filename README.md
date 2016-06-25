# Digital_Clock_Project
testing 1 second timer

real source code of this project : real_clock_sc.c

setting fuse bit for XTAL 8 mHz : avrdude -c usbasp -p m16 -U lfuse:w:0xFD:m -U hfuse:w:0x89:m
