#define MUI_OBSOLETE

#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>

/* Increase stack size */
LONG __stack = 32000;

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#include "iGameGUI.h"

struct ObjApp* app = NULL; /* Object */
extern void AppStart();
extern void app_stop();
extern void read_tool_types();

struct Library *MUIMasterBase;

void CleanExit(CONST_STRPTR s)
{
	if (s)
	{
		PutStr(s);
	}
	app_stop();
	CloseLibrary(MUIMasterBase);
}

BOOL InitApp(int argc, char **argv)
{
	if ((MUIMasterBase = OpenLibrary("muimaster.library", 19)) == NULL)
	{
		CleanExit("Can't open muimaster.library v19\n");
	}
	app = CreateApp();
	if (!app)
		CleanExit("Can't initialize application\n");
	else
	{
		//read tooltypes here
		read_tool_types();
		AppStart();
	}
	
	return TRUE;
}

int main(int argc, char** argv)
{
	InitApp(argc, argv);
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
		CleanExit("Can't create application\n");
	}
	
	CleanExit(NULL);
	return 0;
}
