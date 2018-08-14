/*
  iGameGUI.c
  Implements MUI GUI for iGame
  
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

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

/* Libraries */
#include <libraries/mui.h>

#include <MUI/Guigfx_mcc.h>
#include <MUI/TextEditor_mcc.h>
//#include <libraries/NList_mcc.h>
//#include <libraries/NListview_mcc.h>
#include <libraries/gadtools.h> /* for Barlabel in MenuItem */
#include <exec/memory.h>
#include <proto/icon.h>
#include <proto/asl.h>
#include <dos/dos.h>
#include <clib/alib_protos.h>
#include "version.h"

#define MUIA_Dtpic_Name 0x80423d72

extern int SS_WIDTH, SS_HEIGHT;
extern int NOGUIGFX;
extern int FILTERUSEENTER;
extern int NOSCREENSHOT;

/* Prototypes */
#include <proto/muimaster.h>
#include <proto/exec.h>
#include <clib/alib_protos.h>
#include <proto/intuition.h>

#include <clib/muimaster_protos.h>
#include <pragmas/muimaster_pragmas.h>

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

#include "iGameGUI.h"
#include "iGameExtern.h"

struct ObjApp * CreateApp(void)
{
	struct ObjApp * object_app;

	APTR	MNlabel1Actions, MNlabel1ScanRepository, MNlabel1Addslave, MNlabel1BarLabel1;
	APTR	MNlabel1About, MNlabel1BarLabel2, MNlabel1BarLabel3, MNlabel1MUISettings, MNlabel1Quit, MNlabel1Settings, MNlabel1ShowHidden;
	APTR	MNlabel1Gamespath, MNlabel1Game, MNlabel1GameProperties, GROUP_ROOT_0, IM_Pic;
	APTR	GR_grp_Img, GR_grp_5, GR_grp_2, obj_aux0, obj_aux1, obj_aux2, obj_aux3, obj_aux4, GR_grp_0, GROUP_ROOT_1, GR_grp_4, GR_grp_6, GR_grp_7;
	APTR	GROUP_ROOT_2, GR_grp_3, GROUP_ROOT_3, GROUP_ROOT_4, GROUP_ROOT_5, GROUP_ROOT_6;
	static struct Hook FilterChangeHook = {{NULL,NULL}, HookEntry, (HOOKFUNC)FilterChange, NULL };
	static struct Hook GameDoubleClickHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)game_double_click, NULL };
	static struct Hook MenuScanHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_scan, NULL };
	static struct Hook AppStartHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)AppStart, NULL };
	static struct Hook RepoAddHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)repo_add, NULL };
	static struct Hook RepoRemoveHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)repo_remove, NULL };
	static struct Hook RepoStopHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)repo_stop, NULL };
	static struct Hook GameClickHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)game_click, NULL };
	static struct Hook MenuGamePropertiesHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_game_properties, NULL };
	static struct Hook GamePropertiesOkHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)game_properties_ok, NULL };
	static struct Hook GenresClickHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)genres_click, NULL };
	static struct Hook MenuAddNonWhdloadHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_add_non_whdload, NULL };
	static struct Hook NonWhdloadOkHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)non_whdload_ok, NULL };
	static struct Hook MenuShowHiddenHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)menu_show_hidden, NULL };

	if (!(object_app = AllocVec(sizeof(struct ObjApp), MEMF_PUBLIC | MEMF_CLEAR)))
		return(NULL);

	object_app->STR_TX_Status = NULL;
	object_app->STR_TX_Path = NULL;
	object_app->STR_TX_About = "iGame version 1.8 beta\n\n(c) 2005-2018 Emmanuel Vasilakis\nmrzammler@gmail.com\n\nUpdates by Dimitris Panokostas\nmidwan@gmail.com";
	object_app->STR_TX_Wait = "Writing to disk, please wait...";

	object_app->CY_Genre_Content[0] = "Unknown";
	object_app->CY_Genre_Content[1] = NULL;
	/*
	ObjectApp->CY_Genre_Content[1] = "Adult";
	ObjectApp->CY_Genre_Content[2] = "Adventure";
	ObjectApp->CY_Genre_Content[3] = "Bat and ball";
	ObjectApp->CY_Genre_Content[4] = "Beat 'em up";
	ObjectApp->CY_Genre_Content[5] = "Board";
	ObjectApp->CY_Genre_Content[6] = "Cards";
	ObjectApp->CY_Genre_Content[7] = "Demo";
	ObjectApp->CY_Genre_Content[8] = "Gambling";
	ObjectApp->CY_Genre_Content[9] = "Maze";
	ObjectApp->CY_Genre_Content[10] = "Misc";
	ObjectApp->CY_Genre_Content[11] = "Pinball";
	ObjectApp->CY_Genre_Content[12] = "Platform";
	ObjectApp->CY_Genre_Content[13] = "Puzzle";
	ObjectApp->CY_Genre_Content[14] = "Quiz";
	ObjectApp->CY_Genre_Content[15] = "Racing";
	ObjectApp->CY_Genre_Content[16] = "RPG";
	ObjectApp->CY_Genre_Content[17] = "Shoot 'em up";
	ObjectApp->CY_Genre_Content[18] = "Simulation";
	ObjectApp->CY_Genre_Content[19] = "Sports";
	ObjectApp->CY_Genre_Content[20] = "Strategy";
	ObjectApp->CY_Genre_Content[21] = "Unknown";
	ObjectApp->CY_Genre_Content[22] = NULL;
	*/

	object_app->STR_Filter = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Filter",
		End;

	obj_aux1 = Label2("Filter:");

	obj_aux0 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux1,
		Child, object_app->STR_Filter,
		End;

	GR_grp_2 = GroupObject,
		MUIA_HelpNode, "GR_grp_2",
		MUIA_Group_Horiz, TRUE,
		Child, obj_aux0,
		End;

	//	  ObjectApp->LV_GamesList = NListObject,
	//		  MUIA_Frame, MUIV_Frame_InputList,
	//	  End;

	//	  ObjectApp->LV_GamesList = NListviewObject,
	//		  MUIA_HelpNode, "LV_GamesList",
	//		  MUIA_NList_DoubleClick, TRUE,
	//		  MUIA_NListview_Vert_ScrollBar, MUIV_NListview_VSB_Always,
	//		  MUIA_NListview_NList, ObjectApp->LV_GamesList,

	//	  End;

	object_app->LV_GamesList = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		End;

	object_app->LV_GamesList = ListviewObject,
		MUIA_HelpNode, "LV_GamesList",
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, object_app->LV_GamesList,
		End;

	if (!NOSCREENSHOT) {

		if (NOGUIGFX) {
			object_app->IM_GameImage_0 = MUI_NewObject("Dtpic.mui",
				MUIA_Dtpic_Name, "PROGDIR:igame.iff",
				MUIA_Frame, MUIV_Frame_ImageButton,
				End;
		}
		else {
			object_app->IM_GameImage_0 = GuigfxObject,
				MUIA_Guigfx_FileName, FILENAME_DEFAULT,
				MUIA_Guigfx_Quality, MUIV_Guigfx_Quality_Best,
				MUIA_Guigfx_ScaleMode, NISMF_SCALEFREE,
				MUIA_Frame, MUIV_Frame_ImageButton,
				MUIA_FixHeight, SS_HEIGHT,
				MUIA_FixWidth, SS_WIDTH,
				End;
		}
	}

	object_app->LV_GenresList = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		End;

	object_app->LV_GenresList = ListviewObject,
		MUIA_HelpNode, "LV_GenresList",
		MUIA_Listview_List, object_app->LV_GenresList,
		MUIA_MaxWidth, SS_WIDTH,	//keep the same width as if there was a screenshot area
		End;

	if (!NOSCREENSHOT) {

		object_app->GR_grp_1 = GroupObject,
			MUIA_HelpNode, "GR_grp_1",
			MUIA_Group_Rows, 2,
			Child, object_app->IM_GameImage_0,
			Child, object_app->LV_GenresList,
			End;
	}
	else {

		object_app->GR_grp_1 = GroupObject,
			MUIA_HelpNode, "GR_grp_1",
			MUIA_Group_Rows, 1,
			Child, object_app->LV_GenresList,
			End;

	}

	GR_grp_0 = GroupObject,
		MUIA_HelpNode, "GR_grp_0",
		MUIA_Group_Columns, 2,
		Child, object_app->LV_GamesList,
		Child, object_app->GR_grp_1,
		End;

	object_app->TX_Status = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, object_app->STR_TX_Status,
		End;

	GROUP_ROOT_0 = GroupObject,
		Child, GR_grp_2,
		Child, GR_grp_0,
		Child, object_app->TX_Status,
		End;

	MNlabel1ScanRepository = MenuitemObject,
		MUIA_Menuitem_Title, "Scan Repositories",
		MUIA_Menuitem_Shortcut, "S",
		End;

	MNlabel1Addslave = MenuitemObject,
		MUIA_Menuitem_Title, "Add non-whdload game...",
		End;

	MNlabel1ShowHidden = MenuitemObject,
		MUIA_Menuitem_Title, "Show/Hide hidden slaves",
		End;

	MNlabel1BarLabel1 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNlabel1About = MenuitemObject,
		MUIA_Menuitem_Title, "About...",
		End;

	MNlabel1BarLabel2 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNlabel1Quit = MenuitemObject,
		MUIA_Menuitem_Title, "Quit",
		MUIA_Menuitem_Shortcut, "Q",
		End;

	MNlabel1Actions = MenuitemObject,
		MUIA_Menuitem_Title, "Actions",
		MUIA_Family_Child, MNlabel1ScanRepository,
		MUIA_Family_Child, MNlabel1Addslave,
		MUIA_Family_Child, MNlabel1ShowHidden,
		MUIA_Family_Child, MNlabel1BarLabel1,
		MUIA_Family_Child, MNlabel1About,
		MUIA_Family_Child, MNlabel1BarLabel2,
		MUIA_Family_Child, MNlabel1Quit,
		End;

	MNlabel1Gamespath = MenuitemObject,
		MUIA_Menuitem_Title, "Games Repositories...",
		End;

	MNlabel1BarLabel3 = MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0);

	MNlabel1MUISettings = MenuitemObject,
		MUIA_Menuitem_Title, "MUI Settings...",
		End;

	//	  MNlabel1ShowHidden = MenuitemObject,
	//		  MUIA_Menuitem_Title, "Show hidden slaves",
	//	  End;

	MNlabel1Settings = MenuitemObject,
		MUIA_Menuitem_Title, "Settings",
		MUIA_Family_Child, MNlabel1Gamespath,
		//		  MUIA_Family_Child, MNlabel1ShowHidden,
		MUIA_Family_Child, MNlabel1BarLabel3,
		MUIA_Family_Child, MNlabel1MUISettings,
		End;

	MNlabel1GameProperties = MenuitemObject,
		MUIA_Menuitem_Title, "Game properties...",
		MUIA_Menuitem_Shortcut, "P",
		End;

	MNlabel1Game = MenuitemObject,
		MUIA_Menuitem_Title, "Game",
		MUIA_Family_Child, MNlabel1GameProperties,
		End;

	object_app->MN_MainMenu = MenustripObject,
		MUIA_Family_Child, MNlabel1Actions,
		MUIA_Family_Child, MNlabel1Settings,
		MUIA_Family_Child, MNlabel1Game,
		End;

	object_app->WI_MainWindow = WindowObject,
		MUIA_Window_Title, "iGame",
		MUIA_Window_Menustrip, object_app->MN_MainMenu,
		MUIA_Window_ID, MAKE_ID('0', 'I', 'G', 'A'),
		MUIA_Window_AppWindow, TRUE,
		WindowContents, GROUP_ROOT_0,
		End;

	object_app->LV_RepoList = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		End;

	object_app->LV_RepoList = ListviewObject,
		MUIA_HelpNode, "LV_RepoList",
		MUIA_Listview_List, object_app->LV_RepoList,
		End;

	object_app->STR_PA_RepoPath = String("", 80);

	object_app->PA_RepoPath = PopButton(MUII_PopDrawer);

	object_app->PA_RepoPath = PopaslObject,
		MUIA_HelpNode, "PA_RepoPath",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object_app->STR_PA_RepoPath,
		MUIA_Popstring_Button, object_app->PA_RepoPath,
		ASLFR_TitleText, "Select dir...",
		End;

	object_app->STR_PA_AddGame = String("", 80);

	object_app->PA_AddGamePath = PopButton(MUII_PopDrawer);

	object_app->PA_AddGamePath = PopaslObject,
		MUIA_HelpNode, "PA_AddGamePath",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object_app->STR_PA_AddGame,
		MUIA_Popstring_Button, object_app->PA_AddGamePath,
		ASLFR_TitleText, "Select game executable...",
		End;

	object_app->STR_AddGameTitle = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_FrameTitle, "Title",
		MUIA_HelpNode, "STR_AddGameTitle",
		End;

	GR_grp_6 = GroupObject,
		MUIA_HelpNode, "GR_grp_6",
		//MUIA_Group_Vert, TRUE,
		Child, object_app->STR_AddGameTitle,
		Child, object_app->PA_AddGamePath,
		End;

	object_app->BT_NonWhdloadOk = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Ok",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_NonWhdloadOk",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	object_app->BT_NonWhdloadCancel = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Cancel",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_NonWhdloadCancel",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	GR_grp_7 = GroupObject,
		MUIA_HelpNode, "GR_grp_7",
		MUIA_Group_Horiz, TRUE,
		Child, object_app->BT_NonWhdloadOk,
		Child, object_app->BT_NonWhdloadCancel,
		End;

	GROUP_ROOT_5 = GroupObject,
		Child, GR_grp_6,
		Child, GR_grp_7,
		End;

	object_app->WI_NonWhdload = WindowObject,
		MUIA_Window_Title, "Adding a non whdload game",
		MUIA_Window_ID, MAKE_ID('3', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_5,
		End;

	object_app->BT_RepoAdd = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Add",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_RepoAdd",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	object_app->BT_RepoRemove = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_RepoRemove",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	object_app->BT_RepoClose = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Close",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_RepoClose",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	GR_grp_4 = GroupObject,
		MUIA_HelpNode, "GR_grp_4",
		MUIA_Group_Horiz, TRUE,
		Child, object_app->PA_RepoPath,
		Child, object_app->BT_RepoAdd,
		End;

	GR_grp_5 = GroupObject,
		MUIA_HelpNode, "GR_grp_5",
		MUIA_Group_Horiz, TRUE,
		Child, object_app->BT_RepoRemove,
		Child, object_app->BT_RepoClose,
		End;

	GROUP_ROOT_1 = GroupObject,
		Child, GR_grp_4,
		Child, object_app->LV_RepoList,
		Child, GR_grp_5,
		End;

	object_app->WI_GameRepositories = WindowObject,
		MUIA_Window_Title, "Games repositories",
		MUIA_Window_ID, MAKE_ID('1', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_1,
		End;

	object_app->STR_GameTitle = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_FrameTitle, "Title",
		MUIA_HelpNode, "STR_GameTitle",
		End;

	object_app->CY_Genre = CycleObject,
		MUIA_HelpNode, "CY_Genre",
		MUIA_Cycle_Entries, object_app->CY_Genre_Content,
		End;

	/* For future tooltype editing */
	object_app->TX_Tooltypes = TextEditorObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		//MUIA_Text_SetVMax, FALSE,
		//MUIA_Text_SetMin, FALSE,
		MUIA_FrameTitle, "ToolTypes",
		End;

	object_app->TX_GamePath = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, object_app->STR_TX_Path,
		MUIA_Text_SetMin, TRUE,
		End;

	object_app->BT_GamePropertiesOK = SimpleButton("OK");

	object_app->BT_GamePropertiesCancel = SimpleButton("Cancel");

	object_app->CH_Favorite = CheckMark(FALSE);
	obj_aux2 = Label2("Favorite: ");

	object_app->CH_Hidden = CheckMark(FALSE);
	obj_aux4 = Label2("  Hidden: ");

	obj_aux3 = GroupObject,
		MUIA_Group_Columns, 4,
		Child, obj_aux2,
		Child, object_app->CH_Favorite,
		Child, obj_aux4,
		Child, object_app->CH_Hidden,
		End;

	object_app->TX_NumPlayed = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, object_app->STR_TX_NumPlayed,
		MUIA_Text_SetMin, TRUE,
		End;

	GR_grp_3 = GroupObject,
		MUIA_HelpNode, "GR_grp_3",
		MUIA_Group_Columns, 2,
		Child, object_app->BT_GamePropertiesOK,
		Child, object_app->BT_GamePropertiesCancel,
		End;

	GROUP_ROOT_2 = GroupObject,
		Child, object_app->STR_GameTitle,
		Child, object_app->CY_Genre,
		Child, obj_aux3,
		Child, object_app->TX_NumPlayed,
		Child, object_app->TX_GamePath,
		Child, GR_grp_3,
		End;

	GROUP_ROOT_4 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, GROUP_ROOT_2,
		Child, object_app->TX_Tooltypes,
		End;

	object_app->WI_GameProperties = WindowObject,
		MUIA_Window_Title, "Game Properties",
		MUIA_Window_ID, MAKE_ID('2', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_4,
		End;

	object_app->TX_label_1 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, object_app->STR_TX_About,
		MUIA_Text_SetMin, TRUE,
		End;

	GROUP_ROOT_3 = GroupObject,
		Child, object_app->TX_label_1,
		End;

	object_app->WI_About = WindowObject,
		MUIA_Window_Title, "About iGame",
		/*	  MUIA_Window_ID, MAKE_ID('4', 'I', 'G', 'A'), */
		WindowContents, GROUP_ROOT_3,
		End;

	/* ------ Wait window ------- */

	object_app->TX_label_2 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, object_app->STR_TX_Wait,
		MUIA_Text_SetMin, TRUE,
		End;

	GROUP_ROOT_6 = GroupObject,
		Child, object_app->TX_label_2,
		End;

	object_app->WI_Wait = WindowObject,
		/*	  MUIA_Window_Title, "iGame", */
		/* MUIA_Window_ID, MAKE_ID('7', 'I', 'G', 'A'), */
		WindowContents, GROUP_ROOT_6,
		MUIA_Window_RefWindow, object_app->WI_MainWindow,
		MUIA_Window_Borderless, TRUE,
		MUIA_Window_CloseGadget, FALSE,
		MUIA_Window_DepthGadget, FALSE,
		MUIA_Window_SizeGadget, FALSE,
		MUIA_Window_DragBar, FALSE,
		End;

	/* ------ End of Wait Window ------ */

	object_app->App = ApplicationObject,
		MUIA_Application_Author, "Emmanuel Vasilakis",
		MUIA_Application_Base, "iGame",
		MUIA_Application_Title, "iGame",
		MUIA_Application_Version, VERSION,
		MUIA_Application_Copyright, "2014",
		MUIA_Application_Description, "A frontend to WHDLoad",
		MUIA_Application_HelpFile, "igame.guide",
		SubWindow, object_app->WI_MainWindow,
		SubWindow, object_app->WI_GameRepositories,
		SubWindow, object_app->WI_GameProperties,
		SubWindow, object_app->WI_About,
		SubWindow, object_app->WI_NonWhdload,
		SubWindow, object_app->WI_Wait,
		End;

	if (!object_app->App)
	{
		FreeVec(object_app);
		return(NULL);
	}

	DoMethod(object_app->App,
		MUIM_Notify, MUIA_Application_Active, TRUE,
		object_app->App,
		2,
		MUIM_CallHook, &AppStartHook
	);

	DoMethod(MNlabel1ScanRepository,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object_app->App,
		2,
		MUIM_CallHook, &MenuScanHook
	);

	DoMethod(MNlabel1About,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object_app->WI_About,
		3,
		MUIM_Set, MUIA_Window_Open, TRUE
	);

	DoMethod(MNlabel1MUISettings,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object_app->App,
		3,
		MUIM_Application_OpenConfigWindow, 0
	);

	DoMethod(MNlabel1Quit,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object_app->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
	);

	DoMethod(MNlabel1Gamespath,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object_app->WI_GameRepositories,
		3,
		MUIM_Set, MUIA_Window_Open, TRUE
	);

	DoMethod(MNlabel1GameProperties,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object_app->App,
		2,
		MUIM_CallHook, &MenuGamePropertiesHook
	);

	DoMethod(MNlabel1ShowHidden,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object_app->App,
		2,
		MUIM_CallHook, &MenuShowHiddenHook
	);

	DoMethod(MNlabel1Addslave,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		object_app->App,
		2,
		MUIM_CallHook, &MenuAddNonWhdloadHook
	);

	DoMethod(object_app->WI_MainWindow,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object_app->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
	);
	//call whenever the string is changed
	if (FILTERUSEENTER == 0) {

		DoMethod(object_app->STR_Filter,
			MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
			object_app->STR_Filter,
			2,
			MUIM_CallHook, &FilterChangeHook
		);
	}
	else {
		//call only after return key is pressed
		DoMethod(object_app->STR_Filter,
			MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime,
			object_app->STR_Filter,
			2,
			MUIM_CallHook, &FilterChangeHook
		);
	}

	DoMethod(object_app->LV_GamesList,
		MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
		object_app->LV_GamesList,
		2,
		MUIM_CallHook, &GameDoubleClickHook
	);

	DoMethod(object_app->LV_GamesList,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		object_app->LV_GamesList,
		2,
		MUIM_CallHook, &GameClickHook
	);

	DoMethod(object_app->LV_GenresList,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		object_app->LV_GenresList,
		2,
		MUIM_CallHook, &GenresClickHook
	);

	if (!NOSCREENSHOT) {

		DoMethod(object_app->WI_MainWindow,
			MUIM_Window_SetCycleChain, object_app->STR_Filter,
			object_app->LV_GamesList,
			object_app->IM_GameImage_0,
			object_app->LV_GenresList,
			0
		);
	}
	else {

		DoMethod(object_app->WI_MainWindow,
			MUIM_Window_SetCycleChain, object_app->STR_Filter,
			object_app->LV_GamesList,
			object_app->LV_GenresList,
			0
		);
	}

	DoMethod(object_app->WI_GameRepositories,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object_app->WI_GameRepositories,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object_app->BT_NonWhdloadOk,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object_app->BT_NonWhdloadOk,
		2,
		MUIM_CallHook, &NonWhdloadOkHook
	);

	DoMethod(object_app->BT_NonWhdloadCancel,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object_app->WI_NonWhdload,
		2,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object_app->WI_NonWhdload,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object_app->WI_NonWhdload,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object_app->WI_GameRepositories,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object_app->WI_GameRepositories,
		2,
		MUIM_CallHook, &RepoStopHook
	);

	DoMethod(object_app->BT_RepoAdd,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object_app->BT_RepoAdd,
		2,
		MUIM_CallHook, &RepoAddHook
	);

	DoMethod(object_app->BT_GamePropertiesOK,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object_app->BT_GamePropertiesOK,
		2,
		MUIM_CallHook, &GamePropertiesOkHook
	);

	DoMethod(object_app->BT_RepoRemove,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object_app->BT_RepoRemove,
		2,
		MUIM_CallHook, &RepoRemoveHook
	);

	DoMethod(object_app->BT_GamePropertiesCancel,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object_app->WI_GameProperties,
		2,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object_app->BT_RepoClose,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object_app->BT_RepoClose,
		2,
		MUIM_CallHook, &RepoStopHook
	);

	DoMethod(object_app->BT_RepoClose,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object_app->WI_GameRepositories,
		2,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object_app->WI_GameRepositories,
		MUIM_Window_SetCycleChain, object_app->LV_RepoList,
		object_app->PA_RepoPath,
		object_app->BT_RepoAdd,
		object_app->BT_RepoRemove,
		0
	);

	DoMethod(object_app->WI_GameProperties,
		MUIM_Window_SetCycleChain, object_app->STR_GameTitle,
		object_app->CY_Genre,
		object_app->BT_GamePropertiesOK,
		object_app->BT_GamePropertiesCancel,
		0
	);

	DoMethod(object_app->WI_About,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object_app->WI_About,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object_app->WI_GameProperties,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object_app->WI_GameProperties,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object_app->WI_About,
		MUIM_Window_SetCycleChain, 0
	);

	return object_app;
}

void DisposeApp(struct ObjApp * object_app)
{
	if (object_app)
	{
		MUI_DisposeObject(object_app->App);
		FreeVec(object_app);
		object_app = NULL;
	}
}
