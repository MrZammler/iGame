/*
  iGameGUI.c
  GUI source for iGame

  Copyright (c) 2019, Emmanuel Vasilakis and contributors

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

/* MUI */
#include <libraries/mui.h>
#include <mui/Guigfx_mcc.h>
#include <mui/TextEditor_mcc.h>
#include <mui/NListview_mcc.h>

#ifdef __morphos__
#define SDI_TRAP_LIB
#endif
#include <SDI_hook.h>

/* Prototypes */
#include <clib/alib_protos.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <proto/asl.h>
#include <proto/datatypes.h>
#include <proto/dos.h>
#include <proto/muimaster.h>
#include <proto/utility.h>

/* System */
#include <libraries/gadtools.h> /* for Barlabel in MenuItem */
#include <exec/memory.h>
#include <dos/dos.h>
#if defined(__amigaos4__)
#include <dos/obsolete.h>
#endif

/* ANSI C */
#include <stdio.h>
#include <string.h>

#ifndef CPU_VERS
#define CPU_VERS 68000
#endif

#define iGame_NUMBERS
#include "iGame_strings.h"

#include "version.h"
#include "iGameExtern.h"
#include "fsfuncs.h"
#include "funcs.h"
#include "strfuncs.h"
#include "iGameGUI.h"
#include "WinInfo.h"
#include "WinProps.h"
#include "WinAbout.h"

extern igame_settings *current_settings;
extern blockGuiGfx;

#ifndef __morphos__
static void translateMenu(struct NewMenu *);
static void flagMenuItem(struct NewMenu *, APTR, UWORD);

#define TICK (CHECKIT|MENUTOGGLE)
#define DIS  NM_ITEMDISABLED
#define STR_ID(x) ( (STRPTR)(x) )
#endif // ndef __morphos__

#if defined(__amigaos4__)
#define AllocVecShared(size, flags)  AllocVecTags((size), AVT_Type, MEMF_SHARED, AVT_Lock, FALSE, ((flags)&MEMF_CLEAR) ? AVT_ClearWithValue : TAG_IGNORE, 0, TAG_DONE)
#else
#define AllocVecShared(size, flags)  AllocVec((size), (flags))
#endif

#ifndef __morphos__
static struct NewMenu MenuMainWin[] =
{
	{ NM_TITLE, STR_ID(MSG_MNlabel2Actions)							, 0 ,0 ,0			,(APTR)MENU_ACTIONS },
	{ NM_ITEM ,  STR_ID(MSG_MNlabelScan)							,"R",0 ,0			,(APTR)MENU_SCAN },
	{ NM_ITEM ,  STR_ID(MSG_MNMainAddnonWHDLoadgame)				,"A",0 ,0			,(APTR)MENU_ADDGAME },
	{ NM_ITEM ,  STR_ID(MSG_MNMainMenuShowHidehiddenentries)		, 0 ,TICK ,0		,(APTR)MENU_SHOWHIDDEN },
	{ NM_ITEM ,  NM_BARLABEL										, 0 ,0 ,0			,(APTR)0 },
	{ NM_ITEM ,  STR_ID(MSG_MNMainAbout)							,"?",0 ,0			,(APTR)MENU_ABOUT },
	{ NM_ITEM ,  NM_BARLABEL										, 0 ,0 ,0			,(APTR)0 },
	{ NM_ITEM ,  STR_ID(MSG_MNMainQuit)								,"Q",0 ,0			,(APTR)MENU_QUIT },

	{ NM_TITLE, STR_ID(MSG_MNlabel2Game)							, 0 ,0 ,0			,(APTR)MENU_GAME },
	{ NM_ITEM ,  STR_ID(MSG_MNMainProperties)						,"P",0 ,0			,(APTR)MENU_GAMEPROPERTIES },
	{ NM_ITEM ,  STR_ID(MSG_MNMainIformation)						,"I",0 ,0			,(APTR)MENU_ITEMINFO },
	{ NM_ITEM ,  STR_ID(MSG_MNMainOpenCurrentDir)					,"D",0 ,0			,(APTR)MENU_GAMEFOLDER },

	{ NM_TITLE, STR_ID(MSG_MNMainPreferences)						, 0 ,0 ,0			,(APTR)MENU_PREFERENCES },
	{ NM_ITEM ,  STR_ID(MSG_MNMainiGameSettings)					, 0 ,0 ,0			,(APTR)MENU_SETTINGS },
	{ NM_ITEM ,  STR_ID(MSG_MNlabel2GameRepositories)				, 0 ,0 ,0			,(APTR)MENU_REPOSITORIES },
	{ NM_ITEM ,  NM_BARLABEL										, 0 ,0 ,0			,(APTR)0 },
	{ NM_ITEM ,  STR_ID(MSG_MNMainMUISettings)						, 0 ,0 ,0			,(APTR)MENU_MUISETTINGS },

	{ NM_END,NULL,0,0,0,NULL }
};
#endif // ndef __morphos__

static struct Listentry
{
	char title[MAX_SLAVE_TITLE_SIZE];
	char year[5];
	char players[3];
} Listentry;

HOOKPROTONH(DisplayLI_TextFunc, LONG, char **array, struct Listentry *entry)
{
	if (entry)
	{
		*array++ = entry->title;
		*array++ = entry->year;
		*array++ = entry->players;
	}
	else
	{
		*array++ = GetMBString(MSG_LV_GAMESLIST_TITLE);
		*array++ = GetMBString(MSG_LV_GAMESLIST_YEAR);
		*array++ = GetMBString(MSG_LV_GAMESLIST_PLAYERS);
	}
	return 0;
}
MakeStaticHook(DisplayLI_TextHook, DisplayLI_TextFunc);

HOOKPROTONH(ConstructLI_TextFunc, APTR, APTR pool, struct slavesList *item)
{
	struct Listentry *newentry = AllocPooled(pool, sizeof(struct Listentry));

	if (newentry)
	{
		// Decide from where the item name will be taken
		if(!isStringEmpty(item->user_title))
		{
			snprintf(newentry->title, sizeof(newentry->title), "%s", item->user_title);
		}
		else if(isStringEmpty(item->user_title) && (item->instance > 0))
		{
			// snprintf(item->user_title, sizeof(item->user_title),
				// "%s [%d]", item->title, item->instance);
			snprintf(newentry->title, sizeof(newentry->title), "%s [%d]", item->title, item->instance);
		}
		else
		{
			snprintf(newentry->title, sizeof(newentry->title), "%s", item->title);
		}

		snprintf(newentry->year, sizeof(newentry->year), "%d", item->year);
		snprintf(newentry->players, sizeof(newentry->players), "%d", item->players);

		return newentry;
	}

	return NULL;
}
MakeStaticHook(ConstructLI_TextHook, ConstructLI_TextFunc);

