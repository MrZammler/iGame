/*
  funcs.c
  Misc functions for iGame

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
#include <mui/Guigfx_mcc.h>
#include <mui/TextEditor_mcc.h>

/* Prototypes */
#include <clib/alib_protos.h>
#include <proto/lowlevel.h>
#include <proto/wb.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/icon.h>
#include <proto/graphics.h>
#include <proto/muimaster.h>

/* System */
#if defined(__amigaos4__)
#include <dos/obsolete.h>
#define ASL_PRE_V38_NAMES
#endif

/* ANSI C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define iGame_NUMBERS
#include "iGame_strings.h"

#include "iGameGUI.h"
#include "iGameExtern.h"
#include "strfuncs.h"
#include "fsfuncs.h"
#include "slavesList.h"
#include "funcs.h"

extern struct ObjApp* app;
extern struct Library *IconBase;
#if defined(__morphos__)
extern struct Library *IntuitionBase;
#endif
extern char* executable_name;

/* global variables */
int total_hidden = 0;
int total_games;
int no_of_genres;
char* game_tooltypes;
char fname[255];
int IntroPic = 0;
int wbrun = 0;

/* function definitions */
// int get_genre(char* title, char* genre);
static int hex2dec(char* hexin);
static void check_for_wbrun();
static void showSlavesList(void);

/* structures */
struct EasyStruct msgbox;

games_list *item_games = NULL, *games = NULL;
repos_list *item_repos = NULL, *repos = NULL;
genres_list *item_genres = NULL, *genres = NULL;
igame_settings *current_settings = NULL;
listFilters filters = {0};

void setStatusText(char *text)
{
	set(app->TX_Status, MUIA_Text_Contents, text);
}

void status_show_total(void)
{
	char helper[200];
	set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
	sprintf(helper, (const char*)GetMBString(MSG_TotalNumberOfGames), total_games);
	set(app->TX_Status, MUIA_Text_Contents, helper);
}

static void apply_settings()
{
	if (current_settings == NULL)
		return;

	set(app->CH_Screenshots, MUIA_Selected, current_settings->hide_screenshots);
	set(app->CH_NoGuiGfx, MUIA_Selected, current_settings->no_guigfx);

	if (current_settings->screenshot_width == 160 && current_settings->screenshot_height == 128)
	{
		set(app->CY_ScreenshotSize, MUIA_Cycle_Active, 0);
		set(app->STR_Width, MUIA_Disabled, TRUE);
		set(app->STR_Height, MUIA_Disabled, TRUE);
	}
	else if (current_settings->screenshot_width == 320 && current_settings->screenshot_height == 256)
	{
		set(app->CY_ScreenshotSize, MUIA_Cycle_Active, 1);
		set(app->STR_Width, MUIA_Disabled, TRUE);
		set(app->STR_Height, MUIA_Disabled, TRUE);
	}
	else
	{
		set(app->CY_ScreenshotSize, MUIA_Cycle_Active, 2);
		set(app->STR_Width, MUIA_Disabled, FALSE);
		set(app->STR_Height, MUIA_Disabled, FALSE);
		set(app->STR_Width, MUIA_String_Integer, current_settings->screenshot_width);
		set(app->STR_Height, MUIA_String_Integer, current_settings->screenshot_height);
	}

	set(app->RA_TitlesFrom, MUIA_Radio_Active, current_settings->titles_from_dirs);
	if (current_settings->titles_from_dirs)
	{
		set(app->CH_SmartSpaces, MUIA_Disabled, FALSE);
		set(app->CH_SmartSpaces, MUIA_Selected, current_settings->no_smart_spaces);
	}
	else
	{
		set(app->CH_SmartSpaces, MUIA_Disabled, TRUE);
		set(app->CH_SmartSpaces, MUIA_Disabled, TRUE);
	}

	set(app->CH_SaveStatsOnExit, MUIA_Selected, current_settings->save_stats_on_exit);
	set(app->CH_FilterUseEnter, MUIA_Selected, current_settings->filter_use_enter);
	set(app->CH_HideSidepanel, MUIA_Selected, current_settings->hide_side_panel);
	set(app->CH_StartWithFavorites, MUIA_Selected, current_settings->start_with_favorites);
}

static void setDefaultSettings(igame_settings *settings)
{
	current_settings->no_guigfx				= TRUE;
	current_settings->filter_use_enter		= TRUE;
	current_settings->hide_side_panel		= FALSE;
	current_settings->save_stats_on_exit	= TRUE;
	current_settings->no_smart_spaces		= FALSE;
	current_settings->titles_from_dirs		= TRUE;
	current_settings->hide_screenshots		= TRUE;
	current_settings->screenshot_width		= 160;
	current_settings->screenshot_height		= 128;
	current_settings->start_with_favorites	= FALSE;
}

static void getPrefsPath(STRPTR prefsPath, STRPTR prefsFile)
{
	strcpy(prefsPath, "ENVARC:");
	strncat(prefsPath, prefsFile, MAX_PATH_SIZE - 5);
	if (!check_path_exists(prefsPath))
	{
		strcpy(prefsPath, "PROGDIR:");
		strncat(prefsPath, prefsFile, MAX_PATH_SIZE - 10);
	}
}

igame_settings *load_settings(const char* filename)
{
	const int buffer_size = 512;

	STRPTR file_line = malloc(buffer_size * sizeof(char));
	if (file_line == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return NULL;
	}

	STRPTR prefsPath = AllocVec(sizeof(char) * MAX_PATH_SIZE, MEMF_CLEAR);
	if(prefsPath == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return NULL;
	}
	getPrefsPath(prefsPath, (STRPTR)filename);

	if (current_settings != NULL)
	{
		free(current_settings);
		current_settings = NULL;
	}
	current_settings = (igame_settings *)calloc(1, sizeof(igame_settings));
	setDefaultSettings(current_settings);

	if (check_path_exists(prefsPath))
	{
		const BPTR fpsettings = Open(prefsPath, MODE_OLDFILE);
		if (fpsettings)
		{
			do
			{
				if (FGets(fpsettings, file_line, buffer_size) == NULL)
					break;

				file_line[strlen(file_line) - 1] = '\0';
				if (strlen(file_line) == 0)
					continue;

				if (!strncmp(file_line, "no_guigfx=", 10))
					current_settings->no_guigfx = atoi((const char*)file_line + 10);
				if (!strncmp(file_line, "filter_use_enter=", 17))
					current_settings->filter_use_enter = atoi((const char*)file_line + 17);
				if (!strncmp(file_line, "hide_side_panel=", 16))
					current_settings->hide_side_panel = atoi((const char*)file_line + 16);
				if (!strncmp(file_line, "save_stats_on_exit=", 19))
					current_settings->save_stats_on_exit = atoi((const char*)file_line + 19);
				if (!strncmp(file_line, "no_smart_spaces=", 16))
					current_settings->no_smart_spaces = atoi((const char*)file_line + 16);
				if (!strncmp(file_line, "titles_from_dirs=", 17))
					current_settings->titles_from_dirs = atoi((const char*)file_line + 17);
				if (!strncmp(file_line, "hide_screenshots=", 17))
					current_settings->hide_screenshots = atoi((const char*)file_line + 17);
				if (!strncmp(file_line, "screenshot_width=", 17))
					current_settings->screenshot_width = atoi((const char*)file_line + 17);
				if (!strncmp(file_line, "screenshot_height=", 18))
					current_settings->screenshot_height = atoi((const char*)file_line + 18);
				if (!strncmp(file_line, "start_with_favorites=", 21))
					current_settings->start_with_favorites = atoi((const char*)file_line + 21);
			}
			while (1);

			Close(fpsettings);
		}
	}

	if (prefsPath)
		FreeVec(prefsPath);

	if (file_line)
		free(file_line);

	return current_settings;
}

