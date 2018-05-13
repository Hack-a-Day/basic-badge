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

uint8_t key_state=0,key_last,key;

uint16_t rnd_var1,rnd_var2,rnd_var3;

const int8_t keys_normal[50] = 
	{
	'3','4','2','5','1','9','6','7','0','8',
	'e','r','w','t','q','o','y','u','p','i',
	's','d','a','f','/','k','g','h','l','j',
	'x','c','z','v',' ',0x2c,'b','n','.','m',
	K_DN,K_RT,K_LT,';',K_UP,K_DEL,'=',K_ENT,BACKSPACE,'-',
	};

const int8_t keys_shift_l[50] = 
	{
	'#','$','@','%','!','(',' ','&',')','*',
	'E','R','W','T','Q','O','Y','U','P','I',
	'S','D','A','F','?','K','G','H','L','J',
	'X','C','Z','V',' ','<','B','N','>','M',
	K_DN,K_RT,K_LT,':',K_UP,K_DEL,'+',K_ECR,BACKSPACE,'_',
	};
const int8_t keys_shift_r[50] = 
	{
	'#','$','@','%','!','(',' ','&',')','*',
	'E','R','W','T','Q','O','Y','U','P','I',
	'S','D','A','F','?','K','G','H','L','J',
	'X','C','Z','V',' ','<','B','N','>','M',
	K_DN,K_RT,K_LT,':',K_UP,K_DEL,'+',K_ECR,BACKSPACE,'"',
	};

int8_t key_char;


