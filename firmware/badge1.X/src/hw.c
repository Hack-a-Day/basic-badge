#include <xc.h>
#include "hw.h"
#include <plib.h>

// DEVCFG3
// USERID = No Setting
#pragma config FSRSSEL = PRIORITY_7     // Shadow Register Set Priority Select (SRS Priority 7)
#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is Disabled)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

unsigned char flash_buff[4096];
unsigned int last_addr;
unsigned char unwritten;

unsigned char key_state=0,key_last,key;
const char keys_normal[50] = 
	{
	'3','4','2','5','1','9','6','7','0','8',
	'e','r','w','t','q','o','y','u','p','i',
	's','d','a','f','/','k','g','h','l','j',
	'x','c','z','v',' ',0x2c,'b','n','.','m',
	K_DN,K_RT,K_LT,';',K_UP,K_DEL,'=',K_ENT,BACKSPACE,'_',
	};

const char keys_shift_l[50] = 
	{
	'#','$','@','%','!','(',' ','&',')','*',
	'E','R','W','T','Q','O','Y','U','P','I',
	'S','D','A','F','?','K','G','H','L','J',
	'X','C','Z','V',' ','<','B','N','>','M',
	K_DN,K_RT,K_LT,':',K_UP,K_DEL,'+',K_ECR,BACKSPACE,'"',
	};
const char keys_shift_r[50] = 
	{
	'#','$','@','%','!','(',' ','&',')','*',
	'E','R','W','T','Q','O','Y','U','P','I',
	'S','D','A','F','?','K','G','H','L','J',
	'X','C','Z','V',' ','<','B','N','>','M',
	K_DN,K_RT,K_LT,':',K_UP,K_DEL,'+',K_ECR,BACKSPACE,'"',
	};

char key_char;




void hw_init (void)
	{
    SYSTEMConfigPerformance(SYS_CLK);
    OSCCONbits.FRCDIV = 0b000;
	ANSELB = 0;
	ANSELC = 0;
	ANSELD = 0;
	ANSELE = 0;
	ANSELF = 0;
	ANSELG = 0;
	TRISD = 0;
	TRISDbits.TRISD9 = 1;
	TRISDbits.TRISD10 = 1;
	LEDR = 0;
	LEDG = 0;
	LEDB = 0;
    TRISE = 0;
    TRISG = 0;
    TRISF = 0;
	TRISFbits.TRISF5 = 1;
    TRISB = 0x0FFF; 
    CNPUB = 0x0FFF;
    CNPUDbits.CNPUD10 = 1;
    CNPUDbits.CNPUD9 = 1;
	TRISCbits.TRISC12 = 0;
	TRISDbits.TRISD8 = 0;
	FLASH_WP = 1;
	FLASH_HOLD = 1;
    /*
    MOSI	F3	
    MISO	F5	
    SCK		F6	
			C13	
			C14	
     */
    PPSUnLock;
    PPSOutput(4, RPF3, SDO1);
	PPSInput(1, SDI1, RPF5);
	PPSInput(1, U3RX, RPC13);
	PPSOutput(1, RPC14, U3TX);	
    PPSLock;

	U3MODEbits.ON = 1;
//	U1MODEbits.STSEL = 1;
    U3STAbits.URXEN = 1;
    U3STAbits.UTXEN = 1;
    U3BRG = ((PB_CLK)/(16*19200)) - 1;
//    INTEnable(INT_SOURCE_UART_RX(UART3), INT_ENABLED);
//    IPC7bits.U3IP = 6;
    U3STAbits.OERR=0;

	SPI1CONbits.MSTEN = 1;
    SPI1CONbits.CKP = 0;
    SPI1CONbits.SMP = 0;
    SPI1CONbits.CKE = 1;
    SPI1BRG = 1;
    SPI1CONbits.ON = 1;
    
//    TRISBbits.TRISB13 = 0;
    LATFbits.LATF0 = 0;
    LATCbits.LATC15 = 0;
	TRISCbits.TRISC15 = 0;
    TFT_24_7789_Init();
    tft_set_write_area(0,0,320,240);
  
	
    PR2 = 10 *(FPB / 64 / 1000);
    T2CONbits.TCKPS = 0b110;
    T2CONbits.TON = 1;
    IEC0bits.T2IE = 1;	
    IPC2bits.T2IP = 3;
    
	
    INTEnableSystemMultiVectoredInt();
	
	
	}


