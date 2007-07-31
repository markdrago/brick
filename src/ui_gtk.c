/*
 * Brick - A 'Twenty-One' Score Tracker
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
#include "control.h"
#include "model.h"

#include <gtk/gtk.h>
#include <libintl.h>
#include <locale.h>

#ifdef HAVE_HILDON
#include <hildon-widgets/hildon-program.h>
#endif

/* gettext macro */
#define _(string) gettext(string)

/* widgets that are accessed in callbacks */
GtkListStore *name_list_store;
GtkWidget *adder_entry, *current_player;
GtkWidget *score5, *score3, *score1, *score0, *skip;
GtkWidget *air5, *air3, *air1, *brb5, *brb3, *imiss5, *imiss3;

static void update_score_display() {
  GtkTreeIter iter;
  gint i, score, num_players;
  guint current;
  gchar *name, *name_markup;

  /* uncheck the check boxes */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(air5), FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(air3), FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(air1), FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(imiss5), FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(imiss3), FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(brb5), FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(brb3), FALSE);

  /* clear out the existing scores from the display */
  gtk_list_store_clear(name_list_store);

  /* select the current player - put their name on top */
  current = control_get_current_player_num();
  model_get_player_name(&name, current);
  name_markup = g_strdup_printf("<span size=\"large\" weight=\"bold\">%s</span>", name);
  gtk_label_set_markup(GTK_LABEL(current_player), name_markup);
  g_free(name_markup);

  /* repopulate the scores */
  num_players = model_get_number_of_players();
  for (i = 0; i < num_players; i++) {

    /* get name of player */
    if (! model_get_player_name(&name, i)) {
      continue;
    }

    /* get current score for player */
    if (! model_get_player_score(&score, i)) {
      continue;
    }

    gtk_list_store_append(name_list_store, &iter);
    gtk_list_store_set(name_list_store, &iter, 0, (i+1), 1, name, 2, score, -1);
  }
}

static void end_round(GtkWidget *button, gpointer data)
{
  gint score;

  if (button == score5) {
    score = 5;
  } else if (button == score3) {
    score = 3;
  } else if (button == score1) {
    score = 1;
  } else if (button == score0) {
    score = 0;
  } else if (button == skip) {
    score = -1;
  }

  control_end_round(score,
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(air5)),
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(air3)),
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(air1)),
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(imiss5)),
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(imiss3)),
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(brb5)),
		    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(brb3)));

  update_score_display();
}

static void add_player(GtkWidget *button, gpointer data)
{
  const gchar *name;

  /* get the text from the adder entry */
  name = gtk_entry_get_text(GTK_ENTRY(adder_entry));

  /* pass this name on to the controller */
  control_add_player(name);

  /* clear out the adder entry */
  gtk_entry_set_text(GTK_ENTRY(adder_entry), "");

  /* update the list */
  update_score_display();
}

static void get_name_container(GtkWidget **name_container)
{
  GtkCellRenderer *renderer;
  GtkWidget *view;
  GtkTreeViewColumn *tree_column;
  GtkWidget *adder_hbox;
  GtkWidget *adder_button;
  GtkTreeSelection *tree_selection;

  /* create the list store & tree view */
  name_list_store = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
  view = gtk_tree_view_new();

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

  /* add signal to adder button and entry */
  g_signal_connect(adder_button, "clicked", G_CALLBACK(add_player), NULL);
  g_signal_connect(adder_entry, "activate", G_CALLBACK(add_player), NULL);
  
  /* pack entry and button in to the hbox */
  gtk_box_pack_start(GTK_BOX(adder_hbox), adder_entry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(adder_hbox), adder_button, FALSE, FALSE, 0);

  /* create vbox for names and adder */
  *name_container = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(*name_container), view, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(*name_container), adder_hbox, FALSE, FALSE, 0);
}

