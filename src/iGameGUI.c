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

#include <proto/dos.h>
#include <proto/icon.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <proto/asl.h>
#include <proto/utility.h>
#include <dos/dos.h>
//#include <graphics/gfxmacros.h>
#include <workbench/workbench.h>
#include <clib/alib_protos.h>
#include <clib/debug_protos.h>
//#include <lib/mb_utils.h>
//#include <pragmas/dos_pragmas.h>
//#include <mui/guigfx_mcc.h>
#include "version.h"


#define MUIA_Dtpic_Name 0x80423d72

extern int SS_WIDTH, SS_HEIGHT;
extern int NOGUIGFX;
extern int FILTERUSEENTER;
extern int NOSCREENSHOT;

/* Prototypes */
#include <proto/muimaster.h>
#include <proto/exec.h>
#ifdef __GNUC__
#else
#include <clib/alib_protos.h>
#endif /* __GNUC__ */
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

/*
static APTR CreatePictureObject( STRPTR Name,
											LONG Quality,
											BOOL ScaleUp,
											BOOL ScaleDown,
											BOOL PicAspect,
											BOOL ScreenAspect,
											BOOL TransMask,
											BOOL TransColor,
											LONG TransRGB)
{
	return GuigfxObject,
				MUIA_Guigfx_FileName, Name,
				MUIA_Guigfx_Quality, Quality,
				MUIA_Guigfx_ScaleMode,SCALEMODEMASK(ScaleUp,ScaleDown,PicAspect,ScreenAspect),
				MUIA_Guigfx_Transparency,(TransMask ? 1:0) |
													(TransColor ? 1 : 0),
			End;
}
*/
#include "iGameGUI.h"
#include "iGameExtern.h"
#include "Hook_utility.h"
struct ObjApp * CreateApp(void)
{
	struct ObjApp * ObjectApp;

	APTR	MNlabel1Actions, MNlabel1ScanRepository, MNlabel1Addslave, MNlabel1BarLabel1;
	APTR	MNlabel1About, MNlabel1BarLabel2, MNlabel1BarLabel3, MNlabel1MUISettings, MNlabel1Quit, MNlabel1Settings, MNlabel1ShowHidden;
	APTR	MNlabel1Gamespath, MNlabel1Game, MNlabel1GameProperties, GROUP_ROOT_0, IM_Pic;
	APTR	GR_grp_Img, GR_grp_5, GR_grp_2, obj_aux0, obj_aux1, obj_aux2, obj_aux3, obj_aux4, GR_grp_0, GROUP_ROOT_1, GR_grp_4, GR_grp_6, GR_grp_7;
	APTR	GROUP_ROOT_2, GR_grp_3, GROUP_ROOT_3, GROUP_ROOT_4, GROUP_ROOT_5, GROUP_ROOT_6;
	static struct Hook FilterChangeHook;
	static struct Hook GameDoubleClickHook;
	static struct Hook MenuScanHook;
	static struct Hook AppStartHook;
	static struct Hook RepoAddHook;
	static struct Hook RepoRemoveHook;
	static struct Hook RepoStartHook;
	static struct Hook RepoStopHook;
	static struct Hook GameClickHook;
	static struct Hook MenuGamePropertiesHook;
	static struct Hook GamePropertiesOkHook;
	static struct Hook GenresClickHook;
	static struct Hook MenuAddNonWhdloadHook;
	static struct Hook NonWhdloadOkHook;
	static struct Hook MenuMUISettingsHook;
	static struct Hook MenuShowHiddenHook;

	if (!(ObjectApp = AllocVec(sizeof(struct ObjApp), MEMF_CLEAR)))
		return(NULL);

	ObjectApp->STR_TX_Status_0 = NULL;
	ObjectApp->STR_TX_Path_2 = NULL;
	ObjectApp->STR_TX_label_1 = "iGame version 1.7 beta\n\n(c) 2005-2018 Emmanuel Vasilakis\nmrzammler@gmail.com";
	ObjectApp->STR_TX_label_2 = "Writing to disk, please wait...";

