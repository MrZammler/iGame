/*
  funcs.c
  Misc functions for iGame
  
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

#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>
#include <clib/icon_protos.h>
#include <libraries/mui.h>
#include <MUI/Guigfx_mcc.h>
#include <MUI/TextEditor_mcc.h>
#include <clib/muimaster_protos.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <exec/memory.h>
#include <workbench/startup.h>
#include <exec/types.h>
#include <workbench/workbench.h>
#include <clib/graphics_protos.h>

#include "iGameGUI.h"

#define MUIA_Dtpic_Name 0x80423d72
#define TEMPLATE 	"SCREENSHOT/K"

extern char* strdup(const char* s);
extern struct ObjApp* app;
extern char* executable_name;

/* global variables */
int global_filter_check = 0;
int total_hidden = 0;
int showing_hidden = 0;
char pathstr[512];
int total_games;
int no_of_genres;
char gamestooltypes[1024];
int SS_WIDTH, SS_HEIGHT;
int NOGUIGFX;
int FILTERUSEENTER;
int NOSCREENSHOT;
int SAVESTATSONEXIT;
int TITLESFROMDIRS;
int IntroPic = 0;

/* function definitions */
char** my_split(char* str, char* spl);
int get_genre(char* title, char* genre);
void get_path(char* title, char* path);
void followthread(BPTR lock, int tab_level);
void refresh_list(int check_exists);
void save_list(int check_exists);
int hex2dec(char* hexin);
void msg_box(char* msg);
int get_title_from_slave(char* slave, char* title);
int check_dup_title(char* title);
int get_delimiter_position(const char* str);
char* get_directory_name(char* str);
char* get_executable_name(int argc, char** argv);

/* structures */
struct EasyStruct msgbox;

typedef struct games
{
	char title[200];
	char genre[30];
	int index;
	char path[256];
	int favorite;
	int times_played;
	int last_played; //indicates whether this one was the last game played
	int exists; //indicates whether this game still exists after a scan
	int hidden; //game is hidden from normal operation
	struct games* next;
} games_list;

games_list *item_games = NULL, *games = NULL;

typedef struct repos
{
	char repo[256];
	struct repos* next;
} repos_list;

repos_list *item_repos = NULL, *repos = NULL;

typedef struct genres
{
	char genre[256];
	struct genres* next;
} genres_list;

genres_list *item_genres = NULL, *genres = NULL;

void load_games_list()
{
	char file_line[500];
	char helperstr[250];

	FILE* fpgames = fopen("PROGDIR:gameslist", "r");
	if (fpgames)
	{
		do
		{
			if (fgets(file_line, sizeof file_line, fpgames) == NULL) { break; }

			file_line[strlen(file_line) - 1] = '\0';

			if (strlen(file_line) == 0) continue;

			item_games = (games_list *)calloc(1, sizeof(games_list));
			item_games->next = NULL;

			if (!strcmp((char *)file_line, "index=0"))
			{
				item_games->index = 0;
				item_games->exists = 0;

				do
				{
					if (fgets(file_line, sizeof file_line, fpgames) == NULL) { break; }

					file_line[strlen(file_line) - 1] = '\0';

					if (strlen(file_line) == 0) break;

					//this is to make sure that gameslist goes ok from 1.2 to 1.3
					item_games->hidden = 0;

					if (!strncmp(file_line, "title=", 6))
						strcpy(item_games->title, file_line + 6);
					else if (!strncmp(file_line, "genre=", 6))
						strcpy(item_games->genre, file_line + 6);
					else if (!strncmp(file_line, "path=", 5))
						strcpy(item_games->path, file_line + 5);
					else if (!strncmp(file_line, "favorite=", 9))
						item_games->favorite = atoi(file_line + 9);
					else if (!strncmp(file_line, "timesplayed=", 12))
						item_games->times_played = atoi(file_line + 12);
					else if (!strncmp(file_line, "lastplayed=", 11))
						item_games->last_played = atoi(file_line + 11);
					else if (!strncmp(file_line, "hidden=", 7))
						item_games->hidden = atoi(file_line + 7);
				}
				while (1);

				if (games == NULL)
				{
					games = item_games;
				}
				else
				{
					item_games->next = games;
					games = item_games;
				}
			}
		}
		while (1); //read of gameslist ends here

		//add the games to the list
		total_games = 0;
		for (item_games = games; item_games != NULL; item_games = item_games->next)
		{
			if (item_games->hidden != 1)
			{
				total_games++;
				DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
			}
		}
		sprintf(helperstr, "Total %d games.", total_games);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);

		fclose(fpgames);
	}
}

void load_repos()
{
	char file_line[500];

	FILE* fprepos = fopen("PROGDIR:repos.prefs", "r");
	if (fprepos)
	{
		do
		{
			if (fgets(file_line, sizeof file_line, fprepos) == NULL) { break; }

			file_line[strlen(file_line) - 1] = '\0';

			if (strlen(file_line) == 0) continue;

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

			DoMethod(app->LV_RepoList, MUIM_List_InsertSingle, item_repos->repo, 1, MUIV_List_Insert_Bottom);
		}
		while (1);

		fclose(fprepos);
	}
}

void load_genres()
{
	char file_line[500];
	int i;
	FILE* fpgenres = fopen("PROGDIR:genres", "r");
	if (fpgenres)
	{
		no_of_genres = 0;
		do
		{
			if (fgets(file_line, sizeof file_line, fpgenres) == NULL) { break; }

			file_line[strlen(file_line) - 1] = '\0';

			if (strlen(file_line) == 0) continue;

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
		while (1);

		for (i = 0; i < no_of_genres; i++)
		{
			DoMethod(app->LV_GenresList, MUIM_List_GetEntry, i + 5, &app->CY_Genre_Content[i]);
		}

		app->CY_Genre_Content[i] = "Unknown";
		app->CY_Genre_Content[i + 1] = NULL;

		set(app->CY_Genre, MUIA_Cycle_Entries, app->CY_Genre_Content);

		fclose(fpgenres);
	}
}

void AppStart()
{
	load_games_list();
	load_repos();

	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, "--Show All--", MUIV_List_Insert_Bottom);
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, "--Favorites--", MUIV_List_Insert_Bottom);
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, "--Last Played--", MUIV_List_Insert_Bottom);
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, "--Most Played--", MUIV_List_Insert_Bottom);
	DoMethod(app->LV_GenresList, MUIM_List_InsertSingle, "--Never Played--", MUIV_List_Insert_Bottom);

	load_genres();

	IntroPic = 1;

	set(app->WI_MainWindow,
		MUIA_Window_Open, TRUE
	);
}

