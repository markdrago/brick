#include "config.h"

#include <gtk/gtk.h>

#ifdef HAVE_HILDON
#include <hildon-widgets/hildon-program.h>
#endif

#include <sqlite3.h>

/* gettext macro */
#define _(string) gettext(string)