	ObjectApp->CY_Genre_0Content[0] = "Unknown";
	ObjectApp->CY_Genre_0Content[1] = NULL;
	/*
	ObjectApp->CY_Genre_0Content[1] = "Adult";
	ObjectApp->CY_Genre_0Content[2] = "Adventure";
	ObjectApp->CY_Genre_0Content[3] = "Bat and ball";
	ObjectApp->CY_Genre_0Content[4] = "Beat 'em up";
	ObjectApp->CY_Genre_0Content[5] = "Board";
	ObjectApp->CY_Genre_0Content[6] = "Cards";
	ObjectApp->CY_Genre_0Content[7] = "Demo";
	ObjectApp->CY_Genre_0Content[8] = "Gambling";
	ObjectApp->CY_Genre_0Content[9] = "Maze";
	ObjectApp->CY_Genre_0Content[10] = "Misc";
	ObjectApp->CY_Genre_0Content[11] = "Pinball";
	ObjectApp->CY_Genre_0Content[12] = "Platform";
	ObjectApp->CY_Genre_0Content[13] = "Puzzle";
	ObjectApp->CY_Genre_0Content[14] = "Quiz";
	ObjectApp->CY_Genre_0Content[15] = "Racing";
	ObjectApp->CY_Genre_0Content[16] = "RPG";
	ObjectApp->CY_Genre_0Content[17] = "Shoot 'em up";
	ObjectApp->CY_Genre_0Content[18] = "Simulation";
	ObjectApp->CY_Genre_0Content[19] = "Sports";
	ObjectApp->CY_Genre_0Content[20] = "Strategy";
	ObjectApp->CY_Genre_0Content[21] = "Unknown";
	ObjectApp->CY_Genre_0Content[22] = NULL;
	*/

	InstallHook(&FilterChangeHook, FilterChange, ObjectApp);
	InstallHook(&GameDoubleClickHook, GameDoubleClick, ObjectApp);
	InstallHook(&MenuScanHook, MenuScan, ObjectApp);
	InstallHook(&AppStartHook, AppStart, ObjectApp);
	InstallHook(&RepoAddHook, RepoAdd, ObjectApp);
	InstallHook(&RepoRemoveHook, RepoRemove, ObjectApp);
	InstallHook(&RepoStartHook, RepoStart, ObjectApp);
	InstallHook(&RepoStopHook, RepoStop, ObjectApp);
	InstallHook(&GameClickHook, GameClick, ObjectApp);
	InstallHook(&MenuGamePropertiesHook, MenuGameProperties, ObjectApp);
	InstallHook(&GamePropertiesOkHook, GamePropertiesOk, ObjectApp);
	InstallHook(&GenresClickHook, GenresClick, ObjectApp);
	InstallHook(&MenuAddNonWhdloadHook, MenuAddNonWhdload, ObjectApp);
	InstallHook(&NonWhdloadOkHook, NonWhdloadOk, ObjectApp);
	InstallHook(&MenuShowHiddenHook, MenuShowHidden, ObjectApp);

	ObjectApp->STR_Filter_0 = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Filter_0",
		End;

	obj_aux1 = Label2("Filter:");

	obj_aux0 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux1,
		Child, ObjectApp->STR_Filter_0,
		End;

	GR_grp_2 = GroupObject,
		MUIA_HelpNode, "GR_grp_2",
		MUIA_Group_Horiz, TRUE,
		Child, obj_aux0,
		End;

	//	  ObjectApp->LV_GamesList_0 = NListObject,
	//		  MUIA_Frame, MUIV_Frame_InputList,
	//	  End;

	//	  ObjectApp->LV_GamesList_0 = NListviewObject,
	//		  MUIA_HelpNode, "LV_GamesList_0",
	//		  MUIA_NList_DoubleClick, TRUE,
	//		  MUIA_NListview_Vert_ScrollBar, MUIV_NListview_VSB_Always,
	//		  MUIA_NListview_NList, ObjectApp->LV_GamesList_0,