void FilterChange()
{
	char *str = NULL, helper[210];
	char *str_gen = NULL, helperstr[200];
	int i, max = 0, _strlen;

	get(app->STR_Filter, MUIA_String_Contents, &str);

	DoMethod(app->LV_GenresList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &str_gen);
	if (str && strlen(str) != 0) for (i = 0; i <= strlen((char *)str) - 1; i++) str[i] = tolower(str[i]);

	if (str_gen == NULL || !strcmp(str_gen, "--Show All--"))
	{
		// Erase list
		DoMethod(app->LV_GamesList, MUIM_List_Clear);

		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);

		total_games = 0;
		// Find the entries in Games and update the list
		if (games)
		{
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				strcpy(helper, item_games->title);
				_strlen = strlen(helper);
				for (i = 0; i <= _strlen - 1; i++) helper[i] = tolower(helper[i]);

				if (strstr(helper, (char *)str))
				{
					if (item_games->hidden != 1)
					{
						DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title,
						         MUIV_List_Insert_Sorted);
						total_games++;
					}
				}
			}
		}

		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);

		sprintf(helper, "Total %d games.", total_games);
		set(app->TX_Status, MUIA_Text_Contents, helper);
	}

	else if (!strcmp(str_gen, "--Favorites--"))
	{
		// Erase list
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);

		total_games = 0;
		if (games)
		{
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				strcpy(helper, item_games->title);
				_strlen = strlen(helper);
				for (i = 0; i <= _strlen - 1; i++) helper[i] = tolower(helper[i]);

				if (item_games->favorite == 1 && item_games->hidden != 1 && strstr(helper, (char *)str))
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
					total_games++;
				}
			}
		}

		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);

		sprintf(helperstr, "Total %d games.", total_games);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
	else if (!strcmp(str_gen, "--Last Played--"))
	{
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		total_games = 0;
		if (games)
		{
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				strcpy(helper, item_games->title);
				_strlen = strlen(helper);
				for (i = 0; i <= _strlen - 1; i++) helper[i] = tolower(helper[i]);

				if (item_games->last_played == 1 && strstr(helper, (char *)str))
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
					total_games++;
				}
			}
		}

		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);

		sprintf(helperstr, "Total %d games.", total_games);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
	else if (!strcmp(str_gen, "--Most Played--"))
	{
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		total_games = 0;
		if (games)
		{
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				strcpy(helper, item_games->title);
				_strlen = strlen(helper);
				for (i = 0; i <= _strlen - 1; i++) helper[i] = tolower(helper[i]);

				if (item_games->times_played && strstr(helper, (char *)str))
				{
					if (item_games->times_played >= max && item_games->hidden != 1)
					{
						max = item_games->times_played;
						//sprintf(helper, "%ld - %s", item_games->TimesPlayed, item_games->Title);
						DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Top);
						total_games++;
						//printf("%s - Top max:%d\n", item_games->Title, max);
					}
					else
					{
						//sprintf(helper, "%ld - %s", item_games->TimesPlayed, item_games->Title);
						DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title,
						         MUIV_List_Insert_Bottom);
						total_games++;
						//printf("%s - Bottom max:%d\n", item_games->Title, max);
					}
				}
			}
		}
		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
		sprintf(helperstr, "Total %d games.", total_games);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
	else if (!strcmp(str_gen, "--Never Played--"))
	{
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		total_games = 0;
		if (games)
		{
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				strcpy(helper, item_games->title);
				_strlen = strlen(helper);
				for (i = 0; i <= _strlen - 1; i++) helper[i] = tolower(helper[i]);

				if (item_games->times_played == 0 && item_games->hidden != 1 && strstr(helper, (char *)str))
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
					total_games++;
				}
			}
		}

		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);

		sprintf(helperstr, "Total %d games.", total_games);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
		// the
	else
	{
		// Erase list
		DoMethod(app->LV_GamesList, MUIM_List_Clear);

		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		//if (strlen(str)>0) for (i=0;i<=strlen((char *)str)-1;i++) str[i] = tolower(str[i]);
		total_games = 0;
		// Find the entries in Games and update the list
		if (games)
		{
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				strcpy(helper, item_games->title);
				_strlen = strlen(helper);
				for (i = 0; i <= _strlen - 1; i++) helper[i] = tolower(helper[i]);

				if (!strcmp(item_games->genre, str_gen) && item_games->hidden != 1 && strstr(helper, (char *)str))
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
					total_games++;
				}
			}
		}

		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);

		sprintf(helper, "Total %d games.", total_games);
		set(app->TX_Status, MUIA_Text_Contents, helper);
	}
}

/*
*   Executes whdload with the slave
*/
void game_double_click()
{
	struct Library* icon_base;
	struct DiskObject* disk_obj;
	char *game_title = NULL, path[256], exec[256], naked_path[256], *tool_type;
	int success, i, k, whdload = 0;
	int z = 0;
	char str2[512], fullpath[800], slave[256], helperstr[250], to_check[256];

	//clear vars:
	str2[0] = '\0';
	fullpath[0] = '\0';

	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &game_title);
	get_path((char *)game_title, path);

	sprintf(helperstr, "Running %s...", game_title);
	set(app->TX_Status, MUIA_Text_Contents, helperstr);

	/* strip the path from the slave file and get the rest */
	for (i = strlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '/')
			break;
	}
	for (k = 0; k <= i - 1; k++)
		naked_path[k] = path[k];
	naked_path[k] = '\0';

	int length = strlen(path);
	for (k = i + 1; k <=  length - 1; k++)
	{
		slave[z] = path[k];
		z++;
	}

	slave[z] = '\0';
	 length = strlen(slave);
	for (i = 0; i <=  length - 1; i++) slave[i] = tolower(slave[i]);

	if (strstr(slave, ".slave"))
	{
		whdload = 1;
	}

	const BPTR oldlock = Lock("PROGDIR:", ACCESS_READ);
	const BPTR lock = Lock(naked_path, ACCESS_READ);
	CurrentDir(lock);

	if (whdload == 1) sprintf(exec, "whdload ");
	else strcpy(exec, slave);

	//tooltypes only for whdload games
	if (whdload == 1)
	{
		if (icon_base = (struct Library *)OpenLibrary("icon.library", 0))
		{
			//scan the .info files in the current dir.
			//one of them should be the game's project icon.

			/*  allocate space for a FileInfoBlock */
			struct FileInfoBlock* m = (struct FileInfoBlock *)AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);

			success = Examine(lock, m);
			if (m->fib_DirEntryType <= 0)
			{
				/*  We don't allow "opta file", only "opta dir" */
				return;
			}

			while (success = ExNext(lock, m))
			{
				if (strstr(m->fib_FileName, ".info"))
				{
					NameFromLock(lock, str2, 512);
					sprintf(fullpath, "%s/%s", str2, m->fib_FileName);

					//lose the .info
					for (i = strlen(fullpath) - 1; i >= 0; i--)
					{
						if (fullpath[i] == '.')
							break;
					}
					fullpath[i] = '\0';

					if (disk_obj = (struct DiskObject *)GetDiskObject(fullpath))
					{
						if (MatchToolValue(FindToolType(disk_obj->do_ToolTypes, "SLAVE"), slave) || MatchToolValue(
							FindToolType(disk_obj->do_ToolTypes, "slave"), slave))
						{
						}
						else
						{
							FreeDiskObject(disk_obj);
							continue;
						}

						for (char **tool_types = (char **)disk_obj->do_ToolTypes; tool_type = *tool_types; ++tool_types)
						{
							if (!strncmp(tool_type, "IM", 2)) continue;
							if (tool_type[0] == ' ') continue;
							if (tool_type[0] == '(') continue;
							if (tool_type[0] == '*') continue;
							if (tool_type[0] == ';') continue;
							if (tool_type[0] == '\0') continue;

							/* Must check here for numerical values */
							/* Those (starting with $ should be transformed to dec from hex) */
							char **temp_tbl = my_split((char *)(tool_type), "=");
							if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp((char *)temp_tbl[0], " ") || !strcmp(
								(char *)temp_tbl[0], ""))
							{
								continue;
							}

							if (temp_tbl[1] != NULL)
							{
								sprintf(to_check, "%s", temp_tbl[1]);
								if (to_check[0] == '$')
								{
									int dec_rep = hex2dec(to_check);
									sprintf(tool_type, "%s=%d", temp_tbl[0], dec_rep);
								}
							}

							free(temp_tbl);
							//req: more free's

							sprintf(exec, "%s %s", exec, tool_type);
						}

						FreeDiskObject(disk_obj);

						break;
					}
				}
			}

			CloseLibrary(icon_base);
		}

		//if we're still here, and exec contains just whdload, add the slave and execute
		if (!strcmp(exec, "whdload "))
		{
			sprintf(exec, "%s %s", exec, slave);
		}
	}

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

	if (SAVESTATSONEXIT == 0) save_list(0);
	success = Execute(exec, 0, 0);

	if (success == 0) msg_box("Error while executing WHDLoad or Game.\nPlease make sure WHDLoad is in your path.");

	CurrentDir(oldlock);

	sprintf(helperstr, "Total %d games.", total_games);
	set(app->TX_Status, MUIA_Text_Contents, helperstr);
}