HOOKPROTONH(DestructLI_TextFunc, LONG, APTR pool, struct Listentry *entry)
{
	FreePooled(pool, entry, sizeof(struct Listentry));

	return 0;
}
MakeStaticHook(DestructLI_TextHook, DestructLI_TextFunc);

HOOKPROTONHNO(CompareLI_TextFunc, LONG, struct NList_CompareMessage *ncm)
{
	struct Listentry *entry1 = (struct Listentry *) ncm->entry1;
	struct Listentry *entry2 = (struct Listentry *) ncm->entry2;
	LONG column = ncm->sort_type & MUIV_NList_TitleMark_ColMask;
	LONG result = 0;

	if(ncm->sort_type == (LONG)MUIV_NList_SortType_None)
	{
		result = (LONG) strcmp(entry1->title, entry2->title);
		return result;
	}

	if (column == 0)
	{
		if (ncm->sort_type & MUIV_NList_TitleMark_TypeMask)
			result = (LONG) strcmp(entry2->title, entry1->title);
		else
			result = (LONG) strcmp(entry1->title, entry2->title);
	}
	else if (column == 1)
	{
		if (ncm->sort_type & MUIV_NList_TitleMark_TypeMask)
			result = (LONG) strcmp(entry2->year, entry1->year);
		else
			result = (LONG) strcmp(entry1->year, entry2->year);
	}
	else if (column == 2)
	{
		if (ncm->sort_type & MUIV_NList_TitleMark_TypeMask)
			result = (LONG) strcmp(entry2->players, entry1->players);
		else
			result = (LONG) strcmp(entry1->players, entry2->players);
	}

//   if      (col1 == 0)
//   { if (ncm->sort_type & MUIV_NList_TitleMark_TypeMask)
//       result = entry2->num - entry1->num;
//     else
//       result = entry1->num - entry2->num;
//   }
//   else if (col1 == 1)
//   { if (ncm->sort_type & MUIV_NList_TitleMark_TypeMask)
//       result = (LONG) stricmp(entry2->str1,entry1->str1);
//     else
//       result = (LONG) stricmp(entry1->str1,entry2->str1);
//   }
//   else if (col1 == 2)
//   { if (ncm->sort_type & MUIV_NList_TitleMark_TypeMask)
//       result = (LONG) stricmp(entry2->str2,entry1->str2);
//     else
//       result = (LONG) stricmp(entry1->str2,entry2->str2);
//   }

	return result;
}
MakeHook(CompareLI_TextHook, CompareLI_TextFunc);


struct ObjApp *CreateApp(void)
{
	struct ObjApp *object;
	static char about_text[512];
	static char version_string[30];

#ifndef __morphos__
	APTR strip;
	translateMenu(MenuMainWin);
#endif // ndef __morphos__

	snprintf(version_string, sizeof(version_string),
		"%s %d.%d.%d (%s)",
		GetMBString(MSG_WI_MainWindow),
		MAJOR_VERS, MINOR_VERS, PATCH_VERS, STR(RELEASE_DATE)
	);

	snprintf(about_text, sizeof(about_text),
		"\033c%s\n%s %s\n\nCopyright 2005-%d\n%s",
		version_string, GetMBString(MSG_compiledForAboutWin), STR(CPU_VERS), COPY_END_YEAR, GetMBString(MSG_TX_About)
	);

#ifdef __morphos__
	APTR	MNlabel2Actions, MNlabelScan, MNMainAddnonWHDLoadgame, MNMainMenuShowHidehiddenentries;
	APTR	MNMainBarLabel0, MNMainAbout;
	APTR	MNMainBarLabel1, MNMainQuit, MNlabel2Game, MNMainProperties, MNMainIformation, MNMainOpenCurrentDir;
	APTR	MNlabel2Tools, MNMainiGameSettings;
	APTR	MNlabel2GameRepositories, MNMainBarLabel2, MNMainMUISettings;
#endif // ndef __morphos__

	APTR	MNMainOpenList, MNMainSaveList, MNMainSaveListAs;
	// APTR	MNMainMenuDuplicate;
	// APTR	MNMainDelete;
	APTR	GROUP_ROOT;
	APTR	GR_Filter, LA_Filter, GR_main;
	APTR	GROUP_ROOT_2, GR_Path, GR_ReposButtons, GROUP_ROOT_3;
	APTR	GR_AddGameTitle, LA_AddGameTitle, GR_AddGamePath, LA_AddGamePath;
	APTR	GR_AddGameGenre, LA_AddGameGenre, Space_AddGame, GR_AddGameButtons;
	APTR	GROUP_ROOT_4, GROUP_ROOT_Settings, GR_Settings;
	APTR	LA_HideScreenshots, GR_Screenshots;
	APTR	LA_NoGuiGfx, GR_ScreenshotSize, LA_ScreenshotSize;
	APTR	Space_ScreenshotSize, GR_CustomSize, LA_Width, LA_Height, GR_Titles;
	APTR	GR_TitlesFrom, Space_TitlesFrom, GR_SmartSpaces, Space_SmartSpaces;
	APTR	LA_UseIgameDataTitle, GR_UseIgameDataTitle;
	APTR	LA_SmartSpaces, GR_Misc;
	APTR	LA_SaveStatsOnExit, LA_FilterUseEnter, LA_StartWithFavorites;
	APTR	LA_HideSidepanel;
	APTR	GR_SettingsButtons, Space_SettingsButtons1, Space_SettingsButtons2;

