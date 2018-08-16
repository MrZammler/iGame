#define MUI_OBSOLETE

#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <proto/exec.h>
#include <proto/intuition.h>

#include <MUI/Guigfx_mcc.h>
#include <mui/TextEditor_mcc.h>
//#include <libraries/NList_mcc.h>
//#include <libraries/NListview_mcc.h>
#include <libraries/gadtools.h> /* for Barlabel in MenuItem */
#include <exec/memory.h>
#include <proto/icon.h>
#include <proto/asl.h>
#include <dos/dos.h>

#include "version.h"
#include <string.h>

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#include "iGameGUI.h"
#include "iGameExtern.h"
#include "iGameStrings_cat.h"

#define MUIA_Dtpic_Name 0x80423d72

extern int SS_WIDTH, SS_HEIGHT;
extern int NOGUIGFX;
extern int FILTERUSEENTER;
extern int NOSCREENSHOT;

#define FILENAME_DEFAULT "PROGDIR:igame.iff"
#define FILENAME_HOTKEY 'f'
#define QUALITY_HOTKEY 'q'
#define QUALITY_DEFAULT MUIV_Guigfx_Quality_Low
#define SCALEUP_HOTKEY 'u'
#define SCALEUP_DEFAULT FALSE
#define SCALEDOWN_HOTKEY 'd'
#define SCALEDOWN_DEFAULT TRUE
#define TRANSMASK_HOTKEY 'm'
#define TRANSMASK_DEFAULT FALSE
#define TRANSCOLOR_HOTKEY 'c'
#define TRANSCOLOR_DEFAULT FALSE
#define TRANSRGB_HOTKEY 'r'
#define TRANSRGB_DEFAULT (0x0)
#define PICASPECT_DEFAULT TRUE
#define PICASPECT_HOTKEY 'a'
#define SCREENASPECT_DEFAULT TRUE
#define SCREENASPECT_HOTKEY 's'

#define SCALEMODEMASK(u,d,p,s) (((u)?GGSMF_SCALEUP:0)|((d)?GGSMF_SCALEDOWN:0)|((p)?GGSMF_KEEPASPECT_PICTURE:0)|((s)?GGSMF_KEEPASPECT_SCREEN:0))

static CONST_STRPTR GetMBString(CONST_STRPTR ref)
{
	if (ref[1] == '\0')
		return &ref[2];
	else
		return ref;
}

struct ObjApp * CreateApp(void)
{
	struct ObjApp * object;

	APTR	MNlabel2File, MNlabelScan, MNMainAddnonWHDLoadgame, MNMainMenuShowHidehiddenentries;
	APTR	MNMainBarLabel5, MNMainOpenList, MNMainSaveList, MNMainSaveListAs;
	APTR	MNMainBarLabel3, MNMainExportListtoTextfile, MNMainBarLabel0, MNMainAbout;
	APTR	MNMainBarLabel1, MNMainQuit, MNlabel2Edit, MNMainCopy, MNMainPasteasNew;
	APTR	MNMainDelete, MNMainBarLabel4, MNMainProperties, MNlabel2Tools, MNMainiGameSettings;
	APTR	MNlabel2GameRepositories, MNMainBarLabel2, MNMainMUISettings, GROUP_ROOT;
	APTR	GR_Filter, LA_Filter, GR_main, Space_Gamelist;
	APTR	GROUP_ROOT_1, GR_Genre, LA_PropertiesGenre, Space_Genre;
	APTR	GR_PropertiesChecks, obj_aux0, obj_aux1, Space_Properties, obj_aux2;
	APTR	obj_aux3, GR_TimesPlayed, LA_PropertiesTimesPlayed, Space_TimesPlayed;
	APTR	GR_SlavePath, LA_PropertiesSlavePath, Space_Path, GR_Tooltypes, Space_Tooltypes;
	APTR	GR_PropertiesButtons, Space_Buttons, GROUP_ROOT_2, GR_Path, GR_ReposButtons;
	APTR	GROUP_ROOT_3, GR_AddGameTitle, LA_AddGameTitle, GR_AddGamePath, LA_AddGamePath;
	APTR	GR_AddGameGenre, LA_AddGameGenre, Space_AddGame, GR_AddGameButtons;
	APTR	GROUP_ROOT_4, GROUP_ROOT_5, LA_Nothing;
#if defined(__amigaos4__)
	static const struct Hook MenuScanHook = { { NULL,NULL }, (HOOKFUNC)scan_repositories, NULL, NULL };
#else
	static const struct Hook MenuScanHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)scan_repositories, NULL };
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
	static const struct Hook MenuExportListHook = { { NULL,NULL }, (HOOKFUNC)export_list, NULL, NULL };
