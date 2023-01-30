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
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/icon.h>
#include <proto/muimaster.h>

#include <workbench/icon.h>

/* System */
#if defined(__amigaos4__)
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
#include "slavesList.h"
#include "funcs.h"
#include "fsfuncs.h"

extern struct ObjApp* app;
extern char* executable_name;

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


/*
 * Get the path of the parent folder
 */
void getParentPath(char *filename, char *result)
{
	BPTR fileLock = Lock(filename, SHARED_LOCK);
	if (fileLock)
	{
		BPTR folderLock = ParentDir(fileLock);
		NameFromLock(folderLock, result, sizeof(char) * MAX_PATH_SIZE);

		UnLock(folderLock);
		UnLock(fileLock);
	}
}

/*
 * Get the filename of a folder/file from a path
 */
void getNameFromPath(char *path, char *result, unsigned int size)
{
	BPTR pathLock = Lock(path, SHARED_LOCK);
	if (pathLock) {
		struct FileInfoBlock *FIblock = (struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock), MEMF_CLEAR);

		if (Examine(pathLock, FIblock))
		{
			strncpy(result, FIblock->fib_FileName, size);
			FreeVec(FIblock);
		}
		UnLock(pathLock);
	}
}

void getFullPath(const char *path, char *result)
{
	char *buf = malloc(sizeof(char) * MAX_PATH_SIZE);
	strcpy(buf, path);
	if (path[0] == '/')
	{
		sprintf(buf, "PROGDIR:%s", path);
	}

	BPTR pathLock = Lock(buf, SHARED_LOCK);
	if (pathLock)
	{
		NameFromLock(pathLock, result, sizeof(char) * MAX_PATH_SIZE);
		UnLock(pathLock);
		free(buf);
		return;
	}

	free(buf);
	return ;
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

// TODO: Replaced by slavesListLoadFromCSV() - OBSOLETE
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

// Replaces load_games_csv_list()
void slavesListLoadFromCSV(char *filename)
{
	int bufSize = sizeof(char) * 1024;

	if (check_path_exists(filename))
	{
		const BPTR fpgames = Open((CONST_STRPTR) filename, MODE_OLDFILE);
		if (fpgames)
		{
			char *lineBuf = AllocVec(bufSize, MEMF_CLEAR);
			char *buf = AllocVec(MAX_PATH_SIZE, MEMF_CLEAR);

			while (FGets(fpgames, lineBuf, bufSize) != NULL)
			{
				slavesList *node = malloc(sizeof(slavesList));

				buf = strtok(lineBuf, ";");
				node->instance = atoi(buf);

				buf = strtok(NULL, ";");
				node->title[0] = '\0';
				if (strncmp(buf, "\"\"", 2))
				{
					sprintf(node->title,"%s", substring(buf, 1, -2));
				}

				buf = strtok(NULL, ";");
				node->genre[0] = '\0';
				if (strncmp(buf, "\"\"", 2))
				{
					sprintf(node->genre,"%s", substring(buf, 1, -2));
				}
				if(isStringEmpty(node->genre))
				{
					sprintf(node->genre,"Unknown");
				}

				buf = strtok(NULL, ";");
				node->path[0] = '\0';
				if (strncmp(buf, "\"\"", 2))
				{
					sprintf(node->path,"%s", substring(buf, 1, -2));
				}

				buf = strtok(NULL, ";");
				node->favourite = atoi(buf);

				buf = strtok(NULL, ";");
				node->times_played = atoi(buf);

				buf = strtok(NULL, ";");
				node->last_played = atoi(buf);

				buf = strtok(NULL, ";");
				node->hidden = atoi(buf);

				buf = strtok(NULL, ";");
				node->deleted = atoi(buf);

				// TODO: Add here a check for the old CSV files
				buf = strtok(NULL, ";");
				node->user_title[0] = '\0';
				if (strncmp(buf, "\"\"", 2))
				{
					sprintf(node->user_title,"%s", substring(buf, 1, -2));
				}

				slavesListAddTail(node);
			}
			Close(fpgames);
			// FreeVec(buf);
			// FreeVec(lineBuf);
		}
	}
	// TODO: Remove this
	// slavesListPrint();
}

// TODO: Replaced by slavesListSaveToCSV() - OBSOLETE
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

// Replaces save_to_csv()
void slavesListSaveToCSV(const char *filename)
{
	// return;
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

	slavesList *currPtr = getSlavesListHead();

	while (currPtr != NULL)
	{
		fprintf(
			fpgames,
			"%d;\"%s\";\"%s\";\"%s\";%d;%d;%d;%d;%d;\"%s\";\n",
			currPtr->instance, currPtr->title,
			currPtr->genre, currPtr->path, currPtr->favourite,
			currPtr->times_played, currPtr->last_played, currPtr->hidden,
			currPtr->deleted, currPtr->user_title
		);
		currPtr = currPtr->next;
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
	char slave_title[MAX_SLAVE_TITLE_SIZE];

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

/*
 * Set the item title name based on the path on disk
 */
void getTitleFromPath(char *path, char *title)
{
	int bufSize = sizeof(char) * MAX_PATH_SIZE;
	char *buf = AllocVec(bufSize, MEMF_CLEAR);

	STRPTR itemFolderPath = AllocVec(bufSize, MEMF_CLEAR);
	getParentPath(path, itemFolderPath);
	if (itemFolderPath)
	{
		getNameFromPath(itemFolderPath, buf, bufSize);

		if (current_settings->no_smart_spaces)
		{
			strcpy(title, buf);
		}
		else
		{
			strcpy(title, add_spaces_to_string(buf));
		}

	}
	FreeVec(itemFolderPath);
	FreeVec(buf);
}

// TODO: This seems OBSOLETE and can be replaced by getNameFromPath(). Needs investigation
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

// TODO: This is replaced by slavesListSearchByTitle() - OBSOLETE
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

BOOL isPathFolder(char *path)
{
	if (path[strlen(path)-1] == ':')
		return FALSE;

	return TRUE;
}

/*
* Get the icon tooltypes and copies them in result
* The path needs to be the full file path without .info at the end
*/
void getIconTooltypes(char *path, char *result)
{
	if (IconBase)
	{
		struct DiskObject *diskObj = GetDiskObjectNew(path);
		if(diskObj)
		{
			char *buf = AllocVec(sizeof(char) * 64, MEMF_CLEAR);
			// size_t cnt = 0;
			for (STRPTR *tool_types = diskObj->do_ToolTypes; (buf = *tool_types); ++tool_types)
			{
				// printf("%s\n", buf);
				// if (cnt > 10) break;

				// cnt++;

				if (!strncmp(buf, "*** DON'T EDIT", 14) || !strncmp(buf, "IM", 2))
					continue;

				sprintf(result, "%s%s\n", result, buf);
			}
			FreeVec(buf);
			FreeDiskObject(diskObj);
		}
	}
}

void setIconTooltypes(char *path, char *tooltypes)
{
	if (IconBase)
	{
		// struct DiskObject *diskObj = GetDiskObjectNew(path);
		struct DiskObject *diskObj = GetIconTags(path, TAG_DONE);
		if(diskObj && FALSE)
		{
			size_t oldToolTypesCnt = 0;
			size_t cutPos = 0;
			size_t newToolTypesCnt = 0;

			char *buf = AllocVec(sizeof(char) * 64, MEMF_CLEAR);

			// Get the number of the new tooltypes
			char **table = my_split(tooltypes, "\n");
			for (table; (buf = *table); ++table)
			{
				printf("DBG: %s\n", buf);
				newToolTypesCnt++;
			}

			// Get the number of the old tooltypes, as well as where the image data start (cutPos)
			// for (STRPTR *oldToolTypes = diskObj->do_ToolTypes; (buf = *oldToolTypes); ++oldToolTypes)
			// {
			// 	oldToolTypesCnt++;
			// 	if (!strncmp(buf, "*** DON'T EDIT", 14) && (cutPos == 0))
			// 		cutPos = oldToolTypesCnt;

			// 	if (!strncmp(buf, "IM", 2) && (cutPos == 0))
			// 		cutPos = oldToolTypesCnt;

			// }

			// unsigned int allCnt = (oldToolTypesCnt - cutPos) + newToolTypesCnt + 29;


			// newToolTypesCnt++; // That's for the last NULL

			unsigned char **newToolTypes = AllocVec(sizeof(char *) * newToolTypesCnt, MEMF_CLEAR);
			if (newToolTypes)
			{
				printf("DBG: newToolTypes set\n");

				char **table2 = my_split(tooltypes, "\n");
				size_t table2Cnt = 0;
				for (table2; (buf = *table2); ++table2)
				{
					printf("DBG: %s\n", buf);
					newToolTypes[table2Cnt] = buf;
					table2Cnt++;
				}
				printf("DBG: table2Cnt %d\tcutPos %d\n", table2Cnt, cutPos);
				// printf("DBG: Before old tooltypes %d\t%d\n", newToolTypesCnt, cutPos);
				// for(size_t i = cutPos; i < oldToolTypesCnt; i++)
				// for(size_t i = 0; i < oldToolTypesCnt; i++)
				// {
				// 	// printf("%d\t%d\t%d\n", newToolTypesCnt+i-cutPos, i, i-cutPos);
				// 	// if (i > 10) break;
				// 	newToolTypes[table2Cnt+i] = diskObj->do_ToolTypes[i];
				// }
				newToolTypes[newToolTypesCnt-1] = NULL;

				diskObj->do_ToolTypes = newToolTypes;
				printf("DBG: PutDiskObject\n");

				// LONG errorCode;
				// BOOL success;

				// success = PutIconTags(path, diskObj,
				// 	ICONPUTA_DropNewIconToolTypes, TRUE,
				// 	ICONA_ErrorCode, &errorCode,
				// TAG_DONE);

				// if(success == FALSE)
				// {
				// 	Printf("could not store default picture icon;\n");
				// 	PrintFault(errorCode, NULL);
				// }

				// PutDiskObject(path, diskObj);
				FreeVec(newToolTypes);
			}

			// printf("DBG: %d\t%d\t%d\t%d\n",
			// 	oldToolTypesCnt, cutPos, newToolTypesCnt, allCnt);
			FreeDiskObject(diskObj);
		}
	}
}

// void setIconTooltypes(char *path, char *tooltypes)
// {
// 	if (IconBase)
// 	{
// 		struct DiskObject *diskObj = GetDiskObjectNew(path);
// 		if(diskObj)
// 		{
// 			char* game_tooltypes;
// 			char* tool_type;
// 			int i;
// 			int new_tool_type_count = 1, old_tool_type_count = 0, old_real_tool_type_count = 0;

// 			for (i = 0; i <= strlen(tooltypes); i++)
// 				if (tooltypes[i] == '\n') new_tool_type_count++;

// 			//add one for the last tooltype that doesnt end with \n
// 			new_tool_type_count++;

// 			for (i = 0; i <= strlen(game_tooltypes); i++)
// 				if (game_tooltypes[i] == '\n') old_tool_type_count++;

// 			for (char** tool_types = (char **)diskObj->do_ToolTypes; (tool_type = *tool_types); ++
// 					tool_types)
// 				old_real_tool_type_count++;

// 			unsigned char** new_tool_types = AllocVec(new_tool_type_count * sizeof(char *),
// 														MEMF_FAST | MEMF_CLEAR);
// 			unsigned char** newptr = new_tool_types;

// 			char** temp_tbl = my_split((char *)tooltypes, "\n");
// 			// if (temp_tbl == NULL
// 			// 	|| temp_tbl[0] == NULL
// 			// 	|| !strcmp((char *)temp_tbl[0], " ")
// 			// 	|| !strcmp((unsigned char *)temp_tbl[0], ""))
// 			// 	break;

// 			for (i = 0; i <= new_tool_type_count - 2; i++)
// 				*newptr++ = (unsigned char*)temp_tbl[i];

// 			*newptr = NULL;

// 			diskObj->do_ToolTypes = new_tool_types;
// 			PutDiskObject(path, diskObj);
// 			FreeDiskObject(diskObj);

// 			if (temp_tbl)
// 				free(temp_tbl);
// 			if (new_tool_types)
// 				FreeVec(new_tool_types);
// 		}
// 	}
// }