/*
* Scans the repos for games
*/
void menu_scan()
{
	char repotemp[256], helperstr[256];

	if (!repos)
	{
		//printf("Nothing to scan!\n");
	}
	else
	{
		for (item_games = games; item_games != NULL; item_games = item_games->next)
		{
			//only apply the not exists hack to slaves that are in the current repos, that will be scanned later
			//Binaries (that are added through add-non-whdload, should be handled afterwards
			if (strstr(item_games->path, ".slave") || strlen(item_games->path) == 0)
			{
				item_games->exists = 0;
			}
			else
			{
				item_games->exists = 1;
			}
		}

		BPTR currentlock = Lock("PROGDIR:", ACCESS_READ);

		for (item_repos = repos; item_repos != NULL; item_repos = item_repos->next)
		{
			sprintf(repotemp, "%s", item_repos->repo);
			sprintf(helperstr, "Scanning [%s]. Please wait...", repotemp);
			set(app->TX_Status, MUIA_Text_Contents, helperstr);
			BPTR oldlock = Lock(repotemp, ACCESS_READ);

			if (oldlock != 0)
			{
				CurrentDir(oldlock);
				followthread(oldlock, 0);
				CurrentDir(currentlock);
			}
			else
			{
				//could not lock
			}
		}

		save_list(1);
		refresh_list(1);
	}
}

void game_click()
{
	char *game_title = NULL, path[350], naked_path[400];
	int i, k;
	FILE* fp = NULL;

	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &game_title);

	if (!NOSCREENSHOT && game_title) //for some reason, game_click is called and game_title is null??
	{
		get_path((char *)game_title, path);

		/* strip the path from the slave file and get the rest */
		for (i = strlen(path) - 1; i >= 0; i--)
		{
			if (path[i] == '/')
				break;
		}

		for (k = 0; k <= i - 1; k++)
			naked_path[k] = path[k];
		naked_path[k] = '\0';

		//Check the string, when filter is populated there is trouble
		if (strlen(naked_path) != 0)
		{
			sprintf(naked_path, "%s/igame.iff", naked_path);

			fp = fopen(naked_path, "r");
			if (!fp) //no igame.iff, try .info and newicons
			{
				if (strstr(path, ".slave")) //check for whdload game
				{
					path[strlen(path) - 6] = '\0';
					sprintf(naked_path, "%s.info", path);
					fp = fopen(naked_path, "r");
				}
			}

			if (fp)
			{
				if (NOGUIGFX)
				{
					app->IM_GameImage_1 = MUI_NewObject("Dtpic.mui",
					                                    MUIA_Dtpic_Name, naked_path,
					                                    MUIA_Frame, MUIV_Frame_ImageButton,
					End;
				}
				else
				{
					app->IM_GameImage_1 = GuigfxObject,
					                                  MUIA_Guigfx_FileName, naked_path,
					                                  MUIA_Guigfx_Quality, MUIV_Guigfx_Quality_Best,
					                                  MUIA_Guigfx_ScaleMode, NISMF_SCALEFREE,
					                                  MUIA_Guigfx_Transparency, 0,
					                                  MUIA_Frame, MUIV_Frame_ImageButton,
					                                  MUIA_FixHeight, SS_HEIGHT,
					                                  MUIA_FixWidth, SS_WIDTH,
					End;
				}

				if (app->IM_GameImage_1)
				{
					DoMethod(app->GR_grp_1, MUIM_Group_InitChange);
					DoMethod(app->GR_grp_1, OM_REMMEMBER, app->IM_GameImage_0);
					DoMethod(app->GR_grp_1, OM_REMMEMBER, app->LV_GenresList);
					MUI_DisposeObject(app->IM_GameImage_0);
					DoMethod(app->GR_grp_1, OM_ADDMEMBER, app->IM_GameImage_0 = app->IM_GameImage_1);
					DoMethod(app->GR_grp_1, OM_ADDMEMBER, app->LV_GenresList);
					DoMethod(app->GR_grp_1, MUIM_Group_ExitChange);
					IntroPic = 0;
				}
				else
					//in case it failed to load something, lets hope it gets picked up here and is forced to load the default igame.iff
				{
					goto loaddef;
				}
			}
			else //no pic found
			{
			loaddef:
				if (IntroPic == 0)
				{
					if (NOGUIGFX)
					{
						app->IM_GameImage_1 = (Object *)MUI_NewObject("Dtpic.mui",
						                                              MUIA_Dtpic_Name, "PROGDIR:igame.iff",
						                                              MUIA_Frame, MUIV_Frame_ImageButton,
						End;
					}
					else
					{
						app->IM_GameImage_1 = GuigfxObject,
						                                  MUIA_Guigfx_FileName, "PROGDIR:igame.iff",
						                                  MUIA_Guigfx_Quality, MUIV_Guigfx_Quality_Best,
						                                  MUIA_Guigfx_ScaleMode, NISMF_SCALEFREE,
						                                  MUIA_Guigfx_Transparency, 0,
						                                  MUIA_Frame, MUIV_Frame_ImageButton,
						                                  MUIA_FixHeight, SS_HEIGHT,
						                                  MUIA_FixWidth, SS_WIDTH,
						End;
					}
					if (app->IM_GameImage_1)
					{
						DoMethod(app->GR_grp_1, MUIM_Group_InitChange);
						DoMethod(app->GR_grp_1, OM_REMMEMBER, app->IM_GameImage_0);
						DoMethod(app->GR_grp_1, OM_REMMEMBER, app->LV_GenresList);
						MUI_DisposeObject(app->IM_GameImage_0);
						DoMethod(app->GR_grp_1, OM_ADDMEMBER, app->IM_GameImage_0 = app->IM_GameImage_1);
						DoMethod(app->GR_grp_1, OM_ADDMEMBER, app->LV_GenresList);
						DoMethod(app->GR_grp_1, MUIM_Group_ExitChange);
						IntroPic = 1;
					}
				}
			}
		}

		if (fp) fclose(fp);
	}
}

/* Retrieves the Genre from the file, using the Title */
int get_genre(char* title, char* genre)
{
	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		if (!strcmp(title, item_games->title))
		{
			strcpy(genre, item_games->genre);
			break;
		}
	}

	return 0;
}

