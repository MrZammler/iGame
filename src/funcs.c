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
#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <workbench/workbench.h>
#include <clib/graphics_protos.h>
#include "iGameGUI.h"

#define MUIA_Dtpic_Name 0x80423d72
#define TEMPLATE 	"SCREENSHOT/K"

extern char *strdup(const char *s);

/* global variables */
int global_filter_check=0;
int TotalHidden=0;
int Showing_hidden=0;
char pathstr[512];
int TotalGames;
int NoOfGenres;
char gamestooltypes[1024];
int SS_WIDTH, SS_HEIGHT;
int NOGUIGFX;
int FILTERUSEENTER;
int NOSCREENSHOT;
int SAVESTATSONEXIT;
int IntroPic = 0;

/* function definitions */
char **my_split (char *str, char *spl);
int GetGenre(char *Title, char *Genre);
void GetPath(char *Title, char *Path);
void followthread(BPTR lock, int tab_level);
void RefreshList(int CheckExists);
void SaveList(int CheckExists);
int hex2dec (char *hexin);
void MsgBox(char *msg);
int GetTitleFromSlave(char *slave, char *title);
int CheckDupTitle(char *title);

/* structures */
struct EasyStruct msgbox;

typedef struct games {
        char    Title[200];
        char    Genre[30];
        int     Index;
        char    Path[256];
	int	Favorite;
	int 	TimesPlayed;
	int	LastPlayed;		//indicates whether this one was the last game played
	int	Exists;			//indicates whether this game still exists after a scan
	int	Hidden;			//game is hidden from normal operation
        struct 	games *next;
} Games_list;

Games_list   *item_games=NULL, *Games=NULL;

typedef struct repos {
        char    Repo[256];
        struct repos *next;
} Repos_list;

Repos_list  *item_repos=NULL, *Repos=NULL;

typedef struct genres {
		char 	Genre[256];
		struct genres *next;
}Genres_list;

Genres_list *item_genres=NULL, *Genres=NULL;

void AppStart()
{
  FILE *fpgames=NULL;
  FILE *fprepos=NULL;
  FILE *fpgenres=NULL;
  char FileLine[500];
  extern struct ObjApp * App;
  char *str=NULL, helperstr[250];
  //char **temp_tbl=NULL;
  APTR pic;
  int i;

  fpgames = fopen ("PROGDIR:gameslist", "r");

  if (fpgames)
    {
      do
	{
	  if (fgets (FileLine, sizeof(FileLine), fpgames)==NULL) { break; }
	  FileLine[strlen(FileLine)-1]='\0';
	  
	  if (strlen(FileLine)==0) continue;
	  
	  item_games = (Games_list *)calloc(1, sizeof(Games_list));
	  item_games->next=NULL;
	  
	  if (!strcmp((char *)FileLine, "index=0")){
	    
	    item_games->Index  = 0;
	    item_games->Exists = 0;
	    
	    do 
	      {
		
		if (fgets (FileLine, sizeof(FileLine), fpgames)==NULL) { break; }
		
		FileLine[strlen(FileLine)-1]='\0';

		if (strlen(FileLine)==0) break;

		//this is to make sure that gameslist goes ok from 1.2 to 1.3
		item_games->Hidden=0;

		if (!strncmp(FileLine, "title=",6))
		  strcpy (item_games->Title, FileLine+6);
		else if (!strncmp(FileLine, "genre=",6))
		  strcpy (item_games->Genre, FileLine+6);
		else if (!strncmp(FileLine, "path=",5))
		  strcpy (item_games->Path, FileLine+5);
		else if (!strncmp(FileLine, "favorite=",9))
		  item_games->Favorite = atoi(FileLine+9);
		else if (!strncmp(FileLine, "timesplayed=",12))
		  item_games->TimesPlayed = atoi(FileLine+12);
		else if (!strncmp(FileLine, "lastplayed=",11))
		  item_games->LastPlayed = atoi(FileLine+11);
		else if (!strncmp(FileLine, "hidden=",7))
		  item_games->Hidden = atoi(FileLine+7);

	      } while (1);

	    if (Games == NULL) {
	      Games = item_games;
	    }
	    else {
	      item_games->next = Games;
	      Games = item_games;
	    }

	  }

  }while(1); //read of gameslist ends here

      //add the games to the list
      TotalGames=0;
      for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

	if (item_games->Hidden!=1){
	  TotalGames++;
	  DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
	}
      }
      sprintf(helperstr, "Total %d games.", TotalGames);
      set(App->TX_Status_0, MUIA_Text_Contents, helperstr);

    }//end of if gamesfp

  fprepos = fopen ("PROGDIR:repos.prefs", "r");
  if (!fprepos){
    //stupid!
  }else{
    do{
      if (fgets (FileLine, sizeof(FileLine), fprepos)==NULL) { break; }

            FileLine[strlen(FileLine)-1]='\0';

            if (strlen(FileLine)==0) continue;

            item_repos = (Repos_list *)calloc(1, sizeof(Repos_list));
            item_repos->next=NULL;
            strcpy(item_repos->Repo, FileLine);

            if (Repos == NULL) {
                Repos = item_repos;
            }
            else {
                item_repos->next = Repos;
                Repos = item_repos;
            }

            DoMethod(App->LV_RepoList_3, MUIM_List_InsertSingle, item_repos->Repo, 1, MUIV_List_Insert_Bottom);

        }while(1);

    }//end of else !fprepos

	DoMethod(App->LV_GenresList_1, MUIM_List_InsertSingle, "--Show All--", MUIV_List_Insert_Bottom);
	DoMethod(App->LV_GenresList_1, MUIM_List_InsertSingle, "--Favorites--", MUIV_List_Insert_Bottom);
	DoMethod(App->LV_GenresList_1, MUIM_List_InsertSingle, "--Last Played--", MUIV_List_Insert_Bottom);
	DoMethod(App->LV_GenresList_1, MUIM_List_InsertSingle, "--Most Played--", MUIV_List_Insert_Bottom);
	DoMethod(App->LV_GenresList_1, MUIM_List_InsertSingle, "--Never Played--", MUIV_List_Insert_Bottom);

	NoOfGenres=0;

	fpgenres = fopen ("PROGDIR:genres", "r");
	if (!fpgenres){
	  //stupid!
	}
	else
	  {
	    do
	      {
		if (fgets(FileLine, sizeof(FileLine), fpgenres)==NULL) { break; }

		FileLine[strlen(FileLine)-1]='\0';

		if (strlen(FileLine)==0) continue;

		item_genres = (Genres_list *)calloc(1, sizeof(Genres_list));
		item_genres->next=NULL;
		strcpy(item_genres->Genre, FileLine);

		if (Genres == NULL) {
		  Genres = item_genres;
		}
		else {
		  item_genres->next = Genres;
		  Genres = item_genres;
		}

		NoOfGenres++;
		
		DoMethod(App->LV_GenresList_1, MUIM_List_InsertSingle, item_genres->Genre, MUIV_List_Insert_Sorted);

	      }while(1);

	   
	    for (i=0;i<NoOfGenres;i++)
	      {
	    	DoMethod(App->LV_GenresList_1, MUIM_List_GetEntry, i+5, &App->CY_Genre_0Content[i]);
	      }
	   
	    App->CY_Genre_0Content[i] = "Unknown";
	    App->CY_Genre_0Content[i+1] = NULL;
	   
	    set (App->CY_Genre_0,MUIA_Cycle_Entries, App->CY_Genre_0Content);
	   
	   }//end of else !fpgenres

	if (fpgames) fclose(fpgames);
	if (fprepos) fclose(fprepos);
	if (fpgenres) fclose(fpgenres);
	IntroPic = 1;

	set(App->WI_Main_0,
		MUIA_Window_Open, TRUE
		);
}

