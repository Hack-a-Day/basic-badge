#include <xc.h>
#include "hw.h"
#include "Z80/hwz.h"
#include <plib.h>
#include <stdint.h>

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

unsigned char key_state=0,key_last,key;
volatile uint32_t ticks;	// millisecond timer incremented in ISR

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


const unsigned int tone_pr_table[64] = 
	{
	0	,
	61224	,
	57788	,
	54544	,
	51483	,
	48593	,
	45866	,
	43292	,
	40862	,
	38568	,
	36404	,
	34360	,
	32432	,
	30612	,
	28893	,
	27272	,
	25741	,
	24296	,
	22933	,
	21645	,
	20430	,
	19284	,
	18201	,
	17180	,
	16215	,
	15305	,
	14446	,
	13635	,
	12870	,
	12148	,
	11466	,
	10822	,
	10215	,
	9641	,
	9100	,
	8589	,
	8107	,
	7652	,
	7223	,
	6817	,
	6435	,
	6073	,
	5732	,
	5411	,
	5107	,
	4820	,
	4550	,
	4294	,
	4053	,
	3826	,
	3611	,
	3408	,
	3217	,
	3036	,
	2866	,
	2705	,
	2553	,
	2410	,
	2274	,
	2147	,
	2026	,
	1912	,
	1805	,
	1704	,

	};

void sound_play_notes (unsigned char note1, unsigned char note2, unsigned char note3, unsigned int wait)
	{
	IEC0bits.T5IE = 0;		//sound is a bit shaky without this
							//quick hack, needs more debugging
	sound_set_note(note1,0);
	sound_set_note(note2,1);
	sound_set_note(note3,2);
	wait_ms(wait);
	IEC0bits.T5IE = 1;
	}

void sound_set_note (unsigned char note, unsigned char generator)
	{
	sound_set_generator(tone_pr_table[note],generator);
	}

void sound_set_generator (unsigned int period, unsigned char generator)
	{
	if (generator==0)
		{
		T2CONbits.TON = 0;
		PR2 = period;
		T2CONbits.TCKPS = 0b011;
		if (period!=0)
			T2CONbits.TON = 1;
		else
			GEN_0_PIN = 0;
		}
	if (generator==1)
		{
		T3CONbits.TON = 0;
		PR3 = period;
		T3CONbits.TCKPS = 0b011;
		if (period!=0)
			T3CONbits.TON = 1;
		else
			GEN_1_PIN = 0;
		}
	if (generator==2)
		{
		T4CONbits.TON = 0;
		PR4 = period;
		T4CONbits.TCKPS = 0b011;
		if (period!=0)
			T4CONbits.TON = 1;
		else
			GEN_2_PIN = 0;
		}
	}


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
	CS_FLASH = 1;
    /*
    MOSI	F3	
    MISO	F5	
    SCK		F6	
			C13	
			C14	
     */
    PPSUnLock;
    PPSOutput(4, RPF3, SDO1);	//MOSI for FLASH
	PPSInput(1, SDI1, RPF5);	//MISO for FLASH
								//SCK is fixed
	PPSInput(1, U3RX, RPC13);	//RX pin
	PPSOutput(1, RPC14, U3TX);	//TX pin
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
    LCD_BKLT = 1;
    LCD_PWR = 0;
	TRISCbits.TRISC15 = 0;

  
	
    PR5 = 10 *(FPB / 64 / 1000);
    T5CONbits.TCKPS = 0b110;
    T5CONbits.TON = 1;
    IEC0bits.T5IE = 1;	
    IPC5bits.T5IP = 3;

    IEC0bits.T2IE = 1;	
    IPC2bits.T2IP = 6;
    IEC0bits.T3IE = 1;	
    IPC3bits.T3IP = 6;
    IEC0bits.T4IE = 1;	
    IPC4bits.T4IP = 6;

    PR1 = (1*(FPB / 64 / 1000)) - 1;
    T1CONbits.TCKPS = 0b10;	//Prescale 64 makes 1ms = 750 ticks at 48 MHz
    T1CONbits.TON = 1;
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 4;

	sound_set_generator(0,0);
	sound_set_generator(0,1);
	sound_set_generator(0,2);
	GEN_ENABLE = 1;
    INTEnableSystemMultiVectoredInt();
	
    TFT_24_7789_Init();
	tft_fill_area(0,0,320,240,0);	//fill with black color
	wait_ms(80);					//wait a moment to avoid flicker
	LCD_BKLT = 0;					//turn backlight on
	fl_rst_pb();	
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
//nope, need to rework
	}


void wait_ms (unsigned int count)
{
	unsigned int ticks_wait;
	ticks_wait = ticks + count;
	while (ticks<= ticks_wait);
}

unsigned char	SPI_dat (unsigned char data)
{
SPI1BUF = data;
while (SPI1STATbits.SPIRBF==0);
return (SPI1BUF);
}