void get_path(char* title, char* path)
{
	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		if (!strcmp(title, item_games->title))
		{
			strcpy(path, item_games->path);
			break;
		}
	}
}

/*
* Adds a repository (path on the disk)
* to the list of repositories
*/
void repo_add()
{
	char* str = NULL;

	get(app->PA_RepoPath, MUIA_String_Contents, &str);

	if (str && strlen((char *)str) != 0)
	{
		item_repos = (repos_list *)calloc(1, sizeof(repos_list));
		item_repos->next = NULL;

		strcpy(item_repos->repo, (char *)str);

		if (repos == NULL)
		{
			repos = item_repos;
		}
		else
		{
			item_repos->next = repos;
			repos = item_repos;
		}

		DoMethod(app->LV_RepoList, MUIM_List_InsertSingle, item_repos->repo, 1, MUIV_List_Insert_Bottom);
	}
}

void repo_remove()
{
	DoMethod(app->LV_RepoList, MUIM_List_Remove, MUIV_List_Remove_Active);
}

/*
* Writes the repositories to the repo.prefs file
*/
void repo_stop()
{
	FILE* fprepos = fopen("PROGDIR:repos.prefs", "w");
	if (!fprepos)
	{
		msg_box("Could not create repos.prefs file!");
	}
	else
	{
		for (int i = 0;; i++)
		{
			char* str = NULL;
			DoMethod(app->LV_RepoList, MUIM_List_GetEntry, i, &str);
			if (!str) break;
			//printf("%d - %s\n", i, str);
			fprintf(fprepos, "%s\n", (char *)str);
			fflush(fprepos);
		}
		fclose(fprepos);
	}
}

//shows and inits the GameProperties Window
void menu_game_properties()
{
	char* str = NULL;
	char helperstr[512];
	char path[512];
	int i, found = 0;

	struct Library* icon_base;
	struct DiskObject* disk_obj;
	char naked_path[256], *tool_type;
	int k, open;
	int z = 0;
	char str2[512], fullpath[800], slave[256];

	//if window is already open
	get(app->WI_GameProperties, MUIA_Window_Open, &open);
	if (open) return;

	//set the elements on the window
	DoMethod(app->LV_GamesList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &str);

	if (str == NULL || strlen(str) == 0)
	{
		msg_box("Please select a Game from the List.");
		return;
	}

	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		if (!strcmp(str, item_games->title))
		{
			found = 1;
			break;
		}
	}

	if (found == 1)
	{
		set(app->STR_GameTitle, MUIA_String_Contents, str);
		set(app->TX_GamePath, MUIA_Text_Contents, item_games->path);

		sprintf(helperstr, "Times Played: %d", item_games->times_played);
		set(app->TX_NumPlayed, MUIA_Text_Contents, helperstr);

		//set the genre
		for (i = 0; i < no_of_genres; i++)
			if (!strcmp(app->CY_Genre_Content[i], item_games->genre)) break;
		set(app->CY_Genre, MUIA_Cycle_Active, i);

		if (item_games->favorite == 1)
			set(app->CH_Favorite, MUIA_Selected, TRUE);
		else
			set(app->CH_Favorite, MUIA_Selected, FALSE);

		if (item_games->hidden == 1)
			set(app->CH_Hidden, MUIA_Selected, TRUE);
		else
			set(app->CH_Hidden, MUIA_Selected, FALSE);

		//set up the tooltypes

		get_path((char *)str, path);

		/* strip the path from the slave file and get the rest */
		for (i = strlen(path) - 1; i >= 0; i--)
		{
			if (path[i] == '/')
				break;
		}
		for (k = 0; k <= i - 1; k++)
			naked_path[k] = path[k];
		naked_path[k] = '\0';

		for (k = i + 1; k <= strlen(path) - 1; k++)
		{
			slave[z] = path[k];
			z++;
		}
		slave[z] = '\0';

		for (i = 0; i <= strlen(slave) - 1; i++) slave[i] = tolower(slave[i]);

		BPTR oldlock = Lock("PROGDIR:", ACCESS_READ);
		BPTR lock = Lock(naked_path, ACCESS_READ);
		CurrentDir(lock);

		gamestooltypes[0] = '\0';

		if (icon_base = (struct Library *)OpenLibrary("icon.library", 0))
		{
			//scan the .info files in the current dir.
			//one of them should be the game's project icon.

			/*  allocate space for a FileInfoBlock */
			struct FileInfoBlock * m = (struct FileInfoBlock *)AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);

			int success = Examine(lock, m);
			if (m->fib_DirEntryType <= 0)
			{
				/*  We don't allow "opta file", only "opta dir" */
				return;
			}

			while (success = ExNext(lock, m))
			{
				if (strstr(m->fib_FileName, ".info"))
				{
					NameFromLock(lock, str2, 512);
					sprintf(fullpath, "%s/%s", str2, m->fib_FileName);

					//lose the .info
					for (i = strlen(fullpath) - 1; i >= 0; i--)
					{
						if (fullpath[i] == '.')
							break;
					}
					fullpath[i] = '\0';

					if (disk_obj = GetDiskObject(fullpath))
					{
						//printf("trying: [%s]\n", fullpath);
						if (MatchToolValue(FindToolType(disk_obj->do_ToolTypes, "SLAVE"), slave) || MatchToolValue(
							FindToolType(disk_obj->do_ToolTypes, "slave"), slave))
						{
							//printf("Winner!\n");
						}
						else
						{
							FreeDiskObject(disk_obj);
							continue;
						}

						for (char **tool_types = (char **)disk_obj->do_ToolTypes; tool_type = *tool_types; ++tool_types)
						{
							if (!strncmp(tool_type, "IM", 2)) continue;

							sprintf(gamestooltypes, "%s%s\n", gamestooltypes, tool_type);
							set(app->TX_Tooltypes, MUIA_TextEditor_ReadOnly, FALSE);
							//printf("[%s]\n", tool_type);
						}
						//printf("%s\n", disk_obj->do_ToolTypes);

						FreeDiskObject(disk_obj);

						break;
					}
				}
			}

			CloseLibrary(icon_base);
		}

		if (strlen(gamestooltypes) == 0)
		{
			sprintf(gamestooltypes, "No .info file");
			set(app->TX_Tooltypes, MUIA_TextEditor_ReadOnly, TRUE);
		}

		set(app->TX_Tooltypes, MUIA_TextEditor_Contents, gamestooltypes);
		//set(App->TX_Tooltypes, MUIA_Floattext_Text, helperstr);
		CurrentDir(oldlock);

		set(app->WI_GameProperties, MUIA_Window_Open, TRUE);
	}
	else
	{
		msg_box("Please select a Game from the List.");
	}
}