unsigned char keyb_tasks (void)
	{
	static char shift=0;
	unsigned char retval = 0;
	K_R1 = 1;
	K_R2 = 1;
	K_R3 = 1;
	K_R4 = 1;
	K_R5 = 1;
	if (key_state==0) key = 255;
	
	if (key_state==0) K_R1 = 0;
	if (key_state==1) K_R2 = 0;
	if (key_state==2) K_R3 = 0;
	if (key_state==3) K_R4 = 0;
	if (key_state==4) K_R5 = 0;

	if (key_state==5)
		{
		if ((key<255)&(key_last!=key))
			{
			if (K_SHIFTL==0) key_char = keys_shift_l[key];
			else if (K_SHIFTR==0) key_char = keys_shift_r[key];
			else key_char = keys_normal[key];
			retval = key_char;
			}
		key_last = key;
		key_state = 0;
		}
	else
		{
		if (K_C1==0)	key = 0 + (key_state*10);
		if (K_C2==0)	key = 1 + (key_state*10);
		if (K_C3==0)	key = 2 + (key_state*10);
		if (K_C4==0)	key = 3 + (key_state*10);
		if (K_C5==0)	key = 4 + (key_state*10);
		if (K_C6==0)	key = 5 + (key_state*10);
		if (K_C7==0)	key = 6 + (key_state*10);
		if (K_C8==0)	key = 7 + (key_state*10);
		if (K_C9==0)	key = 8 + (key_state*10);
		if (K_C10==0)	key = 9 + (key_state*10);
		key_state++;
		}
	return retval;
	}


void delay_us (unsigned long howmuch)
	{
	T1CONbits.TON = 1;
	T1CONbits.TCKPS = 0b01;
	PR1 = howmuch * PB_CLK/(1000000*8);
	TMR1=0;
	while (IFS0bits.T1IF==0);
	IFS0bits.T1IF=0;
	}


void wait_1ms (void)
{
T1CONbits.TON = 1;
T1CONbits.TCKPS = 0b01;
PR1 = PB_CLK/(1000*8);
TMR1=0;
while (IFS0bits.T1IF==0);
IFS0bits.T1IF=0;
}


void wait_ms (unsigned int count)
{
unsigned int i;
for (i=0;i<count;i++) wait_1ms();
}

unsigned char	SPI_dat (unsigned char data)
{
SPI1BUF = data;
while (SPI1STATbits.SPIRBF==0);
return (SPI1BUF);
}

unsigned char fl_rdsr(void)
{
volatile unsigned char temp;
CS_FLASH = 0;
SPI_dat(0x05);
temp = SPI_dat(0xFF);
CS_FLASH = 1;
return temp;
}


unsigned char fl_rdid(void)
{
volatile unsigned char temp;
CS_FLASH = 0;
SPI_dat(0x9F);
temp = SPI_dat(0xFF);
temp = SPI_dat(0x55);
temp = SPI_dat(0xAA);
CS_FLASH = 1;
}

void fl_read_4k(unsigned long addr, unsigned char * data)
{
unsigned int i;
CS_FLASH = 0;
SPI_dat(0x03);
SPI_dat((addr>>16)&0xFF);
SPI_dat((addr>>8)&0xFF);
SPI_dat((addr>>0)&0xFF);
for (i=0;i<4096;i++) *data++ = SPI_dat(0xFF);
CS_FLASH = 1;
}

