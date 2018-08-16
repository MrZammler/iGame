#pragma once

void scan_repositories(struct Hook* h, Object* o);
void save_list(struct Hook* h, Object* o);
void save_list_as(struct Hook* h, Object* o);
void export_list(struct Hook* h, Object* o);
void menu_copy(struct Hook* h, Object* o);
void menu_paste_as_new(struct Hook* h, Object* o);
void menu_delete(struct Hook* h, Object* o);
void game_properties_ok(struct Hook* h, Object* o);
void add_non_whd_load_game(struct Hook* h, Object* o);
void filter_change(struct Hook* h, Object* o);
void launch_game(struct Hook* h, Object* o);
void menu_show_hidden(struct Hook* h, Object* o);
void app_start(struct Hook* h, Object* o);
void menu_game_properties(struct Hook* h, Object* o);
void menu_add_non_whdload(struct Hook* h, Object* o);
void game_click(struct Hook* h, Object* o);
void genres_click(struct Hook* h, Object* o);
void non_whdload_ok(struct Hook* h, Object* o);
void repo_stop(struct Hook* h, Object* o);
void repo_add(struct Hook* h, Object* o);
void repo_remove(struct Hook* h, Object* o);
