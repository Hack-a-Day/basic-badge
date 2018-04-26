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
 *	starting with 0xdd
 */

#include "sim.h"
#include "simglb.h"
#include "fdefs.h"

long op_dd_handel()
{
	long trap_dd();
	long op_popix(), op_pusix();
	long op_jpix();
	long op_exspx();
	long op_ldspx();
	long op_ldixnn(), op_ldixinn(),	op_ldinx();
	long op_adaxd(), op_acaxd(), op_suaxd(), op_scaxd();
	long op_andxd(), op_xorxd(), op_orxd(),	op_cpxd();
	long op_decxd(), op_incxd();
	long op_addxb(), op_addxd(), op_addxs(), op_addxx();
	long op_incix(), op_decix();
	long op_ldaxd(), op_ldbxd(), op_ldcxd(), op_lddxd(), op_ldexd();
	long op_ldhxd(), op_ldlxd();
	long op_ldxda(), op_ldxdb(), op_ldxdc(), op_ldxdd(), op_ldxde();
	long op_ldxdh(), op_ldxdl(), op_ldxdn();
	extern long op_ddcb_handel();

	static long (*op_dd[256]) () = {
		trap_dd,			/* 0x00	*/
		trap_dd,			/* 0x01	*/
		trap_dd,			/* 0x02	*/
		trap_dd,			/* 0x03	*/
		trap_dd,			/* 0x04	*/
		trap_dd,			/* 0x05	*/
		trap_dd,			/* 0x06	*/
		trap_dd,			/* 0x07	*/
		trap_dd,			/* 0x08	*/
		op_addxb,			/* 0x09	*/
		trap_dd,			/* 0x0a	*/
		trap_dd,			/* 0x0b	*/
		trap_dd,			/* 0x0c	*/
		trap_dd,			/* 0x0d	*/
		trap_dd,			/* 0x0e	*/
		trap_dd,			/* 0x0f	*/
		trap_dd,			/* 0x10	*/
		trap_dd,			/* 0x11	*/
		trap_dd,			/* 0x12	*/
		trap_dd,			/* 0x13	*/
		trap_dd,			/* 0x14	*/
		trap_dd,			/* 0x15	*/
		trap_dd,			/* 0x16	*/
		trap_dd,			/* 0x17	*/
		trap_dd,			/* 0x18	*/
		op_addxd,			/* 0x19	*/
		trap_dd,			/* 0x1a	*/
		trap_dd,			/* 0x1b	*/
		trap_dd,			/* 0x1c	*/
		trap_dd,			/* 0x1d	*/
		trap_dd,			/* 0x1e	*/
		trap_dd,			/* 0x1f	*/
		trap_dd,			/* 0x20	*/
		op_ldixnn,			/* 0x21	*/
		op_ldinx,			/* 0x22	*/
		op_incix,			/* 0x23	*/
		trap_dd,			/* 0x24	*/
		trap_dd,			/* 0x25	*/
		trap_dd,			/* 0x26	*/
		trap_dd,			/* 0x27	*/
		trap_dd,			/* 0x28	*/
		op_addxx,			/* 0x29	*/
		op_ldixinn,			/* 0x2a	*/
		op_decix,			/* 0x2b	*/
		trap_dd,			/* 0x2c	*/
		trap_dd,			/* 0x2d	*/
		trap_dd,			/* 0x2e	*/
		trap_dd,			/* 0x2f	*/
		trap_dd,			/* 0x30	*/
		trap_dd,			/* 0x31	*/
		trap_dd,			/* 0x32	*/
		trap_dd,			/* 0x33	*/
		op_incxd,			/* 0x34	*/
		op_decxd,			/* 0x35	*/
		op_ldxdn,			/* 0x36	*/
		trap_dd,			/* 0x37	*/
		trap_dd,			/* 0x38	*/
		op_addxs,			/* 0x39	*/
		trap_dd,			/* 0x3a	*/
		trap_dd,			/* 0x3b	*/
		trap_dd,			/* 0x3c	*/
		trap_dd,			/* 0x3d	*/
		trap_dd,			/* 0x3e	*/
		trap_dd,			/* 0x3f	*/
		trap_dd,			/* 0x40	*/
		trap_dd,			/* 0x41	*/
		trap_dd,			/* 0x42	*/
		trap_dd,			/* 0x43	*/
		trap_dd,			/* 0x44	*/
		trap_dd,			/* 0x45	*/
		op_ldbxd,			/* 0x46	*/
		trap_dd,			/* 0x47	*/
		trap_dd,			/* 0x48	*/
		trap_dd,			/* 0x49	*/
		trap_dd,			/* 0x4a	*/
		trap_dd,			/* 0x4b	*/
		trap_dd,			/* 0x4c	*/
		trap_dd,			/* 0x4d	*/
		op_ldcxd,			/* 0x4e	*/
		trap_dd,			/* 0x4f	*/
		trap_dd,			/* 0x50	*/
		trap_dd,			/* 0x51	*/
		trap_dd,			/* 0x52	*/
		trap_dd,			/* 0x53	*/
		trap_dd,			/* 0x54	*/
		trap_dd,			/* 0x55	*/
		op_lddxd,			/* 0x56	*/
		trap_dd,			/* 0x57	*/
		trap_dd,			/* 0x58	*/
		trap_dd,			/* 0x59	*/
		trap_dd,			/* 0x5a	*/
		trap_dd,			/* 0x5b	*/
		trap_dd,			/* 0x5c	*/
		trap_dd,			/* 0x5d	*/
		op_ldexd,			/* 0x5e	*/
		trap_dd,			/* 0x5f	*/
		trap_dd,			/* 0x60	*/
		trap_dd,			/* 0x61	*/
		trap_dd,			/* 0x62	*/
		trap_dd,			/* 0x63	*/
		trap_dd,			/* 0x64	*/
		trap_dd,			/* 0x65	*/
		op_ldhxd,			/* 0x66	*/
		trap_dd,			/* 0x67	*/
		trap_dd,			/* 0x68	*/
		trap_dd,			/* 0x69	*/
		trap_dd,			/* 0x6a	*/
		trap_dd,			/* 0x6b	*/
		trap_dd,			/* 0x6c	*/
		trap_dd,			/* 0x6d	*/
		op_ldlxd,			/* 0x6e	*/
		trap_dd,			/* 0x6f	*/
		op_ldxdb,			/* 0x70	*/
		op_ldxdc,			/* 0x71	*/
		op_ldxdd,			/* 0x72	*/
		op_ldxde,			/* 0x73	*/
		op_ldxdh,			/* 0x74	*/
		op_ldxdl,			/* 0x75	*/
		trap_dd,			/* 0x76	*/
		op_ldxda,			/* 0x77	*/
		trap_dd,			/* 0x78	*/
		trap_dd,			/* 0x79	*/
		trap_dd,			/* 0x7a	*/
		trap_dd,			/* 0x7b	*/
		trap_dd,			/* 0x7c	*/
		trap_dd,			/* 0x7d	*/
		op_ldaxd,			/* 0x7e	*/
		trap_dd,			/* 0x7f	*/
		trap_dd,			/* 0x80	*/
		trap_dd,			/* 0x81	*/
		trap_dd,			/* 0x82	*/
		trap_dd,			/* 0x83	*/
		trap_dd,			/* 0x84	*/
		trap_dd,			/* 0x85	*/
		op_adaxd,			/* 0x86	*/
		trap_dd,			/* 0x87	*/
		trap_dd,			/* 0x88	*/
		trap_dd,			/* 0x89	*/
		trap_dd,			/* 0x8a	*/
		trap_dd,			/* 0x8b	*/
		trap_dd,			/* 0x8c	*/
		trap_dd,			/* 0x8d	*/
		op_acaxd,			/* 0x8e	*/
		trap_dd,			/* 0x8f	*/
		trap_dd,			/* 0x90	*/
		trap_dd,			/* 0x91	*/
		trap_dd,			/* 0x92	*/
		trap_dd,			/* 0x93	*/
		trap_dd,			/* 0x94	*/
		trap_dd,			/* 0x95	*/
		op_suaxd,			/* 0x96	*/
		trap_dd,			/* 0x97	*/
		trap_dd,			/* 0x98	*/
		trap_dd,			/* 0x99	*/
		trap_dd,			/* 0x9a	*/
		trap_dd,			/* 0x9b	*/
		trap_dd,			/* 0x9c	*/
		trap_dd,			/* 0x9d	*/
		op_scaxd,			/* 0x9e	*/
		trap_dd,			/* 0x9f	*/
		trap_dd,			/* 0xa0	*/
		trap_dd,			/* 0xa1	*/
		trap_dd,			/* 0xa2	*/
		trap_dd,			/* 0xa3	*/
		trap_dd,			/* 0xa4	*/
		trap_dd,			/* 0xa5	*/
		op_andxd,			/* 0xa6	*/
		trap_dd,			/* 0xa7	*/
		trap_dd,			/* 0xa8	*/
		trap_dd,			/* 0xa9	*/
		trap_dd,			/* 0xaa	*/
		trap_dd,			/* 0xab	*/
		trap_dd,			/* 0xac	*/
		trap_dd,			/* 0xad	*/
		op_xorxd,			/* 0xae	*/
		trap_dd,			/* 0xaf	*/
		trap_dd,			/* 0xb0	*/
		trap_dd,			/* 0xb1	*/
		trap_dd,			/* 0xb2	*/
		trap_dd,			/* 0xb3	*/
		trap_dd,			/* 0xb4	*/
		trap_dd,			/* 0xb5	*/
		op_orxd,			/* 0xb6	*/
		trap_dd,			/* 0xb7	*/
		trap_dd,			/* 0xb8	*/
		trap_dd,			/* 0xb9	*/
		trap_dd,			/* 0xba	*/
		trap_dd,			/* 0xbb	*/
		trap_dd,			/* 0xbc	*/
		trap_dd,			/* 0xbd	*/
		op_cpxd,			/* 0xbe	*/
		trap_dd,			/* 0xbf	*/
		trap_dd,			/* 0xc0	*/
		trap_dd,			/* 0xc1	*/
		trap_dd,			/* 0xc2	*/
		trap_dd,			/* 0xc3	*/
		trap_dd,			/* 0xc4	*/
		trap_dd,			/* 0xc5	*/
		trap_dd,			/* 0xc6	*/
		trap_dd,			/* 0xc7	*/
		trap_dd,			/* 0xc8	*/
		trap_dd,			/* 0xc9	*/
		trap_dd,			/* 0xca	*/
		op_ddcb_handel,			/* 0xcb	*/
		trap_dd,			/* 0xcc	*/
		trap_dd,			/* 0xcd	*/
		trap_dd,			/* 0xce	*/
		trap_dd,			/* 0xcf	*/
		trap_dd,			/* 0xd0	*/
		trap_dd,			/* 0xd1	*/
		trap_dd,			/* 0xd2	*/
		trap_dd,			/* 0xd3	*/
		trap_dd,			/* 0xd4	*/
		trap_dd,			/* 0xd5	*/
		trap_dd,			/* 0xd6	*/
		trap_dd,			/* 0xd7	*/
		trap_dd,			/* 0xd8	*/
		trap_dd,			/* 0xd9	*/
		trap_dd,			/* 0xda	*/
		trap_dd,			/* 0xdb	*/
		trap_dd,			/* 0xdc	*/
		trap_dd,			/* 0xdd	*/
		trap_dd,			/* 0xde	*/
		trap_dd,			/* 0xdf	*/
		trap_dd,			/* 0xe0	*/
		op_popix,			/* 0xe1	*/
		trap_dd,			/* 0xe2	*/
		op_exspx,			/* 0xe3	*/
		trap_dd,			/* 0xe4	*/
		op_pusix,			/* 0xe5	*/
		trap_dd,			/* 0xe6	*/
		trap_dd,			/* 0xe7	*/
		trap_dd,			/* 0xe8	*/
		op_jpix,			/* 0xe9	*/
		trap_dd,			/* 0xea	*/
		trap_dd,			/* 0xeb	*/
		trap_dd,			/* 0xec	*/
		trap_dd,			/* 0xed	*/
		trap_dd,			/* 0xee	*/
		trap_dd,			/* 0xef	*/
		trap_dd,			/* 0xf0	*/
		trap_dd,			/* 0xf1	*/
		trap_dd,			/* 0xf2	*/
		trap_dd,			/* 0xf3	*/
		trap_dd,			/* 0xf4	*/
		trap_dd,			/* 0xf5	*/
		trap_dd,			/* 0xf6	*/
		trap_dd,			/* 0xf7	*/
		trap_dd,			/* 0xf8	*/
		op_ldspx,			/* 0xf9	*/
		trap_dd,			/* 0xfa	*/
		trap_dd,			/* 0xfb	*/
		trap_dd,			/* 0xfc	*/
		trap_dd,			/* 0xfd	*/
		trap_dd,			/* 0xfe	*/
		trap_dd				/* 0xff	*/
	};


#ifdef WANT_TIM
	register long t;
	t = (*op_dd[*PC++]) ();		/* execute next opcode */
#else
	(*op_dd[*PC++]) ();
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
 *	initial 0xdd of a multi byte opcode.
 */
static long trap_dd()
{
	cpu_error = OPTRAP2;
	cpu_state = STOPPED;
#ifdef WANT_TIM
	return(0L);
#endif
}

static long op_popix()		     /*	POP IX */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	IX = *STACK++;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	IX += *STACK++ << 8;
#ifdef WANT_TIM
	return(14L);
#endif
}

