#define MUI_OBSOLETE
#define MUIMASTER_LIBRARY "muimaster.library"

#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include "iGameExtern.h"

/* Increase stack size */
LONG __stack = 32768;

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#include "iGameGUI.h"

struct ObjApp* app = NULL; /* Object */
extern void app_stop();
extern void load_settings(const char* filename);
extern char* get_executable_name(int argc, char** argv);

struct Library* MUIMasterBase;
struct Library* LowLevelBase;
char* executable_name;

void joystick_buttons(ULONG val)
{
	//if (val & JPF_BUTTON_PLAY) printf("[PLAY/MMB]\n");
	//if (val & JPF_BUTTON_REVERSE) printf("[REVERSE]\n");
	//if (val & JPF_BUTTON_FORWARD) printf("[FORWARD]\n");
	//if (val & JPF_BUTTON_GREEN) printf("[SHUFFLE]\n");
	if (val & JPF_BUTTON_RED) 
	{
		launch_game();
	}
	//if (val & JPF_BUTTON_BLUE) printf("[STOP/RMB]\n");
}

void joystick_directions(ULONG val)
{
	if (val & JPF_JOY_UP)
		set(app->LV_GamesList, MUIA_List_Active, MUIV_List_Active_Up);

	if (val & JPF_JOY_DOWN) 
		set(app->LV_GamesList, MUIA_List_Active, MUIV_List_Active_Down);

	//if (val & JPF_JOY_LEFT) printf("[LEFT]\n");
	//if (val & JPF_JOY_RIGHT) printf("[RIGHT]\n");
}

void joystick_input(ULONG val)
{
	if ((val & JP_TYPE_MASK) == JP_TYPE_NOTAVAIL)
		return;
	if ((val & JP_TYPE_MASK) == JP_TYPE_UNKNOWN)
		return;
	if ((val & JP_TYPE_MASK) == JP_TYPE_MOUSE)
		return;

	if ((val & JP_TYPE_MASK) == JP_TYPE_JOYSTK)
	{
		joystick_directions(val);
		joystick_buttons(val);
	}

	if ((val & JP_TYPE_MASK) == JP_TYPE_GAMECTLR)
	{
		joystick_directions(val);
		joystick_buttons(val);
	}
}

void clean_exit(CONST_STRPTR s)
{
	if (s)
		PutStr(s);

	app_stop();
	executable_name = NULL;
	CloseLibrary(MUIMasterBase);
	if (LowLevelBase)
		CloseLibrary(LowLevelBase);
}

BOOL init_app(int argc, char** argv)
{
	MUIMasterBase = OpenLibrary((CONST_STRPTR)MUIMASTER_LIBRARY, 19);
	if (MUIMasterBase == NULL)
	{
		clean_exit((unsigned char*)"Can't open muimaster.library v19\n");
		return FALSE;
	}

	LowLevelBase = OpenLibrary("lowlevel.library", 0);

	executable_name = get_executable_name(argc, argv);
	load_settings(DEFAULT_SETTINGS_FILE);

	app = CreateApp();

	if (!app)
		clean_exit((unsigned char*)"Can't initialize application\n");
	else
		app_start();

	return TRUE;
}

int main(int argc, char** argv)
{
	init_app(argc, argv);
	ULONG sigs = 0;
	ULONG old = 0;
	int unit = 1;

	if (app)
	{
		while (DoMethod(app->App, MUIM_Application_NewInput, &sigs)
			!= (ULONG)MUIV_Application_ReturnID_Quit)
		{
			if (sigs)
			{
				sigs = Wait(sigs | SIGBREAKF_CTRL_C);
				if (sigs & SIGBREAKF_CTRL_C)
					break;
			}

			if (LowLevelBase)
			{
				ULONG new = ReadJoyPort(unit);
				if (new != old)
				{
					old = new;
					joystick_input(new);
				}

				Delay(1);
			}
		}
		DisposeApp(app);
	}
	else
		clean_exit((unsigned char*)"Can't create application\n");

	clean_exit(NULL);
	return 0;
}
