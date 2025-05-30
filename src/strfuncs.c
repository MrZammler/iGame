/*
  strfuncs.c
  String functions source for iGame

  Copyright (c) 2018, Emmanuel Vasilakis

  This file is part of iGame.

  iGame is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  iGame is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with iGame. If not, see <http://www.gnu.org/licenses/>.
*/

#include <proto/wb.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// #define iGame_CODE
#define iGame_NUMBERS
#define iGame_ARRAY
#include "iGame_strings.h"
#include "iGameExtern.h"

#include "strfuncs.h"

// extern struct LocaleBase *LocaleBase;
#ifndef __amigaos4__
	extern struct Library		*LocaleBase;
#else
	extern struct LocaleIFace	*ILocale;
#endif
extern struct Catalog *Catalog;

/*
 * strcasestr() implementation for AmigaOS
 *
 * This function finds the first occurrence of the substring needle in the
 * string haystack, ignoring the case of both arguments. The terminating
 * null bytes ('\0') are not compared.
 *
 * Cribbed from https://stackoverflow.com/a/45109082/5697 and renamed from
 * stristr() to strcasestr() to match GCC function of same name
 */
char *strcasestr(const char *haystack, const char *needle)
{
	int c = tolower((unsigned char)*needle);
	if (c == '\0')
		return (char *)haystack;

	for (; *haystack; haystack++) {
		if (tolower((unsigned char)*haystack) == c) {
			for (int i = 0;;) {
				if (needle[++i] == '\0')
					return (char *)haystack;
				if (tolower((unsigned char)haystack[i]) != tolower((unsigned char)needle[i]))
					break;
			}
		}
	}

	return NULL;
}


/*
 * POSIX Compatibility Library for AmigaOS
 *
 * Written by Frank Wille <frank@phoenix.owl.de> in 2003
 *
 * $Id: strdup.c,v 1.2 2005/02/27 13:45:27 phx Exp $
 */
char* strdup(const char* s)
{
	char* new;

	if ((new = malloc(strlen(s) + 1)))
		strcpy(new, s);
	return new;
}


/*
 * Convert a string to lower case
 */
void string_to_lower(char *text)
{
	for (int i = 0; i <= strlen(text) - 1; i++)
		text[i] = tolower(text[i]);
}


/*
 * Splits a string using spl
 */
char** my_split(char* str, char* spl)
{
	char **ret, *buffer[256], buf[4096];
	int i;

	if (!spl)
	{
		ret = (char **)malloc(2 * sizeof(char *));
		ret[0] = (char *)strdup(str);
		ret[1] = NULL;
		return (ret);
	}

	int count = 0;

	char* fptr = str;
	const int spl_len = strlen(spl);
	char* sptr = strstr(fptr, spl);
	while (sptr)
	{
		i = sptr - fptr;
		memcpy(buf, fptr, i);
		buf[i] = '\0';
		buffer[count++] = (char *)strdup(buf);
		fptr = sptr + spl_len;
		sptr = strstr(fptr, spl);
	}
	sptr = strchr(fptr, '\0');
	i = sptr - fptr;
	memcpy(buf, fptr, i);
	buf[i] = '\0';
	buffer[count++] = (char *)strdup(buf);

	ret = (char **)malloc((count + 1) * sizeof(char *));

	for (i = 0; i < count; i++)
	{
		ret[i] = buffer[i];
	}
	ret[count] = NULL;

	return ret;
}

