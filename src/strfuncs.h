/*
  strfuncs.h
  String functions header for iGame

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

#ifndef _STR_FUNCS_H
#define _STR_FUNCS_H

char *strcasestr(const char *, const char *);
char* strdup(const char *);
void string_to_lower(char *);
char** my_split(char *, char *);
char **str_split(char *, const char);
int get_delimiter_position(const char *);
void add_spaces_to_string(const char *, char *, int);
STRPTR substring(STRPTR, int, int);
STRPTR GetMBString(ULONG);
BOOL isStringEmpty(const char *);
BOOL isNumeric(const char *);
int strlcpy(char *, const char *, int);
int strlcat(char *, const char *, int);

#endif
