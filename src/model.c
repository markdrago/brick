/*
 * Brick - A '531' Score Tracker
 * Copyright (C) 2007 Mark Drago
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "brick.h"
#include <sqlite3.h>

struct round {
  gint points_scored;
  gboolean air_5;
  gboolean air_3;
  gboolean air_1;
  gboolean imiss_5;
  gboolean imiss_3;
  gboolean brb_5;
  gboolean brb_3;
};

struct player {
  gchar *name;
  gint score;
  gint num_turns;
  GArray *rounds;
};

struct game {
  GArray *players;
};

/* global game structure */
static struct game *maingame;

static void free_player(struct player *player_to_free) {
  if (player_to_free != NULL) {
    if (player_to_free->name != NULL) {
      g_free(player_to_free->name);
    }
    g_free(player_to_free);
  }
}

guint model_get_number_of_players() {
  return maingame->players->len;
}

static gboolean add_player_to_game(struct player *player_to_add) {
  struct player p;
  gint i, num_players;

  num_players = model_get_number_of_players();

  for (i = 0; i < num_players; i++) {
    p = g_array_index(maingame->players, struct player, i);
    if (! strcmp(p.name, player_to_add->name)) {
      return FALSE;
    }
  }

  g_array_append_val(maingame->players, *player_to_add);
  return TRUE;
}

static struct player *create_player(const gchar *name) {
  struct player *new_player;

  new_player = g_new(struct player, 1);
  new_player->name = g_strdup(name);

  new_player->score = 0;
  new_player->rounds = NULL;

  return new_player;
}

gboolean model_set_score(const guint player_num, gint newscore)
{
  struct player *p;

  p = &g_array_index(maingame->players, struct player, player_num);
  p->score = newscore;

  return TRUE;
}

gboolean model_append_round(guint player_num, gint score, gboolean air_5,
			    gboolean air_3, gboolean air_1, gboolean imiss_5,
			    gboolean imiss_3, gboolean brb_5, gboolean brb_3)
{
  struct player *p;

  /*  p = &g_array_index(maingame->players, struct player, player_num);
      p->score += score; */

  return TRUE;
}

gboolean model_add_player(const gchar *name) {
  struct player *new_player;

  new_player = create_player(name);
  return add_player_to_game(new_player);
}

gboolean model_get_player_name(gchar **name, const guint player_num) {
  struct player p;
  
  *name = NULL;
  p = g_array_index(maingame->players, struct player, player_num);

  *name = g_strdup(p.name);
  return TRUE;
}

gboolean model_get_player_score(gint *score, const guint player_num) {
  struct player p;

  *score = 0;
  p = g_array_index(maingame->players, struct player, player_num);

  *score = p.score;
  return TRUE;
}

gboolean model_store() {
  

gboolean model_init() {
  maingame = g_new(struct game, 1);
  maingame->players = g_array_new(TRUE, FALSE, sizeof(struct player));
}