	MakeStaticHook(MenuScanHook, scan_repositories);
	MakeStaticHook(MenuShowHideHiddenHook, list_show_hidden);
	MakeStaticHook(MenuPropertiesHook, slaveProperties);
	MakeStaticHook(MenuItemInfoHook, getItemInformation);
	MakeStaticHook(MenuAddNonWhdloadHook, add_non_whdload);
	MakeStaticHook(OpenCurrentDirHook, open_current_dir);
	MakeStaticHook(MenuOpenListHook, open_list);
	MakeStaticHook(MenuSaveListHook, save_list);
	MakeStaticHook(MenuSaveListAsHook, save_list_as);
	// MakeStaticHook(MenuDeleteHook, game_delete);
	MakeStaticHook(FilterChangeHook, filter_change);
	MakeStaticHook(LaunchGameHook, launch_game);
	MakeStaticHook(AppStartHook, app_start);
	MakeStaticHook(GameClickHook, game_click);
	MakeStaticHook(GenreClickHook, filter_change);
	MakeStaticHook(NonWHDLoadOKHook, non_whdload_ok);
	MakeStaticHook(RepoStopHook, repo_stop);
	MakeStaticHook(RepoAddHook, repo_add);
	MakeStaticHook(RepoRemoveHook, repo_remove);
	MakeStaticHook(SettingFilterUseEnterChangedHook, setting_filter_use_enter_changed);
	MakeStaticHook(SettingSaveStatsOnExitChangedHook, setting_save_stats_on_exit_changed);
	MakeStaticHook(SettingSmartSpacesChangedHook, setting_smart_spaces_changed);
	MakeStaticHook(SettingTitlesFromChangedHook, setting_titles_from_changed);
	MakeStaticHook(SettingHideScreenshotChangedHook, setting_hide_screenshot_changed);
	MakeStaticHook(SettingNoGuiGfxChangedHook, setting_no_guigfx_changed);
	MakeStaticHook(SettingScreenshotSizeChangedHook, setting_screenshot_size_changed);
	MakeStaticHook(SettingsSaveHook, settings_save);
	MakeStaticHook(SettingHideSidePanelChangedHook, setting_hide_side_panel_changed);
	MakeStaticHook(SettingStartWithFavoritesChangedHook, setting_start_with_favorites_changed);
	MakeStaticHook(SettingsUseHook, settings_use);
	MakeStaticHook(SettingUseIgameDataTitleHook, settingUseIgameDataTitleChanged);

	if (!((object = AllocVec(sizeof(struct ObjApp), MEMF_PUBLIC | MEMF_CLEAR))))
		return NULL;

	object->STR_TX_Status = NULL;
	object->STR_TX_PropertiesTimesPlayed = NULL;
	object->STR_TX_PropertiesSlavePath = NULL;
	object->STR_TX_PropertiesTooltypes = NULL;

	object->STR_TX_About = about_text;

	object->GenresContent[0] = (CONST_STRPTR)GetMBString(MSG_CY_PropertiesGenre0);
	object->GenresContent[1] = NULL;

	object->CY_ScreenshotSizeContent[0] = (CONST_STRPTR)GetMBString(MSG_CY_ScreenshotSize0);
	object->CY_ScreenshotSizeContent[1] = (CONST_STRPTR)GetMBString(MSG_CY_ScreenshotSize1);
	object->CY_ScreenshotSizeContent[2] = (CONST_STRPTR)GetMBString(MSG_CY_ScreenshotSize2);
	object->CY_ScreenshotSizeContent[3] = NULL;

	object->RA_TitlesFromContent[0] = (CONST_STRPTR)GetMBString(MSG_RA_TitlesFrom0);
	object->RA_TitlesFromContent[1] = (CONST_STRPTR)GetMBString(MSG_RA_TitlesFrom1);
	object->RA_TitlesFromContent[2] = NULL;

	object->CY_FilterListContent[0] = (CONST_STRPTR)GetMBString(MSG_FilterShowAll);
	object->CY_FilterListContent[1] = (CONST_STRPTR)GetMBString(MSG_FilterFavorites);
	object->CY_FilterListContent[2] = (CONST_STRPTR)GetMBString(MSG_FilterLastPlayed);
	object->CY_FilterListContent[3] = (CONST_STRPTR)GetMBString(MSG_FilterMostPlayed);
	object->CY_FilterListContent[4] = (CONST_STRPTR)GetMBString(MSG_FilterNeverPlayed);
	object->CY_FilterListContent[5] = NULL;

	object->CY_ChipsetListContent[0] = (CONST_STRPTR)GetMBString(MSG_FilterShowAll);
	object->CY_ChipsetListContent[1] = NULL;

	LA_Filter = Label(GetMBString(MSG_LA_Filter));

	object->STR_Filter = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Filter",
		End;

	GR_Filter = GroupObject,
		MUIA_HelpNode, "GR_Filter",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_Filter,
		Child, object->STR_Filter,
		End;

	object->LV_GamesList = NListviewObject,
		MUIA_NListview_NList, NListObject,
			MUIA_ObjectID, MAKE_ID('N','L','0','1'),
			MUIA_NList_DefaultObjectOnClick, TRUE,
			MUIA_NList_MultiSelect, MUIV_NList_MultiSelect_None,
			MUIA_NList_DisplayHook, &DisplayLI_TextHook,
			MUIA_NList_CompareHook2, &CompareLI_TextHook,
			MUIA_NList_ConstructHook, &ConstructLI_TextHook,
			MUIA_NList_DestructHook, &DestructLI_TextHook,
			MUIA_NList_Format, "BAR W=70,BAR W=15,BAR W=15",
			MUIA_NList_AutoVisible, TRUE,
			MUIA_NList_TitleSeparator, TRUE,
			MUIA_NList_Title, TRUE,
			MUIA_NList_EntryValueDependent, FALSE,
			MUIA_NList_MinColSortable, 100,
			MUIA_NList_Imports, MUIV_NList_Imports_Cols,
			MUIA_NList_Exports, MUIV_NList_Exports_Cols,
			End,
		MUIA_NListview_Horiz_ScrollBar, MUIV_NListview_HSB_None,
		MUIA_NListview_Vert_ScrollBar,  MUIV_NListview_VSB_FullAuto,
		End;

	object->CY_FilterList = CycleObject,
		MUIA_HelpNode, "CY_FilterList",
		MUIA_Frame, MUIV_Frame_Button,
		MUIA_Cycle_Entries, object->CY_FilterListContent,
		End;

