/*
  chipsetList.h
  chipset list functions header for iGame

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

#ifndef _CHIPSET_LIST_H
#define _CHIPSET_LIST_H

void chipsetListAddTail(chipsetList *);
void chipsetListPrint(void);
chipsetList *chipsetListSearchByTitle(char *, unsigned int);
chipsetList *getChipsetListHead(void);
void emptyChipsetList(void);
int chipsetListNodeCount(int);
void addChipsetInList(char *);

#endif
