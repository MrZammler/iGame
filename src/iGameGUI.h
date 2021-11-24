/*
  iGameGUI.h
  GUI header file for iGame

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

#ifndef GUI_FILE_H
#define GUI_FILE_H

#define STR_HELPER(s) #s      //stringify argument
#define STR(s) STR_HELPER(s)  //indirection to expand argument macros

/* Types */
#include <exec/types.h>

struct ObjApp
{
	APTR App;
	APTR WI_MainWindow;
	APTR MN_MainMenu;
	APTR STR_Filter;
	APTR GR_sidepanel;
	APTR IM_GameImage_0;
	APTR IM_GameImage_1;
	APTR Space_Sidepanel;
	APTR LV_GamesList;
	APTR LV_GenresList;
	APTR TX_Status;
	APTR WI_Properties;
	APTR STR_PropertiesGameTitle;
	APTR CY_PropertiesGenre;
	APTR CH_PropertiesFavorite;
	APTR CH_PropertiesHidden;
	APTR TX_PropertiesTimesPlayed;
	APTR TX_PropertiesSlavePath;
	APTR TX_PropertiesTooltypes;
	APTR BT_PropertiesOK;
	APTR BT_PropertiesCancel;
	APTR WI_GameRepositories;
	APTR PA_RepoPath;
	APTR STR_PA_RepoPath;
	APTR BT_AddRepo;
	APTR LV_GameRepositories;
	APTR BT_RemoveRepo;
	APTR BT_CloseRepoWindow;
	APTR WI_AddNonWHDLoad;
	APTR STR_AddTitle;
	APTR PA_AddGame;
	APTR STR_PA_AddGame;
	APTR CY_AddGameGenre;
	APTR BT_AddGameOK;
	APTR BT_AddGameCancel;
	APTR WI_About;
	APTR TX_About;
	APTR BT_AboutOK;
	APTR WI_Settings;
	APTR CH_Screenshots;
	APTR CH_NoGuiGfx;
	APTR CY_ScreenshotSize;
	APTR STR_Width;
	APTR STR_Height;
	APTR RA_TitlesFrom;
	APTR CH_SmartSpaces;
	APTR CH_SaveStatsOnExit;
	APTR CH_FilterUseEnter;
	APTR CH_HideSidepanel;
	APTR CH_StartWithFavorites;
	APTR BT_SettingsSave;
	APTR BT_SettingsUse;
	APTR BT_SettingsCancel;
	CONST_STRPTR STR_TX_Status;
	CONST_STRPTR STR_TX_PropertiesTimesPlayed;
	CONST_STRPTR STR_TX_PropertiesSlavePath;
	CONST_STRPTR STR_TX_PropertiesTooltypes;
	CONST_STRPTR STR_TX_About;
	CONST_STRPTR CY_PropertiesGenreContent[512];
	CONST_STRPTR CY_AddGameGenreContent[512];
	CONST_STRPTR CY_ScreenshotSizeContent[4];
	CONST_STRPTR RA_TitlesFromContent[3];
};

extern struct ObjApp* CreateApp(void);
extern void DisposeApp(struct ObjApp*);

BOOL checkImageDatatype(STRPTR);

#endif
