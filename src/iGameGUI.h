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
	APTR App;
	APTR WI_MainWindow;
	APTR MN_MainMenu;
	APTR STR_Filter;
	APTR LV_GamesList;
	APTR GR_grp_1;
	APTR IM_GameImage_0;
	APTR IM_GameImage_1;
	APTR LV_GenresList;
	APTR TX_Status;
	APTR WI_GameRepositories;
	APTR LV_RepoList;
	APTR PA_RepoPath;
	APTR PA_AddGamePath;
	APTR STR_PA_RepoPath;
	APTR STR_PA_AddGame;
	APTR BT_RepoAdd;
	APTR BT_RepoRemove;
	APTR BT_RepoClose;
	APTR BT_NonWhdloadOk;
	APTR BT_NonWhdloadCancel;
	APTR WI_GameProperties;
	APTR WI_NonWhdload;
	APTR STR_GameTitle;
	APTR STR_AddGameTitle;
	APTR CY_Genre;
	APTR TX_GamePath;
	APTR BT_GamePropertiesOK;
	APTR BT_GamePropertiesCancel;
	APTR WI_About;
	APTR TX_label_1;
	APTR TX_label_2;
	APTR CH_Favorite;
	APTR CH_Hidden;
	APTR TX_NumPlayed;
	APTR TX_Tooltypes;
	APTR WI_Wait;
	char* STR_TX_Status;
	char* STR_TX_Path;
	char* STR_TX_About;
	char* STR_TX_Wait;
	char* STR_TX_NumPlayed;
	char* STR_TX_Tooltypes;
	char* CY_Genre_Content[512];
};

extern struct ObjApp* CreateApp(void);
extern void DisposeApp(struct ObjApp*);

#endif