void FilterChange()
{
    extern struct ObjApp * App;
    char *str=NULL, helper[210];
    char *str_gen=NULL, helperstr[200];
    int i, max=0, _strlen;
        
    get(App->STR_Filter_0, MUIA_String_Contents, &str);
    
    DoMethod(App->LV_GenresList_1, MUIM_List_GetEntry ,MUIV_List_GetEntry_Active,&str_gen);
    if (str && strlen(str)!=0) for (i=0;i<=strlen((char *)str)-1;i++) str[i] = tolower(str[i]);
    
    if (str_gen == NULL || !strcmp(str_gen, "--Show All--")) 
      {
	
	// Erase list
	DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
	
	set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
	
	TotalGames=0;
	// Find the entries in Games and update the list
	if (Games)
	  {
	    for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
	      
	      strcpy(helper, item_games->Title);
	      _strlen = strlen(helper);
	      for (i=0;i<=_strlen-1;i++) helper[i] = tolower(helper[i]);
	      
	      if (strstr(helper, (char *)str))
		{
		  if (item_games->Hidden!=1)
		    {
		      DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
		      TotalGames++;
		    }
		}
 	      
	    }
	  }
	
	set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
	
	sprintf(helper, "Total %d games.", TotalGames);
	set(App->TX_Status_0, MUIA_Text_Contents, helper);
      }
    
    else if (!strcmp(str_gen, "--Favorites--"))
      {
      // Erase list
	DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
	set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
	
	TotalGames=0;
	if(Games)
	  {
	    for (item_games=Games; item_games!=NULL;item_games=item_games->next) 
	      {
	      
		strcpy(helper, item_games->Title);
		_strlen = strlen(helper);
		for (i=0;i<=_strlen-1;i++) helper[i] = tolower(helper[i]);
	      
		if (item_games->Favorite==1 && item_games->Hidden!=1 && strstr(helper, (char *)str))
		  {
		    DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
		    TotalGames++;
		  }
	      }
	  }
	
	set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
	
	sprintf(helperstr, "Total %d games.", TotalGames);
	set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
      }
    else if (!strcmp(str_gen, "--Last Played--"))
      {
      
	DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
	set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
	TotalGames=0;
	if(Games)
	  {
	    for (item_games=Games; item_games!=NULL;item_games=item_games->next) 
	      {
		
                strcpy(helper, item_games->Title);
		_strlen = strlen(helper);
		for (i=0;i<=_strlen-1;i++) helper[i] = tolower(helper[i]);
		
		if (item_games->LastPlayed==1 && strstr(helper, (char *)str)){
		  DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
		  TotalGames++;
		}
	      }
	  }
	
	set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
	
	sprintf(helperstr, "Total %d games.", TotalGames);
	set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
	
	
      }
	else if (!strcmp(str_gen, "--Most Played--")){

	  DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
	  set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
	  TotalGames=0;
	  if(Games){

            for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
	      
	      strcpy(helper, item_games->Title);
	      _strlen = strlen(helper);
	      for (i=0;i<=_strlen-1;i++) helper[i] = tolower(helper[i]);
	      
	      if (item_games->TimesPlayed && strstr(helper, (char *)str)){
		
		if (item_games->TimesPlayed >= max && item_games->Hidden!=1) {
		  max=item_games->TimesPlayed;
		  //sprintf(helper, "%ld - %s", item_games->TimesPlayed, item_games->Title);
		  DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Top);
		  TotalGames++;
		  //printf("%s - Top max:%d\n", item_games->Title, max);
		}else{
		  //sprintf(helper, "%ld - %s", item_games->TimesPlayed, item_games->Title);
		  DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Bottom);
		  TotalGames++;
		  //printf("%s - Bottom max:%d\n", item_games->Title, max);
		}
	      }
	    }
	  }
	  set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
	  sprintf(helperstr, "Total %d games.", TotalGames);
	  set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
	}
	else if (!strcmp(str_gen, "--Never Played--")){

	  DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
	  set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
	  TotalGames=0;
	  if(Games){
	    for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

	      strcpy(helper, item_games->Title);
	      _strlen = strlen(helper);
	      for (i=0;i<=_strlen-1;i++) helper[i] = tolower(helper[i]);
	      
	      if (item_games->TimesPlayed==0 && item_games->Hidden!=1 && strstr(helper, (char *)str)){
		DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
		TotalGames++;
	      }
	    }
	  }
	  
	  set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
	  
	  sprintf(helperstr, "Total %d games.", TotalGames);
	  set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
	  
	}
	// the
	else 
	  {

	    // Erase list
	    DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
	    
	    set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
	    //if (strlen(str)>0) for (i=0;i<=strlen((char *)str)-1;i++) str[i] = tolower(str[i]);
	    TotalGames=0;
	    // Find the entries in Games and update the list
	    if (Games)
	      {
		for (item_games=Games; item_games!=NULL;item_games=item_games->next) 
		  {
		    
		    strcpy(helper, item_games->Title);
		    _strlen = strlen(helper);
		    for (i=0;i<=_strlen-1;i++) helper[i] = tolower(helper[i]);
		    
		    if (!strcmp(item_games->Genre,str_gen) && item_games->Hidden!=1 && strstr(helper, (char *)str))
		      {
		      
			DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
			TotalGames++;
			
		      }
		    
		  }
	      }
	    
	    set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
	    
	    sprintf(helper, "Total %d games.", TotalGames);
	    set(App->TX_Status_0, MUIA_Text_Contents, helper);
	    
	  }
   
}

/*
*   Executes whdload with the slave
*/
void GameDoubleClick()
{
  extern struct ObjApp * App;
  //struct FileLock *oldlock, *newlock, *origlock;
  struct Library *IconBase;
  struct  DiskObject      *diskObj;
  char *str=NULL, genre[256], path[256], exec[256], naked_path[256], *tool_type, **tool_types, **temp_tbl=NULL;
  int success,i,k, whdload=0;
  struct FileInfoBlock *m;
  //BPTR newlock, origlock;
  BPTR lock, oldlock, newlock, origlock;
  int exists=0, j,n, z=0, dec_rep=0, _strlen;
  char str2[512], fullpath[800], temptitle[256], slave[256], helperstr[250], to_check[256];

  //clear vars:
  str2[0]='\0';
  fullpath[0]='\0';

  DoMethod(App->LV_GamesList_0, MUIM_List_GetEntry ,MUIV_List_GetEntry_Active,&str);
  GetPath((char *)str, path);

  sprintf(helperstr, "Running %s...", str);
  set(App->TX_Status_0, MUIA_Text_Contents, helperstr);

  /* strip the path from the slave file and get the rest */
  for (i=strlen(path)-1;i>=0;i--){
    if (path[i]=='/')
      break;
  }
  for (k=0;k<=i-1;k++)
    naked_path[k]=path[k];
  naked_path[k]='\0';
  
  _strlen = strlen(path);
  for (k=i+1;k<=_strlen-1;k++) {
    slave[z]=path[k];
    z++;
  }
    
  slave[z]='\0';
  _strlen = strlen(slave);
  for (i=0;i<=_strlen-1;i++) slave[i] = tolower(slave[i]);
  
  if (strstr(slave, ".slave")){
    whdload=1;
  }

    oldlock = Lock("PROGDIR:", ACCESS_READ);

    lock = Lock(naked_path, ACCESS_READ);
    CurrentDir(lock);
    
    if (whdload==1) sprintf(exec, "whdload ");
    else strcpy(exec, slave);

    //tooltypes only for whdload games
    if (whdload==1){

      if (IconBase=(struct Library *)OpenLibrary("icon.library",0))
	{
	  //scan the .info files in the current dir.
	  //one of them should be the game's project icon.

	  /*  allocate space for a FileInfoBlock */
	  m = (struct FileInfoBlock *) AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);
	  
	  success = Examine(lock, m);
	  if (m->fib_DirEntryType <= 0)
	    {
	      /*  We don't allow "opta file", only "opta dir" */
	      return;
	    }
	  
	  while (success = ExNext(lock, m))
	    {
	      if (strstr(m->fib_FileName, ".info") ){
		
		NameFromLock(lock, str2, 512);
		sprintf(fullpath, "%s/%s", str2, m->fib_FileName);
		
		//lose the .info
		for (i=strlen(fullpath)-1;i>=0;i--){
		  if (fullpath[i]=='.')
		    break;
		}
		fullpath[i]='\0';

		if (diskObj=(struct DiskObject *)GetDiskObject(fullpath))
		  {
		    
		    if (MatchToolValue(FindToolType(diskObj->do_ToolTypes, "SLAVE"),slave) || MatchToolValue(FindToolType(diskObj->do_ToolTypes, "slave"),slave) )
		      {
			
		      }
		    else {
		      FreeDiskObject(diskObj);
		      continue;
		    }
		    
		    for (tool_types=(char **)diskObj->do_ToolTypes; tool_type = *tool_types; ++tool_types){
		      
		      if (!strncmp(tool_type, "IM", 2)) continue;
		      if (tool_type[0]==' ') continue;
		      if (tool_type[0]=='(') continue;
		      if (tool_type[0]=='*') continue;
		      if (tool_type[0]==';') continue;
		      if (tool_type[0]=='\0')  continue;

		      /* Must check here for numerical values */
		      /* Those (starting with $ should be transformed to dec from hex) */
		      temp_tbl = my_split((char *)(tool_type), "=");
		      if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp((char *)temp_tbl[0], " ") || !strcmp((char *)temp_tbl[0], ""))
			{
			  continue;
			}
		      
		      if (temp_tbl[1]!=NULL){
			sprintf(to_check, "%s", temp_tbl[1]);
			if(to_check[0]=='$'){
			  dec_rep = hex2dec(to_check);
			  sprintf(tool_type, "%s=%d", temp_tbl[0], dec_rep);
			}
		      }
		      
		      free(temp_tbl);
		      //req: more free's
		      
		      sprintf(exec, "%s %s", exec, tool_type);
		    }
		    
		    FreeDiskObject(diskObj);
		    
		    break;
		  }
	      }
	    }
	  
	  CloseLibrary(IconBase);
	}
      
      //if we're still here, and exec contains just whdload, add the slave and execute
      if (!strcmp(exec, "whdload ")){
	sprintf(exec, "%s %s", exec, slave);
      }
      
      
    }
    
    //set the counters for this game
    for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
      
      if (!strcmp(item_games->Title,str)){
	item_games->LastPlayed=1;
	item_games->TimesPlayed++;
      }
      
      if (item_games->LastPlayed==1 && strcmp(item_games->Title,str)){
	item_games->LastPlayed=0;
      }
      
      
    }
    
    if (SAVESTATSONEXIT==0) SaveList(0);
    success = Execute(exec,0,0);
    
    if (success == 0) MsgBox("Error while executing whdload or game.\nPlease make sure whdload is in your path.");
    
    CurrentDir(oldlock);
    
    sprintf(helperstr, "Total %d games.", TotalGames);
    set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
}

