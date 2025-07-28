This repository contains all documentation/schematics and code for DOOMbox.

My final goal for this project is an encased physical console dedicated to
running a purpose-built DOOM clone, maybe with a bunch of extra features thrown
in for fun.

This is an evolution of my previous ambition to write a functional DOOM clone
in pure x86 assembly. The software for DOOMbox will instead be written in a
combination of C and assembly. The old project is below:

    https://gitlab.com/kosude/doom

It should be noted that Graham Saunderson has done something similar:

    https://kilograham.github.io/rp2040-doom

To be clear, DOOMbox was not inspired by RP2040 Doom, though his documentation
will probably be very helpful going forward, as I will be using similar
hardware. The main difference is that this project is more intended to just be
a fun learning experience as opposed to trying to make an authentic DOOM port
(which is why I'm not porting at all).

The rest of this document serves as the project's most current design
specification. KiCad files are also provided in the kicad/ directory.


-------------------------------[ PREREQUISITES ]-------------------------------

    ==== UNITS ====

The design specification will follow IEC 80000-13 (B: byte, b: bit):
    * 1 B   = 8 b
    * 1 kiB = 1024 B
    * 1 kB  = 1000 B
    * etc...
In all cases, use of the standard SI units indicates a decimal value, whereas
use of the IEC 80000-13 units indicates a binary value.


-----------------------------[ FILESYSTEM LAYOUT ]-----------------------------

Available capacity:
    * 4 MiB on-board flash


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


----------------------------[ POWER SPECIFICATION ]----------------------------

For the time being, a simple LDO regulator is being used to step down input
voltage to 5V.


---------------------------[ GRAPHICS SPECIFICATION ]--------------------------

Doombox provides graphical output via VGA. Colour data is output serially from
the Pico (SPI), through an MCP23S08 for parallel expansion, to the VGA
connector.

Colour data is output in the form RGB-332, i.e. 3 bits for the red and green
channels, and 2 for the blue.

On the Pico, GPIOs 2-4 and 6-7 are reserved for graphical output:
    2: SCL0     SPI clock
    3: SDO0     SPI colour data output stream
    4: SCS_VGA  chip select for the related MCP23S08
    6: HSYNC
    7: VSYNC

When writing colour data serially, the bit-order is as follows:
    R R R G G G B B
  first -------> last
where the leftmost bit of each channel is the MSB as usual.


----------------------------[ AUDIO SPECIFICATION ]----------------------------

Stereo audio is provided via pulse width modulation (PWM). GPIOs 8-9 are
reserved for this purpose, where GPIO8 outputs left audio, and GPIO9 right.

    TODO better description


----------------------------[ INPUT SPECIFICATION ]----------------------------

    TODO

Required controls:
    * Move forward + move backwards | Single joystick: vertical: moving
    * Turn left + turn right        |                  horizontal: turning
    * Strafe                          Modifier btn: strafe with hoz. joystick
    * Run / speed modifier            Modifier button
    * Fire                            Button
    * Interact                        Button
    * Menu                            Button
    * Weapon selection                7 buttons

Automap necessary?