const uint16_t tone_pr_table[128] = 
	{
	0, //No Note
	/* Sacrifice this for 0 to be 'no note' 366927, // 0 | C-1 | 8.176Hz */
	/* Lowest notes have timer values too big for uint16_t so setting to 0 as workaround*/
	0, //346340, // 1 | C?/D?-1 | 8.662Hz
	0, //326904, // 2 | D-1 | 9.177Hz
	0, //308546, // 3 | E?/D?-1 | 9.723Hz
	0, //291233, // 4 | E-1 | 10.301Hz
	0, //274876, // 5 | F-1 | 10.914Hz
	0, //259448, // 6 | F?/G?-1 | 11.563Hz
	0, //244897, // 7 | G-1 | 12.25Hz
	0, //231142, // 8 | A?/G?-1 | 12.979Hz
	0, //218181, // 9 | A-1 | 13.75Hz
	0, //205930, // 10 | B?/A?-1 | 14.568Hz
	0, //194376, // 11 | B-1 | 15.434Hz
	0, //183463, // 12 | C0 | 16.352Hz
	0, //173170, // 13 | C?/D?0 | 17.324Hz
	0, //163452, // 14 | D0 | 18.354Hz
	0, //154281, // 15 | E?/D?0 | 19.445Hz
	0, //145616, // 16 | E0 | 20.602Hz
	0, //137444, // 17 | F0 | 21.827Hz
	0, //129729, // 18 | F?/G?0 | 23.125Hz
	0, //122448, // 19 | G0 | 24.5Hz
	0, //115575, // 20 | A?/G?0 | 25.957Hz
	0, //109090, // 21 | A0 | 27.5Hz
	0, //102968, // 22 | B?/A?0 | 29.135Hz
	0, //97188, // 23 | B0 | 30.868Hz
	0, //91734, // 24 | C1 | 32.703Hz
	0, //86585, // 25 | C?/D?1 | 34.648Hz
	0, //81726, // 26 | D1 | 36.708Hz
	0, //77138, // 27 | E?/D?1 | 38.891Hz
	0, //72810, // 28 | E1 | 41.203Hz
	0, //68722, // 29 | F1 | 43.654Hz
	64866, // 30 | F?/G?1 | 46.249Hz
	61225, // 31 | G1 | 48.999Hz
	57788, // 32 | A?/G?1 | 51.913Hz
	54545, // 33 | A1 | 55.0Hz
	51484, // 34 | B?/A?1 | 58.27Hz
	48594, // 35 | B1 | 61.735Hz
	45867, // 36 | C2 | 65.406Hz
	43292, // 37 | C?/D?2 | 69.296Hz
	40863, // 38 | D2 | 73.416Hz
	38569, // 39 | E?/D?2 | 77.782Hz
	36404, // 40 | E2 | 82.407Hz
	34361, // 41 | F2 | 87.307Hz
	32432, // 42 | F?/G?2 | 92.499Hz
	30612, // 43 | G2 | 97.999Hz
	28893, // 44 | A?/G?2 | 103.83Hz
	27272, // 45 | A2 | 110.0Hz
	25742, // 46 | B?/A?2 | 116.54Hz
	24297, // 47 | B2 | 123.47Hz
	22934, // 48 | C3 | 130.81Hz
	21646, // 49 | C?/D?3 | 138.59Hz
	20431, // 50 | D3 | 146.83Hz
	19285, // 51 | E?/D?3 | 155.56Hz
	18202, // 52 | E3 | 164.81Hz
	17181, // 53 | F3 | 174.61Hz
	16216, // 54 | F?/G?3 | 185.0Hz
	15306, // 55 | G3 | 196.0Hz
	14447, // 56 | A?/G?3 | 207.65Hz
	13636, // 57 | A3 | 220.0Hz
	12871, // 58 | B?/A?3 | 233.08Hz
	12148, // 59 | B3 | 246.94Hz
	11466, // 60 | C4 | 261.63Hz
	10823, // 61 | C?/D?4 | 277.18Hz
	10215, // 62 | D4 | 293.66Hz
	9642, // 63 | E?/D?4 | 311.13Hz
	9101, // 64 | E4 | 329.63Hz
	8590, // 65 | F4 | 349.23Hz
	8108, // 66 | F?/G?4 | 369.99Hz
	7653, // 67 | G4 | 392.0Hz
	7223, // 68 | A?/G?4 | 415.3Hz
	6818, // 69 | A4 | 440.0Hz
	6435, // 70 | B?/A?4 | 466.16Hz
	6074, // 71 | B4 | 493.88Hz
	5733, // 72 | C5 | 523.25Hz
	5411, // 73 | C?/D?5 | 554.37Hz
	5107, // 74 | D5 | 587.33Hz
	4821, // 75 | E?/D?5 | 622.25Hz
	4550, // 76 | E5 | 659.26Hz
	4295, // 77 | F5 | 698.46Hz
	4054, // 78 | F?/G?5 | 739.99Hz
	3826, // 79 | G5 | 783.99Hz
	3611, // 80 | A?/G?5 | 830.61Hz
	3409, // 81 | A5 | 880.0Hz
	3217, // 82 | B?/A?5 | 932.33Hz
	3037, // 83 | B5 | 987.77Hz
	2866, // 84 | C6 | 1046.5Hz
	2705, // 85 | C?/D?6 | 1108.7Hz
	2553, // 86 | D6 | 1174.7Hz
	2410, // 87 | E?/D?6 | 1244.5Hz
	2275, // 88 | E6 | 1318.5Hz
	2147, // 89 | F6 | 1396.9Hz
	2027, // 90 | F?/G?6 | 1480.0Hz
	1913, // 91 | G6 | 1568.0Hz
	1805, // 92 | A?/G?6 | 1661.2Hz
	1704, // 93 | A6 | 1760.0Hz
	1608, // 94 | B?/A?6 | 1864.7Hz
	1518, // 95 | B6 | 1975.5Hz
	1433, // 96 | C7 | 2093.0Hz
	1352, // 97 | C?/D?7 | 2217.5Hz
	1276, // 98 | D7 | 2349.3Hz
	1205, // 99 | E?/D?7 | 2489.0Hz
	1137, // 100 | E7 | 2637.0Hz
	1073, // 101 | F7 | 2793.8Hz
	1013, // 102 | F?/G?7 | 2960.0Hz
	956, // 103 | G7 | 3136.0Hz
	902, // 104 | A?/G?7 | 3322.4Hz
	852, // 105 | A7 | 3520.0Hz
	804, // 106 | B?/A?7 | 3729.3Hz
	759, // 107 | B7 | 3951.1Hz
	716, // 108 | C8 | 4186.0Hz
	676, // 109 | C?/D?8 | 4434.9Hz
	638, // 110 | D8 | 4698.6Hz
	602, // 111 | E?/D?8 | 4978.0Hz
	568, // 112 | E8 | 5274.0Hz
	536, // 113 | F8 | 5587.7Hz
	506, // 114 | F?/G?8 | 5919.9Hz
	478, // 115 | G8 | 6271.9Hz
	451, // 116 | A?/G?8 | 6644.9Hz
	426, // 117 | A8 | 7040.0Hz
	402, // 118 | B?/A?8 | 7458.6Hz
	379, // 119 | B8 | 7902.1Hz
	358, // 120 | C9 | 8372.0Hz
	338, // 121 | C?/D?9 | 8869.8Hz
	319, // 122 | D9 | 9397.3Hz
	301, // 123 | E?/D?9 | 9956.1Hz
	284, // 124 | E9 | 10548.1Hz
	268, // 125 | F9 | 11175.3Hz
	253, // 126 | F?/G?9 | 11839.8Hz
	239, // 127 | G9 | 12543.9Hz
	};

