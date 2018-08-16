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
	CONST_STRPTR STR_TX_Status;
	CONST_STRPTR STR_TX_PropertiesTimesPlayed;
	CONST_STRPTR STR_TX_PropertiesSlavePath;
	CONST_STRPTR STR_TX_PropertiesTooltypes;
	CONST_STRPTR STR_TX_About;
	CONST_STRPTR CY_PropertiesGenreContent[512];
	CONST_STRPTR CY_AddGameGenreContent[512];
};

extern struct ObjApp* CreateApp(void);
extern void DisposeApp(struct ObjApp*);

#endif
