/*
  slavesList.h
  slaves list functions header for iGame

  Copyright (c) 2018-2022, Emmanuel Vasilakis

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

#ifndef _SLAVES_LIST_H
#define _SLAVES_LIST_H


int isListEmpty(void *);
void slavesListAddHead(slavesList *);
void slavesListAddTail(slavesList *);
void slavesListPrint(void);
void slavesListClearTitles(void);
slavesList *slavesListSearchByPath(char *, unsigned int);
slavesList *slavesListSearchByTitle(char *, unsigned int);
// unsigned int slavesListCountInstancesByTitle(char *, unsigned int);
void slavesListCountInstancesByTitle(slavesList *);
// static void showSlavesList(void);
slavesList *getSlavesListHead(void);
void setSlavesListBuffer(slavesList *);
slavesList *getSlavesListBuffer(void);

#endif
