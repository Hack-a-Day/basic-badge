# Retrocomputing BASIC Badge

2018 Hackaday Belgrade Hardware Badge

* Hardware Lead: Voja Antonic
* Software Lead: Jaromir Sukuba

Hardware details available on [the BASIC Badge project page](https://hackaday.io/project/80627-badge-for-hackaday-conference-2018-in-belgrade). 

## Setting Up the Toolchain for BASIC Badge

The BASIC badge is based around a PIC32MX370F512H. Software is compiled using MPLABX, XC32 compiler, and legacy peripheral libraries from Microchip.

1. Download three packages:
1. * Navigate to the Downloads" tab at the bottom of the [Microchip Compilers](http://www.microchip.com/mplab/compilers) page.
1. * Download `MPLAB® XC32/32++ Compiler v2.05` for your operating system
1. * Download `PIC32 Legacy Peripheral Libraries` for your operating system
1. * Navigate to the "Downloads" tab part way down the screen at the [Microchip MPLAB X IDE](http://www.microchip.com/mplab/mplab-x-ide) page.

2. Install the packages:
2. * Install the XC32 compiler first
2. * Install the peripheral libraries next. *IMPORTANT:* You must specify the same directory as the XC32 compiler. This will likely not be the default option during install. For Linux installation this director was: `/opt/microchip/xc32/v2.05/`
2. * Install MPLABX IDE

3. Open the MPLABX project from this repository
3. * A PIC programmer like the PICKIT3 is required to program your badge
3. * On the PIC programmer, pins 1-5 correspond to RES, V+, GND, B0, and B1 on the badge


## Proposed Functionality

* High Importance:
* * BASIC interpreter
* * Line editor for BASIC programs
* * Save/Load ability (stored in program memory)
* * Simple menu system
* Medium Importance
* * Hardware control on expansion port
* * Game(s) that come with the badge
* * Ability for users to write C files and compile them into new firmward to flash to badge
* Low Importance
* * Serial connection allows users to program BASIC onto badge from computer without full reflash (needs USB to serial adapter)
* * Programming challenge? Like a game that encourages writing BASIC to solve it
* * Bling (demos that run on the badge)