	if (!current_settings->hide_side_panel)
	{
		if (!current_settings->hide_screenshots) {

			if (current_settings->no_guigfx) {
				object->IM_GameImage_0 = MUI_NewObject(Dtpic_Classname,
					MUIA_Dtpic_Name, DEFAULT_SCREENSHOT_FILE,
					MUIA_Frame, MUIV_Frame_ImageButton,
				TAG_DONE);
			}
			else {
				object->IM_GameImage_0 = GuigfxObject,
					MUIA_Guigfx_FileName, DEFAULT_SCREENSHOT_FILE,
					MUIA_Guigfx_Quality, MUIV_Guigfx_Quality_Best,
					MUIA_Guigfx_ScaleMode, NISMF_SCALEFREE | NISMF_KEEPASPECT_PICTURE,
					MUIA_Frame, MUIV_Frame_ImageButton,
					MUIA_FixHeight, current_settings->screenshot_height,
					MUIA_FixWidth, current_settings->screenshot_width,
				End;
			}

			object->GR_spacedScreenshot = HGroup, MUIA_Group_Spacing, 0,
				Child, HSpace(0),
				Child, object->IM_GameImage_0,
				Child, HSpace(0),
				End;
		}

		object->Space_Sidepanel = VSpace(1);

		object->CY_ChipsetList = CycleObject,
			MUIA_HelpNode, "CY_ChipsetList",
			MUIA_Frame, MUIV_Frame_Button,
			MUIA_Cycle_Entries, object->CY_ChipsetListContent,
			End;

		object->LV_GenresList = ListObject,
			MUIA_Frame, MUIV_Frame_InputList,
			End;

		object->LV_GenresList = ListviewObject,
			MUIA_HelpNode, "LV_GenresList",
			MUIA_Listview_List, object->LV_GenresList,
			End;

		if (!current_settings->hide_screenshots) {

			object->GR_sidepanel = GroupObject,
				MUIA_HelpNode, "GR_sidepanel",
				MUIA_Weight, 100,
				Child, object->GR_spacedScreenshot,
				Child, object->Space_Sidepanel,
				Child, object->CY_ChipsetList,
				Child, object->LV_GenresList,
				End;
		}
		else {

			object->GR_sidepanel = GroupObject,
				MUIA_HelpNode, "GR_sidepanel",
				MUIA_Weight, 100,
				Child, object->Space_Sidepanel,
				Child, object->CY_ChipsetList,
				Child, object->LV_GenresList,
				End;
		}

		object->GR_leftpanel = GroupObject,
			MUIA_HelpNode, "GR_leftpanel",
			Child, object->CY_FilterList,
			Child, object->LV_GamesList,
			End;

		GR_main = GroupObject,
			MUIA_HelpNode, "GR_main",
			MUIA_Group_Horiz, TRUE,
			Child, object->GR_leftpanel,
			Child, BalanceObject,
				MUIA_CycleChain, 1,
				MUIA_ObjectID, MAKE_ID('B', 'A', 'L', 0),
				End,
			Child, object->GR_sidepanel,
			End;
	}
	else
	{
		GR_main = GroupObject,
			MUIA_HelpNode, "GR_main",
			Child, object->CY_FilterList,
			Child, object->LV_GamesList,
			End;
	}

	object->TX_Status = TextObject,
		MUIA_Frame, MUIV_Frame_None,
		MUIA_Text_Contents, object->STR_TX_Status,
		MUIA_Text_SetMin, TRUE,
		End;

	GROUP_ROOT = GroupObject,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, GR_Filter,
		Child, GR_main,
		Child, object->TX_Status,
		End;

#ifdef __morphos__
	MNlabelScan = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNlabelScan),
		MUIA_Menuitem_Shortcut, MENU_SCANREPOS_HOTKEY,
		End;

	MNMainAddnonWHDLoadgame = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainAddnonWHDLoadgame),
		MUIA_Menuitem_Shortcut, MENU_ADDNONWHDLOADGAME_HOTKEY,
		End;

	MNMainMenuShowHidehiddenentries = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainMenuShowHidehiddenentries),
		End;
#endif

	MNMainOpenList = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainOpenList),
		MUIA_Menuitem_Shortcut, MENU_OPENLIST_HOTKEY,
		End;

	MNMainSaveList = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainSaveList),
		MUIA_Menuitem_Shortcut, MENU_SAVELIST_HOTKEY,
		End;

	MNMainSaveListAs = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainSaveListAs),
		End;

#ifdef __morphos__
	MNMainBarLabel0 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNMainAbout = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainAbout),
		MUIA_Menuitem_Shortcut, MENU_ABOUT_HOTKEY,
		End;

	MNMainBarLabel1 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNMainQuit = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainQuit),
		MUIA_Menuitem_Shortcut, MENU_QUIT_HOTKEY,
		End;

	MNlabel2Actions = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNlabel2Actions),
		MUIA_Family_Child, MNlabelScan,
		MUIA_Family_Child, MNMainAddnonWHDLoadgame,
		MUIA_Family_Child, MNMainMenuShowHidehiddenentries,
		/* MUIA_Family_Child, MNMainBarLabel5, */
		/* MUIA_Family_Child, MNMainOpenList, */
		/* MUIA_Family_Child, MNMainSaveList, */
		/* MUIA_Family_Child, MNMainSaveListAs, */
		MUIA_Family_Child, MNMainBarLabel0,
		MUIA_Family_Child, MNMainAbout,
		MUIA_Family_Child, MNMainBarLabel1,
		MUIA_Family_Child, MNMainQuit,
		End;

	MNMainProperties = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainProperties),
		MUIA_Menuitem_Shortcut, MENU_PROPERTIES_HOTKEY,
		End;

	MNMainIformation = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainIformation),
		MUIA_Menuitem_Shortcut, MENU_INFORMATION_HOTKEY,
		End;

	MNMainOpenCurrentDir = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainOpenCurrentDir),
		MUIA_Menuitem_Shortcut, MENU_DELETE_HOTKEY,
		End;
#endif // ndef __morphos__

	// MNMainMenuDuplicate = MenuitemObject,
	// 	MUIA_Menuitem_Title, GetMBString(MSG_MNMainMenuDuplicate),
	// 	End;

	// MNMainDelete = MenuitemObject,
	// 	MUIA_Menuitem_Title, GetMBString(MSG_MNMainDelete),
	// 	MUIA_Menuitem_Shortcut, MENU_DELETE_HOTKEY,
	// 	End;