static void load_repos(const char* filename)
{
	const int buffer_size = 512;
	STRPTR file_line = malloc(buffer_size * sizeof(char));

	if (file_line == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}

	const BPTR fprepos = Open((CONST_STRPTR)filename, MODE_OLDFILE);
	if (fprepos)
	{
		while (FGets(fprepos, file_line, buffer_size))
		{
			if (file_line[strlen(file_line) - 1] == '\n') file_line[strlen(file_line) - 1] = '\0';
			if (strlen(file_line) == 0)
				break;

			item_repos = (repos_list *)calloc(1, sizeof(repos_list));
			item_repos->next = NULL;
			strcpy(item_repos->repo, file_line);

			if (repos == NULL)
			{
				repos = item_repos;
			}
			else
			{
				item_repos->next = repos;
				repos = item_repos;
			}

			DoMethod(app->LV_GameRepositories, MUIM_List_InsertSingle, item_repos->repo, 1, MUIV_List_Insert_Bottom);
		}

		Close(fprepos);
	}

	if (file_line)
		free(file_line);
}

static void load_genres(const char* filename)
{
	const int buffer_size = 512;
	STRPTR file_line = malloc(buffer_size * sizeof(char));
	if (file_line == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}

	int i;
	const BPTR fpgenres = Open((CONST_STRPTR)filename, MODE_OLDFILE);
	if (fpgenres)
	{
		no_of_genres = 0;
		while (FGets(fpgenres, file_line, buffer_size))
		{
			file_line[strlen(file_line) - 1] = '\0';
			if (strlen(file_line) == 0)
				break;

			item_genres = (genres_list *)calloc(1, sizeof(genres_list));
			item_genres->next = NULL;
			strcpy(item_genres->genre, file_line);

			if (genres == NULL)
			{
				genres = item_genres;
			}
			else
			{
				item_genres->next = genres;
				genres = item_genres;
			}

			no_of_genres++;
			DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, item_genres->genre, MUIV_List_Insert_Sorted);
		}

		DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, GetMBString(MSG_UnknownGenre), MUIV_List_Insert_Bottom);

		for (i = 0; i < no_of_genres; i++)
		{
			DoMethod(app->LV_GenresList, MUIM_List_GetEntry, i + 5, &app->CY_PropertiesGenreContent[i]);
		}

		app->CY_PropertiesGenreContent[i] = (unsigned char*)GetMBString(MSG_UnknownGenre);
		app->CY_PropertiesGenreContent[i + 1] = NULL;
		set(app->CY_PropertiesGenre, MUIA_Cycle_Entries, app->CY_PropertiesGenreContent);
		set(app->CY_AddGameGenre, MUIA_Cycle_Entries, app->CY_PropertiesGenreContent);

		Close(fpgenres);
	}
	if (file_line)
		free(file_line);
}

static void add_default_filters()
{
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, GetMBString(MSG_FilterShowAll), MUIV_List_Insert_Bottom);
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, GetMBString(MSG_FilterFavorites), MUIV_List_Insert_Bottom);
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, GetMBString(MSG_FilterLastPlayed), MUIV_List_Insert_Bottom);
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, GetMBString(MSG_FilterMostPlayed), MUIV_List_Insert_Bottom);
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, GetMBString(MSG_FilterNeverPlayed), MUIV_List_Insert_Bottom);
}

// Clears the list of items
static void clear_gameslist(void)
{
	DoMethod(app->LV_GamesList, MUIM_List_Clear);
	set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
}

void app_start(void)
{
	// check if the gamelist csv file exists. If not, try to load the old one
	char csvFilename[32];
	strcpy(csvFilename, (CONST_STRPTR)DEFAULT_GAMESLIST_FILE);
	strcat(csvFilename, ".csv");

	load_repos(DEFAULT_REPOS_FILE);
	add_default_filters();
	load_genres(DEFAULT_GENRES_FILE);
	apply_settings();
	check_for_wbrun();

	IntroPic = 1;

	if (current_settings->start_with_favorites)
	{
		filters.showGroup = GROUP_FAVOURITES;
	}

	DoMethod(app->App,
		MUIM_Application_Load,
		MUIV_Application_Load_ENV
	);

	set(app->WI_MainWindow, MUIA_Window_Open, TRUE);

	// Load the slaves list from the gameslist file
	// TODO: Add a message at the status line of the window
	set(app->WI_MainWindow, MUIA_Window_Sleep, TRUE);
	slavesListLoadFromCSV(csvFilename);
	showSlavesList();
	set(app->WI_MainWindow, MUIA_Window_Sleep, FALSE);
	set(app->WI_MainWindow, MUIA_Window_ActiveObject, app->LV_GamesList);
}

void filter_change(void)
{
	char *title = NULL;
	char *genreSelection = NULL;
	filters.showGenre[0] = '\0';

	get(app->STR_Filter, MUIA_String_Contents, &title);
	DoMethod(app->LV_GenresList, MUIM_List_GetEntry,
		MUIV_List_GetEntry_Active, &genreSelection);

	if (title && strlen(title) > 0)
	{
		string_to_lower(title);
		strncpy(filters.title, title, sizeof(filters.title));
	}
	else filters.title[0] = '\0';

	if (genreSelection == NULL || !strcmp(genreSelection, GetMBString(MSG_FilterShowAll)))
		filters.showGroup = GROUP_SHOWALL;

	else if (!strcmp(genreSelection, GetMBString(MSG_FilterFavorites)))
		filters.showGroup = GROUP_FAVOURITES;

	else if (!strcmp(genreSelection, GetMBString(MSG_FilterLastPlayed)))
		filters.showGroup = GROUP_LAST_PLAYED;

	else if (!strcmp(genreSelection, GetMBString(MSG_FilterMostPlayed)))
		filters.showGroup = GROUP_MOST_PLAYED;

	else if (!strcmp(genreSelection, GetMBString(MSG_FilterNeverPlayed)))
		filters.showGroup = GROUP_NEVER_PLAYED;

	else strncpy(filters.showGenre, genreSelection, sizeof(filters.showGenre));

	showSlavesList();
}

/*
 * Checks if WBRun exists in C:
 */
static void check_for_wbrun(void)
{
	// TODO: check these to use the check_path_exists() method
	const BPTR oldlock = Lock((CONST_STRPTR)PROGDIR, ACCESS_READ);
	const BPTR lock = Lock("C:WBRun", ACCESS_READ);

	if (lock)
	{
		wbrun=1;
	}
	else
	{
		wbrun=0;
	}

	CurrentDir(oldlock);
}

static void prepareWHDExecution(slavesList *node, char *result)
{
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = AllocVec(bufSize, MEMF_CLEAR);

	snprintf(buf, bufSize, "%s", substring(node->path, 0, -6));
	struct DiskObject *diskObj = GetDiskObjectNew(buf);
	if (diskObj)
	{
		char to_check[256];
		sprintf(result, "whdload ");

		for (STRPTR *tool_types = diskObj->do_ToolTypes; (buf = *tool_types); ++tool_types)
		{
			if (!strncmp(buf, "*** DON'T EDIT", 14) || !strncmp(buf, "IM", 2)) continue;
			if (buf[0] == ' ') continue;
			if (buf[0] == '(') continue;
			if (buf[0] == '*') continue;
			if (buf[0] == ';') continue;
			if (buf[0] == '\0') continue;
			if (buf[0] == -69) continue; // »
			if (buf[0] == -85) continue; // «
			if (buf[0] == '.') continue;
			if (buf[0] == '=') continue;
			if (buf[0] == '#') continue;
			if (buf[0] == '!') continue;

			/* Add quotes to Execute.... ToolTypes for WHDLoad compatibility */
			if (!strncmp(buf, "Execute", 7))
			{
				char** temp_tbl = my_split((char *)buf, "=");
				if (temp_tbl == NULL) continue;
				if (temp_tbl[1] != NULL)
				{
					sprintf(buf,"%s=\"%s\"", temp_tbl[0],temp_tbl[1]);
				}
				if (temp_tbl)
					free(temp_tbl);
			}

			/* Must check here for numerical values */
			/* Those (starting with $ should be transformed to dec from hex) */
			char** temp_tbl = my_split((char *)buf, "=");
			if (temp_tbl == NULL
				|| temp_tbl[0] == NULL
				|| !strcmp((char *)temp_tbl[0], " ")
				|| !strcmp((char *)temp_tbl[0], ""))
				continue;

			if (temp_tbl[1] != NULL)
			{
				sprintf(to_check, "%s", temp_tbl[1]);
				if (to_check[0] == '$')
				{
					const int dec_rep = hex2dec(to_check);
					sprintf(buf, "%s=%d", temp_tbl[0], dec_rep);
				}
			}
			if (temp_tbl)
				free(temp_tbl);

			sprintf(result, "%s %s", result, buf);
		}

		FreeDiskObject(diskObj);
	}

	FreeVec(buf);
}

