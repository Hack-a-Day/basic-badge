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
 * 10-JUN-92 Release 1.6  long casting problem solved with COHERENT 3.2
 *			 and some optimization
 * 25-JUN-92 Release 1.7 comments in english
 */

#include "sim.h"
#include "simglb.h"
#include "fdefs.h"

/*
 *	This function builds the Z80 central processing unit.
 *	The opcode where PC points to is fetched from the memory
 *	and PC incremented by one. The opcode is used as an
 *	index to an array with function pointers, to execute a
 *	function which emulates this Z80 opcode.
 */
cpu()
{
	 long op_notimpl();
	 long op_nop(), op_halt(), op_scf(), op_ccf(), op_cpl(),	op_daa();
	 long op_ei(), op_di();
	 long op_in(), op_out();
	 long op_ldan(),	op_ldbn(), op_ldcn(), op_lddn(), op_lden();
	 long op_ldhn(),	op_ldln();
	 long op_ldabc(), op_ldade(), op_ldann();
	 long op_ldbca(), op_lddea(), op_ldnna();
	 long op_ldhla(), op_ldhlb(), op_ldhlc(), op_ldhld();
	 long op_ldhle(), op_ldhlh(), op_ldhll(), op_ldhl1();
	 long op_ldaa(),	op_ldab(), op_ldac(), op_ldad(), op_ldae();
	 long op_ldah(),	op_ldal(), op_ldahl();
	 long op_ldba(),	op_ldbb(), op_ldbc(), op_ldbd(), op_ldbe();
	 long op_ldbh(),	op_ldbl(), op_ldbhl();
	 long op_ldca(),	op_ldcb(), op_ldcc(), op_ldcd(), op_ldce();
	 long op_ldch(),	op_ldcl(), op_ldchl();
	 long op_ldda(),	op_lddb(), op_lddc(), op_lddd(), op_ldde();
	 long op_lddh(),	op_lddl(), op_lddhl();
	 long op_ldea(),	op_ldeb(), op_ldec(), op_lded(), op_ldee();
	 long op_ldeh(),	op_ldel(), op_ldehl();
	 long op_ldha(),	op_ldhb(), op_ldhc(), op_ldhd(), op_ldhe();
	 long op_ldhh(),	op_ldhl(), op_ldhhl();
	 long op_ldla(),	op_ldlb(), op_ldlc(), op_ldld(), op_ldle();
	 long op_ldlh(),	op_ldll(), op_ldlhl();
	 long op_ldbcnn(), op_lddenn(), op_ldhlnn(), op_ldspnn(), op_ldsphl();
	 long op_ldhlin(), op_ldinhl();
	 long op_incbc(), op_incde(), op_inchl(), op_incsp();
	 long op_decbc(), op_decde(), op_dechl(), op_decsp();
	 long op_adhlbc(), op_adhlde(), op_adhlhl(), op_adhlsp();
	 long op_anda(),	op_andb(), op_andc(), op_andd(), op_ande();
	 long op_andh(),	op_andl(), op_andhl(), op_andn();
	 long op_ora(), op_orb(), op_orc(), op_ord(), op_ore();
	 long op_orh(), op_orl(), op_orhl(), op_orn();
	 long op_xora(),	op_xorb(), op_xorc(), op_xord(), op_xore();
	 long op_xorh(),	op_xorl(), op_xorhl(), op_xorn();
	 long op_adda(),	op_addb(), op_addc(), op_addd(), op_adde();
	 long op_addh(),	op_addl(), op_addhl(), op_addn();
	 long op_adca(),	op_adcb(), op_adcc(), op_adcd(), op_adce();
	 long op_adch(),	op_adcl(), op_adchl(), op_adcn();
	 long op_suba(),	op_subb(), op_subc(), op_subd(), op_sube();
	 long op_subh(),	op_subl(), op_subhl(), op_subn();
	 long op_sbca(),	op_sbcb(), op_sbcc(), op_sbcd(), op_sbce();
	 long op_sbch(),	op_sbcl(), op_sbchl(), op_sbcn();
	 long op_cpa(), op_cpb(), op_cpc(), op_cpd(), op_cpe();
	 long op_cph(), op_cplr(), op_cphl(), op_cpn();
	 long op_inca(),	op_incb(), op_incc(), op_incd(), op_ince();
	 long op_inch(),	op_incl(), op_incihl();
	 long op_deca(),	op_decb(), op_decc(), op_decd(), op_dece();
	 long op_dech(),	op_decl(), op_decihl();
	 long op_rlca(),	op_rrca(),op_rla(),op_rra();
	 long op_exdehl(), op_exafaf(), op_exx(), op_exsphl();
	 long op_pushaf(), op_pushbc(), op_pushde(), op_pushhl();
	 long op_popaf(), op_popbc(), op_popde(), op_pophl();
	 long op_jp(), op_jphl(), op_jr(), op_djnz(), op_call(),	op_ret();
	 long op_jpz(), op_jpnz(), op_jpc(), op_jpnc();
	 long op_jppe(),	op_jppo(), op_jpm(), op_jpp();
	 long op_calz(),	op_calnz(), op_calc(), op_calnc();
	 long op_calpe(), op_calpo(), op_calm(),	op_calp();
	 long op_retz(),	op_retnz(), op_retc(), op_retnc();
	 long op_retpe(), op_retpo(), op_retm(),	op_retp();
	 long op_jrz(), op_jrnz(), op_jrc(), op_jrnc();
	 long op_rst00(), op_rst08(), op_rst10(), op_rst18();
	 long op_rst20(), op_rst28(), op_rst30(), op_rst38();
	extern  long op_cb_handel(), op_dd_handel(), op_ed_handel(),
		   op_fd_handel();

	static long (*op_sim[256]) () =	{
		op_nop,				/* 0x00	*/
		op_ldbcnn,			/* 0x01	*/
		op_ldbca,			/* 0x02	*/
		op_incbc,			/* 0x03	*/
		op_incb,			/* 0x04	*/
		op_decb,			/* 0x05	*/
		op_ldbn,			/* 0x06	*/
		op_rlca,			/* 0x07	*/
		op_exafaf,			/* 0x08	*/
		op_adhlbc,			/* 0x09	*/
		op_ldabc,			/* 0x0a	*/
		op_decbc,			/* 0x0b	*/
		op_incc,			/* 0x0c	*/
		op_decc,			/* 0x0d	*/
		op_ldcn,			/* 0x0e	*/
		op_rrca,			/* 0x0f	*/
		op_djnz,			/* 0x10	*/
		op_lddenn,			/* 0x11	*/
		op_lddea,			/* 0x12	*/
		op_incde,			/* 0x13	*/
		op_incd,			/* 0x14	*/
		op_decd,			/* 0x15	*/
		op_lddn,			/* 0x16	*/
		op_rla,				/* 0x17	*/
		op_jr,				/* 0x18	*/
		op_adhlde,			/* 0x19	*/
		op_ldade,			/* 0x1a	*/
		op_decde,			/* 0x1b	*/
		op_ince,			/* 0x1c	*/
		op_dece,			/* 0x1d	*/
		op_lden,			/* 0x1e	*/
		op_rra,				/* 0x1f	*/
		op_jrnz,			/* 0x20	*/
		op_ldhlnn,			/* 0x21	*/
		op_ldinhl,			/* 0x22	*/
		op_inchl,			/* 0x23	*/
		op_inch,			/* 0x24	*/
		op_dech,			/* 0x25	*/
		op_ldhn,			/* 0x26	*/
		op_daa,				/* 0x27	*/
		op_jrz,				/* 0x28	*/
		op_adhlhl,			/* 0x29	*/
		op_ldhlin,			/* 0x2a	*/
		op_dechl,			/* 0x2b	*/
		op_incl,			/* 0x2c	*/
		op_decl,			/* 0x2d	*/
		op_ldln,			/* 0x2e	*/
		op_cpl,				/* 0x2f	*/
		op_jrnc,			/* 0x30	*/
		op_ldspnn,			/* 0x31	*/
		op_ldnna,			/* 0x32	*/
		op_incsp,			/* 0x33	*/
		op_incihl,			/* 0x34	*/
		op_decihl,			/* 0x35	*/
		op_ldhl1,			/* 0x36	*/
		op_scf,				/* 0x37	*/
		op_jrc,				/* 0x38	*/
		op_adhlsp,			/* 0x39	*/
		op_ldann,			/* 0x3a	*/
		op_decsp,			/* 0x3b	*/
		op_inca,			/* 0x3c	*/
		op_deca,			/* 0x3d	*/
		op_ldan,			/* 0x3e	*/
		op_ccf,				/* 0x3f	*/
		op_ldbb,			/* 0x40	*/
		op_ldbc,			/* 0x41	*/
		op_ldbd,			/* 0x42	*/
		op_ldbe,			/* 0x43	*/
		op_ldbh,			/* 0x44	*/
		op_ldbl,			/* 0x45	*/
		op_ldbhl,			/* 0x46	*/
		op_ldba,			/* 0x47	*/
		op_ldcb,			/* 0x48	*/
		op_ldcc,			/* 0x49	*/
		op_ldcd,			/* 0x4a	*/
		op_ldce,			/* 0x4b	*/
		op_ldch,			/* 0x4c	*/
		op_ldcl,			/* 0x4d	*/
		op_ldchl,			/* 0x4e	*/
		op_ldca,			/* 0x4f	*/
		op_lddb,			/* 0x50	*/
		op_lddc,			/* 0x51	*/
		op_lddd,			/* 0x52	*/
		op_ldde,			/* 0x53	*/
		op_lddh,			/* 0x54	*/
		op_lddl,			/* 0x55	*/
		op_lddhl,			/* 0x56	*/
		op_ldda,			/* 0x57	*/
		op_ldeb,			/* 0x58	*/
		op_ldec,			/* 0x59	*/
		op_lded,			/* 0x5a	*/
		op_ldee,			/* 0x5b	*/
		op_ldeh,			/* 0x5c	*/
		op_ldel,			/* 0x5d	*/
		op_ldehl,			/* 0x5e	*/
		op_ldea,			/* 0x5f	*/
		op_ldhb,			/* 0x60	*/
		op_ldhc,			/* 0x61	*/
		op_ldhd,			/* 0x62	*/
		op_ldhe,			/* 0x63	*/
		op_ldhh,			/* 0x64	*/
		op_ldhl,			/* 0x65	*/
		op_ldhhl,			/* 0x66	*/
		op_ldha,			/* 0x67	*/
		op_ldlb,			/* 0x68	*/
		op_ldlc,			/* 0x69	*/
		op_ldld,			/* 0x6a	*/
		op_ldle,			/* 0x6b	*/
		op_ldlh,			/* 0x6c	*/
		op_ldll,			/* 0x6d	*/
		op_ldlhl,			/* 0x6e	*/
		op_ldla,			/* 0x6f	*/
		op_ldhlb,			/* 0x70	*/
		op_ldhlc,			/* 0x71	*/
		op_ldhld,			/* 0x72	*/
		op_ldhle,			/* 0x73	*/
		op_ldhlh,			/* 0x74	*/
		op_ldhll,			/* 0x75	*/
		op_halt,			/* 0x76	*/
		op_ldhla,			/* 0x77	*/
		op_ldab,			/* 0x78	*/
		op_ldac,			/* 0x79	*/
		op_ldad,			/* 0x7a	*/
		op_ldae,			/* 0x7b	*/
		op_ldah,			/* 0x7c	*/
		op_ldal,			/* 0x7d	*/
		op_ldahl,			/* 0x7e	*/
		op_ldaa,			/* 0x7f	*/
		op_addb,			/* 0x80	*/
		op_addc,			/* 0x81	*/
		op_addd,			/* 0x82	*/
		op_adde,			/* 0x83	*/
		op_addh,			/* 0x84	*/
		op_addl,			/* 0x85	*/
		op_addhl,			/* 0x86	*/
		op_adda,			/* 0x87	*/
		op_adcb,			/* 0x88	*/
		op_adcc,			/* 0x89	*/
		op_adcd,			/* 0x8a	*/
		op_adce,			/* 0x8b	*/
		op_adch,			/* 0x8c	*/
		op_adcl,			/* 0x8d	*/
		op_adchl,			/* 0x8e	*/
		op_adca,			/* 0x8f	*/
		op_subb,			/* 0x90	*/
		op_subc,			/* 0x91	*/
		op_subd,			/* 0x92	*/
		op_sube,			/* 0x93	*/
		op_subh,			/* 0x94	*/
		op_subl,			/* 0x95	*/
		op_subhl,			/* 0x96	*/
		op_suba,			/* 0x97	*/
		op_sbcb,			/* 0x98	*/
		op_sbcc,			/* 0x99	*/
		op_sbcd,			/* 0x9a	*/
		op_sbce,			/* 0x9b	*/
		op_sbch,			/* 0x9c	*/
		op_sbcl,			/* 0x9d	*/
		op_sbchl,			/* 0x9e	*/
		op_sbca,			/* 0x9f	*/
		op_andb,			/* 0xa0	*/
		op_andc,			/* 0xa1	*/
		op_andd,			/* 0xa2	*/
		op_ande,			/* 0xa3	*/
		op_andh,			/* 0xa4	*/
		op_andl,			/* 0xa5	*/
		op_andhl,			/* 0xa6	*/
		op_anda,			/* 0xa7	*/
		op_xorb,			/* 0xa8	*/
		op_xorc,			/* 0xa9	*/
		op_xord,			/* 0xaa	*/
		op_xore,			/* 0xab	*/
		op_xorh,			/* 0xac	*/
		op_xorl,			/* 0xad	*/
		op_xorhl,			/* 0xae	*/
		op_xora,			/* 0xaf	*/
		op_orb,				/* 0xb0	*/
		op_orc,				/* 0xb1	*/
		op_ord,				/* 0xb2	*/
		op_ore,				/* 0xb3	*/
		op_orh,				/* 0xb4	*/
		op_orl,				/* 0xb5	*/
		op_orhl,			/* 0xb6	*/
		op_ora,				/* 0xb7	*/
		op_cpb,				/* 0xb8	*/
		op_cpc,				/* 0xb9	*/
		op_cpd,				/* 0xba	*/
		op_cpe,				/* 0xbb	*/
		op_cph,				/* 0xbc	*/
		op_cplr,			/* 0xbd	*/
		op_cphl,			/* 0xbe	*/
		op_cpa,				/* 0xbf	*/
		op_retnz,			/* 0xc0	*/
		op_popbc,			/* 0xc1	*/
		op_jpnz,			/* 0xc2	*/
		op_jp,				/* 0xc3	*/
		op_calnz,			/* 0xc4	*/
		op_pushbc,			/* 0xc5	*/
		op_addn,			/* 0xc6	*/
		op_rst00,			/* 0xc7	*/
		op_retz,			/* 0xc8	*/
		op_ret,				/* 0xc9	*/
		op_jpz,				/* 0xca	*/
		op_cb_handel,			/* 0xcb	*/
		op_calz,			/* 0xcc	*/
		op_call,			/* 0xcd	*/
		op_adcn,			/* 0xce	*/
		op_rst08,			/* 0xcf	*/
		op_retnc,			/* 0xd0	*/
		op_popde,			/* 0xd1	*/
		op_jpnc,			/* 0xd2	*/
		op_out,				/* 0xd3	*/
		op_calnc,			/* 0xd4	*/
		op_pushde,			/* 0xd5	*/
		op_subn,			/* 0xd6	*/
		op_rst10,			/* 0xd7	*/
		op_retc,			/* 0xd8	*/
		op_exx,				/* 0xd9	*/
		op_jpc,				/* 0xda	*/
		op_in,				/* 0xdb	*/
		op_calc,			/* 0xdc	*/
		op_dd_handel,			/* 0xdd	*/
		op_sbcn,			/* 0xde	*/
		op_rst18,			/* 0xdf	*/
		op_retpo,			/* 0xe0	*/
		op_pophl,			/* 0xe1	*/
		op_jppo,			/* 0xe2	*/
		op_exsphl,			/* 0xe3	*/
		op_calpo,			/* 0xe4	*/
		op_pushhl,			/* 0xe5	*/
		op_andn,			/* 0xe6	*/
		op_rst20,			/* 0xe7	*/
		op_retpe,			/* 0xe8	*/
		op_jphl,			/* 0xe9	*/
		op_jppe,			/* 0xea	*/
		op_exdehl,			/* 0xeb	*/
		op_calpe,			/* 0xec	*/
		op_ed_handel,			/* 0xed	*/
		op_xorn,			/* 0xee	*/
		op_rst28,			/* 0xef	*/
		op_retp,			/* 0xf0	*/
		op_popaf,			/* 0xf1	*/
		op_jpp,				/* 0xf2	*/
		op_di,				/* 0xf3	*/
		op_calp,			/* 0xf4	*/
		op_pushaf,			/* 0xf5	*/
		op_orn,				/* 0xf6	*/
		op_rst30,			/* 0xf7	*/
		op_retm,			/* 0xf8	*/
		op_ldsphl,			/* 0xf9	*/
		op_jpm,				/* 0xfa	*/
		op_ei,				/* 0xfb	*/
		op_calm,			/* 0xfc	*/
		op_fd_handel,			/* 0xfd	*/
		op_cpn,				/* 0xfe	*/
		op_rst38			/* 0xff	*/
	};

#ifdef WANT_TIM
	register  long t;
#endif

	do {

#ifdef HISIZE		/* write history */
		his[h_next].h_adr = PC - ram;
		his[h_next].h_af = (A << 8) + F;
		his[h_next].h_bc = (B << 8) + C;
		his[h_next].h_de = (D << 8) + E;
		his[h_next].h_hl = (H << 8) + L;
		his[h_next].h_ix = IX;
		his[h_next].h_iy = IY;
		his[h_next].h_sp = STACK - ram;
		h_next++;
		if (h_next == HISIZE) {
			h_flag = 1;
			h_next = 0;
		}
#endif

#ifdef WANT_TIM		/* check for start address of runtime measurement */
		if (PC == t_start && !t_flag) {
			t_flag = 1;	/* switch measurement on */
			t_states = 0L;	/* initialize counted T-states */
		}
#endif

#ifdef WANT_INT		/* CPU interrupt handling */
		if (int_type)
			switch (int_type) {
			case INT_NMI:	/* non maskable interrupt */
				break;
			case INT_INT:	/* maskable interrupt */
				break;
			}
#endif

#ifdef WANT_TIM
		t = (*op_sim[*PC++]) ();/* execute next opcode */
#else
		(*op_sim[*PC++]) ();
#endif

#ifdef WANT_PCC
		if (PC > ram + 65535)	/* check for PC overrun */
			PC = ram;
#endif

		R++;			/* increment refresh register */

#ifdef WANT_TIM				/* do runtime measurement */
		if (t_flag) {
			t_states += t;	/* add T-states for this opcode */
			if (PC == t_end) /* check for end address */
				t_flag = 0; /* if reached, switch measurement off */
		}
#endif

	} while	(cpu_state);
}

