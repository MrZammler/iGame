/*
*	update_titles.c
*	scans a gamelist file and updates the titles from the slave info
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* strdup(const char* s);
extern char* strcasestr(const char *haystack, const char *needle);

typedef struct games
{
	char title[200];
	char genre[100];
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

/*
* Gets title from a slave file
* returns 0 on success, 1 on fail
*/
int get_title_from_slave(char* slave, char* title)
{
	char Title[100];

	struct SlaveInfo
	{
		unsigned long Security;
		char ID[8];
		unsigned short Version;
		unsigned short Flags;
		unsigned long BaseMemSize;
		unsigned long ExecInstall;
		unsigned short GameLoader;
		unsigned short CurrentDir;
		unsigned short DontCache;
		char keydebug;
		char keyexit;
		unsigned long ExpMem;
		unsigned short name;
		unsigned short copy;
		unsigned short info;
	};

	struct SlaveInfo sl;

	FILE* fp = fopen(slave, "rbe");
	if (fp == NULL)
	{
		return 1;
	}

	//seek to +0x20
	fseek(fp, 32, SEEK_SET);

	fread(&sl, 1, sizeof(sl), fp);


	//sl.Version = (sl.Version>>8) | (sl.Version<<8);
	//sl.name = (sl.name>>8) | (sl.name<<8);

	//printf ("[%s] [%d]\n", sl.ID, sl.Version);

	//sl.name holds the offset for the slave name
	fseek(fp, sl.name + 32, SEEK_SET);
	//title = calloc (1, 100);
	//fread (title, 1, 100, fp);

	if (sl.Version < 10)
	{
		return 1;
	}

	for (int i = 0; i <= 99; i++)
	{
		Title[i] = fgetc(fp);
		if (Title[i] == '\n')
		{
			Title[i] = '\0';
			break;
		}
	}

	strcpy(title, Title);

	//printf("[%s]\n", Title);

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
			//			  printf("[%s] [%s]\n", check_games->Title, title);
			return 1;
		}
	}

	return 0;
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

	char* fptr = str;
	const int spl_len = strlen(spl);
	char* sptr = strstr(fptr, spl);
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

/*
* Saves the current Games struct to disk
*/
void save_list(int CheckExists)
{
	FILE* fpgames = fopen("PROGDIR:gameslist", "we");

	if (!fpgames)
	{
		printf("Could not open gameslist file!");
	}
	else
	{
		for (item_games = games; item_games != NULL; item_games = item_games->next)
		{
			//printf("Saving: %s\n", item_games->Title);
			if (CheckExists == 1)
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

					fflush(fpgames);
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

				fflush(fpgames);
			}
		}

		fclose(fpgames);
	}
}

int main()
{
	char FileLine[1000];
	char helperstr[250];

	printf("This program will update your gameslist with game titles from the slave files\n");
	printf("Do you wish to continue? (Y/N): ");

	const char resp = fgetc(stdin);

	if (resp != 'Y' && resp != 'y')
	{
		printf("Exiting...\n");
		exit(0);
	}

	FILE* fpgames = fopen("PROGDIR:gameslist", "re");
	if (!fpgames)
	{
		printf(
			"Could not open gameslist.Please make sure you run this command within the same dir as the gamelist file.\n");
	}
	else
	{
		do
		{
			if (fgets(FileLine, sizeof(FileLine), fpgames) == NULL) { break; }
			FileLine[strlen(FileLine) - 1] = '\0';
			//printf("%s\n", FileLine);

			if (strlen(FileLine) == 0) continue;

			char** temp_tbl = my_split((char *)FileLine, "=");
			if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp(temp_tbl, " ") || !strcmp(temp_tbl, ""))
			{
				continue;
			}

			item_games = (games_list *)calloc(1, sizeof(games_list));
			item_games->next = NULL;

			if (!strcmp(temp_tbl[0], "index"))
			{
				item_games->index = atoi(temp_tbl[1]);
				item_games->exists = 0;
				do
				{
					if (fgets(FileLine, sizeof(FileLine), fpgames) == NULL) { break; }

					FileLine[strlen(FileLine) - 1] = '\0';

					/* split */
					temp_tbl = my_split((char *)(FileLine), "=");
					if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp(temp_tbl[0], " ") || !strcmp(temp_tbl[0], "")
					)
					{
						break;
					}

					//this is to make sure that gameslist goes ok from 1.2 to 1.3
					item_games->hidden = 0;

					if (!strcmp(temp_tbl[0], "title"))
						strcpy(item_games->title, temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "genre"))
						strcpy(item_games->genre, temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "path"))
						strcpy(item_games->path, temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "favorite"))
						item_games->favorite = atoi(temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "timesplayed"))
						item_games->times_played = atoi(temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "lastplayed"))
						item_games->last_played = atoi(temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "hidden"))
						item_games->hidden = atoi(temp_tbl[1]);

					//	  break;

					//}


					/* free some mem */
					free(temp_tbl[1]);
					free(temp_tbl[0]);
					free(temp_tbl);
					temp_tbl = NULL;
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

			free(temp_tbl[1]);
			free(temp_tbl[0]);
			free(temp_tbl);

			//    fgets (FileLine, sizeof(FileLine), fpgames);
			//    strcpy(item_games->Title, temp_tbl[1]

			//    str = malloc(strlen((char *)temp_tbl[1]+1));
			//    printf("here\n");
			//    if (str) strcpy(str,(char *)temp_tbl[1]);
			//    printf("here2\n");
			//    DoMethod(App->LV_GamesList, MUIM_List_Insert, &str, 1, MUIV_List_Insert_Bottom);
			//    printf("here3\n");
			//if (str) free(str);
			//}

			//free(temp_tbl[1]);free(temp_tbl[0]);free(temp_tbl);
		}
		while (1);
	}
	fclose(fpgames);

	printf("Gameslist loaded, please wait...");

	for (item_games = games; item_games != NULL; item_games = item_games->next)
	{
		//only if it is a slave file ;-)
		if (strcasestr(item_games->path, ".slave"))
		{
			if (!get_title_from_slave(item_games->path, helperstr))
			{
				printf("Changing [%s] to [%s]\n", item_games->title, helperstr);
				item_games->title[0] = '\0';
				while (check_dup_title(helperstr))
				{
					strcat(helperstr, " Alt");
				}
				strcpy(item_games->title, helperstr);
			}
		}
	}

	save_list(0);

	printf("All done.!\n");
}