//when ok is pressed in GameProperties
void game_properties_ok()
{
	char* str = NULL;
	char *path = NULL, helperstr[256];
	int fav = 0, genre = 0, hid = 0;
	struct Library* icon_base;
	struct DiskObject* disk_obj;
	char naked_path[256], *tool_type;
	int i, k;
	int new_tool_type_count = 1, old_tool_type_count = 0, old_real_tool_type_count = 0;
	char str2[512], fullpath[800], slave[256];

	get(app->STR_GameTitle, MUIA_String_Contents, &str);
	get(app->TX_GamePath, MUIA_Text_Contents, &path);
	get(app->CY_Genre, MUIA_Cycle_Active, &genre);
	get(app->CH_Favorite, MUIA_Selected, &fav);
	get(app->CH_Hidden, MUIA_Selected, &hid);

	//printf("%s\n", App->CY_Genre_Content[genre]);

	//find the entry, and update it:
	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		if (!strcmp(path, item_games->path))
		{
			if (strcmp(item_games->title, str))
			{
				//check dup for title
				if (check_dup_title(str))
				{
					msg_box("The Title you selected, already exists.");
					return;
				}
			}
			strcpy(item_games->title, str);
			strcpy(item_games->genre, app->CY_Genre_Content[genre]);
			if (fav == 1) item_games->favorite = 1;
			else item_games->favorite = 0;

			//if it was previously not hidden, hide now
			if (hid == 1 && item_games->hidden != 1)
			{
				item_games->hidden = 1;
				DoMethod(app->LV_GamesList, MUIM_List_Remove, MUIV_List_Remove_Selected);
				total_games = total_games - 1;
				sprintf(helperstr, "Total %d games.", total_games);
				set(app->TX_Status, MUIA_Text_Contents, helperstr);
			}

			if (hid == 0 && item_games->hidden == 1)
			{
				item_games->hidden = 0;
				DoMethod(app->LV_GamesList, MUIM_List_Remove, MUIV_List_Remove_Selected);
				total_hidden--;
				sprintf(helperstr, "Total %d games.", total_hidden);
				set(app->TX_Status, MUIA_Text_Contents, helperstr);
			}
			//RefreshList();
			//SaveList();
			break;
		}
	}

	//update the games tooltypes
	STRPTR tools = (STRPTR)DoMethod(app->TX_Tooltypes, MUIM_TextEditor_ExportText);

	//tooltypes changed
	if (strcmp((char *)tools, gamestooltypes))
	{
		/* strip the path from the slave file and get the rest */
		for (i = strlen(path) - 1; i >= 0; i--)
		{
			if (path[i] == '/')
				break;
		}
		for (k = 0; k <= i - 1; k++)
			naked_path[k] = path[k];
		naked_path[k] = '\0';

		int z = 0;
		for (k = i + 1; k <= strlen(path) - 1; k++)
		{
			slave[z] = path[k];
			z++;
		}
		slave[z] = '\0';

		for (i = 0; i <= strlen(slave) - 1; i++) slave[i] = tolower(slave[i]);

		BPTR oldlock = Lock("PROGDIR:", ACCESS_READ);
		BPTR lock = Lock(naked_path, ACCESS_READ);
		CurrentDir(lock);

		if (icon_base = (struct Library *)OpenLibrary("icon.library", 0))
		{
			//scan the .info files in the current dir.
			//one of them should be the game's project icon.

			/*  allocate space for a FileInfoBlock */
			struct FileInfoBlock * m = (struct FileInfoBlock *)AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);

			int success = Examine(lock, m);
			if (m->fib_DirEntryType <= 0)
			{
				/*  We don't allow "opta file", only "opta dir" */
				return;
			}

			while (success = ExNext(lock, m))
			{
				if (strstr(m->fib_FileName, ".info"))
				{
					NameFromLock(lock, str2, 512);
					sprintf(fullpath, "%s/%s", str2, m->fib_FileName);
					//printf("%s\n", fullpath);

					//lose the .info
					for (i = strlen(fullpath) - 1; i >= 0; i--)
					{
						if (fullpath[i] == '.')
							break;
					}
					fullpath[i] = '\0';

					//printf("Without info: [%s]\n", fullpath);

					if (disk_obj = (struct DiskObject *)GetDiskObject(fullpath))
					{
						//						  printf("trying: [%s]\n", fullpath);
						if (MatchToolValue(FindToolType(disk_obj->do_ToolTypes, "SLAVE"), slave) || MatchToolValue(
							FindToolType(disk_obj->do_ToolTypes, "slave"), slave))
						{
							//							  printf("Winner!\n");
						}
						else
						{
							FreeDiskObject(disk_obj);
							continue;
						}

						//check numbers for old and new tooltypes
						for (i = 0; i <= strlen(tools); i++)
						{
							if (tools[i] == '\n') new_tool_type_count++;
						}
						//add one for the last tooltype that doesnt end with \n
						new_tool_type_count++;

						for (i = 0; i <= strlen(gamestooltypes); i++)
						{
							if (gamestooltypes[i] == '\n') old_tool_type_count++;
						}

						for (char **tool_types = (char **)disk_obj->do_ToolTypes; tool_type = *tool_types; ++tool_types)
						{
							old_real_tool_type_count++;
						}

						unsigned char **new_tool_types = AllocVec(new_tool_type_count * sizeof(char *), MEMF_FAST | MEMF_CLEAR);
						unsigned char **newptr = new_tool_types;

						char **temp_tbl = my_split((char *)tools, "\n");
						if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp((char *)temp_tbl[0], " ") || !strcmp((
							unsigned char *)temp_tbl[0], ""))
						{
							break;
						}

						for (i = 0; i <= new_tool_type_count - 2; i++)
						{
							*newptr++ = (unsigned char*)temp_tbl[i];
						}

						*newptr = NULL;

						disk_obj->do_ToolTypes = new_tool_types;
						PutDiskObject(fullpath, disk_obj);
						FreeDiskObject(disk_obj);

						break;
					}
				}
			}

			CloseLibrary(icon_base);
		}
	}
	FreeVec(tools);

	DoMethod(app->LV_GamesList, MUIM_List_Redraw, MUIV_List_Redraw_Active);

	set(app->WI_GameProperties, MUIA_Window_Open, FALSE);
	//RefreshList();
	save_list(0);
}