char **str_split(char *a_str, const char a_delim)
{
	char** result    = 0;
	size_t count     = 0;
	char* tmp        = a_str;
	char* last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	/* Count how many elements will be extracted. */
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	/* Add space for trailing token. */
	count += last_comma < (a_str + strlen(a_str) - 1);

	/* Add space for terminating null string so caller
		knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char*) * count);

	if (result)
	{
		size_t idx  = 0;
		char* token = strtok(a_str, delim);

		while (token)
		{
			// assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		// assert(idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
}

int get_delimiter_position(const char* str)
{
	char* delimiter = strrchr(str, '/');
	if (!delimiter)
		delimiter = strrchr(str, ':');

	if (!delimiter)
	{
		return 0;
	}

	return delimiter - str;
}

// Add spaces to a string, based on letter capitalization and numbers
// E.g. input "A10TankKiller2Disk" -> "A10 Tank Killer 2 Disk"
void add_spaces_to_string(const char* input, char *result, int resultSize)
{
	char input_string[MAX_SLAVE_TITLE_SIZE];
	strcpy(input_string, input);
	char* output = (char*)malloc(sizeof(input_string) * 2);

	// Special case for the first character, we don't touch it
	output[0] = input_string[0];

	unsigned int output_index = 1;
	unsigned int input_index = 1;
	unsigned int capital_found = 0;
	unsigned int number_found = 0;
	while (input_string[input_index])
	{
		if (isspace(input_string[input_index]))
		{
			strncpy(result, input, resultSize);
			free(output);
			return;
		}

		if (isdigit(input_string[input_index]))
		{
			if (number_found < input_index - 1)
			{
				output[output_index] = ' ';
				output_index++;
				output[output_index] = input_string[input_index];
			}
			number_found = input_index;
		}

		else if (isupper(input_string[input_index]))
		{
			if (capital_found < input_index - 1)
			{
				output[output_index] = ' ';
				output_index++;
				output[output_index] = input_string[input_index];
			}
			capital_found = input_index;
		}

		output[output_index] = input_string[input_index];
		output_index++;
		input_index++;
	}
	output[output_index] = '\0';

	strncpy(result, output, resultSize);
	free(output);
}

STRPTR substring(STRPTR string, int position, int length)
{
	STRPTR p;
	unsigned int c;
	if (position < 0) position = 0;
	if (length < 0) length = strlen(string) + length;

	p = malloc(length+1);

	if (p == NULL)
	{
		return NULL;
	}

	for (c = 0; c < length; c++)
	{
		*(p+c) = *(string + position);
		string++;
	}

	*(p+c) = '\0';

	return p;
}

STRPTR GetMBString(ULONG refId)
{
	const struct iGame_ArrayType *t = iGame_Array + refId;

	#ifndef __amigaos4__
	return LocaleBase ? GetCatalogStr(Catalog, t->cca_ID, t->cca_Str) : t->cca_Str;
	#else
	return GetCatalogStr(Catalog, t->cca_ID, t->cca_Str);
	#endif
}

BOOL isStringEmpty(const char *string)
{
	if (string == NULL)
		return TRUE;

	return string[0] == '\0';
}

BOOL isNumeric(const char *string)
{
	while(*string)
	{
		if(*string < '0' || *string > '9')
			return FALSE;
		++string;
	}
	return TRUE;
}

#include <stddef.h>

// strlcpy implementation
int strlcpy(char *dst, const char *src, int dstsize)
{
	const char *s = src;
	int n = dstsize;

	// Copy as many bytes as will fit
	if (n != 0) {
		while (--n != 0) {
			if ((*dst++ = *src++) == '\0') {
				break;
			}
		}
	}

	// Not enough room in dst; add NUL
	if (n == 0) {
		if (dstsize != 0) {
			*dst = '\0'; // NULL-terminate
		}
		while (*src++){}  // Traverse remaining source characters
	}

	return src - s - 1; // Return strlen(src)
}

// strlcat implementation
int strlcat(char *dst, const char *src, int dstsize)
{
	const char *s = src;
	char *d = dst;
	int n = dstsize;
	int dlen;

	// Find the end of dst and adjust bytes left but don't go past end
	while (n-- != 0 && *d != '\0') {
		d++;
	}

	dlen = d - dst;
	n = dstsize - dlen;

	if (n == 0) {
		return dlen + strlen(src);
	}

	while (*src != '\0') {
		if (n != 1) {
			*d++ = *src;
			n--;
		}
		src++;
	}

	*d = '\0'; // Null-terminate

	return dlen + (src - s); // Total length of string tried to create
}