#ifndef __morphos__
	if (get_wb_version() < 44)
	{
		flagMenuItem(MenuMainWin, (APTR)MENU_GAMEFOLDER, DIS);
	}
#else
	MNlabel2Game = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNlabel2Game),
		MUIA_Family_Child, MNMainProperties,
		MUIA_Family_Child, MNMainIformation,
		MUIA_Family_Child, MNMainOpenCurrentDir,
		End;

	MNMainiGameSettings = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainiGameSettings),
		End;

	MNlabel2GameRepositories = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNlabel2GameRepositories),
		End;

	MNMainBarLabel2 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNMainMUISettings = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainMUISettings),
		End;

	MNlabel2Tools = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNlabel2Tools),
		MUIA_Family_Child, MNMainiGameSettings,
		MUIA_Family_Child, MNlabel2GameRepositories,
		MUIA_Family_Child, MNMainBarLabel2,
		MUIA_Family_Child, MNMainMUISettings,
		End;

	object->MN_MainMenu = MenustripObject,
		MUIA_Family_Child, MNlabel2Actions,
		MUIA_Family_Child, MNlabel2Game,
		MUIA_Family_Child, MNlabel2Tools,
		End;
#endif // ndef __morphos__

	object->WI_MainWindow = WindowObject,
		MUIA_Window_ScreenTitle, version_string,
		MUIA_Window_Title, GetMBString(MSG_WI_MainWindow),
#ifndef __morphos__
		MUIA_Window_Menustrip, strip = MUI_MakeObject(MUIO_MenustripNM, MenuMainWin, 0),
#else
		MUIA_Window_Menustrip, object->MN_MainMenu,