/*
* Scans the repos for games
*/
void MenuScan ()
{
  extern struct ObjApp * App;
  //struct FileLock *oldlock, *currentlock;
  BPTR oldlock, currentlock;
  char repotemp[256], helperstr[256];
  
  if (!Repos)
    {
      //printf("Nothing to scan!\n");
    }
  else
    {
      
      for (item_games=Games; item_games!=NULL;item_games=item_games->next) 
	{
	  //only apply the not exists hack to slaves that are in the current repos, that will be scanned later
	  //Binaries (that are added through add-non-whdload, should be handled afterwards
	  if (strstr(item_games->Path, ".slave") || strlen(item_games->Path)==0)
	    {
	      item_games->Exists=0;
	    }
	  else
	    {
	      item_games->Exists=1;
	    }
	}
      
      currentlock = Lock("PROGDIR:", ACCESS_READ);
      
      for (item_repos=Repos; item_repos!=NULL;item_repos=item_repos->next) 
	{
	  
	  sprintf(repotemp, "%s", item_repos->Repo);
	  sprintf(helperstr, "Scanning [%s]. Please wait...", repotemp);
	  set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
	  oldlock = Lock(repotemp, ACCESS_READ);
	  
	  if(oldlock != 0)
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
      
      SaveList(1);
      
      RefreshList(1);
      
    }
  
  
  
}

void GameClick()
{
    extern struct ObjApp * App;
    char *str=NULL, path[350], naked_path[400];
    int i,k;
    FILE *fp=NULL;
    
    DoMethod(App->LV_GamesList_0, MUIM_List_GetEntry ,MUIV_List_GetEntry_Active,&str);
    
    if (!NOSCREENSHOT && str) //for some reason, GameClick is called and str is null??
      {
	GetPath((char *)str, path);
	
	//printf("Path: %s\n", path);
	
	/* strip the path from the slave file and get the rest */
	for (i=strlen(path)-1;i>=0;i--){
	  if (path[i]=='/')
	    break;
	}
      
	for (k=0;k<=i-1;k++)
	  naked_path[k]=path[k];
	naked_path[k]='\0';

	//Check the string, when filter is populeted there is trouble
	if (strlen(naked_path)!=0)
	  {
	    sprintf(naked_path, "%s/igame.iff", naked_path);
	    
	    //printf("%s\n", naked_path);
	    fp = fopen(naked_path, "r");
	    if (!fp) //no igame.iff, try .info and newicons
	      {
		if (strstr(path, ".slave")) //check for whdload game
		  {
		    path[strlen(path)-6] = '\0';
		    sprintf(naked_path, "%s.info", path);
		    fp = fopen(naked_path, "r");
		  }
	      }

	    if (fp)
	      {
		if (NOGUIGFX)
		  {
		    App->IM_GameImage_1 = MUI_NewObject("Dtpic.mui",
							MUIA_Dtpic_Name, naked_path,
							MUIA_Frame, MUIV_Frame_ImageButton,
							End;
		  }
		else
		  {
		    
		    App->IM_GameImage_1 = GuigfxObject,
		      MUIA_Guigfx_FileName, naked_path,
		      MUIA_Guigfx_Quality, MUIV_Guigfx_Quality_Best,
		      MUIA_Guigfx_ScaleMode, NISMF_SCALEFREE,
		      MUIA_Guigfx_Transparency, 0,
		      MUIA_Frame, MUIV_Frame_ImageButton,
		      MUIA_FixHeight, SS_HEIGHT,
		      MUIA_FixWidth, SS_WIDTH,
		      End;
		  }

		if(App->IM_GameImage_1)
		  {
		    DoMethod(App->GR_grp_1,MUIM_Group_InitChange);
		    DoMethod(App->GR_grp_1,OM_REMMEMBER,App->IM_GameImage_0);
		    DoMethod(App->GR_grp_1,OM_REMMEMBER,App->LV_GenresList_1);
		    MUI_DisposeObject(App->IM_GameImage_0);
		    DoMethod(App->GR_grp_1,OM_ADDMEMBER,App->IM_GameImage_0=App->IM_GameImage_1);
		    DoMethod(App->GR_grp_1,OM_ADDMEMBER,App->LV_GenresList_1);
		    DoMethod(App->GR_grp_1,MUIM_Group_ExitChange);
		    IntroPic = 0;
		  }
	      } 
	    else //no pic found
	      {
		if (IntroPic == 0)
		  {
		  
		    if (NOGUIGFX)
		      {
			App->IM_GameImage_1 = (Object *)MUI_NewObject("Dtpic.mui",
								      MUIA_Dtpic_Name, "PROGDIR:igame.iff",
								      MUIA_Frame, MUIV_Frame_ImageButton,
								      End;
		      }
		    else
		      {
			
			App->IM_GameImage_1 = GuigfxObject,
			  MUIA_Guigfx_FileName, "PROGDIR:igame.iff",
			  MUIA_Guigfx_Quality, MUIV_Guigfx_Quality_Best,
			  MUIA_Guigfx_ScaleMode, NISMF_SCALEFREE,
			  MUIA_Guigfx_Transparency, 0,
			  MUIA_Frame, MUIV_Frame_ImageButton,
			  MUIA_FixHeight, SS_HEIGHT,
			  MUIA_FixWidth, SS_WIDTH,
			  End;
		      }
		    if(App->IM_GameImage_1)
		      {
			DoMethod(App->GR_grp_1,MUIM_Group_InitChange);
			DoMethod(App->GR_grp_1,OM_REMMEMBER,App->IM_GameImage_0);
			DoMethod(App->GR_grp_1,OM_REMMEMBER,App->LV_GenresList_1);
			MUI_DisposeObject(App->IM_GameImage_0);
			DoMethod(App->GR_grp_1,OM_ADDMEMBER,App->IM_GameImage_0=App->IM_GameImage_1);
			DoMethod(App->GR_grp_1,OM_ADDMEMBER,App->LV_GenresList_1);
			DoMethod(App->GR_grp_1,MUIM_Group_ExitChange);
			IntroPic = 1;
		      }
		  }
		
	      }
	  }
	
	if (fp) fclose(fp);
	
      }
}

/* Retrieves the Genre from the file, using the Title */
int GetGenre(char *Title, char *Genre)
{
    for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

        if (!strcmp(Title, item_games->Title)){
            strcpy(Genre, item_games->Genre);
            break;
        }
    }

    return 0;
}

void GetPath(char *Title, char *Path)
{
    for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
        if (!strcmp(Title, item_games->Title)){
            strcpy(Path, item_games->Path);
            break;
        }
    }

}

void RepoStart()
{
    //printf("RepoStart??\n");
}

/*
* Adds a repository (path on the disk)
* to the list of repositories
*/
void RepoAdd()
{
    extern struct ObjApp * App;
    char *str=NULL;

    get(App->PA_label_0, MUIA_String_Contents, &str);

	if (str && strlen((char *)str)!=0){

        item_repos = (Repos_list *)calloc(1, sizeof(Repos_list));
        item_repos->next=NULL;

        strcpy(item_repos->Repo, (char *)str);

        if (Repos == NULL) {
            Repos = item_repos;
        }
        else {
            item_repos->next = Repos;
            Repos = item_repos;
        }

        DoMethod(App->LV_RepoList_3, MUIM_List_InsertSingle, item_repos->Repo, 1, MUIV_List_Insert_Bottom);
    }
}

void RepoRemove()
{
    extern struct ObjApp * App;
    DoMethod(App->LV_RepoList_3, MUIM_List_Remove, MUIV_List_Remove_Active);
}

/*
* Writes the repositories to the repo.prefs file
*/
void RepoStop()
{
  extern struct ObjApp * App;
  int i;
  FILE *fprepos = NULL;

  fprepos = fopen ("PROGDIR:repos.prefs", "w");
  if (!fprepos)
    {
      MsgBox("Could not create repos.prefs file!");
    }
  else
    {
      for (i=0;;i++)
	{
	  char *str=NULL;
	  DoMethod(App->LV_RepoList_3, MUIM_List_GetEntry, i, &str);
	  if (!str) break;
	  //printf("%d - %s\n", i, str);
	  fprintf(fprepos, "%s\n", (char *)str);
	  fflush(fprepos);
	}
      fclose(fprepos);
    }
}

//shows and inits the GameProperties Window
void MenuGameProperties()
{
	extern struct ObjApp * App;
	char *str=NULL;
	char helperstr[512];
	char path[512], genre[100];
	int i, found=0;

	//struct FileLock *lock, *oldlock;
	BPTR lock, oldlock;
    struct Library *IconBase;
	struct  DiskObject      *diskObj;
	char naked_path[256], *tool_type, **tool_types;
	int success,k,open;
    struct FileInfoBlock *m;
	BPTR newlock, origlock;
	int exists=0, j,n, z=0, dec_rep=0;
	char str2[512], fullpath[800], temptitle[256], slave[256], to_check[256];

	//printf("MenuGameProperties\n");

	//if window is already open
	get(App->WI_GameProperties_2, MUIA_Window_Open, &open);
	if (open) return;


	//set the elements on the window
    DoMethod(App->LV_GamesList_0, MUIM_List_GetEntry ,MUIV_List_GetEntry_Active,&str);

    if (str==NULL || strlen(str)==0)
      {
	MsgBox("Please select a game.");
	return;
      }
    
    for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
		if (!strcmp(str, item_games->Title)){
			//strcpy(path, item_games->Path);
			//strcpy(genre, item_games->Genre);
			found=1;
			break;
        }
    }

	if (found==1){
		set(App->STR_Title_1, MUIA_String_Contents, str);
		set(App->TX_Path_2, MUIA_Text_Contents, item_games->Path);

		sprintf(helperstr, "Times Played: %d", item_games->TimesPlayed);
		set(App->TX_NumPlayed_0 , MUIA_Text_Contents, helperstr);

		//set the genre
		for (i=0; i<NoOfGenres;i++)
			if (!strcmp(App->CY_Genre_0Content[i],item_games->Genre)) break;
		set(App->CY_Genre_0, MUIA_Cycle_Active, i);

		if (item_games->Favorite==1)
			set(App->CH_label_0, MUIA_Selected, TRUE);
		else
			set(App->CH_label_0, MUIA_Selected, FALSE);

		if (item_games->Hidden==1)
			set(App->CH_label_1, MUIA_Selected, TRUE);
		else
			set(App->CH_label_1, MUIA_Selected, FALSE);

		//set up the tooltypes

		GetPath((char *)str, path);

		/* strip the path from the slave file and get the rest */
		for (i=strlen(path)-1;i>=0;i--){
			if (path[i]=='/')
				break;
		}
		for (k=0;k<=i-1;k++)
			naked_path[k]=path[k];
		naked_path[k]='\0';

		for (k=i+1;k<=strlen(path)-1;k++) {
			slave[z]=path[k];
			z++;
		}
		slave[z]='\0';

		for (i=0;i<=strlen(slave)-1;i++) slave[i] = tolower(slave[i]);

		//printf("Slave: [%s]\n", slave);

		//printf("Passed : %ld\n", whdload);

		oldlock = Lock("PROGDIR:", ACCESS_READ);

		lock = Lock(naked_path, ACCESS_READ);
		CurrentDir(lock);

		gamestooltypes[0]='\0';

		if (IconBase=(struct Library *)OpenLibrary("icon.library",0))
		{
			//scan the .info files in the current dir.
			//one of them should be the game's project icon.

			/*  allocate space for a FileInfoBlock */
			m = (struct FileInfoBlock *) AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);

			success = Examine(lock, m);
			if (m->fib_DirEntryType <= 0)
			{
				/*  We don't allow "opta file", only "opta dir" */
				return;
			}

			while (success = ExNext(lock, m))
			{
				if (strstr(m->fib_FileName, ".info") ){

					NameFromLock(lock, str2, 512);
					sprintf(fullpath, "%s/%s", str2, m->fib_FileName);
					//printf("%s\n", fullpath);

					//lose the .info
					for (i=strlen(fullpath)-1;i>=0;i--){
						if (fullpath[i]=='.')
							break;
					}
					fullpath[i]='\0';

					//printf("Without info: [%s]\n", fullpath);

					if (diskObj=GetDiskObject(fullpath))
					{
//						  printf("trying: [%s]\n", fullpath);
						if (MatchToolValue(FindToolType(diskObj->do_ToolTypes, "SLAVE"),slave) || MatchToolValue(FindToolType(diskObj->do_ToolTypes, "slave"),slave) )
						{
							//printf("Winner!\n");
						}
						else {
							FreeDiskObject(diskObj);
							continue;
						}

						//printf("Here\n");

						for (tool_types=(char **)diskObj->do_ToolTypes; tool_type = *tool_types; ++tool_types){

							if (!strncmp(tool_type, "IM", 2)) continue;
							//if (tool_type[0]==' ') continue;
							//if (tool_type[0]=='(') continue;
							//if (tool_type[0]=='*') continue;
							//if (tool_type[0]==';') continue;
							//if (tool_type[0]=='\0')  continue;

							sprintf(gamestooltypes, "%s%s\n", gamestooltypes, tool_type);
							set(App->TX_Tooltypes_0, MUIA_TextEditor_ReadOnly, FALSE);
							//printf("[%s]\n", tool_type);




						}
						//printf("%s\n", diskObj->do_ToolTypes);


						FreeDiskObject(diskObj);

						break;
					}
				}
			}

		   CloseLibrary(IconBase);
		}

		if (strlen(gamestooltypes)==0){
			sprintf(gamestooltypes, "No .info file");
			set(App->TX_Tooltypes_0, MUIA_TextEditor_ReadOnly, TRUE);
		}

		set(App->TX_Tooltypes_0, MUIA_TextEditor_Contents, gamestooltypes);
		//set(App->TX_Tooltypes_0, MUIA_Floattext_Text, helperstr);
        CurrentDir(oldlock);

        set(App->WI_GameProperties_2, MUIA_Window_Open, TRUE);

	}else{
		MsgBox("Please select a game.");
	}

}