void set_led (uint8_t led_n, uint8_t led_v)
	{
	if (led_n==0)
		LED_R = led_v;
	if (led_n==1)
		LED_G = led_v;	
	if (led_n==2)
		LED_B = led_v;	
	}

void sound_play_notes (uint8_t note1, uint8_t note2, uint8_t note3, uint16_t wait)
	{
	IEC0bits.T5IE = 0;		//sound is a bit shaky without this
							//quick hack, needs more debugging
	sound_set_note(note1,0);
	sound_set_note(note2,1);
	sound_set_note(note3,2);
	wait_ms(wait);
	sound_set_note(0,0);
	sound_set_note(0,1);
	sound_set_note(0,2);
	IEC0bits.T5IE = 1;
	}

void sound_set_note (uint8_t note, uint8_t generator)
	{
	sound_set_generator(tone_pr_table[note],generator);
	}

void sound_set_generator (uint16_t period, uint8_t generator)
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

void hw_sleep (void)
	{
	T1CONbits.TON = 0;
	T2CONbits.TON = 0;
	T3CONbits.TON = 0;
	T4CONbits.TON = 0;
	T5CONbits.TON = 0;
	SPI1CONbits.ON = 0;
	U3MODEbits.ON = 0;
	LCD_PWR = 1;
	LCD_BKLT = 1;
	LCD_PORT = 0;
	LCD_DC = 0;
	LCD_RD = 0;
	LCD_RES = 0;
	LCD_WR = 0;
	GEN_0_PIN = 0;
	GEN_1_PIN = 0;
	GEN_2_PIN = 0;
	TRISBbits.TRISB0 = 0;
	TRISBbits.TRISB1 = 0;
	TRISG = 0;
	TRISGbits.TRISG9 = 1;
	TRISGbits.TRISG6 = 1;
	TRISC = 0;
	LATC = 0;
	TRISD = 0;
	LATD = 0;
	TRISDbits.TRISD9 = 1;
	TRISDbits.TRISD10 = 1;
    CNPUDbits.CNPUD10 = 1;
    CNPUDbits.CNPUD9 = 1;
	TRISE = 0;
	LATE = 0;	
	TRISF = 0;
	K_R1 = 1;
	K_R2 = 1;
	K_R3 = 1;
	K_R4 = 1;
	K_R5 = 1;
	PMD1 = 0xFFFFFFFF;
	PMD2 = 0xFFFFFFFF;
	PMD3 = 0xFFFFFFFF;
	PMD4 = 0xFFFFFFFF;
	PMD5 = 0xFFFFFFFF;
	PMD6 = 0xFFFFFFFF;
	SYSKEY = 0x0;            // Write invalid key to force lock
	SYSKEY = 0xAA996655;     // Write Key1 to SYSKEY
	SYSKEY = 0x556699AA;     // Write Key2 to SYSKEY
	OSCCONSET = 0x10; // set Power-Saving mode to Slee
	SYSKEY = 0x0; // Write invalid key to force lock

	IFS0bits.INT2IF = 0;
	IEC0bits.INT2IE = 1;
	IPC2bits.INT2IP = 4;
	asm volatile("wait"); 
	IEC0bits.INT2IE = 0;
	PMD1 = 0;
	PMD2 = 0;
	PMD3 = 0;
	PMD4 = 0;
	PMD5 = 0;
	PMD6 = 0;
	hw_init();
	start_after_wake();
	}