#endif // ndef __morphos__
		MUIA_Window_ID, MAKE_ID('0', 'I', 'G', 'A'),
		MUIA_Window_AppWindow, TRUE,
		WindowContents, GROUP_ROOT,
		End;

	object->STR_PA_RepoPath = String("", 80);

	object->PA_RepoPath = PopaslObject,
		MUIA_HelpNode, "PA_RepoPath",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_RepoPath,
		MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
		ASLFR_TitleText, GetMBString(MSG_SelectDir),
		ASLFR_DrawersOnly, TRUE,
		End;

	object->BT_AddRepo = TextObject,
		ButtonFrame,
		MUIA_Weight, 30,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, GetMBString(MSG_BT_AddRepo),
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_AddRepo",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	GR_Path = GroupObject,
		MUIA_HelpNode, "GR_Path",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->PA_RepoPath,
		Child, object->BT_AddRepo,
		End;

	object->LV_GameRepositories = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		End;

	object->LV_GameRepositories = ListviewObject,
		MUIA_HelpNode, "LV_GameRepositories",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_None,
		MUIA_Listview_List, object->LV_GameRepositories,
		End;

	object->BT_RemoveRepo = SimpleButton(GetMBString(MSG_BT_RemoveRepo));

	object->BT_CloseRepoWindow = SimpleButton(GetMBString(MSG_BT_CloseRepoWindow));

	GR_ReposButtons = GroupObject,
		MUIA_HelpNode, "GR_ReposButtons",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->BT_RemoveRepo,
		Child, object->BT_CloseRepoWindow,
		End;

	GROUP_ROOT_2 = GroupObject,
		Child, GR_Path,
		Child, object->LV_GameRepositories,
		Child, GR_ReposButtons,
		End;

	object->WI_GameRepositories = WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_GameRepositories),
		MUIA_Window_ID, MAKE_ID('2', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_2,
		End;

	LA_AddGameTitle = Label(GetMBString(MSG_LA_AddGameTitle));

	object->STR_AddTitle = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_AddTitle",
		End;

	GR_AddGameTitle = GroupObject,
		MUIA_HelpNode, "GR_AddGameTitle",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_AddGameTitle,
		Child, object->STR_AddTitle,
		End;

	LA_AddGamePath = Label(GetMBString(MSG_LA_AddGamePath));

	object->STR_PA_AddGame = String("", 80);

	object->PA_AddGame = PopButton(MUII_PopFile);

	object->PA_AddGame = PopaslObject,
		MUIA_HelpNode, "PA_AddGame",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_AddGame,
		MUIA_Popstring_Button, object->PA_AddGame,
		ASLFR_TitleText, GetMBString(MSG_GameExecutable),
		End;

	GR_AddGamePath = GroupObject,
		MUIA_HelpNode, "GR_AddGamePath",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_AddGamePath,
		Child, object->PA_AddGame,
		End;

	LA_AddGameGenre = Label(GetMBString(MSG_LA_AddGameGenre));

	object->CY_AddGameGenre = CycleObject,
		MUIA_HelpNode, "CY_AddGameGenre",
		MUIA_Frame, MUIV_Frame_Button,
		End;

	GR_AddGameGenre = GroupObject,
		MUIA_HelpNode, "GR_AddGameGenre",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_AddGameGenre,
		Child, object->CY_AddGameGenre,
		End;

	Space_AddGame = VSpace(10);

	object->BT_AddGameOK = SimpleButton(GetMBString(MSG_BT_AddGameOK));

	object->BT_AddGameCancel = SimpleButton(GetMBString(MSG_BT_AddGameCancel));

	GR_AddGameButtons = GroupObject,
		MUIA_HelpNode, "GR_AddGameButtons",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->BT_AddGameOK,
		Child, object->BT_AddGameCancel,
		End;

	GROUP_ROOT_3 = GroupObject,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, GR_AddGameTitle,
		Child, GR_AddGamePath,
		Child, GR_AddGameGenre,
		Child, Space_AddGame,
		Child, GR_AddGameButtons,
		End;

	object->WI_AddNonWHDLoad = WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_AddNonWHDLoad),
		MUIA_Window_ID, MAKE_ID('3', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_3,
		End;


	object->CH_Screenshots = CheckMark(FALSE);

	LA_HideScreenshots = Label(GetMBString(MSG_LA_HideScreenshots));

	object->CH_NoGuiGfx = CheckMark(FALSE);
	if (blockGuiGfx)
	{
		set(object->CH_NoGuiGfx, MUIA_Disabled, TRUE);
	}

	LA_NoGuiGfx = Label(GetMBString(MSG_LA_NoGuiGfx));

	LA_ScreenshotSize = Label(GetMBString(MSG_LA_ScreenshotSize));

	Space_ScreenshotSize = HSpace(1);

	object->CY_ScreenshotSize = CycleObject,
		MUIA_HelpNode, "CY_ScreenshotSize",
		MUIA_Frame, MUIV_Frame_Button,
		MUIA_Cycle_Entries, object->CY_ScreenshotSizeContent,
		End;

	GR_ScreenshotSize = GroupObject,
		MUIA_HelpNode, "GR_ScreenshotSize",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_ScreenshotSize,
		Child, Space_ScreenshotSize,
		Child, object->CY_ScreenshotSize,
		End;

	LA_Width = TextObject,
		MUIA_Text_PreParse, "\033r",
		MUIA_Text_Contents, GetMBString(MSG_LA_Width),
		MUIA_Disabled, FALSE,
		MUIA_InnerLeft, 0,
		MUIA_InnerRight, 0,
		End;

	object->STR_Width = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Width",
		MUIA_String_Accept, "-0123456789",
		End;

	LA_Height = TextObject,
		MUIA_Text_PreParse, "\033r",
		MUIA_Text_Contents, GetMBString(MSG_LA_Height),
		MUIA_Disabled, FALSE,
		MUIA_InnerLeft, 0,
		MUIA_InnerRight, 0,
		End;

	object->STR_Height = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Height",
		MUIA_String_Accept, "-0123456789",
		End;

	GR_CustomSize = GroupObject,
		MUIA_HelpNode, "GR_CustomSize",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_Width,
		Child, object->STR_Width,
		Child, LA_Height,
		Child, object->STR_Height,
		End;

	GR_Screenshots = GroupObject,
		MUIA_HelpNode, "GR_Screenshots",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, GetMBString(MSG_GR_ScreenshotsTitle),
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, VGroup,
		       MUIA_Group_Columns, 2,
		       Child, LA_HideScreenshots,
		       Child, object->CH_Screenshots,
		       Child, LA_NoGuiGfx,
		       Child, object->CH_NoGuiGfx,
		End,
		Child, GR_ScreenshotSize,
		Child, GR_CustomSize,
		End;

	object->RA_TitlesFrom = RadioObject,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, GetMBString(MSG_RA_TitlesFromTitle),
		MUIA_HelpNode, "RA_TitlesFrom",
		MUIA_Radio_Entries, object->RA_TitlesFromContent,
		End;

	Space_TitlesFrom = VSpace(1);

	object->CH_SmartSpaces = CheckMark(FALSE);

	Space_SmartSpaces = VSpace(1);

	LA_SmartSpaces = TextObject,
		MUIA_Text_PreParse, "\033r",
		MUIA_Text_Contents, GetMBString(MSG_LA_SmartSpaces),
		MUIA_Disabled, FALSE,
		MUIA_InnerLeft, 0,
		MUIA_InnerRight, 0,
		End;

	GR_SmartSpaces = GroupObject,
		MUIA_HelpNode, "GR_SmartSpaces",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->CH_SmartSpaces,
		Child, Space_SmartSpaces,
		Child, LA_SmartSpaces,
		End;

	object->CH_UseIgameDataTitle = CheckMark(FALSE);

	LA_UseIgameDataTitle = TextObject,
		MUIA_Text_PreParse, "\033r",
		MUIA_Text_Contents, GetMBString(MSG_LA_UseIgameDataTitle),
		MUIA_Disabled, FALSE,
		MUIA_InnerLeft, 0,
		MUIA_InnerRight, 0,
		End;

	GR_UseIgameDataTitle = GroupObject,
		MUIA_HelpNode, "GR_UseIgameDataTitle",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->CH_UseIgameDataTitle,
		Child, VSpace(1),
		Child, LA_UseIgameDataTitle,
		End;

	GR_TitlesFrom = GroupObject,
		MUIA_HelpNode, "GR_TitlesFrom",
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->RA_TitlesFrom,
		Child, Space_TitlesFrom,
		Child, GR_SmartSpaces,
		Child, GR_UseIgameDataTitle,
	End;

	GR_Titles = GroupObject,
		MUIA_HelpNode, "GR_Titles",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, GetMBString(MSG_GR_TitlesTitle),
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, GR_TitlesFrom,
		End;

	GR_Settings = GroupObject,
		MUIA_HelpNode, "GR_Settings",
		MUIA_Group_Columns, 2,
		Child, GR_Screenshots,
		Child, GR_Titles,
		End;

	object->CH_SaveStatsOnExit = CheckMark(FALSE);

	LA_SaveStatsOnExit = Label(GetMBString(MSG_LA_SaveStatsOnExit));

	object->CH_FilterUseEnter = CheckMark(FALSE);

	LA_FilterUseEnter = Label(GetMBString(MSG_LA_FilterUseEnter));

	object->CH_HideSidepanel = CheckMark(FALSE);

	LA_HideSidepanel = Label(GetMBString(MSG_LA_HideSidepanel));

	object->CH_StartWithFavorites = CheckMark(FALSE);

	LA_StartWithFavorites = Label(GetMBString(MSG_LA_StartWithFavorites));

	GR_Misc = GroupObject,
		MUIA_HelpNode, "GR_Misc",
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Columns, 2,
		MUIA_FrameTitle, GetMBString(MSG_GR_MiscTitle),
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_SaveStatsOnExit,
		Child, object->CH_SaveStatsOnExit,
		Child, LA_FilterUseEnter,
		Child, object->CH_FilterUseEnter,
		Child, LA_HideSidepanel,
		Child, object->CH_HideSidepanel,
		Child, LA_StartWithFavorites,
		Child, object->CH_StartWithFavorites,
	End;

	object->BT_SettingsSave = SimpleButton(GetMBString(MSG_BT_SettingsSave));

	Space_SettingsButtons1 = VSpace(1);

	object->BT_SettingsUse = SimpleButton(GetMBString(MSG_BT_SettingsUse));

	Space_SettingsButtons2 = VSpace(1);

	object->BT_SettingsCancel = SimpleButton(GetMBString(MSG_BT_SettingsCancel));

	GR_SettingsButtons = GroupObject,
		MUIA_HelpNode, "GR_SettingsButtons",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->BT_SettingsSave,
		Child, Space_SettingsButtons1,
		Child, object->BT_SettingsUse,
		Child, Space_SettingsButtons2,
		Child, object->BT_SettingsCancel,
		End;

	GROUP_ROOT_Settings = GroupObject,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, GR_Settings,
		Child, GR_Misc,
		Child, GR_SettingsButtons,
		End;

	object->WI_Settings = WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_Settings),
		MUIA_Window_ID, MAKE_ID('5', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_Settings,
		End;

	object->App = ApplicationObject,
		MUIA_Application_Author, "Emmanuel Vasilakis",
		MUIA_Application_Base, "iGame",
		MUIA_Application_Title, "iGame",
		MUIA_Application_Version, VERSION,
		MUIA_Application_Copyright, GetMBString(MSG_AppCopyright),
		MUIA_Application_Description, GetMBString(MSG_AppDescription),
		MUIA_Application_HelpFile, "iGame.guide",
		MUIA_Application_DiskObject, GetDiskObject("PROGDIR:iGame"),
		SubWindow, object->WI_MainWindow,
		SubWindow, object->WI_Properties = getPropertiesWindow(object),
		SubWindow, object->WI_GameRepositories,
		SubWindow, object->WI_AddNonWHDLoad,
		SubWindow, object->WI_About = getAboutWindow(object),
		SubWindow, object->WI_Settings,
		SubWindow, object->WI_Information = getInformationWindow(object),
		End;

	if (!object->App)
	{
		FreeVec(object);
		return NULL;
	}

	setInformationWindowMethods(object);
	setPropertiesWindowMethods(object);
	setAboutWindowMethods(object);

	DoMethod(object->LV_GamesList, MUIM_Notify, MUIA_NList_TitleClick, MUIV_EveryTime,
		object->LV_GamesList, 4, MUIM_NList_Sort3, MUIV_TriggerValue, MUIV_NList_SortTypeAdd_2Values, MUIV_NList_Sort3_SortType_Both);
	DoMethod(object->LV_GamesList, MUIM_Notify, MUIA_NList_TitleClick2, MUIV_EveryTime,
		object->LV_GamesList, 4, MUIM_NList_Sort3, MUIV_TriggerValue, MUIV_NList_SortTypeAdd_2Values, MUIV_NList_Sort3_SortType_2);
	DoMethod(object->LV_GamesList, MUIM_Notify, MUIA_NList_SortType, MUIV_EveryTime,
		object->LV_GamesList, 3, MUIM_Set, MUIA_NList_TitleMark, MUIV_TriggerValue);
	DoMethod(object->LV_GamesList, MUIM_Notify, MUIA_NList_SortType2, MUIV_EveryTime,
		object->LV_GamesList, 3, MUIM_Set, MUIA_NList_TitleMark2, MUIV_TriggerValue);

	DoMethod(object->App,
		MUIM_Notify, MUIA_Application_Active, TRUE,
		object->WI_MainWindow,
		3,
		MUIM_Set, MUIA_Window_ActiveObject, object->LV_GamesList
	);

	DoMethod(object->App,
		MUIM_Notify, MUIA_Application_Active, TRUE,
		object->App,
		2,
		MUIM_CallHook, &AppStartHook
	);

	DoMethod(object->App,
		MUIM_Notify, MUIA_Application_Iconified, FALSE,
		object->WI_MainWindow,
		3,
		MUIM_Set, MUIA_Window_ActiveObject, object->LV_GamesList
	);

	DoMethod(MNMainOpenList,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuOpenListHook
		);

	DoMethod(MNMainSaveList,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuSaveListHook
	);

	DoMethod(MNMainSaveListAs,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuSaveListAsHook
	);

	// DoMethod(MNMainMenuDuplicate,
	// 	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
	// 	object->App,
	// 	2,
	// 	MUIM_CallHook, &MenuDuplicateHook
	// );

	// DoMethod(MNMainDelete,
	// 	MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
	// 	object->App,
	// 	2,
	// 	MUIM_CallHook, &MenuDeleteHook
	// );

	DoMethod(object->WI_MainWindow,
		MUIM_Notify, MUIA_Window_Activate, TRUE,
		object->WI_MainWindow,
		3,
		MUIM_Set, MUIA_Window_ActiveObject, object->LV_GamesList
	);

	DoMethod(object->WI_MainWindow,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
	);

	//call whenever the string is changed
	if (!current_settings->filter_use_enter) {
		DoMethod(object->STR_Filter,
			MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
			object->App,
			2,
			MUIM_CallHook, &FilterChangeHook
		);
	}
	else {
		//call only after return key is pressed
		DoMethod(object->STR_Filter,
			MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime,
			object->STR_Filter,
			2,
			MUIM_CallHook, &FilterChangeHook
		);
	}

	DoMethod(object->CY_FilterList,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &GenreClickHook
	);

	DoMethod(object->CY_ChipsetList,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &GenreClickHook
	);

	DoMethod(object->LV_GamesList,
		MUIM_Notify, MUIA_NList_Active, MUIV_EveryTime,
		object->LV_GamesList,
		2,
		MUIM_CallHook, &GameClickHook
	);

	DoMethod(object->LV_GamesList,
		MUIM_Notify, MUIA_NList_DoubleClick, TRUE,
		object->LV_GamesList,
		2,
		MUIM_CallHook, &LaunchGameHook
	);

	DoMethod(object->LV_GenresList,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		MUIV_Notify_Self,
		2,
		MUIM_CallHook, &GenreClickHook
	);

	DoMethod(object->WI_MainWindow,
		MUIM_Window_SetCycleChain, object->STR_Filter,
		object->LV_GamesList,
		object->LV_GenresList,
		0
	);

	DoMethod(object->WI_GameRepositories,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->App,
		2,
		MUIM_CallHook, &RepoStopHook
	);

	DoMethod(object->WI_GameRepositories,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->WI_GameRepositories,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->BT_AddRepo,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_CallHook, &RepoAddHook
	);

	DoMethod(object->BT_RemoveRepo,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_CallHook, &RepoStopHook
	);

	DoMethod(object->BT_RemoveRepo,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_CallHook, &RepoRemoveHook
	);

	DoMethod(object->BT_CloseRepoWindow,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_CallHook, &RepoStopHook
	);

	DoMethod(object->BT_CloseRepoWindow,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_GameRepositories,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->WI_GameRepositories,
		MUIM_Window_SetCycleChain, object->PA_RepoPath,
		object->BT_AddRepo,
		object->LV_GameRepositories,
		object->BT_RemoveRepo,
		object->BT_CloseRepoWindow,
		0
	);

	DoMethod(object->WI_AddNonWHDLoad,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->WI_AddNonWHDLoad,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->BT_AddGameOK,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_CallHook, &NonWHDLoadOKHook
	);

	DoMethod(object->BT_AddGameOK,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_AddNonWHDLoad,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->BT_AddGameCancel,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_AddNonWHDLoad,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->WI_AddNonWHDLoad,
		MUIM_Window_SetCycleChain, object->STR_AddTitle,
		object->PA_AddGame,
		object->CY_AddGameGenre,
		object->BT_AddGameOK,
		object->BT_AddGameCancel,
		0
	);


	DoMethod(object->WI_Settings,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->WI_Settings,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->CH_Screenshots,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingHideScreenshotChangedHook
	);

	DoMethod(object->CH_Screenshots,
		MUIM_Notify, MUIA_Selected, TRUE,
		GR_ScreenshotSize,
		3,
		MUIM_Set, MUIA_Disabled, TRUE
	);

	DoMethod(object->CH_Screenshots,
		MUIM_Notify, MUIA_Selected, FALSE,
		GR_ScreenshotSize,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
	);

	DoMethod(object->CH_Screenshots,
		MUIM_Notify, MUIA_Selected, FALSE,
		GR_CustomSize,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
	);

	DoMethod(object->CH_NoGuiGfx,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingNoGuiGfxChangedHook
	);

	DoMethod(object->CY_ScreenshotSize,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingScreenshotSizeChangedHook
	);

	DoMethod(object->RA_TitlesFrom,
		MUIM_Notify, MUIA_Radio_Active, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingTitlesFromChangedHook
	);

	DoMethod(object->CH_SmartSpaces,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingSmartSpacesChangedHook
	);

	DoMethod(object->CH_UseIgameDataTitle,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingUseIgameDataTitleHook
	);

	DoMethod(object->CH_SaveStatsOnExit,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingSaveStatsOnExitChangedHook
	);

	DoMethod(object->CH_FilterUseEnter,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingFilterUseEnterChangedHook
	);

	DoMethod(object->CH_HideSidepanel,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingHideSidePanelChangedHook
	);

	DoMethod(object->CH_StartWithFavorites,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &SettingStartWithFavoritesChangedHook
	);

	DoMethod(object->BT_SettingsSave,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_CallHook, &SettingsSaveHook
	);

	DoMethod(object->BT_SettingsSave,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Settings,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->BT_SettingsUse,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_CallHook, &SettingsUseHook
	);

	DoMethod(object->BT_SettingsCancel,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Settings,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->WI_Settings,
		MUIM_Window_SetCycleChain, object->CH_Screenshots,
		object->CH_NoGuiGfx,
		object->CY_ScreenshotSize,
		object->STR_Width,
		object->STR_Height,
		object->RA_TitlesFrom,
		object->CH_SmartSpaces,
		object->CH_UseIgameDataTitle,
		object->CH_SaveStatsOnExit,
		object->CH_FilterUseEnter,
		object->CH_HideSidepanel,
		object->BT_SettingsSave,
		object->BT_SettingsUse,
		object->BT_SettingsCancel,
		0
	);

#ifdef __morphos__
	DoMethod(MNlabelScan,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuScanHook
	);

	DoMethod(MNMainAddnonWHDLoadgame,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuAddNonWhdloadHook
	);

	DoMethod(MNMainMenuShowHidehiddenentries,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuShowHideHiddenHook
	);
	DoMethod(MNMainAbout,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->WI_About,
		3,
		MUIM_Set, MUIA_Window_Open, TRUE
	);

	DoMethod(MNMainQuit,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
	);

	DoMethod(MNMainProperties,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuPropertiesHook
	);

	DoMethod(MNMainIformation,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuItemInfoHook
	);

	//OPEN CURRENT DIR
	DoMethod(MNMainOpenCurrentDir,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &OpenCurrentDirHook
	);

	DoMethod(MNMainiGameSettings,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->WI_Settings,
		3,
		MUIM_Set, MUIA_Window_Open, TRUE
	);

	DoMethod(MNlabel2GameRepositories,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->WI_GameRepositories,
		3,
		MUIM_Set, MUIA_Window_Open, TRUE
	);

	DoMethod(MNMainMUISettings,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_Application_OpenConfigWindow, 0
	);
#endif

	return object;
}