//when ok is pressed in GameProperties
void GamePropertiesOk()
{
	extern struct ObjApp * App;
	char *str=NULL;
	char *path=NULL, helperstr[256];
	int fav=0, genre=0, hid=0;
	STRPTR	tools;

	//struct FileLock *lock, *oldlock;
	BPTR lock, oldlock;
	struct Library *IconBase;
	struct  DiskObject      *diskObj;
	char **temp_tbl=NULL, naked_path[256], *tool_type, **tool_types, **NewToolTypes, **ptr, **newptr,ModToolType[30];
	//int success,k,open;
	int i,k,success,z;
	struct FileInfoBlock *m;
	BPTR newlock, origlock;
	int NewToolTypeCount=1, OldToolTypeCount=0, NewRealToolTypeCount=1, OldRealToolTypeCount=0;

	char str2[512], fullpath[800], slave[256];

	//printf("GamePropertiesOk\n");

	get(App->STR_Title_1, MUIA_String_Contents, &str);
	get(App->TX_Path_2, MUIA_Text_Contents, &path);
	get(App->CY_Genre_0, MUIA_Cycle_Active, &genre);
	get(App->CH_label_0, MUIA_Selected, &fav);
	get(App->CH_label_1, MUIA_Selected, &hid);

	//printf("%s\n", App->CY_Genre_0Content[genre]);

	//find the entry, and update it:
    for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
		if (!strcmp(path, item_games->Path)){

			if (strcmp(item_games->Title, str)){
				//check dup for title
				if (CheckDupTitle(str)){
					MsgBox("The title you selected, already exists.");
					return;
				}
			}
			strcpy(item_games->Title, str);
			strcpy(item_games->Genre, App->CY_Genre_0Content[genre]);
			if (fav==1) item_games->Favorite=1;
			else item_games->Favorite=0;

			//if it was previously not hidden, hide now
			if (hid==1 && item_games->Hidden!=1){
				item_games->Hidden=1;
				DoMethod(App->LV_GamesList_0, MUIM_List_Remove, MUIV_List_Remove_Selected);
				TotalGames=TotalGames-1;
				sprintf(helperstr, "Total %d games.", TotalGames);
				set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
			}

			if (hid==0 && item_games->Hidden==1){
				item_games->Hidden=0;
                DoMethod(App->LV_GamesList_0, MUIM_List_Remove, MUIV_List_Remove_Selected);
				TotalHidden--;
				sprintf(helperstr, "Total %d games.", TotalHidden);
				set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
			}
			//RefreshList();
			//SaveList();
            break;
        }
    }

	//update the games tooltypes
	tools = (STRPTR)DoMethod(App->TX_Tooltypes_0, MUIM_TextEditor_ExportText);

