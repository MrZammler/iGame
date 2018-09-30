/*
 * POSIX Compatibility Library for AmigaOS
 *
 * Written by Frank Wille <frank@phoenix.owl.de> in 2003
 *
 * $Id: strdup.c,v 1.2 2005/02/27 13:45:27 phx Exp $
 */

#include <stdlib.h>
#include <string.h>

char* strdup(const char* s)
{
	char* new;

	if ((new = malloc(strlen(s) + 1)))
		strcpy(new,s);
	return new;
}
