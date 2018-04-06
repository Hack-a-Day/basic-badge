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
 * 25-JUN-92 Release 1.7 comments in english and ported to COHERENT 4.0
 */

/*
 *	Declaration of variables in simglb.c
 */

extern BYTE	A,B,C,D,E,H,L,A_,B_,C_,D_,E_,H_,L_,*PC,*STACK,I,IFF;
extern WORD	IX,IY;
extern int	F,F_;
extern long	R;

extern BYTE	ram[],*wrk_ram;

extern int	s_flag,l_flag,m_flag,x_flag,break_flag,cpu_state,cpu_error,
		int_type,int_mode,cntl_c,cntl_bs,parrity[],sb_next;

extern char	xfn[];

#ifdef HISIZE
extern struct	history	his[];
extern int	h_next,	h_flag;
#endif

#ifdef SBSIZE
extern struct	softbreak soft[];
#endif

#ifdef WANT_TIM
extern long	t_states;
extern int	t_flag;
extern BYTE	*t_start, *t_end;
#endif

#if defined(COHERENT) && !defined(_I386)
extern char adr_err[];
#endif