static BOOL createRunGameScript(slavesList *node)
{
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = AllocVec(bufSize, MEMF_CLEAR);

	getParentPath(node->path, buf);
	FILE* runGameScript = fopen("T:rungame", "we");
	if (!runGameScript)
	{
		printf("Could not open rungame file!");
		FreeVec(buf);
		return FALSE;
	}
	getParentPath(node->path, buf);
	fprintf(runGameScript, "Cd \"%s\"\n", buf);
	getNameFromPath(node->path, buf, bufSize);
	fprintf(runGameScript, "Run >NIL: \"%s\"\n", buf);
	fclose(runGameScript);

	FreeVec(buf);
	return TRUE;
}

static void launchSlave(slavesList *node)
{
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = AllocVec(bufSize, MEMF_CLEAR);

	getParentPath(node->path, buf);
	const BPTR pathLock = Lock(buf, SHARED_LOCK);

	if (pathLock && IconBase)
	{
		struct FileInfoBlock *FIblock = (struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock), MEMF_CLEAR);
		char *tooltypesBuffer = AllocVec(sizeof(char) * 1024, MEMF_CLEAR);

		const BPTR oldLock = CurrentDir(pathLock);

		// Get the slave filename and replace extension
		getNameFromPath(node->path, buf, bufSize);
		snprintf(buf, bufSize, "%s.info", substring(buf, 0, -6));

		if (Examine(pathLock, FIblock))
		{
			while (ExNext(pathLock, FIblock))
			{
				if (
					(FIblock->fib_DirEntryType < 0) &&
					(strcasestr(FIblock->fib_FileName, ".info")) &&
					!strncmp(FIblock->fib_FileName, buf, sizeof(FIblock->fib_FileName))
				) {
					int execSize = sizeof(char) * MAX_EXEC_SIZE;
					char *exec = AllocVec(bufSize, MEMF_CLEAR);
					prepareWHDExecution(node, exec);

					// TODO: Need to make all the other items to 0
					// Update statistics info
					node->last_played = 1;
					node->times_played++;

					// Save stats
					if (!current_settings->save_stats_on_exit)
						save_list(0);

					int success = Execute(exec, 0, 0);
					if (success == 0)
						msg_box((const char*)GetMBString(MSG_ErrorExecutingWhdload));

					FreeVec(exec);
				}
			}

			FreeVec(FIblock);
		}

		UnLock(pathLock);
		CurrentDir(oldLock);
		UnLock(oldLock);
	}

	FreeVec(buf);
}

static void launchFromWB(slavesList *node)
{
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = AllocVec(bufSize, MEMF_CLEAR);
	char *exec = AllocVec(bufSize, MEMF_CLEAR);

	strncpy(buf, "C:WBRun", bufSize);
	if (check_path_exists(buf))
	{
		sprintf(exec, "C:WBRun \"%s\"", node->path);
	}

	strncpy(buf, "C:WBLoad", bufSize);
	if (check_path_exists(buf))
	{
		sprintf(exec, "C:WBLoad \"%s\"", node->path);
	}

	if (isStringEmpty(exec))
	{
		if (createRunGameScript(node))
		{
			sprintf(exec, "Execute T:rungame");
		}
	}


	if (!isStringEmpty(exec))
	{
		// Update statistics info
		node->last_played = 1;
		node->times_played++;

		// Save stats
		if (!current_settings->save_stats_on_exit)
			save_list(0);

		int success = Execute(exec, 0, 0);
		if (success == 0)
			msg_box((const char*)GetMBString(MSG_ErrorExecutingWhdload));
	}

	FreeVec(exec);
	FreeVec(buf);
}

void launch_game(void)
{
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = AllocVec(bufSize, MEMF_CLEAR);
	char *game_title = NULL;

	// Get the selected item from the list
	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &game_title);
	if (game_title == NULL || game_title[0] == '\0')
	{
		msg_box((const char*)GetMBString(MSG_SelectGameFromList));
		return;
	}

	slavesList *existingNode = malloc(sizeof(slavesList));
	if ((existingNode = slavesListSearchByTitle(game_title, sizeof(char) * MAX_SLAVE_TITLE_SIZE)) == NULL)
	{
		msg_box((const char*)GetMBString(MSG_SelectGameFromList));
		return;
	}

	if(!check_path_exists(existingNode->path)) {
		msg_box((const char*)GetMBString(MSG_slavePathDoesntExist));
		return;
	}

	getNameFromPath(existingNode->path, buf, bufSize);

	// Slave file found
	if (strcasestr(buf, ".slave"))
	{
		launchSlave(existingNode);
	}
	else
	{
		launchFromWB(existingNode);
	}
}

