/*
 * strcasestr() implementation for AmigaOS
 *
 * Cribbed from https://stackoverflow.com/a/45109082/5697 and renamed from
 * stristr() to strcasestr() to match GCC function of same name
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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
