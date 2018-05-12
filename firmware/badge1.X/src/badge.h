//Badge firmware version should be defined as a string here:
#define FIRMWARE_VERSION "0.54"

//define length of BASIC program RAM buffer
//increasing the length will allow you to hold larger programs
// load and save functions are set to 4096B only though
#define	BPROG_LEN	4096

//amount of BASIC program slots
#define		BASIC_SAVNUM	16

//where is the start of slot region in FLASH
#define		BASIC_BASEADDR	0
/*
 * FLASH organization is as follows
 * 0x000000-0x000FFF - first slot
 * 0x001000-0x002FFF - second slot
 * ...etc
 * 0x00F000-0x00FFFF - 16-th slot
 * 0x010000-0x07FFFF - empty space
 * 0x080000-0x0FFFFF - D disk of CP/M machine
 * 0x100000-0x17FFFF - E disk of CP/M machine
 * 0x180000-0x1FFFFF - F disk of CP/M machine
 */

//Set SHOW_SPLASH to 0 to skip splash screen at boot
#define SHOW_SPLASH	0

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


