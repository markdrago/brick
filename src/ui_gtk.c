#include "brick.h"
#include <gtk/gtk.h>
#include <libintl.h>
#include <locale.h>

#ifdef HAVE_HILDON
#include <hildon-widgets/hildon-program.h>
#endif

/* gettext macro */
#define _(string) gettext(string)

static void ui_gtk_get_name_container(GtkWidget **name_container)
{
  GtkListStore *name_list_store;
  GtkTreeIter iter;
  GtkCellRenderer *renderer;
  GtkWidget *view;
  GtkTreeViewColumn *tree_column;
  GtkWidget *adder_hbox;
  GtkWidget *adder_entry;
  GtkWidget *adder_button;
  GtkTreeSelection *tree_selection;

  /* create the list store & tree view */
  name_list_store = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
  view = gtk_tree_view_new();

  /* create some fake data */
  gtk_list_store_append(name_list_store, &iter);
  gtk_list_store_set(name_list_store, &iter, 0, 1, 1, "Mark", 2, 10, -1);
  gtk_list_store_append(name_list_store, &iter);
  gtk_list_store_set(name_list_store, &iter, 0, 2, 1, "Byron", 2, 8, -1);

  /* tell the tree view how to render the columns */
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, _("#"),
					      renderer, "text", 0, NULL);
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, _("Name"),
					      renderer, "text", 1, NULL);
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, _("Score"),
					      renderer, "text", 2, NULL);

  tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  gtk_tree_selection_set_mode(tree_selection, GTK_SELECTION_NONE);

  /* marry the view to the model */
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(name_list_store));
  g_object_unref(name_list_store);

  /* create hbox for adder that is below the names */
  adder_hbox = gtk_hbox_new(FALSE, 0);

  /* create a text entry and button for adder */
  adder_entry = gtk_entry_new();
  adder_button = gtk_button_new_from_stock(GTK_STOCK_ADD);
  gtk_widget_set_size_request(adder_entry, 100, -1);
  
  /* pack entry and button in to the hbox */
  gtk_box_pack_start(GTK_BOX(adder_hbox), adder_entry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(adder_hbox), adder_button, FALSE, FALSE, 0);

  /* create vbox for names and adder */
  *name_container = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(*name_container), view, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(*name_container), adder_hbox, FALSE, FALSE, 0);
}

static void ui_gtk_get_control_container(GtkWidget **control_container) {
  GtkWidget *current_player, *label2;

  current_player = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(current_player),
		       "<span size=\"large\" weight=\"bold\">Mark</span>");
  gtk_misc_set_alignment(GTK_MISC(current_player), 0.1, 1.0);
  gtk_widget_set_size_request(current_player, -1, 30);

  label2 = gtk_label_new("Label2");
  *control_container = gtk_vbox_new(FALSE, 0);

  gtk_box_pack_start(GTK_BOX(*control_container), current_player, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(*control_container), label2, TRUE, TRUE, 0);
}

static void ui_gtk_get_main_window(GtkContainer *window) {
  GtkWidget *main_hbox;
  GtkWidget *control_container;
  GtkWidget *list_container;
  GtkWidget *v_separator;

  main_hbox = gtk_hbox_new(FALSE, 0);

  ui_gtk_get_name_container(&list_container);
  ui_gtk_get_control_container(&control_container);
  v_separator = gtk_vseparator_new();

  /* pack widgets in to the hbox */
  gtk_box_pack_start(GTK_BOX(main_hbox), control_container, TRUE, TRUE, 2);
  gtk_box_pack_start(GTK_BOX(main_hbox), v_separator, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX(main_hbox), list_container, FALSE, FALSE, 2);

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

  ui_gtk_get_main_window(GTK_CONTAINER(window));

  /* make close work */
  g_signal_connect(G_OBJECT(window), "delete_event",
		   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(GTK_WIDGET(window));
}

void ui_gtk_main_loop() {
  gtk_main();
}