void wake_return(void)
	{
	//By default, this will be called after waking from sleep. It should do
	//noting. This is a placeholder for user programs to set the function pointer.
	return;
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
	TRISGbits.TRISG6 = 1;
	TRISGbits.TRISG9 = 1;
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
	PPSInput(3, INT2, RPG6);	//power on/off, tie to external interrupt2
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
	
    PR5 = 12 *(FPB / 64 / 1000);
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
	
	wait_ms(50);
    TFT_24_7789_Init();
	tft_fill_area(0,0,320,240,0);	//fill with black color
	wait_ms(80);					//wait a moment to avoid flicker
	LCD_BKLT = 0;					//turn backlight on
	fl_rst_pb();

  	}


uint8_t keyb_tasks (void)
	{
	static int8_t shift=0;
	uint8_t retval = 0;
	rnd_var3 = rnd_var3 + 12345;
	rnd_var2 = rnd_var2 + millis();
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
		rnd_var1 = rnd_var1  + key;
		key_state++;
		}
	return retval;
	}



void wait_ms (uint32_t count)
{
	uint32_t ticks_wait;
	ticks_wait = millis() + count;
	rnd_var2 = rnd_var2  + ticks_wait;
	while (millis()<= ticks_wait);
}

unsigned char	SPI_dat (uint8_t data)
{
SPI1BUF = data;
while (SPI1STATbits.SPIRBF==0);
return (SPI1BUF);
}


uint16_t get_rnd (void)
	{
	uint32_t  var;
	static uint32_t  var_prev;
	var = rnd_var1 + rnd_var2 + rnd_var3 + (var_prev*1103515245) + 12345;
	var = var & 0xFFFF;
	var_prev = var;
	return var;
	}

void __ISR(_TIMER_2_VECTOR, IPL6AUTO) Timer2Handler(void)
//void __ISR(_TIMER_2_VECTOR, ipl6) Timer2Handler(void)
	{
    IFS0bits.T2IF = 0;
	GEN_0_PIN = ~ GEN_0_PIN;
	rnd_var3++;
	}
void __ISR(_TIMER_3_VECTOR, IPL6AUTO) Timer3Handler(void)
//void __ISR(_TIMER_3_VECTOR, ipl6) Timer3Handler(void)
	{
    IFS0bits.T3IF = 0;
	GEN_1_PIN = ~ GEN_1_PIN;
	rnd_var3++;
	}
void __ISR(_TIMER_4_VECTOR, IPL6AUTO) Timer4Handler(void)
//void __ISR(_TIMER_4_VECTOR, ipl6) Timer4Handler(void)
	{
    IFS0bits.T4IF = 0;
	GEN_2_PIN = ~ GEN_2_PIN;
	rnd_var3++;
	}

void exp_set(uint8_t pos, uint8_t val)
	{
	if (pos==0) EXP_0_OUT = val;
	if (pos==1) EXP_1_OUT = val;
	if (pos==2) EXP_2_OUT = val;
	if (pos==3) EXP_3_OUT = val;
	}

void exp_ddr(uint8_t pos, uint8_t val)
	{
	if (pos==0) EXP_0_T = val;
	if (pos==1) EXP_1_T = val;
	if (pos==2) EXP_2_T = val;
	if (pos==3) EXP_3_T = val;
	}

uint8_t exp_get (uint8_t pos)
	{
	if (pos==0) return EXP_0_IN;
	if (pos==1) return EXP_1_IN;
	if (pos==2) return EXP_2_IN;
	if (pos==3) return EXP_3_IN;
	return 0;
	}