#else
	static const struct Hook MenuExportListHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)export_list, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuCopyHook = { { NULL,NULL }, (HOOKFUNC)menu_copy, NULL, NULL };
#else
	static const struct Hook MenuCopyHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_copy, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuPasteAsNewHook = { { NULL,NULL }, (HOOKFUNC)menu_paste_as_new, NULL, NULL };
#else
	static const struct Hook MenuPasteAsNewHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_paste_as_new, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuDeleteHook = { { NULL,NULL }, (HOOKFUNC)menu_delete, NULL, NULL };
#else
	static const struct Hook MenuDeleteHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_delete, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook PropertiesOKButtonHook = { { NULL,NULL }, (HOOKFUNC)game_properties_ok, NULL, NULL };
#else
	static const struct Hook PropertiesOKButtonHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)game_properties_ok, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook RepositoriesAddButtonHook = { { NULL,NULL }, (HOOKFUNC)repo_add, NULL, NULL };
#else
	static const struct Hook RepositoriesAddButtonHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)repo_add, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook AddNonWHDLoadGameHook = { { NULL,NULL }, (HOOKFUNC)add_non_whd_load_game, NULL, NULL };
#else
	static const struct Hook AddNonWHDLoadGameHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)add_non_whd_load_game, NULL };
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
	static const struct Hook MenuShowHideHiddenHook = { { NULL,NULL }, (HOOKFUNC)menu_show_hidden, NULL, NULL };
#else
	static const struct Hook MenuShowHideHiddenHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_show_hidden, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook AppStartHook = { { NULL,NULL }, (HOOKFUNC)app_start, NULL, NULL };
#else
	static const struct Hook AppStartHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)app_start, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuPropertiesHook = { { NULL,NULL }, (HOOKFUNC)menu_game_properties, NULL, NULL };
#else
	static const struct Hook MenuPropertiesHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_game_properties, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook MenuAddNonWhdloadHook = { { NULL,NULL }, (HOOKFUNC)menu_add_non_whdload, NULL, NULL };
