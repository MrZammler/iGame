/*
  iGameMain.c
  Misc functions header for iGame

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

#ifndef _FUNCS_H
#define _FUNCS_H

void msg_box(const char *);
void status_show_total(void);
void get_screen_size(int *, int *);
int title_exists(char *);
void game_click(void);
void joystick_input(ULONG);
void app_stop(void);
void save_list(const int);
ULONG get_wb_version(void);
void scan_repositories(void);
void open_list(void);
void save_list_as(void);
void game_duplicate(void);
void game_delete(void);
void game_properties_ok(void);
void filter_change(void);
void launch_game(void);
void list_show_hidden(void);
void app_start(void);
void slaveProperties(void);
void saveItemProperties(void);
void game_properties(void);
void add_non_whdload(void);
void genres_click(void);
void non_whdload_ok(void);
void repo_stop(void);
void repo_add(void);
void repo_remove(void);
void setting_filter_use_enter_changed(void);
void setting_save_stats_on_exit_changed(void);
void setting_smart_spaces_changed(void);
void setting_titles_from_changed(void);
void setting_hide_screenshot_changed(void);
void setting_no_guigfx_changed(void);
void setting_screenshot_size_changed(void);
void settings_save(void);
void setting_hide_side_panel_changed(void);
void setting_start_with_favorites_changed(void);
void settings_use(void);
void add_games_to_listview(void);
igame_settings *load_settings(const char *);

#endif
