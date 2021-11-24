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
#define MUIA_Dtpic_Name 0x80423d72
#define TEMPLATE "SCREENSHOT/K"
#define PROGDIR "PROGDIR:"
#define DEFAULT_GAMESLIST_FILE "PROGDIR:gameslist"
#define DEFAULT_REPOS_FILE "PROGDIR:repos.prefs"
#define DEFAULT_GENRES_FILE "PROGDIR:genres"
#define DEFAULT_SCREENSHOT_FILE "PROGDIR:igame.iff"
#define DEFAULT_SETTINGS_FILE "PROGDIR:igame.prefs"
#define SLAVE_STRING "slave"
#define ICON_LIBRARY "icon.library"
#define INTUITION_LIBRARY "intuition.library"
#define GRAPHICS_LIBRARY "graphics.library"
#define WB_PUBSCREEN_NAME "Workbench"

#define TOOLTYPE_SCREENSHOT "SCREENSHOT"
#define TOOLTYPE_NOGUIGFX "NOGUIGFX"
#define TOOLTYPE_FILTERUSEENTER "FILTERUSEENTER"
#define TOOLTYPE_NOSCREENSHOT "NOSCREENSHOT"
#define TOOLTYPE_SAVESTATSONEXIT "SAVESTATSONEXIT"
#define TOOLTYPE_TITLESFROMDIRS "TITLESFROMDIRS"
#define TOOLTYPE_NOSMARTSPACES "NOSMARTSPACES"
#define TOOLTYPE_NOSIDEPANEL "NOSIDEPANEL"

#define FILENAME_HOTKEY 'f'
#define QUALITY_HOTKEY 'q'
#define QUALITY_DEFAULT MUIV_Guigfx_Quality_Low
#define SCALEUP_HOTKEY 'u'
#define SCALEUP_DEFAULT FALSE
#define SCALEDOWN_HOTKEY 'd'
#define SCALEDOWN_DEFAULT TRUE
#define TRANSMASK_HOTKEY 'm'
#define TRANSMASK_DEFAULT FALSE
#define TRANSCOLOR_HOTKEY 'c'
#define TRANSCOLOR_DEFAULT FALSE
#define TRANSRGB_HOTKEY 'r'
#define TRANSRGB_DEFAULT (0x0)
#define PICASPECT_DEFAULT TRUE
#define PICASPECT_HOTKEY 'a'
#define SCREENASPECT_DEFAULT TRUE
#define SCREENASPECT_HOTKEY 's'
#define SCALEMODEMASK(u,d,p,s) (((u)?GGSMF_SCALEUP:0)|((d)?GGSMF_SCALEDOWN:0)|((p)?GGSMF_KEEPASPECT_PICTURE:0)|((s)?GGSMF_KEEPASPECT_SCREEN:0))

#define MENU_SCANREPOS_HOTKEY "R"
#define MENU_ADDNONWHDLOADGAME_HOTKEY "A"
#define MENU_OPENLIST_HOTKEY "O"
#define MENU_SAVELIST_HOTKEY "S"
#define MENU_ABOUT_HOTKEY "?"
#define MENU_QUIT_HOTKEY "Q"
#define MENU_PROPERTIES_HOTKEY "P"
#define MENU_DELETE_HOTKEY "D"

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
} igame_settings;

typedef struct genres
{
	char genre[256];
	struct genres* next;
} genres_list;

typedef struct repos
{
	char repo[256];
	struct repos* next;
} repos_list;

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
	int deleted; // indicates this entry should be deleted when the list is saved
	struct games* next;
} games_list;

void scan_repositories();
void open_list();
void save_list(int check_exists);
void save_list_as();
void game_duplicate();
void game_delete();
void game_properties_ok();
void filter_change();
void launch_game();
void list_show_hidden();
void app_start();
void game_properties();
void add_non_whdload();
void genres_click();
void non_whdload_ok();
void repo_stop();
void repo_add();
void repo_remove();
void setting_filter_use_enter_changed();
void setting_save_stats_on_exit_changed();
void setting_smart_spaces_changed();
void setting_titles_from_changed();
void setting_hide_screenshot_changed();
void setting_start_with_favorites_changed();
void setting_no_guigfx_changed();
void setting_screenshot_size_changed();
void settings_save();
void setting_hide_side_panel_changed();
void settings_use();
const unsigned char* GetMBString(const unsigned char* ref);
void joy_left();
void joy_right();
ULONG get_wb_version();

#endif
