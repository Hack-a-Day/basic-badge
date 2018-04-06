#include "hwz.h"
#include <xc.h>
#include <plib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/appio.h>

#include "sim.h"
#include "simglb.h"
#include "../hw.h"

#define	FLASH_BUFFERING	

extern const unsigned char rom_image[65536];
extern const unsigned char rd_image[131072];
unsigned char drive, sector, track,disk_temp_pointer;
unsigned char disk_temp[128];

unsigned char flash_buff[4096];

unsigned char conin_buffer[30];
unsigned char conin_buffer_pointer;

unsigned int last_addr;
unsigned char unwritten;

unsigned char fl_rdsr(void);
unsigned char fl_rdid(void);

#ifdef	USE_RAM_IMAGE
extern const unsigned char ram_image[65536];
#endif

unsigned char ram_disk[RAMDISK_SIZE];


void reload_cpm_warm (void)
{
unsigned int i;
#ifdef	USE_RAM_IMAGE	
	for (i=0xD400;i<(0xD400+0x1EFF);i++) ram[i] = ram_image[i];
#endif
}


void init_diskb(void)
{
unsigned int i;
for (i=0;i<256;i++)
	{
	ee_wren();
	write_sector(ram_disk+ (i*128),i);
	read_sector(disk_temp,i);
	read_sector(ram_disk+ (i*128),i);
	}
for (i=0;i<256;i++)
	{
	read_sector(disk_temp,i);
	read_sector(ram_disk+ (i*128),i);
	}


}



void init_usart (void)
{
/*
U3AMODEbits.ON = 1;
U3ASTAbits.URXEN = 1;
U3ASTAbits.UTXEN = 1;
U3ABRG = ((PB_CLK*1000)/(16*BAUD_U3A)) - 1;
INTEnable(INT_SOURCE_UART_RX(UART3A), INT_ENABLED);
IPC8bits.U3AIP = 2;

U3BMODEbits.ON = 1;
U3BSTAbits.URXEN = 1;
U3BSTAbits.UTXEN = 1;
U3BBRG = ((PB_CLK*1000)/(16*BAUD_U3A)) - 1;
//INTEnable(INT_SOURCE_UART_RX(UART3B), INT_ENABLED);

AD1PCFG=0xFFFFFFFF;
*/
}



//-------------------device at 0x68-----------------
unsigned char rxm_sta (void)
{
/*
if (U3BSTAbits.URXDA==1) return 0xFF;
	else return 0x00;
 */
}
unsigned char rxm_read (void)
{
/*
return U3BRXREG;
 */
}
void txm_write (unsigned char data)
{
/*
U3BTXREG = data;
while (U3BSTAbits.UTXBF==1);
 */
}



void set_drive (unsigned char dat)
{
drive = dat;
disk_temp_pointer = 0;
}
void set_sector (unsigned char dat)
{
sector = dat;
disk_temp_pointer = 0;
}
void set_track (unsigned char dat)
{
track = dat;
disk_temp_pointer = 0;
}

unsigned char read_disk_byte (void)
{
unsigned char temp;
unsigned long base;
base = (((unsigned long)(track))*16) + sector;
if (drive==0)
	{
	base = base*128;
	temp = ram_disk[base + disk_temp_pointer];
	}
if (drive==1)
	{
	base = base*128;
#ifdef	USE_ROMDISK
	temp = rd_image[base + disk_temp_pointer];
#endif
	}
if (drive==2)
	{
	}
if (drive==3)
	{
	if (disk_temp_pointer==0) fl_read_128(base,disk_temp);
	temp = disk_temp[disk_temp_pointer];
	}
if (drive==4)
	{
	if (disk_temp_pointer==0) fl_read_128(base+4096,disk_temp);
	temp = disk_temp[disk_temp_pointer];
	}
if (drive==5)
	{
	if (disk_temp_pointer==0) fl_read_128(base+(2*4096),disk_temp);
	temp = disk_temp[disk_temp_pointer];
	}
if (drive==6)
	{
	if (disk_temp_pointer==0) fl_read_128(base+(3*4096),disk_temp);
	temp = disk_temp[disk_temp_pointer];
	}

disk_temp_pointer++;
return temp;
}

void write_disk_byte (unsigned char dat)
{
unsigned char temp;
unsigned int base;
base = (((unsigned int)(track))*16) + sector;
if (drive==0)
	{
	base = base*128;
	ram_disk[base + disk_temp_pointer] = dat;
	}
if (drive==1)
	{
	//rom disk, no writes allowed
	}
if (drive==2)
	{
	//rom disk, no writes allowed
	}

if (drive==3)
	{
	disk_temp[disk_temp_pointer] = dat;
	if (disk_temp_pointer==127) 
		{
		fl_write_128(base,disk_temp);
		}
	}
if (drive==4)
	{
	disk_temp[disk_temp_pointer] = dat;
	if (disk_temp_pointer==127) 
		{
		fl_write_128(base+(1*4096),disk_temp);
		}
	}
if (drive==5)
	{
	disk_temp[disk_temp_pointer] = dat;
	if (disk_temp_pointer==127) 
		{
		fl_write_128(base+(2*4096),disk_temp);
		}
	}
if (drive==6)
	{
	disk_temp[disk_temp_pointer] = dat;
	if (disk_temp_pointer==127) 
		{
		fl_write_128(base+(3*4096),disk_temp);
		}
	}

disk_temp_pointer++;
}

/*
unsigned char	SPI_dat (unsigned char data)
{

SPI2ABUF = data;
while (SPI2ASTATbits.SPIRBF==0);
return (SPI2ABUF);
}

 */

void write_sector (unsigned char *data, unsigned int addr)
{
unsigned char i,temp;
CS_MEM = 0;
SPI_dat(0x02);
temp = (addr>>9);
SPI_dat(temp);
temp = (addr>>1);
SPI_dat(temp);
temp = (addr<<7);
SPI_dat(temp);

for (i=0;i<128;i++) SPI_dat(*data++);

CS_MEM = 1;
temp = ee_rs();
temp = temp&0x01;

while (temp>0)
	{
	temp = ee_rs();
	temp = temp&0x01;
	}
}

void read_sector (unsigned char *data, unsigned int addr)
{
unsigned char i,temp;
CS_MEM = 0;
SPI_dat(0x03);
temp = (addr>>9);
SPI_dat(temp);
temp = (addr>>1);
SPI_dat(temp);
temp = (addr<<7);
SPI_dat(temp);


for (i=0;i<128;i++) 
	{
	*data = SPI_dat(0xFF);
	*data++;
	}

CS_MEM = 1;
}
unsigned char ee_rs (void)
{
unsigned char temp;
CS_MEM = 0;
SPI_dat(0x05);
temp = SPI_dat(0xFF);
CS_MEM = 1;
return temp;
}

void ee_wren (void)
{
CS_MEM = 0;
SPI_dat(0x06);
CS_MEM = 1;
}

void ee_wrdi (void)
{
CS_MEM = 0;
SPI_dat(0x04);
CS_MEM = 1;
}





void wait_ms (unsigned int count)
{
unsigned int i;
for (i=0;i<count;i++) wait_1ms();
}

void wait_1ms (void)
{
T1CONbits.TON = 1;
T1CONbits.TCKPS = 0b01;
PR1 = 10000;
TMR1=0;
while (IFS0bits.T1IF==0);
IFS0bits.T1IF=0;
}

void wait_cyc (unsigned int cyc)
{
T1CONbits.TON = 1;
PR1 = cyc;
TMR1=0;
while (IFS0bits.T1IF==0);
IFS0bits.T1IF=0;
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
