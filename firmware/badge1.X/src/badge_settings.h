//badge user settings

#ifndef __BADGE_SETTINGS_H
#define __BADGE_SETTINGS_H

//Badge firmware version should be defined as a string here:
#define FIRMWARE_VERSION "0.77"

//define length of BASIC program RAM buffer
//increasing the length will allow you to hold larger programs
// load and save functions are set to 4096B only though
#define	BPROG_LEN	16384

//define size and number of sectors for saving BASIC programs
//BPROG_LEN = BPROG_SECSIZ*BPROG_SECNUM
#define	BPROG_SECSIZ	4096
#define	BPROG_SECNUM	4

//amount of BASIC program slots
#define		BASIC_SAVNUM	16
//FLASH_CONSUMED = BASIC_SAVNUM*BPROG_LEN - that is how much of FLASH is consumed
//the rest up to 0x080000 is free for user
//if you don't care for BASIC or CP/M, do as you wish

//where is the start of slot region in FLASH
#define		BASIC_BASEADDR	0
/*
 * FLASH organization is as follows
 * 0x000000-0x003FFF - first slot
 * 0x004000-0x007FFF - second slot
 * ...etc
 * 0x03C000-0x03FFFF - 16-th slot
 * 0x040000-0x07FFFF - empty space
 * 0x080000-0x0FFFFF - D disk of CP/M machine
 * 0x100000-0x17FFFF - E disk of CP/M machine
 * 0x180000-0x1FFFFF - F disk of CP/M machine
 */

//Set SHOW_SPLASH to 0 to skip splash screen at boot
#define SHOW_SPLASH	0

//B_BDG010
//this one enables 64k of RAM image for CP/M 
//if off, disables all CP/M stuff and leaves "empty" Z80 machine
#define	USE_RAM_IMAGE	
//first ROM disk, full of utils, mapped as disk B. you can disable this one
#define	USE_ROMDISK
//second ROM disk, with games, mapped as disk C. you can disable this one
#define	USE_ROMDISK2
//RAM disk, you can save 22kb of RAM by disabling it
#define	USE_RAMDISK

//FLASH buffering of CP/M disk drives. 
//enabled - use RAM buffering, faster, less wear-out
//disabled - more stable and straight-forward
#define	FLASH_BUFFERING


#endif