/*
 *	Trap not implemented opcodes. This function may be usefull
 *	later to trap some wanted opcodes.
 */
static  long op_notimpl()
{
	cpu_error = OPTRAP1;
	cpu_state = STOPPED;
#ifdef WANT_TIM
	return(0L);
#endif
}

static  long op_nop()		     /*	NOP */
{
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_halt()		     /*	HALT */
{
	if (break_flag)	{
		cpu_error = OPHALT;
		cpu_state = STOPPED;
	} else
		while (int_type	== 0)
			R++;
#ifdef WANT_TIM
	return(0L);
#endif
}

static  long op_scf()		     /*	SCF */
{
	F |= C_FLAG;
	F &= ~(N_FLAG |	H_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ccf()		     /*	CCF */
{
	if (F &	C_FLAG)	{
		F |= H_FLAG;
		F &= ~C_FLAG;
	} else {
		F &= ~H_FLAG;
		F |= C_FLAG;
	}
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_cpl()		     /*	CPL */
{
	A = ~A;
	F |= H_FLAG | N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_daa()		     /*	DAA */
{
	register int old_a;

	old_a =	A;
	if (F &	N_FLAG)	{		/* subtractions */
		if (((A	& 0x0f)	> 9) ||	(F & H_FLAG)) {
			(((old_a & 0x0f) - 6) <	0) ? (F	|= H_FLAG) : (F	&= ~H_FLAG);
			A = old_a -= 6;
		}
		if (((A	& 0xf0)	> 0x90)	|| (F &	C_FLAG)) {
			A -= 0x60;
			if (old_a - 0x60 < 0)
				F |= C_FLAG;
		}
	} else {			/* additions */
		if (((A	& 0x0f)	> 9) ||	(F & H_FLAG)) {
			(((old_a & 0x0f) + 6) >	0x0f) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
			A = old_a += 6;
		}
		if (((A	& 0xf0)	> 0x90)	|| (F &	C_FLAG)) {
			A += 0x60;
			if (old_a + 0x60 > 255)
				F |= C_FLAG;
		}
	}
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ei()		     /*	EI */
{
	IFF = 2;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_di()		     /*	DI */
{
	IFF = 0;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_in()		     /*	IN A,(n) */
{
	BYTE io_in();

	A = io_in(*PC++);
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_out()		     /*	OUT (n),A */
{
	BYTE io_out();

	io_out(*PC++, A);
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_ldan()		     /*	LD A,n */
{
	A = *PC++;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldbn()		     /*	LD B,n */
{
	B = *PC++;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldcn()		     /*	LD C,n */
{
	C = *PC++;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_lddn()		     /*	LD D,n */
{
	D = *PC++;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_lden()		     /*	LD E,n */
{
	E = *PC++;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldhn()		     /*	LD H,n */
{
	H = *PC++;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldln()		     /*	LD L,n */
{
	L = *PC++;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldabc()		     /*	LD A,(BC) */
{
	A = *(ram + (B << 8) + C);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldade()		     /*	LD A,(DE) */
{
	A = *(ram + (D << 8) + E);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldann()		     /*	LD A,(nn) */
{
	register unsigned i;

	i = *PC++;
	i += *PC++ << 8;
	A = *(ram + i);
#ifdef WANT_TIM
	return(13L);
#endif
}

static  long op_ldbca()		     /*	LD (BC),A */
{
	*(ram +	(B << 8) + C) =	A;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_lddea()		     /*	LD (DE),A */
{
	*(ram +	(D << 8) + E) =	A;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldnna()		     /*	LD (nn),A */
{
	register unsigned i;

	i = *PC++;
	i += *PC++ << 8;
	*(ram +	i) = A;
#ifdef WANT_TIM
	return(13L);
#endif
}

static  long op_ldhla()		     /*	LD (HL),A */
{
	*(ram +	(H << 8) + L) =	A;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldhlb()		     /*	LD (HL),B */
{
	*(ram +	(H << 8) + L) =	B;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldhlc()		     /*	LD (HL),C */
{
	*(ram +	(H << 8) + L) =	C;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldhld()		     /*	LD (HL),D */
{
	*(ram +	(H << 8) + L) =	D;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldhle()		     /*	LD (HL),E */
{
	*(ram +	(H << 8) + L) =	E;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldhlh()		     /*	LD (HL),H */
{
	*(ram +	(H << 8) + L) =	H;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldhll()		     /*	LD (HL),L */
{
	*(ram +	(H << 8) + L) =	L;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldhl1()		     /*	LD (HL),n */
{
	*(ram +	(H << 8) + L) =	*PC++;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_ldaa()		     /*	LD A,A */
{
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldab()		     /*	LD A,B */
{
	A = B;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldac()		     /*	LD A,C */
{
	A = C;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldad()		     /*	LD A,D */
{
	A = D;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldae()		     /*	LD A,E */
{
	A = E;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldah()		     /*	LD A,H */
{
	A = H;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldal()		     /*	LD A,L */
{
	A = L;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldahl()		     /*	LD A,(HL) */
{
	A = *(ram + (H << 8) + L);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldba()		     /*	LD B,A */
{
	B = A;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldbb()		     /*	LD B,B */
{
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldbc()		     /*	LD B,C */
{
	B = C;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldbd()		     /*	LD B,D */
{
	B = D;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldbe()		     /*	LD B,E */
{
	B = E;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldbh()		     /*	LD B,H */
{
	B = H;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldbl()		     /*	LD B,L */
{
	B = L;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldbhl()		     /*	LD B,(HL) */
{
	B = *(ram + (H << 8) + L);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldca()		     /*	LD C,A */
{
	C = A;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldcb()		     /*	LD C,B */
{
	C = B;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldcc()		     /*	LD C,C */
{
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldcd()		     /*	LD C,D */
{
	C = D;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldce()		     /*	LD C,E */
{
	C = E;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldch()		     /*	LD C,H */
{
	C = H;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldcl()		     /*	LD C,L */
{
	C = L;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldchl()		     /*	LD C,(HL) */
{
	C = *(ram + (H << 8) + L);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldda()		     /*	LD D,A */
{
	D = A;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_lddb()		     /*	LD D,B */
{
	D = B;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_lddc()		     /*	LD D,C */
{
	D = C;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_lddd()		     /*	LD D,D */
{
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldde()		     /*	LD D,E */
{
	D = E;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_lddh()		     /*	LD D,H */
{
	D = H;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_lddl()		     /*	LD D,L */
{
	D = L;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_lddhl()		     /*	LD D,(HL) */
{
	D = *(ram + (H << 8) + L);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldea()		     /*	LD E,A */
{
	E = A;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldeb()		     /*	LD E,B */
{
	E = B;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldec()		     /*	LD E,C */
{
	E = C;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_lded()		     /*	LD E,D */
{
	E = D;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldee()		     /*	LD E,E */
{
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldeh()		     /*	LD E,H */
{
	E = H;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldel()		     /*	LD E,L */
{
	E = L;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldehl()		     /*	LD E,(HL) */
{
	E = *(ram + (H << 8) + L);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldha()		     /*	LD H,A */
{
	H = A;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldhb()		     /*	LD H,B */
{
	H = B;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldhc()		     /*	LD H,C */
{
	H = C;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldhd()		     /*	LD H,D */
{
	H = D;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldhe()		     /*	LD H,E */
{
	H = E;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldhh()		     /*	LD H,H */
{
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldhl()		     /*	LD H,L */
{
	H = L;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldhhl()		     /*	LD H,(HL) */
{
	H = *(ram + (H << 8) + L);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldla()		     /*	LD L,A */
{
	L = A;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldlb()		     /*	LD L,B */
{
	L = B;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldlc()		     /*	LD L,C */
{
	L = C;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldld()		     /*	LD L,D */
{
	L = D;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldle()		     /*	LD L,E */
{
	L = E;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldlh()		     /*	LD L,H */
{
	L = H;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldll()		     /*	LD L,L */
{
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ldlhl()		     /*	LD L,(HL) */
{
	L = *(ram + (H << 8) + L);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ldbcnn()		     /*	LD BC,nn */
{
	C = *PC++;
	B = *PC++;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_lddenn()		     /*	LD DE,nn */
{
	E = *PC++;
	D = *PC++;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_ldhlnn()		     /*	LD HL,nn */
{
	L = *PC++;
	H = *PC++;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_ldspnn()		     /*	LD SP,nn */
{
	STACK =	ram + *PC++;
	STACK += *PC++ << 8;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_ldsphl()		     /*	LD SP,HL */
{
	STACK =	ram + (H << 8) + L;
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_ldhlin()		     /*	LD HL,(nn) */
{
	register unsigned i;

	i = *PC++;
	i += *PC++ << 8;
	L = *(ram + i);
	H = *(ram + i +	1);
#ifdef WANT_TIM
	return(16L);
#endif
}

static  long op_ldinhl()		     /*	LD (nn),HL */
{
	register unsigned i;

	i = *PC++;
	i += *PC++ << 8;
	*(ram +	i) = L;
	*(ram +	i + 1) = H;
#ifdef WANT_TIM
	return(16L);
#endif
}

static  long op_incbc()		     /*	INC BC */
{
	C++;
	if (!C)
		B++;
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_incde()		     /*	INC DE */
{
	E++;
	if (!E)
		D++;
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_inchl()		     /*	INC HL */
{
	L++;
	if (!L)
		H++;
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_incsp()		     /*	INC SP */
{
	STACK++;
#ifdef WANT_SPC
	if (STACK > ram	+ 65535)
		STACK =	ram;
#endif
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_decbc()		     /*	DEC BC */
{
	C--;
	if (C == 0xff)
		B--;
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_decde()		     /*	DEC DE */
{
	E--;
	if (E == 0xff)
		D--;
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_dechl()		     /*	DEC HL */
{
	L--;
	if (L == 0xff)
		H--;
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_decsp()		     /*	DEC SP */
{
	STACK--;
#ifdef WANT_SPC
	if (STACK < ram)
		STACK =	ram + 65535;
#endif
#ifdef WANT_TIM
	return(6L);
#endif
}

static  long op_adhlbc()		     /*	ADD HL,BC */
{
	register  long i;

	i = (((( long)H) << 8) + L + ((( long)B) << 8) + C);
	H = i >> 8;
	L = i;
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_adhlde()		     /*	ADD HL,DE */
{
	register  long i;

	i = (((( long)H) << 8) + L + ((( long)D) << 8) + E);
	H = i >> 8;
	L = i;
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_adhlhl()		     /*	ADD HL,HL */
{
	register  long i;

	i = ((((( long)H) << 8) + L) << 1);
	H = i >> 8;
	L = i;
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_adhlsp()		     /*	ADD HL,SP */
{
	register  long i;

	i = (((( long)H) << 8) + L + STACK - ram);
	H = i >> 8;
	L = i;
	(i > 0xffffL) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_anda()		     /*	AND A */
{
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_andb()		     /*	AND B */
{
	A &= B;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_andc()		     /*	AND C */
{
	A &= C;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_andd()		     /*	AND D */
{
	A &= D;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ande()		     /*	AND E */
{
	A &= E;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_andh()		     /*	AND H */
{
	A &= H;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_andl()		     /*	AND L */
{
	A &= L;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_andhl()		     /*	AND (HL) */
{
	A &= *(ram + (H	<< 8) +	L);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_andn()		     /*	AND n */
{
	A &= *PC++;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= H_FLAG;
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(N_FLAG |	C_FLAG);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_ora()		     /*	OR A */
{
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_orb()		     /*	OR B */
{
	A |= B;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_orc()		     /*	OR C */
{
	A |= C;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ord()		     /*	OR D */
{
	A |= D;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ore()		     /*	OR E */
{
	A |= E;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_orh()		     /*	OR H */
{
	A |= H;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_orl()		     /*	OR L */
{
	A |= L;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_orhl()		     /*	OR (HL)	*/
{
	A |= *(ram + (H	<< 8) +	L);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_orn()		     /*	OR n */
{
	A |= *PC++;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_xora()		     /*	XOR A */
{
	A = 0;
	F &= ~(S_FLAG |	H_FLAG | N_FLAG	| C_FLAG);
	F |= Z_FLAG | P_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_xorb()		     /*	XOR B */
{
	A ^= B;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_xorc()		     /*	XOR C */
{
	A ^= C;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_xord()		     /*	XOR D */
{
	A ^= D;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_xore()		     /*	XOR E */
{
	A ^= E;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_xorh()		     /*	XOR H */
{
	A ^= H;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_xorl()		     /*	XOR L */
{
	A ^= L;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_xorhl()		     /*	XOR (HL) */
{
	A ^= *(ram + (H	<< 8) +	L);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_xorn()		     /*	XOR n */
{
	A ^= *PC++;
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	(parrity[A]) ? (F &= ~P_FLAG) :	(F |= P_FLAG);
	F &= ~(H_FLAG |	N_FLAG | C_FLAG);
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_adda()		     /*	ADD A,A	*/
{
	register int i;

	((A & 0xf) + (A	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	((A << 1) > 255) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	A = i =	((char)	A) << 1;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_addb()		     /*	ADD A,B	*/
{
	register int i;

	((A & 0xf) + (B	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + B > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) B;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_addc()		     /*	ADD A,C	*/
{
	register int i;

	((A & 0xf) + (C	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + C > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) C;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_addd()		     /*	ADD A,D	*/
{
	register int i;

	((A & 0xf) + (D	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + D > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) D;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_adde()		     /*	ADD A,E	*/
{
	register int i;

	((A & 0xf) + (E	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + E > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) E;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_addh()		     /*	ADD A,H	*/
{
	register int i;

	((A & 0xf) + (H	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + H > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) H;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_addl()		     /*	ADD A,L	*/
{
	register int i;

	((A & 0xf) + (L	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + L > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) L;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_addhl()		     /*	ADD A,(HL) */
{
	register int i;
	register BYTE P;

	P = *(ram + (H << 8) + L);
	((A & 0xf) + (P	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + P > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_addn()		     /*	ADD A,n	*/
{
	register int i;
	register BYTE P;

	P = *PC++;
	((A & 0xf) + (P	& 0xf) > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + P > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_adca()		     /*	ADC A,A	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((A & 0xf) + (A	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	((A << 1) + carry > 255) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	A = i =	(((char) A) << 1) + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_adcb()		     /*	ADC A,B	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((A & 0xf) + (B	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + B + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) B + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_adcc()		     /*	ADC A,C	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((A & 0xf) + (C	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + C + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) C + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_adcd()		     /*	ADC A,D	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((A & 0xf) + (D	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + D + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) D + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_adce()		     /*	ADC A,E	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((A & 0xf) + (E	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + E + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) E + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_adch()		     /*	ADC A,H	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((A & 0xf) + (H	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + H + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) H + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_adcl()		     /*	ADC A,L	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((A & 0xf) + (L	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + L + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) L + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_adchl()		     /*	ADC A,(HL) */
{
	register int i,	carry;
	register BYTE P;

	P = *(ram + (H << 8) + L);
	carry =	(F & C_FLAG) ? 1 : 0;
	((A & 0xf) + (P	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + P + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) P + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_adcn()		     /*	ADC A,n	*/
{
	register int i,	carry;
	register BYTE P;

	carry =	(F & C_FLAG) ? 1 : 0;
	P = *PC++;
	((A & 0xf) + (P	& 0xf) + carry > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	(A + P + carry > 255) ?	(F |= C_FLAG) :	(F &= ~C_FLAG);
	A = i =	(char) A + (char) P + carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_suba()		     /*	SUB A,A	*/
{
	A = 0;
	F &= ~(S_FLAG |	H_FLAG | P_FLAG	| C_FLAG);
	F |= Z_FLAG | N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_subb()		     /*	SUB A,B	*/
{
	register int i;

	((B & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(B > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) B;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_subc()		     /*	SUB A,C	*/
{
	register int i;

	((C & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(C > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) C;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_subd()		     /*	SUB A,D	*/
{
	register int i;

	((D & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(D > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) D;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_sube()		     /*	SUB A,E	*/
{
	register int i;

	((E & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(E > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) E;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_subh()		     /*	SUB A,H	*/
{
	register int i;

	((H & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(H > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) H;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_subl()		     /*	SUB A,L	*/
{
	register int i;

	((L & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(L > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) L;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_subhl()		     /*	SUB A,(HL) */
{
	register int i;
	register BYTE P;

	P = *(ram + (H << 8) + L);
	((P & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_subn()		     /*	SUB A,n	*/
{
	register int i;
	register BYTE P;

	P = *PC++;
	((P & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_sbca()		     /*	SBC A,A	*/
{
	if (F &	C_FLAG)	{
		F |= S_FLAG | H_FLAG | N_FLAG |	C_FLAG;
		F &= ~(Z_FLAG |	P_FLAG);
		A = 255;
	} else {
		F |= Z_FLAG | N_FLAG;
		F &= ~(S_FLAG |	H_FLAG | P_FLAG	| C_FLAG);
		A = 0;
	}
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_sbcb()		     /*	SBC A,B	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((B & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(B + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) B - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_sbcc()		     /*	SBC A,C	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((C & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(C + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) C - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_sbcd()		     /*	SBC A,D	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((D & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(D + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) D - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_sbce()		     /*	SBC A,E	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((E & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(E + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) E - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_sbch()		     /*	SBC A,H	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((H & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(H + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) H - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_sbcl()		     /*	SBC A,L	*/
{
	register int i,	carry;

	carry =	(F & C_FLAG) ? 1 : 0;
	((L & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(L + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) L - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_sbchl()		     /*	SBC A,(HL) */
{
	register int i,	carry;
	register BYTE P;

	P = *(ram + (H << 8) + L);
	carry =	(F & C_FLAG) ? 1 : 0;
	((P & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) P - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_sbcn()		     /*	SBC A,n	*/
{
	register int i,	carry;
	register BYTE P;

	P = *PC++;
	carry =	(F & C_FLAG) ? 1 : 0;
	((P & 0xf) + carry > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P + carry > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	A = i =	(char) A - (char) P - carry;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_cpa()		     /*	CP A */
{
	F &= ~(S_FLAG |	H_FLAG | P_FLAG	| C_FLAG);
	F |= Z_FLAG | N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_cpb()		     /*	CP B */
{
	register int i;

	((B & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(B > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) B;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_cpc()		     /*	CP C */
{
	register int i;

	((C & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(C > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) C;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_cpd()		     /*	CP D */
{
	register int i;

	((D & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(D > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) D;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_cpe()		     /*	CP E */
{
	register int i;

	((E & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(E > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) E;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_cph()		     /*	CP H */
{
	register int i;

	((H & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(H > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) H;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_cplr()		     /*	CP L */
{
	register int i;

	((L & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(L > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) L;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_cphl()		     /*	CP (HL)	*/
{
	register int i;
	register BYTE P;

	P = *(ram + (H << 8) + L);
	((P & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_cpn()		     /*	CP n */
{
	register int i;
	register BYTE P;

	P = *PC++;
	((P & 0xf) > (A	& 0xf))	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(P > A)	? (F |=	C_FLAG)	: (F &=	~C_FLAG);
	i = (char) A - (char) P;
	(i < -128 || i > 127) ?	(F |= P_FLAG) :	(F &= ~P_FLAG);
	(i & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(i) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(7L);
#endif
}

static  long op_inca()		     /*	INC A */
{
	((A & 0xf) + 1 > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	A++;
	(A == 128) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_incb()		     /*	INC B */
{
	((B & 0xf) + 1 > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	B++;
	(B == 128) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_incc()		     /*	INC C */
{
	((C & 0xf) + 1 > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	C++;
	(C == 128) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_incd()		     /*	INC D */
{
	((D & 0xf) + 1 > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	D++;
	(D == 128) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_ince()		     /*	INC E */
{
	((E & 0xf) + 1 > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	E++;
	(E == 128) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_inch()		     /*	INC H */
{
	((H & 0xf) + 1 > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	H++;
	(H == 128) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_incl()		     /*	INC L */
{
	((L & 0xf) + 1 > 0xf) ?	(F |= H_FLAG) :	(F &= ~H_FLAG);
	L++;
	(L == 128) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_incihl()		     /*	INC (HL) */
{
	register BYTE *p;

	p = ram	+ (H <<	8) + L;
	((*p & 0xf) + 1	> 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	(*p)++;
	(*p == 128) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	F &= ~N_FLAG;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_deca()		     /*	DEC A */
{
	((A - 1	& 0xf) == 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	A--;
	(A == 127) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(A & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(A) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_decb()		     /*	DEC B */
{
	((B - 1	& 0xf) == 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	B--;
	(B == 127) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(B & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(B) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_decc()		     /*	DEC C */
{
	((C - 1	& 0xf) == 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	C--;
	(C == 127) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(C & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(C) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_decd()		     /*	DEC D */
{
	((D - 1	& 0xf) == 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	D--;
	(D == 127) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(D & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(D) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_dece()		     /*	DEC E */
{
	((E - 1	& 0xf) == 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	E--;
	(E == 127) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(E & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(E) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_dech()		     /*	DEC H */
{
	((H - 1	& 0xf) == 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	H--;
	(H == 127) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(H & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(H) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_decl()		     /*	DEC L */
{
	((L - 1	& 0xf) == 0xf) ? (F |= H_FLAG) : (F &= ~H_FLAG);
	L--;
	(L == 127) ? (F	|= P_FLAG) : (F	&= ~P_FLAG);
	(L & 128) ? (F |= S_FLAG) : (F &= ~S_FLAG);
	(L) ? (F &= ~Z_FLAG) : (F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_decihl()		     /*	DEC (HL) */
{
	register BYTE *p;

	p = ram	+ (H <<	8) + L;
	((*p - 1 & 0xf)	== 0xf)	? (F |=	H_FLAG)	: (F &=	~H_FLAG);
	(*p)--;
	(*p == 127) ? (F |= P_FLAG) : (F &= ~P_FLAG);
	(*p & 128) ? (F	|= S_FLAG) : (F	&= ~S_FLAG);
	(*p) ? (F &= ~Z_FLAG) :	(F |= Z_FLAG);
	F |= N_FLAG;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_rlca()		     /*	RLCA */
{
	register int i;

	i = (A & 128) ?	1 : 0;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	A <<= 1;
	A |= i;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_rrca()		     /*	RRCA */
{
	register int i;

	i = A &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	A >>= 1;
	if (i) A |= 128;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_rla()		     /*	RLA */
{
	register int old_c_flag;

	old_c_flag = F & C_FLAG;
	(A & 128) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	A <<= 1;
	if (old_c_flag)	A |= 1;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_rra()		     /*	RRA */
{
	register int i,	old_c_flag;

	old_c_flag = F & C_FLAG;
	i = A &	1;
	(i) ? (F |= C_FLAG) : (F &= ~C_FLAG);
	F &= ~(H_FLAG |	N_FLAG);
	A >>= 1;
	if (old_c_flag)	A |= 128;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_exdehl()		     /*	EX DE,HL */
{
	register unsigned i;

	i = D;
	D = H;
	H = i;
	i = E;
	E = L;
	L = i;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_exafaf()		     /*	EX AF,AF' */
{
	register unsigned i;

	i = A;
	A = A_;
	A_ = i;
	i = F;
	F = F_;
	F_ = i;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_exx()		     /*	EXX */
{
	register unsigned i;

	i = B;
	B = B_;
	B_ = i;
	i = C;
	C = C_;
	C_ = i;
	i = D;
	D = D_;
	D_ = i;
	i = E;
	E = E_;
	E_ = i;
	i = H;
	H = H_;
	H_ = i;
	i = L;
	L = L_;
	L_ = i;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_exsphl()		     /*	EX (SP),HL */
{
	register int i;

	i = *STACK;
	*STACK = L;
	L = i;
	i = *(STACK + 1);
	*(STACK	+ 1) = H;
	H = i;
#ifdef WANT_TIM
	return(19L);
#endif
}

static  long op_pushaf()		     /*	PUSH AF	*/
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = A;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = F;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_pushbc()		     /*	PUSH BC	*/
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = B;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = C;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_pushde()		     /*	PUSH DE	*/
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = D;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = E;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_pushhl()		     /*	PUSH HL	*/
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = H;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = L;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_popaf()		     /*	POP AF */
{
	F = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
	A = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_popbc()		     /*	POP BC */
{
	C = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
	B = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_popde()		     /*	POP DE */
{
	E = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
	D = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_pophl()		     /*	POP HL */
{
	L = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
	H = *STACK++;
#ifdef WANT_SPC
	if (STACK >= ram + 65536L)
		STACK =	ram;
#endif
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jp()		     /*	JP */
{
	register unsigned i;

	i = *PC++;
	i += *PC << 8;
	PC = ram + i;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jphl()		     /*	JP (HL)	*/
{
	PC = ram + (H << 8) + L;
#ifdef WANT_TIM
	return(4L);
#endif
}

static  long op_jr()		     /*	JR */
{
	PC += (char) *PC + 1;
#ifdef WANT_TIM
	return(12L);
#endif
}

static  long op_djnz()		     /*	DJNZ */
{
	if (--B) {
		PC += (char) *PC + 1;
#ifdef WANT_TIM
		return(13L);
#endif
	} else {
		PC++;
#ifdef WANT_TIM
		return(8L);
#endif
	}
}

static  long op_call()		     /*	CALL */
{
	register unsigned i;

	i = *PC++;
	i += *PC++ << 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram + i;
#ifdef WANT_TIM
	return(17L);
#endif
}

static  long op_ret()		     /*	RET */
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
	return(10L);
#endif
}

static  long op_jpz()		     /*	JP Z,nn	*/
{
	register unsigned i;

	if (F &	Z_FLAG)	{
		i = *PC++;
		i += *PC++ << 8;
		PC = ram + i;
	} else
		PC += 2;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jpnz()		     /*	JP NZ,nn */
{
	register unsigned i;

	if (!(F	& Z_FLAG)) {
		i = *PC++;
		i += *PC++ << 8;
		PC = ram + i;
	} else
		PC += 2;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jpc()		     /*	JP C,nn	*/
{
	register unsigned i;

	if (F &	C_FLAG)	{
		i = *PC++;
		i += *PC++ << 8;
		PC = ram + i;
	} else
		PC += 2;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jpnc()		     /*	JP NC,nn */
{
	register unsigned i;

	if (!(F	& C_FLAG)) {
		i = *PC++;
		i += *PC++ << 8;
		PC = ram + i;
	} else
		PC += 2;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jppe()		     /*	JP PE,nn */
{
	register unsigned i;

	if (F &	P_FLAG)	{
		i = *PC++;
		i += *PC++ << 8;
		PC = ram + i;
	} else
		PC += 2;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jppo()		     /*	JP PO,nn */
{
	register unsigned i;

	if (!(F	& P_FLAG)) {
		i = *PC++;
		i += *PC++ << 8;
		PC = ram + i;
	} else
		PC += 2;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jpm()		     /*	JP M,nn	*/
{
	register unsigned i;

	if (F &	S_FLAG)	{
		i = *PC++;
		i += *PC++ << 8;
		PC = ram + i;
	} else
		PC += 2;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_jpp()		     /*	JP P,nn	*/
{
	register unsigned i;

	if (!(F	& S_FLAG)) {
		i = *PC++;
		i += *PC++ << 8;
		PC = ram + i;
	} else
		PC += 2;
#ifdef WANT_TIM
	return(10L);
#endif
}

static  long op_calz()		     /*	CALL Z,nn */
{
	register unsigned i;

	if (F &	Z_FLAG)	{
		i = *PC++;
		i += *PC++ << 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram);
		PC = ram + i;
#ifdef WANT_TIM
		return(17L);
#endif
	} else {
		PC += 2;
#ifdef WANT_TIM
		return(10L);
#endif
	}
}

static  long op_calnz()		     /*	CALL NZ,nn */
{
	register unsigned i;

	if (!(F	& Z_FLAG)) {
		i = *PC++;
		i += *PC++ << 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram);
		PC = ram + i;
#ifdef WANT_TIM
		return(17L);
#endif
	} else {
		PC += 2;
#ifdef WANT_TIM
		return(10L);
#endif
	}
}

static  long op_calc()		     /*	CALL C,nn */
{
	register unsigned i;

	if (F &	C_FLAG)	{
		i = *PC++;
		i += *PC++ << 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram);
		PC = ram + i;
#ifdef WANT_TIM
		return(17L);
#endif
	} else {
		PC += 2;
#ifdef WANT_TIM
		return(10L);
#endif
	}
}

static  long op_calnc()		     /*	CALL NC,nn */
{
	register unsigned i;

	if (!(F	& C_FLAG)) {
		i = *PC++;
		i += *PC++ << 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram);
		PC = ram + i;
#ifdef WANT_TIM
		return(17L);
#endif
	} else {
		PC += 2;
#ifdef WANT_TIM
		return(10L);
#endif
	}
}

static  long op_calpe()		     /*	CALL PE,nn */
{
	register unsigned i;

	if (F &	P_FLAG)	{
		i = *PC++;
		i += *PC++ << 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram);
		PC = ram + i;
#ifdef WANT_TIM
		return(17L);
#endif
	} else {
		PC += 2;
#ifdef WANT_TIM
		return(10L);
#endif
	}
}

static  long op_calpo()		     /*	CALL PO,nn */
{
	register unsigned i;

	if (!(F	& P_FLAG)) {
		i = *PC++;
		i += *PC++ << 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram);
		PC = ram + i;
#ifdef WANT_TIM
		return(17L);
#endif
	} else {
		PC += 2;
#ifdef WANT_TIM
		return(10L);
#endif
	}
}

static  long op_calm()		     /*	CALL M,nn */
{
	register unsigned i;

	if (F &	S_FLAG)	{
		i = *PC++;
		i += *PC++ << 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram);
		PC = ram + i;
#ifdef WANT_TIM
		return(17L);
#endif
	} else {
		PC += 2;
#ifdef WANT_TIM
		return(10L);
#endif
	}
}

static  long op_calp()		     /*	CALL P,nn */
{
	register unsigned i;

	if (!(F	& S_FLAG)) {
		i = *PC++;
		i += *PC++ << 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
		if (STACK <= ram)
			STACK =	ram + 65536L;
#endif
		*--STACK = (PC - ram);
		PC = ram + i;
#ifdef WANT_TIM
		return(17L);
#endif
	} else {
		PC += 2;
#ifdef WANT_TIM
		return(10L);
#endif
	}
}

static  long op_retz()		     /*	RET Z */
{
	register unsigned i;

	if (F &	Z_FLAG)	{
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
		return(11L);
#endif
	} else {
#ifdef WANT_TIM
		return(5L);
#endif
	}
}

static  long op_retnz()		     /*	RET NZ */
{
	register unsigned i;

	if (!(F	& Z_FLAG)) {
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
		return(11L);
#endif
	} else {
#ifdef WANT_TIM
		return(5L);
#endif
	}
}

static  long op_retc()		     /*	RET C */
{
	register unsigned i;

	if (F &	C_FLAG)	{
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
		return(11L);
#endif
	} else {
#ifdef WANT_TIM
		return(5L);
#endif
	}
}

static  long op_retnc()		     /*	RET NC */
{
	register unsigned i;

	if (!(F	& C_FLAG)) {
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
		return(11L);
#endif
	} else {
#ifdef WANT_TIM
		return(5L);
#endif
	}
}

static  long op_retpe()		     /*	RET PE */
{
	register unsigned i;

	if (F &	P_FLAG)	{
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
		return(11L);
#endif
	} else {
#ifdef WANT_TIM
		return(5L);
#endif
	}
}

static  long op_retpo()		     /*	RET PO */
{
	register unsigned i;

	if (!(F	& P_FLAG)) {
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
		return(11L);
#endif
	} else {
#ifdef WANT_TIM
		return(5L);
#endif
	}
}

static  long op_retm()		     /*	RET M */
{
	register unsigned i;

	if (F &	S_FLAG)	{
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
		return(11L);
#endif
	} else {
#ifdef WANT_TIM
		return(5L);
#endif
	}
}

static  long op_retp()		     /*	RET P */
{
	register unsigned i;

	if (!(F	& S_FLAG)) {
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
		return(11L);
#endif
	} else {
#ifdef WANT_TIM
		return(5L);
#endif
	}
}

static  long op_jrz()		     /*	JR Z,n */
{
	if (F &	Z_FLAG)	{
		PC += (char) *PC + 1;
#ifdef WANT_TIM
		return(12L);
#endif
	} else {
		PC++;
#ifdef WANT_TIM
		return(7L);
#endif
	}
}

static  long op_jrnz()		     /*	JR NZ,n	*/
{
	if (!(F	& Z_FLAG)) {
		PC += (char) *PC + 1;
#ifdef WANT_TIM
		return(12L);
#endif
	} else {
		PC++;
#ifdef WANT_TIM
		return(7L);
#endif
	}
}

static  long op_jrc()		     /*	JR C,n */
{
	if (F &	C_FLAG)	{
		PC += (char) *PC + 1;
#ifdef WANT_TIM
		return(12L);
#endif
	} else {
		PC++;
#ifdef WANT_TIM
		return(7L);
#endif
	}
}

static  long op_jrnc()		     /*	JR NC,n	*/
{
	if (!(F	& C_FLAG)) {
		PC += (char) *PC + 1;
#ifdef WANT_TIM
		return(12L);
#endif
	} else {
		PC++;
#ifdef WANT_TIM
		return(7L);
#endif
	}
}

static  long op_rst00()		     /*	RST 00 */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_rst08()		     /*	RST 08 */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram + 0x08;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_rst10()		     /*	RST 10 */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram + 0x10;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_rst18()		     /*	RST 18 */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram + 0x18;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_rst20()		     /*	RST 20 */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram + 0x20;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_rst28()		     /*	RST 28 */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram + 0x28;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_rst30()		     /*	RST 30 */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram + 0x30;
#ifdef WANT_TIM
	return(11L);
#endif
}

static  long op_rst38()		     /*	RST 38 */
{
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram) >> 8;
#ifdef WANT_SPC
	if (STACK <= ram)
		STACK =	ram + 65536L;
#endif
	*--STACK = (PC - ram);
	PC = ram + 0x38;
#ifdef WANT_TIM
	return(11L);
#endif
}
