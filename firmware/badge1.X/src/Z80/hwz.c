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

extern const uint8_t rom_image[65536];
extern const uint8_t rd_image[131072];
uint8_t drive, sector, track,disk_temp_pointer;
uint8_t disk_temp[128];

uint8_t flash_buff[4096];

uint8_t conin_buffer[30];
uint8_t conin_buffer_pointer;

uint32_t last_addr;
uint8_t unwritten;

uint8_t fl_rdsr(void);
uint8_t fl_rdid(void);

#ifdef	USE_RAM_IMAGE
extern const uint8_t ram_image[65536];
#endif

uint8_t ram_disk[RAMDISK_SIZE];


void reload_cpm_warm (void)
{
uint16_t i;
#ifdef	USE_RAM_IMAGE	
	for (i=0xD400;i<(0xD400+0x1EFF);i++) ram[i] = ram_image[i];
#endif
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
uint8_t rxm_sta (void)
{
/*
if (U3BSTAbits.URXDA==1) return 0xFF;
	else return 0x00;
 */
}
uint8_t rxm_read (void)
{
/*
return U3BRXREG;
 */
}
void txm_write (uint8_t data)
{
/*
U3BTXREG = data;
while (U3BSTAbits.UTXBF==1);
 */
}



void set_drive (uint8_t dat)
{
drive = dat;
disk_temp_pointer = 0;
}
void set_sector (uint8_t dat)
{
sector = dat;
disk_temp_pointer = 0;
}
void set_track (uint8_t dat)
{
track = dat;
disk_temp_pointer = 0;
}

uint8_t read_disk_byte (void)
{
uint8_t temp;
uint32_t  base,ptr;
base = (((uint32_t )(track))*16) + sector;
if (drive==0)
	{
	base = base*128;
	ptr = base + disk_temp_pointer;
	if (ptr<RAMDISK_SIZE)
		temp = ram_disk[ptr];
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
	base = base*128;
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

void write_disk_byte (uint8_t dat)
{
uint8_t temp;
uint16_t base;
uint32_t  ptr;
base = (((unsigned int)(track))*16) + sector;
if (drive==0)
	{
	base = base*128;
	ptr = base + disk_temp_pointer;
	if (ptr<RAMDISK_SIZE)
		ram_disk[ptr] = dat;
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






uint8_t fl_rdsr(void)
{
volatile uint8_t temp;
CS_FLASH = 0;
SPI_dat(0x05);
temp = SPI_dat(0xFF);
CS_FLASH = 1;
return temp;
}


uint8_t fl_rdid(void)
{
volatile uint8_t temp;
CS_FLASH = 0;
SPI_dat(0x9F);
temp = SPI_dat(0xFF);
temp = SPI_dat(0x55);
temp = SPI_dat(0xAA);
CS_FLASH = 1;
}

void fl_read_4k(uint32_t  addr, uint8_t * data)
{
uint16_t i;
CS_FLASH = 0;
SPI_dat(0x03);
SPI_dat((addr>>16)&0xFF);
SPI_dat((addr>>8)&0xFF);
SPI_dat((addr>>0)&0xFF);
for (i=0;i<4096;i++) *data++ = SPI_dat(0xFF);
CS_FLASH = 1;
}

void fl_read_nk(uint32_t  addr, uint8_t * data, uint16_t n)
{
uint16_t i;
CS_FLASH = 0;
SPI_dat(0x03);
SPI_dat((addr>>16)&0xFF);
SPI_dat((addr>>8)&0xFF);
SPI_dat((addr>>0)&0xFF);
for (i=0;i<n;i++) *data++ = SPI_dat(0xFF);
CS_FLASH = 1;
}


void fl_erase_4k(uint32_t  addr)
{
uint16_t i;
fl_wren();
CS_FLASH = 0;
SPI_dat(0x20);
SPI_dat((addr>>16)&0xFF);
SPI_dat((addr>>8)&0xFF);
SPI_dat((addr>>0)&0xFF);
CS_FLASH = 1;
while ((fl_rdsr())&0x01);
}


void fl_write(uint32_t  addr,uint8_t data)
{
uint16_t i;
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


void fl_write_4k(uint32_t  addr, uint8_t * data)
{
uint16_t i;
for (i=0;i<4096;i++) 
	{
	fl_write(addr+i,*data++);
	while ((fl_rdsr())&0x01);
	}
}

void fl_write_128(uint32_t sector,uint8_t * data)
{
uint32_t  addr;
uint8_t i;
addr = ((uint32_t )(sector))*128UL;
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
addr = ((uint32_t )(sector))*128UL;
addr = addr&0x00000FFF;
for (i=0;i<128;i++) flash_buff[addr+i] = data[i];
addr = ((uint32_t )(sector))*128UL;
addr = addr&0xFFFFF000;
#ifndef	FLASH_BUFFERING	
fl_erase_4k(addr);
fl_write_4k(addr,flash_buff);
#endif
}

void fl_read_128(uint32_t sector,uint8_t * data)
{
uint32_t  addr;
#ifdef	FLASH_BUFFERING	
if (unwritten == 1)
	{
	fl_erase_4k(last_addr);
	fl_write_4k(last_addr,flash_buff);
	unwritten = 0;	
	last_addr = 0xFFFF;
	}
#endif
addr = ((uint32_t )(sector))*128UL;
fl_read_nk(addr,data,128);
}

void init_first_x_sects (uint8_t i)			//format directory area
{
uint32_t j;
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