//	  printf("[%s]\n[%s]\n", tools, gamestooltypes);

	//tooltypes changed
	if (strcmp((char *)tools, gamestooltypes)){

//		  printf("Tools changed\n");

		/* strip the path from the slave file and get the rest */
		for (i=strlen(path)-1;i>=0;i--){
			if (path[i]=='/')
				break;
		}
		for (k=0;k<=i-1;k++)
			naked_path[k]=path[k];
		naked_path[k]='\0';

		z=0;
		for (k=i+1;k<=strlen(path)-1;k++) {
			slave[z]=path[k];
			z++;
		}
		slave[z]='\0';

		for (i=0;i<=strlen(slave)-1;i++) slave[i] = tolower(slave[i]);

//		  printf("Slave: [%s]\n", slave);

		//printf("Passed : %ld\n", whdload);

		oldlock = Lock("PROGDIR:", ACCESS_READ);

		lock = Lock(naked_path, ACCESS_READ);
		CurrentDir(lock);

		if (IconBase=(struct Library *)OpenLibrary("icon.library",0))
		{
			//scan the .info files in the current dir.
			//one of them should be the game's project icon.

			/*  allocate space for a FileInfoBlock */
			m = (struct FileInfoBlock *) AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);

			success = Examine(lock, m);
			if (m->fib_DirEntryType <= 0)
			{
				/*  We don't allow "opta file", only "opta dir" */
				return;
			}

			while (success = ExNext(lock, m))
			{
				if (strstr(m->fib_FileName, ".info") ){

					NameFromLock(lock, str2, 512);
					sprintf(fullpath, "%s/%s", str2, m->fib_FileName);
					//printf("%s\n", fullpath);

					//lose the .info
					for (i=strlen(fullpath)-1;i>=0;i--){
						if (fullpath[i]=='.')
							break;
					}
					fullpath[i]='\0';

					//printf("Without info: [%s]\n", fullpath);

					if (diskObj=(struct DiskObject *)GetDiskObject(fullpath))
					{
//						  printf("trying: [%s]\n", fullpath);
						if (MatchToolValue(FindToolType(diskObj->do_ToolTypes, "SLAVE"),slave) || MatchToolValue(FindToolType(diskObj->do_ToolTypes, "slave"),slave) )
						{
//							  printf("Winner!\n");
						}
						else {
							FreeDiskObject(diskObj);
							continue;
						}

						//check numbers for old and new tooltypes
						for(i=0;i<=strlen(tools);i++){
							if (tools[i]=='\n') NewToolTypeCount++;
						}
						//add one for the last tooltype that doesnt end with \n
						NewToolTypeCount++;

						for(i=0;i<=strlen(gamestooltypes);i++){
							if (gamestooltypes[i]=='\n') OldToolTypeCount++;
						}

//						  printf("Old [%d] New [%d]\n", OldToolTypeCount, NewToolTypeCount);

                        for (tool_types=(char **)diskObj->do_ToolTypes; tool_type = *tool_types; ++tool_types){
							OldRealToolTypeCount++;
						}

//						  printf("Old real count [%d]\n", OldRealToolTypeCount);

						NewToolTypes = AllocVec(NewToolTypeCount*sizeof(char *), MEMF_FAST|MEMF_CLEAR);
						newptr = NewToolTypes;

						temp_tbl = my_split((char *)(tools), "\n");
						if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp((char *)temp_tbl[0], " ") || !strcmp((char *)temp_tbl[0], ""))
						{
							break;
						}

						for (i=0;i<=NewToolTypeCount-2;i++){
//							  printf("[%s]\n", temp_tbl[i]);
							*newptr++ = temp_tbl[i];
						}

						*newptr=NULL;

						diskObj->do_ToolTypes=NewToolTypes;
						PutDiskObject(fullpath, diskObj);

						//for (tool_types=diskObj->do_ToolTypes; tool_type = *tool_types; ++tool_types){

						//	  if (!strncmp(tool_type, "IM", 2)) continue;
							//if (tool_type[0]==' ') continue;
							//if (tool_type[0]=='(') continue;
							//if (tool_type[0]=='*') continue;
							//if (tool_type[0]==';') continue;
							//if (tool_type[0]=='\0')  continue;

							//sprintf(gamestooltypes, "%s%s\n", gamestooltypes, tool_type);
							//set(App->TX_Tooltypes_0, MUIA_TextEditor_ReadOnly, FALSE);
							//printf("[%s]\n", tool_type);

						//	  if(!strcmp(tool_type, "PreLoad")){
						//		  sprintf((char *)tool_type, "PRELOADS");
						//	  }
						//
						//	  printf("[%s]\n", tool_type);
						//}

						//diskObj->do_ToolTypes = tool_types;
						//PutDiskObject(fullpath, diskObj);
						//printf("%s\n", diskObj->do_ToolTypes);


						FreeDiskObject(diskObj);

						break;
					}
				}
			}

		   CloseLibrary(IconBase);
		}


	}
	FreeVec(tools);

	DoMethod(App->LV_GamesList_0,MUIM_List_Redraw,MUIV_List_Redraw_Active );

	set(App->WI_GameProperties_2, MUIA_Window_Open, FALSE);
	//RefreshList();
	SaveList(0);

}

