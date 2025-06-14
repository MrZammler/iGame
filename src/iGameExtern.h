/*
  iGameExtern.h
  Header file for iGame

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

#ifndef IGAME_EXT_H
#define IGAME_EXT_H

#define Dtpic_Classname "Dtpic.mui"
// #define MUIA_Dtpic_Name 0x80423d72
#define PROGDIR "PROGDIR:"
#define DEFAULT_GAMESLIST_FILE "PROGDIR:gameslist"
#define DEFAULT_REPOS_FILE "PROGDIR:repos.prefs"
#define DEFAULT_GENRES_FILE "PROGDIR:genres"
#define DEFAULT_SCREENSHOT_FILE "PROGDIR:igame.iff"
#define DEFAULT_LOGO_FILE "PROGDIR:logo.iff"
#define DEFAULT_SETTINGS_FILE "igame.prefs"
#define DEFAULT_IGAMEDATA_FILE "igame.data"
#define DEFAULT_LOCALHOST_URL "http://localhost"
#define DEFAULT_LEMONAMIGA_URL "https://www.lemonamiga.com/games/details.php?id=%s"
#define DEFAULT_HOL_URL "https://amiga.abime.net/games/view/game/id/%s"
#define DEFAULT_POUET_URL "https://www.pouet.net/prod.php?which=%s"
// #define SLAVE_STRING "slave"
// #define WB_PUBSCREEN_NAME "Workbench"

// #define FILENAME_HOTKEY 'f'
// #define QUALITY_HOTKEY 'q'
// #define QUALITY_DEFAULT MUIV_Guigfx_Quality_Low
// #define SCALEUP_HOTKEY 'u'
// #define SCALEUP_DEFAULT FALSE
// #define SCALEDOWN_HOTKEY 'd'
// #define SCALEDOWN_DEFAULT TRUE
// #define TRANSMASK_HOTKEY 'm'
// #define TRANSMASK_DEFAULT FALSE
// #define TRANSCOLOR_HOTKEY 'c'
// #define TRANSCOLOR_DEFAULT FALSE
// #define TRANSRGB_HOTKEY 'r'
// #define TRANSRGB_DEFAULT (0x0)
// #define PICASPECT_DEFAULT TRUE
// #define PICASPECT_HOTKEY 'a'
// #define SCREENASPECT_DEFAULT TRUE
// #define SCREENASPECT_HOTKEY 's'
// #define SCALEMODEMASK(u,d,p,s) (((u)?GGSMF_SCALEUP:0)|((d)?GGSMF_SCALEDOWN:0)|((p)?GGSMF_KEEPASPECT_PICTURE:0)|((s)?GGSMF_KEEPASPECT_SCREEN:0))

#define MENU_SCANREPOS_HOTKEY "R"
#define MENU_ADDNONWHDLOADGAME_HOTKEY "A"
#define MENU_OPENLIST_HOTKEY "O"
#define MENU_SAVELIST_HOTKEY "S"
#define MENU_ABOUT_HOTKEY "?"
#define MENU_QUIT_HOTKEY "Q"
#define MENU_PROPERTIES_HOTKEY "P"
#define MENU_INFORMATION_HOTKEY "I"
#define MENU_DELETE_HOTKEY "D"

#define MAX_SLAVE_TITLE_SIZE 128
#define MAX_GENRE_NAME_SIZE 32
#define MAX_PATH_SIZE 256
#define MAX_EXEC_SIZE 256
#define MAX_TOOLTYPE_SIZE 64
#define MAX_ARGUMENTS_SIZE 64
#define MIN_TITLE_FILTER_CHARS 3
#define MAX_CHIPSET_SIZE 16

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

typedef struct settings
{
	int filter_use_enter;
	int save_stats_on_exit;
	int no_smart_spaces;
	int titles_from_dirs;
	int hide_screenshots;
	int screenshot_width;
	int screenshot_height;
	int hide_side_panel;
	int no_guigfx;
	int start_with_favorites;
	int useIgameDataTitle;
	int show_url_links;
	int lastScanSetup;  // It keeps info of the settings on the last scan
							// that influence the item data
							// It is a bitfield with the following structure
							//  ------------ no_smart_spaces
							//  |  --------- titles_from_dirs
							//  |  |  ------ useIgameDataTitle
							//  |  |  |
							//  0  0  0
} igame_settings;

typedef struct genresList
{
	char title[MAX_GENRE_NAME_SIZE];
	struct genresList *next;
} genresList;

typedef struct chipsetList
{
	char title[MAX_CHIPSET_SIZE];
	struct chipsetList *next;
} chipsetList;

typedef struct repos
{
	char repo[256];
	struct repos* next;
} repos_list;

typedef struct slavesList
{
	char title[MAX_SLAVE_TITLE_SIZE];
	char user_title[MAX_SLAVE_TITLE_SIZE];
	char path[MAX_PATH_SIZE];
	char genre[MAX_GENRE_NAME_SIZE];
	char arguments[MAX_ARGUMENTS_SIZE];
	char chipset[MAX_CHIPSET_SIZE];

	int instance;
	int times_played;

	int favourite;// TODO: IDEA - This could be a tag

	int last_played; //indicates whether this one was the last game played

	int exists; // indicates whether this game still exists after a scan
				// TODO: IDEA - Maybe needs to be removed when game deletion removes files as well

	int hidden; // game is hidden from normal operation
				// TODO: This could be a tag

	int deleted; // indicates this entry should be deleted when the list is saved

	int year; // Year of release
	int players; // Maximum number of players

	struct slavesList *next;
} slavesList;

typedef struct listFilters
{
	char title[MAX_SLAVE_TITLE_SIZE];
	BOOL showHiddenOnly;
	int showGroup;
	char showGenre[MAX_GENRE_NAME_SIZE];
	char showChipset[MAX_CHIPSET_SIZE];
} listFilters;

enum {
	MENU_ACTIONS=1,
	MENU_SCAN,
	MENU_ADDGAME,
	MENU_SHOWHIDDEN,
	MENU_ABOUT,
	MENU_QUIT,
	MENU_GAME,
	MENU_GAMEPROPERTIES,
	MENU_ITEMINFO,
	MENU_GAMEFOLDER,
	MENU_PREFERENCES,
	MENU_SETTINGS,
	MENU_REPOSITORIES,
	MENU_MUISETTINGS,
	MENU_LAST
};

enum {
	GROUP_SHOWALL,
	GROUP_FAVOURITES,
	GROUP_LAST_PLAYED,
	GROUP_MOST_PLAYED,
	GROUP_NEVER_PLAYED,
	GROUP_LAST
};

#endif
