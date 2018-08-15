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

APTR filter_change(struct Hook* a0, APTR a2, APTR a1);
APTR game_double_click(struct Hook* a0, APTR a2, APTR a1);
APTR menu_scan(struct Hook* a0, APTR a2, APTR a1);
APTR game_click(struct Hook* a0, APTR a2, APTR a1);


APTR app_start(struct Hook* a0, APTR a2, APTR a1);
APTR repo_add(struct Hook* a0, APTR a2, APTR a1);
APTR repo_remove(struct Hook* a0, APTR a2, APTR a1);

APTR repo_stop(struct Hook* a0, APTR a2, APTR a1);

APTR menu_game_properties(struct Hook* a0, APTR a2, APTR a1);
APTR game_properties_ok(struct Hook* a0, APTR a2, APTR a1);

APTR menu_add_non_whdload(struct Hook* a0, APTR a2, APTR a1);
APTR non_whdload_ok(struct Hook* a0, APTR a2, APTR a1);

APTR genres_click(struct Hook* a0, APTR a2, APTR a1);

APTR menu_show_hidden(struct Hook* a0, APTR a2, APTR a1);

//APTR MenuMUISettings( struct Hook *a0, APTR a2, APTR a1 );
