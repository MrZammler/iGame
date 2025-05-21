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
#include <mui/TextEditor_mcc.h>

#ifdef __MORPHOS__
#define SDI_TRAP_LIB
#endif
#include <SDI_hook.h>

/* Prototypes */
#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <proto/icon.h>

#define iGame_NUMBERS
#include "iGame_strings.h"

#include "iGameExtern.h"
#include "funcs.h"
#include "strfuncs.h"
#include "iGameGUI.h"
#include "WinProps.h"

extern igame_settings *current_settings;

APTR getPropertiesWindow(struct ObjApp *object)
{
	return WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_Properties),
		MUIA_Window_ID, MAKE_ID('1', 'I', 'G', 'A'),
		MUIA_HelpNode, "WINPROP",
		WindowContents, VGroup,
			Child, ColGroup(2),
				Child, TextObject,
					MUIA_Weight, 30,
					MUIA_Text_PreParse, "\0338",
					MUIA_Text_Contents, GetMBString(MSG_STR_PropertiesGameTitleTitle),
					MUIA_InnerLeft, 0,
					MUIA_InnerRight, 0,
				End,
				Child, object->TX_PropertiesGameTitle = TextObject,
					MUIA_Frame, MUIV_Frame_None,
					MUIA_Text_SetMin, FALSE,
					MUIA_Text_Shorten, MUIV_Text_Shorten_ElideCenter,
				End,
				Child, TextObject,
					MUIA_Weight, 30,
					MUIA_Text_PreParse, "\0338",
					MUIA_Text_Contents, GetMBString(MSG_LA_PropertiesSlavePath),
					MUIA_InnerLeft, 0,
					MUIA_InnerRight, 0,
				End,
				Child, object->TX_PropertiesSlavePath = TextObject,
					MUIA_Frame, MUIV_Frame_None,
					MUIA_Text_SetMin, FALSE,
					MUIA_Text_Shorten, MUIV_Text_Shorten_ElideCenter,
				End,
			End,
			Child, object->TX_PropertiesTooltypes = TextEditorObject,
				MUIA_Background, MUII_TextBack,
				MUIA_Frame, MUIV_Frame_String,
				MUIA_Disabled, IconBase->lib_Version < 44,
				MUIA_FrameTitle, GetMBString(MSG_TX_PropertiesTooltypesTitle),
			End,
			Child, MUI_MakeObject(MUIO_HBar,4),
			Child, HGroup, MUIA_Group_SameSize, TRUE,
				Child, object->BT_PropertiesOK = MUI_MakeObject(MUIO_Button, GetMBString(MSG_BT_PropertiesOK)),
				Child, object->BT_PropertiesCancel = MUI_MakeObject(MUIO_Button, GetMBString(MSG_BT_PropertiesCancel)),
			End,
		End,
	End;
}

void setPropertiesWindowMethods(struct ObjApp *object)
{
	MakeStaticHook(PropertiesOKButtonHook, saveItemProperties);

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
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Properties,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	// DoMethod(object->WI_Properties,
	// 	MUIM_Window_SetCycleChain,
	// 	object->BT_PropertiesOK,
	// 	object->BT_PropertiesCancel,
	// 	0
	// );
}