/*
*   Executes whdload with the slave
*/
// TODO: Replaced by the new launch_game() - DEPRECATED
void launch_game_old(void)
{
	struct Library* icon_base;
	struct DiskObject* disk_obj;
	char *game_title = NULL, exec[256], *tool_type;
	int success, i, whdload = 0;
	char str2[512], fullpath[800], helperstr[250], to_check[256];

	// clear vars:
	str2[0] = '\0';
	fullpath[0] = '\0';

	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &game_title);
	if (game_title == NULL || strlen(game_title) == 0)
	{
		msg_box((const char*)GetMBString(MSG_SelectGameFromList));
		return;
	}

	char* path = malloc(256 * sizeof(char));
	if (path == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}
	get_path(game_title, path);

	if(!check_path_exists(path)) {
		msg_box((const char*)GetMBString(MSG_slavePathDoesntExist));
		return;
	}

	sprintf(helperstr, (const char*)GetMBString(MSG_RunningGameTitle), game_title);
	set(app->TX_Status, MUIA_Text_Contents, helperstr);

	unsigned char* naked_path = malloc(256 * sizeof(char));
	if (naked_path != NULL)
		strip_path(path, (char*)naked_path);
	else
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}

	char* slave = malloc(256 * sizeof(char));
	if (slave != NULL)
		slave = get_slave_from_path(slave, strlen(naked_path), path);
	else
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}
	string_to_lower(slave);
	if (strstr(slave, ".slave"))
		whdload = 1;

	const BPTR oldlock = Lock((CONST_STRPTR)PROGDIR, ACCESS_READ);
	const BPTR lock = Lock(naked_path, ACCESS_READ);
	if (lock)
		CurrentDir(lock);
	else
	{
		msg_box((const char*)GetMBString(MSG_DirectoryNotFound));
		return;
	}

	if (whdload == 1)
		sprintf(exec, "whdload ");
	else
	{
		if (wbrun)
			sprintf(exec, "C:WBRun \"%s\"", path);
		else
			sprintf(exec, "\"%s\"", path);
	}

	//tooltypes only for whdload games
	if (whdload == 1)
	{
		if (IconBase)
		{
			//scan the .info files in the current dir.
			//one of them should be the game's project icon.

			/*  allocate space for a FileInfoBlock */
			struct FileInfoBlock* m = (struct FileInfoBlock *)AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);

			Examine(lock, m);
			if (m->fib_DirEntryType <= 0)
			{
				/*  We don't allow "opta file", only "opta dir" */
				return;
			}

			while ((success = ExNext(lock, m)))
			{
				if (strcasestr(m->fib_FileName, ".info"))
				{
					NameFromLock(lock, (unsigned char*)str2, 512);
					sprintf(fullpath, "%s/%s", str2, m->fib_FileName);

					//lose the .info
					for (i = strlen(fullpath) - 1; i >= 0; i--)
					{
						if (fullpath[i] == '.')
							break;
					}
					fullpath[i] = '\0';

					if ((disk_obj = GetDiskObject((STRPTR)fullpath)))
					{
						if (MatchToolValue(FindToolType(disk_obj->do_ToolTypes, (unsigned char*)"SLAVE"), (STRPTR)slave)
							|| MatchToolValue(FindToolType(disk_obj->do_ToolTypes, (unsigned char*)"slave"), (STRPTR)slave))
						{
							for (char** tool_types = (char **)disk_obj->do_ToolTypes; (tool_type = *tool_types); ++
							     tool_types)
							{
								if (!strncmp(tool_type, "IM", 2)) continue;
								if (tool_type[0] == ' ') continue;
								if (tool_type[0] == '(') continue;
								if (tool_type[0] == '*') continue;
								if (tool_type[0] == ';') continue;
								if (tool_type[0] == '\0') continue;
								if (tool_type[0] == -69) continue; // »
								if (tool_type[0] == -85) continue; // «
								if (tool_type[0] == '.') continue;
								if (tool_type[0] == '=') continue;
								if (tool_type[0] == '#') continue;
								if (tool_type[0] == '!') continue;

								/* Add quotes to Execute.... ToolTypes for WHDLoad compatibility */
								if (!strncmp(tool_type, "Execute", 7))
								{
									char** temp_tbl = my_split((char *)tool_type, "=");
									if (temp_tbl == NULL) continue;
									if (temp_tbl[1] != NULL)
									{
										sprintf(tool_type,"%s=\"%s\"", temp_tbl[0],temp_tbl[1]);
									}
									if (temp_tbl)
										free(temp_tbl);
								}

								/* Must check here for numerical values */
								/* Those (starting with $ should be transformed to dec from hex) */
								char** temp_tbl = my_split((char *)tool_type, "=");
								if (temp_tbl == NULL
									|| temp_tbl[0] == NULL
									|| !strcmp((char *)temp_tbl[0], " ")
									|| !strcmp((char *)temp_tbl[0], ""))
									continue;

								if (temp_tbl[1] != NULL)
								{
									sprintf(to_check, "%s", temp_tbl[1]);
									if (to_check[0] == '$')
									{
										const int dec_rep = hex2dec(to_check);
										sprintf(tool_type, "%s=%d", temp_tbl[0], dec_rep);
									}
								}
								if (temp_tbl)
									free(temp_tbl);
								//req: more free's

								sprintf(exec, "%s %s", exec, tool_type);
							}

							FreeDiskObject(disk_obj);
							break;
						}
						FreeDiskObject(disk_obj);
					}
				}
			}
		}

		//if we're still here, and exec contains just whdload, add the slave and execute
		if (!strcmp(exec, "whdload "))
		{
			sprintf(exec, "%s %s", exec, path);
		}
	}

	// Cleanup the memory allocations
	if (slave)
		free(slave);
	if (path)
		free(path);
	if (naked_path)
		free(naked_path);

	//set the counters for this game
	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		if (!strcmp(item_games->title, game_title))
		{
			item_games->last_played = 1;
			item_games->times_played++;
		}

		if (item_games->last_played == 1 && strcmp(item_games->title, game_title))
		{
			item_games->last_played = 0;
		}
	}

	if (!current_settings->save_stats_on_exit)
		save_list(0);

	success = Execute((unsigned char*)exec, 0, 0);

	if (success == 0)
		msg_box((const char*)GetMBString(MSG_ErrorExecutingWhdload));

	CurrentDir(oldlock);
	status_show_total();
}

static void showSlavesList(void)
{
	size_t cnt = 0;
	int bufSize = sizeof(char) * MAX_SLAVE_TITLE_SIZE;
	char *buf = malloc(bufSize);
	slavesList *currPtr = getSlavesListHead();
	int mostPlayedTimes = 0;

	clear_gameslist();

	while (currPtr != NULL)
	{
		if (
			(
				(!filters.showHiddenOnly && !currPtr->hidden) ||
				(filters.showHiddenOnly && currPtr->hidden)
			) && !currPtr->deleted
		) {
			// Filter the list by the selected genre
			if (
				!isStringEmpty(filters.showGenre) &&
				strncmp(filters.showGenre, currPtr->genre, sizeof(currPtr->genre))
			)
			{
				goto nextItem;
			}

			// Decide from where the item name will be taken
			if(!isStringEmpty(currPtr->user_title))
			{
				snprintf(buf, bufSize, "%s", currPtr->user_title);
			}
			else if(isStringEmpty(currPtr->user_title) && (currPtr->instance > 0))
			{
				snprintf(currPtr->user_title, sizeof(currPtr->user_title),
					"%s [%d]", currPtr->title, currPtr->instance);
				snprintf(buf, bufSize, "%s", currPtr->user_title);
			}
			else
			{
				snprintf(buf, bufSize, "%s", currPtr->title);
			}

			// Filter list based on entered string
			if (!isStringEmpty(filters.title) && !strcasestr(buf, filters.title))
			{
				goto nextItem;
			}

			// Filter results based on selected group
			if (filters.showGroup > 0) {
				switch (filters.showGroup)
				{
					case GROUP_FAVOURITES:
						if (!currPtr->favourite) goto nextItem;
						break;
					case GROUP_LAST_PLAYED:
						if (!currPtr->last_played) goto nextItem;
						break;
					case GROUP_MOST_PLAYED:
						if (currPtr->times_played > 0)
						{
							if (currPtr->times_played < mostPlayedTimes)
							{
								DoMethod(app->LV_GamesList,
									MUIM_List_InsertSingle, currPtr->title,
									MUIV_List_Insert_Bottom);
							}
							else
							{
								mostPlayedTimes = currPtr->times_played;
								DoMethod(app->LV_GamesList,
									MUIM_List_InsertSingle, currPtr->title,
									MUIV_List_Insert_Top);
							}
						}
						goto nextItem;
						break;
					case GROUP_NEVER_PLAYED:
						if (currPtr->times_played > 0)
						{
							goto nextItem;
						}
						break;
				}
			}

			DoMethod(app->LV_GamesList,
				MUIM_List_InsertSingle, buf,
				MUIV_List_Insert_Sorted);

			cnt++;
		nextItem:
		}

		currPtr = currPtr->next;
	}
	set(app->LV_GamesList, MUIA_List_Quiet, FALSE);

	sprintf(buf, GetMBString(MSG_TotalNumberOfGames), cnt);
	setStatusText(buf);
	free(buf);
}

/*
 * Generate a title name based on user settings and the path
 */
static void generateItemName(char *path, char *result)
{
	if (current_settings->titles_from_dirs)
	{
		getTitleFromPath(path, result);
	}
	else
	{
		get_title_from_slave(path, result);
		if (!strncmp(result, "", sizeof(char) * MAX_SLAVE_TITLE_SIZE))
		{
			getTitleFromPath(path, result);
		}
	}
}

