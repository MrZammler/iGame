/*
  iGameMain.c
  Main source for iGame

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

#define MUI_OBSOLETE

/* MUI */
#include <libraries/mui.h>

/* Prototypes */
#include <clib/alib_protos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/lowlevel.h>
#include <proto/muimaster.h>

/* ANSI C */
#include <stdlib.h>

#ifndef __amigaos4__
#define __NOLIBBASE__
#include <proto/locale.h>
#undef __NOLIBBASE__
#else
#include <proto/locale.h>
#endif

#include "iGameExtern.h"
#include "fsfuncs.h"
#include "funcs.h"
#include "iGameGUI.h"

/* Increase stack size */
#if defined(__amigaos4__)
static const char USED min_stack[] = "$STACK:102400";
#else
LONG __stack = 32768;
#endif

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

struct ObjApp* app = NULL; /* Object */
struct Catalog *Catalog;

struct Library* MUIMasterBase;
#ifndef __amigaos4__
struct IntuitionBase	*IntuitionBase;
#else
struct Library			*IntuitionBase;
struct DataTypesIFace	*IDataTypes;
struct MUIMasterIFace	*IMUIMaster;
struct GraphicsIFace	*IGraphics;
struct IconIFace		*IIcon;
struct IntuitionIFace	*IIntuition;
struct LocaleIFace		*ILocale;
struct UtilityIFace		*IUtility;
struct WorkbenchIFace	*IWorkbench;
#endif
struct Library			*DataTypesBase;
struct Library			*GfxBase;
struct Library			*IconBase;
struct Library			*LocaleBase;
struct Library			*GuiGfxMCC;
struct Library			*GuiGfxBase;
struct Library			*LowLevelBase;
struct Library			*RenderLibBase;
struct Library			*TextEditorMCC;
struct Library			*UrltextMCC;
struct Library			*NListviewMCC;
struct Library			*UtilityBase;
struct Library			*WorkbenchBase;

char *executable_name;   // TODO: Why global?

igame_settings* iGameSettings = NULL;
BOOL blockGuiGfx = FALSE;

static int initLibraries(void);
static void cleanupLibraries(void);

static void cleanUp(void)
{
	if (app)
		app_stop();

	executable_name = NULL;

	cleanupLibraries();

	DisposeApp(app);
}

static int clean_exit(STRPTR msg)
{
	if (msg)
	{
		PutStr(msg);
		return RETURN_ERROR;
	}

	return RETURN_OK;
}

int main(int argc, char **argv)
{
	executable_name = get_executable_name(argc, argv);

	if (initLibraries() != RETURN_OK)
	{
		cleanUp();
		return RETURN_ERROR;
	}

	app = CreateApp();
	if (app)
	{
		app_start();
	}
	else
	{
		cleanUp();
		return RETURN_ERROR;
	}

	const int unit = 1;
	ULONG old = 0;
	ULONG signals;
	BOOL running = TRUE;

	while (running)
	{
		ULONG eventId = DoMethod(app->App, MUIM_Application_Input, &signals);

		switch (eventId)
		{
			case MENU_QUIT:
			case MUIV_Application_ReturnID_Quit:
				running = FALSE;
				break;

			case MENU_SCAN:
				scan_repositories();
				break;

			case MENU_ADDGAME:
				add_non_whdload();
				break;

			case MENU_ABOUT:
				set(app->WI_About, MUIA_Window_Open, TRUE);
				break;

			case MENU_GAMEPROPERTIES:
				slaveProperties();
				break;

			case MENU_ITEMINFO:
				getItemInformation();
				break;

			case MENU_GAMEFOLDER:
				open_current_dir();
				break;

			case MENU_SHOWHIDDEN:
				list_show_hidden();
				break;

			case MENU_SETTINGS:
				set(app->WI_Settings, MUIA_Window_Open, TRUE);
				break;

			case MENU_REPOSITORIES:
				set(app->WI_GameRepositories, MUIA_Window_Open, TRUE);
				break;

			case MENU_MUISETTINGS:
				DoMethod(app->App, MUIM_Application_OpenConfigWindow, 0);
				break;
		}

		// TODO: This doesn't work on AmigaOS 4. Needs to be updated with compatible code
		#if !defined(__amigaos4__)
		if (LowLevelBase)
		{
			const ULONG new = ReadJoyPort(unit);
			if (new != old)
			{
				old = new;
				joystick_input(new);
			}

			Delay(1);
		}
		#endif

		if (running && signals) Wait(signals);
	}

	cleanUp();
	return RETURN_OK;
}

