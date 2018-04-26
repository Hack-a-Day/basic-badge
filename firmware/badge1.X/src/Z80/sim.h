/*
 * Z80SIM  -  a	Z80-CPU	simulator
 *
 * Copyright (C) 1987-92 by Udo Munk
 *
 * This is the configuration I'm using for software testing and debugging
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
 *	The following defines may be activated, commented or modified
 *	by user for her/his own purpose.
 */

#include <plib.h>

/*#define WANT_INT*/	/* activate CPU's interrupts */
#define WANT_SPC	/* activate SP over-/underrun handling 0000<->FFFF */
#define WANT_PCC	/* activate PC overrun handling FFFF->0000 */
//#define	CNTL_C		/* cntl-c will stop running emulation */
//#define	CNTL_BS		/* cntl-\ will stop running emulation */
//#define	WANT_TIM	/* activate runtime measurement */
//#define	HISIZE	100	/* number of entrys in history */
//#define	SBSIZE	4	/* number of software breakpoints */


/*
 *	The following defines may be modified and activated by
 *	user, to print her/his copyright for a developed system,
 *	which contains the Z80-CPU simulation as a part.
 */
/*
#define	USR_COM	"XYZ-System Simulation"
#define	USR_REL	"x.y"
#define	USR_CPR	"Copyright (C) 19xx by XYZ"
*/

/*
 *	The following lines of this file must not be modified by user,
 *	see license agreement!
 */
#define	COPYR	"Copyright (C) 1987-92 by Udo Munk"
#define	RELEASE	"1.7"

#define	LENCMD		80		/* length of command buffers etc */

#define	S_FLAG		128		/* bit definitions of CPU flags */
#define	Z_FLAG		64
#define	N2_FLAG		32
#define	H_FLAG		16
#define	N1_FLAG		8
#define	P_FLAG		4
#define	N_FLAG		2
#define	C_FLAG		1

					/* operation of simulated CPU */
#define	SINGLE_STEP	0		/* single step */
#define	CONTIN_RUN	1		/* continual run */
#define	STOPPED		0		/* stop CPU because of error */

					/* causes of error */
#define	NONE		0		/* no error */
#define	OPHALT		1		/* HALT	op-code	trap */
#define	IOTRAP		2		/* IN/OUT trap */
#define	OPTRAP1		3		/* illegal 1 byte op-code trap */
#define	OPTRAP2		4		/* illegal 2 byte op-code trap */
#define	OPTRAP4		5		/* illegal 4 byte op-code trap */
#define	USERINT		6		/* user	interrupt */

					/* type of CPU interrupt */
#define	INT_NMI		1		/* non maskable interrupt */
#define	INT_INT		2		/* maskable interrupt */

//typedef	unsigned short WORD;		/* 16 bit unsigned */
//typedef	unsigned char  BYTE;		/* 8 bit unsigned */

#ifdef HISIZE
struct history {			/* structure of a history entry */
	WORD	h_adr;			/* address of execution */
	WORD	h_af;			/* register AF */
	WORD	h_bc;			/* register BC */
	WORD	h_de;			/* register DE */
	WORD	h_hl;			/* register HL */
	WORD	h_ix;			/* register IX */
	WORD	h_iy;			/* register IY */
	WORD	h_sp;			/* register SP */
};
#endif

#ifdef SBSIZE
struct softbreak {			/* structure of a breakpoint */
	WORD	sb_adr;			/* address of breakpoint */
	BYTE	sb_oldopc;		/* op-code at address of breakpoint */
	int	sb_passcount;		/* pass counter of breakpoint */
	int	sb_pass;		/* no. of pass to break */
};
#endif
