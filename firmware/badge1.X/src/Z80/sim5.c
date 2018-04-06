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
 *	Like the function "cpu()" this one emulates multi byte opcodes
 *	starting with 0xfd
 */

#include "sim.h"
#include "simglb.h"
#include "fdefs.h"
long op_fd_handel()
{
	long trap_fd();
	long op_popiy(), op_pusiy();
	long op_jpiy();
	long op_exspy();
	long op_ldspy();
	long op_ldiynn(), op_ldiyinn(),	op_ldiny();
	long op_adayd(), op_acayd(), op_suayd(), op_scayd();
	long op_andyd(), op_xoryd(), op_oryd(),	op_cpyd();
	long op_decyd(), op_incyd();
	long op_addyb(), op_addyd(), op_addys(), op_addyy();
	long op_andyd(), op_xoryd(), op_oryd(),	op_cpyd();
	long op_decyd(), op_incyd();
	long op_inciy(), op_deciy();
	long op_ldayd(), op_ldbyd(), op_ldcyd(), op_lddyd(), op_ldeyd();
	long op_ldhyd(), op_ldlyd();
	long op_ldyda(), op_ldydb(), op_ldydc(), op_ldydd(), op_ldyde();
	long op_ldydh(), op_ldydl(), op_ldydn();
	extern long op_fdcb_handel();

	static long (*op_fd[256]) () = {
		trap_fd,			/* 0x00	*/
		trap_fd,			/* 0x01	*/
		trap_fd,			/* 0x02	*/
		trap_fd,			/* 0x03	*/
		trap_fd,			/* 0x04	*/
		trap_fd,			/* 0x05	*/
		trap_fd,			/* 0x06	*/
		trap_fd,			/* 0x07	*/
		trap_fd,			/* 0x08	*/
		op_addyb,			/* 0x09	*/
		trap_fd,			/* 0x0a	*/
		trap_fd,			/* 0x0b	*/
		trap_fd,			/* 0x0c	*/
		trap_fd,			/* 0x0d	*/
		trap_fd,			/* 0x0e	*/
		trap_fd,			/* 0x0f	*/
		trap_fd,			/* 0x10	*/
		trap_fd,			/* 0x11	*/
		trap_fd,			/* 0x12	*/
		trap_fd,			/* 0x13	*/
		trap_fd,			/* 0x14	*/
		trap_fd,			/* 0x15	*/
		trap_fd,			/* 0x16	*/
		trap_fd,			/* 0x17	*/
		trap_fd,			/* 0x18	*/
		op_addyd,			/* 0x19	*/
		trap_fd,			/* 0x1a	*/
		trap_fd,			/* 0x1b	*/
		trap_fd,			/* 0x1c	*/
		trap_fd,			/* 0x1d	*/
		trap_fd,			/* 0x1e	*/
		trap_fd,			/* 0x1f	*/
		trap_fd,			/* 0x20	*/
		op_ldiynn,			/* 0x21	*/
		op_ldiny,			/* 0x22	*/
		op_inciy,			/* 0x23	*/
		trap_fd,			/* 0x24	*/
		trap_fd,			/* 0x25	*/
		trap_fd,			/* 0x26	*/
		trap_fd,			/* 0x27	*/
		trap_fd,			/* 0x28	*/
		op_addyy,			/* 0x29	*/
		op_ldiyinn,			/* 0x2a	*/
		op_deciy,			/* 0x2b	*/
		trap_fd,			/* 0x2c	*/
		trap_fd,			/* 0x2d	*/
		trap_fd,			/* 0x2e	*/
		trap_fd,			/* 0x2f	*/
		trap_fd,			/* 0x30	*/
		trap_fd,			/* 0x31	*/
		trap_fd,			/* 0x32	*/
		trap_fd,			/* 0x33	*/
		op_incyd,			/* 0x34	*/
		op_decyd,			/* 0x35	*/
		op_ldydn,			/* 0x36	*/
		trap_fd,			/* 0x37	*/
		trap_fd,			/* 0x38	*/
		op_addys,			/* 0x39	*/
		trap_fd,			/* 0x3a	*/
		trap_fd,			/* 0x3b	*/
		trap_fd,			/* 0x3c	*/
		trap_fd,			/* 0x3d	*/
		trap_fd,			/* 0x3e	*/
		trap_fd,			/* 0x3f	*/
		trap_fd,			/* 0x40	*/
		trap_fd,			/* 0x41	*/
		trap_fd,			/* 0x42	*/
		trap_fd,			/* 0x43	*/
		trap_fd,			/* 0x44	*/
		trap_fd,			/* 0x45	*/
		op_ldbyd,			/* 0x46	*/
		trap_fd,			/* 0x47	*/
		trap_fd,			/* 0x48	*/
		trap_fd,			/* 0x49	*/
		trap_fd,			/* 0x4a	*/
		trap_fd,			/* 0x4b	*/
		trap_fd,			/* 0x4c	*/
		trap_fd,			/* 0x4d	*/
		op_ldcyd,			/* 0x4e	*/
		trap_fd,			/* 0x4f	*/
		trap_fd,			/* 0x50	*/
		trap_fd,			/* 0x51	*/
		trap_fd,			/* 0x52	*/
		trap_fd,			/* 0x53	*/
		trap_fd,			/* 0x54	*/
		trap_fd,			/* 0x55	*/
		op_lddyd,			/* 0x56	*/
		trap_fd,			/* 0x57	*/
		trap_fd,			/* 0x58	*/
		trap_fd,			/* 0x59	*/
		trap_fd,			/* 0x5a	*/
		trap_fd,			/* 0x5b	*/
		trap_fd,			/* 0x5c	*/
		trap_fd,			/* 0x5d	*/
		op_ldeyd,			/* 0x5e	*/
		trap_fd,			/* 0x5f	*/
		trap_fd,			/* 0x60	*/
		trap_fd,			/* 0x61	*/
		trap_fd,			/* 0x62	*/
		trap_fd,			/* 0x63	*/
		trap_fd,			/* 0x64	*/
		trap_fd,			/* 0x65	*/
		op_ldhyd,			/* 0x66	*/
		trap_fd,			/* 0x67	*/
		trap_fd,			/* 0x68	*/
		trap_fd,			/* 0x69	*/
		trap_fd,			/* 0x6a	*/
		trap_fd,			/* 0x6b	*/
		trap_fd,			/* 0x6c	*/
		trap_fd,			/* 0x6d	*/
		op_ldlyd,			/* 0x6e	*/
		trap_fd,			/* 0x6f	*/
		op_ldydb,			/* 0x70	*/
		op_ldydc,			/* 0x71	*/
		op_ldydd,			/* 0x72	*/
		op_ldyde,			/* 0x73	*/
		op_ldydh,			/* 0x74	*/
		op_ldydl,			/* 0x75	*/
		trap_fd,			/* 0x76	*/
		op_ldyda,			/* 0x77	*/
		trap_fd,			/* 0x78	*/
		trap_fd,			/* 0x79	*/
		trap_fd,			/* 0x7a	*/
		trap_fd,			/* 0x7b	*/
		trap_fd,			/* 0x7c	*/
		trap_fd,			/* 0x7d	*/
		op_ldayd,			/* 0x7e	*/
		trap_fd,			/* 0x7f	*/
		trap_fd,			/* 0x80	*/
		trap_fd,			/* 0x81	*/
		trap_fd,			/* 0x82	*/
		trap_fd,			/* 0x83	*/
		trap_fd,			/* 0x84	*/
		trap_fd,			/* 0x85	*/
		op_adayd,			/* 0x86	*/
		trap_fd,			/* 0x87	*/
		trap_fd,			/* 0x88	*/
		trap_fd,			/* 0x89	*/
		trap_fd,			/* 0x8a	*/
		trap_fd,			/* 0x8b	*/
		trap_fd,			/* 0x8c	*/
		trap_fd,			/* 0x8d	*/
		op_acayd,			/* 0x8e	*/
		trap_fd,			/* 0x8f	*/
		trap_fd,			/* 0x90	*/
		trap_fd,			/* 0x91	*/
		trap_fd,			/* 0x92	*/
		trap_fd,			/* 0x93	*/
		trap_fd,			/* 0x94	*/
		trap_fd,			/* 0x95	*/
		op_suayd,			/* 0x96	*/
		trap_fd,			/* 0x97	*/
		trap_fd,			/* 0x98	*/
		trap_fd,			/* 0x99	*/
		trap_fd,			/* 0x9a	*/
		trap_fd,			/* 0x9b	*/
		trap_fd,			/* 0x9c	*/
		trap_fd,			/* 0x9d	*/
		op_scayd,			/* 0x9e	*/
		trap_fd,			/* 0x9f	*/
		trap_fd,			/* 0xa0	*/
		trap_fd,			/* 0xa1	*/
		trap_fd,			/* 0xa2	*/
		trap_fd,			/* 0xa3	*/
		trap_fd,			/* 0xa4	*/
		trap_fd,			/* 0xa5	*/
		op_andyd,			/* 0xa6	*/
		trap_fd,			/* 0xa7	*/
		trap_fd,			/* 0xa8	*/
		trap_fd,			/* 0xa9	*/
		trap_fd,			/* 0xaa	*/
		trap_fd,			/* 0xab	*/
		trap_fd,			/* 0xac	*/
		trap_fd,			/* 0xad	*/
		op_xoryd,			/* 0xae	*/
		trap_fd,			/* 0xaf	*/
		trap_fd,			/* 0xb0	*/
		trap_fd,			/* 0xb1	*/
		trap_fd,			/* 0xb2	*/
		trap_fd,			/* 0xb3	*/
		trap_fd,			/* 0xb4	*/
		trap_fd,			/* 0xb5	*/
		op_oryd,			/* 0xb6	*/
		trap_fd,			/* 0xb7	*/
		trap_fd,			/* 0xb8	*/
		trap_fd,			/* 0xb9	*/
		trap_fd,			/* 0xba	*/
		trap_fd,			/* 0xbb	*/
		trap_fd,			/* 0xbc	*/
		trap_fd,			/* 0xbd	*/
		op_cpyd,			/* 0xbe	*/
		trap_fd,			/* 0xbf	*/
		trap_fd,			/* 0xc0	*/
		trap_fd,			/* 0xc1	*/
		trap_fd,			/* 0xc2	*/
		trap_fd,			/* 0xc3	*/
		trap_fd,			/* 0xc4	*/
		trap_fd,			/* 0xc5	*/
		trap_fd,			/* 0xc6	*/
		trap_fd,			/* 0xc7	*/
		trap_fd,			/* 0xc8	*/
		trap_fd,			/* 0xc9	*/
		trap_fd,			/* 0xca	*/
		op_fdcb_handel,			/* 0xcb	*/
		trap_fd,			/* 0xcc	*/
		trap_fd,			/* 0xcd	*/
		trap_fd,			/* 0xce	*/
		trap_fd,			/* 0xcf	*/
		trap_fd,			/* 0xd0	*/
		trap_fd,			/* 0xd1	*/
		trap_fd,			/* 0xd2	*/
		trap_fd,			/* 0xd3	*/
		trap_fd,			/* 0xd4	*/
		trap_fd,			/* 0xd5	*/
		trap_fd,			/* 0xd6	*/
		trap_fd,			/* 0xd7	*/
		trap_fd,			/* 0xd8	*/
		trap_fd,			/* 0xd9	*/
		trap_fd,			/* 0xda	*/
		trap_fd,			/* 0xdb	*/
		trap_fd,			/* 0xdc	*/
		trap_fd,			/* 0xdd	*/
		trap_fd,			/* 0xde	*/
		trap_fd,			/* 0xdf	*/
		trap_fd,			/* 0xe0	*/
		op_popiy,			/* 0xe1	*/
		trap_fd,			/* 0xe2	*/
		op_exspy,			/* 0xe3	*/
		trap_fd,			/* 0xe4	*/
		op_pusiy,			/* 0xe5	*/
		trap_fd,			/* 0xe6	*/
		trap_fd,			/* 0xe7	*/
		trap_fd,			/* 0xe8	*/
		op_jpiy,			/* 0xe9	*/
		trap_fd,			/* 0xea	*/
		trap_fd,			/* 0xeb	*/
		trap_fd,			/* 0xec	*/
		trap_fd,			/* 0xed	*/
		trap_fd,			/* 0xee	*/
		trap_fd,			/* 0xef	*/
		trap_fd,			/* 0xf0	*/
		trap_fd,			/* 0xf1	*/
		trap_fd,			/* 0xf2	*/
		trap_fd,			/* 0xf3	*/
		trap_fd,			/* 0xf4	*/
		trap_fd,			/* 0xf5	*/
		trap_fd,			/* 0xf6	*/
		trap_fd,			/* 0xf7	*/
		trap_fd,			/* 0xf8	*/
		op_ldspy,			/* 0xf9	*/
		trap_fd,			/* 0xfa	*/
		trap_fd,			/* 0xfb	*/
		trap_fd,			/* 0xfc	*/
		trap_fd,			/* 0xfd	*/
		trap_fd,			/* 0xfe	*/
		trap_fd				/* 0xff	*/
	};

#ifdef WANT_TIM
	register long t;
	t = (*op_fd[*PC++]) ();		/* execute next opcode */
#else
	(*op_fd[*PC++]) ();
#endif

#ifdef WANT_PCC
		if (PC > ram + 65535)	/* correct PC overrun */
			PC = ram;
#endif

#ifdef WANT_TIM
	return(t);
#endif
}