void menu_show_hidden()
{
	char helperstr[50];

	if (showing_hidden == 0)
	{
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		total_hidden = 0;
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		//DoMethod(App->LV_GenresList,MUIM_List_Select,1,MUIV_List_Select_Off, NULL);
		set(app->LV_GenresList, MUIA_Disabled, TRUE);
		set(app->STR_Filter, MUIA_Disabled, TRUE);
		if (games)
		{
			/* Find the entries in Games and update the list */
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				if (item_games->hidden == 1)
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
					total_hidden++;
				}
			}
		}

		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);

		sprintf(helperstr, "Total %d games.", total_hidden);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);

		showing_hidden = 1;
	}
	else
	{
		set(app->LV_GenresList, MUIA_Disabled, FALSE);
		set(app->STR_Filter, MUIA_Disabled, FALSE);
		showing_hidden = 0;
		refresh_list(0);
	}
}

void app_stop()
{
	free(games);
	free(repos);
	free(genres);

	if (SAVESTATSONEXIT == 1) save_list(0);
}

void genres_click()
{
	char *str = NULL, helperstr[50];
	int max = 0, mini_totals = 0;

	DoMethod(app->LV_GenresList, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &str);

	if (!strcmp(str, "--Show All--"))
		refresh_list(0);
	else if (!strcmp(str, "--Favorites--"))
	{
		/* Erase list */
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		//DoMethod(App->LV_GamesList,MUIM_Hide);
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		if (games)
		{
			/* Find the entries in Games and update the list */
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				if (item_games->favorite == 1 && item_games->hidden != 1)
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
					mini_totals++;
				}
			}
		}

		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);

		sprintf(helperstr, "Total %d games.", mini_totals);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
	else if (!strcmp(str, "--Last Played--"))
	{
		/* Erase list */
		DoMethod(app->LV_GamesList, MUIM_List_Clear);

		if (games)
		{
			/* Find the entries in Games and update the list */
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				if (item_games->last_played == 1)
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
					break;
				}
			}
		}
		sprintf(helperstr, "Total 1 games.");
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
	else if (!strcmp(str, "--Most Played--"))
	{
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		//DoMethod(App->LV_GamesList,MUIM_Hide);
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		if (games)
		{
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				if (item_games->times_played)
				{
					if (item_games->times_played >= max && item_games->hidden != 1)
					{
						max = item_games->times_played;
						//sprintf(helper, "%ld - %s", item_games->TimesPlayed, item_games->Title);
						DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Top);
						mini_totals++;
						//printf("%s - Top max:%d\n", item_games->Title, max);
					}
					else if (item_games->hidden != 1)
					{
						//sprintf(helper, "%ld - %s", item_games->TimesPlayed, item_games->Title);
						DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title,
						         MUIV_List_Insert_Bottom);
						mini_totals++;
						//printf("%s - Bottom max:%d\n", item_games->Title, max);
					}
				}
			}
		}
		//DoMethod(App->LV_GamesList,MUIM_Show);
		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
		// DoMethod(App->LV_GamesList,MUIM_List_Redraw,MUIV_List_Redraw_All);
		//DoMethod(App->LV_GamesList,MUIM_List_Jump,mini_totals);
		//DoMethod(App->LV_GamesList,MUIM_List_Jump,0);
		sprintf(helperstr, "Total %d games.", mini_totals);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
	else if (!strcmp(str, "--Never Played--"))
	{
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		//DoMethod(App->LV_GamesList,MUIM_Hide);
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		if (games)
		{
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				if (item_games->times_played == 0)
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Top);
					mini_totals++;
				}
			}
		}

		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
		sprintf(helperstr, "Total %d games.", mini_totals);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
		// the
	else
	{
		/* Erase list */
		DoMethod(app->LV_GamesList, MUIM_List_Clear);
		//DoMethod(App->LV_GamesList,MUIM_Hide);
		set(app->LV_GamesList, MUIA_List_Quiet, TRUE);
		if (games)
		{
			/* Find the entries in Games and update the list */
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				if (!strcmp(item_games->genre, str) && item_games->hidden != 1)
				{
					DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
					mini_totals++;
				}
			}
		}
		//DoMethod(App->LV_GamesList,MUIM_Show);
		set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
		// DoMethod(App->LV_GamesList,MUIM_List_Redraw,MUIV_List_Redraw_All);
		//DoMethod(App->LV_GamesList,MUIM_List_Jump,mini_totals);
		//DoMethod(App->LV_GamesList,MUIM_List_Jump,0);
		sprintf(helperstr, "Total %d games.", mini_totals);
		set(app->TX_Status, MUIA_Text_Contents, helperstr);
	}
}

/*
* Splits a string using spl
*/
char** my_split(char* str, char* spl)
{
	char **ret, *buffer[256], buf[4096];
	int i;

	if (!spl)
	{
		ret = (char **)malloc(2 * sizeof(char *));
		ret[0] = (char *)strdup(str);
		ret[1] = NULL;
		return (ret);
	}

	int count = 0;

	char * fptr = str;
	const int spl_len = strlen(spl);
	char * sptr = strstr(fptr, spl);
	while (sptr)
	{
		i = sptr - fptr;
		memcpy(buf, fptr, i);
		buf[i] = '\0';
		buffer[count++] = (char *)strdup(buf);
		fptr = sptr + spl_len;
		sptr = strstr(fptr, spl);
	}
	sptr = strchr(fptr, '\0');
	i = sptr - fptr;
	memcpy(buf, fptr, i);
	buf[i] = '\0';
	buffer[count++] = (char *)strdup(buf);

	ret = (char **)malloc((count + 1) * sizeof(char *));

	for (i = 0; i < count; i++)
	{
		ret[i] = buffer[i];
	}
	ret[count] = NULL;

	return (ret);
}

