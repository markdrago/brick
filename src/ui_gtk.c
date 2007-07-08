#include "brick.h"
#include <gtk/gtk.h>
#include <libintl.h>
#include <locale.h>

#ifdef HAVE_HILDON
#include <hildon-widgets/hildon-program.h>
#endif

/* gettext macro */
#define _(string) gettext(string)

static void setup_main_window(GtkContainer *window) {
  GtkWidget *main_hbox;
  GtkListStore *name_list_store;
  GtkTreeIter iter;
  GtkCellRenderer *renderer;
  GtkWidget *view;
  GtkWidget *left_label;
  GtkTreeViewColumn *tree_column;

  main_hbox = gtk_hbox_new(FALSE, 0);
  name_list_store = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
  view = gtk_tree_view_new();

  /* create some fake data */
  gtk_list_store_append(name_list_store, &iter);
  gtk_list_store_set(name_list_store, &iter, 0, 1, 1, "Mark", 2, 10, -1);
  gtk_list_store_append(name_list_store, &iter);
  gtk_list_store_set(name_list_store, &iter, 0, 2, 1, "Byron", 2, 8, -1);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, _("#"),
					      renderer, "text", 0, NULL);
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, _("Name"),
					      renderer, "text", 1, NULL);
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, _("Score"),
					      renderer, "text", 2, NULL);

  gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(name_list_store));
  g_object_unref(name_list_store);

  left_label = gtk_label_new("Test");

  /* pack widgets in to the hbox */
  gtk_box_pack_start(GTK_BOX(main_hbox), left_label, TRUE, TRUE, 2);
  gtk_box_pack_start(GTK_BOX(main_hbox), view, FALSE, FALSE, 2);

  gtk_container_add(GTK_CONTAINER(window), main_hbox);
}

void ui_gtk_init(int argc, char *argv[]) {
#ifdef HAVE_HILDON
  HildonProgram *program;
  HildonWindow *window;
#else
  GtkWindow *window;
#endif

  /* set up gettext */
  setlocale(LC_ALL, "");
  bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
  textdomain(GETTEXT_PACKAGE);

  gtk_init(&argc, &argv);
  g_set_application_name(_("Brick"));

#ifdef HAVE_HILDON
  program = HILDON_PROGRAM(hildon_program_get_instance());
  window = HILDON_WINDOW(hildon_window_new());
  hildon_program_add_window(program, window);
#else
  window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
  /*  gtk_window_set_title(window, _("Brick")); */
  gtk_window_set_default_size(window, 400, 200);
#endif

  setup_main_window(GTK_CONTAINER(window));

  /* make close work */
  g_signal_connect(G_OBJECT(window), "delete_event",
		   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(GTK_WIDGET(window));
}

void ui_gtk_main_loop() {
  gtk_main();
}
