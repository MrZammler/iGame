/*
  iGameGUI.h
  Header file for iGameGUI.c for iGame
  
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

#ifndef GUI_FILE_H
#define GUI_FILE_H

/* Types */
#include <exec/types.h>
struct ObjApp
{
	APTR	App;
	APTR	WI_Main_0;
	APTR	MN_label_1;
	APTR	STR_Filter_0;
	APTR	LV_GamesList_0;
	APTR	GR_grp_1;
	APTR	IM_GameImage_0;
	APTR	IM_GameImage_1;
	APTR	LV_GenresList_1;
	APTR	TX_Status_0;
	APTR	WI_GamesRepo_1;
	APTR	LV_RepoList_3;
	APTR	PA_label_0;
	APTR	PA_label_1;
	APTR	STR_PA_label_0;
	APTR	STR_PA_label_1;
	APTR	BT_RepoAdd_2;
	APTR	BT_RepoRemove_3;
	APTR	BT_RepoClose_1;
	APTR	BT_NonWhdloadOk_0;
	APTR	BT_NonWhdloadCancel_0;
	APTR	WI_GameProperties_2;
	APTR	WI_NonWhdload;
	APTR	STR_Title_1;
	APTR	STR_Title_2;
	APTR	CY_Genre_0;
	APTR	TX_Path_2;
	APTR	BT_GPOK_0;
	APTR	BT_GPCancel_1;
	APTR	WI_About_3;
	APTR	TX_label_1;
	APTR	TX_label_2;
	APTR	CH_label_0;
	APTR	CH_label_1;
	APTR	TX_NumPlayed_0;
	APTR    TX_Tooltypes_0;
	APTR	WI_Wait_0;
	char *	STR_TX_Status_0;
	char *	STR_TX_Path_2;
	char *	STR_TX_label_1;
	char *  STR_TX_label_2;
	char *  STR_TX_NumPlayed_0;
	char *  STR_TX_Tooltypes_0;
	char *	CY_Genre_0Content[256];
};


extern struct ObjApp * CreateApp(void);
extern void DisposeApp(struct ObjApp *);

#endif
