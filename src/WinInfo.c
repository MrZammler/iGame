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
#include <mui/Urltext_mcc.h>

#ifdef __morphos__
#define SDI_TRAP_LIB
#endif
#include <SDI_hook.h>

/* Prototypes */
#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <string.h>

#define iGame_NUMBERS
#include "iGame_strings.h"

#include "iGameExtern.h"
#include "funcs.h"
#include "strfuncs.h"
#include "iGameGUI.h"
#include "WinInfo.h"

extern igame_settings *current_settings;

HOOKPROTONH(StrObjFunc, int, Object *popObject, Object *strObject)
{
	char *listEntry, *currentGenre;
	get(strObject, MUIA_String_Contents, &currentGenre);

	for (int i=0;;i++)
	{
		DoMethod(popObject, MUIM_List_GetEntry, i, &listEntry);
		if (!listEntry)
		{
			set(popObject, MUIA_List_Active, MUIV_List_Active_Off);
			break;
		}
		else if (!strcmp(listEntry, currentGenre))
		{
			set(popObject, MUIA_List_Active, i);
			DoMethod(popObject, MUIM_List_Jump, MUIV_List_Jump_Active);
			break;
		}
	}
	return(TRUE);
}
MakeStaticHook(StrObjHook, StrObjFunc);

HOOKPROTONH(ObjStrFunc, int, Object *popObject, Object *strObject)
{
	char *selectedGenre;
	DoMethod(popObject, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &selectedGenre);
	set(strObject, MUIA_String_Contents, selectedGenre);
}
MakeStaticHook(ObjStrHook, ObjStrFunc);

