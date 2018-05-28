#include <xc.h>
#include "badge_settings.h"
#include "badge.h"
#include "hw.h"
#include "post.h"

int16_t main(void)
	{
	hw_init();
	badge_init();
	if (KEY_BRK==0) post();
	if ((SHOW_SPLASH)&(K_SHIFTR==1)) boot_animation();
	badge_menu();
	while (1); //menu is never left, but anyway
	}

/*
 * Interesting places to take look at, in MPLABX use CtrlCtrl+Shift+F to search in all files
 * for those "bookmarks"
 *
 *  
 * ----------------badge stuff----------------
 * After reset, badge checks whether BRK key is pressed. In case it is, test routine starts, see
 * B_BDG001
 * If not, B_BDG002 is the place to jump. Welcome screen is painstakingly drawn via lot of setxy
 * and print commands. Then it waits for user input. All interaction with user is done mainly via
 * stdio functions, that can be redirected either to local screen or serial port, available on pins
 * C13 and C14 of expansion port. There is also stdio buffer that can be written from software, 
 * for specific purposes
 * The four stdio functions are under B_BDG003
 *
 * For this badge, I opted to refresh display from interrupt handler linked to timer5, set to fire
 * every 12ms - B_BDG003. This is heartbeat of badge, where both partial display refresh (two 
 * lines at once) and keyboard scanning tasks do take place. Notice, one can disable refreshing 
 * the display if needed via function B_BDG004 or manipulating the variable handle_display 
 * directly. Timer 1 runs with 1ms period, and increments variable ticks, available via function
 * millis().
 * 
 * User can write its own programs to be incorporated within "big firmware" of badge, see B_BDG006
 * and B_BDG007 and B_BDG008. Badge can be turned on or off via power button. This erases display.
 * Since badge doesn't have graphic framebuffer (it has terminal text buffer though), the user should 
 * refresh the display in appropriate function, set up via function pointer, see B_BDG005 and B_BDG009
 * Currently, there is about 16kB of RAM and 45kB of FLASH free to use for user
 * If some CP/M features are disabled, see B_BDG010, user can gain additional quarter of megabyte of 
 * FLASH space and about 22kB of RAM.
 * 
 * Other than that, simple firmware skeleton will be provided too, so user doesn't need to use
 * software infrastructure of badge firmware, with all its dependencies; uncovering full hardware 
 * potential of badge.
 * 
 * 
 * ----------------BASIC interpreter----------------
 * 
 * B_BAS001: This is where BASIC tokens are defined, have you want to add your word, add one
 * into this list, then go to B_BAS002 to define appropriate action. Of course, don't forget
 * to add the token number into list in B_BAS003
 * This way you can make words like LED, invoked as LED 2,1
 * If you want to add word returning value like RND in form of LET A = RND 10, take a look how it's
 * made in B_BAS004
 * 
 * A lot of things around BASIC happens outside the interpreter itself. After invoking BASIC, the first
 * function to be called is B_BAS005 where everything is set to run BASIC loop in B_BAS006
 * It basically (no pun intended) collects users input and once buffer si full, it is sent to decide what 
 * it is - B_BAS007
 * If it appears to be valid line of BASIC program, including line number, it goes to be added to
 * current program B_BAS008
 * If not, program flows through series of comparisons to determine whether the thing that user wrote is 
 * something from a few commands that command interpreter can execute. One of them is RUN that sets up 
 * interpreter in B_BAS009 and runs main loop in B_BAS010.
 * Program buffer is set to 16kB, so you can't have programs longer than that. You may redefine buffer 
 * length (see badge_settings.h), you may want to check save/load routines, see B_BAS011
 * 
 * 
 * ----------------CP/M machine----------------
 * 
 * This one runs on Z80 interpreter, written by Udo Munk. It is directory /Z80, along with two other files,
 * defining memory and hardware configuration of this virtual machine, files hwz.c and iosim.c
 * 
 * iosim.c defines the memory map. I wrote the BIOS for CP/M machine to be extremely simple, with more 
 * automation and intelligence on software side, not achievable with classic hardware means. For example,
 * writing to address 0xFF triggers CP/M system reload. so BIOS hook can be as simple as writing single byte 
 * into this address. Classic BIOS needed a lot of more Z80 instructions to achieve th same.
 * On address 0x68 (+0x6D) is implemented simple UART, linked to UART3 of badge (available on C13 and C14 of
 * expansion port). This UART is utilized in XMODEM program (available at drive B:), so user can transfer 
 * CP/M binaries from PC via any compatible Xmodem terminal. I used moserial and cutecom in linux, or teraterm
 * in windows.
 * CP/M machine is setup to use six different drives:
 * A: - 22kB in size, RAM disk. Fast, no wearout, ideal for temporary storage. Trashed on reboot.
 * B: - 128kB ROM disk, contains a lot of goodies. You could pack a lot of software into 128kB then.
 * C: - 128kB ROM dosk, contains Zork and Sargon. You know, games.
 * D: - 512kB FLASH disk, free for your use
 * hwz.c contains all stuff to access disks and devices of CP/M machine, like disk byte read and write
 * functions, called directly by BIOS via addresses 0x07 and 0x08, with 0x05 and 0x06 setting track and
 * sector of drive selected via 0x04.
 *
 * CP/M defines some standard IO devices - reader, puncher for your punch cards and list device to print 
 * out your program listings. Those are not implemented in badge hardware iosim.c, see B_CPM001 and B_CPM002
 * If defined, you can - for example - list files directly from CP/M via virtual printer, or make virtual
 * punch card reader/puncher to EEPROM, thumb drive, PC, FTP server via ESP32 or anything you can connect 
 * to bitbanged interface on badge expansion port.
 * 
 */