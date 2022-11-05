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

#define MUI_OBSOLETE

/* MUI */
#include <libraries/mui.h>
#include <mui/Guigfx_mcc.h>
#include <mui/TextEditor_mcc.h>

/* Prototypes */
#include <clib/alib_protos.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <proto/asl.h>
#include <proto/datatypes.h>
#include <proto/dos.h>
#include <proto/muimaster.h>

/* System */
#include <libraries/gadtools.h> /* for Barlabel in MenuItem */
#include <exec/memory.h>
#include <dos/dos.h>
#if defined(__amigaos4__)
#include <dos/obsolete.h>
#endif

/* ANSI C */
#include <string.h>
#include <stdio.h>

#ifndef CPU_VERS
#define CPU_VERS 68000
#endif

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif


#define iGame_NUMBERS
#include "iGame_strings.h"

#include "version.h"
#include "iGameExtern.h"
#include "fsfuncs.h"
#include "funcs.h"
#include "strfuncs.h"
#include "iGameGUI.h"

extern igame_settings *current_settings;

static void translateMenu(struct NewMenu *);
static void flagMenuItem(struct NewMenu *, APTR, UWORD);

#define TICK (CHECKIT|MENUTOGGLE)
#define DIS  NM_ITEMDISABLED
#define STR_ID(x) ( (STRPTR)(x) )

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
	{ NM_ITEM ,  STR_ID(MSG_MNMainOpenCurrentDir)					,"D",0 ,0			,(APTR)MENU_GAMEFOLDER },

	{ NM_TITLE, STR_ID(MSG_MNMainPreferences)						, 0 ,0 ,0			,(APTR)MENU_PREFERENCES },
	{ NM_ITEM ,  STR_ID(MSG_MNMainiGameSettings)					, 0 ,0 ,0			,(APTR)MENU_SETTINGS },
	{ NM_ITEM ,  STR_ID(MSG_MNlabel2GameRepositories)				, 0 ,0 ,0			,(APTR)MENU_REPOSITORIES },
	{ NM_ITEM ,  NM_BARLABEL										, 0 ,0 ,0			,(APTR)0 },
	{ NM_ITEM ,  STR_ID(MSG_MNMainMUISettings)						, 0 ,0 ,0			,(APTR)MENU_MUISETTINGS },

	{ NM_END,NULL,0,0,0,NULL }
};

struct ObjApp *CreateApp(void)
{
	struct ObjApp *object;
	APTR strip;
	static char about_text[512];
	static char version_string[16];

	translateMenu(MenuMainWin);

	snprintf(version_string, sizeof(version_string),
		"%s v%d.%d.%d",
		GetMBString(MSG_WI_MainWindow),
		MAJOR_VERS, MINOR_VERS, PATCH_VERS
	);

	snprintf(about_text, sizeof(about_text),
		"%s (%s)\n%s %s\n\nCopyright 2005-%d\n%s"
		, version_string, STR(RELEASE_DATE), GetMBString(MSG_compiledForAboutWin), STR(CPU_VERS), COPY_END_YEAR, GetMBString(MSG_TX_About)
	);

	APTR	MNMainOpenList, MNMainSaveList, MNMainSaveListAs;
	APTR	MNMainMenuDuplicate;
	APTR	MNMainDelete;
	APTR	GROUP_ROOT;
	APTR	GR_Filter, LA_Filter, GR_main, Space_Gamelist;
	APTR	GROUP_ROOT_1, GR_Genre, LA_PropertiesGenre, Space_Genre;
	APTR	GR_PropertiesChecks, obj_aux0, obj_aux1, Space_Properties, obj_aux2;
	APTR	obj_aux3, GR_TimesPlayed, LA_PropertiesTimesPlayed, Space_TimesPlayed;
	APTR	GR_SlavePath, LA_PropertiesSlavePath, Space_Path, GR_Tooltypes, GR_PropertiesButtons;
	APTR	Space_Buttons, GROUP_ROOT_2, GR_Path, GR_ReposButtons, GROUP_ROOT_3;
	APTR	GR_AddGameTitle, LA_AddGameTitle, GR_AddGamePath, LA_AddGamePath;
	APTR	GR_AddGameGenre, LA_AddGameGenre, Space_AddGame, GR_AddGameButtons;
	APTR	GROUP_ROOT_4, GROUP_ROOT_Settings, GR_Settings;
	APTR	LA_HideScreenshots, GR_Screenshots;
	APTR	LA_NoGuiGfx, GR_ScreenshotSize, LA_ScreenshotSize;
	APTR	Space_ScreenshotSize, GR_CustomSize, LA_Width, LA_Height, GR_Titles;
	APTR	GR_TitlesFrom, Space_TitlesFrom, GR_SmartSpaces, Space_SmartSpaces;
	APTR	LA_SmartSpaces, GR_Misc;
	APTR	LA_SaveStatsOnExit, LA_FilterUseEnter, LA_StartWithFavorites;
	APTR	LA_HideSidepanel;
	APTR	GR_SettingsButtons, Space_SettingsButtons1, Space_SettingsButtons2;

#if defined(__amigaos4__)
	static const struct Hook MenuOpenListHook = { { NULL,NULL }, (HOOKFUNC)open_list, NULL, NULL };
#else
	static const struct Hook MenuOpenListHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)open_list, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuSaveListHook = { { NULL,NULL }, (HOOKFUNC)save_list, NULL, NULL };
