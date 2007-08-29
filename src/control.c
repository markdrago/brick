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
#include "model.h"

static guint current_player;

static void advance_player()
{
  guint num_players;
  
  num_players = model_get_number_of_players();
  
  if (current_player == (num_players - 1)) {
    current_player = 0;
  } else {
    current_player++;
  }
}

gboolean control_add_player(const gchar *name)
{
  return model_add_player(name);
}

gboolean control_end_round(gint score, gboolean air_5, gboolean air_3,
			   gboolean air_1, gboolean imiss_5, gboolean imiss_3,
			   gboolean brb_5, gboolean brb_3)
{
  gint newscore, current_score;

  /* do not store skipped rounds */
  if (score < 0) {
    advance_player();
    return TRUE;
  }

  model_get_player_score(&current_score, current_player);

  /* reset the score to zero if we should */
  if (score == 0) {
    current_score = 0;
  } else if ((score < 5) && (air_5 == TRUE)) {
    current_score = 0;
  } else if ((score < 3) && (air_3 == TRUE)) {
    current_score = 0;
  }

  newscore = current_score + score;

  if (newscore > 21) {
    newscore -= 21;
  }

  model_set_score(current_player, newscore);
  model_append_round(current_player, score, air_5, air_3, air_1, imiss_5,
		     imiss_3, brb_5, brb_3);

  /* do not advance the player after a 5 point shot */
  if (score < 5) {
    advance_player();
  }
}

guint control_get_current_player_num() {
  return current_player;
}

gboolean control_init()
{
  current_player = 0;
}
