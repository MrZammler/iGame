
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "strfuncs.h"

/*
 * strcasestr() implementation for AmigaOS
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
			for (size_t i = 0;;) {
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
void string_to_lower(char* slave)
{
	for (int i = 0; i <= strlen(slave) - 1; i++)
		slave[i] = tolower(slave[i]);
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
const char* add_spaces_to_string(const char* input)
{
	char input_string[100];
	strcpy(input_string, input);
	char* output = (char*)malloc(sizeof input_string * 2);

	// Special case for the first character, we don't touch it
	output[0] = input_string[0];

	unsigned int output_index = 1;
	unsigned int input_index = 1;
	unsigned int capital_found = 0;
	unsigned int number_found = 0;
	while (input_string[input_index])
	{
		if (isspace(input_string[input_index]))
			return input;

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

	return output;
}
