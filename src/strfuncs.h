


#ifndef _STR_FUNCS_H
#define _STR_FUNCS_H

char *strcasestr(const char *, const char *);
char* strdup(const char *); // TODO: Possible obsolete. Maybe needed on some old tools. Better move it there
void string_to_lower(char *);
char** my_split(char *, char *);
int get_delimiter_position(const char *);
const char* add_spaces_to_string(const char *);

#endif