void MenuShowHidden()
{
	extern struct ObjApp * App;
	char helperstr[50];
	//int mini_totals=0;

	if (Showing_hidden == 0){

		set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
		TotalHidden=0;
		DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
		//DoMethod(App->LV_GenresList_1,MUIM_List_Select,1,MUIV_List_Select_Off, NULL);
		set(App->LV_GenresList_1, MUIA_Disabled, TRUE);
		set(App->STR_Filter_0, MUIA_Disabled, TRUE);
		if(Games){

			/* Find the entries in Games and update the list */
			for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

			if (item_games->Hidden==1){
					DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
					TotalHidden++;
				}
			}
		}

		set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);

		sprintf(helperstr, "Total %d games.", TotalHidden);
		set(App->TX_Status_0, MUIA_Text_Contents, helperstr);

		Showing_hidden=1;

	}else{

		set(App->LV_GenresList_1, MUIA_Disabled, FALSE);
		set(App->STR_Filter_0, MUIA_Disabled, FALSE);
		Showing_hidden = 0;
		RefreshList(0);
	}


}



void AppStop()
{
    free(Games);
    free(Repos);

    if (SAVESTATSONEXIT==1) SaveList(0);
    
    /* free some more stuff here! */
    //return 0;

}

void GenresClick()
{
    extern struct ObjApp * App;
	char *str=NULL, helperstr[50];
	int max=0, mini_totals=0;
	//int i,k;
	//FILE *fp;

	DoMethod(App->LV_GenresList_1, MUIM_List_GetEntry ,MUIV_List_GetEntry_Active,&str);

	if (!strcmp(str, "--Show All--"))
		RefreshList(0);
	else if (!strcmp(str, "--Favorites--")){
        /* Erase list */
		DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
		//DoMethod(App->LV_GamesList_0,MUIM_Hide);
		set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
		if(Games){

			/* Find the entries in Games and update the list */
			for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

				if (item_games->Favorite==1 && item_games->Hidden!=1){
					DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
					mini_totals++;
				}
			}
		}

		//DoMethod(App->LV_GamesList_0,MUIM_Show);
		set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
		//DoMethod(App->LV_GamesList_0,MUIM_List_Redraw,MUIV_List_Redraw_All);
		//DoMethod(App->LV_GamesList_0,MUIM_List_Jump,mini_totals);
		//DoMethod(App->LV_GamesList_0,MUIM_List_Jump,0);

		sprintf(helperstr, "Total %d games.", mini_totals);
		set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
		//maybe re-sorting the list isn't needed.
		//DoMethod(App->LV_GamesList_0,MUIM_List_Sort);
	}
	else if (!strcmp(str, "--Last Played--")){

        /* Erase list */
		DoMethod(App->LV_GamesList_0,MUIM_List_Clear);

		if (Games){
			/* Find the entries in Games and update the list */
			for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

				if (item_games->LastPlayed==1){
					DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
					break;
				}
			}
		}
		sprintf(helperstr, "Total 1 games.");
		set(App->TX_Status_0, MUIA_Text_Contents, helperstr);

	}
	else if (!strcmp(str, "--Most Played--")){
		DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
		//DoMethod(App->LV_GamesList_0,MUIM_Hide);
		set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
		if (Games){

			for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

				if (item_games->TimesPlayed){

					if (item_games->TimesPlayed >= max && item_games->Hidden!=1) {
						max=item_games->TimesPlayed;
						//sprintf(helper, "%ld - %s", item_games->TimesPlayed, item_games->Title);
						DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Top);
						mini_totals++;
						//printf("%s - Top max:%d\n", item_games->Title, max);
					}else if (item_games->Hidden!=1){
						//sprintf(helper, "%ld - %s", item_games->TimesPlayed, item_games->Title);
						DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Bottom);
						mini_totals++;
						//printf("%s - Bottom max:%d\n", item_games->Title, max);
					}
				}
			}
		}
		//DoMethod(App->LV_GamesList_0,MUIM_Show);
		set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
		// DoMethod(App->LV_GamesList_0,MUIM_List_Redraw,MUIV_List_Redraw_All);
		//DoMethod(App->LV_GamesList_0,MUIM_List_Jump,mini_totals);
		//DoMethod(App->LV_GamesList_0,MUIM_List_Jump,0);
		sprintf(helperstr, "Total %d games.", mini_totals);
		set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
	}
	else if (!strcmp(str, "--Never Played--")){
		DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
		//DoMethod(App->LV_GamesList_0,MUIM_Hide);
		set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
		if (Games){

			for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

				if (item_games->TimesPlayed == 0){

					DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Top);
					mini_totals++;
				}
			}
		}

		set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
		sprintf(helperstr, "Total %d games.", mini_totals);
		set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
	}
	// the
	else {
        /* Erase list */
		DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
		//DoMethod(App->LV_GamesList_0,MUIM_Hide);
		set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);
		if (Games){
			/* Find the entries in Games and update the list */
			for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

				if (!strcmp(item_games->Genre,str) && item_games->Hidden!=1){
					DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
					mini_totals++;
				}
			}
		}
		//DoMethod(App->LV_GamesList_0,MUIM_Show);
		set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
		// DoMethod(App->LV_GamesList_0,MUIM_List_Redraw,MUIV_List_Redraw_All);
		//DoMethod(App->LV_GamesList_0,MUIM_List_Jump,mini_totals);
		//DoMethod(App->LV_GamesList_0,MUIM_List_Jump,0);
		sprintf(helperstr, "Total %d games.", mini_totals);
		set(App->TX_Status_0, MUIA_Text_Contents, helperstr);
	}



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

void followthread(BPTR lock, int tab_level)

    //BPTR lock;
