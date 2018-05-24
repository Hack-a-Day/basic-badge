# Retrocomputing BASIC Badge

2018 Hackaday Belgrade Hardware Badge

* Hardware Lead: Voja Antonic
* Software Lead: Jaromir Sukuba

Hardware details available on [the BASIC Badge project page](https://hackaday.io/project/80627-badge-for-hackaday-conference-2018-in-belgrade).

## BASIC Word List:

### Standard Words:
* `print`
* `println`
* `if`
* `else`
* `goto`
* `gosub`
* `return`
* `for`
* `next`
* `end`
* `let`
* `rem`

### Badge Custom Words

* `led X,Y` - control LED, X[0..2] RGB, Y[0..1] on/off
* `tune A,B,C,D` - plays tones A, B, C for duration D
* `clrscr` - clear screen
* `setxy X,Y` - set cursor at X,Y
* `wait X` - wait for X ms
* `color X,Y` - X=Foreground, Y=Background using EGA 16-color palette
* `chr X` - prints character with ASCII representation X
* `termt X` - sets VT100 terminal on or off (1-on, 0-off)
* `rnd X` - function to return random number 0..X
* `ein X` - function to return value of expansion pin X
* `eout X,Y` - control expansion pin, X[0..3], Y[0..1] on/off
* `edr X,Y` - sets expansion pin function X[0..3], Y[0..1] output/input
* `uin X` - function to return received byte from serial port, if X=0, function is non-blocking, if X=1, function is blocking
* `uout X` - outputs value X to serial port as single byte
* `input "string"` - prints string and returns user entered value
* `peek X` - returns value from memory location X
* `poke X,Y` - write value Y into memory, on location X
* `cursor X` - turns cursor on (X=1) or off (X=0)


### BASIC CLI commands

* `run` - runs program
* `save X` - Save program buffer to flash. There are 16 slots: X[0..15]
* `load X` - Load program into buffer from flash. There are 16 slots: X[0..15]
* `list` - list whole program
* `more` - list program by pages
* `clr` - clears all code from the program buffer
* `free` - prints amount of free program buffer
* `sload` - load new program into buffer from serial port
* `ssave` - output program buffer via serial port
* `help` - prints out basic BASIC help



## Setting Up the Toolchain for BASIC Badge

The BASIC badge is based around a PIC32MX370F512H. Software is compiled using MPLABX, XC32 compiler, and legacy peripheral libraries from Microchip.

1. Download three packages:
    * Navigate to the Downloads" tab at the bottom of the [Microchip Compilers](http://www.microchip.com/mplab/compilers) page.
    * Download `MPLAB® XC32/32++ Compiler v2.05` for your operating system
    * Download `PIC32 Legacy Peripheral Libraries` for your operating system
    * Navigate to the "Downloads" tab part way down the screen at the [Microchip MPLAB X IDE](http://www.microchip.com/mplab/mplab-x-ide) page.

2. Install the packages:
    * Install the XC32 compiler first
    * Install the peripheral libraries next. *IMPORTANT:* You must specify the same directory as the XC32 compiler. This will likely not be the default option during install. For Linux installation this director was: `/opt/microchip/xc32/v2.05/`
    * Install MPLABX IDE

3. Open the MPLABX project from this repository
    * A PIC programmer like the PICKIT3 is required to program your badge
    * On the PIC programmer, pins 1-5 correspond to RES, V+, GND, B0, and B1 on the badge

## Using XMODEM

# Badge side

* Launch "CP/M @ Z80" on the badge
* b:
* xmdm r e:ZORK1.DAT

# Linux XMODEM info

* sudo apt-get install lrzsz
* sudo chmod 777 /dev/ttyUSB0
* sx -X ZORK1.DAT > /dev/ttyUSB0 < /dev/ttyUSB0

## Proposed Functionality

* High Importance:
  * BASIC interpreter
  * Line editor for BASIC programs
  * Save/Load ability (stored in program memory)
  * Simple menu system
* Medium Importance
  * Hardware control on expansion port
  * Game(s) that come with the badge
  * Ability for users to write C files and compile them into new firmward to flash to badge
* Low Importance
  * Serial connection allows users to program BASIC onto badge from computer without full reflash (needs USB to serial adapter)
  * Programming challenge? Like a game that encourages writing BASIC to solve it
  * Bling (demos that run on the badge)