APTR getInformationWindow(struct ObjApp *object)
{
	object->GR_WI_Information_Links = HSpace(0);
	if (current_settings->show_url_links)
	{
		object->GR_WI_Information_Links = GroupObject,
			MUIA_Weight, 100,
			MUIA_Frame, MUIV_Frame_Group,
			MUIA_FrameTitle, GetMBString(MSG_GR_LinksTitle),
			MUIA_ShowMe, FALSE,
			Child, object->URL_WI_Information_Lemonamiga = UrltextObject,
				MUIA_Font, MUIV_Font_Normal,
				MUIA_Urltext_Text, "Lemon Amiga",
				MUIA_Urltext_Url, DEFAULT_LOCALHOST_URL,
				MUIA_Urltext_SetMax, FALSE,
				MUIA_ShowMe, FALSE,
			End,
			Child, object->URL_WI_Information_HOL = UrltextObject,
				MUIA_Font, MUIV_Font_Normal,
				MUIA_Urltext_Text, "Hall Of Light",
				MUIA_Urltext_Url, DEFAULT_LOCALHOST_URL,
				MUIA_Urltext_SetMax, FALSE,
				MUIA_ShowMe, FALSE,
			End,
			Child, object->URL_WI_Information_Pouet = UrltextObject,
				MUIA_Font, MUIV_Font_Normal,
				MUIA_Urltext_Text, "Pouet",
				MUIA_Urltext_Url, DEFAULT_LOCALHOST_URL,
				MUIA_Urltext_SetMax, FALSE,
				MUIA_ShowMe, FALSE,
			End,
		End;
	}

	return WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_Information),
		MUIA_Window_ID, MAKE_ID('6', 'I', 'G', 'A'),
		MUIA_HelpNode, "WININFO",
		WindowContents, VGroup,
			Child, TextObject,
				MUIA_Weight, 30,
				MUIA_Text_PreParse, "\0338",
				MUIA_Text_Contents, GetMBString(MSG_STR_PropertiesGameTitleTitle),
				MUIA_InnerLeft, 0,
				MUIA_InnerRight, 0,
			End,
			Child, object->STR_WI_Information_Title = StringObject,
				MUIA_Frame, MUIV_Frame_String,
			End,
			Child, TextObject,
				MUIA_Weight, 30,
				MUIA_Text_PreParse, "\0338",
				MUIA_Text_Contents, GetMBString(MSG_LA_PropertiesGenre),
				MUIA_InnerLeft, 0,
				MUIA_InnerRight, 0,
			End,
			Child, GroupObject,
				Child, object->POP_WI_Information_Genre = PopobjectObject,
					MUIA_Popstring_String, object->STR_WI_Information_Genre = KeyString(0, MAX_GENRE_NAME_SIZE, 'n'),
					MUIA_Popstring_Button, PopButton(MUII_PopUp),
					MUIA_Popobject_StrObjHook, &StrObjHook,
					MUIA_Popobject_ObjStrHook, &ObjStrHook,
					MUIA_Popobject_Object, object->LV_WI_Information_Genre = ListviewObject,
						MUIA_Listview_List, ListObject,
							InputListFrame,
							MUIA_List_AutoVisible, TRUE,
							End,
						End,
				End,
			End,
			Child, ColGroup(2),
				Child, ColGroup(2),
					MUIA_Weight, 70,
					Child, TextObject,
						MUIA_Weight, 30,
						MUIA_Text_PreParse, "\0338",
						MUIA_Text_Contents, GetMBString(MSG_LA_ReleaseYear),
						MUIA_InnerLeft, 0,
						MUIA_InnerRight, 0,
					End,
					Child, object->TX_WI_Information_Year = TextObject,
						MUIA_Weight, 70,
						MUIA_Background, MUII_WindowBack,
						MUIA_Text_SetMin, TRUE,
					End,
					Child, TextObject,
						MUIA_Weight, 30,
						MUIA_Text_PreParse, "\0338",
						MUIA_Text_Contents, GetMBString(MSG_LA_ReleasedBy),
						MUIA_InnerLeft, 0,
						MUIA_InnerRight, 0,
					End,
					Child, object->TX_WI_Information_ReleasedBy = TextObject,
						MUIA_Weight, 70,
						MUIA_Background, MUII_WindowBack,
						MUIA_Text_SetMin, TRUE,
					End,
					Child, TextObject,
						MUIA_Weight, 30,
						MUIA_Text_PreParse, "\0338",
						MUIA_Text_Contents, GetMBString(MSG_LA_Chipset),
						MUIA_InnerLeft, 0,
						MUIA_InnerRight, 0,
					End,
					Child, object->TX_WI_Information_Chipset = TextObject,
						MUIA_Weight, 70,
						MUIA_Background, MUII_WindowBack,
						MUIA_Text_SetMin, TRUE,
					End,
					Child, TextObject,
						MUIA_Weight, 30,
						MUIA_Text_PreParse, "\0338",
						MUIA_Text_Contents, GetMBString(MSG_LA_NoOfPlayers),
						MUIA_InnerLeft, 0,
						MUIA_InnerRight, 0,
					End,
					Child, object->TX_WI_Information_Players = TextObject,
						MUIA_Weight, 70,
						MUIA_Background, MUII_WindowBack,
						MUIA_Text_SetMin, TRUE,
					End,
					Child, TextObject,
						MUIA_Weight, 30,
						MUIA_Text_PreParse, "\0338",
						MUIA_Text_Contents, GetMBString(MSG_LA_TimesPlayed),
						MUIA_InnerLeft, 0,
						MUIA_InnerRight, 0,
					End,
					Child, object->TX_WI_Information_TimesPlayed = TextObject,
						MUIA_Weight, 70,
						MUIA_Background, MUII_WindowBack,
						MUIA_Text_SetMin, TRUE,
					End,
				End,
				Child, ColGroup(2),
					MUIA_Weight, 30,
					Child, object->CH_WI_Information_Favourite = ImageObject,
						ImageButtonFrame,\
						MUIA_Weight, 50,
						MUIA_InputMode, MUIV_InputMode_Toggle,
						MUIA_Image_Spec, MUII_CheckMark,
						MUIA_Image_FreeVert, TRUE,
						MUIA_ShowSelState, FALSE,
						MUIA_Background, MUII_ButtonBack,
					End,
					Child, TextObject,
						MUIA_Text_Contents, GetMBString(MSG_LA_Favourite),
						MUIA_Weight, 50,
						MUIA_InnerLeft, 0,
						MUIA_InnerRight, 0,
					End,
					Child, object->CH_WI_Information_Hidden = ImageObject,
						ImageButtonFrame,\
						MUIA_Weight, 50,
						MUIA_InputMode, MUIV_InputMode_Toggle,
						MUIA_Image_Spec, MUII_CheckMark,
						MUIA_Image_FreeVert, TRUE,
						MUIA_ShowSelState, FALSE,
						MUIA_Background, MUII_ButtonBack,
					End,
					Child, TextObject,
						MUIA_Text_Contents, GetMBString(MSG_LA_Hidden),
						MUIA_Weight, 50,
						MUIA_InnerLeft, 0,
						MUIA_InnerRight, 0,
					End,
					Child, HVSpace,
					Child, HVSpace,
				End,
			End,
			Child, object->GR_WI_Information_Links,
			Child, MUI_MakeObject(MUIO_HBar, 4),
			Child, HGroup, MUIA_Group_SameSize, TRUE,
				Child, object->BT_WI_Information_Save = MUI_MakeObject(MUIO_Button, GetMBString(MSG_BT_PropertiesOK)),
				Child, object->BT_WI_Information_Cancel = MUI_MakeObject(MUIO_Button, GetMBString(MSG_BT_PropertiesCancel)),
			End,
		End,
	End;
}

void setInformationWindowMethods(struct ObjApp *object)
{
	MakeStaticHook(InformationOKButtonHook, saveItemInformation);

	DoMethod(object->WI_Information,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		MUIV_Notify_Self, 3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->WI_Information,
		MUIM_Notify, MUIA_Window_Open, FALSE,
		object->POP_WI_Information_Genre, 2,
		MUIM_Popstring_Close, TRUE
	);

	DoMethod(
		object->BT_WI_Information_Save,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Information, 3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(
		object->BT_WI_Information_Save,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App, 2,
		MUIM_CallHook, &InformationOKButtonHook
	);

	DoMethod(
		object->BT_WI_Information_Cancel,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Information, 3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(
		object->LV_WI_Information_Genre,
		MUIM_Notify, MUIA_List_DoubleClick, TRUE,
		object->POP_WI_Information_Genre, 2,
		MUIM_Popstring_Close, TRUE
	);
}