void DisposeApp(struct ObjApp * object)
{
	if (object)
	{
		DoMethod(object->App,
			MUIM_Application_Save,
			MUIV_Application_Save_ENVARC
		);

		MUI_DisposeObject(object->App);
		FreeVec(object);
	}
}

BOOL checkImageDatatype(STRPTR filename)
{
	BOOL result = FALSE;
	BPTR lock;
	struct DataType *dtn;

	if((lock = Lock(filename, SHARED_LOCK)))
	{
		if((dtn = ObtainDataType(DTST_FILE, (APTR)lock, TAG_END)))
		{
			const struct DataTypeHeader *dth = dtn->dtn_Header;
			if (dth->dth_GroupID == GID_PICTURE)
			{
				result = TRUE;
			}

			ReleaseDataType(dtn);
		}
		UnLock(lock);
	}

	return result;
}

static void translateMenu(struct NewMenu *nm)
{
	while(nm->nm_Type != NM_END)
	{
		if(nm->nm_Label != NM_BARLABEL)
		{
			nm->nm_Label = GetMBString((ULONG)nm->nm_Label);
		}
		nm++;
	}
}

static void flagMenuItem(struct NewMenu *nm, APTR userData, UWORD flags)
{
	while(nm->nm_Type != NM_END)
	{
		if(nm->nm_Label != NM_BARLABEL)
		{
			if (nm->nm_UserData == userData)
			{
				nm->nm_Flags = flags;
			}
		}
		nm++;
	}
}