#else
	static const struct Hook MenuSaveListHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)save_list, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuSaveListAsHook = { { NULL,NULL }, (HOOKFUNC)save_list_as, NULL, NULL };
#else
	static const struct Hook MenuSaveListAsHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)save_list_as, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuDuplicateHook = { { NULL,NULL }, (HOOKFUNC)game_duplicate, NULL, NULL };
#else
	static const struct Hook MenuDuplicateHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)game_duplicate, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuDeleteHook = { { NULL,NULL }, (HOOKFUNC)game_delete, NULL, NULL };
#else
	static const struct Hook MenuDeleteHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)game_delete, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook PropertiesOKButtonHook = { { NULL,NULL }, (HOOKFUNC)game_properties_ok, NULL, NULL };
#else
	static const struct Hook PropertiesOKButtonHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)game_properties_ok, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook FilterChangeHook = { { NULL,NULL }, (HOOKFUNC)filter_change, NULL, NULL };
#else
	static const struct Hook FilterChangeHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)filter_change, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook LaunchGameHook = { { NULL,NULL }, (HOOKFUNC)launch_game, NULL, NULL };
#else
	static const struct Hook LaunchGameHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)launch_game, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook AppStartHook = { { NULL,NULL }, (HOOKFUNC)app_start, NULL, NULL };
#else
	static const struct Hook AppStartHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)app_start, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook GameClickHook = { { NULL,NULL }, (HOOKFUNC)game_click, NULL, NULL };
#else
	static const struct Hook GameClickHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)game_click, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook GenreClickHook = { { NULL,NULL }, (HOOKFUNC)genres_click, NULL, NULL };
#else
	static const struct Hook GenreClickHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)genres_click, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook NonWHDLoadOKHook = { { NULL,NULL }, (HOOKFUNC)non_whdload_ok, NULL, NULL };
#else
	static const struct Hook NonWHDLoadOKHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)non_whdload_ok, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook RepoStopHook = { { NULL,NULL }, (HOOKFUNC)repo_stop, NULL, NULL };
#else
	static const struct Hook RepoStopHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)repo_stop, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook RepoAddHook = { { NULL,NULL }, (HOOKFUNC)repo_add, NULL, NULL };
#else
	static const struct Hook RepoAddHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)repo_add, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook RepoRemoveHook = { { NULL,NULL }, (HOOKFUNC)repo_remove, NULL, NULL };
#else
	static const struct Hook RepoRemoveHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)repo_remove, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingFilterUseEnterChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_filter_use_enter_changed, NULL, NULL };
#else
	static const struct Hook SettingFilterUseEnterChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_filter_use_enter_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingSaveStatsOnExitChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_save_stats_on_exit_changed, NULL, NULL };
#else
	static const struct Hook SettingSaveStatsOnExitChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_save_stats_on_exit_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingSmartSpacesChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_smart_spaces_changed, NULL, NULL };
#else
	static const struct Hook SettingSmartSpacesChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_smart_spaces_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingTitlesFromChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_titles_from_changed, NULL, NULL };
#else
	static const struct Hook SettingTitlesFromChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_titles_from_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingHideScreenshotChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_hide_screenshot_changed, NULL, NULL };
#else
	static const struct Hook SettingHideScreenshotChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_hide_screenshot_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingNoGuiGfxChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_no_guigfx_changed, NULL, NULL };
#else
	static const struct Hook SettingNoGuiGfxChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_no_guigfx_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingScreenshotSizeChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_screenshot_size_changed, NULL, NULL };
