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

/* Prototypes */
#include <clib/alib_protos.h>
#include <proto/muimaster.h>

#define iGame_NUMBERS
#include "iGame_strings.h"

#include "iGameExtern.h"
#include "strfuncs.h"
#include "iGameGUI.h"
#include "WinAbout.h"

extern igame_settings *current_settings;

APTR getAboutWindow(struct ObjApp *object)
{
	object->GR_WI_About_Links = HSpace(0);
	if (current_settings->show_url_links)
	{
		object->GR_WI_About_Links = VGroup,
			Child, UrltextObject,
				MUIA_Font, MUIV_Font_Normal,
				MUIA_Urltext_Text, "Website",
				MUIA_Urltext_Url, "http://winterland.no-ip.org/igame/index.html",
				MUIA_Urltext_SetMax, FALSE,
			End,
			Child, UrltextObject,
				MUIA_Font, MUIV_Font_Normal,
				MUIA_Urltext_Text, "Translations",
				MUIA_Urltext_Url, "https://translate.igame.rocks",
				MUIA_Urltext_SetMax, FALSE,
			End,
			Child, UrltextObject,
				MUIA_Font, MUIV_Font_Normal,
				MUIA_Urltext_Text, "GitHub Repository",
				MUIA_Urltext_Url, "https://github.com/MrZammler/iGame",
				MUIA_Urltext_SetMax, FALSE,
			End,
			Child, UrltextObject,
				MUIA_Font, MUIV_Font_Normal,
				MUIA_Urltext_Text, "Report issues",
				MUIA_Urltext_Url, "https://github.com/MrZammler/iGame/issues",
				MUIA_Urltext_SetMax, FALSE,
			End,
		End;
	}

	return WindowObject,
		MUIA_Window_Title, GetMBString(MSG_WI_About),
		MUIA_Window_ID, MAKE_ID('4', 'I', 'G', 'A'),
		MUIA_Window_SizeGadget, FALSE,
		WindowContents, VGroup,
			Child, MUI_NewObject(Dtpic_Classname,
					MUIA_Dtpic_Name, DEFAULT_LOGO_FILE,
					MUIA_Frame, MUIV_Frame_None,
				TAG_DONE),
			Child, VGroup,
				MUIA_Frame, MUIV_Frame_Virtual,
				MUIA_InnerTop, 10,
				MUIA_InnerLeft, 10,
				MUIA_InnerRight, 10,
				MUIA_InnerBottom, 10,
				Child, TextObject,
					MUIA_Text_Contents, object->STR_TX_About,
					MUIA_Text_SetMin, TRUE,
				End,
				Child, object->GR_WI_About_Links,
			End,
			Child, MUI_MakeObject(MUIO_HBar, 4),
			Child, object->BT_AboutOK = SimpleButton(GetMBString(MSG_BT_AboutOK)),
		End,
	End;
}

void setAboutWindowMethods(struct ObjApp *object)
{
	DoMethod(object->WI_About,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->WI_About, 3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->BT_AboutOK,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_About, 3,
		MUIM_Set, MUIA_Window_Open, FALSE
	);

	DoMethod(object->WI_About,
		MUIM_Window_SetCycleChain, object->BT_AboutOK,
		0
	);
}
