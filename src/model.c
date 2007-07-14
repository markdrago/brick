#include "brick.h"

struct round {
  gint points_scored;
  gboolean air_5;
  gboolean air_3;
  gboolean air_1;
  gboolean brb_5;
  gboolean brb_3;
};

struct player {
  gchar *name;
  GList *rounds;
};

struct game {
  GList *players;
};

/* global game structure */
static struct game maingame;

/* this function is used by GList to compare elements */
static gint compare_player_names(gconstpointer a, gconstpointer b) {
  struct player *playera, *playerb;

  playera = (struct player*)a;
  playerb = (struct player*)b;

  return strcmp(playera->name, playerb->name);
}

static void free_player(struct player *player_to_free) {
  if (player_to_free != NULL) {
    if (player_to_free->name != NULL) {
      g_free(player_to_free->name);
    }
    g_free(player_to_free);
  }
}

static gboolean add_player_to_game(struct player *player_to_add) {
  if (g_list_find_custom(maingame.players, player_to_add,
			 compare_player_names) != NULL) {
    free_player(player_to_add);
    return FALSE;
  }

  maingame.players = g_list_append(maingame.players, player_to_add);
  return TRUE;
}

static struct player *create_player(const gchar *name) {
  struct player *new_player;

  new_player = g_new(struct player, 1);
  new_player->name = g_strdup(name);

  return new_player;
}

gboolean model_add_player(const gchar *name) {
  struct player *new_player;

  new_player = create_player(name);
  return add_player_to_game(new_player);
}

GList *model_get_player_names() {
  GList *names = NULL;
  GList *players;
  struct player *player1;
  
  players = maingame.players;

  while (players != NULL) {
    player1 = (struct player*)players->data;
    names = g_list_append(names, player1->name);
    players = g_list_next(players);
  }

  return g_list_first(names);
}
