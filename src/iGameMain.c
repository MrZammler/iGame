#define MUI_OBSOLETE

#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>

/* Increase stack size */
LONG __stack = 32768;

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#include "iGameGUI.h"

struct ObjApp* app = NULL; /* Object */
extern void app_start();
extern void app_stop();
extern void read_tool_types();
extern char* get_executable_name(int argc, char** argv);

struct Library *MUIMasterBase;
char* executable_name;

void clean_exit(CONST_STRPTR s)
{
	if (s)
	{
		PutStr(s);
	}
	app_stop();
	CloseLibrary(MUIMasterBase);
}

BOOL init_app(int argc, char** argv)
{
	if ((MUIMasterBase = OpenLibrary("muimaster.library", 19)) == NULL)
	{
		clean_exit("Can't open muimaster.library v19\n");
		return FALSE;
	}

	executable_name = get_executable_name(argc, argv);
	read_tool_types();
	app = CreateApp();

	if (!app)
		clean_exit("Can't initialize application\n");
	else
	{
		
		app_start();
	}
	return TRUE;
}

int main(int argc, char** argv)
{
	init_app(argc, argv);
	ULONG sigs = 0;

	if (app)
	{
		while (DoMethod(app->App, MUIM_Application_NewInput, &sigs)
			!= MUIV_Application_ReturnID_Quit)
		{
			if (sigs)
			{
				sigs = Wait(sigs | SIGBREAKF_CTRL_C);
				if (sigs & SIGBREAKF_CTRL_C)
					break;
			}
		}
		DisposeApp(app);
	}
	else
	{
		clean_exit("Can't create application\n");
	}

	clean_exit(NULL);
	return 0;
}
