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
 *	This modul contains some commonly used functions
 */

#include <ctype.h>
#include "sim.h"

/*
 *	atoi for hexadecimal numbers
 */
exatoi(str)
register char *str;
{
	register int num = 0;

	while (isxdigit(*str)) {
		num *= 16;
		if (*str <= '9')
			num += *str - '0';
		else
			num += toupper(*str) - '7';
		str++;
	}
	return(num);
}

/*
 *	Wait for a single keystroke without echo
 */
getkey()
{
//jar
/*
	register int c;
#if defined(COHERENT) && !defined(_I386)
	struct sgttyb old_term,	new_term;
#else
	struct termio old_term, new_term;
#endif

#if defined(COHERENT) && !defined(_I386)
	gtty(0, &old_term);
	new_term = old_term;
	new_term.sg_flags |= CBREAK;
	new_term.sg_flags &= ~ECHO;
	stty(0, &new_term);
#else
	ioctl(0, TCGETA, &old_term);
	new_term = old_term;
	new_term.c_lflag &= ~(ICANON | ECHO);
	new_term.c_cc[4] = 1;
#endif
	c = getchar();
#if defined(COHERENT) && !defined(_I386)
	stty(0, &old_term);
#else
	ioctl(0, TCSETAW, &old_term);
#endif
	return(c);
*/
}