void fl_read_nk(unsigned long addr, unsigned char * data, unsigned int n)
{
unsigned int i;
CS_FLASH = 0;
SPI_dat(0x03);
SPI_dat((addr>>16)&0xFF);
SPI_dat((addr>>8)&0xFF);
SPI_dat((addr>>0)&0xFF);
for (i=0;i<n;i++) *data++ = SPI_dat(0xFF);
CS_FLASH = 1;
}


void fl_erase_4k(unsigned long addr)
{
unsigned int i;
fl_wren();
CS_FLASH = 0;
SPI_dat(0x20);
SPI_dat((addr>>16)&0xFF);
SPI_dat((addr>>8)&0xFF);
SPI_dat((addr>>0)&0xFF);
CS_FLASH = 1;
while ((fl_rdsr())&0x01);
}


void fl_write(unsigned long addr,unsigned char data)
{
unsigned int i;
fl_wren();
CS_FLASH = 0;
SPI_dat(0x02);
SPI_dat((addr>>16)&0xFF);
SPI_dat((addr>>8)&0xFF);
SPI_dat((addr>>0)&0xFF);
SPI_dat(data);
CS_FLASH = 1;
}

void fl_rst_pb(void)
{
CS_FLASH = 0;
SPI_dat(0x50);
CS_FLASH = 1;
CS_FLASH = 0;
SPI_dat(0x01);
SPI_dat(0x00);
CS_FLASH = 1;
}

void fl_wren(void)
{
CS_FLASH = 0;
SPI_dat(0x06);
CS_FLASH = 1;
}


void fl_write_4k(unsigned long addr, unsigned char * data)
{
unsigned int i;
for (i=0;i<4096;i++) 
	{
	fl_write(addr+i,*data++);
	while ((fl_rdsr())&0x01);
	}
}

void fl_write_128(unsigned int sector,unsigned char * data)
{
unsigned long addr;
unsigned char i;
addr = ((unsigned long)(sector))*128UL;
addr = addr&0xFFFFF000;
#ifdef	FLASH_BUFFERING	
if (last_addr!=addr)
	{
	if (last_addr!=0xFFFF)
		{
		fl_erase_4k(last_addr);
		fl_write_4k(last_addr,flash_buff);
		}
	fl_read_4k(addr,flash_buff);
	last_addr = addr;
	}
unwritten = 1;
#endif
#ifndef	FLASH_BUFFERING	
fl_read_4k(addr,flash_buff);
#endif
addr = ((unsigned long)(sector))*128UL;
addr = addr&0x00000FFF;
for (i=0;i<128;i++) flash_buff[addr+i] = data[i];
addr = ((unsigned long)(sector))*128UL;
addr = addr&0xFFFFF000;
#ifndef	FLASH_BUFFERING	
fl_erase_4k(addr);
fl_write_4k(addr,flash_buff);
#endif
}

void fl_read_128(unsigned int sector,unsigned char * data)
{
unsigned long addr;
#ifdef	FLASH_BUFFERING	
if (unwritten == 1)
	{
	fl_erase_4k(last_addr);
	fl_write_4k(last_addr,flash_buff);
	unwritten = 0;	
	last_addr = 0xFFFF;
	}
#endif
addr = ((unsigned long)(sector))*128UL;
fl_read_nk(addr,data,128);
}

/*
void init_first_x_sects (unsigned char i)			//format directory area
{
unsigned int j;
for (j=0;j<128;j++) disk_temp[j]=0xE5;
for (j=0;j<32;j++) 
	{
	fl_write_128(j,disk_temp);
	}

for (j=0;j<32;j++) 
	{
	fl_write_128(j+(1*4096),disk_temp);
	}
for (j=0;j<32;j++) 
	{
	fl_write_128(j+(2*4096),disk_temp);
	}
for (j=0;j<32;j++) 
	{
	fl_write_128(j+(3*4096),disk_temp);
	}

}

*/