static BOOL initLocale(void)
{
	if ((LocaleBase = OpenLibrary("locale.library", 37)))
	{
		#ifdef __amigaos4__
		ILocale = (struct LocaleIFace *)GetInterface( LocaleBase, "main", 1, NULL );
		if(!ILocale) return clean_exit("Can't open locale.library Interface");
		#endif

		Catalog = OpenCatalog(NULL, "iGame.catalog", OC_BuiltInLanguage, (ULONG) "english", TAG_DONE);
	}
	else
	{
		return clean_exit("Can't open locale.library v37 or greater\n");
	}

	return TRUE;
}

static void cleanupLocale(void)
{
	if (Catalog)
	{
		CloseCatalog(Catalog);
		Catalog = NULL;
	}

	#if defined(__amigaos4__)
	if (ILocale)
		DropInterface((struct Interface *)ILocale);
	#endif

	if (LocaleBase) {
		CloseLibrary(LocaleBase);
		LocaleBase = NULL;
	}
}

static int initLibraries(void)
{
	if ((MUIMasterBase = OpenLibrary(MUIMASTER_NAME, 19)))
	{
		#ifdef __amigaos4__
		IMUIMaster = (struct MUIMasterIFace *)GetInterface(MUIMasterBase, "main", 1, NULL);
		if(!IMUIMaster) return clean_exit("Can't open muimaster Interface\n");
		#endif
	}
	else return clean_exit("Can't open muimaster.library v19\n");

	if (!(TextEditorMCC = OpenLibrary("mui/TextEditor.mcc", 15)))
	{
		return clean_exit("Can't open TextEditor.mcc v15 or greater\n");
	}

	if (!(NListviewMCC = OpenLibrary("mui/NListview.mcc", 19)))
	{
		return clean_exit("Can't open NListview.mcc v19 or greater\n");
	}

	if (!(LowLevelBase = OpenLibrary("lowlevel.library", 0)))
	{
		return clean_exit("Can't open lowlevel.library\n");
	}

	if ((IconBase = OpenLibrary("icon.library", 37)))
	{
		#ifdef __amigaos4__
		IIcon = (struct IconIFace *)GetInterface( IconBase, "main", 1, NULL );
		if(!IIcon) return clean_exit("Can't open icon.library Interface\n");
		#endif
	}
	else return clean_exit("Can't open icon.library v37 or greater\n");

	#ifndef __amigaos4__
	if ((IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37)))
	#else
	if ((IntuitionBase = OpenLibrary("intuition.library", 37)))
	#endif
	{
		#ifdef __amigaos4__
		IIntuition = (struct IntuitionIFace *)GetInterface( IntuitionBase, "main", 1, NULL );
		if(!IIntuition) return clean_exit("Can't open intuition.library Interface\n");
		#endif
	}
	else return clean_exit("Can't open intuition.library v37 or greater\n");

	if ((GfxBase = OpenLibrary("graphics.library", 37)))
	{
		#ifdef __amigaos4__
		IGraphics = (struct GraphicsIFace *)GetInterface( GfxBase, "main", 1, NULL );
		if(!IGraphics) return clean_exit("Can't open graphics.library Interface\n");
		#endif
	}
	else return clean_exit("Can't open graphics.library v37 or greater\n");

	if ((WorkbenchBase = OpenLibrary("workbench.library", 37)))
	{
		#ifdef __amigaos4__
		IWorkbench = (struct WorkbenchIFace *)GetInterface( WorkbenchBase, "main", 1, NULL );
		if(!IWorkbench) return clean_exit("Can't open workbench.library Interface\n");
		#endif
	}
	else return clean_exit("Can't open workbench.library v37 or greater\n");

	if ((DataTypesBase = OpenLibrary("datatypes.library", 37)))
	{
		#ifdef __amigaos4__
		IDataTypes = (struct DataTypesIFace *)GetInterface( DataTypesBase, "main", 1, NULL );
		if(!IDataTypes) return clean_exit("Can't open datatypes.library Interface\n");
		#endif
	}
	else return clean_exit("Can't open datatypes.library v37 or greater\n");

	if ((UtilityBase = OpenLibrary("utility.library", 37)))
	{
		#ifdef __amigaos4__
		IUtility = (struct UtilityIFace *)GetInterface( UtilityBase, "main", 1, NULL );
		if(!IUtility) return clean_exit("Can't open Utility.library Interface\n");
		#endif
	}
	else return clean_exit("Can't open utility.library v37 or greater\n");

	if (!initLocale())
	{
		return RETURN_ERROR;
	}

	// Load settings here, after we load the icon.library
	iGameSettings = load_settings(DEFAULT_SETTINGS_FILE);

	// Check if MCC URLtext is installed. If not disable its usage
	iGameSettings->show_url_links = TRUE;
	if (!(UrltextMCC = OpenLibrary("mui/Urltext.mcc", 19)))
	{
		iGameSettings->show_url_links = FALSE;
	}

	if (
		!(RenderLibBase = OpenLibrary("render.library", 30)) ||
		!(GuiGfxBase = OpenLibrary("guigfx.library", 17)) ||
		!(GuiGfxMCC = OpenLibrary("mui/Guigfx.mcc", 19))
	) {
		iGameSettings->no_guigfx = 1;
		blockGuiGfx = TRUE;
	}

	return RETURN_OK;
}

