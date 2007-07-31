#include "brick.h"

gboolean control_add_player(const gchar *name);
gboolean control_end_round(gint score, gboolean air_5, gboolean air_3,
			   gboolean air_1, gboolean imiss_5, gboolean imiss_3,
			   gboolean brb_5, gboolean brb_3);
guint control_get_current_player_num();
gboolean control_init();