void followthread(BPTR lock, int tab_level)
{
	int exists = 0, j;
	char str[512], fullpath[512], temptitle[256];

	/*  if at the end of the road, don't print anything */
	if (!lock)
	{
		return;
	}

	//CurrentDir(lock);

	/*  allocate space for a FileInfoBlock */
	struct FileInfoBlock * m = (struct FileInfoBlock *)AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);

	int success = Examine(lock, m);
	if (m->fib_DirEntryType <= 0)
	{
		return;
	}

	/*  The first call to Examine fills the FileInfoBlock with information
	about the directory.  If it is called at the root level, it contains
	the volume name of the disk.  Thus, this program is only printing
	the output of ExNext rather than both Examine and ExNext.  If it
	printed both, it would list directory entries twice! */

	while (success = ExNext(lock, m))
	{
		/*  Print what we've got: */
		//printf("\n");
		//for (i = 0; i < tab_level; i++)
		//    printf("\t");
		if (m->fib_DirEntryType > 0)
		{
		}

		//make m->fib_FileName to lower
		const int kp = strlen((char *)m->fib_FileName);
		for (int s = 0; s < kp; s++) m->fib_FileName[s] = tolower(m->fib_FileName[s]);
		if (strstr(m->fib_FileName, ".slave"))
		{
			NameFromLock(lock, str, 511);
			sprintf(fullpath, "%s/%s", str, m->fib_FileName);

			/* add the slave to the gameslist (if it does not already exist) */
			for (item_games = games; item_games != NULL; item_games = item_games->next)
			{
				//printf("[%s]\n", item_games->Path);
				//printf("comparing [%s] - [%s]\n", fullpath, item_games->Path);
				if (!strcmp(item_games->path, fullpath))
				{
					exists = 1;
					//printf("Exists: [%s]\n", fullpath);
					item_games->exists = 1;
					break;
				}
			}
			if (exists == 0)
			{
				item_games = (games_list *)calloc(1, sizeof(games_list));
				item_games->next = NULL;

				/* strip the path from the slave file and get the rest */
				//printf("str- [%s]\n", str);
				for (j = strlen(str) - 1; j >= 0; j--)
				{
					if (str[j] == '/' || str[j] == ':')
						break;
					//		  printf("Break. j=%ld\n", j);
				}

				//CHANGE 2007-12-03: init n=0 here
				int n = 0;
				for (int k = j + 1; k <= strlen(str) - 1; k++)
					temptitle[n++] = str[k];
				temptitle[n] = '\0';
				//printf("title: [%s]\n", temptitle);
				//strcpy (item_games->Title, temptitle);
				if (TITLESFROMDIRS)
				{
					// If the TITLESFROMDIRS tooltype is enabled, set Titles from Directory names
					char* title = get_directory_name(fullpath);
					if (title != NULL)
						strcpy(item_games->title, title);
				}
				else 
				{
					// Default behavior: set Titles by the .slave contents
					if (get_title_from_slave(fullpath, item_games->title))
						strcpy(item_games->title, temptitle);
				}

				while (check_dup_title(item_games->title))
				{
					strcat(item_games->title, " Alt");
				}

				strcpy(item_games->genre, "Unknown");
				strcpy(item_games->path, fullpath);
				item_games->favorite = 0;
				item_games->times_played = 0;
				item_games->last_played = 0;
				item_games->exists = 1;
				item_games->hidden = 0;

				if (games == NULL)
				{
					games = item_games;
				}
				else
				{
					item_games->next = games;
					games = item_games;
				}
			}
			exists = 0;
		}

		/*  If we have a directory, then enter it: */
		if (m->fib_DirEntryType > 0)
		{
			/*  Since it is a directory, get a lock on it: */
			const BPTR newlock = Lock(m->fib_FileName, ACCESS_READ);

			/*  cd to this directory: */
			const BPTR oldlock = CurrentDir(newlock);

			/*  recursively follow the thread down to the bottom: */
			followthread(newlock, tab_level + 1);

			/*  cd back to where we used to be: */
			CurrentDir(oldlock);

			/*  Unlock the directory we just visited: */
			if (newlock)
				UnLock(newlock);
		}
	}
	FreeMem(m, sizeof(struct FileInfoBlock));
}

void refresh_list(const int check_exists)
{
	char helperstr[512];

	DoMethod(app->LV_GamesList, MUIM_List_Clear);
	total_games = 0;
	set(app->LV_GamesList, MUIA_List_Quiet, TRUE);

	if (check_exists == 0)
	{
		for (item_games = games; item_games != NULL; item_games = item_games->next)
		{
			if (strlen(item_games->path) != 0 && item_games->hidden != 1)
			{
				total_games++;
				DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
			}
		}
	}
	else
	{
		for (item_games = games; item_games != NULL; item_games = item_games->next)
		{
			if (strlen(item_games->path) != 0 && item_games->hidden != 1 && item_games->exists == 1)
			{
				total_games++;
				DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);
			}
			else
			{
				printf("[%s] [%s] [%d]\n", item_games->title, item_games->path, item_games->exists);
			}
		}
	}

	//DoMethod(App->LV_GamesList,MUIM_List_Redraw,MUIV_List_Redraw_All);

	set(app->LV_GamesList, MUIA_List_Quiet, FALSE);
	sprintf(helperstr, "Total %d games.", total_games);
	set(app->TX_Status, MUIA_Text_Contents, helperstr);
}

/*
* Saves the current Games struct to disk
*/
void save_list(const int check_exists)
{
	set(app->WI_Wait, MUIA_Window_Open, TRUE);

	FILE* fpgames = fopen("PROGDIR:gameslist", "w");
	if (!fpgames)
	{
		msg_box("Could not open gameslist file!");
	}
	else
	{
		for (item_games = games; item_games != NULL; item_games = item_games->next)
		{
			//printf("Saving: %s\n", item_games->Title);
			if (check_exists == 1)
			{
				if (item_games->exists == 1)
				{
					fprintf(fpgames, "index=%d\n", item_games->index);
					fprintf(fpgames, "title=%s\n", item_games->title);
					fprintf(fpgames, "genre=%s\n", item_games->genre);
					fprintf(fpgames, "path=%s\n", item_games->path);
					fprintf(fpgames, "favorite=%d\n", item_games->favorite);
					fprintf(fpgames, "timesplayed=%d\n", item_games->times_played);
					fprintf(fpgames, "lastplayed=%d\n", item_games->last_played);
					fprintf(fpgames, "hidden=%d\n\n", item_games->hidden);

					//fflush(fpgames);
				}
				else
				{
					strcpy(item_games->path, "");
				}
			}
			else
			{
				fprintf(fpgames, "index=%d\n", item_games->index);
				fprintf(fpgames, "title=%s\n", item_games->title);
				fprintf(fpgames, "genre=%s\n", item_games->genre);
				fprintf(fpgames, "path=%s\n", item_games->path);
				fprintf(fpgames, "favorite=%d\n", item_games->favorite);
				fprintf(fpgames, "timesplayed=%d\n", item_games->times_played);
				fprintf(fpgames, "lastplayed=%d\n", item_games->last_played);
				fprintf(fpgames, "hidden=%d\n\n", item_games->hidden);

				//fflush(fpgames);
			}
		}
		fflush(fpgames);
		fclose(fpgames);
	}

	set(app->WI_Wait, MUIA_Window_Open, FALSE);
}

//function to return the dec eq of a hex no.
int hex2dec(char* hexin)
{
	int dec;
	//lose the first $ character
	hexin++;
	sscanf(hexin, "%lx", &dec);
	return dec;
}

void msg_box(char* msg)
{
	msgbox.es_StructSize = sizeof msgbox;
	msgbox.es_Flags = 0;
	msgbox.es_Title = "iGame";
	msgbox.es_TextFormat = (unsigned char*)msg;
	msgbox.es_GadgetFormat = "Ok";

	EasyRequest(NULL, &msgbox, NULL);
}