/*
 *	This function traps all illegal opcodes following the
 *	initial 0xfd of a multi byte opcode.
 */
static long trap_fd()
{
	cpu_error = OPTRAP2;
	cpu_state = STOPPED;
#ifdef WANT_TIM
	return(0L);
#endif
}

static long op_popiy()		     /*	POP IY */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	IY = *STACK++;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	IY += *STACK++ << 8;
#ifdef WANT_TIM
	return(14L);
#endif
}

static long op_pusiy()		     /*	PUSH IY	*/
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = IY >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = IY;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_jpiy()		     /*	JP (IY)	*/
{
	PC = ram + IY;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_exspy()		     /*	EX (SP),IY */
{
	register int i;

	i = *STACK + (*(STACK +	1) << 8);
	*STACK = IY;
	*(STACK	+ 1) = IY >> 8;
	IY = i;
#ifdef WANT_TIM
	return(23L);
#endif
}

static long op_ldspy()		     /*	LD SP,IY */
{
	STACK =	ram + IY;
#ifdef WANT_TIM
	return(10L);
#endif
}

static long op_ldiynn()		     /*	LD IY,nn */
{
	IY = *PC++;
	IY += *PC++ << 8;
#ifdef WANT_TIM
	return(14L);
#endif
}

static long op_ldiyinn()	     /*	LD IY,(nn) */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	IY = *p++;
	IY += *p << 8;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_ldiny()		     /*	LD (nn),IY */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	*p++ = IY;
	*p = IY	>> 8;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_adayd()		     /*	ADD A,(IY+d) */
{
	register int i;
	register BYTE P;

	P = *(ram + IY + (char)	*PC++);
	((A & 0xf) + (P	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + P > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_acayd()		     /*	ADC A,(IY+d) */
{
	register int i,	carry;
	register BYTE P;

	carry =	(F & C_FLAG) ? 1 : 0;
	P = *(ram + IY + (char)	*PC++);
	((A & 0xf) + (P	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + P + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) P + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_suayd()		     /*	SUB A,(IY+d) */
{
	register int i;
	register BYTE P;

	P = *(ram + IY + (char)	*PC++);
	((P & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_scayd()		     /*	SBC A,(IY+d) */
{
	register int i,	carry;
	register BYTE P;

	carry =	(F & C_FLAG) ? 1 : 0;
	P = *(ram + IY + (char)	*PC++);
	((P & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) P - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_andyd()		     /*	AND (IY+d) */
{
	A &= *(ram + IY	+ (char) *PC++);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_xoryd()		     /*	XOR (IY+d) */
{
	A ^= *(ram + IY	+ (char) *PC++);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_oryd()		     /*	OR (IY+d) */
{
	A |= *(ram + IY	+ (char) *PC++);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_cpyd()		     /*	CP (IY+d) */
{
	register int i;
	register BYTE P;

	P = *(ram + IY + (char)	*PC++);
	((P & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_incyd()		     /*	INC (IY+d) */
{
	register BYTE *p;

	p = ram	+ IY + (char) *PC++;
	((*p & 0xf) + 1	> 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	(*p)++;
	(*p == 128) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(23L);
#endif
}

static long op_decyd()		     /*	DEC (IY+d) */
{
	register BYTE *p;

	p = ram	+ IY + (char) *PC++;
	((*p - 1 & 0xf)	== 0xf)	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(*p)--;
	(*p == 127) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(23L);
#endif
}

static long op_addyb()		     /*	ADD IY,BC */
{
	register long i;

	i = (((long)IY) + (((long)B) << 8) + C);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	IY = i;
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_addyd()		     /*	ADD IY,DE */
{
	register long i;

	i = (((long)IY) + (((long)D) << 8) + E);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	IY = i;
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_addys()		     /*	ADD IY,SP */
{
	register long i;

	i = ((long)IY) + ((long)STACK) - ((long)ram);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	IY = i;
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_addyy()		     /*	ADD IY,IY */
{
	register long i;

	i = (((long)IY) << 1);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	IY = i;
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_inciy()		     /*	INC IY */
{
	IY++;
#ifdef WANT_TIM
	return(10L);
#endif
}

static long op_deciy()		     /*	DEC IY */
{
	IY--;
#ifdef WANT_TIM
	return(10L);
#endif
}

static long op_ldayd()		     /*	LD A,(IY+d) */
{
	A = *(IY + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldbyd()		     /*	LD B,(IY+d) */
{
	B = *(IY + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldcyd()		     /*	LD C,(IY+d) */
{
	C = *(IY + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_lddyd()		     /*	LD D,(IY+d) */
{
	D = *(IY + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldeyd()		     /*	LD E,(IY+d) */
{
	E = *(IY + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldhyd()		     /*	LD H,(IY+d) */
{
	H = *(IY + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldlyd()		     /*	LD L,(IY+d) */
{
	L = *(IY + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldyda()		     /*	LD (IY+d),A */
{
	*(IY + (char) *PC++ + ram) = A;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldydb()		     /*	LD (IY+d),B */
{
	*(IY + (char) *PC++ + ram) = B;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldydc()		     /*	LD (IY+d),C */
{
	*(IY + (char) *PC++ + ram) = C;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldydd()		     /*	LD (IY+d),D */
{
	*(IY + (char) *PC++ + ram) = D;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldyde()		     /*	LD (IY+d),E */
{
	*(IY + (char) *PC++ + ram) = E;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldydh()		     /*	LD (IY+d),H */
{
	*(IY + (char) *PC++ + ram) = H;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldydl()		     /*	LD (IY+d),L */
{
	*(IY + (char) *PC++ + ram) = L;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldydn()		     /*	LD (IY+d),n */
{
	register int d;

	d = (char) *PC++;
	*(IY + d + ram)	= *PC++;
#ifdef WANT_TIM
	return(19L);
#endif
}