static void examineFolder(char *path)
{
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = malloc(bufSize);
	if(buf == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}

	const BPTR lock = Lock(path, SHARED_LOCK);
	if (lock) {
		struct FileInfoBlock *FIblock = (struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock), MEMF_CLEAR);

		if (Examine(lock, FIblock))
		{
			while (ExNext(lock, FIblock))
			{
				if(FIblock->fib_DirEntryType > 0)
				{
					if (
						strncmp((unsigned char *)FIblock->fib_FileName, "data\0", 5) &&
						strncmp((unsigned char *)FIblock->fib_FileName, "Data\0", 5)
					) {
						strcpy(buf, FIblock->fib_FileName);

						if(!isPathFolder(path))
						{
							sprintf(buf, "%s%s", path, FIblock->fib_FileName);
						}
						else
						{
							sprintf(buf, "%s/%s", path, FIblock->fib_FileName);
						}

						getFullPath(buf, buf);
						setStatusText(buf);
						examineFolder(buf);
					}
				}

				if(FIblock->fib_DirEntryType < 0)
				{
					// Slave file found
					if (strcasestr(FIblock->fib_FileName, ".slave"))
					{
						slavesList *node = malloc(sizeof(slavesList));
						slavesList *existingNode = malloc(sizeof(slavesList));

						if(!isPathFolder(path))
						{
							sprintf(buf, "%s%s", path, FIblock->fib_FileName);
						}
						else
						{
							sprintf(buf, "%s/%s", path, FIblock->fib_FileName);
						}

						// Find if already exists in the list and ignore it
						if (!(existingNode = slavesListSearchByPath(buf, bufSize)))
						{
							node->instance = 0;
							node->genre[0] = '\0';
							node->user_title[0] = '\0';
							node->times_played = 0;
							node->favourite = 0;
							node->last_played = 0;
							node->exists = 1;
							node->hidden = 0;
							node->deleted = 0;

							getFullPath(buf, buf);
							strncpy(node->path, buf, sizeof(node->path));

							// Generate title and add in the list
							generateItemName(node->path, node->title);

							// Scan how many others with same title exist and increase a number at the end of the list (alt)
							slavesListCountInstancesByTitle(node);

							// TODO: Instead of adding at the tail insert it in sorted position
							slavesListAddTail(node);
						}
						else
						{
							// Generate title and add in the list
							generateItemName(existingNode->path, existingNode->title);

							slavesListCountInstancesByTitle(existingNode);
						}
					}
				}
			}

			FreeVec(FIblock);
		}

		UnLock(lock);
	}

	free(buf);
}

// Replaces scan_repositories_old()
void scan_repositories(void)
{
	if (repos)
	{
		set(app->WI_MainWindow, MUIA_Window_Sleep, TRUE);
		// This is necessary for counting the same title instance
		// when the list is not empty (start/change of settings)
		// slavesListClearTitles();

		for (item_repos = repos; item_repos != NULL; item_repos = item_repos->next)
		{
			if(check_path_exists(item_repos->repo))
			{
				examineFolder(item_repos->repo);
			}
		}
		showSlavesList();
		set(app->WI_MainWindow, MUIA_Window_Sleep, FALSE);
	}
}

static void refresh_sidepanel()
{
	DoMethod(app->GR_sidepanel, MUIM_Group_InitChange);
	DoMethod(app->GR_sidepanel, OM_REMMEMBER, app->IM_GameImage_0);
	DoMethod(app->GR_sidepanel, OM_REMMEMBER, app->Space_Sidepanel);
	DoMethod(app->GR_sidepanel, OM_REMMEMBER, app->LV_GenresList);
	MUI_DisposeObject(app->IM_GameImage_0);
	DoMethod(app->GR_sidepanel, OM_ADDMEMBER, app->IM_GameImage_0 = app->IM_GameImage_1);
	DoMethod(app->GR_sidepanel, OM_ADDMEMBER, app->Space_Sidepanel);
	DoMethod(app->GR_sidepanel, OM_ADDMEMBER, app->LV_GenresList);
	DoMethod(app->GR_sidepanel, MUIM_Group_ExitChange);
}

static void show_screenshot(STRPTR screenshot_path)
{
	static char prvScreenshot[MAX_PATH_SIZE];

	if (strcmp(screenshot_path, prvScreenshot))
	{
		if (current_settings->no_guigfx)
		{
			app->IM_GameImage_1 = MUI_NewObject(Dtpic_Classname,
						MUIA_Dtpic_Name,				screenshot_path,
						MUIA_Frame, 					MUIV_Frame_ImageButton,
			End;
		}
		else
		{
			app->IM_GameImage_1 = GuigfxObject,
						MUIA_Guigfx_FileName,			screenshot_path,
						MUIA_Guigfx_Quality,			MUIV_Guigfx_Quality_Best,
						MUIA_Guigfx_ScaleMode,			NISMF_SCALEFREE | NISMF_KEEPASPECT_PICTURE,
						MUIA_Guigfx_Transparency,		0,
						MUIA_Frame, 					MUIV_Frame_ImageButton,
						MUIA_FixHeight, 				current_settings->screenshot_height,
						MUIA_FixWidth, 					current_settings->screenshot_width,
			End;
		}

		if (app->IM_GameImage_1)
		{
			refresh_sidepanel();
		}

		strcpy(prvScreenshot, screenshot_path);
	}
}

static void get_screenshot_path(char *game_title, char *result)
{
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = malloc(bufSize);
	if(buf == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}

	slavesList *existingNode = malloc(sizeof(slavesList));
	if (existingNode = slavesListSearchByTitle(game_title, sizeof(char) * MAX_SLAVE_TITLE_SIZE))
	{
		getParentPath(existingNode->path, buf);

		// Return the igame.iff from the game folder, if exists
		snprintf(result, sizeof(char) * MAX_PATH_SIZE, "%s/igame.iff", buf);
		if(checkImageDatatype(result))
		{
			free(buf);
			return;
		}

		// Return the slave icon from the game folder, if exists
		// TODO: Check if this item is a slave. If not don't use the substring
		snprintf(result, sizeof(char) * MAX_PATH_SIZE, "%s.info", substring(existingNode->path, 0, -6));
		if(checkImageDatatype(result))
		{
			free(buf);
			return;
		}

		// Return the default image from iGame folder, if exists
		if(check_path_exists(DEFAULT_SCREENSHOT_FILE))
		{
			snprintf(result, sizeof(char) * MAX_PATH_SIZE, "%s", DEFAULT_SCREENSHOT_FILE);
		}
	}
	free(buf);
}

static void showDefaultScreenshot(void)
{
	if (current_settings->hide_side_panel || current_settings->hide_screenshots)
		return;

	show_screenshot(DEFAULT_SCREENSHOT_FILE);
}

void game_click(void)
{
	if (current_settings->hide_side_panel || current_settings->hide_screenshots)
		return;

	char *game_title = NULL;
	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &game_title);

	if (game_title) //for some reason, game_click is called and game_title is null??
	{
		char *image_path = AllocVec(sizeof(char) * MAX_PATH_SIZE, MEMF_CLEAR);
		if(image_path == NULL)
		{
			msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
			return;
		}

		get_screenshot_path(game_title, image_path);
		show_screenshot(image_path);

		FreeVec(image_path);
	}
}

// TODO: Seems unused
/* Retrieves the Genre from the file, using the Title */
// int get_genre(char* title, char* genre)
// {
// 	for (item_games = games; item_games != NULL; item_games = item_games->next)
// 	{
// 		if (item_games->deleted != 1)
// 		{
// 			if (!strcmp(title, item_games->title))
// 			{
// 				strcpy(genre, item_games->genre);
// 				break;
// 			}
// 		}
// 	}

// 	return 0;
// }

