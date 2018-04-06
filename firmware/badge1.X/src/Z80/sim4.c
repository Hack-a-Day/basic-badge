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
 *	starting with 0xed
 */

#include "sim.h"
#include "simglb.h"
#include "fdefs.h"
long op_ed_handel()
{
	long trap_ed();
	long op_im0(), op_im1(), op_im2();
	long op_reti(),	op_retn();
	long op_neg();
	long op_inaic(), op_inbic(), op_incic(), op_indic(), op_ineic();
	long op_inhic(), op_inlic();
	long op_outca(), op_outcb(), op_outcc(), op_outcd(), op_outce();
	long op_outch(), op_outcl();
	long op_ini(), op_inir(), op_ind(), op_indr();
	long op_outi(),	op_otir(), op_outd(), op_otdr();
	long op_ldai(),	op_ldar(), op_ldia(), op_ldra();
	long op_ldbcinn(), op_lddeinn(), op_ldspinn();
	long op_ldinbc(), op_ldinde(), op_ldinsp();
	long op_adchb(), op_adchd(), op_adchh(), op_adchs();
	long op_sbchb(), op_sbchd(), op_sbchh(), op_sbchs();
	long op_ldi(), op_ldir(), op_ldd(), op_lddr();
	long op_cpi(), op_cpir(), op_cpdop(), op_cpdr();
	long op_oprld(), op_oprrd();

	static long (*op_ed[256]) () = {
		trap_ed,			/* 0x00	*/
		trap_ed,			/* 0x01	*/
		trap_ed,			/* 0x02	*/
		trap_ed,			/* 0x03	*/
		trap_ed,			/* 0x04	*/
		trap_ed,			/* 0x05	*/
		trap_ed,			/* 0x06	*/
		trap_ed,			/* 0x07	*/
		trap_ed,			/* 0x08	*/
		trap_ed,			/* 0x09	*/
		trap_ed,			/* 0x0a	*/
		trap_ed,			/* 0x0b	*/
		trap_ed,			/* 0x0c	*/
		trap_ed,			/* 0x0d	*/
		trap_ed,			/* 0x0e	*/
		trap_ed,			/* 0x0f	*/
		trap_ed,			/* 0x10	*/
		trap_ed,			/* 0x11	*/
		trap_ed,			/* 0x12	*/
		trap_ed,			/* 0x13	*/
		trap_ed,			/* 0x14	*/
		trap_ed,			/* 0x15	*/
		trap_ed,			/* 0x16	*/
		trap_ed,			/* 0x17	*/
		trap_ed,			/* 0x18	*/
		trap_ed,			/* 0x19	*/
		trap_ed,			/* 0x1a	*/
		trap_ed,			/* 0x1b	*/
		trap_ed,			/* 0x1c	*/
		trap_ed,			/* 0x1d	*/
		trap_ed,			/* 0x1e	*/
		trap_ed,			/* 0x1f	*/
		trap_ed,			/* 0x20	*/
		trap_ed,			/* 0x21	*/
		trap_ed,			/* 0x22	*/
		trap_ed,			/* 0x23	*/
		trap_ed,			/* 0x24	*/
		trap_ed,			/* 0x25	*/
		trap_ed,			/* 0x26	*/
		trap_ed,			/* 0x27	*/
		trap_ed,			/* 0x28	*/
		trap_ed,			/* 0x29	*/
		trap_ed,			/* 0x2a	*/
		trap_ed,			/* 0x2b	*/
		trap_ed,			/* 0x2c	*/
		trap_ed,			/* 0x2d	*/
		trap_ed,			/* 0x2e	*/
		trap_ed,			/* 0x2f	*/
		trap_ed,			/* 0x30	*/
		trap_ed,			/* 0x31	*/
		trap_ed,			/* 0x32	*/
		trap_ed,			/* 0x33	*/
		trap_ed,			/* 0x34	*/
		trap_ed,			/* 0x35	*/
		trap_ed,			/* 0x36	*/
		trap_ed,			/* 0x37	*/
		trap_ed,			/* 0x38	*/
		trap_ed,			/* 0x39	*/
		trap_ed,			/* 0x3a	*/
		trap_ed,			/* 0x3b	*/
		trap_ed,			/* 0x3c	*/
		trap_ed,			/* 0x3d	*/
		trap_ed,			/* 0x3e	*/
		trap_ed,			/* 0x3f	*/
		op_inbic,			/* 0x40	*/
		op_outcb,			/* 0x41	*/
		op_sbchb,			/* 0x42	*/
		op_ldinbc,			/* 0x43	*/
		op_neg,				/* 0x44	*/
		op_retn,			/* 0x45	*/
		op_im0,				/* 0x46	*/
		op_ldia,			/* 0x47	*/
		op_incic,			/* 0x48	*/
		op_outcc,			/* 0x49	*/
		op_adchb,			/* 0x4a	*/
		op_ldbcinn,			/* 0x4b	*/
		trap_ed,			/* 0x4c	*/
		op_reti,			/* 0x4d	*/
		trap_ed,			/* 0x4e	*/
		op_ldra,			/* 0x4f	*/
		op_indic,			/* 0x50	*/
		op_outcd,			/* 0x51	*/
		op_sbchd,			/* 0x52	*/
		op_ldinde,			/* 0x53	*/
		trap_ed,			/* 0x54	*/
		trap_ed,			/* 0x55	*/
		op_im1,				/* 0x56	*/
		op_ldai,			/* 0x57	*/
		op_ineic,			/* 0x58	*/
		op_outce,			/* 0x59	*/
		op_adchd,			/* 0x5a	*/
		op_lddeinn,			/* 0x5b	*/
		trap_ed,			/* 0x5c	*/
		trap_ed,			/* 0x5d	*/
		op_im2,				/* 0x5e	*/
		op_ldar,			/* 0x5f	*/
		op_inhic,			/* 0x60	*/
		op_outch,			/* 0x61	*/
		op_sbchh,			/* 0x62	*/
		trap_ed,			/* 0x63	*/
		trap_ed,			/* 0x64	*/
		trap_ed,			/* 0x65	*/
		trap_ed,			/* 0x66	*/
		op_oprrd,			/* 0x67	*/
		op_inlic,			/* 0x68	*/
		op_outcl,			/* 0x69	*/
		op_adchh,			/* 0x6a	*/
		trap_ed,			/* 0x6b	*/
		trap_ed,			/* 0x6c	*/
		trap_ed,			/* 0x6d	*/
		trap_ed,			/* 0x6e	*/
		op_oprld,			/* 0x6f	*/
		trap_ed,			/* 0x70	*/
		trap_ed,			/* 0x71	*/
		op_sbchs,			/* 0x72	*/
		op_ldinsp,			/* 0x73	*/
		trap_ed,			/* 0x74	*/
		trap_ed,			/* 0x75	*/
		trap_ed,			/* 0x76	*/
		trap_ed,			/* 0x77	*/
		op_inaic,			/* 0x78	*/
		op_outca,			/* 0x79	*/
		op_adchs,			/* 0x7a	*/
		op_ldspinn,			/* 0x7b	*/
		trap_ed,			/* 0x7c	*/
		trap_ed,			/* 0x7d	*/
		trap_ed,			/* 0x7e	*/
		trap_ed,			/* 0x7f	*/
		trap_ed,			/* 0x80	*/
		trap_ed,			/* 0x81	*/
		trap_ed,			/* 0x82	*/
		trap_ed,			/* 0x83	*/
		trap_ed,			/* 0x84	*/
		trap_ed,			/* 0x85	*/
		trap_ed,			/* 0x86	*/
		trap_ed,			/* 0x87	*/
		trap_ed,			/* 0x88	*/
		trap_ed,			/* 0x89	*/
		trap_ed,			/* 0x8a	*/
		trap_ed,			/* 0x8b	*/
		trap_ed,			/* 0x8c	*/
		trap_ed,			/* 0x8d	*/
		trap_ed,			/* 0x8e	*/
		trap_ed,			/* 0x8f	*/
		trap_ed,			/* 0x90	*/
		trap_ed,			/* 0x91	*/
		trap_ed,			/* 0x92	*/
		trap_ed,			/* 0x93	*/
		trap_ed,			/* 0x94	*/
		trap_ed,			/* 0x95	*/
		trap_ed,			/* 0x96	*/
		trap_ed,			/* 0x97	*/
		trap_ed,			/* 0x98	*/
		trap_ed,			/* 0x99	*/
		trap_ed,			/* 0x9a	*/
		trap_ed,			/* 0x9b	*/
		trap_ed,			/* 0x9c	*/
		trap_ed,			/* 0x9d	*/
		trap_ed,			/* 0x9e	*/
		trap_ed,			/* 0x9f	*/
		op_ldi,				/* 0xa0	*/
		op_cpi,				/* 0xa1	*/
		op_ini,				/* 0xa2	*/
		op_outi,			/* 0xa3	*/
		trap_ed,			/* 0xa4	*/
		trap_ed,			/* 0xa5	*/
		trap_ed,			/* 0xa6	*/
		trap_ed,			/* 0xa7	*/
		op_ldd,				/* 0xa8	*/
		op_cpdop,			/* 0xa9	*/
		op_ind,				/* 0xaa	*/
		op_outd,			/* 0xab	*/
		trap_ed,			/* 0xac	*/
		trap_ed,			/* 0xad	*/
		trap_ed,			/* 0xae	*/
		trap_ed,			/* 0xaf	*/
		op_ldir,			/* 0xb0	*/
		op_cpir,			/* 0xb1	*/
		op_inir,			/* 0xb2	*/
		op_otir,			/* 0xb3	*/
		trap_ed,			/* 0xb4	*/
		trap_ed,			/* 0xb5	*/
		trap_ed,			/* 0xb6	*/
		trap_ed,			/* 0xb7	*/
		op_lddr,			/* 0xb8	*/
		op_cpdr,			/* 0xb9	*/
		op_indr,			/* 0xba	*/
		op_otdr,			/* 0xbb	*/
		trap_ed,			/* 0xbc	*/
		trap_ed,			/* 0xbd	*/
		trap_ed,			/* 0xbe	*/
		trap_ed,			/* 0xbf	*/
		trap_ed,			/* 0xc0	*/
		trap_ed,			/* 0xc1	*/
		trap_ed,			/* 0xc2	*/
		trap_ed,			/* 0xc3	*/
		trap_ed,			/* 0xc4	*/
		trap_ed,			/* 0xc5	*/
		trap_ed,			/* 0xc6	*/
		trap_ed,			/* 0xc7	*/
		trap_ed,			/* 0xc8	*/
		trap_ed,			/* 0xc9	*/
		trap_ed,			/* 0xca	*/
		trap_ed,			/* 0xcb	*/
		trap_ed,			/* 0xcc	*/
		trap_ed,			/* 0xcd	*/
		trap_ed,			/* 0xce	*/
		trap_ed,			/* 0xcf	*/
		trap_ed,			/* 0xd0	*/
		trap_ed,			/* 0xd1	*/
		trap_ed,			/* 0xd2	*/
		trap_ed,			/* 0xd3	*/
		trap_ed,			/* 0xd4	*/
		trap_ed,			/* 0xd5	*/
		trap_ed,			/* 0xd6	*/
		trap_ed,			/* 0xd7	*/
		trap_ed,			/* 0xd8	*/
		trap_ed,			/* 0xd9	*/
		trap_ed,			/* 0xda	*/
		trap_ed,			/* 0xdb	*/
		trap_ed,			/* 0xdc	*/
		trap_ed,			/* 0xdd	*/
		trap_ed,			/* 0xde	*/
		trap_ed,			/* 0xdf	*/
		trap_ed,			/* 0xe0	*/
		trap_ed,			/* 0xe1	*/
		trap_ed,			/* 0xe2	*/
		trap_ed,			/* 0xe3	*/
		trap_ed,			/* 0xe4	*/
		trap_ed,			/* 0xe5	*/
		trap_ed,			/* 0xe6	*/
		trap_ed,			/* 0xe7	*/
		trap_ed,			/* 0xe8	*/
		trap_ed,			/* 0xe9	*/
		trap_ed,			/* 0xea	*/
		trap_ed,			/* 0xeb	*/
		trap_ed,			/* 0xec	*/
		trap_ed,			/* 0xed	*/
		trap_ed,			/* 0xee	*/
		trap_ed,			/* 0xef	*/
		trap_ed,			/* 0xf0	*/
		trap_ed,			/* 0xf1	*/
		trap_ed,			/* 0xf2	*/
		trap_ed,			/* 0xf3	*/
		trap_ed,			/* 0xf4	*/
		trap_ed,			/* 0xf5	*/
		trap_ed,			/* 0xf6	*/
		trap_ed,			/* 0xf7	*/
		trap_ed,			/* 0xf8	*/
		trap_ed,			/* 0xf9	*/
		trap_ed,			/* 0xfa	*/
		trap_ed,			/* 0xfb	*/
		trap_ed,			/* 0xfc	*/
		trap_ed,			/* 0xfd	*/
		trap_ed,			/* 0xfe	*/
		trap_ed				/* 0xff	*/
	};

#ifdef WANT_TIM
	register long t;
	t = (*op_ed[*PC++]) ();		/* execute next opcode */
#else
	(*op_ed[*PC++]) ();
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
 *	initial 0xed of a multi byte opcode.
 */
static long trap_ed()
{
	cpu_error = OPTRAP2;
	cpu_state = STOPPED;
#ifdef WANT_TIM
	return(0L);
#endif
}

static long op_im0()		     /*	IM 0 */
{
	int_mode = 0;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_im1()		     /*	IM 1 */
{
	int_mode = 1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_im2()		     /*	IM 2 */
{
	int_mode = 2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_reti()		     /*	RETI */
{
	register unsigned i;

	i = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
	i += *STACK++ << 8;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
	PC = ram + i;
#ifdef WANT_TIM
	return(14L);
#endif
}

static long op_retn()		     /*	RETN */
{
	register unsigned i;

	i = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
	i += *STACK++ << 8;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
	PC = ram + i;
	(IFF & 2) ? (IFF |= 1) : (IFF &= ~1);
#ifdef WANT_TIM
	return(14L);
#endif
}

static long op_neg()		     /*	NEG */
{
	(A) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	(A == 0x80) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(0 - ((char) A & 0xf) <	0) ? (F	|= H_FLAG) : (F	&= ~H_FLAG);
	A = 0 -	A;
	F |= N_FLAG;
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_inaic()		     /*	IN A,(C) */
{
	BYTE io_in();

	A = io_in(C);
	F &= ~(N_FLAG |	H_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_inbic()		     /*	IN B,(C) */
{
	BYTE io_in();

	B = io_in(C);
	F &= ~(N_FLAG |	H_FLAG);
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[B]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_incic()		     /*	IN C,(C) */
{
	BYTE io_in();

	C = io_in(C);
	F &= ~(N_FLAG |	H_FLAG);
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[C]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_indic()		     /*	IN D,(C) */
{
	BYTE io_in();

	D = io_in(C);
	F &= ~(N_FLAG |	H_FLAG);
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[D]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_ineic()		     /*	IN E,(C) */
{
	BYTE io_in();

	E = io_in(C);
	F &= ~(N_FLAG |	H_FLAG);
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[E]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_inhic()		     /*	IN H,(C) */
{
	BYTE io_in();

	H = io_in(C);
	F &= ~(N_FLAG |	H_FLAG);
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[H]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_inlic()		     /*	IN L,(C) */
{
	BYTE io_in();

	L = io_in(C);
	F &= ~(N_FLAG |	H_FLAG);
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[L]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_outca()		     /*	OUT (C),A */
{
	BYTE io_out();

	io_out(C, A);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_outcb()		     /*	OUT (C),B */
{
	BYTE io_out();

	io_out(C, B);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_outcc()		     /*	OUT (C),C */
{
	BYTE io_out();

	io_out(C, C);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_outcd()		     /*	OUT (C),D */
{
	BYTE io_out();

	io_out(C, D);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_outce()		     /*	OUT (C),E */
{
	BYTE io_out();

	io_out(C, E);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_outch()		     /*	OUT (C),H */
{
	BYTE io_out();

	io_out(C, H);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_outcl()		     /*	OUT (C),L */
{
	BYTE io_out();

	io_out(C, L);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_ini()		     /*	INI */
{
	BYTE io_in();

	*(ram +	(H << 8) + L) =	io_in(C);
	L++;
	if (!L)
		H++;
	B--;
	F |= N_FLAG;
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(16L);
#endif
}

static long op_inir()		     /*	INIR */
{
#ifdef WANT_TIM
	register long t	= -21L;
#endif
	register BYTE *d;
	BYTE io_in();

	d = ram	+ (H <<	8) + L;
	do {
		*d++ = io_in(C);
		B--;
#ifdef WANT_TIM
		t += 21L;
#endif
	} while	(B);
	H = (d - ram) >> 8;
	L = d -	ram;
	F |= N_FLAG | Z_FLAG;
#ifdef WANT_TIM
	return(t + 16L);
#endif
}

static long op_ind()		     /*	IND */
{
	BYTE io_in();

	*(ram +	(H << 8) + L) =	io_in(C);
	L--;
	if (L == 0xff)
		H--;
	B--;
	F |= N_FLAG;
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(16L);
#endif
}

static long op_indr()		     /*	INDR */
{
#ifdef WANT_TIM
	register long t	= -21L;
#endif
	register BYTE *d;
	BYTE io_in();

	d = ram	+ (H <<	8) + L;
	do {
		*d-- = io_in(C);
		B--;
#ifdef WANT_TIM
		t += 21L;
#endif
	} while	(B);
	H = (d - ram) >> 8;
	L = d -	ram;
	F |= N_FLAG | Z_FLAG;
#ifdef WANT_TIM
	return(t + 16L);
#endif
}

static long op_outi()		     /*	OUTI */
{
	BYTE io_out();

	io_out(C, *(ram	+ (H <<	8) * L));
	L++;
	if (!L)
		H++;
	B--;
	F |= N_FLAG;
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(16L);
#endif
}

static long op_otir()		     /*	OTIR */
{
#ifdef WANT_TIM
	register long t	= -21L;
#endif
	register BYTE *d;
	BYTE io_out();

	d = ram	+ (H <<	8) + L;
	do {
		io_out(C, *d++);
		B--;
#ifdef WANT_TIM
		t += 21L;
#endif
	} while	(B);
	H = (d - ram) >> 8;
	L = d -	ram;
	F |= N_FLAG | Z_FLAG;
#ifdef WANT_TIM
	return(t + 16L);
#endif
}

static long op_outd()		     /*	OUTD */
{
	BYTE io_out();

	io_out(C, *(ram	+ (H <<	8) * L));
	L--;
	if (L == 0xff)
		H--;
	B--;
	F |= N_FLAG;
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(16L);
#endif
}

static long op_otdr()		     /*	OTDR */
{
#ifdef WANT_TIM
	register long t	= -21L;
#endif
	register BYTE *d;
	BYTE io_out();

	d = ram	+ (H <<	8) + L;
	do {
		io_out(C, *d--);
		B--;
#ifdef WANT_TIM
		t += 21L;
#endif
	} while	(B);
	H = (d - ram) >> 8;
	L = d -	ram;
	F |= N_FLAG | Z_FLAG;
#ifdef WANT_TIM
	return(t + 16L);
#endif
}

static long op_ldai()		     /*	LD A,I */
{
	A = I;
	F &= ~(N_FLAG |	H_FLAG);
	(IFF & 2) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(9L);
#endif
}

static long op_ldar()		     /*	LD A,R */
{
	A = (BYTE) R;
	F &= ~(N_FLAG |	H_FLAG);
	(IFF & 2) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(9L);
#endif
}

static long op_ldia()		     /*	LD I,A */
{
	I = A;
#ifdef WANT_TIM
	return(9L);
#endif
}

static long op_ldra()		     /*	LD R,A */
{
	R = A;
#ifdef WANT_TIM
	return(9L);
#endif
}

static long op_ldbcinn()	     /*	LD BC,(nn) */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	C = *p++;
	B = *p;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_lddeinn()	     /*	LD DE,(nn) */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	E = *p++;
	D = *p;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_ldspinn()	     /*	LD SP,(nn) */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	STACK =	ram + *p++;
	STACK += *p << 8;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_ldinbc()		     /*	LD (nn),BC */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	*p++ = C;
	*p = B;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_ldinde()		     /*	LD (nn),DE */
{
	register BYTE *p;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	*p++ = E;
	*p = D;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_ldinsp()		     /*	LD (nn),SP */
{
	register BYTE *p;
	register int i;

	p = ram	+ *PC++;
	p += *PC++ << 8;
	i = STACK - ram;
	*p++ = i;
	*p = i >> 8;
#ifdef WANT_TIM
	return(20L);
#endif
}

static long op_adchb()		     /*	ADC HL,BC */
{
	register int carry;
	register WORD hl, bc;
	register long i;

	carry =	(F & C_FLAG) ? 1 : 0;
	hl = (H	<< 8) +	L;
	bc = (B	<< 8) +	C;
	i = ((long)hl) + ((long)bc) + carry;
	((hl < 0x8000) && (i > 0x7fffL)) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	H = i >> 8;
	L = i;
	F &= ~N_FLAG;
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_adchd()		     /*	ADC HL,DE */
{
	register int carry;
	register WORD hl, de;
	register long i;

	carry =	(F & C_FLAG) ? 1 : 0;
	hl = (H	<< 8) +	L;
	de = (D	<< 8) +	E;
	i = ((long)hl) + ((long)de) + carry;
	((hl < 0x8000) && (i > 0x7fffL)) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	H = i >> 8;
	L = i;
	F &= ~N_FLAG;
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_adchh()		     /*	ADC HL,HL */
{
	register int carry;
	register WORD hl;
	register long i;

	carry =	(F & C_FLAG) ? 1 : 0;
	hl = (H	<< 8) +	L;
	i = ((((long)hl) << 1) + carry);
	((hl < 0x8000) && (i > 0x7fffL)) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	H = i >> 8;
	L = i;
	F &= ~N_FLAG;
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_adchs()		     /*	ADC HL,SP */
{
	register int carry;
	register WORD hl, sp;
	register long i;

	carry =	(F & C_FLAG) ? 1 : 0;
	hl = (H	<< 8) +	L;
	sp = STACK - ram;
	i = ((long)hl) + ((long)sp) + carry;
	((hl < 0x8000) && (i > 0x7fffL)) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	H = i >> 8;
	L = i;
	F &= ~N_FLAG;
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sbchb()		     /*	SBC HL,BC */
{
	register int carry;
	register WORD hl, bc;
	register long i;

	carry =	(F & C_FLAG) ? 1 : 0;
	hl = (H	<< 8) +	L;
	bc = (B	<< 8) +	C;
	i = ((long)hl) - ((long)bc) - carry;
	((hl > 0x7fff) && (i < 0x8000L)) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(i < 0L) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	H = i >> 8;
	L = i;
	F |= N_FLAG;
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sbchd()		     /*	SBC HL,DE */
{
	register int carry;
	register WORD hl, de;
	register long i;

	carry =	(F & C_FLAG) ? 1 : 0;
	hl = (H	<< 8) +	L;
	de = (D	<< 8) +	E;
	i = ((long)hl) - ((long)de) - carry;
	((hl > 0x7fff) && (i < 0x8000L)) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(i < 0L) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	H = i >> 8;
	L = i;
	F |= N_FLAG;
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sbchh()		     /*	SBC HL,HL */
{
	if (F &	C_FLAG)	{
		F |= S_FLAG | P_FLAG | N_FLAG |	C_FLAG;
		F &= ~Z_FLAG;
		H = L =	255;
	} else {
		F |= Z_FLAG | N_FLAG;
		F &= ~(S_FLAG |	P_FLAG | C_FLAG);
		H = L =	0;
	}
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sbchs()		     /*	SBC HL,SP */
{
	register int carry;
	register WORD hl, sp;
	register long i;

	carry =	(F & C_FLAG) ? 1 : 0;
	hl = (H	<< 8) +	L;
	sp = STACK - ram;
	i = ((long)hl) - ((long)sp) - carry;
	((hl > 0x7fff) && (i < 0x8000L)) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(i < 0L) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	H = i >> 8;
	L = i;
	F |= N_FLAG;
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_ldi()		     /*	LDI */
{
	*(ram +	(D << 8) + E) =	*(ram +	(H << 8) + L);
	E++;
	if (!E)
		D++;
	L++;
	if (!L)
		H++;
	C--;
	if (C == 0xff)
		B--;
	(B | C)	? (F |=	P_FLAG)	: (F &=	~P_FLAG);
	F &= ~(N_FLAG |	H_FLAG);
#ifdef WANT_TIM
	return(16L);
#endif
}

static long op_ldir()		     /*	LDIR */
{
#ifdef WANT_TIM
	register long t	= -21L;
#endif
	register WORD i;
	register BYTE *s, *d;

	i = (B << 8) + C;
	d = ram	+ (D <<	8) + E;
	s = ram	+ (H <<	8) + L;
	do {
		*d++ = *s++;
#ifdef WANT_TIM
		t += 21L;
#endif
	} while	(--i);
	B = C =	0;
	D = (d - ram) >> 8;
	E = d -	ram;
	H = (s - ram) >> 8;
	L = s -	ram;
	F &= ~(N_FLAG |	P_FLAG | H_FLAG);
#ifdef WANT_TIM
	return(t + 16L);
#endif
}

static long op_ldd()		     /*	LDD */
{
	*(ram +	(D << 8) + E) =	*(ram +	(H << 8) + L);
	E--;
	if (E == 0xff)
		D--;
	L--;
	if (L == 0xff)
		H--;
	C--;
	if (C == 0xff)
		B--;
	(B | C)	? (F |=	P_FLAG)	: (F &=	~P_FLAG);
	F &= ~(N_FLAG |	H_FLAG);
#ifdef WANT_TIM
	return(16L);
#endif
}

static long op_lddr()		     /*	LDDR */
{
#ifdef WANT_TIM
	register long t	= -21L;
#endif
	register WORD i;
	register BYTE *s, *d;

	i = (B << 8) + C;
	d = ram	+ (D <<	8) + E;
	s = ram	+ (H <<	8) + L;
	do {
		*d-- = *s--;
#ifdef WANT_TIM
		t += 21L;
#endif
	} while	(--i);
	B = C =	0;
	D = (d - ram) >> 8;
	E = d -	ram;
	H = (s - ram) >> 8;
	L = s -	ram;
	F &= ~(N_FLAG |	P_FLAG | H_FLAG);
#ifdef WANT_TIM
	return(t + 16L);
#endif
}

static long op_cpi()		     /*	CPI */
{			/* H flag isn't set here ! */
	register BYTE i;

	i = A -	*(ram +	(H << 8) + L);
	L++;
	if (!L)
		H++;
	C--;
	if (C == 0xff)
		B--;
	F |= N_FLAG;
	(B | C)	? (F |=	P_FLAG)	: (F &=	~P_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(16L);
#endif
}

static long op_cpir()		     /*	CPIR */
{			/* H flag isn't set here ! */
#ifdef WANT_TIM
	register long t	= -21L;
#endif
	register BYTE *s;
	register BYTE d;
	register WORD i;

	i = (B << 8) + C;
	s = ram	+ (H <<	8) + L;
	do {
		d = A -	*s++;
#ifdef WANT_TIM
		t += 21L;
#endif
	} while	(--i &&	d);
	F |= N_FLAG;
	B = i >> 8;
	C = i;
	H = (s - ram) >> 8;
	L = s -	ram;
	(i) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(d) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(d & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(t + 16L);
#endif
}

static long op_cpdop()		     /*	CPD */
{			/* H flag isn't set here ! */
	register BYTE i;

	i = A -	*(ram +	(H << 8) + L);
	L--;
	if (L == 0xff)
		H--;
	C--;
	if (C == 0xff)
		B--;
	F |= N_FLAG;
	(B | C)	? (F |=	P_FLAG)	: (F &=	~P_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(16L);
#endif
}

static long op_cpdr()		     /*	CPDR */
{			/* H flag isn't set here ! */
#ifdef WANT_TIM
	register long t	= -21L;
#endif
	register BYTE *s;
	register BYTE d;
	register WORD i;

	i = (B << 8) + C;
	s = ram	+ (H <<	8) + L;
	do {
		d = A -	*s--;
#ifdef WANT_TIM
		t += 21L;
#endif
	} while	(--i &&	d);
	F |= N_FLAG;
	B = i >> 8;
	C = i;
	H = (s - ram) >> 8;
	L = s -	ram;
	(i) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(d) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(d & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
#ifdef WANT_TIM
	return(t + 16L);
#endif
}

static long op_oprld()		     /*	RLD (HL) */
{
	register int i,	j;

	i = *(ram + (H << 8) + L);
	j = A &	0x0f;
	A = (A & 0xf0) | (i >> 4);
	i = (i << 4) | j;
	*(ram +	(H << 8) + L) =	i;
	F &= ~(H_FLAG |	N_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(18L);
#endif
}

static long op_oprrd()		     /*	RRD (HL) */
{
	register int i,	j;

	i = *(ram + (H << 8) + L);
	j = A &	0x0f;
	A = (A & 0xf0) | (i & 0x0f);
	i = (i >> 4) | (j << 4);
	*(ram +	(H << 8) + L) =	i;
	F &= ~(H_FLAG |	N_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(18L);
#endif
}

