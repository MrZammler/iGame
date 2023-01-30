/*
  fsfuncs.h
  Filesystem functions header for iGame

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

#ifndef _FS_FUNCS_H
#define _FS_FUNCS_H

void getParentPath(char *, char *);
void getNameFromPath(char *, char *, unsigned int);
void getFullPath(const char *, char *);
void strip_path(const char *, char *);
char *get_slave_from_path(char *, int, char *);
BOOL check_path_exists(char *);
BOOL get_filename(const char *, const char *, const BOOL);
void load_games_csv_list(const char *);
void slavesListLoadFromCSV(char *);
void save_to_csv(const char *, const int);
void slavesListSaveToCSV(const char *);
int get_title_from_slave(char *, char *);
void getTitleFromPath(char *, char *);
const char *get_directory_name(const char *);
const char *get_directory_path(const char *);
char *get_executable_name(int, char **);
void open_current_dir(void);
void get_path(char *, char *);
BOOL isPathFolder(char *);
void getIconTooltypes(char *, char *);
void setIconTooltypes(char *, char *);

#endif