/*
* Adds a repository (path on the disk)
* to the list of repositories
*/
void repo_add(void)
{
	char *buf = malloc(sizeof(char) * MAX_PATH_SIZE);
	char* repo_path = NULL;
	get(app->PA_RepoPath, MUIA_String_Contents, &repo_path);

	if (repo_path && strlen(repo_path) != 0)
	{
		item_repos = (repos_list *)calloc(1, sizeof(repos_list));
		item_repos->next = NULL;
		getFullPath(repo_path, buf);
		strcpy(item_repos->repo, buf);

		if (repos == NULL)
			repos = item_repos;
		else
		{
			item_repos->next = repos;
			repos = item_repos;
		}

		DoMethod(app->LV_GameRepositories, MUIM_List_InsertSingle, item_repos->repo, 1, MUIV_List_Insert_Bottom);
	}
	free(buf);
}

void repo_remove(void)
{
	// TODO: If a repo is removed is not removed from the list
	DoMethod(app->LV_GameRepositories, MUIM_List_Remove, MUIV_List_Remove_Active);
}

/*
* Writes the repositories to the repo.prefs file
*/
void repo_stop(void)
{
	const BPTR fprepos = Open((CONST_STRPTR)DEFAULT_REPOS_FILE, MODE_NEWFILE);
	if (!fprepos)
	{
		msg_box((const char*)GetMBString(MSG_CouldNotCreateReposFile));
	}
	else
	{
		CONST_STRPTR repo_path = NULL;
		for (int i = 0;; i++)
		{
			DoMethod(app->LV_GameRepositories, MUIM_List_GetEntry, i, &repo_path);
			if (!repo_path)
				break;

			FPuts(fprepos, repo_path);
			FPutC(fprepos, '\n');
		}
		Close(fprepos);
	}
}

// Shows the Properties window populating the information fields
void slaveProperties(void)
{
	// TODO: This needs to be cleared on Window close
	// set(app->WI_MainWindow, MUIA_Window_Sleep, TRUE);

	char* title = NULL;
	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &title);
	if(isStringEmpty(title))
	{
		msg_box((const char*)GetMBString(MSG_SelectGameFromList));
		return;
	}

	slavesList *node = malloc(sizeof(slavesList));
	if (node = slavesListSearchByTitle(title, sizeof(char) * MAX_SLAVE_TITLE_SIZE))
	{
		int pathBufferSize = sizeof(char) * MAX_PATH_SIZE;
		char *pathBuffer = malloc(pathBufferSize);
		char *tooltypesBuffer = AllocVec(sizeof(char) * 1024, MEMF_CLEAR);
		char timesPlayedStr[6];
		size_t i;

		setSlavesListBuffer(node);

		set(app->STR_PropertiesGameTitle, MUIA_String_Contents, node->title);
		if(!isStringEmpty(node->user_title))
		{
			set(app->STR_PropertiesGameTitle, MUIA_String_Contents, node->user_title);
		}
		set(app->TX_PropertiesSlavePath, MUIA_Text_Contents, node->path);

		snprintf(timesPlayedStr, sizeof(char) * 5, "%d", node->times_played);
		set(app->TX_PropertiesTimesPlayed, MUIA_Text_Contents, timesPlayedStr);

		//set the genre
		// TODO: Make this better when we have the new Genre lists
		for (i = 0; i < no_of_genres; i++)
		{
			if (!strcmp(app->CY_PropertiesGenreContent[i], node->genre))
				break;
		}
		set(app->CY_PropertiesGenre, MUIA_Cycle_Active, i);

		set(app->CH_PropertiesFavorite, MUIA_Selected, node->favourite);
		set(app->CH_PropertiesHidden, MUIA_Selected, node->hidden);

		// TODO: Check if this item is a slave. If not don't use the substring
		snprintf(pathBuffer, sizeof(char) * MAX_PATH_SIZE, "%s", substring(node->path, 0, -6));
		getIconTooltypes(pathBuffer, tooltypesBuffer);
		set(app->TX_PropertiesTooltypes, MUIA_TextEditor_Contents, tooltypesBuffer);

		set(app->WI_Properties, MUIA_Window_Open, TRUE);
	}
}

// Save item properties when the user clicks on Save button
void saveItemProperties(void)
{
	int genreId, hideSelection;
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = AllocVec(bufSize, MEMF_CLEAR);
	ULONG newpos;
	// char *tooltypesBuffer = AllocVec(sizeof(char) * 1024, MEMF_CLEAR);

	slavesList *node = malloc(sizeof(slavesList));
	node = getSlavesListBuffer();

	get(app->STR_PropertiesGameTitle, MUIA_String_Contents, &buf);
	if(strncmp(node->title, buf, sizeof(char) * MAX_SLAVE_TITLE_SIZE))
	{
		strncpy(node->user_title, buf, sizeof(node->user_title));

		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		DoMethod(app->LV_GamesList, MUIM_List_Remove, MUIV_List_Remove_Active);
		DoMethod(app->LV_GamesList,
			MUIM_List_InsertSingle, node->user_title,
			MUIV_List_Insert_Sorted);
		get(app->LV_GamesList, MUIA_List_InsertPosition, &newpos);
		set(app->LV_GamesList, MUIA_List_Active, newpos);
		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
	}

	if(
		!strncmp(node->title, buf, sizeof(char) * MAX_SLAVE_TITLE_SIZE) &&
		strncmp(node->user_title, buf, sizeof(char) * MAX_SLAVE_TITLE_SIZE)
	) {
		node->user_title[0] = '\0';

		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		DoMethod(app->LV_GamesList, MUIM_List_Remove, MUIV_List_Remove_Active);
		DoMethod(app->LV_GamesList,
			MUIM_List_InsertSingle, node->title,
			MUIV_List_Insert_Sorted);
		get(app->LV_GamesList, MUIA_List_InsertPosition, &newpos);
		set(app->LV_GamesList, MUIA_List_Active, newpos);
		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
	}

	// Get the Genre
	get(app->CY_PropertiesGenre, MUIA_Cycle_Active, &genreId);
	strncpy(node->genre,
		app->CY_PropertiesGenreContent[genreId],
		sizeof(node->genre)
	);

	get(app->CH_PropertiesFavorite, MUIA_Selected, &node->favourite);

	get(app->CH_PropertiesHidden, MUIA_Selected, &hideSelection);
	if (node->hidden != hideSelection)
	{
		node->hidden = hideSelection;
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		DoMethod(app->LV_GamesList, MUIM_List_Remove, MUIV_List_Remove_Active);
		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
	}

	// Save the tooltypes
	if (IconBase->lib_Version >= 44)
	{
		showDefaultScreenshot();
		STRPTR tooltypesBuffer = (STRPTR)DoMethod(app->TX_PropertiesTooltypes, MUIM_TextEditor_ExportText);
		snprintf(buf, sizeof(node->path), "%s", substring(node->path, 0, -6));
		setIconTooltypes(buf, tooltypesBuffer);
		game_click();
	}
}

void list_show_hidden(void)
{
	if (!filters.showHiddenOnly)
	{
		set(app->LV_GenresList, MUIA_Disabled, TRUE);
		filters.showHiddenOnly = TRUE;
		showSlavesList();
	}
	else
	{
		set(app->LV_GenresList, MUIA_Disabled, FALSE);
		filters.showHiddenOnly = FALSE;
		showSlavesList();
	}
}

void app_stop(void)
{
	if (current_settings->save_stats_on_exit)
		save_list(0);

	memset(&fname[0], 0, sizeof fname);

	// TODO: Free the new slaves list
	if (games)
	{
		free(games);
		games = NULL;
	}
	if (repos)
	{
		free(repos);
		repos = NULL;
	}
	if (genres)
	{
		free(genres);
		genres = NULL;
	}
}

void genres_click(void)
{
	filter_change();
}

void save_list(const int check_exists)
{
	// TODO: Maybe add here a message in status
	slavesListSaveToCSV(DEFAULT_GAMESLIST_FILE);
}

