#define		LCD_PORT	LATE
#define		LCD_WR		LATDbits.LATD4
#define		LCD_RES		LATGbits.LATG7
#define		LCD_RD		LATDbits.LATD5
#define		LCD_DC		LATGbits.LATG8

#define		LCD_WR_SET	PORTDSET = (1<<4)
#define		LCD_WR_CLR	PORTDCLR = (1<<4)
#define		LCD_DC_SET	PORTGSET = (1<<8)
#define		LCD_DC_CLR	PORTGCLR = (1<<8)

#define		LCD_BKLT	LATFbits.LATF0
#define		LCD_PWR		LATCbits.LATC15

#define     K_R5        LATFbits.LATF4
#define     K_R4        LATBbits.LATB15
#define     K_R3        LATBbits.LATB14
#define     K_R2        LATBbits.LATB13
#define     K_R1        LATBbits.LATB12

#define     K_C1        PORTBbits.RB11
#define     K_C2        PORTBbits.RB10
#define     K_C3        PORTBbits.RB9
#define     K_C4        PORTBbits.RB8
#define     K_C5        PORTBbits.RB7
#define     K_C6        PORTBbits.RB6
#define     K_C7        PORTBbits.RB5
#define     K_C8        PORTBbits.RB4
#define     K_C9        PORTBbits.RB3
#define     K_C10        PORTBbits.RB2
#define     K_SHIFTL    PORTDbits.RD9
#define     K_SHIFTR    PORTDbits.RD10
#define		K_PWR		PORTGbits.RG6
#define		KEY_BRK		PORTGbits.RG9

#define		SYS_CLK		48000000UL
#define		PB_CLK		48000000UL

#define     LEDR        LATDbits.LATD6
#define     LEDG        LATFbits.LATF1
#define     LEDB        LATDbits.LATD7


#define		CS_FLASH	LATFbits.LATF2
#define		FLASH_WP	LATDbits.LATD8
#define		FLASH_HOLD	LATCbits.LATC12

#define		GEN_0_PIN	LATDbits.LATD1
#define		GEN_1_PIN	LATDbits.LATD2
#define		GEN_2_PIN	LATDbits.LATD3
#define		GEN_ENABLE	LATDbits.LATD11

#define		LED_R		LATDbits.LATD6
#define		LED_G		LATFbits.LATF1
#define		LED_B		LATDbits.LATD7

#define		BASIC_SAVNUM	16
#define		BASIC_BASEADDR	1024*1024*2

#define	FLASH_BUFFERING

void delay_us (unsigned long howmuch);
void wait_1ms (void);
void wait_ms (unsigned int count);
unsigned char keyb_tasks (void);
void hw_init (void);
void sound_set_note (unsigned char note, unsigned char generator);
void sound_set_generator (unsigned int period, unsigned char generator);
void sound_play_notes (unsigned char note1, unsigned char note2, unsigned char note3, unsigned int wait);
void hw_sleep (void);
void (*start_after_wake)(void);
void wake_return(void);
void set_led (unsigned char led_n, unsigned char led_v);


typedef union
{
struct
 {
    unsigned up:1;
    unsigned dn:1;
    unsigned lt:1;
    unsigned rt:1;
    unsigned ent:1;
    unsigned col:1;
    unsigned brk:1;
    unsigned del:1;
 };
unsigned char CHAR;
}kspec_var; 

#define	NEWLINE	0x0A
#define	K_ENT	0x0A
#define	K_ECR	0x0D

#define	BACKSPACE	0x08
#define	K_UP		0x1C
#define	K_DN		0x1D
#define	K_LT		0x1E
#define	K_RT		0x1F
#define	ESCAPE		0x1B
#define	K_DEL		0x08
#define	K_COL		0x01
#define	K_BRK		0x02

#define	TBUF_LEN	2048
#define	TERM_WIDTH	40
#define	TERM_LINES	20

#define	STDIO_LOCAL	0
#define	STDIO_TTY1	1

void init_io(unsigned char mode);
unsigned char stdio_write (char * data);
unsigned char stdio_c (unsigned char data);
unsigned char stdio_in (unsigned char block);
char stdio_get_state (void);
char stdio_get (char * dat);

#define	FCY		48000000UL
#define	FPB		FCY/1