void get_screen_size(int* width, int* height)
{
	struct Screen* wbscreen;
	ULONG mode_id;

	struct Library* intuition_base;
	struct Library* gfx_base;

	if (intuition_base = (struct Library *)OpenLibrary("intuition.library", 36))
	{
		if (gfx_base = (struct Library *)OpenLibrary("graphics.library", 36))
		{
			if (wbscreen = LockPubScreen("Workbench"))
			{
				/* Using intuition.library/GetScreenDrawInfo(), we get the pen
				* array we'll use for the screen clone the easy way. */
				struct DrawInfo * drawinfo = GetScreenDrawInfo(wbscreen);

				struct ViewPort * vp = &wbscreen->ViewPort;
				/* Use graphics.library/GetVPModeID() to get the ModeID of the
				* Workbench screen. */
				if ((mode_id = GetVPModeID(vp)) != (unsigned long)INVALID_ID)
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
			CloseLibrary(gfx_base);
		}
		CloseLibrary(intuition_base);
	}
}

void read_tool_types()
{
	struct Library* icon_base;
	struct DiskObject* disk_obj;

	int screen_width, screen_height;

	SS_HEIGHT = -1;
	SS_WIDTH = -1;
	NOGUIGFX = 0;
	FILTERUSEENTER = 0;
	NOSCREENSHOT = 0;
	SAVESTATSONEXIT = 0;
	TITLESFROMDIRS = 0;

	if (icon_base = (struct Library *)OpenLibrary("icon.library", 0))
	{
		char* filename = strcat("PROGDIR:", executable_name);
		if (disk_obj = (struct DiskObject *)GetDiskObject((unsigned char*)filename))
		{
			if (FindToolType(disk_obj->do_ToolTypes, "SCREENSHOT"))
			{
				char **tool_types = (char **)disk_obj->do_ToolTypes;
				char *tool_type = *tool_types;

				char **temp_tbl = my_split((char *)tool_type, "=");
				if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp(temp_tbl[0], " ") || !strcmp(temp_tbl[0], ""))
				{
					msg_box("Bad tooltype!");
					exit(0);
				}

				if (temp_tbl[1] != NULL)
				{
					char **temp_tbl2 = my_split((char *)(temp_tbl[1]), "x");
					if (temp_tbl2[0]) SS_WIDTH = atoi((char *)temp_tbl2[0]);
					if (temp_tbl2[1]) SS_HEIGHT = atoi((char *)temp_tbl2[1]);

					free(temp_tbl2[0]);
					free(temp_tbl2[1]);
					free(temp_tbl2);
					free(temp_tbl[0]);
					free(temp_tbl[1]);
					free(temp_tbl);
				}
			}

			if (FindToolType(disk_obj->do_ToolTypes, "NOGUIGFX"))
			{
				NOGUIGFX = 1;
			}

			if (FindToolType(disk_obj->do_ToolTypes, "FILTERUSEENTER"))
			{
				FILTERUSEENTER = 1;
			}

			if (FindToolType(disk_obj->do_ToolTypes, "NOSCREENSHOT"))
			{
				NOSCREENSHOT = 1;
			}

			if (FindToolType(disk_obj->do_ToolTypes, "SAVESTATSONEXIT"))
			{
				SAVESTATSONEXIT = 1;
			}

			if (FindToolType(disk_obj->do_ToolTypes, "TITLESFROMDIRS"))
			{
				TITLESFROMDIRS = 1;
			}
		}

		CloseLibrary(icon_base);
	}

	//check screen res and adjust image box accordingly
	if (SS_HEIGHT == -1 && SS_WIDTH == -1)
	{
		get_screen_size(&screen_width, &screen_height);

		//if values are ok from the previous function, and user has not provided his own values, calculate a nice size
		if (screen_width != -1 && screen_height != -1)
		{
			//for hi res (>1024x768) default values is ok
			if (screen_width >= 800 && screen_height >= 600)
			{
				SS_HEIGHT = 128;
				SS_WIDTH = 160;
			}
			else
			{
				SS_WIDTH = screen_width / 4.5;
				SS_HEIGHT = screen_height / 4;
			}
		}
		else
		{
			SS_HEIGHT = 128;
			SS_WIDTH = 160;
		}
	}
}

void menu_add_non_whdload()
{
	set(app->STR_AddGameTitle, MUIA_String_Contents, NULL);
	set(app->PA_AddGamePath, MUIA_String_Contents, NULL);

	set(app->WI_NonWhdload, MUIA_Window_Open, TRUE);
}

void non_whdload_ok()
{
	char *str, *str_title, helperstr[200];

	//for some reason it needs this twice to get the contents...
	get(app->PA_AddGamePath, MUIA_String_Contents, &str);
	get(app->STR_AddGameTitle, MUIA_String_Contents, &str_title);

	get(app->PA_AddGamePath, MUIA_String_Contents, &str);
	get(app->STR_AddGameTitle, MUIA_String_Contents, &str_title);

	//printf("[%s] - [%s]\n", str, str_title);

	if (strlen(str_title) == 0)
	{
		msg_box("Please let me know the name of the game...");
		return;
	}
	if (strlen(str) == 0)
	{
		msg_box("Please pick the game executable...");
		return;
	}

	//add the game to the list
	item_games = (games_list *)calloc(1, sizeof(games_list));
	item_games->next = NULL;
	item_games->index = 0;
	strcpy(item_games->title, (char *)str_title);
	strcpy(item_games->genre, "Unknown");
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

	DoMethod(app->LV_GamesList, MUIM_List_InsertSingle, item_games->title, MUIV_List_Insert_Sorted);

	sprintf(helperstr, "Total %d games.", ++total_games);
	set(app->TX_Status, MUIA_Text_Contents, helperstr);

	save_list(0);

	set(app->WI_NonWhdload, MUIA_Window_Open, FALSE);
}

/*
* Gets title from a slave file
* returns 0 on success, 1 on fail
*/
int get_title_from_slave(char* slave, char* title)
{
	char slave_title[100];
	int i = -1;

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

	FILE * fp = fopen(slave, "rb");
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

	for (i = 0; i <= 99; i++)
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

/*
* Checks if the title already exists
* returns 1 if yes, 0 otherwise
*/
int check_dup_title(char* title)
{
	for (games_list* check_games = games; check_games != NULL; check_games = check_games->next)
	{
		if (!strcmp(check_games->title, title))
		{
			//printf("[%s] [%s]\n", check_games->Title, title);
			return 1;
		}
	}

	return 0;
}

int get_delimiter_position(const char* str)
{
	char* delimiter = strrchr(str, '/');
	if (!delimiter)
		delimiter = strrchr(str, ':');

	if (!delimiter)
	{
		//printf("Invalid path detected!\n");
		return 0;
	}

	const int pos = delimiter - str;
	return pos;
}

// Get the Directory part from a full path containing a file
char* get_directory_name(char* str)
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

	//printf("%s -> %s\n", str, dirName);
	return dir_name;
}

// Get the application's executable name
char* get_executable_name(int argc, char** argv)
{
	struct WBStartup *argmsg;
	struct WBArg *wb_arg;
	BPTR olddir;

	// argc is zero when run from the Workbench,
	// positive when run from the CLI
	if (argc == 0)
	{
		// in AmigaOS, argv is a pointer to the WBStartup message
		// when argc is zero (run under the Workbench)
		argmsg = (struct WBStartup *)argv;
		wb_arg = argmsg->sm_ArgList;         /* head of the arg list */

		executable_name = wb_arg->wa_Name;
	}
	else
	{
		// Run from the CLI
		executable_name = argv[0];
	}

	return executable_name;
}