//	struct FileLock *lock;
    //int tab_level;

    {
    struct FileInfoBlock *m;
    BPTR oldlock,newlock;
    //struct FileLock *newlock, *oldlock;
	int s, success, i, exists=0, j,k,n=0, _kp=0;
	char str[512], fullpath[512], temptitle[256], helperstr[256];

    /*  if at the end of the road, don't print anything */
    if (!lock){
        return;
    }

    //CurrentDir(lock);

    /*  allocate space for a FileInfoBlock */
    m = (struct FileInfoBlock *) AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR);

    success = Examine(lock, m);
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
        if (m->fib_DirEntryType > 0){

        }

		//make m->fib_FileName to lower
	_kp = strlen((char *)m->fib_FileName);
	for(s=0;s<_kp;s++) m->fib_FileName[s] = tolower(m->fib_FileName[s]);
	if (strstr(m->fib_FileName, ".slave"))
	  { // || strstr(m->fib_FileName, ".Slave")){

            NameFromLock(lock, str, 511);
            sprintf(fullpath, "%s/%s", str, m->fib_FileName);
	    
            /* add the slave to the gameslist (if it does not already exist) */
            for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
            	//printf("[%s]\n", item_games->Path);
            	//printf("comparing [%s] - [%s]\n", fullpath, item_games->Path);
                if (!strcmp(item_games->Path, fullpath)){
                    exists = 1;
                    //printf("Exists: [%s]\n", fullpath);
					item_games->Exists=1;
					break;
                }
            }
            if (exists==0){
                item_games = (Games_list *)calloc(1, sizeof(Games_list));
                item_games->next=NULL;

                /* strip the path from the slave file and get the rest */
				//printf("str- [%s]\n", str);
				for (j=strlen(str)-1;j>=0;j--){
					if (str[j]=='/' || str[j]==':')
						break;
				//		  printf("Break. j=%ld\n", j);
					}

				//CHANGE 2007-12-03: init n=0 here
				n=0;
				for (k=j+1;k<=strlen(str)-1;k++)
					temptitle[n++]=str[k];
				temptitle[n]='\0';
				//printf("title: [%s]\n", temptitle);
				//strcpy (item_games->Title, temptitle);
				if(GetTitleFromSlave(fullpath, item_games->Title)){
                    strcpy (item_games->Title, temptitle);
				}

				while(CheckDupTitle(item_games->Title)){
					strcat(item_games->Title, " Alt");
				}

				strcpy (item_games->Genre, "Unknown");
                strcpy (item_games->Path, fullpath);
				item_games->Favorite=0;
				item_games->TimesPlayed=0;
				item_games->LastPlayed=0;
				item_games->Exists=1;
				item_games->Hidden=0;

                if (Games == NULL) {
                    Games = item_games;
                }
                else {
                    item_games->next = Games;
                    Games = item_games;
                }
            }
			exists=0;
        }
        //if (m->fib_DirEntryType > 0)
        //{
        //    printf(" [dir]");
        //}

        /*  If we have a directory, then enter it: */
        if (m->fib_DirEntryType > 0)
        {

            /*  Since it is a directory, get a lock on it: */
            newlock = Lock(m->fib_FileName, ACCESS_READ);

            /*  cd to this directory: */
            oldlock = CurrentDir(newlock);

            /*  recursively follow the thread down to the bottom: */
            followthread(newlock, tab_level+1);

            /*  cd back to where we used to be: */
            CurrentDir(oldlock);

            /*  Unlock the directory we just visited: */
            if (newlock)
                UnLock(newlock);
		}
    }
    FreeMem(m, sizeof(struct FileInfoBlock));
}

void
RefreshList(int CheckExists)
{
    extern struct ObjApp * App;
    char helperstr[512];

    DoMethod(App->LV_GamesList_0,MUIM_List_Clear);
    TotalGames=0;
    set(App->LV_GamesList_0, MUIA_List_Quiet, TRUE);

    if (CheckExists == 0)
      {
	for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
	  
	  if (strlen(item_games->Path)!=0 && item_games->Hidden!=1){
	    TotalGames++;
	    DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
	  }
	}
      }
    else
      {
	for (item_games=Games; item_games!=NULL;item_games=item_games->next) {
	  
	  if (strlen(item_games->Path)!=0 && item_games->Hidden!=1 && item_games->Exists==1){
	    TotalGames++;
	    DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);
	  }
	  else
	    {
	      printf("[%s] [%s] [%d]\n", item_games->Title, item_games->Path, item_games->Exists);
	    }
	}
      }

    //DoMethod(App->LV_GamesList_0,MUIM_List_Redraw,MUIV_List_Redraw_All);
    
    set(App->LV_GamesList_0, MUIA_List_Quiet, FALSE);
    sprintf(helperstr, "Total %d games.", TotalGames);
    set(App->TX_Status_0, MUIA_Text_Contents, helperstr);

}

/*
* Saves the current Games struct to disk
*/
void SaveList(int CheckExists)
{
  extern struct ObjApp * App;
  FILE *fpgames=NULL;
  
  set(App->WI_Wait_0, MUIA_Window_Open, TRUE);
  
  fpgames = fopen ("PROGDIR:gameslist", "w");
  
  if (!fpgames)
    {
      MsgBox("Could not open gameslist file!");
    }
  else
    {
      for (item_games=Games; item_games!=NULL;item_games=item_games->next) {

	//printf("Saving: %s\n", item_games->Title);
	if (CheckExists==1)
	  {
	    if (item_games->Exists==1)
	      {
		fprintf(fpgames, "index=%d\n", item_games->Index);
		fprintf(fpgames, "title=%s\n", item_games->Title);
		fprintf(fpgames, "genre=%s\n", item_games->Genre);
		fprintf(fpgames, "path=%s\n", item_games->Path);
		fprintf(fpgames, "favorite=%d\n", item_games->Favorite);
		fprintf(fpgames, "timesplayed=%d\n", item_games->TimesPlayed);
		fprintf(fpgames, "lastplayed=%d\n", item_games->LastPlayed);
		fprintf(fpgames, "hidden=%d\n\n", item_games->Hidden);
		
		//fflush(fpgames);
	      }
	    else
	      {
		strcpy(item_games->Path, "");
	      }
	  }
	else
	  {
	    fprintf(fpgames, "index=%d\n", item_games->Index);
	    fprintf(fpgames, "title=%s\n", item_games->Title);
	    fprintf(fpgames, "genre=%s\n", item_games->Genre);
	    fprintf(fpgames, "path=%s\n", item_games->Path);
	    fprintf(fpgames, "favorite=%d\n", item_games->Favorite);
	    fprintf(fpgames, "timesplayed=%d\n", item_games->TimesPlayed);
	    fprintf(fpgames, "lastplayed=%d\n", item_games->LastPlayed);
	    fprintf(fpgames, "hidden=%d\n\n", item_games->Hidden);
	    
	    //fflush(fpgames);
	  }
	
      }
      fflush(fpgames);
      fclose(fpgames);
    }
  
  set(App->WI_Wait_0, MUIA_Window_Open, FALSE);
}

//function to return the dec eq of a hex no.
int hex2dec (char *hexin)
{
    int dec;
    //lose the first $ character
    hexin++;
    sscanf(hexin, "%lx", &dec);
    return dec;
}

void MsgBox(char *msg)
{
    msgbox.es_StructSize = sizeof(msgbox);
    msgbox.es_Flags = 0;
    msgbox.es_Title = "iGame";
    msgbox.es_TextFormat = msg;
    msgbox.es_GadgetFormat = "Ok";

    EasyRequest(NULL, &msgbox, NULL);
}
/*
void ReportBox(char *msg)
{
    msgbox.es_StructSize = sizeof(msgbox);
	msgbox.es_Flags = 0;
	msgbox.es_Title = "iGame";
	msgbox.es_TextFormat = msg;
	msgbox.es_GadgetFormat = "Ok";

	EasyRequest(NULL, &msgbox, NULL);
}*/

void get_screen_size (int *width, int *height)
{
  struct Screen *wbscreen;
  struct ViewPort *vp;
  struct DrawInfo *drawinfo;
  ULONG modeID;

  //struct IntuitionBase *IntuitionBase;
  struct Library *IntuitionBase;
  //struct GfxBase *GfxBase;
  struct Library *GfxBase;
  extern struct Library *SysBase;
  
  if (IntuitionBase = (struct Library *)OpenLibrary("intuition.library", 36)) {
    if (GfxBase = (struct Library *)OpenLibrary("graphics.library", 36)) {
      
      if (wbscreen = LockPubScreen("Workbench")) {
	/* Using intuition.library/GetScreenDrawInfo(), we get the pen
	 * array we'll use for the screen clone the easy way. */
	drawinfo = GetScreenDrawInfo(wbscreen);
	
	vp = &(wbscreen->ViewPort);
	/* Use graphics.library/GetVPModeID() to get the ModeID of the
	 * Workbench screen. */
	if ((modeID = GetVPModeID(vp)) != INVALID_ID) {
	  *width = wbscreen->Width; *height = wbscreen->Height;
	  
	} else {
	  *width = -1; *height = -1;
	}
	
	FreeScreenDrawInfo(wbscreen, drawinfo);
	UnlockPubScreen(NULL, wbscreen);
	
      } else {
	*width = -1; *height = -1;
      }
      CloseLibrary(GfxBase);
    }
    CloseLibrary(IntuitionBase);
  }
}
  
