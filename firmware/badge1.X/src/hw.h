#define		LCD_PORT	LATE
#define		LCD_WR		LATDbits.LATD4
#define		LCD_RES		LATGbits.LATG7
#define		LCD_RD		LATDbits.LATD5
#define		LCD_DC		LATGbits.LATG8

#define		LCD_WR_SET	PORTDSET = (1<<4)
#define		LCD_WR_CLR	PORTDCLR = (1<<4)
#define		LCD_DC_SET	PORTGSET = (1<<8)
#define		LCD_DC_CLR	PORTGCLR = (1<<8)

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

#define		SYS_CLK		48000000UL
#define		PB_CLK		48000000UL

#define     LEDR        LATDbits.LATD6
#define     LEDG        LATFbits.LATF1
#define     LEDB        LATDbits.LATD7


#define		CS_FLASH	LATFbits.LATF2
#define		FLASH_WP	LATDbits.LATD8
#define		FLASH_HOLD	LATCbits.LATC12


#define	FLASH_BUFFERING

void delay_us (unsigned long howmuch);
void wait_1ms (void);
void wait_ms (unsigned int count);
unsigned char keyb_tasks (void);
void hw_init (void);

void fl_write(unsigned long addr,unsigned char data);
void fl_erase_4k(unsigned long addr);
void fl_read_4k(unsigned long addr, unsigned char * data);
void fl_rst_pb(void);
void fl_wren(void);
void fl_write_4k(unsigned long addr, unsigned char * data);
void fl_read_nk(unsigned long addr, unsigned char * data, unsigned int n);
void fl_read_128(unsigned int sector,unsigned char * data);
void fl_write_128(unsigned int sector,unsigned char * data);

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

unsigned char stdio_write (unsigned char * data);
unsigned char stdio_c (unsigned char data);
unsigned char stdio_in (unsigned char block);
char stdio_get_state (void);
char stdio_get (char * dat);

#define	FCY		48000000UL
#define	FPB		FCY/1
