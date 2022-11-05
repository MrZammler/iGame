/*
  fsfuncs.c
  Filesystem functions source for iGame

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


/* MUI */
#include <libraries/mui.h>

/* Prototypes */
#include <clib/alib_protos.h>
#include <proto/wb.h>

#if defined(__amigaos4__)
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/icon.h>
#include <proto/muimaster.h>
#else
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/icon_protos.h>
#include <clib/muimaster_protos.h>
#endif

/* System */
#if defined(__amigaos4__)
// #include <dos/obsolete.h>
#define ASL_PRE_V38_NAMES
#endif
#include <libraries/asl.h>

/* ANSI C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define iGame_NUMBERS
#include "iGame_strings.h"

#include "iGameGUI.h"
#include "iGameExtern.h"
#include "strfuncs.h"
#include "funcs.h"
#include "fsfuncs.h"

extern struct ObjApp* app;
extern char* executable_name;
extern const int MAX_PATH_SIZE;

/* structures */
struct FileRequester* request;

/* global variables */
extern char fname[255];
extern games_list *item_games, *games;
extern igame_settings *current_settings;

// TODO: This is obsolete. Change it with getParentPath()
void strip_path(const char *path, char *naked_path)
{
	int i, k;
	/* strip the path from the slave file and get the rest */
	for (i = strlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '/')
			break;
	}

	for (k = 0; k <= i - 1; k++)
		naked_path[k] = path[k];
	naked_path[k] = '\0';
}

STRPTR getParentPath(STRPTR filename)
{
	STRPTR path = AllocVec(sizeof(char) * MAX_PATH_SIZE, MEMF_CLEAR);
	if (path)
	{
		BPTR fileLock = Lock(filename, SHARED_LOCK);
		if (fileLock)
		{
			BPTR folderLock = ParentDir(fileLock);
			NameFromLock(folderLock, path, sizeof(char) * MAX_PATH_SIZE);

			UnLock(folderLock);
			UnLock(fileLock);
			return path;
		}
	}

	return NULL;
}

char* get_slave_from_path(char *slave, int start, char *path)
{
	int z = 0;
	for (int k = start + 1; k <= strlen(path); k++)
	{
		slave[z] = path[k];
		z++;
	}
	return slave;
}

/*
 * Check if a path actually exists in hard disk.
 * - Return True if exists
 * - Return False if it doesn't exist
 */
BOOL check_path_exists(char *path)
{
	const BPTR lock = Lock(path, SHARED_LOCK);
	if (lock) {
		UnLock(lock);
		return TRUE;
	}

	return FALSE;
}

BOOL get_filename(const char *title, const char *positive_text, const BOOL save_mode)
{
	BOOL result = FALSE;
	if ((request = MUI_AllocAslRequest(ASL_FileRequest, NULL)) != NULL)
	{
		if (MUI_AslRequestTags(request,
		                       ASLFR_TitleText, title,
		                       ASLFR_PositiveText, positive_text,
		                       ASLFR_DoSaveMode, save_mode,
		                       ASLFR_InitialDrawer, PROGDIR,
		                       TAG_DONE))
		{
			memset(&fname[0], 0, sizeof fname);
			strcat(fname, request->fr_Drawer);
			if (fname[strlen(fname) - 1] != (UBYTE)58) /* Check for : */
				strcat(fname, "/");
			strcat(fname, request->fr_File);

			result = TRUE;
		}

		if (request)
			MUI_FreeAslRequest(request);
	}

	return result;
}

void load_games_csv_list(const char *filename)
{
	char *buf = malloc(256 * sizeof(char));
	char *tmp;

	const BPTR fpgames = Open((CONST_STRPTR) filename, MODE_OLDFILE);
	if (fpgames)
	{
		if (games != NULL)
		{
			free(games);
			games = NULL;
		}

		while (FGets(fpgames, buf, 255) != NULL)
		{
				item_games = (games_list *)calloc(1, sizeof(games_list));
				item_games->next = NULL;

			if (strlen(buf) > 1)
			{
				item_games->index = 0;
				item_games->exists = 0;
				item_games->deleted = 0;
				item_games->hidden = 0;

				tmp = strtok(buf, ";");

				tmp = strtok(NULL, ";");
				strcpy(item_games->title, tmp);

				tmp = strtok(NULL, ";");
				strcpy(item_games->genre, tmp);

				tmp = strtok(NULL, ";");
				strcpy(item_games->path, tmp);

				tmp = strtok(NULL, ";");
				item_games->favorite = atoi(tmp);

				tmp = strtok(NULL, ";");
				item_games->times_played = atoi(tmp);

				tmp = strtok(NULL, ";");
				item_games->last_played = atoi(tmp);

				tmp = strtok(NULL, ";");
				item_games->hidden = atoi(tmp);
			}

			if (games)
			{
				item_games->next = games;
				games = item_games;
			}
			else
			{
				games = item_games;
			}
		}

		add_games_to_listview();
		Close(fpgames);
	}

	if (buf)
		free(buf);
}

