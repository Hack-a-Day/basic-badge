#define	USE_RAM_IMAGE	
#define	USE_ROMDISK
//#define		REMOVE_MAINLOOP

#define	RAMDISK_SIZE	41200


//#define	PB_CLK	80000					//xkHz
#define	BAUD_U3A	38400
#define	BAUD_U3B	38400
#define	BAUD_U2A	38400
#define	CS_MEM		LATBbits.LATB4
#define	CS_MEM_T	TRISBbits.TRISB4

//#define	CS_FLASH	LATBbits.LATB5
//#define	CS_FLASH_T	TRISBbits.TRISB5

#define	CONIO_SWITCH	LATBbits.LATB15
#define	CONIO_SWITCH_T	TRISBbits.TRISB15

unsigned char rx_sta (void);
unsigned char rx_read (void);
void tx_write (unsigned char data);
void set_drive (unsigned char dat);
void set_sector (unsigned char dat);
void set_track (unsigned char dat);
unsigned char read_disk_byte (void);
void write_disk_byte (unsigned char dat);

unsigned char	SPI_dat (unsigned char data);
unsigned char ee_rs (void);
void ee_wren (void);
void ee_wrdi (void);
void read_sector (unsigned char *data, unsigned int addr);
void write_sector (unsigned char *data, unsigned int addr);
void init_diskb(void);

unsigned char rxk_sta (void);
unsigned char rxk_read (void);


void wait_1ms (void);
void wait_ms (unsigned int count);
void wait_cyc (unsigned int cyc);
void init_termint (void);


void write_flash_sector (unsigned char * data, unsigned int addr);
void read_flash_sector (unsigned char *data, unsigned int addr);
void init_first_x_sects (unsigned char i);

void fl_write(unsigned long addr,unsigned char data);
void fl_erase_4k(unsigned long addr);
void fl_read_4k(unsigned long addr, unsigned char * data);
void fl_rst_pb(void);
void fl_wren(void);
void fl_write_4k(unsigned long addr, unsigned char * data);
void fl_read_nk(unsigned long addr, unsigned char * data, unsigned int n);
void fl_read_128(unsigned int sector,unsigned char * data);
void fl_write_128(unsigned int sector,unsigned char * data);
void init_first_x_sects (unsigned char i);


#define		IO_CPM_MODE		0
#define		IO_BASIC_MODE	1
