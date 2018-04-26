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
 *	starting with 0xcb
 */

#include "sim.h"
#include "simglb.h"
#include "fdefs.h"

long op_cb_handel()
{
	long trap_cb();
	long op_srla(),	op_srlb(), op_srlc(), op_srld(), op_srle();
	long op_srlh(),	op_srll(), op_srlhl();
	long op_slaa(),	op_slab(), op_slac(), op_slad(), op_slae();
	long op_slah(),	op_slal(), op_slahl();
	long op_rlra(),	op_rlb(), op_rlc(), op_rld(), op_rle();
	long op_rlh(), op_rll(), op_rlhl();
	long op_rrra(),	op_rrb(), op_rrc(), op_rrd(), op_rre();
	long op_rrh(), op_rrl(), op_rrhl();
	long op_rrcra(), op_rrcb(), op_rrcc(), op_rrcd(), op_rrce();
	long op_rrch(),	op_rrcl(), op_rrchl();
	long op_rlcra(), op_rlcb(), op_rlcc(), op_rlcd(), op_rlce();
	long op_rlch(),	op_rlcl(), op_rlchl();
	long op_sraa(),	op_srab(), op_srac(), op_srad(), op_srae();
	long op_srah(),	op_sral(), op_srahl();
	long op_sb0a(),	op_sb1a(), op_sb2a(), op_sb3a();
	long op_sb4a(),	op_sb5a(), op_sb6a(), op_sb7a();
	long op_sb0b(),	op_sb1b(), op_sb2b(), op_sb3b();
	long op_sb4b(),	op_sb5b(), op_sb6b(), op_sb7b();
	long op_sb0c(),	op_sb1c(), op_sb2c(), op_sb3c();
	long op_sb4c(),	op_sb5c(), op_sb6c(), op_sb7c();
	long op_sb0d(),	op_sb1d(), op_sb2d(), op_sb3d();
	long op_sb4d(),	op_sb5d(), op_sb6d(), op_sb7d();
	long op_sb0e(),	op_sb1e(), op_sb2e(), op_sb3e();
	long op_sb4e(),	op_sb5e(), op_sb6e(), op_sb7e();
	long op_sb0h(),	op_sb1h(), op_sb2h(), op_sb3h();
	long op_sb4h(),	op_sb5h(), op_sb6h(), op_sb7h();
	long op_sb0l(),	op_sb1l(), op_sb2l(), op_sb3l();
	long op_sb4l(),	op_sb5l(), op_sb6l(), op_sb7l();
	long op_sb0hl(), op_sb1hl(), op_sb2hl(), op_sb3hl();
	long op_sb4hl(), op_sb5hl(), op_sb6hl(), op_sb7hl();
	long op_rb0a(),	op_rb1a(), op_rb2a(), op_rb3a();
	long op_rb4a(),	op_rb5a(), op_rb6a(), op_rb7a();
	long op_rb0b(),	op_rb1b(), op_rb2b(), op_rb3b();
	long op_rb4b(),	op_rb5b(), op_rb6b(), op_rb7b();
	long op_rb0c(),	op_rb1c(), op_rb2c(), op_rb3c();
	long op_rb4c(),	op_rb5c(), op_rb6c(), op_rb7c();
	long op_rb0d(),	op_rb1d(), op_rb2d(), op_rb3d();
	long op_rb4d(),	op_rb5d(), op_rb6d(), op_rb7d();
	long op_rb0e(),	op_rb1e(), op_rb2e(), op_rb3e();
	long op_rb4e(),	op_rb5e(), op_rb6e(), op_rb7e();
	long op_rb0h(),	op_rb1h(), op_rb2h(), op_rb3h();
	long op_rb4h(),	op_rb5h(), op_rb6h(), op_rb7h();
	long op_rb0l(),	op_rb1l(), op_rb2l(), op_rb3l();
	long op_rb4l(),	op_rb5l(), op_rb6l(), op_rb7l();
	long op_rb0hl(), op_rb1hl(), op_rb2hl(), op_rb3hl();
	long op_rb4hl(), op_rb5hl(), op_rb6hl(), op_rb7hl();
	long op_tb0a(),	op_tb1a(), op_tb2a(), op_tb3a();
	long op_tb4a(),	op_tb5a(), op_tb6a(), op_tb7a();
	long op_tb0b(),	op_tb1b(), op_tb2b(), op_tb3b();
	long op_tb4b(),	op_tb5b(), op_tb6b(), op_tb7b();
	long op_tb0c(),	op_tb1c(), op_tb2c(), op_tb3c();
	long op_tb4c(),	op_tb5c(), op_tb6c(), op_tb7c();
	long op_tb0d(),	op_tb1d(), op_tb2d(), op_tb3d();
	long op_tb4d(),	op_tb5d(), op_tb6d(), op_tb7d();
	long op_tb0e(),	op_tb1e(), op_tb2e(), op_tb3e();
	long op_tb4e(),	op_tb5e(), op_tb6e(), op_tb7e();
	long op_tb0h(),	op_tb1h(), op_tb2h(), op_tb3h();
	long op_tb4h(),	op_tb5h(), op_tb6h(), op_tb7h();
	long op_tb0l(),	op_tb1l(), op_tb2l(), op_tb3l();
	long op_tb4l(),	op_tb5l(), op_tb6l(), op_tb7l();
	long op_tb0hl(), op_tb1hl(), op_tb2hl(), op_tb3hl();
	long op_tb4hl(), op_tb5hl(), op_tb6hl(), op_tb7hl();

	static long (*op_cb[256]) () = {
		op_rlcb,			/* 0x00	*/
		op_rlcc,			/* 0x01	*/
		op_rlcd,			/* 0x02	*/
		op_rlce,			/* 0x03	*/
		op_rlch,			/* 0x04	*/
		op_rlcl,			/* 0x05	*/
		op_rlchl,			/* 0x06	*/
		op_rlcra,			/* 0x07	*/
		op_rrcb,			/* 0x08	*/
		op_rrcc,			/* 0x09	*/
		op_rrcd,			/* 0x0a	*/
		op_rrce,			/* 0x0b	*/
		op_rrch,			/* 0x0c	*/
		op_rrcl,			/* 0x0d	*/
		op_rrchl,			/* 0x0e	*/
		op_rrcra,			/* 0x0f	*/
		op_rlb,				/* 0x10	*/
		op_rlc,				/* 0x11	*/
		op_rld,				/* 0x12	*/
		op_rle,				/* 0x13	*/
		op_rlh,				/* 0x14	*/
		op_rll,				/* 0x15	*/
		op_rlhl,			/* 0x16	*/
		op_rlra,			/* 0x17	*/
		op_rrb,				/* 0x18	*/
		op_rrc,				/* 0x19	*/
		op_rrd,				/* 0x1a	*/
		op_rre,				/* 0x1b	*/
		op_rrh,				/* 0x1c	*/
		op_rrl,				/* 0x1d	*/
		op_rrhl,			/* 0x1e	*/
		op_rrra,			/* 0x1f	*/
		op_slab,			/* 0x20	*/
		op_slac,			/* 0x21	*/
		op_slad,			/* 0x22	*/
		op_slae,			/* 0x23	*/
		op_slah,			/* 0x24	*/
		op_slal,			/* 0x25	*/
		op_slahl,			/* 0x26	*/
		op_slaa,			/* 0x27	*/
		op_srab,			/* 0x28	*/
		op_srac,			/* 0x29	*/
		op_srad,			/* 0x2a	*/
		op_srae,			/* 0x2b	*/
		op_srah,			/* 0x2c	*/
		op_sral,			/* 0x2d	*/
		op_srahl,			/* 0x2e	*/
		op_sraa,			/* 0x2f	*/
		trap_cb,			/* 0x30	*/
		trap_cb,			/* 0x31	*/
		trap_cb,			/* 0x32	*/
		trap_cb,			/* 0x33	*/
		trap_cb,			/* 0x34	*/
		trap_cb,			/* 0x35	*/
		trap_cb,			/* 0x36	*/
		trap_cb,			/* 0x37	*/
		op_srlb,			/* 0x38	*/
		op_srlc,			/* 0x39	*/
		op_srld,			/* 0x3a	*/
		op_srle,			/* 0x3b	*/
		op_srlh,			/* 0x3c	*/
		op_srll,			/* 0x3d	*/
		op_srlhl,			/* 0x3e	*/
		op_srla,			/* 0x3f	*/
		op_tb0b,			/* 0x40	*/
		op_tb0c,			/* 0x41	*/
		op_tb0d,			/* 0x42	*/
		op_tb0e,			/* 0x43	*/
		op_tb0h,			/* 0x44	*/
		op_tb0l,			/* 0x45	*/
		op_tb0hl,			/* 0x46	*/
		op_tb0a,			/* 0x47	*/
		op_tb1b,			/* 0x48	*/
		op_tb1c,			/* 0x49	*/
		op_tb1d,			/* 0x4a	*/
		op_tb1e,			/* 0x4b	*/
		op_tb1h,			/* 0x4c	*/
		op_tb1l,			/* 0x4d	*/
		op_tb1hl,			/* 0x4e	*/
		op_tb1a,			/* 0x4f	*/
		op_tb2b,			/* 0x50	*/
		op_tb2c,			/* 0x51	*/
		op_tb2d,			/* 0x52	*/
		op_tb2e,			/* 0x53	*/
		op_tb2h,			/* 0x54	*/
		op_tb2l,			/* 0x55	*/
		op_tb2hl,			/* 0x56	*/
		op_tb2a,			/* 0x57	*/
		op_tb3b,			/* 0x58	*/
		op_tb3c,			/* 0x59	*/
		op_tb3d,			/* 0x5a	*/
		op_tb3e,			/* 0x5b	*/
		op_tb3h,			/* 0x5c	*/
		op_tb3l,			/* 0x5d	*/
		op_tb3hl,			/* 0x5e	*/
		op_tb3a,			/* 0x5f	*/
		op_tb4b,			/* 0x60	*/
		op_tb4c,			/* 0x61	*/
		op_tb4d,			/* 0x62	*/
		op_tb4e,			/* 0x63	*/
		op_tb4h,			/* 0x64	*/
		op_tb4l,			/* 0x65	*/
		op_tb4hl,			/* 0x66	*/
		op_tb4a,			/* 0x67	*/
		op_tb5b,			/* 0x68	*/
		op_tb5c,			/* 0x69	*/
		op_tb5d,			/* 0x6a	*/
		op_tb5e,			/* 0x6b	*/
		op_tb5h,			/* 0x6c	*/
		op_tb5l,			/* 0x6d	*/
		op_tb5hl,			/* 0x6e	*/
		op_tb5a,			/* 0x6f	*/
		op_tb6b,			/* 0x70	*/
		op_tb6c,			/* 0x71	*/
		op_tb6d,			/* 0x72	*/
		op_tb6e,			/* 0x73	*/
		op_tb6h,			/* 0x74	*/
		op_tb6l,			/* 0x75	*/
		op_tb6hl,			/* 0x76	*/
		op_tb6a,			/* 0x77	*/
		op_tb7b,			/* 0x78	*/
		op_tb7c,			/* 0x79	*/
		op_tb7d,			/* 0x7a	*/
		op_tb7e,			/* 0x7b	*/
		op_tb7h,			/* 0x7c	*/
		op_tb7l,			/* 0x7d	*/
		op_tb7hl,			/* 0x7e	*/
		op_tb7a,			/* 0x7f	*/
		op_rb0b,			/* 0x80	*/
		op_rb0c,			/* 0x81	*/
		op_rb0d,			/* 0x82	*/
		op_rb0e,			/* 0x83	*/
		op_rb0h,			/* 0x84	*/
		op_rb0l,			/* 0x85	*/
		op_rb0hl,			/* 0x86	*/
		op_rb0a,			/* 0x87	*/
		op_rb1b,			/* 0x88	*/
		op_rb1c,			/* 0x89	*/
		op_rb1d,			/* 0x8a	*/
		op_rb1e,			/* 0x8b	*/
		op_rb1h,			/* 0x8c	*/
		op_rb1l,			/* 0x8d	*/
		op_rb1hl,			/* 0x8e	*/
		op_rb1a,			/* 0x8f	*/
		op_rb2b,			/* 0x90	*/
		op_rb2c,			/* 0x91	*/
		op_rb2d,			/* 0x92	*/
		op_rb2e,			/* 0x93	*/
		op_rb2h,			/* 0x94	*/
		op_rb2l,			/* 0x95	*/
		op_rb2hl,			/* 0x96	*/
		op_rb2a,			/* 0x97	*/
		op_rb3b,			/* 0x98	*/
		op_rb3c,			/* 0x99	*/
		op_rb3d,			/* 0x9a	*/
		op_rb3e,			/* 0x9b	*/
		op_rb3h,			/* 0x9c	*/
		op_rb3l,			/* 0x9d	*/
		op_rb3hl,			/* 0x9e	*/
		op_rb3a,			/* 0x9f	*/
		op_rb4b,			/* 0xa0	*/
		op_rb4c,			/* 0xa1	*/
		op_rb4d,			/* 0xa2	*/
		op_rb4e,			/* 0xa3	*/
		op_rb4h,			/* 0xa4	*/
		op_rb4l,			/* 0xa5	*/
		op_rb4hl,			/* 0xa6	*/
		op_rb4a,			/* 0xa7	*/
		op_rb5b,			/* 0xa8	*/
		op_rb5c,			/* 0xa9	*/
		op_rb5d,			/* 0xaa	*/
		op_rb5e,			/* 0xab	*/
		op_rb5h,			/* 0xac	*/
		op_rb5l,			/* 0xad	*/
		op_rb5hl,			/* 0xae	*/
		op_rb5a,			/* 0xaf	*/
		op_rb6b,			/* 0xb0	*/
		op_rb6c,			/* 0xb1	*/
		op_rb6d,			/* 0xb2	*/
		op_rb6e,			/* 0xb3	*/
		op_rb6h,			/* 0xb4	*/
		op_rb6l,			/* 0xb5	*/
		op_rb6hl,			/* 0xb6	*/
		op_rb6a,			/* 0xb7	*/
		op_rb7b,			/* 0xb8	*/
		op_rb7c,			/* 0xb9	*/
		op_rb7d,			/* 0xba	*/
		op_rb7e,			/* 0xbb	*/
		op_rb7h,			/* 0xbc	*/
		op_rb7l,			/* 0xbd	*/
		op_rb7hl,			/* 0xbe	*/
		op_rb7a,			/* 0xbf	*/
		op_sb0b,			/* 0xc0	*/
		op_sb0c,			/* 0xc1	*/
		op_sb0d,			/* 0xc2	*/
		op_sb0e,			/* 0xc3	*/
		op_sb0h,			/* 0xc4	*/
		op_sb0l,			/* 0xc5	*/
		op_sb0hl,			/* 0xc6	*/
		op_sb0a,			/* 0xc7	*/
		op_sb1b,			/* 0xc8	*/
		op_sb1c,			/* 0xc9	*/
		op_sb1d,			/* 0xca	*/
		op_sb1e,			/* 0xcb	*/
		op_sb1h,			/* 0xcc	*/
		op_sb1l,			/* 0xcd	*/
		op_sb1hl,			/* 0xce	*/
		op_sb1a,			/* 0xcf	*/
		op_sb2b,			/* 0xd0	*/
		op_sb2c,			/* 0xd1	*/
		op_sb2d,			/* 0xd2	*/
		op_sb2e,			/* 0xd3	*/
		op_sb2h,			/* 0xd4	*/
		op_sb2l,			/* 0xd5	*/
		op_sb2hl,			/* 0xd6	*/
		op_sb2a,			/* 0xd7	*/
		op_sb3b,			/* 0xd8	*/
		op_sb3c,			/* 0xd9	*/
		op_sb3d,			/* 0xda	*/
		op_sb3e,			/* 0xdb	*/
		op_sb3h,			/* 0xdc	*/
		op_sb3l,			/* 0xdd	*/
		op_sb3hl,			/* 0xde	*/
		op_sb3a,			/* 0xdf	*/
		op_sb4b,			/* 0xe0	*/
		op_sb4c,			/* 0xe1	*/
		op_sb4d,			/* 0xe2	*/
		op_sb4e,			/* 0xe3	*/
		op_sb4h,			/* 0xe4	*/
		op_sb4l,			/* 0xe5	*/
		op_sb4hl,			/* 0xe6	*/
		op_sb4a,			/* 0xe7	*/
		op_sb5b,			/* 0xe8	*/
		op_sb5c,			/* 0xe9	*/
		op_sb5d,			/* 0xea	*/
		op_sb5e,			/* 0xeb	*/
		op_sb5h,			/* 0xec	*/
		op_sb5l,			/* 0xed	*/
		op_sb5hl,			/* 0xee	*/
		op_sb5a,			/* 0xef	*/
		op_sb6b,			/* 0xf0	*/
		op_sb6c,			/* 0xf1	*/
		op_sb6d,			/* 0xf2	*/
		op_sb6e,			/* 0xf3	*/
		op_sb6h,			/* 0xf4	*/
		op_sb6l,			/* 0xf5	*/
		op_sb6hl,			/* 0xf6	*/
		op_sb6a,			/* 0xf7	*/
		op_sb7b,			/* 0xf8	*/
		op_sb7c,			/* 0xf9	*/
		op_sb7d,			/* 0xfa	*/
		op_sb7e,			/* 0xfb	*/
		op_sb7h,			/* 0xfc	*/
		op_sb7l,			/* 0xfd	*/
		op_sb7hl,			/* 0xfe	*/
		op_sb7a				/* 0xff	*/
	};

#ifdef WANT_TIM
	register long t;
	t = (*op_cb[*PC++]) ();		/* execute next opcode */
#else
	(*op_cb[*PC++]) ();
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
 *	initial 0xcb of a multi byte opcode.
 */
static long trap_cb()
{
	cpu_error = OPTRAP2;
	cpu_state = STOPPED;
#ifdef WANT_TIM
	return(0L);
#endif
}

static long op_srla()		     /*	SRL A */
{
	(A & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A >>= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srlb()		     /*	SRL B */
{
	(B & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	B >>= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[B]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srlc()		     /*	SRL C */
{
	(C & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	C >>= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[C]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srld()		     /*	SRL D */
{
	(D & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	D >>= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[D]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srle()		     /*	SRL E */
{
	(E & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	E >>= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[E]) ? (F &= ~P_FLAG) :(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srlh()		     /*	SRL H */
{
	(H & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	H >>= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[H]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srll()		     /*	SRL L */
{
	(L & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	L >>= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[L]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srlhl()		     /*	SRL (HL) */
{
	register BYTE *p;

	p = ram	+ (H <<	8) + L;
	(*p & 1) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	*p >>= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(parrity[*p]) ?	(F &= ~P_FLAG) : (F |= P_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_slaa()		     /*	SLA A */
{
	(A & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	A <<= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_slab()		     /*	SLA B */
{
	(B & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	B <<= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[B]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_slac()		     /*	SLA C */
{
	(C & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	C <<= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[C]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_slad()		     /*	SLA D */
{
	(D & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	D <<= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[D]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_slae()		     /*	SLA E */
{
	(E & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	E <<= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[E]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_slah()		     /*	SLA H */
{
	(H & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	H <<= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[H]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_slal()		     /*	SLA L */
{
	(L & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	L <<= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[L]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_slahl()		     /*	SLA (HL) */
{
	register BYTE *p;

	p = ram	+ (H <<	8) + L;
	(*p & 128) ? (F	|= C_FLAG) : (F	&= ~C_FLAG);
	*p <<= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(parrity[*p]) ?	(F &= ~P_FLAG) : (F |= P_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rlra()		     /*	RL A */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(A & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	A <<= 1;
	if (old_c_flag)	A |= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlb()		     /*	RL B */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(B & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	B <<= 1;
	if (old_c_flag)	B |= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[B]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlc()		     /*	RL C */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(C & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	C <<= 1;
	if (old_c_flag)	C |= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[C]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rld()		     /*	RL D */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(D & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	D <<= 1;
	if (old_c_flag)	D |= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[D]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rle()		     /*	RL E */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(E & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	E <<= 1;
	if (old_c_flag)	E |= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[E]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlh()		     /*	RL H */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(H & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	H <<= 1;
	if (old_c_flag)	H |= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[H]) ? (F &= ~P_FLAG) :(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rll()		     /*	RL L */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(L & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	L <<= 1;
	if (old_c_flag)	L |= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[L]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlhl()		     /*	RL (HL)	*/
{
	register int old_c_flag;
	register BYTE *p;

	p = ram	+ (H <<	8) + L;
	old_c_flag = F & C_FLAG;
	(*p & 128) ? (F	|= C_FLAG) : (F	&= ~C_FLAG);
	*p <<= 1;
	if (old_c_flag)	*p |= 1;
	F &= ~(H_FLAG |	N_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(parrity[*p]) ?	(F &= ~P_FLAG) : (F |= P_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rrra()		     /*	RR A */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(A & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A >>= 1;
	if (old_c_flag)	A |= 128;
	F &= ~(H_FLAG |	N_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrb()		     /*	RR B */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(B & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	B >>= 1;
	if (old_c_flag)	B |= 128;
	F &= ~(H_FLAG |	N_FLAG);
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[B]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrc()		     /*	RR C */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(C & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	C >>= 1;
	if (old_c_flag)	C |= 128;
	F &= ~(H_FLAG |	N_FLAG);
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[C]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrd()		     /*	RR D */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(D & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	D >>= 1;
	if (old_c_flag)	D |= 128;
	F &= ~(H_FLAG |	N_FLAG);
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[D]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rre()		     /*	RR E */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(E & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	E >>= 1;
	if (old_c_flag)	E |= 128;
	F &= ~(H_FLAG |	N_FLAG);
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[E]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrh()		     /*	RR H */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(H & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	H >>= 1;
	if (old_c_flag)	H |= 128;
	F &= ~(H_FLAG |	N_FLAG);
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[H]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrl()		     /*	RR L */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(L & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	L >>= 1;
	if (old_c_flag)	L |= 128;
	F &= ~(H_FLAG |	N_FLAG);
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[L]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrhl()		     /*	RR (HL)	*/
{
	register int old_c_flag;
	register BYTE *p;

	old_c_flag = F & C_FLAG;
	p = ram	+ (H <<	8) + L;
	(*p & 1) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	*p >>= 1;
	if (old_c_flag)	*p |= 128;
	F &= ~(H_FLAG |	N_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(parrity[*p]) ?	(F &= ~P_FLAG) : (F |= P_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rrcra()		     /*	RRC A */
{
	register int i;

	i = A &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	A >>= 1;
	if (i) A |= 128;
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrcb()		     /*	RRC B */
{
	register int i;

	i = B &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	B >>= 1;
	if (i) B |= 128;
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[B]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrcc()		     /*	RRC C */
{
	register int i;

	i = C &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	C >>= 1;
	if (i) C |= 128;
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[C]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrcd()		     /*	RRC D */
{
	register int i;

	i = D &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	D >>= 1;
	if (i) D |= 128;
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[D]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrce()		     /*	RRC E */
{
	register int i;

	i = E &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	E >>= 1;
	if (i) E |= 128;
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[E]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrch()		     /*	RRC H */
{
	register int i;

	i = H &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	H >>= 1;
	if (i) H |= 128;
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[H]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrcl()		     /*	RRC L */
{
	register int i;

	i = L &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	L >>= 1;
	if (i) L |= 128;
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[L]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rrchl()		     /*	RRC (HL) */
{
	register int i;
	register BYTE *p;

	p = ram	+ (H <<	8) + L;
	i = *p & 1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	*p >>= 1;
	if (i) *p |= 128;
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(parrity[*p]) ?	(F &= ~P_FLAG) : (F |= P_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rlcra()		     /*	RLC A */
{
	register int i;

	i = A &	128;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	A <<= 1;
	if (i) A |= 1;
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlcb()		     /*	RLC B */
{
	register int i;

	i = B &	128;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	B <<= 1;
	if (i) B |= 1;
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[B]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlcc()		     /*	RLC C */
{
	register int i;

	i = C &	128;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	C <<= 1;
	if (i) C |= 1;
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[C]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlcd()		     /*	RLC D */
{
	register int i;

	i = D &	128;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	D <<= 1;
	if (i) D |= 1;
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[D]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlce()		     /*	RLC E */
{
	register int i;

	i = E &	128;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	E <<= 1;
	if (i) E |= 1;
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[E]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlch()		     /*	RLC H */
{
	register int i;

	i = H &	128;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	H <<= 1;
	if (i) H |= 1;
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[H]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlcl()		     /*	RLC L */
{
	register int i;

	i = L &	128;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	L <<= 1;
	if (i) L |= 1;
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[L]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rlchl()		     /*	RLC (HL) */
{
	register int i;
	register BYTE *p;

	p = ram	+ (H <<	8) + L;
	i = *p & 128;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	*p <<= 1;
	if (i) *p |= 1;
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(parrity[*p]) ?	(F &= ~P_FLAG) : (F |= P_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sraa()		     /*	SRA A */
{
	register int i;

	i = A &	128;
	(A & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A >>= 1;
	A |= i;
	F &= ~(H_FLAG |	N_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srab()		     /*	SRA B */
{
	register int i;

	i = B &	128;
	(B & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	B >>= 1;
	B |= i;
	F &= ~(H_FLAG |	N_FLAG);
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[B]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srac()		     /*	SRA C */
{
	register int i;

	i = C &	128;
	(C & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	C >>= 1;
	C |= i;
	F &= ~(H_FLAG |	N_FLAG);
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[C]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srad()		     /*	SRA D */
{
	register int i;

	i = D &	128;
	(D & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	D >>= 1;
	D |= i;
	F &= ~(H_FLAG |	N_FLAG);
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[D]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srae()		     /*	SRA E */
{
	register int i;

	i = E &	128;
	(E & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	E >>= 1;
	E |= i;
	F &= ~(H_FLAG |	N_FLAG);
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[E]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srah()		     /*	SRA H */
{
	register int i;

	i = H &	128;
	(H & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	H >>= 1;
	H |= i;
	F &= ~(H_FLAG |	N_FLAG);
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[H]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sral()		     /*	SRA L */
{
	register int i;

	i = L &	128;
	(L & 1)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	L >>= 1;
	L |= i;
	F &= ~(H_FLAG |	N_FLAG);
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[L]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_srahl()		     /*	SRA (HL) */
{
	register int i;
	register BYTE *p;

	p = ram	+ (H <<	8) + L;
	i = *p & 128;
	(*p & 1) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	*p >>= 1;
	*p |= i;
	F &= ~(H_FLAG |	N_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(parrity[*p]) ?	(F &= ~P_FLAG) : (F |= P_FLAG);
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sb0a()		     /*	SET 0,A	*/
{
	A |= 1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb1a()		     /*	SET 1,A	*/
{
	A |= 2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb2a()		     /*	SET 2,A	*/
{
	A |= 4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb3a()		     /*	SET 3,A	*/
{
	A |= 8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb4a()		     /*	SET 4,A	*/
{
	A |= 16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb5a()		     /*	SET 5,A	*/
{
	A |= 32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb6a()		     /*	SET 6,A	*/
{
	A |= 64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb7a()		     /*	SET 7,A	*/
{
	A |= 128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb0b()		     /*	SET 0,B	*/
{
	B |= 1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb1b()		     /*	SET 1,B	*/
{
	B |= 2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb2b()		     /*	SET 2,B	*/
{
	B |= 4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb3b()		     /*	SET 3,B	*/
{
	B |= 8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb4b()		     /*	SET 4,B	*/
{
	B |= 16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb5b()		     /*	SET 5,B	*/
{
	B |= 32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb6b()		     /*	SET 6,B	*/
{
	B |= 64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb7b()		     /*	SET 7,B	*/
{
	B |= 128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb0c()		     /*	SET 0,C	*/
{
	C |= 1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb1c()		     /*	SET 1,C	*/
{
	C |= 2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb2c()		     /*	SET 2,C	*/
{
	C |= 4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb3c()		     /*	SET 3,C	*/
{
	C |= 8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb4c()		     /*	SET 4,C	*/
{
	C |= 16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb5c()		     /*	SET 5,C	*/
{
	C |= 32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb6c()		     /*	SET 6,C	*/
{
	C |= 64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb7c()		     /*	SET 7,C	*/
{
	C |= 128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb0d()		     /*	SET 0,D	*/
{
	D |= 1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb1d()		     /*	SET 1,D	*/
{
	D |= 2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb2d()		     /*	SET 2,D	*/
{
	D |= 4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb3d()		     /*	SET 3,D	*/
{
	D |= 8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb4d()		     /*	SET 4,D	*/
{
	D |= 16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb5d()		     /*	SET 5,D	*/
{
	D |= 32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb6d()		     /*	SET 6,D	*/
{
	D |= 64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb7d()		     /*	SET 7,D	*/
{
	D |= 128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb0e()		     /*	SET 0,E	*/
{
	E |= 1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb1e()		     /*	SET 1,E	*/
{
	E |= 2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb2e()		     /*	SET 2,E	*/
{
	E |= 4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb3e()		     /*	SET 3,E	*/
{
	E |= 8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb4e()		     /*	SET 4,E	*/
{
	E |= 16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb5e()		     /*	SET 5,E	*/
{
	E |= 32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb6e()		     /*	SET 6,E	*/
{
	E |= 64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb7e()		     /*	SET 7,E	*/
{
	E |= 128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb0h()		     /*	SET 0,H	*/
{
	H |= 1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb1h()		     /*	SET 1,H	*/
{
	H |= 2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb2h()		     /*	SET 2,H	*/
{
	H |= 4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb3h()		     /*	SET 3,H	*/
{
	H |= 8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb4h()		     /*	SET 4,H	*/
{
	H |= 16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb5h()		     /*	SET 5,H	*/
{
	H |= 32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb6h()		     /*	SET 6,H	*/
{
	H |= 64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb7h()		     /*	SET 7,H	*/
{
	H |= 128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb0l()		     /*	SET 0,L	*/
{
	L |= 1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb1l()		     /*	SET 1,L	*/
{
	L |= 2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb2l()		     /*	SET 2,L	*/
{
	L |= 4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb3l()		     /*	SET 3,L	*/
{
	L |= 8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb4l()		     /*	SET 4,L	*/
{
	L |= 16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb5l()		     /*	SET 5,L	*/
{
	L |= 32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb6l()		     /*	SET 6,L	*/
{
	L |= 64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb7l()		     /*	SET 7,L	*/
{
	L |= 128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_sb0hl()		     /*	SET 0,(HL) */
{
	*(ram +	(H << 8) + L) |= 1;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sb1hl()		     /*	SET 1,(HL) */
{
	*(ram +	(H << 8) + L) |= 2;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sb2hl()		     /*	SET 2,(HL) */
{
	*(ram +	(H << 8) + L) |= 4;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sb3hl()		     /*	SET 3,(HL) */
{
	*(ram +	(H << 8) + L) |= 8;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sb4hl()		     /*	SET 4,(HL) */
{
	*(ram +	(H << 8) + L) |= 16;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sb5hl()		     /*	SET 5,(HL) */
{
	*(ram +	(H << 8) + L) |= 32;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sb6hl()		     /*	SET 6,(HL) */
{
	*(ram +	(H << 8) + L) |= 64;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_sb7hl()		     /*	SET 7,(HL) */
{
	*(ram +	(H << 8) + L) |= 128;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rb0a()		     /*	RES 0,A	*/
{
	A &= ~1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb1a()		     /*	RES 1,A	*/
{
	A &= ~2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb2a()		     /*	RES 2,A	*/
{
	A &= ~4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb3a()		     /*	RES 3,A	*/
{
	A &= ~8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb4a()		     /*	RES 4,A	*/
{
	A &= ~16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb5a()		     /*	RES 5,A	*/
{
	A &= ~32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb6a()		     /*	RES 6,A	*/
{
	A &= ~64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb7a()		     /*	RES 7,A	*/
{
	A &= ~128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb0b()		     /*	RES 0,B	*/
{
	B &= ~1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb1b()		     /*	RES 1,B	*/
{
	B &= ~2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb2b()		     /*	RES 2,B	*/
{
	B &= ~4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb3b()		     /*	RES 3,B	*/
{
	B &= ~8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb4b()		     /*	RES 4,B	*/
{
	B &= ~16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb5b()		     /*	RES 5,B	*/
{
	B &= ~32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb6b()		     /*	RES 6,B	*/
{
	B &= ~64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb7b()		     /*	RES 7,B	*/
{
	B &= ~128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb0c()		     /*	RES 0,C	*/
{
	C &= ~1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb1c()		     /*	RES 1,C	*/
{
	C &= ~2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb2c()		     /*	RES 2,C	*/
{
	C &= ~4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb3c()		     /*	RES 3,C	*/
{
	C &= ~8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb4c()		     /*	RES 4,C	*/
{
	C &= ~16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb5c()		     /*	RES 5,C	*/
{
	C &= ~32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb6c()		     /*	RES 6,C	*/
{
	C &= ~64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb7c()		     /*	RES 7,C	*/
{
	C &= ~128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb0d()		     /*	RES 0,D	*/
{
	D &= ~1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb1d()		     /*	RES 1,D	*/
{
	D &= ~2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb2d()		     /*	RES 2,D	*/
{
	D &= ~4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb3d()		     /*	RES 3,D	*/
{
	D &= ~8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb4d()		     /*	RES 4,D	*/
{
	D &= ~16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb5d()		     /*	RES 5,D	*/
{
	D &= ~32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb6d()		     /*	RES 6,D	*/
{
	D &= ~64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb7d()		     /*	RES 7,D	*/
{
	D &= ~128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb0e()		     /*	RES 0,E	*/
{
	E &= ~1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb1e()		     /*	RES 1,E	*/
{
	E &= ~2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb2e()		     /*	RES 2,E	*/
{
	E &= ~4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb3e()		     /*	RES 3,E	*/
{
	E &= ~8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb4e()		     /*	RES 4,E	*/
{
	E &= ~16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb5e()		     /*	RES 5,E	*/
{
	E &= ~32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb6e()		     /*	RES 6,E	*/
{
	E &= ~64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb7e()		     /*	RES 7,E	*/
{
	E &= ~128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb0h()		     /*	RES 0,H	*/
{
	H &= ~1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb1h()		     /*	RES 1,H	*/
{
	H &= ~2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb2h()		     /*	RES 2,H	*/
{
	H &= ~4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb3h()		     /*	RES 3,H	*/
{
	H &= ~8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb4h()		     /*	RES 4,H	*/
{
	H &= ~16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb5h()		     /*	RES 5,H	*/
{
	H &= ~32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb6h()		     /*	RES 6,H	*/
{
	H &= ~64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb7h()		     /*	RES 7,H	*/
{
	H &= ~128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb0l()		     /*	RES 0,L	*/
{
	L &= ~1;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb1l()		     /*	RES 1,L	*/
{
	L &= ~2;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb2l()		     /*	RES 2,L	*/
{
	L &= ~4;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb3l()		     /*	RES 3,L	*/
{
	L &= ~8;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb4l()		     /*	RES 4,L	*/
{
	L &= ~16;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb5l()		     /*	RES 5,L	*/
{
	L &= ~32;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb6l()		     /*	RES 6,L	*/
{
	L &= ~64;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb7l()		     /*	RES 7,L	*/
{
	L &= ~128;
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_rb0hl()		     /*	RES 0,(HL) */
{
	*(ram +	(H << 8) + L) &= ~1;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rb1hl()		     /*	RES 1,(HL) */
{
	*(ram +	(H << 8) + L) &= ~2;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rb2hl()		     /*	RES 2,(HL) */
{
	*(ram +	(H << 8) + L) &= ~4;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rb3hl()		     /*	RES 3,(HL) */
{
	*(ram +	(H << 8) + L) &= ~8;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rb4hl()		     /*	RES 4,(HL) */
{
	*(ram +	(H << 8) + L) &= ~16;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rb5hl()		     /*	RES 5,(HL) */
{
	*(ram +	(H << 8) + L) &= ~32;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rb6hl()		     /*	RES 6,(HL) */
{
	*(ram +	(H << 8) + L) &= ~64;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_rb7hl()		     /*	RES 7,(HL) */
{
	*(ram +	(H << 8) + L) &= ~128;
#ifdef WANT_TIM
	return(15L);
#endif
}

static long op_tb0a()		     /*	BIT 0,A	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(A & 1)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb1a()		     /*	BIT 1,A	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(A & 2)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb2a()		     /*	BIT 2,A	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(A & 4)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb3a()		     /*	BIT 3,A	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(A & 8)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb4a()		     /*	BIT 4,A	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(A & 16) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb5a()		     /*	BIT 5,A	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(A & 32) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb6a()		     /*	BIT 6,A	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(A & 64) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb7a()		     /*	BIT 7,A	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(A & 128) ? (F &= ~Z_FLAG) : (F	|= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb0b()		     /*	BIT 0,B	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(B & 1)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb1b()		     /*	BIT 1,B	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(B & 2)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb2b()		     /*	BIT 2,B	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(B & 4)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb3b()		     /*	BIT 3,B	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(B & 8)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb4b()		     /*	BIT 4,B	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(B & 16) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb5b()		     /*	BIT 5,B	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(B & 32) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb6b()		     /*	BIT 6,B	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(B & 64) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb7b()		     /*	BIT 7,B	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(B & 128) ? (F &= ~Z_FLAG) : (F	|= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb0c()		     /*	BIT 0,C	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(C & 1)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb1c()		     /*	BIT 1,C	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(C & 2)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb2c()		     /*	BIT 2,C	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(C & 4)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb3c()		     /*	BIT 3,C	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(C & 8)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb4c()		     /*	BIT 4,C	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(C & 16) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb5c()		     /*	BIT 5,C	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(C & 32) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb6c()		     /*	BIT 6,C	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(C & 64) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb7c()		     /*	BIT 7,C	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(C & 128) ? (F &= ~Z_FLAG) : (F	|= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb0d()		     /*	BIT 0,D	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(D & 1)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb1d()		     /*	BIT 1,D	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(D & 2)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb2d()		     /*	BIT 2,D	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(D & 4)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb3d()		     /*	BIT 3,D	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(D & 8)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb4d()		     /*	BIT 4,D	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(D & 16) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb5d()		     /*	BIT 5,D	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(D & 32) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb6d()		     /*	BIT 6,D	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(D & 64) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb7d()		     /*	BIT 7,D	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(D & 128) ? (F &= ~Z_FLAG) : (F	|= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb0e()		     /*	BIT 0,E	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(E & 1)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb1e()		     /*	BIT 1,E	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(E & 2)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb2e()		     /*	BIT 2,E	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(E & 4)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb3e()		     /*	BIT 3,E	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(E & 8)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb4e()		     /*	BIT 4,E	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(E & 16) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb5e()		     /*	BIT 5,E	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(E & 32) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb6e()		     /*	BIT 6,E	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(E & 64) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb7e()		     /*	BIT 7,E	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(E & 128) ? (F &= ~Z_FLAG) : (F	|= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb0h()		     /*	BIT 0,H	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(H & 1)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb1h()		     /*	BIT 1,H	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(H & 2)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb2h()		     /*	BIT 2,H	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(H & 4)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb3h()		     /*	BIT 3,H	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(H & 8)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb4h()		     /*	BIT 4,H	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(H & 16) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb5h()		     /*	BIT 5,H	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(H & 32) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb6h()		     /*	BIT 6,H	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(H & 64) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb7h()		     /*	BIT 7,H	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(H & 128) ? (F &= ~Z_FLAG) : (F	|= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb0l()		     /*	BIT 0,L	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(L & 1)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb1l()		     /*	BIT 1,L	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(L & 2)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb2l()		     /*	BIT 2,L	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(L & 4)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb3l()		     /*	BIT 3,L	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(L & 8)	? (F &=	~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb4l()		     /*	BIT 4,L	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(L & 16) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb5l()		     /*	BIT 5,L	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(L & 32) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb6l()		     /*	BIT 6,L	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(L & 64) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb7l()		     /*	BIT 7,L	*/
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(L & 128) ? (F &= ~Z_FLAG) : (F	|= Z_FLAG);
#ifdef WANT_TIM
	return(8L);
#endif
}

static long op_tb0hl()		     /*	BIT 0,(HL) */
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(*(ram + (H << 8) + L) & 1) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_tb1hl()		     /*	BIT 1,(HL) */
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(*(ram + (H << 8) + L) & 2) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_tb2hl()		     /*	BIT 2,(HL) */
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(*(ram + (H << 8) + L) & 4) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_tb3hl()		     /*	BIT 3,(HL) */
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(*(ram + (H << 8) + L) & 8) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_tb4hl()		     /*	BIT 4,(HL) */
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(*(ram + (H << 8) + L) & 16) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_tb5hl()		     /*	BIT 5,(HL) */
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(*(ram + (H << 8) + L) & 32) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_tb6hl()		     /*	BIT 6,(HL) */
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(*(ram + (H << 8) + L) & 64) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

static long op_tb7hl()		     /*	BIT 7,(HL) */
{
	F &= ~N_FLAG;
	F |= H_FLAG;
	(*(ram + (H << 8) + L) & 128) ?	(F &= ~Z_FLAG) : (F |= Z_FLAG);
#ifdef WANT_TIM
	return(12L);
#endif
}

