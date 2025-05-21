This repository contains all documentation and code for DOOMbox. Although note
that 'DOOMbox' isn't necessarily a final name yet.

My final goal for this project is an encased physical console dedicated to
running a purpose-built DOOM clone, with a bunch of extra features just to push
the hardware being used to its limits.

This is an evolution of my previous ambition to write a functional DOOM clone
in pure x86 assembly. The software for DOOMbox will instead be written in a
combination of C and assembly.

It should be noted that Graham Saunderson has done something similar:

    https://kilograham.github.io/rp2040-doom

DOOMbox was not inspired by RP2040 Doom, though his documentation will probably
be very helpful going forward, as I will be using similar hardware. The main
difference is that this project is more intended to just be a fun learning
experience as opposed to trying to make an authentic DOOM port (which is why
I'm not porting at all).

The rest of the contents of this document serves as the project's most current
design specification.


-------------------------------[ PREREQUISITES ]-------------------------------

    ==== UNITS ====

The design specification will follow IEC 80000-13 (B: byte, b: bit):
    * 1 B   = 8 b
    * 1 kiB = 1024 B
    * 1 kB  = 1000 B
    * etc...
In all cases, use of the standard SI units indicates a decimal value, whereas
use of the IEC 80000-13 units indicates a binary value.


-----------------------------[ HARDWARE SUMMARY ]------------------------------

    * Raspberry Pi Pico 2
        This uses the RP2350 chip, but I aim for portability with RP2040 too


-----------------------------[ FILESYSTEM LAYOUT ]-----------------------------

TO-DO


--------------------------------[ MEMORY MAP ]---------------------------------

TO-DO: specify memory map after reserved quantities are designated

Available memory:
    * 520 kiB on-chip SRAM

Reserved memory quantities:
    * 128 kB for VGA output (320x200, 8 bpp):
        64 kB front buffer
        64 kB back buffer

(520 * 1024) - (128 * 1000)
 = 404480 B
is therefore available for code and data.
