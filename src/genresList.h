/*
  genresList.h
  genres list functions header for iGame

  Copyright (c) 2018-2023, Emmanuel Vasilakis

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

#ifndef _GENRES_LIST_H
#define _GENRES_LIST_H

void genresListAddTail(genresList *);
void genresListPrint(void);
genresList *genresListSearchByTitle(char *, unsigned int);
genresList *getGenresListHead(void);
void emptyGenresList(void);
int genresListNodeCount(int);
void addGenreInList(char *);

#endif
