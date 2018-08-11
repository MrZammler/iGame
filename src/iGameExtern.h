/*
  iGameExtern.h
  Function protos for iGame
  
  Copyright (c) 2016, Emmanuel Vasilakis
  
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

#pragma once
#include <utility/hooks.h>

APTR FilterChange(struct Hook* a0, APTR a2, APTR a1);
APTR GameDoubleClick(struct Hook* a0, APTR a2, APTR a1);
APTR MenuScan(struct Hook* a0, APTR a2, APTR a1);
APTR GameClick(struct Hook* a0, APTR a2, APTR a1);


APTR AppStart(struct Hook* a0, APTR a2, APTR a1);
APTR RepoAdd(struct Hook* a0, APTR a2, APTR a1);
APTR RepoRemove(struct Hook* a0, APTR a2, APTR a1);
APTR RepoStart(struct Hook* a0, APTR a2, APTR a1);
APTR RepoStop(struct Hook* a0, APTR a2, APTR a1);

APTR MenuGameProperties(struct Hook* a0, APTR a2, APTR a1);
APTR GamePropertiesOk(struct Hook* a0, APTR a2, APTR a1);

APTR MenuAddNonWhdload(struct Hook* a0, APTR a2, APTR a1);
APTR NonWhdloadOk(struct Hook* a0, APTR a2, APTR a1);

APTR GenresClick(struct Hook* a0, APTR a2, APTR a1);

APTR MenuShowHidden(struct Hook* a0, APTR a2, APTR a1);

//APTR MenuMUISettings( struct Hook *a0, APTR a2, APTR a1 );
