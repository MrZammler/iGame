/*
  version.h
  Version manipulation header for iGame

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

#ifndef MAJOR_VERS
#define MAJOR_VERS 2
#endif

#ifndef MINOR_VERS
#define MINOR_VERS 2
#endif

#ifndef PATCH_VERS
#define PATCH_VERS 0
#endif

#ifndef RELEASE_DATE
#define RELEASE_DATE "2022"
#endif

#define COPY_END_YEAR 2022

#ifndef VERSION
#define VERSION "$VER: iGame v" STR(MAJOR_VERS) "." STR(MINOR_VERS) "." STR(PATCH_VERS) " for " STR(CPU_VERS) " (" STR(RELEASE_DATE) ")"
#endif