// TODO: Make this work
void save_list_as(void)
{
	// TODO: Check if file exists, warn the user about overwriting it
	// if (get_filename("Save List As...", "Save", TRUE))
	// 	save_to_csv(fname, 0);
}

// TODO: Make this work
void open_list(void)
{
	if (get_filename("Open List", "Open", FALSE))
	{
		clear_gameslist();
		// load_games_list(fname);
	}
}

//function to return the dec eq of a hex no.
static int hex2dec(char* hexin)
{
	int dec;
	//lose the first $ character
	hexin++;
	sscanf(hexin, "%x", &dec);
	return dec;
}

// TODO: This seems unused and maybe needs to be removed
void game_duplicate(void)
{
	char* str = NULL;
	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &str);

	if (str == NULL || strlen(str) == 0)
	{
		msg_box((const char*)GetMBString(MSG_SelectGameFromList));
		return;
	}

	int found = 0;
	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		if (!strcmp(str, item_games->title) && item_games->deleted != 1)
		{
			found = 1;
			break;
		}
	}

	if (!found)
	{
		msg_box((const char*)GetMBString(MSG_SelectGameFromList));
		return;
	}

	char title_copy[200];
	char path_copy[256];
	char genre_copy[100];
	strcpy(title_copy, item_games->title);
	strcat(title_copy, " copy");
	strcpy(path_copy, item_games->path);
	strcpy(genre_copy, item_games->genre);

	item_games = (games_list *)calloc(1, sizeof(games_list));
	item_games->next = NULL;
	item_games->index = 0;
	item_games->exists = 0;
	item_games->deleted = 0;
	strcpy(item_games->title, title_copy);
	strcpy(item_games->genre, genre_copy);
	strcpy(item_games->path, path_copy);
	item_games->favorite = 0;
	item_games->times_played = 0;
	item_games->last_played = 0;
	item_games->hidden = 0;

	if (games == NULL)
		games = item_games;
	else
	{
		item_games->next = games;
		games = item_games;
	}

	total_games++;
	DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
	status_show_total();
}

void game_delete(void)
{
	char* str = NULL;
	LONG id = MUIV_List_NextSelected_Start;
	for (;;)
	{
		DoMethod(app->LV_GamesList, MUIM_List_NextSelected, &id);
		if (id == MUIV_List_NextSelected_End) break;

		DoMethod(app->LV_GamesList, MUIM_List_GetEntry, id, &str);
		for (item_games = games; item_games != NULL; item_games = item_games->next)
		{
			if (!strcmp(str, item_games->title))
			{
				item_games->deleted = 1;
				break;
			}
		}
		DoMethod(app->LV_GamesList, MUIM_List_Remove, id);
		total_games--;
	}
	status_show_total();
}

static LONG xget(Object* obj, ULONG attribute)
{
	LONG x;
	get(obj, attribute, &x);
	return x;
}

char* get_str(Object* obj)
{
	return (char *)xget(obj, MUIA_String_Contents);
}

// TODO: Seems unused
// BOOL get_bool(Object* obj)
// {
// 	return (BOOL)xget(obj, MUIA_Selected);
// }

static int get_cycle_index(Object* obj)
{
	int index = 0;
	get(obj, MUIA_Cycle_Active, &index);
	return index;
}

static int get_radio_index(Object* obj)
{
	int index = 0;
	get(obj, MUIA_Radio_Active, &index);
	return index;
}

void setting_filter_use_enter_changed(void)
{
	current_settings->filter_use_enter = (BOOL)xget(app->CH_FilterUseEnter, MUIA_Selected);
}

void setting_save_stats_on_exit_changed(void)
{
	current_settings->save_stats_on_exit = (BOOL)xget(app->CH_SaveStatsOnExit, MUIA_Selected);
}

void setting_smart_spaces_changed(void)
{
	current_settings->no_smart_spaces = (BOOL)xget(app->CH_SmartSpaces, MUIA_Selected);
}

void setting_titles_from_changed(void)
{
	const int index = get_radio_index(app->RA_TitlesFrom);

	// Index=0 -> Titles from Slaves
	// Index=1 -> Titles from Dirs
	current_settings->titles_from_dirs = index;

	if (index == 1)
		set(app->CH_SmartSpaces, MUIA_Disabled, FALSE);
	else
		set(app->CH_SmartSpaces, MUIA_Disabled, TRUE);
}

void setting_hide_screenshot_changed(void)
{
	current_settings->hide_screenshots = (BOOL)xget(app->CH_Screenshots, MUIA_Selected);
}

void setting_no_guigfx_changed(void)
{
	current_settings->no_guigfx = (BOOL)xget(app->CH_NoGuiGfx, MUIA_Selected);
}

void setting_screenshot_size_changed(void)
{
	const int index = get_cycle_index(app->CY_ScreenshotSize);

	// Index=0 -> 160x128
	// Index=1 -> 320x256
	// Index=2 -> Custom size

	if (index == 0)
	{
		set(app->STR_Width, MUIA_Disabled, TRUE);
		set(app->STR_Height, MUIA_Disabled, TRUE);
		current_settings->screenshot_width = 160;
		current_settings->screenshot_height = 128;
		return;
	}

	if (index == 1)
	{
		set(app->STR_Width, MUIA_Disabled, TRUE);
		set(app->STR_Height, MUIA_Disabled, TRUE);
		current_settings->screenshot_width = 320;
		current_settings->screenshot_height = 256;
		return;
	}

	if (index == 2)
	{
		set(app->STR_Width, MUIA_Disabled, FALSE);
		set(app->STR_Height, MUIA_Disabled, FALSE);
	}
}

void settings_use(void)
{
	if (current_settings == NULL)
		return;

	const int index = get_cycle_index(app->CY_ScreenshotSize);
	if (index == 2)
	{
		current_settings->screenshot_width = (int)get_str(app->STR_Width);
		current_settings->screenshot_height = (int)get_str(app->STR_Height);
	}

	set(app->WI_Settings, MUIA_Window_Open, FALSE);
}

void settings_save(void)
{
	settings_use();

	const int buffer_size = 512;
	char* file_line = malloc(buffer_size * sizeof(char));
	if (file_line == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}

	STRPTR prefsPath = AllocVec(sizeof(char) * MAX_PATH_SIZE, MEMF_CLEAR);
	if(prefsPath == NULL)
	{
		msg_box((const char*)GetMBString(MSG_NotEnoughMemory));
		return;
	}
	getPrefsPath(prefsPath, (STRPTR)DEFAULT_SETTINGS_FILE);

	const BPTR fpsettings = Open(prefsPath, MODE_NEWFILE);
	if (!fpsettings)
	{
		msg_box((const char*)"Could not save Settings file!");
		return;
	}

	snprintf(file_line, buffer_size, "no_guigfx=%d\n", current_settings->no_guigfx);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "filter_use_enter=%d\n", current_settings->filter_use_enter);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "hide_side_panel=%d\n", current_settings->hide_side_panel);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "start_with_favorites=%d\n", current_settings->start_with_favorites);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "save_stats_on_exit=%d\n", current_settings->save_stats_on_exit);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "no_smart_spaces=%d\n", current_settings->no_smart_spaces);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "titles_from_dirs=%d\n", current_settings->titles_from_dirs);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "hide_screenshots=%d\n", current_settings->hide_screenshots);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "screenshot_width=%d\n", current_settings->screenshot_width);
	FPuts(fpsettings, (CONST_STRPTR)file_line);
	snprintf(file_line, buffer_size, "screenshot_height=%d\n", current_settings->screenshot_height);
	FPuts(fpsettings, (CONST_STRPTR)file_line);

	Close(fpsettings);

	if (prefsPath)
		FreeVec(prefsPath);

	if (file_line)
		free(file_line);
}

void setting_hide_side_panel_changed(void)
{
	current_settings->hide_side_panel = (BOOL)xget(app->CH_HideSidepanel, MUIA_Selected);
}

