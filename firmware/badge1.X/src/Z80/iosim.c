/*
 * Z80SIM  -  a	Z80-CPU	simulator
 *
 * Copyright (C) 1987-92 by Udo Munk
 *
 * This modul of the simulator contains a simple terminal I/O
 * simulation as an example. It is released to the public domain
 * and may be modified by user.
 *
 * History:
 * 28-SEP-87 Development on TARGON/35 with AT&T Unix System V.3
 * 11-JAN-89 Release 1.1
 * 08-FEB-89 Release 1.2
 * 13-MAR-89 Release 1.3
 * 09-FEB-90 Release 1.4 Ported to TARGON/31 M10/30
 * 20-DEC-90 Release 1.5 Ported to COHERENT 3.0
 * 10-JUN-92 Release 1.6 long casting problem solved with COHERENT 3.2
 *			 and some optimization
 * 25-JUN-92 Release 1.7 comments in english
 */

/*
 *	Sample I/O-handler
 *
 *	Port 0 input:	reads the next byte from stdin
 *	Port 0 output:	writes the byte to stdout
 *
 *	All the other ports are connected to a I/O-trap handler,
 *	I/O to this ports stops the simulation with an I/O error.
 */
#include <plib.h>
#include <stdio.h>
#include "sim.h"
#include "simglb.h"
#include "hwz.h"
#include "../hw.h"

uint8_t iosim_mode;

/*
 *	This function is to initiate the I/O devices.
 *	It will be called from the CPU simulation before
 *	any operation with the Z80 is possible.
 *
 *	In this sample I/O simulation we initialize all
 *	unused port with an error trap handler, so that
 *	simulation stops at I/O on the unused ports.
 */
void init_io(uint8_t mode)
{
iosim_mode = mode;
}

/*
 *	This function is to stop the I/O devices. It is
 *	called from the CPU simulation on exit.
 *
 */
void exit_io()
{
}

/*
 *	This is the main handler for all IN op-codes,
 *	called by the simulator. It calls the input
 *	function for port adr.
 */
BYTE io_in(adr)
BYTE adr;
{
char sstr[3];
uint8_t test,test2;
if (iosim_mode==IO_CPM_MODE)
	{	
	if (adr==0x01)	
		{								//const
	//	return rx_sta();
		return stdio_get_state();
		}
	if (adr==0x02)						//conin
		{
		while (stdio_get_state()==0);
		stdio_get(sstr);
		return sstr[0];
		}
	if (adr==0x07)
		{
		return read_disk_byte();
		}
	//B_CPM001
	if (adr==0x0A)						//reader device
		{
	//not implemented by now
	//	while (rx_sta()==0x00);
	//	return rx_read();
		}

	if (adr==0x68)
		{
		return rx_read();
		}
	if (adr==0x6D)
		{
		test=0x20;
		//if (rxm_sta()==0xFF) test = test|0x01;
		if (rx_sta()==0xFF) test = test|0x01;
		return test;
		}
	}
if (iosim_mode==IO_BASIC_MODE)
	{
//	IN 0 returns serial status: 0x02 for no data on input buffer, 0x22 means data are available
//  IN 1 reads data from serial port
	if (adr==0x00)	
		{		
		if (stdio_get_state()==0)
			return 0x02;
		else
			return 0x22;
		}	
	if (adr==0x01)	
		{		
		stdio_get(sstr);
		return sstr[0];
		}
	}
}

/*
 *	This is the main handler for all OUT op-codes,
 *	called by the simulator. It calls the output
 *	function for port adr.
 */
BYTE io_out(adr, data)
BYTE adr, data;
{
uint8_t test;
if (iosim_mode==IO_CPM_MODE)
	{		
	if (adr==0x03)						//concout device
		{
		stdio_c(data);
		}
	if (adr==0x04)
		{
		set_drive(data);
		}
	if (adr==0x05)
		{
		set_track(data);
		}
	if (adr==0x06)
		{
		set_sector(data);
		}
	if (adr==0x08)
		{
		write_disk_byte(data);
		}
	//B_CPM002
	if (adr==0x09)					//punch device
		{
	//	tx_write(data);				//not implemented by now
		}
	if (adr==0x0B)					//list device
		{
	//	tx_write(data);				//not implemented by now
		}
	if (adr==0x68)
		{
		tx_write(data);
		}
	if (adr==0xFF)
		{
		reload_cpm_warm();
		}
	}
if (iosim_mode==IO_BASIC_MODE)
	{
	if (adr==0x01)						//concout device
		{
		stdio_c(data);
		}	
	}
}

/*
 *	I/O trap funtion
 *	This function should be added into all unused
 *	entrys of the port array. It stops the emulation
 *	with an I/O error.
 */
static BYTE io_trap()
{
	cpu_error = IOTRAP;
	cpu_state = STOPPED;
	return((BYTE) 0);
}

