/*
 * Z80SIM  -  a	Z80-CPU	simulator
 *
 * Copyright (C) 1987-92 by Udo Munk
 *
 * This module of the Z80-CPU simulator must not be modified by a user,
 * see license agreement!
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
 *	This module contain the interrupt handlers for the OS:
 *
 *	int_on()	: initialize interrupt handlers
 *	int_off()	: reset interrupts to default
 *	user_int()	: handler for user interrupt (CNTL-C)
 *	quit_int()	: handler for signal "quit"
 *	nmi_int()	: handler for non maskable interrupt (Z80)
 *	int_int()	: handler for maskable interrupt (Z80)
 */

#define	_FULL_SIGNAL_IMPLEMENTATION

#include <plib.h>
#include <stdio.h>
#include <signal.h>
#include "sim.h"
#include "simglb.h"

int_on()
{
	void user_int();
	void quit_int();

	signal(SIGINT, user_int);
	signal(SIGQUIT,	quit_int);
}

int_off()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT,	SIG_DFL);
}

void user_int()
{
	signal(SIGINT, user_int);
#ifdef CNTL_C
	cpu_error = USERINT;
	cpu_state = STOPPED;
#else
	cntl_c++;
#endif
}

void quit_int()
{
	signal(SIGQUIT,	quit_int);
#ifdef CNTL_BS
	cpu_error = USERINT;
	cpu_state = STOPPED;
#else
	cntl_bs++;
#endif
}