#else
	static const struct Hook SettingScreenshotSizeChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_screenshot_size_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingsSaveHook = { { NULL,NULL }, (HOOKFUNC)settings_save, NULL, NULL };
#else
	static const struct Hook SettingsSaveHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)settings_save, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingHideSidePanelChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_hide_side_panel_changed, NULL, NULL };
#else
	static const struct Hook SettingHideSidePanelChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_hide_side_panel_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingStartWithFavoritesChangedHook = { { NULL,NULL }, (HOOKFUNC)setting_start_with_favorites_changed, NULL, NULL };
#else
	static const struct Hook SettingStartWithFavoritesChangedHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)setting_start_with_favorites_changed, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SettingsUseHook = { { NULL,NULL }, (HOOKFUNC)settings_use, NULL, NULL };
#else
	static const struct Hook SettingsUseHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)settings_use, NULL };
#endif


	if (!((object = AllocVec(sizeof(struct ObjApp), MEMF_PUBLIC | MEMF_CLEAR))))
		return NULL;

	object->STR_TX_Status = NULL;
	object->STR_TX_PropertiesTimesPlayed = NULL;
	object->STR_TX_PropertiesSlavePath = NULL;
	object->STR_TX_PropertiesTooltypes = NULL;

	object->STR_TX_About = about_text;

	object->CY_PropertiesGenreContent[0] = (CONST_STRPTR)GetMBString(MSG_CY_PropertiesGenre0);
	object->CY_PropertiesGenreContent[1] = NULL;
	object->CY_AddGameGenreContent[0] = (CONST_STRPTR)GetMBString(MSG_CY_AddGameGenre0);
	object->CY_AddGameGenreContent[1] = NULL;
	object->CY_ScreenshotSizeContent[0] = (CONST_STRPTR)GetMBString(MSG_CY_ScreenshotSize0);
	object->CY_ScreenshotSizeContent[1] = (CONST_STRPTR)GetMBString(MSG_CY_ScreenshotSize1);
	object->CY_ScreenshotSizeContent[2] = (CONST_STRPTR)GetMBString(MSG_CY_ScreenshotSize2);
	object->CY_ScreenshotSizeContent[3] = NULL;
	object->RA_TitlesFromContent[0] = (CONST_STRPTR)GetMBString(MSG_RA_TitlesFrom0);
	object->RA_TitlesFromContent[1] = (CONST_STRPTR)GetMBString(MSG_RA_TitlesFrom1);
	object->RA_TitlesFromContent[2] = NULL;

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

	object->LV_GamesList = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_Active, MUIV_List_Active_Top,
		End;

	object->LV_GamesList = ListviewObject,
		MUIA_HelpNode, "LV_GamesList",
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_None,
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, object->LV_GamesList,
		End;

	if (!current_settings->hide_side_panel)
	{
		Space_Gamelist = HSpace(1);

		if (!current_settings->hide_screenshots) {

			if (current_settings->no_guigfx) {
				object->IM_GameImage_0 = MUI_NewObject(Dtpic_Classname,
					MUIA_Dtpic_Name, DEFAULT_SCREENSHOT_FILE,
					MUIA_Frame, MUIV_Frame_ImageButton,
					End;
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
		}

		object->Space_Sidepanel = VSpace(1);

		object->LV_GenresList = ListObject,
			MUIA_Frame, MUIV_Frame_InputList,
		        MUIA_List_Active, MUIV_List_Active_Top,
			End;

		object->LV_GenresList = ListviewObject,
			MUIA_HelpNode, "LV_GenresList",
			MUIA_FrameTitle, GetMBString(MSG_LV_GenresListTitle),
			MUIA_Listview_List, object->LV_GenresList,
			MUIA_MaxWidth, current_settings->screenshot_width,	//keep the same width as if there was a screenshot area
			End;

		if (!current_settings->hide_screenshots) {

			object->GR_sidepanel = GroupObject,
				MUIA_HelpNode, "GR_sidepanel",
				Child, object->IM_GameImage_0,
				Child, object->Space_Sidepanel,
				Child, object->LV_GenresList,
				End;
		}
		else {

			object->GR_sidepanel = GroupObject,
				MUIA_HelpNode, "GR_sidepanel",
				Child, object->Space_Sidepanel,
				Child, object->LV_GenresList,
				End;
		}

		GR_main = GroupObject,
			MUIA_HelpNode, "GR_main",
			MUIA_Group_Horiz, TRUE,
			Child, object->LV_GamesList, MUIA_Weight, 60,
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
			Child, object->LV_GamesList,
			End;
	}

	object->TX_Status = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
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

	MNMainMenuDuplicate = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainMenuDuplicate),
		End;

	MNMainDelete = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainDelete),
		MUIA_Menuitem_Shortcut, MENU_DELETE_HOTKEY,
		End;

	if (get_wb_version() < 44)
	{
		flagMenuItem(MenuMainWin, (APTR)MENU_GAMEFOLDER, DIS);
	}

	object->WI_MainWindow = WindowObject,
		MUIA_Window_ScreenTitle, version_string,
		MUIA_Window_Title, GetMBString(MSG_WI_MainWindow),
		MUIA_Window_Menustrip, strip = MUI_MakeObject(MUIO_MenustripNM, MenuMainWin, 0),
		MUIA_Window_ID, MAKE_ID('0', 'I', 'G', 'A'),
		MUIA_Window_AppWindow, TRUE,
		WindowContents, GROUP_ROOT,
		End;

	object->STR_PropertiesGameTitle = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_FrameTitle, GetMBString(MSG_STR_PropertiesGameTitleTitle),
		MUIA_HelpNode, "STR_PropertiesGameTitle",
		End;

	LA_PropertiesGenre = Label(GetMBString(MSG_LA_PropertiesGenre));

	Space_Genre = HSpace(1);

	object->CY_PropertiesGenre = CycleObject,
		MUIA_HelpNode, "CY_PropertiesGenre",
		MUIA_Frame, MUIV_Frame_Button,
		MUIA_Cycle_Entries, object->CY_PropertiesGenreContent,
		End;

	GR_Genre = GroupObject,
		MUIA_HelpNode, "GR_Genre",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_PropertiesGenre,
		Child, Space_Genre,
		Child, object->CY_PropertiesGenre,
		End;

	object->CH_PropertiesFavorite = CheckMark(FALSE);

	obj_aux1 = Label2(GetMBString(MSG_CH_PropertiesFavorite));

	obj_aux0 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux1,
		Child, object->CH_PropertiesFavorite,
		End;

	Space_Properties = HVSpace;

	object->CH_PropertiesHidden = CheckMark(FALSE);

	obj_aux3 = Label2(GetMBString(MSG_CH_PropertiesHidden));

	obj_aux2 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux3,
		Child, object->CH_PropertiesHidden,
		End;

	GR_PropertiesChecks = GroupObject,
		MUIA_HelpNode, "GR_PropertiesChecks",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, obj_aux0,
		Child, Space_Properties,
		Child, obj_aux2,
		End;

	LA_PropertiesTimesPlayed = Label(GetMBString(MSG_LA_PropertiesTimesPlayed));

	Space_TimesPlayed = HSpace(1);

	object->TX_PropertiesTimesPlayed = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, object->STR_TX_PropertiesTimesPlayed,
		MUIA_Text_SetMin, TRUE,
		End;

	GR_TimesPlayed = GroupObject,
		MUIA_HelpNode, "GR_TimesPlayed",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_PropertiesTimesPlayed,
		Child, Space_TimesPlayed,
		Child, object->TX_PropertiesTimesPlayed,
		End;

	LA_PropertiesSlavePath = Label(GetMBString(MSG_LA_PropertiesSlavePath));

	Space_Path = HSpace(1);

	object->TX_PropertiesSlavePath = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, object->STR_TX_PropertiesSlavePath,
		MUIA_Text_SetMin, TRUE,
		End;

	GR_SlavePath = GroupObject,
		MUIA_HelpNode, "GR_SlavePath",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, LA_PropertiesSlavePath,
		Child, Space_Path,
		Child, object->TX_PropertiesSlavePath,
		End;

	object->TX_PropertiesTooltypes = TextEditorObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_FrameTitle, GetMBString(MSG_TX_PropertiesTooltypesTitle),
		End;

	GR_Tooltypes = GroupObject,
		MUIA_HelpNode, "GR_Tooltypes",
		Child, object->TX_PropertiesTooltypes,
		End;

	object->BT_PropertiesOK = SimpleButton(GetMBString(MSG_BT_PropertiesOK));

	Space_Buttons = VSpace(1);

	object->BT_PropertiesCancel = SimpleButton(GetMBString(MSG_BT_PropertiesCancel));

	GR_PropertiesButtons = GroupObject,
		MUIA_HelpNode, "GR_PropertiesButtons",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->BT_PropertiesOK,
		Child, Space_Buttons,
		Child, object->BT_PropertiesCancel,
		End;

	GROUP_ROOT_1 = GroupObject,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->STR_PropertiesGameTitle,
		Child, GR_Genre,
		Child, GR_PropertiesChecks,
		Child, GR_TimesPlayed,
		Child, GR_SlavePath,
		Child, GR_Tooltypes,
		Child, GR_PropertiesButtons,
		End;

	object->WI_Properties = WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_Properties),
		MUIA_Window_ID, MAKE_ID('1', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_1,
		End;

	object->STR_PA_RepoPath = String("", 80);

	object->PA_RepoPath = PopButton(MUII_PopDrawer);

	object->PA_RepoPath = PopaslObject,
		MUIA_HelpNode, "PA_RepoPath",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_RepoPath,
		MUIA_Popstring_Button, object->PA_RepoPath,
		ASLFR_TitleText, GetMBString(MSG_SelectDir),
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
		MUIA_Cycle_Entries, object->CY_AddGameGenreContent,
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

	object->TX_About = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Text_Contents, object->STR_TX_About,
		MUIA_Text_SetMin, TRUE,
		End;

	object->BT_AboutOK = SimpleButton(GetMBString(MSG_BT_AboutOK));

	GROUP_ROOT_4 = GroupObject,
		Child, object->TX_About,
		Child, object->BT_AboutOK,
		End;

	object->WI_About = WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_About),
		MUIA_Window_ID, MAKE_ID('4', 'I', 'G', 'A'),
		MUIA_Window_SizeGadget, FALSE,
		WindowContents, GROUP_ROOT_4,
		End;

	object->CH_Screenshots = CheckMark(FALSE);

	LA_HideScreenshots = Label(GetMBString(MSG_LA_HideScreenshots));

	object->CH_NoGuiGfx = CheckMark(FALSE);

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

	object->CH_SmartSpaces = ImageObject,
		MUIA_Disabled, TRUE,
		MUIA_InputMode, MUIV_InputMode_Toggle,
		MUIA_Image_Spec, MUII_CheckMark,
		MUIA_Image_FreeVert, TRUE,
		MUIA_Selected, FALSE,
		MUIA_ShowSelState, FALSE,
		End;

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

	GR_TitlesFrom = GroupObject,
		MUIA_HelpNode, "GR_TitlesFrom",
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, object->RA_TitlesFrom,
		Child, Space_TitlesFrom,
		Child, GR_SmartSpaces,
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
		SubWindow, object->WI_Properties,
		SubWindow, object->WI_GameRepositories,
		SubWindow, object->WI_AddNonWHDLoad,
		SubWindow, object->WI_About,
		SubWindow, object->WI_Settings,
		End;


	if (!object->App)
	{
		FreeVec(object);
		return NULL;
	}

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

	DoMethod(MNMainMenuDuplicate,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuDuplicateHook
	);

	DoMethod(MNMainDelete,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuDeleteHook
	);

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

	DoMethod(object->LV_GamesList,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		object->LV_GamesList,
		2,
		MUIM_CallHook, &GameClickHook
	);

	DoMethod(object->LV_GamesList,
		MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
		object->LV_GamesList,
		2,
		MUIM_CallHook, &LaunchGameHook
	);

	DoMethod(object->LV_GenresList,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		object->LV_GenresList,
		2,
		MUIM_CallHook, &GenreClickHook
	);

	DoMethod(object->WI_MainWindow,
		MUIM_Window_SetCycleChain, object->STR_Filter,
		object->LV_GamesList,
		object->LV_GenresList,
		0
	);

	DoMethod(object->WI_Properties,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->WI_Properties,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->BT_PropertiesOK,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_CallHook, &PropertiesOKButtonHook
	);

	DoMethod(object->BT_PropertiesOK,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Properties,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->BT_PropertiesCancel,
		MUIM_Notify, MUIA_Pressed, TRUE,
		object->WI_Properties,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->WI_Properties,
		MUIM_Window_SetCycleChain, object->STR_PropertiesGameTitle,
		object->CY_PropertiesGenre,
		object->CH_PropertiesFavorite,
		object->CH_PropertiesHidden,
		object->BT_PropertiesOK,
		object->BT_PropertiesCancel,
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

	DoMethod(object->WI_About,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->WI_About,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->BT_AboutOK,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_About,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->WI_About,
		MUIM_Window_SetCycleChain, object->BT_AboutOK,
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
		object->CH_SaveStatsOnExit,
		object->CH_FilterUseEnter,
		object->CH_HideSidepanel,
		object->BT_SettingsSave,
		object->BT_SettingsUse,
		object->BT_SettingsCancel,
		0
	);

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