void ReadToolTypes()
{
    extern struct ObjApp * App;
    struct FileLock *lock;
    struct Library *IconBase;
    struct  DiskObject      *diskObj;
    char *tool_type, **tool_types, **temp_tbl=NULL, **temp_tbl2=NULL;

    int screen_width, screen_height;

    SS_HEIGHT=-1;
    SS_WIDTH=-1;
    NOGUIGFX=0;
    FILTERUSEENTER=0;
    NOSCREENSHOT=0;
    SAVESTATSONEXIT=0;

    if (IconBase=(struct Library *)OpenLibrary("icon.library",0))
    {
	if (diskObj=(struct DiskObject *)GetDiskObject("PROGDIR:iGame"))
	{
	    if(FindToolType(diskObj->do_ToolTypes, "SCREENSHOT")){

		tool_types=(char **)diskObj->do_ToolTypes;
		tool_type=*tool_types;

		temp_tbl = my_split((char *)(tool_type), "=");
		if (temp_tbl == NULL || temp_tbl[0] == NULL || !strcmp(temp_tbl[0], " ") || !strcmp(temp_tbl[0], ""))
		{
			MsgBox("Bad tooltype!");
			exit(0);
		}

		if (temp_tbl[1]!=NULL){
			temp_tbl2 = my_split((char *)(temp_tbl[1]), "x");
			if (temp_tbl2[0]) SS_WIDTH=atoi((char *)temp_tbl2[0]);
			if (temp_tbl2[1]) SS_HEIGHT=atoi((char *)temp_tbl2[1]);

			free(temp_tbl2[0]);free(temp_tbl2[1]);free(temp_tbl2);
			free(temp_tbl[0]);free(temp_tbl[1]);free(temp_tbl);
		}
	    }

	    if(FindToolType(diskObj->do_ToolTypes, "NOGUIGFX")){
		    NOGUIGFX=1;
	    }

	    if(FindToolType(diskObj->do_ToolTypes, "FILTERUSEENTER")){
		    FILTERUSEENTER=1;
	    }

	    if(FindToolType(diskObj->do_ToolTypes, "NOSCREENSHOT")){
		    NOSCREENSHOT=1;
	    }

	    if(FindToolType(diskObj->do_ToolTypes, "SAVESTATSONEXIT")){
		    SAVESTATSONEXIT=1;
	    }


        }

       CloseLibrary(IconBase);
    }

    //check screen res and adjust image box accordingly
    if (SS_HEIGHT==-1 && SS_WIDTH==-1)
      {
    
	get_screen_size(&screen_width, &screen_height);
    
	//if values are ok from the previous function, and user has not provided his own values, calculate a nice size
	if (screen_width!=-1 && screen_height!=-1)
	  {
	    //for hi res (>1024x768) default values is ok
	    if (screen_width>=800 && screen_height>=600)
	      {
		SS_HEIGHT=128;
		SS_WIDTH=160;
	      }
	    else
	      {
		SS_WIDTH = screen_width / 4.5;
		SS_HEIGHT = screen_height / 4;
	      }
	  }
	else
	  {
	    SS_HEIGHT=128;
	    SS_WIDTH=160;
	  }
      }
}

//void MenuMUISettings()
//{
//	  extern struct ObjApp * App;
//
//	  set(App, MUIM_Application_OpenConfigWindow, 0);
//
//}

void MenuAddNonWhdload()
{
	extern struct ObjApp * App;

	set(App->STR_Title_2, MUIA_String_Contents, NULL);
	set(App->PA_label_1, MUIA_String_Contents, NULL);

	set(App->WI_NonWhdload, MUIA_Window_Open, TRUE);
}

void NonWhdloadOk()
{
	extern struct ObjApp * App;
	char *str, *str_title, helperstr[200];
	//printf("Here!!!\n");

	//for some reason it needs this twice to get the contents...
	get(App->PA_label_1, MUIA_String_Contents, &str);
	get(App->STR_Title_2, MUIA_String_Contents, &str_title);

    get(App->PA_label_1, MUIA_String_Contents, &str);
	get(App->STR_Title_2, MUIA_String_Contents, &str_title);

	//printf("[%s] - [%s]\n", str, str_title);

	if (strlen(str_title)==0) { MsgBox("Please let me know the name of the game..."); return; }
	if (strlen(str)==0) { MsgBox("Please pick the game executable..."); return; }

	//add the game to the list
    item_games = (Games_list *)calloc(1, sizeof(Games_list));
	item_games->next=NULL;

	item_games->Index = 0;

	strcpy (item_games->Title, (char *)str_title);

	strcpy (item_games->Genre, "Unknown");

	strcpy (item_games->Path, (char *)str);
	item_games->Favorite = 0;

	item_games->TimesPlayed = 0;

	item_games->LastPlayed = 0;

	if (Games == NULL) {
		Games = item_games;
	}
	else {
		item_games->next = Games;
		Games = item_games;
	}

    DoMethod(App->LV_GamesList_0, MUIM_List_InsertSingle, item_games->Title, MUIV_List_Insert_Sorted);


	sprintf(helperstr, "Total %d games.", ++TotalGames);
	set(App->TX_Status_0, MUIA_Text_Contents, helperstr);

	SaveList(0);

	set(App->WI_NonWhdload, MUIA_Window_Open, FALSE);
}
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




 
//void ReadToolTypes()
//{
//	  struct  Library *IconBase;
//	  struct  WBArg   *wbArg;
//
//	  struct  ExecBase        *SysBase;
//	  struct  Library         *DOSBase;
//	  struct  Process         *proc;
//	  struct  RDArgs          *rdargs=NULL;
//	  struct  WBStartup       *msg=NULL;
//	  struct  RDArgs          *myRDArgs=NULL;
//	  ULONG           rc=RETURN_FAIL;
//	  LONG            opts[OPT_COUNT];
//
//	  struct  DiskObject      *diskObj;
//	  BPTR            tmplock;

//	  char    **ToolTypes;
//	  char    *TotalString;
//				  ULONG   totalSize=3;

//	  char    *CurrentPos=TotalString;
//					  ULONG   currentLength;

//	  /*
//	   * Started from Workbench so do icon magic...
//	   *
//	   * What we will do here is try all of the tooltypes
//	   * in the icon and keep only those which do not cause
//	   * errors in the RDArgs.
//	   */
//	  if (wbArg=msg->sm_ArgList) if (IconBase=OpenLibrary("icon.library",0))
//	  {
//

//		  /*
//		   * Use project icon if it is there...
//		   */
//		  if (msg->sm_NumArgs > 1) wbArg++;
//
//		  tmplock=CurrentDir(wbArg->wa_Lock);
//		  if (diskObj=GetDiskObject(wbArg->wa_Name))
//		  {
//				if (ToolTypes=diskObj->do_ToolTypes)
//				{
//					while (*ToolTypes)
//					{
//						totalSize+=strlen(*ToolTypes)+1;
//						ToolTypes++;
//				  }
//
//				  if (TotalString=AllocVec(totalSize,MEMF_PUBLIC))
//				  {
//					  ToolTypes=diskObj->do_ToolTypes;
//					  do
//					  {
//						  *CurrentPos='\0';
//						  if (*ToolTypes) strcpy(CurrentPos,*ToolTypes);
//						  currentLength=strlen(CurrentPos);
//						  CurrentPos[currentLength+0]='\n';
//						  CurrentPos[currentLength+1]='\0';
//
//						  if (rdargs) FreeArgs(rdargs);
//						  rdargs=NULL;
//						  memset((char *)opts, 0, sizeof(opts));
//
//						  if (myRDArgs) FreeDosObject(DOS_RDARGS,myRDArgs);
//						  if (myRDArgs=AllocDosObject(DOS_RDARGS,NULL))
//						  {
//							  myRDArgs->RDA_Source.CS_Buffer=TotalString;
//							  myRDArgs->RDA_Source.CS_Length=strlen(TotalString);
//
//							  if (rdargs=ReadArgs(TEMPLATE, opts, myRDArgs))
//							  {
//								  CurrentPos[currentLength]=' ';
//								  CurrentPos+=currentLength+1;
//							  }
//						  }
//					  } while (*ToolTypes++);
//					  FreeVec(TotalString);
//				  }
//			  }
//
//		  }
//		  FreeDiskObject(diskObj);
//	  }

//	  CurrentDir(tmplock);
//	  CloseLibrary(IconBase);


//}


   
