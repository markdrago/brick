#include "brick.h"

gboolean model_init();
gboolean model_add_player(const gchar *name);
gboolean model_set_score(const guint player_num, gint newscore);
gboolean model_get_player_score(gint *score, const guint player_num);
gboolean model_get_player_name(gchar **name, const guint player_num);
guint model_get_number_of_players();
gboolean model_append_round(guint player_num, gint score, gboolean air_5,
			    gboolean air_3, gboolean air_1, gboolean imiss_5,
			    gboolean imiss_3, gboolean brb_5, gboolean brb_3);
