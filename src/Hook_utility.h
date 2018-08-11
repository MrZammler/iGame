/*
  Hook_utility.h
  Hook header file for iGame
  
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

/*  Prototypes for Hook_Utility */

#pragma once
#include <utility/hooks.h>

/* prototype for C_Function

	a0 points to a struct hook.
	See MUI AutoDocs in order to know what registers (a2,a1) point.

*/
typedef APTR (*proto_c_function)(struct Hook* a0, APTR a2, APTR a1);

/*
	Don't forget to allocate the struct Hook.
	Data is the h_Data field of the struct Hook.
*/
void InstallHook(struct Hook* hook, proto_c_function c_function, APTR Data);