static long op_pusix()		     /*	PUSH IX	*/
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = IX >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = IX;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_jpix()		     /*	JP (IX)	*/
{
	PC = ram + IX;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_exspx()		     /*	EX (SP),IX */
{
	register int i;

	i = *STACK + (*(STACK +	1) << 8);
	*STACK = IX;
	*(STACK	+ 1) = IX >> 8;
	IX = i;
#ifdef WANT_TIM
	return(23L);
#endif
}

static long op_ldspx()		     /*	LD SP,IX */
{
	STACK =	ram + IX;
#ifdef WANT_TIM
	return(10L);
#endif
}

static long op_ldixnn()		     /*	LD IX,nn */
{
	IX = *PC++;
	IX += *PC++ << 8;
#ifdef WANT_TIM
	return(14L);
#endif
}

static long op_ldixinn()	     /*	LD IX,(nn) */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	IX = *p++;
	IX += *p << 8;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_ldinx()		     /*	LD (nn),IX */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	*p++ = IX;
	*p = IX	>> 8;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_adaxd()		     /*	ADD A,(IX+d) */
{
	register int i;
	register BYTE P;

	P = *(ram + IX + (char)	*PC++);
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

static long op_acaxd()		     /*	ADC A,(IX+d) */
{
	register int i,	carry;
	register BYTE P;

	carry =	(F & C_FLAG) ? 1 : 0;
	P = *(ram + IX + (char)	*PC++);
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

static long op_suaxd()		     /*	SUB A,(IX+d) */
{
	register int i;
	register BYTE P;

	P = *(ram + IX + (char)	*PC++);
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

static long op_scaxd()		     /*	SBC A,(IX+d) */
{
	register int i,	carry;
	register BYTE P;

	carry =	(F & C_FLAG) ? 1 : 0;
	P = *(ram + IX + (char)	*PC++);
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

static long op_andxd()		     /*	AND (IX+d) */
{
	A &= *(ram + IX	+ (char) *PC++);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_xorxd()		     /*	XOR (IX+d) */
{
	A ^= *(ram + IX	+ (char) *PC++);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_orxd()		     /*	OR (IX+d) */
{
	A |= *(ram + IX	+ (char) *PC++);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_cpxd()		     /*	CP (IX+d) */
{
	register int i;
	register BYTE P;

	P = *(ram + IX + (char)	*PC++);
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

static long op_incxd()		     /*	INC (IX+d) */
{
	register BYTE *p;

	p = ram	+ IX + (char) *PC++;
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

static long op_decxd()		     /*	DEC (IX+d) */
{
	register BYTE *p;

	p = ram	+ IX + (char) *PC++;
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

static long op_addxb()		     /*	ADD IX,BC */
{
	register long i;

	i = (((long)IX) + (((long)B) << 8) + C);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	IX = i;
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_addxd()		     /*	ADD IX,DE */
{
	register long i;

	i = (((long)IX) + (((long)D) << 8) + E);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	IX = i;
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_addxs()		     /*	ADD IX,SP */
{
	register long i;

	i = ((long)IX) + ((long)STACK) - ((long)ram);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	IX = i;
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_addxx()		     /*	ADD IX,IX */
{
	register long i;

	i = (((long)IX) << 1);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	IX = i;
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_incix()		     /*	INC IX */
{
	IX++;
#ifdef WANT_TIM
	return(10L);
#endif
}

static long op_decix()		     /*	DEC IX */
{
	IX--;
#ifdef WANT_TIM
	return(10L);
#endif
}

static long op_ldaxd()		     /*	LD A,(IX+d) */
{
	A = *(IX + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldbxd()		     /*	LD B,(IX+d) */
{
	B = *(IX + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldcxd()		     /*	LD C,(IX+d) */
{
	C = *(IX + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_lddxd()		     /*	LD D,(IX+d) */
{
	D = *(IX + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldexd()		     /*	LD E,(IX+d) */
{
	E = *(IX + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldhxd()		     /*	LD H,(IX+d) */
{
	H = *(IX + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldlxd()		     /*	LD L,(IX+d) */
{
	L = *(IX + (char) *PC++	+ ram);
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldxda()		     /*	LD (IX+d),A */
{
	*(IX + (char) *PC++ + ram) = A;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldxdb()		     /*	LD (IX+d),B */
{
	*(IX + (char) *PC++ + ram) = B;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldxdc()		     /*	LD (IX+d),C */
{
	*(IX + (char) *PC++ + ram) = C;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldxdd()		     /*	LD (IX+d),D */
{
	*(IX + (char) *PC++ + ram) = D;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldxde()		     /*	LD (IX+d),E */
{
	*(IX + (char) *PC++ + ram) = E;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldxdh()		     /*	LD (IX+d),H */
{
	*(IX + (char) *PC++ + ram) = H;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldxdl()		     /*	LD (IX+d),L */
{
	*(IX + (char) *PC++ + ram) = L;
#ifdef WANT_TIM
	return(19L);
#endif
}

static long op_ldxdn()		     /*	LD (IX+d),n */
{
	register int d;

	d = (char) *PC++;
	*(IX + d + ram)	= *PC++;
#ifdef WANT_TIM
	return(19L);
#endif
}
