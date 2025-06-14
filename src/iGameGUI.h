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
	APTR GR_leftpanel;
	APTR GR_sidepanel;
	APTR GR_spacedScreenshot;
	APTR IM_GameImage_0;
	APTR IM_GameImage_1;
	APTR Space_Sidepanel;
	APTR LV_GamesList;
	APTR CY_FilterList;
	APTR LV_GenresList;
	APTR CY_ChipsetList;
	APTR TX_Status;
	// Properties Window
	APTR WI_Properties;
	APTR TX_PropertiesGameTitle;
	APTR PA_PropertiesSlavePath;
	APTR TX_PropertiesTooltypes;
	APTR BT_PropertiesOK;
	APTR BT_PropertiesCancel;
	// Repositories Window
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
	APTR WI_Settings;
	APTR CH_Screenshots;
	APTR CH_NoGuiGfx;
	APTR CY_ScreenshotSize;
	APTR STR_Width;
	APTR STR_Height;
	APTR RA_TitlesFrom;
	APTR CH_SmartSpaces;
	APTR CH_UseIgameDataTitle;
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
	CONST_STRPTR GenresContent[256];
	CONST_STRPTR CY_ScreenshotSizeContent[4];
	CONST_STRPTR RA_TitlesFromContent[3];
	CONST_STRPTR CY_FilterListContent[6];
	CONST_STRPTR CY_ChipsetListContent[16];
	// Information Window
	APTR WI_Information;
	APTR STR_WI_Information_Title;
	APTR TX_WI_Information_Year;
	APTR TX_WI_Information_ReleasedBy;
	APTR TX_WI_Information_Chipset;
	APTR TX_WI_Information_Players;
	APTR TX_WI_Information_TimesPlayed;
	APTR CH_WI_Information_Favourite;
	APTR CH_WI_Information_Hidden;
	APTR POP_WI_Information_Genre;
	APTR STR_WI_Information_Genre;
	APTR LV_WI_Information_Genre;
	APTR GR_WI_Information_Links;
	APTR URL_WI_Information_Lemonamiga;
	APTR URL_WI_Information_HOL;
	APTR URL_WI_Information_Pouet;
	APTR BT_WI_Information_Save;
	APTR BT_WI_Information_Cancel;
	// About Window
	APTR WI_About;
	APTR GR_WI_About_Links;
	APTR TX_About;
	APTR BT_AboutOK;
};

struct ObjApp *CreateApp(void);
void DisposeApp(struct ObjApp *);

BOOL checkImageDatatype(STRPTR);

#endif
