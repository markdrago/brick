#include "brick.h"

gboolean model_init();
gboolean model_add_player(const gchar *name);
gboolean model_get_player_score(gint *score, const guint player_num);
gboolean model_get_player_name(gchar **name, const guint player_num);
guint model_get_number_of_players();
