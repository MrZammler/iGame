/*
*	update_titles.c
*	scans a gamelist file and updates the titles from the slave info
*/

#include <stdio.h>

typedef struct games {
        char    Title[200];
        char    Genre[100];
        int     Index;
        char    Path[256];
		int		Favorite;
		int 	TimesPlayed;
		int		LastPlayed;		//indicates whether this one was the last game played
		int		Exists;			//indicates whether this game still exists after a scan
		int		Hidden;			//game is hidden from normal operation
        struct games *next;
} Games_list;

Games_list   *item_games=NULL, *Games=NULL;

/*
* Gets title from a slave file
* returns 0 on success, 1 on fail
*/
int GetTitleFromSlave(char *slave, char *title)
{
    FILE *fp;
	char Title[100];
	int i=-1;

	struct SlaveInfo {
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

	fp = fopen(slave, "rb");
	if (fp==NULL)
	{
		return 1;
	}

	//seek to +0x20
	fseek (fp, 32, SEEK_SET);

	fread(&sl, 1, sizeof(sl), fp);


	//sl.Version = (sl.Version>>8) | (sl.Version<<8);
	//sl.name = (sl.name>>8) | (sl.name<<8);

	//printf ("[%s] [%d]\n", sl.ID, sl.Version);

	//sl.name holds the offset for the slave name
	fseek (fp, sl.name+32, SEEK_SET);
	//title = calloc (1, 100);
	//fread (title, 1, 100, fp);

	if (sl.Version < 10){
		return 1;
	}

	for (i=0;i<=99;i++){
		Title[i]=fgetc(fp);
		if (Title[i]=='\n') {Title[i]='\0';break;}
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
int CheckDupTitle(char *title)
{
	Games_list   *check_games=NULL;

	for (check_games=Games; check_games!=NULL;check_games=check_games->next) {
		if (!strcmp(check_games->Title, title)){
//			  printf("[%s] [%s]\n", check_games->Title, title);
			return 1;
		}
	}

	return 0;
}

/*
* Splits a string using spl
*/
char **my_split (char *str, char *spl)
{
        char    *fptr;
        char    *sptr;
        char    **ret, *buffer[256], buf[4096];
        int     count, i, spl_len;

        if (!spl) {
                ret = (char **)malloc(2*sizeof(char *));
                ret[0] = (char *)strdup(str);
                ret[1] = NULL;
                return (ret);
        }

        count = 0;

        fptr = str;
        spl_len = strlen(spl);
        sptr = strstr(fptr, spl);
        while (sptr) {
                i = sptr-fptr;
                memcpy(buf, fptr, i);
                buf[i] = '\0';
                buffer[count++] = (char *)strdup(buf);
                fptr = sptr + spl_len;
                sptr = strstr(fptr, spl);
        }
        sptr = strchr(fptr, '\0');
        i = sptr-fptr;
        memcpy(buf, fptr, i);
        buf[i] = '\0';
        buffer[count++] = (char *)strdup(buf);

        ret = (char **)malloc((count+1)*sizeof(char *));

        for (i=0; i<count; i++) {
          ret[i] = buffer[i];
        }
        ret[count] = NULL;

        return (ret);
}

/*
* Saves the current Games struct to disk
*/
void SaveList(int CheckExists)
{
    FILE *fpgames=NULL;

    fpgames = fopen ("PROGDIR:gameslist", "w");

    if (!fpgames){
		printf("Could not open gameslist file!");
    }else{
        for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

            //printf("Saving: %s\n", item_games->Title);
			if (CheckExists==1){
				if (item_games->Exists==1){
					fprintf(fpgames, "index=%d\n", item_games->Index);
					fprintf(fpgames, "title=%s\n", item_games->Title);
					fprintf(fpgames, "genre=%s\n", item_games->Genre);
					fprintf(fpgames, "path=%s\n", item_games->Path);
					fprintf(fpgames, "favorite=%d\n", item_games->Favorite);
					fprintf(fpgames, "timesplayed=%d\n", item_games->TimesPlayed);
					fprintf(fpgames, "lastplayed=%d\n", item_games->LastPlayed);
					fprintf(fpgames, "hidden=%d\n\n", item_games->Hidden);

					fflush(fpgames);
				}else{
					strcpy(item_games->Path, "");
				}
			}else{
                fprintf(fpgames, "index=%d\n", item_games->Index);
				fprintf(fpgames, "title=%s\n", item_games->Title);
				fprintf(fpgames, "genre=%s\n", item_games->Genre);
				fprintf(fpgames, "path=%s\n", item_games->Path);
				fprintf(fpgames, "favorite=%d\n", item_games->Favorite);
				fprintf(fpgames, "timesplayed=%d\n", item_games->TimesPlayed);
				fprintf(fpgames, "lastplayed=%d\n", item_games->LastPlayed);
				fprintf(fpgames, "hidden=%d\n\n", item_games->Hidden);

				fflush(fpgames);
			}

        }

    fclose(fpgames);
    }


}

int main()
{
    FILE *fpgames=NULL;
    char FileLine[1000];
    char *str=NULL, helperstr[250];
	char ***temp_tbl=NULL, resp;

	printf("This program will update your gameslist with game titles from the slave files\n");
	printf("Do you wish to continue? (Y/N): ");

	resp = fgetc(stdin);

	if (resp!='Y' && resp!='y'){
		printf("Exiting...\n");
		exit(0);
	}

    fpgames = fopen ("PROGDIR:gameslist", "r");
    if (!fpgames){
		printf("Could not open gameslist.Please make sure you run this command within the same dir as the gamelist file.\n");
    }else{
        do{
            if (fgets (FileLine, sizeof(FileLine), fpgames)==NULL) { break; }
            FileLine[strlen(FileLine)-1]='\0';
            //printf("%s\n", FileLine);

            if (strlen(FileLine)==0) continue;

            temp_tbl = my_split((char *)(FileLine), "=");
			if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp(temp_tbl, " ") || !strcmp(temp_tbl, ""))
            {
                continue;
            }

            item_games = (Games_list *)calloc(1, sizeof(Games_list));
            item_games->next=NULL;

            if (!strcmp(temp_tbl[0], "index")){
                item_games->Index = atoi(temp_tbl[1]);
				item_games->Exists=0;
                do {

                    if (fgets (FileLine, sizeof(FileLine), fpgames)==NULL) { break; }

                    FileLine[strlen(FileLine)-1]='\0';

                    /* split */
                    temp_tbl = my_split((char *)(FileLine), "=");
                    if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp(temp_tbl[0], " ") || !strcmp(temp_tbl[0], ""))
                    {
						break;
                    }

					//this is to make sure that gameslist goes ok from 1.2 to 1.3
					item_games->Hidden=0;

                    if (!strcmp(temp_tbl[0], "title"))
                        strcpy (item_games->Title, temp_tbl[1]);
                    else if (!strcmp(temp_tbl[0], "genre"))
                        strcpy (item_games->Genre, temp_tbl[1]);
                    else if (!strcmp(temp_tbl[0], "path"))
                        strcpy (item_games->Path, temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "favorite"))
						item_games->Favorite = atoi(temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "timesplayed"))
						item_games->TimesPlayed = atoi(temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "lastplayed"))
						item_games->LastPlayed = atoi(temp_tbl[1]);
					else if (!strcmp(temp_tbl[0], "hidden"))
						item_games->Hidden = atoi(temp_tbl[1]);

					//	  break;

					//}


                    /* free some mem */
                    free(temp_tbl[1]); free (temp_tbl[0]); free (temp_tbl); temp_tbl=NULL;

                } while (1);

                if (Games == NULL) {
                    Games = item_games;
                }
                else {
                    item_games->next = Games;
                    Games = item_games;
                }


            }

            free(temp_tbl[1]); free (temp_tbl[0]); free (temp_tbl); temp_tbl=NULL;

            //    fgets (FileLine, sizeof(FileLine), fpgames);
            //    strcpy(item_games->Title, temp_tbl[1]

            //    str = malloc(strlen((char *)temp_tbl[1]+1));
            //    printf("here\n");
            //    if (str) strcpy(str,(char *)temp_tbl[1]);
            //    printf("here2\n");
            //    DoMethod(App->LV_GamesList_0, MUIM_List_Insert, &str, 1, MUIV_List_Insert_Bottom);
            //    printf("here3\n");
            //if (str) free(str);
            //}

            //free(temp_tbl[1]);free(temp_tbl[0]);free(temp_tbl);

        }while(1);

	}
	fclose(fpgames);

	printf("Gameslist loaded, please wait...");

    for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
		
		//only if it is a slave file ;-)
		if (strstr(item_games->Path, ".slave") || strstr(item_games->Path, ".Slave")){
		
			if (!GetTitleFromSlave(item_games->Path, helperstr)){
				printf("Changing [%s] to [%s]\n", item_games->Title, helperstr);
				item_games->Title[0]='\0';
				while(CheckDupTitle(helperstr)){
					strcat(helperstr, " Alt");
				}
				strcpy(item_games->Title, helperstr);
			}
		}
	}

	SaveList(0);

	printf("All done.!\n");

}