static void cleanupLibraries(void)
{
	#ifdef __amigaos4__
	if(IDataTypes)		DropInterface((struct Interface *) IDataTypes);
	if(IMUIMaster)		DropInterface((struct Interface *) IMUIMaster);
	if(IGraphics)		DropInterface((struct Interface *) IGraphics);
	if(IIcon)			DropInterface((struct Interface *) IIcon);
	if(IIntuition)		DropInterface((struct Interface *) IIntuition);
	if(ILocale)			DropInterface((struct Interface *) ILocale);
	if(IUtility)		DropInterface((struct Interface *) IUtility);
	if(IWorkbench)		DropInterface((struct Interface *) IWorkbench);
	#endif

	if (UrltextMCC)		CloseLibrary(UrltextMCC);
	if (TextEditorMCC)	CloseLibrary(TextEditorMCC);
	if (NListviewMCC)	CloseLibrary(NListviewMCC);
	if (LowLevelBase)	CloseLibrary(LowLevelBase);
	if (RenderLibBase)	CloseLibrary(RenderLibBase);
	if (GfxBase)		CloseLibrary(GfxBase);
	if (GuiGfxBase)		CloseLibrary(GuiGfxBase);
	if (GuiGfxMCC)		CloseLibrary(GuiGfxMCC);
	if (DataTypesBase)	CloseLibrary(DataTypesBase);
	if (IconBase)		CloseLibrary(IconBase);
	if (IntuitionBase)	CloseLibrary((struct Library *)IntuitionBase);
	if (DataTypesBase)	CloseLibrary(DataTypesBase);
	if (UtilityBase)	CloseLibrary(UtilityBase);
	if (WorkbenchBase)	CloseLibrary(WorkbenchBase);

	cleanupLocale();

	#if defined(__amigaos4__)
	if (IMUIMaster)		DropInterface((struct Interface *)IMUIMaster);
	#endif
	if (MUIMasterBase)	CloseLibrary(MUIMasterBase);
}