#else
	static const struct Hook MenuAddNonWhdloadHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_add_non_whdload, NULL };
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

	if (!(object = AllocVec(sizeof(struct ObjApp), MEMF_PUBLIC | MEMF_CLEAR)))
		return NULL;

	char* about_text = "iGame ";
	strcat(about_text, VERSION);
	strcat(about_text, "\n\n");
	strcat(about_text, "(c) 2005-2018 Emmanuel Vasilakis\n");
	strcat(about_text, "mrzammler@gmail.com\n\n");
	strcat(about_text, "Updates by Dimitris Panokostas\n");
	strcat(about_text, "midwan@gmail.com\n\n");

	object->STR_TX_Status = NULL;
	object->STR_TX_PropertiesTimesPlayed = NULL;
	object->STR_TX_PropertiesSlavePath = NULL;
	object->STR_TX_PropertiesTooltypes = NULL;
	object->STR_TX_About = about_text;
	//object->STR_TX_Wait = "Writing to disk, please wait...";

	object->CY_PropertiesGenreContent[0] = GetMBString(MSG_CY_PropertiesGenre0);
	object->CY_PropertiesGenreContent[1] = NULL;
	object->CY_AddGameGenreContent[0] = GetMBString(MSG_CY_AddGameGenre0);
	object->CY_AddGameGenreContent[1] = NULL;

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
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, object->LV_GamesList,
		End;

	Space_Gamelist = HSpace(1);

	if (!NOSCREENSHOT) {

		if (NOGUIGFX) {
			object->IM_GameImage_0 = MUI_NewObject("Dtpic.mui",
				MUIA_Dtpic_Name, "PROGDIR:igame.iff",
				MUIA_Frame, MUIV_Frame_ImageButton,
				End;
		}
		else {
			object->IM_GameImage_0 = GuigfxObject,
				MUIA_Guigfx_FileName, FILENAME_DEFAULT,
				MUIA_Guigfx_Quality, MUIV_Guigfx_Quality_Best,
				MUIA_Guigfx_ScaleMode, NISMF_SCALEFREE,
				MUIA_Frame, MUIV_Frame_ImageButton,
				MUIA_FixHeight, SS_HEIGHT,
				MUIA_FixWidth, SS_WIDTH,
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
		MUIA_MaxWidth, SS_WIDTH,	//keep the same width as if there was a screenshot area
		End;

	if (!NOSCREENSHOT) {

		object->GR_sidepanel = GroupObject,
			MUIA_HelpNode, "GR_sidepanel",
			MUIA_Weight, 80,
			MUIA_Group_Rows, 3,
			Child, object->IM_GameImage_0,
			Child, object->Space_Sidepanel,
			Child, object->LV_GenresList,
			End;
	}
	else {

		object->GR_sidepanel = GroupObject,
			MUIA_HelpNode, "GR_sidepanel",
			MUIA_Weight, 80,
			MUIA_Group_Rows, 2,
			Child, object->Space_Sidepanel,
			Child, object->LV_GenresList,
			End;

	}

	GR_main = GroupObject,
		MUIA_HelpNode, "GR_main",
		MUIA_Group_Columns, 3,
		Child, object->LV_GamesList,
		Child, Space_Gamelist,
		Child, object->GR_sidepanel,
		End;

	object->TX_Status = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, object->STR_TX_Status,
		MUIA_Text_SetMin, TRUE,
		End;

	GROUP_ROOT = GroupObject,
		MUIA_Group_Rows, 3,
		MUIA_Group_HorizSpacing, 5,
		MUIA_Group_VertSpacing, 5,
		Child, GR_Filter,
		Child, GR_main,
		Child, object->TX_Status,
		End;

	MNlabelScan = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNlabelScan),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNlabelScanChar),
		End;

	MNMainAddnonWHDLoadgame = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainAddnonWHDLoadgame),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNMainAddnonWHDLoadgameChar),
		End;

	MNMainMenuShowHidehiddenentries = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainMenuShowHidehiddenentries),
		End;

	MNMainBarLabel5 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNMainOpenList = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainOpenList),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNMainOpenListChar),
		End;

	MNMainSaveList = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainSaveList),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNMainSaveListChar),
		End;

	MNMainSaveListAs = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainSaveListAs),
		End;

	MNMainBarLabel3 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNMainExportListtoTextfile = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainExportListtoTextfile),
		End;

	MNMainBarLabel0 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNMainAbout = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainAbout),
		End;

	MNMainBarLabel1 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNMainQuit = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainQuit),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNMainQuitChar),
		End;

	MNlabel2File = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNlabel2File),
		MUIA_Family_Child, MNlabelScan,
		MUIA_Family_Child, MNMainAddnonWHDLoadgame,
		MUIA_Family_Child, MNMainMenuShowHidehiddenentries,
		MUIA_Family_Child, MNMainBarLabel5,
		MUIA_Family_Child, MNMainOpenList,
		MUIA_Family_Child, MNMainSaveList,
		MUIA_Family_Child, MNMainSaveListAs,
		MUIA_Family_Child, MNMainBarLabel3,
		MUIA_Family_Child, MNMainExportListtoTextfile,
		MUIA_Family_Child, MNMainBarLabel0,
		MUIA_Family_Child, MNMainAbout,
		MUIA_Family_Child, MNMainBarLabel1,
		MUIA_Family_Child, MNMainQuit,
		End;

	MNMainCopy = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainCopy),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNMainCopyChar),
		End;

	MNMainPasteasNew = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainPasteasNew),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNMainPasteasNewChar),
		End;

	MNMainDelete = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainDelete),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNMainDeleteChar),
		End;

	MNMainBarLabel4 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNMainProperties = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNMainProperties),
		MUIA_Menuitem_Shortcut, GetMBString(MSG_MNMainPropertiesChar),
		End;

	MNlabel2Edit = MenuitemObject,
		MUIA_Menuitem_Title, GetMBString(MSG_MNlabel2Edit),
		MUIA_Family_Child, MNMainCopy,
		MUIA_Family_Child, MNMainPasteasNew,
		MUIA_Family_Child, MNMainDelete,
		MUIA_Family_Child, MNMainBarLabel4,
		MUIA_Family_Child, MNMainProperties,
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
		MUIA_Family_Child, MNlabel2File,
		MUIA_Family_Child, MNlabel2Edit,
		MUIA_Family_Child, MNlabel2Tools,
		End;

	object->WI_MainWindow = WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_MainWindow),
		MUIA_Window_Menustrip, object->MN_MainMenu,
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

	Space_Tooltypes = HVSpace;

	GR_Tooltypes = GroupObject,
		MUIA_HelpNode, "GR_Tooltypes",
		Child, object->TX_PropertiesTooltypes,
		Child, Space_Tooltypes,
		End;

	object->BT_PropertiesOK = SimpleButton(GetMBString(MSG_BT_PropertiesOK));

	Space_Buttons = HSpace(1);

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
		ASLFR_TitleText, "Select dir...",
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
		MUIA_Listview_MultiSelect, MUIV_Listview_MultiSelect_Default,
		MUIA_Listview_List, object->LV_GameRepositories,
		End;

	object->BT_RemoveRepo = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, GetMBString(MSG_BT_RemoveRepo),
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_RepoRemove",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	object->BT_CloseRepoWindow = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, GetMBString(MSG_BT_CloseRepoWindow),
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_RepoClose",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

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
		ASLFR_TitleText, "Select game executable...",
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

	object->BT_AddGameOK = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, GetMBString(MSG_BT_AddGameOK),
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_AddGameOk",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	object->BT_AddGameCancel = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, GetMBString(MSG_BT_AddGameCancel),
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_AddGameCancel",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

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

	LA_Nothing = Label(GetMBString(MSG_LA_Nothing));

	GROUP_ROOT_5 = GroupObject,
		Child, LA_Nothing,
		End;

	object->WI_Settings = WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_Settings),
		MUIA_Window_ID, MAKE_ID('5', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_5,
		End;

	object->App = ApplicationObject,
		MUIA_Application_Author, "Emmanuel Vasilakis",
		MUIA_Application_Base, "iGame",
		MUIA_Application_Title, "iGame",
		MUIA_Application_Version, VERSION,
		MUIA_Application_Copyright, GetMBString(MSG_AppCopyright),
		MUIA_Application_Description, GetMBString(MSG_AppDescription),
		MUIA_Application_HelpFile, "igame.guide",
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
		return(NULL);
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

	DoMethod(MNMainExportListtoTextfile,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuExportListHook
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

	DoMethod(MNMainCopy,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuCopyHook
	);

	DoMethod(MNMainPasteasNew,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuPasteAsNewHook
	);

	DoMethod(MNMainDelete,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuDeleteHook
	);

	DoMethod(MNMainProperties,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object->App,
		2,
		MUIM_CallHook, &MenuPropertiesHook
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
	if (FILTERUSEENTER == 0) {

		DoMethod(object->STR_Filter,
			MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
			object->STR_Filter,
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
		MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
		object->LV_GamesList,
		2,
		MUIM_CallHook, &LaunchGameHook
	);

	DoMethod(object->LV_GamesList,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		object->LV_GamesList,
		2,
		MUIM_CallHook, &GameClickHook
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
		object->WI_GameRepositories,
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

	DoMethod(object->WI_Settings,
		MUIM_Window_SetCycleChain, 0
	);

	// Delay opening the Window until after we've loaded everything
	/*set(object->WI_MainWindow,
		MUIA_Window_Open, TRUE
	);*/

	return object;
}

void DisposeApp(struct ObjApp * object)
{
	if (object)
	{
		MUI_DisposeObject(object->App);
		FreeVec(object);
		object = NULL;
	}
}