void setting_start_with_favorites_changed(void)
{
	current_settings->start_with_favorites = (BOOL)xget(app->CH_StartWithFavorites, MUIA_Selected);
}

void msg_box(const char* msg)
{
	msgbox.es_StructSize = sizeof msgbox;
	msgbox.es_Flags = 0;
	msgbox.es_Title = (UBYTE*)GetMBString(MSG_WI_MainWindow);
	msgbox.es_TextFormat = (unsigned char*)msg;
	msgbox.es_GadgetFormat = (UBYTE*)GetMBString(MSG_BT_AboutOK);

	EasyRequest(NULL, &msgbox, NULL);
}

// TODO: This seems unused and possibly needs to be removed
void get_screen_size(int *width, int *height)
{
	struct Screen* wbscreen;

	if (IntuitionBase)
	{
		if (GfxBase)
		{
			if ((wbscreen = LockPubScreen((CONST_STRPTR)WB_PUBSCREEN_NAME)))
			{
				/* Using intuition.library/GetScreenDrawInfo(), we get the pen
				* array we'll use for the screen clone the easy way. */
				struct DrawInfo* drawinfo = GetScreenDrawInfo(wbscreen);

				struct ViewPort* vp = &wbscreen->ViewPort;
				/* Use graphics.library/GetVPModeID() to get the ModeID of the
				* Workbench screen. */
				if (GetVPModeID(vp) != (long)INVALID_ID)
				{
					*width = wbscreen->Width;
					*height = wbscreen->Height;
				}
				else
				{
					*width = -1;
					*height = -1;
				}

				FreeScreenDrawInfo(wbscreen, drawinfo);
				UnlockPubScreen(NULL, wbscreen);
			}
			else
			{
				*width = -1;
				*height = -1;
			}
		}
	}
}

void add_non_whdload(void)
{
	set(app->STR_AddTitle, MUIA_String_Contents, NULL);
	set(app->PA_AddGame, MUIA_String_Contents, NULL);
	set(app->WI_AddNonWHDLoad, MUIA_Window_Open, TRUE);
}

void non_whdload_ok(void)
{
	int genreId = 0;
	char *path, *title;
	ULONG newpos;
	slavesList *node = malloc(sizeof(slavesList));

	node->instance = 0;
	node->user_title[0] = '\0';
	node->times_played = 0;
	node->favourite = 0;
	node->last_played = 0;
	node->exists = 1;
	node->hidden = 0;
	node->deleted = 0;

	get(app->PA_AddGame, MUIA_String_Contents, &path);
	getFullPath(path, node->path);

	get(app->STR_AddTitle, MUIA_String_Contents, &title);
	strncpy(node->title, title, sizeof(node->title));

	get(app->CY_AddGameGenre, MUIA_Cycle_Active, &genreId);
	strncpy(node->genre, app->CY_PropertiesGenreContent[genreId], sizeof(node->genre));

	if (isStringEmpty(title))
	{
		msg_box((const char*)GetMBString(MSG_NoTitleSpecified));
		return;
	}
	if (isStringEmpty(path))
	{
		msg_box((const char*)GetMBString(MSG_NoExecutableSpecified));
		return;
	}

	slavesListAddTail(node);

	set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
	DoMethod(app->LV_GamesList,
		MUIM_List_InsertSingle, node->title,
		MUIV_List_Insert_Sorted);
	get(app->LV_GamesList, MUIA_List_InsertPosition, &newpos);
	set(app->LV_GamesList, MUIA_List_Active, newpos);
	set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
}

// TODO: Replaced by new non_whdload_ok() - OBSOLETE
void non_whdload_ok_OBSOLETE(void)
{
	char *str, *str_title;
	int genre = 0;

	get(app->PA_AddGame, MUIA_String_Contents, &str);
	get(app->STR_AddTitle, MUIA_String_Contents, &str_title);
	get(app->CY_AddGameGenre, MUIA_Cycle_Active, &genre);

	if (strlen(str_title) == 0)
	{
		msg_box((const char*)GetMBString(MSG_NoTitleSpecified));
		return;
	}
	if (strlen(str) == 0)
	{
		msg_box((const char*)GetMBString(MSG_NoExecutableSpecified));
		return;
	}

	//add the game to the list
	item_games = (games_list *)calloc(1, sizeof(games_list));
	item_games->next = NULL;
	item_games->index = 0;
	strcpy(item_games->title, (char *)str_title);
	// strcpy(item_games->genre, app->CY_AddGameGenreContent[genre]);
	strcpy(item_games->path, (char *)str);
	item_games->favorite = 0;
	item_games->times_played = 0;
	item_games->last_played = 0;

	if (games == NULL)
	{
		games = item_games;
	}
	else
	{
		item_games->next = games;
		games = item_games;
	}

	//todo: Small bug. If the list is showing another genre, do not insert it.
	DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
	total_games++;
	status_show_total();

	save_list(0);

	set(app->WI_AddNonWHDLoad, MUIA_Window_Open, FALSE);
}

static void joy_left(void)
{
  char *curr_game = NULL, *prev_game = NULL, *last_game = NULL;
  int ind;

  DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &curr_game);
  get(app->LV_GamesList, MUIA_List_Active, &ind);

  if (curr_game == NULL || strlen(curr_game) == 0)
    {
      set(app->LV_GamesList, MUIA_List_Active, MUIV_List_Active_Top);
      return;
    }

  DoMethod(app->LV_GamesList, MUIM_List_GetEntry, ind--, &prev_game);
  if (prev_game == NULL)
	return;

  while (toupper(curr_game[0]) == toupper(prev_game[0]))
    {
      DoMethod(app->LV_GamesList, MUIM_List_GetEntry, ind--, &prev_game);
      if (prev_game == NULL)
	return;
    }

  last_game = prev_game;

  while (toupper(last_game[0]) == toupper(prev_game[0]))
    {
      DoMethod(app->LV_GamesList, MUIM_List_GetEntry, ind--, &prev_game);
      if (prev_game == NULL)
	return;
    }

  set(app->LV_GamesList, MUIA_List_Active, ind+2);
}

static void joy_right(void)
{
  char *curr_game = NULL, *next_game = NULL;
  int ind;

  DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &curr_game);
  get(app->LV_GamesList, MUIA_List_Active, &ind);

  if (curr_game == NULL || strlen(curr_game) == 0)
    {
      set(app->LV_GamesList, MUIA_List_Active, MUIV_List_Active_Top);
      return;
    }

  DoMethod(app->LV_GamesList, MUIM_List_GetEntry, ind++, &next_game);
  if (next_game == NULL)
	return;

  while (toupper(curr_game[0]) == toupper(next_game[0]))
    {
      DoMethod(app->LV_GamesList, MUIM_List_GetEntry, ind++, &next_game);
      if (next_game == NULL)
	return;
    }

  set(app->LV_GamesList, MUIA_List_Active, ind-1);

}

ULONG get_wb_version(void)
{
	static ULONG ver = 0UL;

	if (ver != 0UL)
	{
		return ver;
	}

	if (WorkbenchBase == NULL)
	{
		//Really Workbench library should be already opened
		// Somehow we're running without Workbench.
		// Nothing to do since the version variable inits with 0.
		return ver;
	}

	// Save workbench.library version for later calls
	ver = WorkbenchBase->lib_Version;

	return ver;
}


static void joystick_buttons(ULONG val)
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

static void joystick_directions(ULONG val)
{
	if (val & JPF_JOY_UP)
		set(app->LV_GamesList, MUIA_List_Active, MUIV_List_Active_Up);

	if (val & JPF_JOY_DOWN)
		set(app->LV_GamesList, MUIA_List_Active, MUIV_List_Active_Down);

	if (val & JPF_JOY_LEFT)
	  joy_left();

	if (val & JPF_JOY_RIGHT)
	  joy_right();
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