static void get_control_container(GtkWidget **control_container) {
  GtkWidget *score_box, *airball_box, *brb_box, *imiss_box, *attr_box;
  GtkWidget *score_label, *airball_label, *brb_label, *imiss_label;
  GtkWidget *airball_align, *brb_align, *imiss_align;
  GtkWidget *air_button_box, *brb_button_box, *imiss_button_box;
  GtkWidget *skip_separator, *skip_box;
  gchar *label_text;

  /* set up current player name label */
  current_player = gtk_label_new(NULL);
  gtk_misc_set_alignment(GTK_MISC(current_player), 0, 1.0);
  gtk_widget_set_size_request(current_player, -1, 30);

  /* create labels */
  score_label = gtk_label_new(NULL);
  label_text = g_strdup_printf("<span weight=\"bold\">%s</span>", _("Score"));
  gtk_label_set_markup(GTK_LABEL(score_label), label_text);
  g_free(label_text);

  airball_label = gtk_label_new(NULL);
  label_text = g_strdup_printf("<span weight=\"bold\">%s</span>", _("Airball"));
  gtk_label_set_markup(GTK_LABEL(airball_label), label_text);
  gtk_misc_set_alignment(GTK_MISC(airball_label), 0.1, 1.0);
  g_free(label_text);
		       
  brb_label = gtk_label_new(NULL);
  label_text = g_strdup_printf("<span weight=\"bold\">%s</span>", _("Bad Rebound"));
  gtk_label_set_markup(GTK_LABEL(brb_label), label_text);
  gtk_misc_set_alignment(GTK_MISC(brb_label), 0.1, 1.0);
  g_free(label_text);

  imiss_label = gtk_label_new(NULL);
  label_text = g_strdup_printf("<span weight=\"bold\">%s</span>", _("Planned Miss"));
  gtk_label_set_markup(GTK_LABEL(imiss_label), label_text);
  gtk_misc_set_alignment(GTK_MISC(imiss_label), 0.1, 1.0);

  /* set up airball buttons */
  air5 = gtk_check_button_new_with_label(_("5"));
  air3 = gtk_check_button_new_with_label(_("3"));
  air1 = gtk_check_button_new_with_label(_("1"));
  air_button_box = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(air_button_box), air5, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(air_button_box), air3, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(air_button_box), air1, FALSE, FALSE, 0);

  /* align the airball button box */
  airball_align = gtk_alignment_new(0.1, 0.5, 0, 0);
  gtk_container_add(GTK_CONTAINER(airball_align), air_button_box);

  airball_box = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(airball_box), airball_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(airball_box), airball_align, FALSE, FALSE, 0);

  /* set up bad rebound buttons */
  brb5 = gtk_check_button_new_with_label(_("5"));
  brb3 = gtk_check_button_new_with_label(_("3"));
  brb_button_box = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(brb_button_box), brb5, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(brb_button_box), brb3, FALSE, FALSE, 0);
  
  brb_align = gtk_alignment_new(0.1, 0.5, 0, 0);
  gtk_container_add(GTK_CONTAINER(brb_align), brb_button_box);

  brb_box = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(brb_box), brb_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(brb_box), brb_align, FALSE, FALSE, 0);

  /* set up planned miss buttons */
  imiss5 = gtk_check_button_new_with_label(_("5"));
  imiss3 = gtk_check_button_new_with_label(_("3"));
  imiss_button_box = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(imiss_button_box), imiss5, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(imiss_button_box), imiss3, FALSE, FALSE, 0);
  
  imiss_align = gtk_alignment_new(0.1, 0.5, 0, 0);
  gtk_container_add(GTK_CONTAINER(imiss_align), imiss_button_box);

  imiss_box = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(imiss_box), imiss_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(imiss_box), imiss_align, FALSE, FALSE, 0);

  /* set up scoring buttons */
  score5 = gtk_button_new_with_label(_("5"));
  score3 = gtk_button_new_with_label(_("3"));
  score1 = gtk_button_new_with_label(_("1"));
  score0 = gtk_button_new_with_label(_("None"));
  skip_separator = gtk_vseparator_new();
  skip = gtk_button_new_with_label(_("Skip"));
  skip_box = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(skip_box), skip_separator, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(skip_box), skip, TRUE, TRUE, 6);
  score_box = gtk_hbox_new(TRUE, 6);
  gtk_box_pack_start(GTK_BOX(score_box), score5, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(score_box), score3, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(score_box), score1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(score_box), score0, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(score_box), skip_box, TRUE, TRUE, 0);

  attr_box = gtk_hbox_new(FALSE, 6);
  gtk_box_pack_start(GTK_BOX(attr_box), airball_box, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(attr_box), brb_box, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(attr_box), imiss_box, TRUE, TRUE, 0);

  /* add signal handlers for the scoring buttons */
  g_signal_connect(score5, "clicked", G_CALLBACK(end_round), NULL);
  g_signal_connect(score3, "clicked", G_CALLBACK(end_round), NULL);
  g_signal_connect(score1, "clicked", G_CALLBACK(end_round), NULL);
  g_signal_connect(score0, "clicked", G_CALLBACK(end_round), NULL);
  g_signal_connect(skip, "clicked", G_CALLBACK(end_round), NULL);

  /* pack everything in to a vbox */
  *control_container = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(*control_container), current_player, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(*control_container), attr_box, TRUE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(*control_container), score_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(*control_container), score_box, TRUE, FALSE, 0);
}

static void get_main_window(GtkContainer *window) {
  GtkWidget *main_hbox;
  GtkWidget *control_container;
  GtkWidget *list_container;
  GtkWidget *v_separator;

  main_hbox = gtk_hbox_new(FALSE, 0);

  get_name_container(&list_container);
  get_control_container(&control_container);
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
  gtk_window_set_default_size(window, 400, 200);
#endif

  get_main_window(GTK_CONTAINER(window));

  /* make close work */
  g_signal_connect(G_OBJECT(window), "delete_event",
		   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(GTK_WIDGET(window));
}

void ui_gtk_main_loop() {
  gtk_main();
}