void save_to_csv(const char *filename, const int check_exists)
{
	char csvFilename[32];
	FILE *fpgames;

	const char* saving_message = (const char*)GetMBString(MSG_SavingGamelist);
	set(app->TX_Status, MUIA_Text_Contents, saving_message);

	strcpy(csvFilename, (CONST_STRPTR)filename);
	strcat(csvFilename, ".csv");

	fpgames = fopen(csvFilename,"w");
	if (!fpgames)
	{
		msg_box((const char*)GetMBString(MSG_FailedOpeningGameslist));
		return;
	}

	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		if (check_exists == 1)
		{
			if (item_games->exists == 1)
			{
				if (strlen(item_games->genre) == 0)
					strcpy(item_games->genre, "Unknown");
				fprintf(
					fpgames,
					"%d;%s;%s;%s;%d;%d;%d;%d\n",
					item_games->index, item_games->title, item_games->genre, item_games->path,
					item_games->favorite, item_games->times_played, item_games->last_played, item_games->hidden
				);
			}
			else
			{
				strcpy(item_games->path, "");
			}
		}
		else
		{
			if (strlen(item_games->genre) == 0)
				strcpy(item_games->genre, "Unknown");
			fprintf(
				fpgames,
				"%d;%s;%s;%s;%d;%d;%d;%d\n",
				item_games->index, item_games->title, item_games->genre, item_games->path,
				item_games->favorite, item_games->times_played, item_games->last_played, item_games->hidden
			);
		}
	}
	fclose(fpgames);

	status_show_total();
}

/*
* Gets title from a slave file
* returns 0 on success, 1 on fail
*/
int get_title_from_slave(char* slave, char* title)
{
	char slave_title[100];

	struct slave_info
	{
		unsigned long security;
		char id[8];
		unsigned short version;
		unsigned short flags;
		unsigned long base_mem_size;
		unsigned long exec_install;
		unsigned short game_loader;
		unsigned short current_dir;
		unsigned short dont_cache;
		char keydebug;
		char keyexit;
		unsigned long exp_mem;
		unsigned short name;
		unsigned short copy;
		unsigned short info;
	};

	struct slave_info sl;

	FILE* fp = fopen(slave, "rbe");
	if (fp == NULL)
	{
		return 1;
	}

	//seek to +0x20
	fseek(fp, 32, SEEK_SET);
	fread(&sl, 1, sizeof sl, fp);

	//sl.Version = (sl.Version>>8) | (sl.Version<<8);
	//sl.name = (sl.name>>8) | (sl.name<<8);

	//printf ("[%s] [%d]\n", sl.ID, sl.Version);

	//sl.name holds the offset for the slave name
	fseek(fp, sl.name + 32, SEEK_SET);
	//title = calloc (1, 100);
	//fread (title, 1, 100, fp);

	if (sl.version < 10)
	{
		return 1;
	}

	for (int i = 0; i <= 99; i++)
	{
		slave_title[i] = fgetc(fp);
		if (slave_title[i] == '\n')
		{
			slave_title[i] = '\0';
			break;
		}
	}

	strcpy(title, slave_title);
	fclose(fp);

	return 0;
}

// TODO: This seems OBSOLETE and can be replaced by getParentPath(). Needs investigation
// Get the Directory part from a full path containing a file
const char* get_directory_name(const char* str)
{
	int pos1 = get_delimiter_position(str);
	if (!pos1)
		return NULL;

	char full_path[100];
	strncpy(full_path, str, pos1);
	full_path[pos1] = '\0';

	const int pos2 = get_delimiter_position(full_path);
	if (!pos2)
		return NULL;

	char* dir_name = malloc(sizeof full_path);
	int c = 0;
	for (unsigned int i = pos2 + 1; i <= sizeof full_path; i++)
	{
		dir_name[c] = full_path[i];
		c++;
	}
	dir_name[c] = '\0';

	return dir_name;
}

// TODO: This seems OBSOLETE and can be replaced by getParentPath(). Needs investigation
// Get the complete directory path from a full path containing a file
const char *get_directory_path(const char *str)
{
	int pos1 = get_delimiter_position(str);
	if (!pos1)
		return NULL;

	char *dir_name = malloc(pos1 + 1);
	strncpy(dir_name, str, pos1);
	dir_name[pos1] = '\0';

	return dir_name;
}

// Get the application's executable name
char *get_executable_name(int argc, char **argv)
{
	// argc is zero when run from the Workbench,
	// positive when run from the CLI
	if (argc == 0)
	{
		// in AmigaOS, argv is a pointer to the WBStartup message
		// when argc is zero (run under the Workbench)
		struct WBStartup* argmsg = (struct WBStartup *)argv;
		struct WBArg* wb_arg = argmsg->sm_ArgList; /* head of the arg list */

		executable_name = wb_arg->wa_Name;
	}
	else
	{
		// Run from the CLI
		executable_name = argv[0];
	}

	return executable_name;
}

// TODO: This can use the getParentPath()
void open_current_dir(void)
{
	// Allocate Memory for variables
	char *game_title = NULL;
	const char *path_only = NULL;

	if (get_wb_version() < 44)
	{
		// workbench.library doesn't support OpenWorkbenchObjectA yet
		return;
	}

	//set the elements on the window
	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &game_title);
	if (game_title == NULL || strlen(game_title) == 0)
	{
		msg_box((const char*)GetMBString(MSG_SelectGameFromList));
		return;
	}

	const int found = title_exists(game_title);
	if (!found)
	{
		msg_box((const char*)GetMBString(MSG_SelectGameFromList));
		return;
	}

	path_only = get_directory_path(item_games->path);
	if(!path_only)
	{
		msg_box((const char*)GetMBString(MSG_DirectoryNotFound));
		return;
	}

	//Open path directory
	OpenWorkbenchObject((char *)path_only);
	free(path_only); // get_directory_path uses malloc()
}

void get_path(char *title, char *path)
{
	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		if (item_games->deleted != 1)
		{
			if (!strcmp(title, item_games->title))
			{
				strcpy(path, item_games->path);
				break;
			}
		}
	}
}