	//	  End;

	ObjectApp->LV_GamesList_0 = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		End;

	ObjectApp->LV_GamesList_0 = ListviewObject,
		MUIA_HelpNode, "LV_GamesList_0",
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, ObjectApp->LV_GamesList_0,
		End;

	if (!NOSCREENSHOT) {

		if (NOGUIGFX) {
			ObjectApp->IM_GameImage_0 = MUI_NewObject("Dtpic.mui",
				MUIA_Dtpic_Name, "PROGDIR:igame.iff",
				MUIA_Frame, MUIV_Frame_ImageButton,
				End;
		}
		else {
			ObjectApp->IM_GameImage_0 = GuigfxObject,
				MUIA_Guigfx_FileName, FILENAME_DEFAULT,
				MUIA_Guigfx_Quality, MUIV_Guigfx_Quality_Best,
				MUIA_Guigfx_ScaleMode, NISMF_SCALEFREE,
				MUIA_Frame, MUIV_Frame_ImageButton,
				MUIA_FixHeight, SS_HEIGHT,
				MUIA_FixWidth, SS_WIDTH,
				End;
		}
	}

	ObjectApp->LV_GenresList_1 = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		End;

	ObjectApp->LV_GenresList_1 = ListviewObject,
		MUIA_HelpNode, "LV_GenresList_1",
		MUIA_Listview_List, ObjectApp->LV_GenresList_1,
		MUIA_MaxWidth, SS_WIDTH,	//keep the same width as if there was a screenshot area
		End;

	if (!NOSCREENSHOT) {

		ObjectApp->GR_grp_1 = GroupObject,
			MUIA_HelpNode, "GR_grp_1",
			MUIA_Group_Rows, 2,
			Child, ObjectApp->IM_GameImage_0,
			Child, ObjectApp->LV_GenresList_1,
			End;
	}
	else {

		ObjectApp->GR_grp_1 = GroupObject,
			MUIA_HelpNode, "GR_grp_1",
			MUIA_Group_Rows, 1,
			Child, ObjectApp->LV_GenresList_1,
			End;

	}

	GR_grp_0 = GroupObject,
		MUIA_HelpNode, "GR_grp_0",
		MUIA_Group_Columns, 2,
		Child, ObjectApp->LV_GamesList_0,
		Child, ObjectApp->GR_grp_1,
		End;

	ObjectApp->TX_Status_0 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, ObjectApp->STR_TX_Status_0,
		End;

	GROUP_ROOT_0 = GroupObject,
		Child, GR_grp_2,
		Child, GR_grp_0,
		Child, ObjectApp->TX_Status_0,
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

	ObjectApp->MN_label_1 = MenustripObject,
		MUIA_Family_Child, MNlabel1Actions,
		MUIA_Family_Child, MNlabel1Settings,
		MUIA_Family_Child, MNlabel1Game,
		End;

	ObjectApp->WI_Main_0 = WindowObject,
		MUIA_Window_Title, "iGame",
		MUIA_Window_Menustrip, ObjectApp->MN_label_1,
		MUIA_Window_ID, MAKE_ID('0', 'I', 'G', 'A'),
		MUIA_Window_AppWindow, TRUE,
		WindowContents, GROUP_ROOT_0,
		End;

	ObjectApp->LV_RepoList_3 = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		End;

	ObjectApp->LV_RepoList_3 = ListviewObject,
		MUIA_HelpNode, "LV_RepoList_3",
		MUIA_Listview_List, ObjectApp->LV_RepoList_3,
		End;

	ObjectApp->STR_PA_label_0 = String("", 80);

	ObjectApp->PA_label_0 = PopButton(MUII_PopDrawer);

	ObjectApp->PA_label_0 = PopaslObject,
		MUIA_HelpNode, "PA_label_0",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, ObjectApp->STR_PA_label_0,
		MUIA_Popstring_Button, ObjectApp->PA_label_0,
		ASLFR_TitleText, "Select dir...",
		End;

	ObjectApp->STR_PA_label_1 = String("", 80);

	ObjectApp->PA_label_1 = PopButton(MUII_PopDrawer);

	ObjectApp->PA_label_1 = PopaslObject,
		MUIA_HelpNode, "PA_label_1",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, ObjectApp->STR_PA_label_1,
		MUIA_Popstring_Button, ObjectApp->PA_label_1,
		ASLFR_TitleText, "Select game executable...",
		End;

	ObjectApp->STR_Title_2 = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_FrameTitle, "Title",
		MUIA_HelpNode, "STR_Title_2",
		End;

	GR_grp_6 = GroupObject,
		MUIA_HelpNode, "GR_grp_6",
		//MUIA_Group_Vert, TRUE,
		Child, ObjectApp->STR_Title_2,
		Child, ObjectApp->PA_label_1,
		End;

	ObjectApp->BT_NonWhdloadOk_0 = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Ok",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_NonWhdloadOk_0",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	ObjectApp->BT_NonWhdloadCancel_0 = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Cancel",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_NonWhdloadCancel_0",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	GR_grp_7 = GroupObject,
		MUIA_HelpNode, "GR_grp_7",
		MUIA_Group_Horiz, TRUE,
		Child, ObjectApp->BT_NonWhdloadOk_0,
		Child, ObjectApp->BT_NonWhdloadCancel_0,
		End;

	GROUP_ROOT_5 = GroupObject,
		Child, GR_grp_6,
		Child, GR_grp_7,
		End;

	ObjectApp->WI_NonWhdload = WindowObject,
		MUIA_Window_Title, "Adding a non whdload game",
		MUIA_Window_ID, MAKE_ID('3', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_5,
		End;

	ObjectApp->BT_RepoAdd_2 = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Add",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_RepoAdd_2",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	ObjectApp->BT_RepoRemove_3 = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Remove",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_RepoRemove_3",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	ObjectApp->BT_RepoClose_1 = TextObject,
		ButtonFrame,
		MUIA_Weight, 50,
		MUIA_Background, MUII_ButtonBack,
		MUIA_Text_Contents, "Close",
		MUIA_Text_PreParse, "\033c",
		MUIA_HelpNode, "BT_RepoClose_1",
		MUIA_InputMode, MUIV_InputMode_RelVerify,
		End;

	GR_grp_4 = GroupObject,
		MUIA_HelpNode, "GR_grp_4",
		MUIA_Group_Horiz, TRUE,
		Child, ObjectApp->PA_label_0,
		Child, ObjectApp->BT_RepoAdd_2,
		End;

	GR_grp_5 = GroupObject,
		MUIA_HelpNode, "GR_grp_5",
		MUIA_Group_Horiz, TRUE,
		Child, ObjectApp->BT_RepoRemove_3,
		Child, ObjectApp->BT_RepoClose_1,
		End;

	GROUP_ROOT_1 = GroupObject,
		Child, GR_grp_4,
		Child, ObjectApp->LV_RepoList_3,
		Child, GR_grp_5,
		End;

	ObjectApp->WI_GamesRepo_1 = WindowObject,
		MUIA_Window_Title, "Games repositories",
		MUIA_Window_ID, MAKE_ID('1', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_1,
		End;

	ObjectApp->STR_Title_1 = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_FrameTitle, "Title",
		MUIA_HelpNode, "STR_Title_1",
		End;

	ObjectApp->CY_Genre_0 = CycleObject,
		MUIA_HelpNode, "CY_Genre_0",
		MUIA_Cycle_Entries, ObjectApp->CY_Genre_0Content,
		End;

	/* For future tooltype editing */
	ObjectApp->TX_Tooltypes_0 = TextEditorObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		//MUIA_Text_SetVMax, FALSE,
		//MUIA_Text_SetMin, FALSE,
		MUIA_FrameTitle, "ToolTypes",
		End;

	ObjectApp->TX_Path_2 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, ObjectApp->STR_TX_Path_2,
		MUIA_Text_SetMin, TRUE,
		End;

	ObjectApp->BT_GPOK_0 = SimpleButton("OK");

	ObjectApp->BT_GPCancel_1 = SimpleButton("Cancel");

	ObjectApp->CH_label_0 = CheckMark(FALSE);
	obj_aux2 = Label2("Favorite: ");

	ObjectApp->CH_label_1 = CheckMark(FALSE);
	obj_aux4 = Label2("  Hidden: ");

	obj_aux3 = GroupObject,
		MUIA_Group_Columns, 4,
		Child, obj_aux2,
		Child, ObjectApp->CH_label_0,
		Child, obj_aux4,
		Child, ObjectApp->CH_label_1,
		End;

	ObjectApp->TX_NumPlayed_0 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, ObjectApp->STR_TX_NumPlayed_0,
		MUIA_Text_SetMin, TRUE,
		End;

	GR_grp_3 = GroupObject,
		MUIA_HelpNode, "GR_grp_3",
		MUIA_Group_Columns, 2,
		Child, ObjectApp->BT_GPOK_0,
		Child, ObjectApp->BT_GPCancel_1,
		End;

	GROUP_ROOT_2 = GroupObject,
		Child, ObjectApp->STR_Title_1,
		Child, ObjectApp->CY_Genre_0,
		Child, obj_aux3,
		Child, ObjectApp->TX_NumPlayed_0,
		Child, ObjectApp->TX_Path_2,
		Child, GR_grp_3,
		End;

	GROUP_ROOT_4 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, GROUP_ROOT_2,
		Child, ObjectApp->TX_Tooltypes_0,
		End;

	ObjectApp->WI_GameProperties_2 = WindowObject,
		MUIA_Window_Title, "Game Properties",
		MUIA_Window_ID, MAKE_ID('2', 'I', 'G', 'A'),
		WindowContents, GROUP_ROOT_4,
		End;

	ObjectApp->TX_label_1 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, ObjectApp->STR_TX_label_1,
		MUIA_Text_SetMin, TRUE,
		End;

	GROUP_ROOT_3 = GroupObject,
		Child, ObjectApp->TX_label_1,
		End;

	ObjectApp->WI_About_3 = WindowObject,
		MUIA_Window_Title, "About iGame",
		/*	  MUIA_Window_ID, MAKE_ID('4', 'I', 'G', 'A'), */
		WindowContents, GROUP_ROOT_3,
		End;

	/* ------ Wait window ------- */

	ObjectApp->TX_label_2 = TextObject,
		MUIA_Background, MUII_TextBack,
		MUIA_Frame, MUIV_Frame_Text,
		MUIA_Text_Contents, ObjectApp->STR_TX_label_2,
		MUIA_Text_SetMin, TRUE,
		End;

	GROUP_ROOT_6 = GroupObject,
		Child, ObjectApp->TX_label_2,
		End;

	ObjectApp->WI_Wait_0 = WindowObject,
		/*	  MUIA_Window_Title, "iGame", */
		/* MUIA_Window_ID, MAKE_ID('7', 'I', 'G', 'A'), */
		WindowContents, GROUP_ROOT_6,
		MUIA_Window_RefWindow, ObjectApp->WI_Main_0,
		MUIA_Window_Borderless, TRUE,
		MUIA_Window_CloseGadget, FALSE,
		MUIA_Window_DepthGadget, FALSE,
		MUIA_Window_SizeGadget, FALSE,
		MUIA_Window_DragBar, FALSE,
		End;

	/* ------ End of Wait Window ------ */

	ObjectApp->App = ApplicationObject,
		MUIA_Application_Author, "Emmanuel Vasilakis",
		MUIA_Application_Base, "IGAM",
		MUIA_Application_Title, "iGame",
		MUIA_Application_Version, VERSION,
		MUIA_Application_Copyright, "2014",
		MUIA_Application_Description, "A frontend to WHDLoad",
		MUIA_Application_HelpFile, "igame.guide",
		SubWindow, ObjectApp->WI_Main_0,
		SubWindow, ObjectApp->WI_GamesRepo_1,
		SubWindow, ObjectApp->WI_GameProperties_2,
		SubWindow, ObjectApp->WI_About_3,
		SubWindow, ObjectApp->WI_NonWhdload,
		SubWindow, ObjectApp->WI_Wait_0,
		End;

	if (!ObjectApp->App)
	{
		FreeVec(ObjectApp);
		return(NULL);
	}

	DoMethod(ObjectApp->App,
		MUIM_Notify, MUIA_Application_Active, TRUE,
		ObjectApp->App,
		2,
		MUIM_CallHook, &AppStartHook
	);

	DoMethod(MNlabel1ScanRepository,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		ObjectApp->App,
		2,
		MUIM_CallHook, &MenuScanHook
	);

	DoMethod(MNlabel1About,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		ObjectApp->WI_About_3,
		3,
		MUIM_Set, MUIA_Window_Open, TRUE
	);

	DoMethod(MNlabel1MUISettings,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		ObjectApp->App,
		3,
		MUIM_Application_OpenConfigWindow, 0
	);

	DoMethod(MNlabel1Quit,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		ObjectApp->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
	);

	DoMethod(MNlabel1Gamespath,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		ObjectApp->WI_GamesRepo_1,
		3,
		MUIM_Set, MUIA_Window_Open, TRUE
	);

	DoMethod(MNlabel1GameProperties,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		ObjectApp->App,
		2,
		MUIM_CallHook, &MenuGamePropertiesHook
	);

	DoMethod(MNlabel1ShowHidden,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		ObjectApp->App,
		2,
		MUIM_CallHook, &MenuShowHiddenHook
	);

	DoMethod(MNlabel1Addslave,
		MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
		ObjectApp->App,
		2,
		MUIM_CallHook, &MenuAddNonWhdloadHook
	);

	DoMethod(ObjectApp->WI_Main_0,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		ObjectApp->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
	);
	//call whenever the string is changed
	if (FILTERUSEENTER == 0) {

		DoMethod(ObjectApp->STR_Filter_0,
			MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
			ObjectApp->STR_Filter_0,
			2,
			MUIM_CallHook, &FilterChangeHook
		);
	}
	else {
		//call only after return key is pressed
		DoMethod(ObjectApp->STR_Filter_0,
			MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime,
			ObjectApp->STR_Filter_0,
			2,
			MUIM_CallHook, &FilterChangeHook
		);
	}

	DoMethod(ObjectApp->LV_GamesList_0,
		MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
		ObjectApp->LV_GamesList_0,
		2,
		MUIM_CallHook, &GameDoubleClickHook
	);

	DoMethod(ObjectApp->LV_GamesList_0,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		ObjectApp->LV_GamesList_0,
		2,
		MUIM_CallHook, &GameClickHook
	);

	DoMethod(ObjectApp->LV_GenresList_1,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		ObjectApp->LV_GenresList_1,
		2,
		MUIM_CallHook, &GenresClickHook
	);

	if (!NOSCREENSHOT) {

		DoMethod(ObjectApp->WI_Main_0,
			MUIM_Window_SetCycleChain, ObjectApp->STR_Filter_0,
			ObjectApp->LV_GamesList_0,
			ObjectApp->IM_GameImage_0,
			ObjectApp->LV_GenresList_1,
			0
		);
	}
	else {

		DoMethod(ObjectApp->WI_Main_0,
			MUIM_Window_SetCycleChain, ObjectApp->STR_Filter_0,
			ObjectApp->LV_GamesList_0,
			ObjectApp->LV_GenresList_1,
			0
		);
	}

	DoMethod(ObjectApp->WI_GamesRepo_1,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		ObjectApp->WI_GamesRepo_1,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(ObjectApp->BT_NonWhdloadOk_0,
		MUIM_Notify, MUIA_Pressed, FALSE,
		ObjectApp->BT_NonWhdloadOk_0,
		2,
		MUIM_CallHook, &NonWhdloadOkHook
	);

	DoMethod(ObjectApp->BT_NonWhdloadCancel_0,
		MUIM_Notify, MUIA_Pressed, FALSE,
		ObjectApp->WI_NonWhdload,
		2,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(ObjectApp->WI_NonWhdload,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		ObjectApp->WI_NonWhdload,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(ObjectApp->WI_GamesRepo_1,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		ObjectApp->WI_GamesRepo_1,
		2,
		MUIM_CallHook, &RepoStopHook
	);

	DoMethod(ObjectApp->LV_RepoList_3,
		MUIM_Notify, MUIA_ShowMe, TRUE,
		ObjectApp->LV_RepoList_3,
		2,
		MUIM_CallHook, &RepoStartHook
	);

	DoMethod(ObjectApp->LV_RepoList_3,
		MUIM_Notify, MUIA_Disabled, FALSE,
		ObjectApp->LV_RepoList_3,
		2,
		MUIM_CallHook, &RepoStartHook
	);

	DoMethod(ObjectApp->BT_RepoAdd_2,
		MUIM_Notify, MUIA_Pressed, FALSE,
		ObjectApp->BT_RepoAdd_2,
		2,
		MUIM_CallHook, &RepoAddHook
	);

	DoMethod(ObjectApp->BT_GPOK_0,
		MUIM_Notify, MUIA_Pressed, FALSE,
		ObjectApp->BT_GPOK_0,
		2,
		MUIM_CallHook, &GamePropertiesOkHook
	);

	DoMethod(ObjectApp->BT_RepoRemove_3,
		MUIM_Notify, MUIA_Pressed, FALSE,
		ObjectApp->BT_RepoRemove_3,
		2,
		MUIM_CallHook, &RepoRemoveHook
	);

	DoMethod(ObjectApp->BT_GPCancel_1,
		MUIM_Notify, MUIA_Pressed, FALSE,
		ObjectApp->WI_GameProperties_2,
		2,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(ObjectApp->BT_RepoClose_1,
		MUIM_Notify, MUIA_Pressed, FALSE,
		ObjectApp->BT_RepoClose_1,
		2,
		MUIM_CallHook, &RepoStopHook
	);

	DoMethod(ObjectApp->BT_RepoClose_1,
		MUIM_Notify, MUIA_Pressed, FALSE,
		ObjectApp->WI_GamesRepo_1,
		2,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(ObjectApp->WI_GamesRepo_1,
		MUIM_Window_SetCycleChain, ObjectApp->LV_RepoList_3,
		ObjectApp->PA_label_0,
		ObjectApp->BT_RepoAdd_2,
		ObjectApp->BT_RepoRemove_3,
		0
	);

	DoMethod(ObjectApp->WI_GameProperties_2,
		MUIM_Window_SetCycleChain, ObjectApp->STR_Title_1,
		ObjectApp->CY_Genre_0,
		ObjectApp->BT_GPOK_0,
		ObjectApp->BT_GPCancel_1,
		0
	);

	DoMethod(ObjectApp->WI_About_3,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		ObjectApp->WI_About_3,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(ObjectApp->WI_GameProperties_2,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		ObjectApp->WI_GameProperties_2,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(ObjectApp->WI_About_3,
		MUIM_Window_SetCycleChain, 0
	);

	//set(ObjectApp->WI_Main_0,
	//	  MUIA_Window_Open, TRUE
	//	  );

	return(ObjectApp);
}

void DisposeApp(struct ObjApp * ObjectApp)
{
	if (ObjectApp)
	{
		MUI_DisposeObject(ObjectApp->App);
		FreeVec(ObjectApp);
		ObjectApp = NULL;
	}
}
